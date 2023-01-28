#ifndef CUSTOM_GRID_H
#define CUSTOM_GRID_H

#include "common/FBAttrs.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/grid.h>

class CustomGridCellTextEditor : public wxGridCellTextEditor
{
public:
    CustomGridCellTextEditor(size_t maxChars = 0) : wxGridCellTextEditor(maxChars){};
    
    virtual void Create(wxWindow* parent, wxWindowID id, wxEvtHandler* evtHandler) override;
    virtual void BeginEdit(int row, int col, wxGrid* grid) override;
    virtual void StartingKey(wxKeyEvent& event) override;
    
    void SetTipString(const wxString& label, const wxString& descript);
    void SetValidString(const wxString& char_includes);

private:
    wxString m_char_includes;
    wxString m_label;
    wxString m_descript;
    bool m_isShowedTip;
};

class CustomGridCellStringRenderer : public wxGridCellStringRenderer
{
public:
    CustomGridCellStringRenderer
    (
        const wxColour& odd_lines_colour,
        const wxColour& even_lines_colour
    )
    {
        m_odd_lines_colour = odd_lines_colour;
        m_even_lines_colour = even_lines_colour;
    };

    virtual void Draw
    (
        wxGrid &grid,
        wxGridCellAttr &attr,
        wxDC &dc,
        const wxRect &rect,
        int row,
        int col,
        bool isSelected
    ) override;

private:
    wxColour m_odd_lines_colour;
    wxColour m_even_lines_colour;
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

    void reset(const FBAttrs& attrs);

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