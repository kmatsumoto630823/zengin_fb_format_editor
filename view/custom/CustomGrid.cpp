#include "CustomGrid.h"
#include <wx/richtooltip.h>

// trimGridCellTextEditor
void trimGridCellTextEditor::Create(wxWindow* parent, wxWindowID id, wxEvtHandler* evtHandler)
{
    wxGridCellTextEditor::Create(parent, id, evtHandler);

    Text()->SetCursor(wxCursor(wxCURSOR_IBEAM));

    if(m_label.empty()) return;
    if(m_description.empty()) return;

    Text()->Bind(wxEVT_CHAR_HOOK, [=, this]([[maybe_unused]]wxKeyEvent& event)
    {
        event.DoAllowNextEvent();

        auto pos = Text()->GetInsertionPoint();
        auto key = event.GetKeyCode();
        
        if(pos == Text()->GetLastPosition() && (key == WXK_DOWN || key == WXK_RIGHT))
        {
            if(m_isShowedTip) return;
            m_isShowedTip = true;
            
            wxRichToolTip tip(m_label, m_description);
            tip.SetIcon(wxICON_INFORMATION);
            tip.SetTimeout(0);
            tip.ShowFor(Text());    
        }
    });
    
}

void trimGridCellTextEditor::BeginEdit(int row, int col, wxGrid* grid)
{
    Text()->Hide();

    wxGridCellTextEditor::BeginEdit(row, col, grid);
    
    auto value = grid->GetTable()->GetValue(row, col);
    value.Trim(true);
    value.Trim(false);

    Text()->SetValue(value);
    Text()->SetInsertionPointEnd();
    Text()->SelectAll();
    Text()->SetFocus();

    Text()->Show();

    m_isShowedTip = false;

}

void trimGridCellTextEditor::StartingKey(wxKeyEvent& event)
{
    int ch;
    bool isPrintable;

    ch = event.GetUnicodeKey();

    if (ch != WXK_NONE)
    {
        isPrintable = true;
    }
    else
    {
        ch = event.GetKeyCode();
        isPrintable = ch >= WXK_SPACE && ch < WXK_START;
    }

    switch (ch)
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
            if (Text()->GetValue().empty() && isPrintable && m_char_includes.Contains(static_cast<wxChar>(ch)))
            {
                Text()->WriteText(static_cast<wxChar>(ch));
            }
            break;
    }
}

void trimGridCellTextEditor::SetValidString(const wxString& char_includes)
{
    m_char_includes = char_includes;
    wxTextValidator col_validator(wxFILTER_INCLUDE_CHAR_LIST);
    col_validator.SetCharIncludes(char_includes);
    SetValidator(col_validator);
}

void trimGridCellTextEditor::SetTipString(const wxString& label, const wxString& description)
{
    m_label = label;
    m_description = description;
}


// CustomGrid
CustomGrid::CustomGrid
(
    wxWindow *parent,
    wxWindowID id,
    const wxPoint &pos,
    const wxSize &size,
    long style,
    const wxString &name 
)
: wxGrid(parent, id, pos, size, style, name)
{
    auto font = parent->GetFont();
    font.SetPointSize(12);
    font.SetFamily(wxFONTFAMILY_TELETYPE);

    SetGridLineColour(wxColour(0, 0, 0));
    SetDefaultCellFont(font);
    SetTabBehaviour(wxGrid::Tab_Wrap);
    SetCellHighlightColour(GetSelectionBackground());
    DisableDragRowSize();
    CreateGrid(0, 0, wxGrid::wxGridSelectRows);

    Bind(wxEVT_GRID_CELL_CHANGED, [=, this]([[maybe_unused]]wxGridEvent& event)
    {
        auto table = GetTable();

        auto row = event.GetRow();
        auto col = event.GetCol();

        auto& attr = m_attrs[col];
        auto&& value = table->GetValue(row, col);

        if(value.length() > attr.length)
        {
            wxLogMessage("value.length() > attr.length");
            return;
        }

        value.Trim(true);
        value.Trim(false);

        if(attr.initial_value != nullptr && value.length() == 0 )
        {
            value = attr.initial_value;
        }

             if(attr.pad_info[0] == 'R') value.append(   attr.length - value.length(), attr.pad_info[1]);
        else if(attr.pad_info[0] == 'L') value.insert(0, attr.length - value.length(), attr.pad_info[1]);

        if(value != attr.initial_value && value.find_first_not_of(attr.char_includes) != wxString::npos)
        {
            wxMessageDialog mdialog(this, "不正な値です\r\n編集前の値に戻します", "警告", wxOK | wxICON_WARNING);
            mdialog.ShowModal();
            value = event.GetString();
        }

        table->SetValue(row, col, value);
    });

    Bind(wxEVT_GRID_LABEL_LEFT_CLICK, [=, this]([[maybe_unused]]wxGridEvent& event)
    {

    });  
};

CustomGrid::~CustomGrid()
{

};

void CustomGrid::reset(const FBAttrs& attrs)
{
    m_attrs = attrs;

    if(auto h = GetNumberRows(); h > 0) DeleteRows(0, h);
    if(auto w = GetNumberCols(); w > 0) DeleteCols(0, w);
    
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
        auto col_editor = new trimGridCellTextEditor(attr.length);

        if(attr.order == 0)
        {
            col_attr->SetReadOnly();
        }


        if(attr.char_includes != nullptr)
        {
            col_editor->SetValidString(attr.char_includes);
        }
        if(attr.label != nullptr && attr.description != nullptr)
        {
            col_editor->SetTipString(attr.label, attr.description);
        }
            
        col_attr->SetEditor(col_editor);
        SetColAttr(col, col_attr);    
    }

    AppendRows();

    if(m_attrs.size() > GetNumberCols())
    {
        wxLogMessage("m_attrs.size() > GetNumberCols()");
        return;
    }

    for(auto& attr : m_attrs)
    {
        auto col = attr.order;
        auto row = GetNumberRows() - 1;
        auto value = wxString(attr.length, attr.pad_info[1]);

        if(attr.initial_value != nullptr) value = attr.initial_value;

        auto table = GetTable();
        table->SetValue(row, col, value);
    }

    auto row = GetNumberRows() - 1;
    auto col = GetGridCursorCol();

    ClearSelection();
    GoToCell(row, col);
    SetFocus();        

    AutoSize();    
}

bool CustomGrid::is_edited()
{
    if(GetNumberRows() != 1) return false;

    for(auto& attr : m_attrs)
    {
        auto col = attr.order;
        auto row = GetNumberRows() - 1;
        auto value = GetCellValue(row, col);

        auto initial_value = wxString(attr.length, attr.pad_info[1]);
        if(attr.initial_value != nullptr) initial_value = attr.initial_value;

        if(value != initial_value) return true;
    }

    return false;       
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
            auto table = GetTable();
            table->DeleteRows(i);
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

    ClearSelection();
    selected.Sort([](auto lhs, auto rhs){return rhs - lhs;});

    auto num_selected = selected.size();
    for(decltype(num_selected) i = 0; i < num_selected; ++i)
    {
        auto last = selected[i];
        while(i < selected.size() - 1)
        {
            auto diff = selected[i] - selected[i+1];
            if(diff == 1) i++;
            else break;
        }
        auto first = selected[i];

        auto insert_pos = first;
        auto insert_num = last - first + 1;
            
        for(decltype(insert_num) j = 0; j < insert_num; ++j)
        {
            auto table = GetTable();

            table->InsertRows(insert_pos);

            for(auto& attr : m_attrs)
            {
                auto row = insert_pos;
                auto col = attr.order;
                auto value = wxString(attr.length, attr.pad_info[1]);

                if(attr.initial_value != nullptr) value = attr.initial_value;

                table->SetValue(row, col, value);                    
            }
            
            if(insert_pos != GetNumberRows() - 1)
            SelectRow(insert_pos, true);
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
    for(auto& i : selected) massage += "[" + wxString::Format("%d", i + 1) + "]";      
    massage += "を削除します";

    wxMessageDialog mdialog(this, massage, "確認", wxOK | wxCANCEL);
    if(mdialog.ShowModal() == wxID_CANCEL) return;

    ClearSelection();

    selected.Sort([](auto lhs, auto rhs){ return rhs - lhs; });
    for(auto& pos : selected)
    {
        auto table = GetTable();
        table->DeleteRows(pos);
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
    if(search_value.empty()) return;

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

    auto height = GetNumberRows();
    auto width  = GetNumberCols();

    auto first_row = GetGridCursorRow();
    auto first_col = GetGridCursorCol();
    
    auto first_value = GetCellValue(first_row, first_col);

    auto next = [d = (is_forward ? 1 : -1)](auto& pos)
    {
        pos += d;
    };

    auto current_pos = first_row * width + first_col;
    if(first_value.find(search_value) != wxString::npos)
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
        if(current_pos >= width * height) break;
        if(current_pos <  0             ) break;
            
        auto current_row = current_pos / width;
        auto current_col = current_pos % width;

        auto current_value = GetCellValue(current_row, current_col);
        if(current_value.find(search_value) != wxString::npos)
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
    auto table = GetTable();
    static wxString value_buff1;
    static wxString value_buff2;
    for(decltype(num_cols) col = 0; col < num_cols; ++col)
    {
        value_buff1 = table->GetValue(row1, col);
        value_buff2 = table->GetValue(row2, col);

        table->SetValue(row1, col, value_buff2);
        table->SetValue(row2, col, value_buff1);
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