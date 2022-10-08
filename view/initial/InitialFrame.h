#ifndef INITIAL_FRAME_H
#define INITIAL_FRAME_H

#include "common/FBAttrs.h"
#include "view/custom/CustomGrid.h"

#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/button.h>
#include <wx/srchctrl.h>


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

    void save_editing_value();
    bool is_edited_any();

    //getter
    wxMenuBar* get_menu_bar(){ return menu_bar; };
    wxMenu* get_menu_file(){ return menu_file; };
    wxMenu* get_menu_edit(){ return menu_edit; };
    wxMenu* get_menu_search(){ return menu_search; };
    wxMenu* get_menu_help(){ return menu_help; };

    wxPanel* get_panel_top(){ return panel_top; };

    CustomGrid* get_grid_header(){ return grid_header; };
    wxButton* get_button_header_import(){ return button_header_import; };
    wxButton* get_button_header_export(){ return button_header_export; };
    wxButton* get_button_header_import_from_clipboard(){ return button_header_import_from_clipboard; };

    CustomGrid* get_grid_data(){ return grid_data; };
    wxButton* get_button_data_clear_selected(){ return button_data_clear_selected; };
    wxButton* get_button_data_select_all(){ return button_data_select_all; };

    wxButton* get_button_data_move_up(){ return button_data_move_up; };
    wxButton* get_button_data_move_down(){ return button_data_move_down; };

    wxButton* get_button_data_add(){ return button_data_add; };
    wxButton* get_button_data_delete(){ return button_data_delete; };
    wxButton* get_button_data_add_from_file(){ return button_data_add_from_file; };
    wxButton* get_button_data_export(){ return button_data_export; };
    wxButton* get_button_data_add_from_clipboard(){ return button_data_add_from_clipboard; };
    wxSearchCtrl* get_searchctrl_data_search(){ return searchctrl_data_search; };
    wxButton* get_button_data_search_forward(){ return button_data_search_forward; };
    wxButton* get_button_data_search_backward(){ return button_data_search_backward; };

    CustomGrid* get_grid_trailer(){ return grid_trailer; };
    wxButton* get_button_trailer_recalculated(){ return button_trailer_recalculated; };

    CustomGrid* get_grid_end(){ return grid_end; };

    CustomGrid* get_grid(FBPart part){ return grids[(FBEnumInt)part]; };

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

    CustomGrid* grids[(FBEnumInt)FBPart::ITEM_NUM];
};
#endif // INITIAL_FRAME_H
