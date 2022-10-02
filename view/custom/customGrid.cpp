#include "customGrid.h"

#include <wx/textctrl.h>
#include <wx/richtooltip.h>
#include <wx/valtext.h>
#include <wx/msgdlg.h>
#include <wx/log.h>

// trimGridCellTextEditor
void trimGridCellTextEditor::Create(wxWindow* parent, wxWindowID id, wxEvtHandler* evtHandler)
{
    wxGridCellTextEditor::Create(parent, id, evtHandler);

    Text()->SetCursor(wxCursor(wxCURSOR_IBEAM));

    if(m_label.empty()) return;
    if(m_description.empty()) return;

    Text()->Bind(wxEVT_CHAR_HOOK, [=, this](wxKeyEvent& event)
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


// customGrid
customGrid::customGrid
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
    auto font_grid_cell = parent->GetFont();
    font_grid_cell.SetPointSize(12);
    font_grid_cell.SetFamily(wxFONTFAMILY_TELETYPE);

    this->SetGridLineColour(wxColour(0, 0, 0));
    this->SetDefaultCellFont(font_grid_cell);
    this->SetTabBehaviour(wxGrid::Tab_Wrap);
    this->SetCellHighlightColour(this->GetSelectionBackground());
    this->DisableDragRowSize();
    this->CreateGrid(0, 0, wxGrid::wxGridSelectRows);

    this->Bind(wxEVT_GRID_CELL_CHANGED, [=, this](wxGridEvent& event)
    {
        auto row = event.GetRow();
        auto col = event.GetCol();

        auto& attr = m_attrs[col];
        auto value = this->GetCellValue(row, col);

        if(attr.length - value.length() < 0)
        {
            wxLogMessage("length - value.length() < 0");
            return;
        }

        value.Trim(true);
        value.Trim(false);

        if(attr.initial_value != nullptr && value.length() == 0 )
        {
            value = attr.initial_value;
        }

        if(attr.pad_info[0] == 'R')      value.append(   attr.length - value.length(), attr.pad_info[1]);
        else if(attr.pad_info[0] == 'L') value.insert(0, attr.length - value.length(), attr.pad_info[1]);

        if(value != attr.initial_value && value.find_first_not_of(attr.char_includes) != wxNOT_FOUND)
        {
            wxMessageDialog mdialog(this, "不正な値です\r\n編集前の値に戻します", "警告", wxOK | wxICON_WARNING);
            mdialog.ShowModal();
            value = event.GetString();
        }

        this->SetCellValue(row, col, value);
    });

    this->Bind(wxEVT_GRID_LABEL_LEFT_CLICK, [=, this](wxGridEvent& event)
    {

    });  
};

customGrid::~customGrid()
{

};

void customGrid::reset(const FBAttrs& attrs)
{
    m_attrs = attrs;

    if(auto h = this->GetNumberRows(); h > 0) this->DeleteRows(0, h);
    if(auto w = this->GetNumberCols(); w > 0) this->DeleteCols(0, w);
    
    if(this->GetNumberRows() != 0 || this->GetNumberCols() != 0)
    {
        wxLogMessage("this->GetNumberRows() != 0 || this->GetNumberCols() != 0");
        return;
    }

    this->AppendCols(m_attrs.size());

    for(auto& attr : m_attrs)
    {
        auto col = attr.order;

        this->SetColLabelValue(col, attr.label);

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
        this->SetColAttr(col, col_attr);    
    }

    this->AppendRows();

    if(m_attrs.size() > this->GetNumberCols())
    {
        wxLogMessage("m_attrs.size() > this->GetNumberCols()");
        return;
    }

    for(auto& attr : m_attrs)
    {
        auto col = attr.order;
        auto row = this->GetNumberRows() - 1;
        auto value = wxString(attr.length, attr.pad_info[1]);

        if(attr.initial_value != nullptr) value = attr.initial_value;

        this->SetCellValue(row, col, value);
    }

    auto row = this->GetNumberRows() - 1;
    auto col = this->GetGridCursorCol();

    this->ClearSelection();
    this->GoToCell(row, col);
    this->SetFocus();        

    this->AutoSize();    
}

bool customGrid::is_edited()
{
    if(this->GetNumberRows() != 1) return false;

    for(auto& attr : m_attrs)
    {
        auto col = attr.order;
        auto row = this->GetNumberRows() - 1;
        auto value = this->GetCellValue(row, col);

        auto initial_value = wxString(attr.length, attr.pad_info[1]);
        if(attr.initial_value != nullptr) initial_value = attr.initial_value;

        if(value != initial_value) return true;
    }

    return false;       
}

void customGrid::insert_selected()
{
    if(m_attrs.size() > this->GetNumberCols())
    {
        wxLogMessage("m_attrs.size() > this->GetNumberCols()");
        return;
    }

    auto selected = this->GetSelectedRows();
    if(selected.empty())
    {
        auto bottom = this->GetNumberRows();
        selected.Add(bottom);
    }

    this->ClearSelection();
    selected.Sort([](auto lhs, auto rhs){return rhs - lhs;});

    for(int i = 0; i < selected.size(); ++i)
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
            
        for(int j = 0; j < insert_num; ++j)
        {
            this->InsertRows(insert_pos);

            for(auto& attr : m_attrs)
            {
                auto row = insert_pos;
                auto col = attr.order;
                auto value = wxString(attr.length, attr.pad_info[1]);

                if(attr.initial_value != nullptr) value = attr.initial_value;

                this->SetCellValue(row, col, value);                    
            }
            
            if(insert_pos != this->GetNumberRows() - 1)
            this->SelectRow(insert_pos, true);
        }
    }

    auto row = selected[0];
    auto col = this->GetGridCursorCol();

    this->GoToCell(row, col);
    this->SetFocus();    
}

void customGrid::delete_selected()
{
    auto selected = this->GetSelectedRows();

    if(selected.empty())
    {
        wxMessageDialog mdialog(this, "レコードが選択されていません", "情報", wxOK);
        mdialog.ShowModal();
        return;
    }

    wxString massage;        
    massage += "レコード";
    for(auto& i : selected) massage += '[' + wxString::Format("%d", i + 1) + ']';      
    massage += "を削除します";

    wxMessageDialog mdialog(this, massage, "確認", wxOK | wxCANCEL);
    if(mdialog.ShowModal() == wxID_CANCEL) return;

    this->ClearSelection();

    selected.Sort([](auto lhs, auto rhs){return rhs - lhs;});
    for(auto& insert_pos : selected)
    {
       this->DeleteRows(insert_pos);
    }
}

void customGrid::search_next_value(const wxString& search_value, bool is_forward)
{
    if(search_value.empty()) return;

    if(this->GetNumberCols() <= 0 || this->GetNumberRows() <= 0)
    {
        wxLogMessage("this->GetNumberCols() <= 0 || this->GetNumberRows() <= 0");
        return;
    }

    if(this->GetGridCursorRow() < 0 || this->GetGridCursorCol() < 0)
    {
        wxLogMessage("this->GetGridCursorRow() < 0 || this->GetGridCursorCol() < 0");
        return;
    }

    auto width = this->GetNumberCols();
    auto height = this->GetNumberRows();

    auto first_row = this->GetGridCursorRow();
    auto first_col = this->GetGridCursorCol();
    
    auto first_value = this->GetCellValue(first_row, first_col);

    auto next = [d = (is_forward ? 1 : -1)](auto& pos)
    {
        pos += d;
    };

    auto current_pos = first_row * width + first_col;
    if(first_value.find(search_value) != wxNOT_FOUND)
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

        auto current_value = this->GetCellValue(current_row, current_col);
        if(current_value.find(search_value) != wxNOT_FOUND)
        {
            this->GoToCell(current_row, current_col);
            this->SetFocus();
            return;
        }

        next(current_pos);
    };

    wxMessageDialog mdialog(this, "見つかりませんでした", "情報", wxOK);
    mdialog.ShowModal();

    this->GoToCell(first_row, first_col);
    this->SetFocus();

    return;
} 