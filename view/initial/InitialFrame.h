#ifndef INITIALFRAME_H
#define INITIALFRAME_H

#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/button.h>
#include <wx/grid.h>
#include <wx/srchctrl.h>

#include "../custom/trimGridCellTextEditor.h"

enum ID_MENU
{
    ID_MENU_NEW,
    ID_MENU_OPEN,
    ID_MENU_SAVEAS,
    ID_MENU_EXIT,

    ID_MENU_HEADER_IMPORT,
    ID_MENU_HEADER_EXPORT,    
    ID_MENU_DATA_ADD,
    ID_MENU_DATA_DELETE,
    ID_MENU_TRAILER_RECALCULATE,

    ID_MENU_DATA_SEARCH_BOX,
    ID_MENU_DATA_SEARCH_FORWARD,
    ID_MENU_DATA_SEARCH_BACKWARD,
};

class InitialFrame final : public wxFrame
{
public:
    InitialFrame();

    wxMenuBar *get_menu_bar();
    wxMenu *get_menu_file();
    wxMenu *get_menu_edit();
    wxMenu *get_menu_search();

    wxPanel *get_panel_top();

    wxGrid *get_grid_header();
    wxButton *get_button_header_import();
    wxButton *get_button_header_export();

    wxGrid *get_grid_data();
    wxButton *get_button_data_add();
    wxButton *get_button_data_delete();
    wxSearchCtrl *get_searchctrl_data_search();
    wxButton *get_button_data_search_forward();
    wxButton *get_button_data_search_backward();

    wxGrid *get_grid_trailer();
    wxButton *get_button_trailer_recalculated();

    wxGrid *get_grid_end();

private:
    wxMenuBar *menu_bar;
    wxMenu *menu_file;
    wxMenu *menu_edit;
    wxMenu *menu_search;

    wxPanel *panel_top;

    wxGrid *grid_header;
    wxButton *button_header_import;
    wxButton *button_header_export;

    wxGrid *grid_data;
    wxButton *button_data_add;
    wxButton *button_data_delete;
    wxSearchCtrl *searchctrl_data_search;
    wxButton *button_data_search_forward;
    wxButton *button_data_search_backward;

    wxGrid *grid_trailer;
    wxButton *button_trailer_recalculate;

    wxGrid *grid_end;
};
#endif //INITIALFRAME_H
