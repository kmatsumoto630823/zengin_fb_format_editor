#ifndef _TRIM_GRID_CELL_TEXT_EDITORS_H_
#define _TRIM_GRID_CELL_TEXT_EDITORS_H_

#include "wx/grid.h"

class trimGridCellTextEditor : public wxGridCellTextEditor
{
public:
    trimGridCellTextEditor(size_t maxChars = 0) : wxGridCellTextEditor(maxChars){};
    virtual ~trimGridCellTextEditor(){};
    
    virtual void Create(wxWindow* parent, wxWindowID id, wxEvtHandler* evtHandler) override;
    virtual void BeginEdit(int row, int col, wxGrid* grid) override;
    virtual void StartingKey(wxKeyEvent &event) override;
    
    void SetTipString(const wxString& label, const wxString& description);
    void SetValidString(const wxString& char_includes);

private:
    wxString m_char_includes;
    wxString m_label;
    wxString m_description;
    bool m_isShowedTip;
};

#endif // _TRIM_GRID_CELL_TEXT_EDITORS_H_