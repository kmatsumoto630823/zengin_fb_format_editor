#include "InitialFrame.h"
#include <wx/artprov.h>

InitialFrame::InitialFrame() : wxFrame(NULL, wxID_ANY, "")
{
    // MENU FILE
    menu_file = new wxMenu;
    menu_file->Append(ID_MENU_NEW, "�V�K�i�t�H�[�}�b�g�I���j\tCtrl+N")->SetBitmap(wxArtProvider::GetBitmap(wxART_NEW, wxART_MENU));
    menu_file->Append(ID_MENU_OPEN, "FB�f�[�^���J��");
    menu_file->Append(ID_MENU_SAVEAS, "���O�����ĕۑ�");
    menu_file->AppendSeparator();
    menu_file->Append(ID_MENU_EXIT, "�I��\tCtrl+Q")->SetBitmap(wxArtProvider::GetBitmap(wxART_CLOSE, wxART_MENU));

    // MENU EDIT
    menu_edit = new wxMenu;
    menu_edit->Append(ID_MENU_HEADER_IMPORT, "���w�b�_�F�Ǎ��i�t�@�C���j")->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_MENU));
    menu_edit->Append(ID_MENU_HEADER_EXPORT, "���w�b�_�F�o�́i�t�@�C���j")->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_MENU));
    menu_edit->Append(ID_MENU_HEADER_IMPORT_FROM_CLIPBOARD, "���w�b�_�F�\�t�i�N���b�v�{�[�h�j")->SetBitmap(wxArtProvider::GetBitmap(wxART_PASTE, wxART_MENU));

    menu_edit->AppendSeparator();
    menu_edit->Append(ID_MENU_DATA_SELECT_ALL, "���f�[�^�F�S�I��\tCtrl+A");
    menu_edit->Append(ID_MENU_DATA_CLEAR_SELECTED, "���f�[�^�F�S����");
    menu_edit->Append(ID_MENU_DATA_MOVE_UP, "���f�[�^�F���R�[�h����Ɉړ�")->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_UP, wxART_MENU));
    menu_edit->Append(ID_MENU_DATA_MOVE_DOWN, "���f�[�^�F���R�[�h�����Ɉړ�")->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_DOWN, wxART_MENU));
    menu_edit->Append(ID_MENU_DATA_ADD, "���f�[�^�F�s��\tInsert")->SetBitmap(wxArtProvider::GetBitmap(wxART_PLUS, wxART_MENU));
    menu_edit->Append(ID_MENU_DATA_DELETE,"���f�[�^�F�s��\tDelete")->SetBitmap(wxArtProvider::GetBitmap(wxART_MINUS, wxART_MENU));
    menu_edit->Append(ID_MENU_DATA_ADD_FROM_FILE, "���f�[�^�F�Ǎ��i�t�@�C���j")->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_MENU));
    menu_edit->Append(ID_MENU_DATA_EXPORT, "���f�[�^�F�o�́i�t�@�C���j")->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_MENU));
    menu_edit->Append(ID_MENU_DATA_ADD_FROM_CLIPBOARD, "���f�[�^�F�\�t�i�N���b�v�{�[�h�j\tCtrl+Insert")->SetBitmap(wxArtProvider::GetBitmap(wxART_PASTE, wxART_MENU));
    menu_edit->AppendSeparator();
    menu_edit->Append(ID_MENU_TRAILER_RECALCULATE, "���g���[���[�F�Čv�Z\tF5")->SetBitmap(wxArtProvider::GetBitmap(wxART_REFRESH, wxART_MENU));

    // MENU SEARCH
    menu_search = new wxMenu;
    menu_search->Append(ID_MENU_DATA_SEARCH_BOX, "���f�[�^�F�����{�b�N�X\tCtrl+F")->SetBitmap(wxArtProvider::GetBitmap(wxART_FIND, wxART_MENU));
    menu_search->AppendSeparator();
    menu_search->Append(ID_MENU_DATA_SEARCH_FORWARD, "���f�[�^�F������\tF3")->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_FORWARD, wxART_MENU));
    menu_search->Append(ID_MENU_DATA_SEARCH_BACKWARD, "���f�[�^�F�O����\tShift+F3")->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_BACK, wxART_MENU));;

    // MENU HELP
    menu_help = new wxMenu;
    menu_help->Append(ID_MENU_HELP_ABOUT, "���̃A�v���ɂ���")->SetBitmap(wxArtProvider::GetBitmap(wxART_INFORMATION, wxART_MENU));
    menu_help->Append(ID_MENU_HELP_MANUAL, "�ȈՑ���}�j���A��.txt");
    menu_help->Append(ID_MENU_HELP_README, "�ŏ��ɂ��ǂ݂�������.txt");

    // MENU BAR
    menu_bar = new wxMenuBar;
    menu_bar->Append(menu_file, "�t�@�C��(&F)");
    menu_bar->Append(menu_edit, "�ҏW(&E)");
    menu_bar->Append(menu_search, "����(&S)");
    menu_bar->Append(menu_help, "�w���v(&H)");
    SetMenuBar(menu_bar);

    // TOP PANEL
    panel_top = new wxPanel(this, wxID_ANY);

    //GRID_HEADER
    grid_header = new CustomGrid(panel_top, wxID_ANY);
    button_header_import = new wxButton(panel_top, wxID_ANY, "�Ǎ�"); button_header_import->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR));
    button_header_export = new wxButton(panel_top, wxID_ANY, "�o��"); button_header_export->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_TOOLBAR));
    button_header_import_from_clipboard = new wxButton(panel_top, wxID_ANY, "�\�t"); button_header_import_from_clipboard->SetBitmap(wxArtProvider::GetBitmap(wxART_PASTE, wxART_TOOLBAR));

    //GRID_DATA
    grid_data = new CustomGrid(panel_top, wxID_ANY);
    button_data_select_all = new wxButton(panel_top, wxID_ANY, "�S�I��");
    button_data_clear_selected = new wxButton(panel_top, wxID_ANY, "�S����");
    button_data_move_up = new wxButton(panel_top, wxID_ANY, "��ړ�"); button_data_move_up->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_UP, wxART_TOOLBAR));
    button_data_move_down = new wxButton(panel_top, wxID_ANY, "���ړ�"); button_data_move_down->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_DOWN, wxART_TOOLBAR));
    button_data_add = new wxButton(panel_top, wxID_ANY, "�s��"); button_data_add->SetBitmap(wxArtProvider::GetBitmap(wxART_PLUS, wxART_TOOLBAR));
    button_data_delete = new wxButton(panel_top, wxID_ANY, "�s��"); button_data_delete->SetBitmap(wxArtProvider::GetBitmap(wxART_MINUS, wxART_TOOLBAR));
    button_data_add_from_file = new wxButton(panel_top, wxID_ANY, "�Ǎ�"); button_data_add_from_file->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR));
    button_data_export = new wxButton(panel_top, wxID_ANY, "�o��"); button_data_export->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_TOOLBAR));
    button_data_add_from_clipboard = new wxButton(panel_top, wxID_ANY, "�\�t"); button_data_add_from_clipboard->SetBitmap(wxArtProvider::GetBitmap(wxART_PASTE, wxART_TOOLBAR));
    searchctrl_data_search = new wxSearchCtrl(panel_top, wxID_ANY);
    button_data_search_forward = new wxButton(panel_top, wxID_ANY, "������"); button_data_search_forward->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_FORWARD, wxART_TOOLBAR));
    button_data_search_backward = new wxButton(panel_top, wxID_ANY, "�O����"); button_data_search_backward->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_BACK, wxART_TOOLBAR));

    //GRID_TRAILER
    grid_trailer = new CustomGrid(panel_top, wxID_ANY);
    button_trailer_recalculated = new wxButton(panel_top, wxID_ANY, "�Čv�Z"); button_trailer_recalculated->SetBitmap(wxArtProvider::GetBitmap(wxART_REFRESH, wxART_TOOLBAR));

    //GRID_END
    grid_end = new CustomGrid(panel_top, wxID_ANY);

    //SIZER
    auto sizer_top = new wxBoxSizer(wxVERTICAL);
    panel_top->SetSizer(sizer_top);
        sizer_top->AddSpacer(10);

        auto sizer_header_label = new wxBoxSizer(wxHORIZONTAL);
        sizer_top->Add(sizer_header_label, 0, wxALIGN_LEFT);
            sizer_header_label->Add(new wxStaticText(panel_top, wxID_ANY, "���w�b�_���R�[�h�F�@"), 0, wxALIGN_BOTTOM);
            sizer_header_label->Add(button_header_import, 0, wxALIGN_BOTTOM);
            sizer_header_label->Add(button_header_export, 0, wxALIGN_BOTTOM);
            sizer_header_label->Add(button_header_import_from_clipboard, 0, wxALIGN_BOTTOM);
        sizer_top->Add(grid_header, 0, wxALIGN_LEFT | wxLEFT, 1);
        sizer_top->AddSpacer(10);

        auto sizer_data_label = new wxBoxSizer(wxHORIZONTAL);
        sizer_top->Add(sizer_data_label, 0, wxALIGN_LEFT);
            sizer_data_label->Add(new wxStaticText(panel_top, wxID_ANY, "���f�[�^���R�[�h�F�@"), 0, wxALIGN_BOTTOM);
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
            sizer_trailer_label->Add(new wxStaticText(panel_top, wxID_ANY, "���g���[�����R�[�h�F�@"), 0, wxALIGN_BOTTOM);
            sizer_trailer_label->Add(button_trailer_recalculated, 0, wxALIGN_BOTTOM);
        sizer_top->Add(grid_trailer, 0, wxALIGN_LEFT | wxLEFT, 1);
        sizer_top->AddSpacer(10);

        auto sizer_end_label = new wxBoxSizer(wxHORIZONTAL);
        sizer_top->Add(sizer_end_label, 0, wxALIGN_LEFT);
            sizer_end_label->Add(new wxStaticText(panel_top, wxID_ANY, "���G���h���R�[�h�F�@"), 0, wxALIGN_BOTTOM);
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


