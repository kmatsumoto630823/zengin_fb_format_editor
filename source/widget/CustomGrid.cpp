#include "CustomGrid.h"

// CustomGridCellTextEditor
void CustomGridCellTextEditor::BeginEdit(int row, int col, wxGrid* grid)
{
    Text()->Hide();

    wxGridCellTextEditor::BeginEdit(row, col, grid);

    auto table = grid->GetTable();

    auto&& value = table->GetValue(row, col);
    value.Trim(true);
    value.Trim(false);

    Text()->SetValue(value);
    Text()->SetInsertionPointEnd();
    Text()->SelectAll();
    Text()->SetFocus();
    Text()->SetCursor(wxCURSOR_IBEAM);    

    Text()->Show();
}

void CustomGridCellTextEditor::StartingKey(wxKeyEvent& e)
{
    [[maybe_unused]] auto& event = e;

    int ch;
    bool isPrintable;

    ch = event.GetUnicodeKey();

    if(ch != WXK_NONE)
    {
        isPrintable = true;
    }
    else
    {
        ch = event.GetKeyCode();
        isPrintable = ch >= WXK_SPACE && ch < WXK_START;
    }

    switch(ch)
    {
        case WXK_DELETE:
            Text()->Remove(0, 1);
            break;

        case WXK_BACK:
            {
                auto pos = Text()->GetLastPosition();
                Text()->Remove(pos - 1, pos);
            }

            break;

        default:
            if(Text()->GetValue().empty() && isPrintable && m_char_includes.Contains(static_cast<wxChar>(ch)))
            {
                Text()->WriteText(static_cast<wxChar>(ch));
            }

            break;
    }
}

void CustomGridCellTextEditor::SetValidString(const wxString& char_includes)
{
    m_char_includes = char_includes;
    
    wxTextValidator col_validator(wxFILTER_INCLUDE_CHAR_LIST);
    col_validator.SetCharIncludes(char_includes);
    
    SetValidator(col_validator);
}

// CustomGridCellStringRenderer
void CustomGridCellStringRenderer::Draw
(
    wxGrid& grid,
    wxGridCellAttr& attr,
    wxDC& dc,
    const wxRect& rect,
    int row,
    int col,
    bool isSelected

)
{
    auto org_colour = attr.GetBackgroundColour();

    if((row & 1) == 0)
    {
        attr.SetBackgroundColour(m_odd_lines_colour);
    }
    else
    {
        attr.SetBackgroundColour(m_even_lines_colour);
    }

    wxGridCellStringRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);

    attr.SetBackgroundColour(org_colour);
}


// CustomGrid
CustomGrid::CustomGrid
(
    wxWindow* parent,
    wxWindowID id,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name 
)
: wxGrid(parent, id, pos, size, style, name)
{
    auto font = parent->GetFont();
    font.SetPointSize(12);
    font.SetFamily(wxFONTFAMILY_TELETYPE);

    SetGridLineColour({0, 0, 0});
    SetDefaultCellFont(font);
    SetTabBehaviour(wxGrid::Tab_Wrap);
    SetCellHighlightColour(GetSelectionBackground());

    auto renderer = new CustomGridCellStringRenderer({255, 255, 255}, {221, 235, 247});
    SetDefaultRenderer(renderer);

    DisableDragRowSize();
    DisableDragColSize();

    CreateGrid(0, 0, wxGrid::wxGridSelectRows);

    Bind(wxEVT_GRID_CELL_CHANGED, [=, this](wxGridEvent& e)
    {
        [[maybe_unused]] auto& event = e;

        auto table = GetTable();
        auto row = event.GetRow();
        auto col = event.GetCol();
        auto& attr = m_attrs[col];

        auto&& value = table->GetValue(row, col);
        value.Trim(true);
        value.Trim(false);
        attr.pad_value(value);

        if(!attr.check_value(value))
        {
            wxMessageDialog mdialog(this, "不正な値です\r\n編集前の値に戻します", "警告", wxOK | wxICON_WARNING);
            mdialog.ShowModal();
            value = event.GetString();              
        }       

        table->SetValue(row, col, value);
    });

    Bind(wxEVT_GRID_LABEL_LEFT_CLICK, [=, this](wxGridEvent& e)
    {
        [[maybe_unused]] auto& event = e;
    });


    auto col_label_window = GetGridColLabelWindow();
    col_label_window->Bind(wxEVT_MOTION, [=](wxMouseEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        event.Skip();

        auto x = event.GetX();
        auto col = XToCol(x);

        if(m_mouseover_lable_col == col)
        {
            return;
        }

        if(col != wxNOT_FOUND)
        {
            auto descript = m_attrs.at(col).descript;
            col_label_window->SetToolTip(descript);
        }

        m_mouseover_lable_col = col;
    });

};

void CustomGrid::reset(const FBAttrs& attrs)
{
    m_attrs = attrs;

    if(auto h = GetNumberRows(); h > 0){ DeleteRows(0, h); }
    if(auto w = GetNumberCols(); w > 0){ DeleteCols(0, w); }
    
    if(GetNumberRows() != 0 || GetNumberCols() != 0)
    {
        wxLogMessage("GetNumberRows() != 0 || GetNumberCols() != 0");
        return;
    }

    AppendCols(m_attrs.size());

    for(auto& attr : m_attrs)
    {
        auto col = attr.order;

        SetColLabelValue(col, attr.label);

        auto col_attr = new wxGridCellAttr;
        auto col_editor = new CustomGridCellTextEditor(attr.length);

        if(attr.order == 0)
        {
            col_attr->SetReadOnly();
        }

        col_editor->SetValidString(attr.char_includes);
            
        col_attr->SetEditor(col_editor);
        SetColAttr(col, col_attr);    
    }

    AppendRows();

    if(m_attrs.size() > GetNumberCols())
    {
        wxLogMessage("m_attrs.size() > GetNumberCols()");
        return;
    }

    wxString wxstr_buff;
    for(auto& attr : m_attrs)
    {
        auto table = GetTable();
        auto col = attr.order;
        auto row = GetNumberRows() - 1;

        auto& initial_value = wxstr_buff;
        initial_value.clear();
        attr.pad_value(initial_value);

        table->SetValue(row, col, initial_value);
    }

    auto row = GetNumberRows() - 1;
    auto col = GetGridCursorCol();

    ClearSelection();
    GoToCell(row, col);
    SetFocus();        

    AutoSize();    
}

void CustomGrid::remain_selected()
{
    auto selected = GetSelectedRows();
    if(selected.empty())
    {
        wxMessageDialog mdialog(this, "レコードが選択されていません", "情報", wxOK);
        mdialog.ShowModal();
    }

    auto num_rows = GetNumberRows();
    for(auto i = num_rows - 1 ; i >= 0; --i)
    {
        if(!IsInSelection(i, 0))
        {
            DeleteRows(i);
        }
    }
}

void CustomGrid::insert_selected()
{
    if(m_attrs.size() > GetNumberCols())
    {
        wxLogMessage("m_attrs.size() > GetNumberCols()");
        return;
    }

    auto selected = GetSelectedRows();

    if(selected.empty())
    {
        auto bottom = GetNumberRows();
        selected.Add(bottom);
    }
    
    auto num_selected = selected.size();
    selected.Sort([](auto lhs, auto rhs){ return lhs - rhs; });

    for(decltype(num_selected) i = 0; i < num_selected - 1; ++i)
    {
        if(selected[i] + 1 != selected[i + 1])
        {
            wxMessageDialog mdialog(this, "複数ブロックが選択されています", "情報", wxOK);
            mdialog.ShowModal();
            return;
        }
    }

    auto pos_selected = selected[0];
    InsertRows(pos_selected, num_selected);

    ClearSelection();

    wxString wxstr_buff;
    for(decltype(num_selected) i = 0; i < num_selected; ++i)
    {
        for(auto& attr : m_attrs)
        {
            auto table = GetTable();
            auto row = selected[i];
            auto col = attr.order;

            auto& initial_value =  wxstr_buff;
            initial_value.clear();
            attr.pad_value(initial_value);

            table->SetValue(row, col, initial_value);                     
        }

        if(selected[i] != GetNumberRows() - 1)
        {
            SelectRow(selected[i], true);
        }
    }

    auto row = selected[0];
    auto col = GetGridCursorCol();

    GoToCell(row, col);
    SetFocus();
}

void CustomGrid::delete_selected()
{
    auto selected = GetSelectedRows();
    if(selected.empty())
    {
        wxMessageDialog mdialog(this, "レコードが選択されていません", "情報", wxOK);
        mdialog.ShowModal();
        return;
    }

    wxString massage;        
    massage += "レコード";
    for(auto& i : selected)
    { 
        massage += "[" + wxString::Format("%d", i + 1) + "]";      
    }
    massage += "を削除します";

    wxMessageDialog mdialog(this, massage, "確認", wxOK | wxCANCEL);
    if(mdialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    ClearSelection();

    selected.Sort([](auto lhs, auto rhs){ return rhs - lhs; });
    for(auto& pos : selected)
    {
        DeleteRows(pos);
    }
}

void CustomGrid::move_up_selected()
{
    move_selected(false);
}

void CustomGrid::move_down_selected()
{
    move_selected(true);
}


void CustomGrid::search_next_value(const wxString& search_value, bool is_forward)
{
    if(search_value.empty())
    {
        return;
    }

    if(GetNumberCols() <= 0 || GetNumberRows() <= 0)
    {
        wxLogMessage("GetNumberCols() <= 0 || GetNumberRows() <= 0");
        return;
    }

    if(GetGridCursorRow() < 0 || GetGridCursorCol() < 0)
    {
        wxLogMessage("GetGridCursorRow() < 0 || GetGridCursorCol() < 0");
        return;
    }

    auto table = GetTable();

    auto height = GetNumberRows();
    auto width  = GetNumberCols();

    auto first_row = GetGridCursorRow();
    auto first_col = GetGridCursorCol();
    
    auto&& first_value = table->GetValue(first_row, first_col);

    auto next = [d = (is_forward ? 1 : -1)](auto& pos)
    {
        pos += d;
    };

    auto current_pos = first_row * width + first_col;
    if(first_value.find(search_value) != first_value.npos)
    {
        next(current_pos);
    }
    else
    {
        if(is_forward)
        {
            current_pos = 0;
        }
        else
        {
            current_pos = width * height - 1;
        }
        
    }

    while(true)
    {
        if(current_pos >= width * height){ break; }
        if(current_pos <  0             ){ break; }
            
        auto current_row = current_pos / width;
        auto current_col = current_pos % width;

        auto&& current_value = table->GetValue(current_row, current_col);
        if(current_value.find(search_value) != current_value.npos)
        {
            GoToCell(current_row, current_col);
            SetFocus();
            return;
        }

        next(current_pos);
    };

    wxMessageDialog mdialog(this, "見つかりませんでした", "情報", wxOK);
    mdialog.ShowModal();

    GoToCell(first_row, first_col);
    SetFocus();

    return;
}

void CustomGrid::swap_rows(int row1, int row2)
{
    if(row1 >= GetNumberRows())
    {
        wxLogMessage("row1 >= GetNumberRows()");
        return;
    }

    if(row2 >= GetNumberRows())
    {
        wxLogMessage("row2 >= GetNumberRows()");
        return;
    } 


    auto num_cols = GetNumberCols();
    wxString wxstr_buff1;
    wxString wxstr_buff2;

    for(decltype(num_cols) col = 0; col < num_cols; ++col)
    {
        auto table = GetTable();
        
        auto& value1 = wxstr_buff1;
        auto& value2 = wxstr_buff2;

        value1 = table->GetValue(row1, col);
        value2 = table->GetValue(row2, col);

        table->SetValue(row1, col, value2);
        table->SetValue(row2, col, value1);
    }    
}

void CustomGrid::move_selected(bool down)
{
    auto selected = GetSelectedRows();
    if(selected.size() != 1)
    {
        wxLogMessage("selected.size() != 1");
        wxLogMessage("レコードが選択されていないか、複数選択されています");
        return;
    }

    auto prev = selected[0];
    auto next = selected[0] + (down ? 1 : -1);
    if(next < 0 || GetNumberRows() <= next)
    {
        SetFocus();
        return;
    }

    swap_rows(prev, next);

    auto row = next;
    auto col = GetGridCursorCol();

    SelectRow(row);
    GoToCell(row, col);
    SetFocus();     
}