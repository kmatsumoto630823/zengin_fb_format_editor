#include "InitialFrame.h"
#include <wx/artprov.h>

InitialFrame::InitialFrame() : wxFrame(NULL, wxID_ANY, "")
{
    // MENU FILE
    menu_file = new wxMenu;
    menu_file->Append(ID_MENU_NEW, "新規（フォーマット選択）\tCtrl+N")->SetBitmap(wxArtProvider::GetBitmap(wxART_NEW, wxART_MENU));
    menu_file->Append(ID_MENU_OPEN, "FBデータを開く");
    menu_file->Append(ID_MENU_SAVEAS, "名前をつけて保存");
    menu_file->AppendSeparator();
    menu_file->Append(ID_MENU_EXIT, "終了\tCtrl+Q")->SetBitmap(wxArtProvider::GetBitmap(wxART_CLOSE, wxART_MENU));

    // MENU EDIT
    menu_edit = new wxMenu;
    menu_edit->Append(ID_MENU_HEADER_IMPORT, "＠ヘッダ：読込（ファイル）")->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_MENU));
    menu_edit->Append(ID_MENU_HEADER_EXPORT, "＠ヘッダ：出力（ファイル）")->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_MENU));
    menu_edit->Append(ID_MENU_HEADER_IMPORT_FROM_CLIPBOARD, "＠ヘッダ：貼付（クリップボード）")->SetBitmap(wxArtProvider::GetBitmap(wxART_PASTE, wxART_MENU));

    menu_edit->AppendSeparator();
    menu_edit->Append(ID_MENU_DATA_SELECT_ALL, "＠データ：全選択\tCtrl+A");
    menu_edit->Append(ID_MENU_DATA_CLEAR_SELECTED, "＠データ：全解除");
    menu_edit->Append(ID_MENU_DATA_MOVE_UP, "＠データ：レコードを上に移動")->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_UP, wxART_MENU));
    menu_edit->Append(ID_MENU_DATA_MOVE_DOWN, "＠データ：レコードを下に移動")->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_DOWN, wxART_MENU));
    menu_edit->Append(ID_MENU_DATA_ADD, "＠データ：行追\tInsert")->SetBitmap(wxArtProvider::GetBitmap(wxART_PLUS, wxART_MENU));
    menu_edit->Append(ID_MENU_DATA_DELETE,"＠データ：行削\tDelete")->SetBitmap(wxArtProvider::GetBitmap(wxART_MINUS, wxART_MENU));
    menu_edit->Append(ID_MENU_DATA_ADD_FROM_FILE, "＠データ：読込（ファイル）")->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_MENU));
    menu_edit->Append(ID_MENU_DATA_EXPORT, "＠データ：出力（ファイル）")->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_MENU));
    menu_edit->Append(ID_MENU_DATA_ADD_FROM_CLIPBOARD, "＠データ：貼付（クリップボード）\tCtrl+Insert")->SetBitmap(wxArtProvider::GetBitmap(wxART_PASTE, wxART_MENU));
    menu_edit->AppendSeparator();
    menu_edit->Append(ID_MENU_TRAILER_RECALCULATE, "＠トレーラー：再計算\tF5")->SetBitmap(wxArtProvider::GetBitmap(wxART_REFRESH, wxART_MENU));

    // MENU SEARCH
    menu_search = new wxMenu;
    menu_search->Append(ID_MENU_DATA_SEARCH_BOX, "＠データ：検索ボックス\tCtrl+F")->SetBitmap(wxArtProvider::GetBitmap(wxART_FIND, wxART_MENU));
    menu_search->AppendSeparator();
    menu_search->Append(ID_MENU_DATA_SEARCH_FORWARD, "＠データ：次検索\tF3")->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_FORWARD, wxART_MENU));
    menu_search->Append(ID_MENU_DATA_SEARCH_BACKWARD, "＠データ：前検索\tShift+F3")->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_BACK, wxART_MENU));;

    // MENU HELP
    menu_help = new wxMenu;
    menu_help->Append(ID_MENU_HELP_ABOUT, "このアプリについて")->SetBitmap(wxArtProvider::GetBitmap(wxART_INFORMATION, wxART_MENU));
    menu_help->Append(ID_MENU_HELP_MANUAL, "簡易操作マニュアル.txt");
    menu_help->Append(ID_MENU_HELP_README, "最初にお読みください.txt");

    // MENU BAR
    menu_bar = new wxMenuBar;
    menu_bar->Append(menu_file, "ファイル(&F)");
    menu_bar->Append(menu_edit, "編集(&E)");
    menu_bar->Append(menu_search, "検索(&S)");
    menu_bar->Append(menu_help, "ヘルプ(&H)");
    SetMenuBar(menu_bar);

    // TOP PANEL
    panel_top = new wxPanel(this, wxID_ANY);

    //GRID_HEADER
    grid_header = new CustomGrid(panel_top, wxID_ANY);
    button_header_import = new wxButton(panel_top, wxID_ANY, "読込"); button_header_import->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR));
    button_header_export = new wxButton(panel_top, wxID_ANY, "出力"); button_header_export->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_TOOLBAR));
    button_header_import_from_clipboard = new wxButton(panel_top, wxID_ANY, "貼付"); button_header_import_from_clipboard->SetBitmap(wxArtProvider::GetBitmap(wxART_PASTE, wxART_TOOLBAR));

    //GRID_DATA
    grid_data = new CustomGrid(panel_top, wxID_ANY);
    button_data_select_all = new wxButton(panel_top, wxID_ANY, "全選択");
    button_data_clear_selected = new wxButton(panel_top, wxID_ANY, "全解除");
    button_data_move_up = new wxButton(panel_top, wxID_ANY, "上移動"); button_data_move_up->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_UP, wxART_TOOLBAR));
    button_data_move_down = new wxButton(panel_top, wxID_ANY, "下移動"); button_data_move_down->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_DOWN, wxART_TOOLBAR));
    button_data_add = new wxButton(panel_top, wxID_ANY, "行追"); button_data_add->SetBitmap(wxArtProvider::GetBitmap(wxART_PLUS, wxART_TOOLBAR));
    button_data_delete = new wxButton(panel_top, wxID_ANY, "行削"); button_data_delete->SetBitmap(wxArtProvider::GetBitmap(wxART_MINUS, wxART_TOOLBAR));
    button_data_add_from_file = new wxButton(panel_top, wxID_ANY, "読込"); button_data_add_from_file->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR));
    button_data_export = new wxButton(panel_top, wxID_ANY, "出力"); button_data_export->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_TOOLBAR));
    button_data_add_from_clipboard = new wxButton(panel_top, wxID_ANY, "貼付"); button_data_add_from_clipboard->SetBitmap(wxArtProvider::GetBitmap(wxART_PASTE, wxART_TOOLBAR));
    searchctrl_data_search = new wxSearchCtrl(panel_top, wxID_ANY);
    button_data_search_forward = new wxButton(panel_top, wxID_ANY, "次検索"); button_data_search_forward->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_FORWARD, wxART_TOOLBAR));
    button_data_search_backward = new wxButton(panel_top, wxID_ANY, "前検索"); button_data_search_backward->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_BACK, wxART_TOOLBAR));

    //GRID_TRAILER
    grid_trailer = new CustomGrid(panel_top, wxID_ANY);
    button_trailer_recalculated = new wxButton(panel_top, wxID_ANY, "再計算"); button_trailer_recalculated->SetBitmap(wxArtProvider::GetBitmap(wxART_REFRESH, wxART_TOOLBAR));

    //GRID_END
    grid_end = new CustomGrid(panel_top, wxID_ANY);

    //SIZER
    auto sizer_top = new wxBoxSizer(wxVERTICAL);
    panel_top->SetSizer(sizer_top);
        sizer_top->AddSpacer(10);

        auto sizer_header_label = new wxBoxSizer(wxHORIZONTAL);
        sizer_top->Add(sizer_header_label, 0, wxALIGN_LEFT);
            sizer_header_label->Add(new wxStaticText(panel_top, wxID_ANY, "＠ヘッダレコード：　"), 0, wxALIGN_BOTTOM);
            sizer_header_label->Add(button_header_import, 0, wxALIGN_BOTTOM);
            sizer_header_label->Add(button_header_export, 0, wxALIGN_BOTTOM);
            sizer_header_label->Add(button_header_import_from_clipboard, 0, wxALIGN_BOTTOM);
        sizer_top->Add(grid_header, 0, wxALIGN_LEFT | wxLEFT, 1);
        sizer_top->AddSpacer(10);

        auto sizer_data_label = new wxBoxSizer(wxHORIZONTAL);
        sizer_top->Add(sizer_data_label, 0, wxALIGN_LEFT);
            sizer_data_label->Add(new wxStaticText(panel_top, wxID_ANY, "＠データレコード：　"), 0, wxALIGN_BOTTOM);
            sizer_data_label->Add(button_data_select_all, 0, wxALIGN_BOTTOM);
            sizer_data_label->Add(button_data_clear_selected, 0, wxALIGN_BOTTOM | wxRIGHT, 20);
            sizer_data_label->Add(button_data_move_up, 0, wxALIGN_BOTTOM);
            sizer_data_label->Add(button_data_move_down, 0, wxALIGN_BOTTOM | wxRIGHT, 20);
            sizer_data_label->Add(button_data_add, 0, wxALIGN_BOTTOM);
            sizer_data_label->Add(button_data_delete, 0, wxALIGN_BOTTOM | wxRIGHT, 20);
            sizer_data_label->Add(button_data_add_from_file, 0, wxALIGN_BOTTOM);
            sizer_data_label->Add(button_data_export, 0, wxALIGN_BOTTOM);
            sizer_data_label->Add(button_data_add_from_clipboard, 0, wxALIGN_BOTTOM | wxRIGHT, 50);
            sizer_data_label->Add(searchctrl_data_search, 0, wxALIGN_BOTTOM);
            sizer_data_label->Add(button_data_search_forward, 0, wxALIGN_BOTTOM);
            sizer_data_label->Add(button_data_search_backward, 0, wxALIGN_BOTTOM | wxRIGHT, 50);
        sizer_top->Add(grid_data, 1, wxALIGN_LEFT | wxEXPAND | wxLEFT, 1);
        sizer_top->AddSpacer(10);

        auto sizer_trailer_label = new wxBoxSizer(wxHORIZONTAL);
        sizer_top->Add(sizer_trailer_label, 0, wxALIGN_LEFT);
            sizer_trailer_label->Add(new wxStaticText(panel_top, wxID_ANY, "＠トレーラレコード：　"), 0, wxALIGN_BOTTOM);
            sizer_trailer_label->Add(button_trailer_recalculated, 0, wxALIGN_BOTTOM);
        sizer_top->Add(grid_trailer, 0, wxALIGN_LEFT | wxLEFT, 1);
        sizer_top->AddSpacer(10);

        auto sizer_end_label = new wxBoxSizer(wxHORIZONTAL);
        sizer_top->Add(sizer_end_label, 0, wxALIGN_LEFT);
            sizer_end_label->Add(new wxStaticText(panel_top, wxID_ANY, "＠エンドレコード：　"), 0, wxALIGN_BOTTOM);
        sizer_top->Add(grid_end, 0, wxALIGN_LEFT | wxLEFT, 1);
        sizer_top->AddSpacer(10);

    // STATUS
    CreateStatusBar();
}

void InitialFrame::force_refresh()
{
    panel_top->GetSizer()->Layout();
    SetSize(GetSize());
    Refresh();
}


void InitialFrame::save_editing_value()
{
    grid_header->SaveEditControlValue();
    grid_data->SaveEditControlValue();
    grid_trailer->SaveEditControlValue();
    grid_end->SaveEditControlValue();   
}


