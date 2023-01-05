#include "InitialController.h"

#include "view/custom/GridDialog.h"
#include "view/custom/MyAboutDialogInfo.h"

#include "model/FBParser.h"
#include "model/GridOps.h"

#include <wx/fileconf.h>
#include <wx/clipbrd.h>

#include <ranges>

#define F() (get_frame())
#define _do_callback_(collback) do {F()->Freeze(); (collback); F()->force_refresh(); F()->Thaw();} while(0)


void InitialController::switch_type(FBType type)
{
    m_type = type;
    m_attrs_array = make_attrs_array(m_type, m_chars_kana, m_padding_kana, m_chars_num, m_padding_num);

    F()->get_grid_header() ->reset(m_attrs_array.at((FBEnumInt)FBPart::HEADER ));
    F()->get_grid_data()   ->reset(m_attrs_array.at((FBEnumInt)FBPart::DATA   ));
    F()->get_grid_trailer()->reset(m_attrs_array.at((FBEnumInt)FBPart::TRAILER)); 
    F()->get_grid_end()    ->reset(m_attrs_array.at((FBEnumInt)FBPart::END    ));

    F()->get_grid_data()->DeleteRows();

    F()->SetStatusText(wxString("�t�H�[�}�b�g�F") + FBTypeLabel.at((FBEnumInt)type));
    F()->force_refresh();
}


void InitialController::initialize()
{
    wxFileConfig config
    (
        wxEmptyString,
        wxEmptyString,
        "config.ini",
        wxEmptyString,
        wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH,
        wxCSConv("cp932")
    );

    config.SetPath("/");

    m_app_name = config.Read("APP_NAME", wxEmptyString);

    m_window_width  = config.Read("WINDOW_WIDTH", 1600);
    m_window_height = config.Read("WINDOW_HEIGHT", 900);
    
    //m_chars_kana   = config.Read("CHARS_KANA",   wxEmptyString).ToStdString(wxCSConv("cp932"));
    //m_padding_kana = config.Read("PADDING_KANA", wxEmptyString).ToStdString(wxCSConv("cp932"));
    //m_chars_num    = config.Read("CHARS_NUM",    wxEmptyString).ToStdString(wxCSConv("cp932"));
    //m_padding_num  = config.Read("PADDING_NUM",  wxEmptyString).ToStdString(wxCSConv("cp932"));
   
    m_chars_kana   = config.Read("CHARS_KANA",   wxEmptyString);
    m_padding_kana = config.Read("PADDING_KANA", wxEmptyString);
    m_chars_num    = config.Read("CHARS_NUM",    wxEmptyString);
    m_padding_num  = config.Read("PADDING_NUM",  wxEmptyString);

    create_frame();
    create_binds();

    m_type = (FBType)config.Read("FBTYPE", (long)FBType::SOHFURI);
    switch_type(m_type);
}

void InitialController::finalize()
{
    //Do Nothing Yet.
}

bool InitialController::toggle_view()
{
    return F()->IsFullScreen() ? F()->Show(false) : F()->Show(true);
}

void InitialController::create_frame()
{
    m_frame = new InitialFrame();
    F()->SetTitle(m_app_name);
    F()->SetSize(m_window_width, m_window_height);
}

void InitialController::create_binds()
{
    // NEW
    auto OnNew = [=, this]()
    {
        if(F()->is_edited_any())
        {
            wxMessageDialog mdialog(F(), "�ҏW���̃f�[�^�������܂�\r\n���s���܂���", "�m�F", wxOK | wxCANCEL);
            if(mdialog.ShowModal() == wxID_CANCEL)
            {
                return;
            }
        }

        auto labels = FBTypeLabel;
        wxArrayString choices(labels.size(), labels.data());

        wxSingleChoiceDialog scdialog(F(), "�t�H�[�}�b�g��I�����Ă�������", "�I��", choices);
        scdialog.SetSelection((FBEnumInt)FBType::SOHFURI);

        if(scdialog.ShowModal() == wxID_CANCEL) 
        {
            return;
        }

        auto type = (FBType)scdialog.GetSelection();
        switch_type(type);
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnNew() );

    }, ID_MENU_NEW);

    // OPEN
    auto OnOpen = [=, this](const wxString& default_path)
    {
        if(F()->is_edited_any())
        {
            wxMessageDialog mdialog(F(), "�ҏW���̃f�[�^�������܂�\r\n���s���܂����H", "�m�F", wxOK | wxCANCEL);
            if(mdialog.ShowModal() == wxID_CANCEL)
            {
                return;
            }
        }

        //auto path = default_path.ToStdString(wxCSConv("cp932"));
        auto&& path = default_path.ToStdString();

        if(path.empty())
        {
            wxFileDialog fdialog(F(), "FB�f�[�^�̑I��", wxEmptyString, wxEmptyString, "TXT files (*.txt)|*.txt|All files (*)|*");
            if(fdialog.ShowModal() == wxID_CANCEL)
            {
                return;
            }

            path = fdialog.GetPath();
        }

        FBParser fb(m_attrs_array);

        if(!fb.open_file(path))
        {
            wxLogMessage("fb.open_file(path) : false");
            wxLogMessage("�t�@�C���̓Ǎ��Ɏ��s���܂���");
            return;
        }

        if(fb[FBPart::HEADER ].get_number_rows() != 1){ wxLogMessage("fb[FBPart::HEADER ].get_number_rows() != 1"); return; }
        if(fb[FBPart::DATA   ].get_number_rows() <= 0){ wxLogMessage("fb[FBPart::DATA   ].get_number_rows() <= 0"); return; }
        if(fb[FBPart::TRAILER].get_number_rows() != 1){ wxLogMessage("fb[FBPart::TRAILER].get_number_rows() != 1"); return; }
        if(fb[FBPart::END    ].get_number_rows() != 1){ wxLogMessage("fb[FBPart::END    ].get_number_rows() != 1"); return; }


        for(FBEnumInt i = 0; i < (FBEnumInt)FBPart::ITEM_NUM; ++i)
        {
            GridOps::Adapter src = fb[i];
            GridOps::Adapter dst = F()->get_grid_array()[i];
            GridOps::copy(src, dst);
        }
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnOpen("") );
        
    }, ID_MENU_OPEN);

    F()->DragAcceptFiles(true);
    F()->Bind(wxEVT_DROP_FILES, [=, this](wxDropFilesEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        
        if(event.GetNumberOfFiles() != 1)
        {
            wxLogMessage("event.GetNumberOfFiles() != 1");
            wxLogMessage("�I�����ꂽ�t�@�C������ł͂���܂���");
            return;
        };
        
        _do_callback_( OnOpen(event.GetFiles()[0]) );
    });

    // SAVEAS
    auto OnSaveAs = [=, this]()
    {
        auto wxgrid_data = F()->get_grid_data();
        auto wxtable_data = wxgrid_data->GetTable();

        auto wxgrid_trailer = F()->get_grid_trailer();
        auto wxtable_trailer = wxgrid_trailer->GetTable();

        F()->save_editing_value();

        if(wxgrid_trailer->GetNumberRows() != 1)
        {
            wxLogMessage("wxgrid_trailer->GetNumberRows() != 1");
            return;
        }

        long long sum_kensu_data = 0;
        long long sum_kingaku_data = 0;
        long long zero_kingaku_count = 0;

        auto num_rows_data = wxgrid_data->GetNumberRows();

        for(decltype(num_rows_data) row = 0; row < num_rows_data; ++row)
        {
            long long value = 0;
            wxtable_data->GetValue(row, FB_ORDER_DATA_KINGAKU).ToLongLong(&value);

            if(value == 0)
            {
                ++zero_kingaku_count;
            }

            sum_kingaku_data += value;
            ++sum_kensu_data;
        }

        long long sum_kensu_trailer = 0;
        long long sum_kingaku_trailer = 0;

        wxtable_trailer->GetValue(0, FB_ORDER_TRAILER_KENSU).ToLongLong(&sum_kensu_trailer);
        wxtable_trailer->GetValue(0, FB_ORDER_TRAILER_KINGAKU).ToLongLong(&sum_kingaku_trailer);

        if(sum_kensu_data != sum_kensu_trailer || sum_kingaku_data != sum_kingaku_trailer)
        {
            wxMessageDialog mdialog(F(), "�f�[�^���R�[�h�ƃg���[�����R�[�h�̍��v�����A���v���z����v���܂���\r\n���s���܂����H", "�x��", wxOK | wxCANCEL | wxICON_WARNING);
            if(mdialog.ShowModal() == wxID_CANCEL)
            {
                return;
            }
        }

        if(zero_kingaku_count > 0)
        {
            wxMessageDialog mdialog(F(), "���z���O�~�̃f�[�^���R�[�h�����݂��܂�\r\n���s���܂����H", "�x��", wxOK | wxCANCEL | wxICON_WARNING);
            if(mdialog.ShowModal() == wxID_CANCEL)
            {
                return;
            }
        }

        auto labels = FBNewLineLabel;
        wxArrayString choices(labels.size(), labels.data());
        
        wxSingleChoiceDialog scdialog(F(), "���s�R�[�h��I�����Ă�������", "�I��", choices);
        scdialog.SetSelection((FBEnumInt)FBNewLine::CRLF);

        if(scdialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }

        auto newline = scdialog.GetSelection();
        FBParser fb(m_attrs_array);
        fb.set_newline((FBNewLine)newline);
        
        wxFileDialog fdialog(F(), "FB�f�[�^�̕ۑ�", wxEmptyString, wxEmptyString, "TXT files (*.txt)|*.txt|All files (*)|*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if(fdialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }

        for(FBEnumInt i = 0; i < (FBEnumInt)FBPart::ITEM_NUM; ++i)
        {
            GridOps::Adapter src = F()->get_grid_array()[i];
            GridOps::Adapter dst = fb[i];
            GridOps::copy(src, dst);
        }

        auto chk_cnt = true;
        if(fb[FBPart::HEADER ].get_number_rows() != 1){ wxLogMessage("fb[FBPart::HEADER ].get_number_rows() != 1"); chk_cnt = false; }
        if(fb[FBPart::DATA   ].get_number_rows() <= 0){ wxLogMessage("fb[FBPart::DATA   ].get_number_rows() <= 0"); chk_cnt = false; }
        if(fb[FBPart::TRAILER].get_number_rows() != 1){ wxLogMessage("fb[FBPart::TRAILER].get_number_rows() != 1"); chk_cnt = false; }
        if(fb[FBPart::END    ].get_number_rows() != 1){ wxLogMessage("fb[FBPart::END    ].get_number_rows() != 1"); chk_cnt = false; }

        if(!chk_cnt)
        {
            wxLogMessage("�s���ȃ��R�[�h���ł�");
            return;
        }

        //auto path = fdialog.GetPath().ToStdString(wxCSConv("cp932"));
        auto path = fdialog.GetPath().ToStdString();

        if(!fb.save_file(path))
        {
            wxLogMessage("fb.save_file(path)");
            wxLogMessage("�t�@�C���̕ۑ��Ɏ��s���܂���");
        }
    };


    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnSaveAs() );

    }, ID_MENU_SAVEAS);

     // EXIT
    auto OnExit = [=, this]()
    {
        if(F()->is_edited_any())
        {
            wxMessageDialog mdialog(F(), "�ҏW���̃f�[�^�������܂�\r\n�I�����܂����H", "�m�F", wxOK | wxCANCEL);
            if(mdialog.ShowModal() == wxID_CANCEL)
            {
                return;
            }
        }

        F()->Destroy();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnExit() );

    }, ID_MENU_EXIT);

    F()->Bind(wxEVT_CLOSE_WINDOW, [=, this](wxCloseEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnExit() );
    });

    // HEADER IMPORT
    auto OnHeaderImport = [=, this]()
    {
        auto wxgrid = F()->get_grid_header();
        auto wxtable = wxgrid->GetTable();

        if(wxgrid->GetNumberRows() != 1)
        {
            wxLogMessage("wxgrid->GetNumberRows() != 1");
            return;
        }

        wxFileDialog fdialog(F(), "���w�b�_�F�Ǎ��i�t�@�C���j", wxEmptyString, wxEmptyString, "TXT files (*.txt)|*.txt|All files (*)|*");
        if(fdialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }

        FBParser fb(m_attrs_array);
        //auto path = fdialog.GetPath().ToStdString(wxCSConv("cp932"));
        auto path = fdialog.GetPath().ToStdString();

        if(!fb.open_file(path))
        {
            wxLogMessage("fb.open_file(path) : false");
            wxLogMessage("�t�@�C���̓Ǎ��Ɏ��s���܂���");
            return;
        }

        if(fb[FBPart::HEADER].get_number_rows() != 1)
        {
            wxLogMessage("fb[FBPart::HEADER].get_number_rows() != 1");
            wxLogMessage("�w�b�_�[���R�[�h�����݂��Ȃ��A�܂��͕����s���݂��܂�");
            return;
        }

        auto other_count = fb[FBPart::DATA].get_number_rows() + fb[FBPart::TRAILER].get_number_rows() + fb[FBPart::END].get_number_rows();
        auto message = 
            "�t�@�C����̃��R�[�h���w�b�_���R�[�h�ɓǍ����܂�" +
            wxString(
                other_count > 0 ? 
                wxString::Format("\r\n���f�[�^�敪�̈قȂ郌�R�[�h�u%d���v�����݂��܂�������������܂�", other_count) : 
                ""
            );
        
        wxMessageDialog mdialog(F(), message, "�m�F", wxOK | wxCANCEL);
        if(mdialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }

        auto&& value = wxtable->GetValue(0, FB_ORDER_HEADER_TORIKUMIBI);

        GridOps::Adapter src = fb[FBPart::HEADER];
        GridOps::Adapter dst = wxgrid;   
        GridOps::copy(src, dst);

        wxtable->SetValue(0, FB_ORDER_HEADER_TORIKUMIBI, value);

        wxgrid->SelectRow(0, true);
        wxgrid->GoToCell(0, 0);
        wxgrid->SetFocus();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnHeaderImport() );

    }, ID_MENU_HEADER_IMPORT);

    F()->get_button_header_import()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnHeaderImport() );
    });

    // HEADER EXPORT
    auto OnHeaderExport = [=, this]()
    {
        auto wxgrid = F()->get_grid_header();

        F()->save_editing_value();

        FBParser fb(m_attrs_array);
        fb.set_newline(FBNewLine::NONE);

        wxFileDialog fdialog(F(), "���w�b�_�F�o�́i�t�@�C���j", wxEmptyString, wxEmptyString, "TXT files (*.txt)|*.txt|All files (*)|*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if(fdialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }

        GridOps::Adapter src = wxgrid;
        GridOps::Adapter dst = fb[FBPart::HEADER];
        GridOps::copy(src, dst);

        fb[FBPart::DATA   ].assign_rows(0);
        fb[FBPart::TRAILER].assign_rows(0);
        fb[FBPart::END    ].assign_rows(0);

        if(fb[FBPart::HEADER ].get_number_rows() != 1){ wxLogMessage("fb[FBPart::HEADER ].get_number_rows() != 1"); return; } 
        if(fb[FBPart::DATA   ].get_number_rows() != 0){ wxLogMessage("fb[FBPart::DATA   ].get_number_rows() != 0"); return; }
        if(fb[FBPart::TRAILER].get_number_rows() != 0){ wxLogMessage("fb[FBPart::TRAILER].get_number_rows() != 0"); return; }
        if(fb[FBPart::END    ].get_number_rows() != 0){ wxLogMessage("fb[FBPart::END    ].get_number_rows() != 0"); return; }

        //auto path = fdialog.GetPath().ToStdString(wxCSConv("cp932"));
        auto path = fdialog.GetPath().ToStdString();
        if(!fb.save_file(path))
        {
            wxLogMessage("fb.save_file(path) : false");
            wxLogMessage("�t�@�C���̕ۑ��Ɏ��s���܂���");
        }
    
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnHeaderExport() );

    }, ID_MENU_HEADER_EXPORT);

    F()->get_button_header_export()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnHeaderExport() );
    });

    // HEADER IMPORT FROM CLIPBOARD
    auto OnHeaderImportFromClipBoard = [=, this]()
    {
        auto wxgrid = F()->get_grid_header();
        auto wxtable = wxgrid->GetTable();

        if(wxgrid->GetNumberRows() != 1)
        {
            wxLogMessage("wxgrid->GetNumberRows() != 1");
            return;
        }

        wxTextDataObject data;
        wxTheClipboard->GetData(data);
        //auto text = data.GetText().ToStdString(wxCSConv("cp932"));
        auto&& text = data.GetText().ToStdString();
        wxTheClipboard->Close();

        FBParser fb(m_attrs_array);
        if(!fb.from_text(text))
        {
            wxLogMessage("fb.from_text(text) : false");
            wxLogMessage("�N���b�v�{�[�h�̎捞�Ɏ��s���܂���");
            return;
        }       


        if(fb[FBPart::HEADER].get_number_rows() != 1)
        {
            wxLogMessage("fb[FBPart::HEADER].get_number_rows() != 1");
            wxLogMessage("�w�b�_�[���R�[�h�����݂��Ȃ��A�܂��͕����s���݂��܂�");
            return;
        }

        auto other_count = fb[FBPart::DATA].get_number_rows() + fb[FBPart::TRAILER].get_number_rows() + fb[FBPart::END].get_number_rows();
        auto message = 
            "�N���b�v�{�[�h��̃��R�[�h���w�b�_���R�[�h�ɓǍ����܂�" +
            wxString(
                other_count > 0 ? 
                wxString::Format("\r\n���f�[�^�敪�̈قȂ郌�R�[�h�u%d���v�����݂��܂�������������܂�", other_count) : 
                ""
            );
        
        wxMessageDialog mdialog(F(), message, "�m�F", wxOK | wxCANCEL);
        if(mdialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }

        auto&& value = wxtable->GetValue(0, FB_ORDER_HEADER_TORIKUMIBI);

        GridOps::Adapter src = fb[FBPart::HEADER];
        GridOps::Adapter dst = wxgrid;
        GridOps::copy(src, dst);

        wxtable->SetValue(0, FB_ORDER_HEADER_TORIKUMIBI, value);

        wxgrid->SelectRow(0, true);
        wxgrid->GoToCell(0, 0);
        wxgrid->SetFocus();

    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnHeaderImportFromClipBoard() );

    }, ID_MENU_HEADER_IMPORT_FROM_CLIPBOARD);

    F()->get_button_header_import_from_clipboard()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnHeaderImportFromClipBoard() );
    });

    // DATA SELECT ALL
    auto OnDataSelectAll = [=, this]()
    {
        auto wxgrid = F()->get_grid_data();
        auto selected = wxgrid->GetSelectedRows();

        if(selected.size() == wxgrid->GetNumberRows())
        {
            wxgrid->ClearSelection();
        }
        else
        {
            wxgrid->SelectAll();
        }

        wxgrid->SetFocus();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataSelectAll() );

    }, ID_MENU_DATA_SELECT_ALL);

    F()->get_button_data_select_all()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataSelectAll() );
    });

    // DATA CLEAR SELECTED
    auto OnDataClearSelected = [=, this]()
    {
        auto wxgrid = F()->get_grid_data();
        wxgrid->ClearSelection();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataClearSelected() );

    }, ID_MENU_DATA_CLEAR_SELECTED);

    F()->get_button_data_clear_selected()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataClearSelected() );
    });

    // DATA MOVE UP
    auto OnDataMoveUp = [=, this]()
    {
        auto wxgrid = F()->get_grid_data();
        wxgrid->move_up_selected();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataMoveUp() );

    }, ID_MENU_DATA_MOVE_UP);

    F()->get_button_data_move_up()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataMoveUp() );
    });

    // DATA MOVE DOWN
    auto OnDataMoveDown = [=, this]()
    {
        auto wxgrid = F()->get_grid_data();
        wxgrid->move_down_selected();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataMoveDown() );

    }, ID_MENU_DATA_MOVE_DOWN);

    F()->get_button_data_move_down()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataMoveDown() );
    });

    // DATA ADD
    auto OnDataAdd = [=, this]()
    {
        auto wxgrid = F()->get_grid_data();
        wxgrid->insert_selected();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataAdd() );

    }, ID_MENU_DATA_ADD);

    F()->get_button_data_add()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataAdd() );
    });


    // DATA DELETE
    auto OnDataDelete = [=, this]()
    {
        auto wxgrid = F()->get_grid_data();
        wxgrid->delete_selected();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataDelete() ); 

    }, ID_MENU_DATA_DELETE);

    F()->get_button_data_delete()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataDelete() );          
    });

    // DATA ADD FROM FILE
    auto OnDataAddFromFile = [=, this]()
    {
        auto wxgrid = F()->get_grid_data();
        auto selected = wxgrid->GetSelectedRows();
        auto pos = wxgrid->GetNumberRows();

        if(selected.size() > 1)
        {
            wxLogMessage("selected.size() > 1");
            wxLogMessage("�}���ʒu�������I������Ă��܂�");
            return;
        }

        if(selected.size() == 1)
        {
            pos = selected[0];
        }

        wxFileDialog fdialog(F(), "���f�[�^�F�Ǎ��i�t�@�C���j", wxEmptyString, wxEmptyString, "TXT files (*.txt)|*.txt|All files (*)|*");
        if(fdialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }

        FBParser fb(m_attrs_array);
        //auto path = fdialog.GetPath().ToStdString(wxCSConv("cp932"));
        auto path = fdialog.GetPath().ToStdString();

        if(!fb.open_file(path))
        {
            wxLogMessage("fb.open_file(path) : false");
            wxLogMessage("�t�@�C���̓Ǎ��Ɏ��s���܂���");
            return;
        }

        if(fb[FBPart::DATA].get_number_rows() <= 0)
        {
            wxLogMessage("fb[FBPart::DATA].get_number_rows() <= 0");
            wxLogMessage("�f�[�^���R�[�h�����݂��܂���");
            return;
        }

        auto other_count = fb[FBPart::HEADER].get_number_rows() + fb[FBPart::TRAILER].get_number_rows() + fb[FBPart::END].get_number_rows();
        auto message = other_count > 0 ? wxString::Format("���f�[�^�敪�̈قȂ郌�R�[�h�u%d���v�����݂��܂�������������܂���", other_count) : wxString("");

        GridDialog gdialog(m_attrs_array[(FBEnumInt)FBPart::DATA], F(), wxID_ANY, "���R�[�h�̑I��" + message);
        auto dlgrid = gdialog.get_grid();

        {
            GridOps::Adapter src = fb[FBPart::DATA];
            GridOps::Adapter dst = dlgrid;
            GridOps::copy(src, dst);
        }

        gdialog.SetSize(m_window_width, m_window_height);
        if(gdialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }
        dlgrid->remain_selected();

        {
            GridOps::Adapter src = dlgrid;
            GridOps::Adapter dst = wxgrid;
            GridOps::insert(src, dst, pos);
        }
        
        wxgrid->ClearSelection();

        auto begin = pos;
        auto end = begin + fb[FBPart::DATA].get_number_rows();
        for(decltype(end) row = begin; row < end; ++row)
        {
            wxgrid->SelectRow(row, true);
        }

        wxgrid->GoToCell(end - 1, 0);
        wxgrid->SetFocus();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataAddFromFile() );

    }, ID_MENU_DATA_ADD_FROM_FILE);

    F()->get_button_data_add_from_file()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataAddFromFile() );
    });

    // DATA EXPORT
    auto OnDataExport = [=, this]()
    {
        auto wxgrid = F()->get_grid_data();

        F()->save_editing_value();

        auto labels = FBNewLineLabel;
        wxArrayString choices(labels.size(), labels.data());
        
        wxSingleChoiceDialog scdialog(F(), "���s�R�[�h��I�����Ă�������", "�I��", choices);
        scdialog.SetSelection((FBEnumInt)FBNewLine::CRLF);

        if(scdialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }

        auto newline = scdialog.GetSelection();
        FBParser fb(m_attrs_array);
        fb.set_newline((FBNewLine)newline);

        wxFileDialog fdialog(F(), "���f�[�^�F�o�́i�t�@�C���j", wxEmptyString, wxEmptyString, "TXT files (*.txt)|*.txt|All files (*)|*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if(fdialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }

        GridOps::Adapter src = wxgrid;
        GridOps::Adapter dst = fb[FBPart::DATA];
        GridOps::copy(src, dst);

        fb[FBPart::HEADER ].assign_rows(0);
        fb[FBPart::TRAILER].assign_rows(0);
        fb[FBPart::END    ].assign_rows(0);

        if(fb[FBPart::HEADER ].get_number_rows() != 0){ wxLogMessage("fb[FBPart::HEADER ].get_number_rows() != 0"); return; }
        if(fb[FBPart::DATA   ].get_number_rows() <= 0){ wxLogMessage("fb[FBPart::DATA   ].get_number_rows() <= 0"); return; }
        if(fb[FBPart::TRAILER].get_number_rows() != 0){ wxLogMessage("fb[FBPart::TRAILER].get_number_rows() != 0"); return; }
        if(fb[FBPart::END    ].get_number_rows() != 0){ wxLogMessage("fb[FBPart::END    ].get_number_rows() != 0"); return; }

        //auto path = fdialog.GetPath().ToStdString(wxCSConv("cp932"));
        auto path = fdialog.GetPath().ToStdString();
        if(!fb.save_file(path))
        {
            wxLogMessage("fb.save_file(path) : false");
            wxLogMessage("�t�@�C���̕ۑ��Ɏ��s���܂���");
        }
    
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataExport() );

    }, ID_MENU_DATA_EXPORT);

    F()->get_button_data_export()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataExport() );
    });

    // DATA ADD FROM CRIPBOARD
    auto OnDataAddFromClipBoard = [=, this]()
    {
        auto wxgrid = F()->get_grid_data();
        auto selected = wxgrid->GetSelectedRows();
        auto pos = wxgrid->GetNumberRows();

        if(selected.size() > 1)
        {
            wxLogMessage("selected.size() > 1");
            wxLogMessage("�}���ʒu�������I������Ă��܂�");
            return;
        }

        if(selected.size() == 1)
        {
            pos = selected[0];
        }

        if(!wxTheClipboard->IsSupported(wxDF_UNICODETEXT))
        {
            wxLogMessage("wxTheClipboard->IsSupported(wxDF_UNICODETEXT) : false");
            return;
        }

        if(!wxTheClipboard->Open())
        {
            wxLogMessage("wxTheClipboard->Open() : false");
            return;
        }
        
        wxTextDataObject data;
        wxTheClipboard->GetData(data);
        //auto text = data.GetText().ToStdString(wxCSConv("cp932"));
        auto text = data.GetText().ToStdString();
        wxTheClipboard->Close();

        FBParser fb(m_attrs_array);
        if(!fb.from_text(text))
        {
            wxLogMessage("fb.from_text(text) : false");
            wxLogMessage("�N���b�v�{�[�h����̎捞�Ɏ��s���܂���");
            return;
        }

        if(fb[FBPart::DATA].get_number_rows() <= 0)
        {
            wxLogMessage("fb[FBPart::DATA].get_number_rows() <= 0");
            wxLogMessage("�f�[�^���R�[�h�����݂��܂���");
            return;
        }

        auto other_count = fb[FBPart::HEADER].get_number_rows() + fb[FBPart::TRAILER].get_number_rows() + fb[FBPart::END].get_number_rows();
        auto message = 
            wxString::Format("�N���b�v�{�[�h��̃��R�[�h�u%d�s�v��\r\n�f�[�^���R�[�h�u%d�s�ځv�̈ʒu�ɑ}�����܂�", fb[FBPart::DATA].get_number_rows(), pos + 1) +
            wxString(
                other_count > 0 ? 
                wxString::Format("\r\n���f�[�^�敪�̈قȂ郌�R�[�h�u%d���v�����݂��܂�������������܂�", other_count) : 
                ""
            );

        wxMessageDialog mdialog(F(), message, "�m�F", wxOK | wxCANCEL);
        if(mdialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }

        GridOps::Adapter src = fb[FBPart::DATA];
        GridOps::Adapter dst = wxgrid;
        GridOps::insert(src, dst, pos);
        
        wxgrid->ClearSelection();

        auto begin = pos;
        auto end = begin + fb[FBPart::DATA].get_number_rows();
        for(decltype(end) row = begin; row < end; ++row)
        {
            wxgrid->SelectRow(row, true);
        }

        wxgrid->GoToCell(end - 1, 0);
        wxgrid->SetFocus();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataAddFromClipBoard() );

    }, ID_MENU_DATA_ADD_FROM_CLIPBOARD);

    F()->get_button_data_add_from_clipboard()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataAddFromClipBoard() );
    });


    // SEARCH BOX
    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_
        (
            F()->get_searchctrl_data_search()->SetFocus()
        );

    }, ID_MENU_DATA_SEARCH_BOX);

    // SEARCH FORWARD
    auto OnDataSearchForward = [=, this]()
    {
        auto wxgrid = F()->get_grid_data();
        auto search_value = F()->get_searchctrl_data_search()->GetValue();
        auto is_forward = true;
        
        wxgrid->search_next_value(search_value, is_forward);
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataSearchForward() );

    }, ID_MENU_DATA_SEARCH_FORWARD);

    F()->get_searchctrl_data_search()->Bind(wxEVT_SEARCH, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataSearchForward() );
    });    

    F()->get_button_data_search_forward()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataSearchForward() );
    });

    // SEARCH BACKWARD
    auto OnDataSearchBackward = [=, this]()
    {
        auto wxgrid = F()->get_grid_data();
        auto search_value = F()->get_searchctrl_data_search()->GetValue();
        auto is_forward = false;
        
        wxgrid->search_next_value(search_value, is_forward);
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataSearchBackward() );

    }, ID_MENU_DATA_SEARCH_BACKWARD);

    F()->get_button_data_search_backward()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnDataSearchBackward() );
    });

    // TRAILER RECALCULATE
    auto OnTrailerRecalculate = [=, this]()
    {
        auto wxgrid_data = F()->get_grid_data();
        auto wxtable_data = wxgrid_data->GetTable();
        auto attrs_data = m_attrs_array.at((FBEnumInt)FBPart::DATA);

        auto wxgrid_trailer = F()->get_grid_trailer();
        auto wxtable_trailer = wxgrid_trailer->GetTable();
        auto attrs_trailer = m_attrs_array.at((FBEnumInt)FBPart::TRAILER);

        if(wxgrid_trailer->GetNumberRows() != 1)
        {
            wxLogMessage("wxgrid_trailer->GetNumberRows() != 1");
            return;
        }

        wxgrid_data->SaveEditControlValue();

        long long sum_kensu   = 0;
        long long sum_kingaku = 0;

        auto num_rows_data = wxgrid_data->GetNumberRows();

        for(decltype(num_rows_data) row = 0; row < num_rows_data; ++row)
        {
            long long value;
            wxtable_data->GetValue(row, FB_ORDER_DATA_KINGAKU).ToLongLong(&value);

            sum_kingaku += value;
            ++sum_kensu;
        }


        long long sum_kensu_old   = 0;
        long long sum_kingaku_old = 0;

        wxtable_trailer->GetValue(0, FB_ORDER_TRAILER_KENSU).ToLongLong(&sum_kensu_old);
        wxtable_trailer->GetValue(0, FB_ORDER_TRAILER_KINGAKU).ToLongLong(&sum_kingaku_old);

        wxString massage;
        massage += "�����F�@";
        massage += wxString::Format("%lld", sum_kensu   - sum_kensu_old  ) + "���@";
        massage += wxString::Format("%lld", sum_kingaku - sum_kingaku_old) + "�~�@";

        wxMessageDialog mdialog(wxgrid_trailer, massage, "���", wxOK);
        mdialog.ShowModal();

        auto sum_kingaku_str = wxString::Format("%lld", sum_kingaku);
        auto sum_kensu_str   = wxString::Format("%lld", sum_kensu  );

        {
            auto& value = sum_kensu_str;
            auto& attr = attrs_trailer[FB_ORDER_TRAILER_KENSU];

            attr.format_value(value);

            if(!attr.check_value(value))
            {
                wxLogMessage("attr.check_value(value) : false");
                return;
            }

            wxtable_trailer->SetValue(0, attr.order, value);            
        }

        {
            auto& value = sum_kingaku_str;
            auto& attr = attrs_trailer[FB_ORDER_TRAILER_KINGAKU];

            attr.format_value(value);

            if(!attr.check_value(value))
            {
                wxLogMessage("attr.check_value(value) : false");
                return;
            }

            wxtable_trailer->SetValue(0, attr.order, value);
        }
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnTrailerRecalculate() );

    }, ID_MENU_TRAILER_RECALCULATE);

    F()->get_button_trailer_recalculated()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( OnTrailerRecalculate() );
    });

    // SHOW ABOUT
    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( MyAboutDialogInfo().myAboutBox(F()) );

    }, ID_MENU_HELP_ABOUT);

    // SHOW MANUAL
    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( wxExecute("notepad.exe �ȈՑ���}�j���A��.txt") );

    }, ID_MENU_HELP_MANUAL);

    // SHOW README
    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& e)
    {
        [[maybe_unused]] auto& event = e;
        _do_callback_( wxExecute("notepad.exe �ŏ��ɂ��ǂ݂�������.txt") );

    }, ID_MENU_HELP_README);

}