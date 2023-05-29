#ifndef GRID_DIALOG_H
#define GRID_DIALOG_H

#include "common/FBAttrs.h"
#include "CustomGrid.h"

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class GridDialog : public wxDialog
{
public:
	GridDialog
    (
        const FBAttrs& attrs,
        wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_DIALOG_STYLE,
        const wxString& name = wxDialogNameStr
    );

    virtual int ShowModal() override;
    CustomGrid* get_grid(){ return m_grid; }

private:
    CustomGrid* m_grid;
};

#endif // GRID_DIALOG_H