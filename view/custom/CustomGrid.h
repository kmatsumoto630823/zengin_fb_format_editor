#ifndef CUSTOM_GRID_H
#define CUSTOM_GRID_H

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

class CustomGrid : public wxGrid
{
public:
    CustomGrid
    (
        wxWindow* parent,
        wxWindowID id,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxWANTS_CHARS,
        const wxString& name = wxGridNameStr
    );
    virtual ~CustomGrid();

    void reset(const FBAttrs& attrs);
    bool is_edited();

    void insert_selected();
    void delete_selected();
    void remain_selected();
    void move_up_selected();
    void move_down_selected();

    void search_next_value(const wxString& search_value, bool forward);   

private:
    inline void swap_rows(int row1, int row2);
    inline void move_selected(bool down);

    FBAttrs m_attrs;
};

#endif // CUSTOM_GRID_H