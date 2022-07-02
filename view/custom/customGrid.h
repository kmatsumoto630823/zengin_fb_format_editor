#ifndef _CUSTOM_GRID_H_
#define _CUSTOM_GRID_H_

#include "common/FBAttrs.h"
#include <wx/grid.h>

class trimGridCellTextEditor : public wxGridCellTextEditor
{
public:
    trimGridCellTextEditor(size_t maxChars = 0) : wxGridCellTextEditor(maxChars){};
    virtual ~trimGridCellTextEditor(){};
    
    virtual void Create(wxWindow* parent, wxWindowID id, wxEvtHandler* evtHandler) override;
    virtual void BeginEdit(int row, int col, wxGrid* grid) override;
    virtual void StartingKey(wxKeyEvent& event) override;
    
    void SetTipString(const wxString& label, const wxString& description);
    void SetValidString(const wxString& char_includes);

private:
    wxString m_char_includes;
    wxString m_label;
    wxString m_description;
    bool m_isShowedTip;
};

class customGrid : public wxGrid
{
public:
    customGrid
    (
        wxWindow* parent,
        wxWindowID id,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxWANTS_CHARS,
        const wxString& name = wxGridNameStr
    );
    virtual ~customGrid();

    void reset(const FBAttrs& attrs);
    
    bool is_edited();
    void insert_selected();
    void delete_selected();
    void search_next_value(const wxString& search_value, bool forward);   

private:
    FBAttrs m_attrs;
};

#endif // _CUSTOM_GRID_H_