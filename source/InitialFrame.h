#ifndef INITIAL_FRAME_H
#define INITIAL_FRAME_H

#include "common/FBAttrs.h"
#include "widget/CustomGrid.h"

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/srchctrl.h>

#include <array>

enum ID_MENU
{
    ID_MENU_NEW,
    ID_MENU_OPEN,
    ID_MENU_SAVEAS,
    ID_MENU_EXIT,

    ID_MENU_HEADER_IMPORT,
    ID_MENU_HEADER_EXPORT,
    ID_MENU_HEADER_IMPORT_FROM_CLIPBOARD,
    ID_MENU_DATA_SELECT_ALL,
    ID_MENU_DATA_CLEAR_SELECTED,
    ID_MENU_DATA_MOVE_UP,
    ID_MENU_DATA_MOVE_DOWN,       
    ID_MENU_DATA_ADD,
    ID_MENU_DATA_DELETE,
    ID_MENU_DATA_ADD_FROM_FILE,
    ID_MENU_DATA_EXPORT,
    ID_MENU_DATA_ADD_FROM_CLIPBOARD,
    ID_MENU_TRAILER_RECALCULATE,

    ID_MENU_DATA_SEARCH_BOX,
    ID_MENU_DATA_SEARCH_FORWARD,
    ID_MENU_DATA_SEARCH_BACKWARD,

    ID_MENU_HELP_ABOUT,
    ID_MENU_HELP_MANUAL,
    ID_MENU_HELP_README,
};


class InitialFrame : public wxFrame
{
public:
    InitialFrame();

    void force_refresh();
    void reset_grids();
    void save_editing_value();

    //getter
    auto get_menu_bar(){ return menu_bar; };
    auto get_menu_file(){ return menu_file; };
    auto get_menu_edit(){ return menu_edit; };
    auto get_menu_search(){ return menu_search; };
    auto get_menu_help(){ return menu_help; };

    auto get_panel_top(){ return panel_top; };

    auto get_grid_header() { return grid_header; };
    auto get_button_header_import(){ return button_header_import; };
    auto get_button_header_export(){ return button_header_export; };
    auto get_button_header_import_from_clipboard(){ return button_header_import_from_clipboard; };

    auto get_grid_data(){ return grid_data; };
    auto get_button_data_clear_selected(){ return button_data_clear_selected; };
    auto get_button_data_select_all(){ return button_data_select_all; };

    auto get_button_data_move_up(){ return button_data_move_up; };
    auto get_button_data_move_down(){ return button_data_move_down; };

    auto get_button_data_add(){ return button_data_add; };
    auto get_button_data_delete(){ return button_data_delete; };
    auto get_button_data_add_from_file(){ return button_data_add_from_file; };
    auto get_button_data_export(){ return button_data_export; };
    auto get_button_data_add_from_clipboard(){ return button_data_add_from_clipboard; };
    auto get_searchctrl_data_search(){ return searchctrl_data_search; };
    auto get_button_data_search_forward(){ return button_data_search_forward; };
    auto get_button_data_search_backward(){ return button_data_search_backward; };

    auto get_grid_trailer(){ return grid_trailer; };
    auto get_button_trailer_recalculated(){ return button_trailer_recalculated; };

    auto get_grid_end(){ return grid_end; };

    using GridArray = std::array<CustomGrid*, (FBEnumInt)FBPart::ITEM_NUM>;
    auto get_grid_array()
    {
        return GridArray{grid_header, grid_data, grid_trailer, grid_end};
    };

private:
    wxMenuBar* menu_bar;
    wxMenu* menu_file;
    wxMenu* menu_edit;
    wxMenu* menu_search;
    wxMenu* menu_help;

    wxPanel* panel_top;

    CustomGrid* grid_header;
    wxButton* button_header_import;
    wxButton* button_header_export;
    wxButton* button_header_import_from_clipboard;

    CustomGrid* grid_data;
    wxButton* button_data_clear_selected;
    wxButton* button_data_select_all;
    wxButton* button_data_move_up;
    wxButton* button_data_move_down;
    wxButton* button_data_add;
    wxButton* button_data_delete;
    wxButton* button_data_add_from_file;
    wxButton* button_data_export;
    wxButton* button_data_add_from_clipboard;
    wxSearchCtrl* searchctrl_data_search;
    wxButton* button_data_search_forward;
    wxButton* button_data_search_backward;

    CustomGrid* grid_trailer;
    wxButton* button_trailer_recalculated;

    CustomGrid* grid_end;
};
#endif // INITIAL_FRAME_H
