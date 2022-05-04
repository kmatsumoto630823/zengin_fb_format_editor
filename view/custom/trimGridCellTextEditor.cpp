#include "trimGridCellTextEditor.h"
#include "wx/textctrl.h"
#include "wx/richtooltip.h"
#include "wx/valtext.h"


void trimGridCellTextEditor::Create(wxWindow* parent, wxWindowID id, wxEvtHandler* evtHandler)
{
    wxGridCellTextEditor::Create(parent, id, evtHandler);

    Text()->SetCursor(wxCursor(wxCURSOR_IBEAM));

    if(m_label.empty()) return;
    if(m_description.empty()) return;

    Text()->Bind(wxEVT_CHAR_HOOK, [=](wxKeyEvent& event)
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
