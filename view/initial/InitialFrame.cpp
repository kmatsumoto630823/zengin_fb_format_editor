#include "InitialFrame.h"

#include <wx/menu.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/artprov.h>
#include <wx/log.h>



InitialFrame::InitialFrame() : wxFrame(NULL, wxID_ANY, "")
{

    // MENU FILE
    menu_file = new wxMenu;
    menu_file->Append(ID_MENU_NEW, "�V�K�i�t�H�[�}�b�g�I���j\tCtrl+N")->SetBitmap(wxArtProvider::GetBitmap(wxART_NEW, wxART_MENU));
    menu_file->Append(ID_MENU_OPEN, "FB�f�[�^���J��")->SetBitmap(wxArtProvider::GetBitmap(wxART_FOLDER_OPEN, wxART_MENU));
    menu_file->Append(ID_MENU_SAVEAS, "���O�����ĕۑ�")->SetBitmap(wxArtProvider::GetBitmap(wxART_FLOPPY, wxART_MENU));
    menu_file->AppendSeparator();
    menu_file->Append(ID_MENU_EXIT, "�I��\tCtrl+Q")->SetBitmap(wxArtProvider::GetBitmap(wxART_CLOSE, wxART_MENU));

    // MENU EDIT
    menu_edit = new wxMenu;
    menu_edit->Append(ID_MENU_HEADER_IMPORT, "���w�b�_�F�v���Z�b�g�I��")->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_MENU));
    menu_edit->Append(ID_MENU_HEADER_EXPORT, "���w�b�_�F�v���Z�b�g�o��")->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_MENU));
    menu_edit->AppendSeparator();
    menu_edit->Append(ID_MENU_DATA_ADD, "���f�[�^�F�s��")->SetBitmap(wxArtProvider::GetBitmap(wxART_PLUS, wxART_MENU));
    menu_edit->Append(ID_MENU_DATA_DELETE,"���f�[�^�F�s��\tDelete")->SetBitmap(wxArtProvider::GetBitmap(wxART_MINUS, wxART_MENU));
    menu_edit->AppendSeparator();
    menu_edit->Append(ID_MENU_TRAILER_RECALCULATE,"���g���[���F�Čv�Z")->SetBitmap(wxArtProvider::GetBitmap(wxART_EXECUTABLE_FILE, wxART_MENU));

    // MENU SEARCH
    menu_search = new wxMenu;
    menu_search->Append(ID_MENU_DATA_SEARCH_BOX,"���f�[�^�F�����{�b�N�X\tCtrl+F")->SetBitmap(wxArtProvider::GetBitmap(wxART_FIND, wxART_MENU));
    menu_search->AppendSeparator();
    menu_search->Append(ID_MENU_DATA_SEARCH_FORWARD, "���f�[�^�F������\tF3")->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_DOWN, wxART_MENU));
    menu_search->Append(ID_MENU_DATA_SEARCH_BACKWARD, "���f�[�^�F�O����\tShift+F3")->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_UP, wxART_MENU));;

    // MENU HELP
    menu_help = new wxMenu;
    menu_help->Append(ID_MENU_HELP_ABOUT, "���̃A�v���ɂ���")->SetBitmap(wxArtProvider::GetBitmap(wxART_INFORMATION, wxART_MENU));

    // MENU BAR
    menu_bar = new wxMenuBar;
    menu_bar->Append(menu_file, "�t�@�C��(&F)");
    menu_bar->Append(menu_edit, "�ҏW(&E)");
    menu_bar->Append(menu_search, "����(&S)");
    menu_bar->Append(menu_help, "���(&H)");
    SetMenuBar(menu_bar);

    // TOP PANEL
    panel_top = new wxPanel(this, wxID_ANY);

    //GRID_HEADER
    grid_header = new customGrid(panel_top, wxID_ANY);
    button_header_import = new wxButton(panel_top, wxID_ANY, "�v���Z�b�g�I��"); button_header_import->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR));
    button_header_export = new wxButton(panel_top, wxID_ANY, "�v���Z�b�g�o��"); button_header_export->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_TOOLBAR));

    //GRID_DATA
    grid_data = new customGrid(panel_top, wxID_ANY);
    button_data_add = new wxButton(panel_top, wxID_ANY, "�s��"); button_data_add->SetBitmap(wxArtProvider::GetBitmap(wxART_PLUS, wxART_TOOLBAR));
    button_data_delete = new wxButton(panel_top, wxID_ANY, "�s��"); button_data_delete->SetBitmap(wxArtProvider::GetBitmap(wxART_MINUS, wxART_TOOLBAR));
    searchctrl_data_search = new wxSearchCtrl(panel_top, wxID_ANY);
    button_data_search_forward = new wxButton(panel_top, wxID_ANY, "������"); button_data_search_forward->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_DOWN, wxART_TOOLBAR));
    button_data_search_backward = new wxButton(panel_top, wxID_ANY, "�O����"); button_data_search_backward->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_UP, wxART_TOOLBAR));

    //GRID_TRAILER
    grid_trailer = new customGrid(panel_top, wxID_ANY);
    button_trailer_recalculate = new wxButton(panel_top, wxID_ANY, "�Čv�Z"); button_trailer_recalculate->SetBitmap(wxArtProvider::GetBitmap(wxART_EXECUTABLE_FILE, wxART_TOOLBAR));

    //GRID_END
    grid_end = new customGrid(panel_top, wxID_ANY);

    //SIZER
    auto sizer_top = new wxBoxSizer(wxVERTICAL);
    panel_top->SetSizer(sizer_top);
        auto sizer_header_label = new wxBoxSizer(wxHORIZONTAL);
        sizer_top->Add(sizer_header_label, 0, wxALIGN_LEFT);
            sizer_header_label->Add(new wxStaticText(panel_top, wxID_ANY, "���w�b�_���R�[�h�F�@"), 0, wxALIGN_BOTTOM);
            sizer_header_label->Add(button_header_import, 0, wxALIGN_BOTTOM);
            sizer_header_label->Add(button_header_export, 0, wxALIGN_BOTTOM);
        sizer_top->Add(grid_header, 0, wxALIGN_LEFT | wxLEFT, 1);
        sizer_top->AddSpacer(10);

        auto sizer_data_label = new wxBoxSizer(wxHORIZONTAL);
        sizer_top->Add(sizer_data_label, 0, wxALIGN_LEFT);
            sizer_data_label->Add(new wxStaticText(panel_top, wxID_ANY, "���f�[�^���R�[�h�F�@"), 0, wxALIGN_BOTTOM);
            sizer_data_label->Add(button_data_add, 0, wxALIGN_BOTTOM);
            sizer_data_label->Add(button_data_delete, 0, wxALIGN_BOTTOM | wxRIGHT, 50);
            sizer_data_label->Add(searchctrl_data_search, 0, wxALIGN_BOTTOM);
            sizer_data_label->Add(button_data_search_forward, 0, wxALIGN_BOTTOM);
            sizer_data_label->Add(button_data_search_backward, 0, wxALIGN_BOTTOM);
        sizer_top->Add(grid_data, 1, wxALIGN_LEFT | wxEXPAND | wxLEFT, 1);
        sizer_top->AddSpacer(10);

        auto sizer_trailer_label = new wxBoxSizer(wxHORIZONTAL);
        sizer_top->Add(sizer_trailer_label, 0, wxALIGN_LEFT);
            sizer_trailer_label->Add(new wxStaticText(panel_top, wxID_ANY, "���g���[�����R�[�h�F�@"), 0, wxALIGN_BOTTOM);
            sizer_trailer_label->Add(button_trailer_recalculate, 0, wxALIGN_BOTTOM);
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



wxMenuBar* InitialFrame::get_menu_bar()
{
    return menu_bar;
}

wxMenu* InitialFrame::get_menu_file()
{
    return menu_file;
}

wxMenu* InitialFrame::get_menu_edit()
{
    return menu_edit;
}

wxMenu* InitialFrame::get_menu_search()
{
    return menu_search;
}

wxPanel* InitialFrame::get_panel_top()
{
    return panel_top;
}

customGrid* InitialFrame::get_grid_header()
{
    return grid_header;
}

wxButton* InitialFrame::get_button_header_import()
{
    return button_header_import;   
}

wxButton* InitialFrame::get_button_header_export()
{
    return button_header_export;
}

customGrid* InitialFrame::get_grid_data()
{
    return grid_data;
}

wxButton* InitialFrame::get_button_data_add()
{
    return button_data_add;
}

wxButton* InitialFrame::get_button_data_delete()
{
    return button_data_delete;
}

wxSearchCtrl* InitialFrame::get_searchctrl_data_search()
{
    return searchctrl_data_search;
}

wxButton* InitialFrame::get_button_data_search_forward()
{
    return button_data_search_forward;
}

wxButton* InitialFrame::get_button_data_search_backward()
{
    return button_data_search_backward;
}

customGrid* InitialFrame::get_grid_trailer()
{
    return grid_trailer;
}

wxButton* InitialFrame::get_button_trailer_recalculated()
{
    return button_trailer_recalculate;
}

customGrid* InitialFrame::get_grid_end()
{
    return grid_end;
}


