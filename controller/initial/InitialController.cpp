#include "InitialController.h"

#include "view/custom/GridDialog.h"
#include "view/custom/MyAboutDialogInfo.h"

#include "model/FBParser.h"
#include "model/GridOps.h"

#include <wx/clipbrd.h>
#include <wx/log.h>


#define F() (InitialController::get_casted_frame())
#define _do_callback_(collback) do {F()->Freeze(); (collback); F()->force_refresh(); F()->Thaw();} while(0)


void InitialController::switch_type(FBType type)
{
    m_attrs_array = make_attrs_array
    (
        type,
        m_chars_kana.empty() ? nullptr : m_chars_kana.data(),
        m_pad_kana.empty()   ? nullptr : m_pad_kana.data() ,
        m_chars_num.empty()  ? nullptr : m_chars_num.data(),
        m_pad_num.empty()    ? nullptr : m_pad_num.data()
    );


    F()->get_grid_header() ->reset(m_attrs_array.at((FBEnumInt)FBPart::HEADER ));
    F()->get_grid_data()   ->reset(m_attrs_array.at((FBEnumInt)FBPart::DATA   ));
    F()->get_grid_trailer()->reset(m_attrs_array.at((FBEnumInt)FBPart::TRAILER)); 
    F()->get_grid_end()    ->reset(m_attrs_array.at((FBEnumInt)FBPart::END    ));

    F()->get_grid_data()->DeleteRows();

    F()->SetStatusText(wxString("フォーマット：") + FBTypeLabel.at((FBEnumInt)type));
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

    m_chars_kana = config.Read("CHARS_KANA", wxEmptyString).ToStdString(wxCSConv("cp932"));
    m_pad_kana   = config.Read("PAD_KANA",   wxEmptyString).ToStdString(wxCSConv("cp932"));
    m_chars_num  = config.Read("CHARS_NUM",  wxEmptyString).ToStdString(wxCSConv("cp932"));
    m_pad_num    = config.Read("PAD_NUM",    wxEmptyString).ToStdString(wxCSConv("cp932"));

    create_frame(config);
    create_binds(config);

    auto type = (FBType)config.Read("FBTYPE", (long)FBType::SOHFURI);
    switch_type(type);
}

void InitialController::create_frame(wxFileConfig& config)
{
    this->frame = new InitialFrame();

    auto title = config.Read("APP_NAME", wxEmptyString);
    this->frame->SetTitle(title);

    auto w = config.Read("WINDOW_WIDTH", 1600);
    auto h = config.Read("WINDOW_HEIGHT", 900);
    F()->SetSize(w, h);
}

void InitialController::finalize()
{

}

void InitialController::create_binds(wxFileConfig& config)
{

    // NEW
    auto OnNew = [=, this]()
    {
        if(F()->is_edited_any())
        {
            wxMessageDialog mdialog(F(), "編集中のデータが失われます\r\n続行しますか", "確認", wxOK | wxCANCEL);
            if(mdialog.ShowModal() == wxID_CANCEL) return;
        }

        static wxString selection_labels[(FBEnumInt)FBType::ITEM_NUM];
        for(FBEnumInt i = 0; i < (FBEnumInt)FBType::ITEM_NUM; ++i)
        {
            selection_labels[i] = FBTypeLabel[i];
        }

        wxSingleChoiceDialog scdialog(F(), "フォーマットを選択してください", "選択", std::size(selection_labels), selection_labels);
        scdialog.SetSelection((FBEnumInt)FBType::SOHFURI);

        if(scdialog.ShowModal() == wxID_CANCEL) return;

        auto type = (FBType) scdialog.GetSelection();
        switch_type(type);
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnNew() );

    }, ID_MENU_NEW);

    // OPEN
    auto OnOpen = [=, this](const wxString& default_path)
    {
        auto path = default_path;

        if(F()->is_edited_any())
        {
            wxMessageDialog mdialog(F(), "編集中のデータが失われます\r\n続行しますか？", "確認", wxOK | wxCANCEL);
            if(mdialog.ShowModal() == wxID_CANCEL) return;
        }

        if(path.empty())
        {
            wxFileDialog fdialog(F(), "FBデータの選択", ".", wxEmptyString, "TXT files (*.txt)|*.txt|All files (*)|*");
            if(fdialog.ShowModal() == wxID_CANCEL) return;

            path = fdialog.GetPath();
        }

        FBParser fb(m_attrs_array);

        if(!fb.open_file(path.ToStdString(wxCSConv("cp932"))))
        {
            wxLogMessage("fb.open_file(path.ToStdString(wxCSConv(\"cp932\"))) : false");
            wxLogMessage("ファイルの読込に失敗しました");
            return;
        }

        if(fb.get_number_rows(FBPart::HEADER)  != 1) {wxLogMessage("fb.get_number_rows(FBPart::HEADER)  != 1"); return;}
        if(fb.get_number_rows(FBPart::DATA)    <= 0) {wxLogMessage("fb.get_number_rows(FBPart::DATA)    <= 0"); return;}
        if(fb.get_number_rows(FBPart::TRAILER) != 1) {wxLogMessage("fb.get_number_rows(FBPart::TRAILER) != 1"); return;}
        if(fb.get_number_rows(FBPart::END)     != 1) {wxLogMessage("fb.get_number_rows(FBPart::END)     != 1"); return;}

/*
        GridOps::FBGrid fbgridAdptr(&fb);  
        //wxProgressDialog pdialog("進捗", " ", 100, F());
*/
        GridOps::Adapter fbgridAdptr(&fb);
        GridOps::Adapter wxgridAdptr_hedar  (F()->get_grid_header ());
        GridOps::Adapter wxgridAdptr_data   (F()->get_grid_data   ());
        GridOps::Adapter wxgridAdptr_trailer(F()->get_grid_trailer());
        GridOps::Adapter wxgridAdptr_end    (F()->get_grid_end    ());

        GridOps::copy(&fbgridAdptr[FBPart::HEADER] , &wxgridAdptr_hedar  );
        GridOps::copy(&fbgridAdptr[FBPart::DATA]   , &wxgridAdptr_data   );
        GridOps::copy(&fbgridAdptr[FBPart::TRAILER], &wxgridAdptr_trailer);
        GridOps::copy(&fbgridAdptr[FBPart::END]    , &wxgridAdptr_end    );

    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnOpen("") );
        
    }, ID_MENU_OPEN);

    F()->DragAcceptFiles(true);
    F()->Bind(wxEVT_DROP_FILES, [=, this](wxDropFilesEvent& event)
    {
        if(event.GetNumberOfFiles() != 1)
        {
            wxLogMessage("event.GetNumberOfFiles() != 1");
            wxLogMessage("選択されたファイルが一つではありません");
            return;
        };
        
        _do_callback_( OnOpen(event.GetFiles()[0]) );
    });

    // SAVEAS
    auto OnSaveAs = [=, this]()
    {
        F()->save_editing_value();

        if(F()->get_grid_trailer()->GetNumberRows() != 1)
        {
            wxLogMessage("F()->get_grid_trailer()->GetNumberRows() != 1");
            return;
        }

        long long sum_data_kensu = 0;
        long long sum_data_kingaku = 0;

        const int data_row_size = F()->get_grid_data()->GetNumberRows();
        const int data_kingaku_col = 9;

        for(int row = 0; row < data_row_size; row++)
        {
            long long value;
            F()->get_grid_data()->GetCellValue(row, data_kingaku_col).ToLongLong(&value);
            sum_data_kingaku += value;
            sum_data_kensu++;
        }

        const int trailer_kensu_col   = 1;
        const int trailer_kingaku_col = 2;

        long long sum_trailer_kensu;
        long long sum_trailer_kingaku;
        F()->get_grid_trailer()->GetCellValue(0, trailer_kensu_col).ToLongLong(&sum_trailer_kensu);
        F()->get_grid_trailer()->GetCellValue(0, trailer_kingaku_col).ToLongLong(&sum_trailer_kingaku);

        if(sum_data_kensu != sum_trailer_kensu || sum_data_kingaku != sum_trailer_kingaku)
        {
            wxMessageDialog mdialog(F(), "データレコードとトレーラレコードの合計件数、合計金額が一致しません\r\n続行しますか？", "警告", wxOK | wxCANCEL | wxICON_WARNING);
            if(mdialog.ShowModal() == wxID_CANCEL) return;
        }

        static wxString selection_labels[(FBEnumInt)FBNewLine::ITEM_NUM];
        for(FBEnumInt i = 0; i < (FBEnumInt)FBNewLine::ITEM_NUM; ++i)
        {
            selection_labels[i] = FBNewLineLabel[i];
        }
        
        wxSingleChoiceDialog scdialog(F(), "改行コードを選択してください", "選択", std::size(selection_labels), selection_labels);
        scdialog.SetSelection((FBEnumInt)FBNewLine::CRLF);

        if(scdialog.ShowModal() == wxID_CANCEL) return;

        auto newline = scdialog.GetSelection();
        FBParser fb(m_attrs_array);
        fb.set_newline((FBNewLine)newline);
        
        wxFileDialog fdialog(F(), "FBデータの保存", ".", wxEmptyString, "TXT files (*.txt)|*.txt|All files (*)|*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if(fdialog.ShowModal() == wxID_CANCEL) return;

        GridOps::Adapter fbgridAdptr(&fb);
        GridOps::Adapter wxgridAdptr_hedar  (F()->get_grid_header() );
        GridOps::Adapter wxgridAdptr_data   (F()->get_grid_data()   );
        GridOps::Adapter wxgridAdptr_trailer(F()->get_grid_trailer());
        GridOps::Adapter wxgridAdptr_end    (F()->get_grid_end ()   );        

        GridOps::copy(&wxgridAdptr_hedar  , &fbgridAdptr[FBPart::HEADER] );
        GridOps::copy(&wxgridAdptr_data   , &fbgridAdptr[FBPart::DATA]   );
        GridOps::copy(&wxgridAdptr_trailer, &fbgridAdptr[FBPart::TRAILER]);
        GridOps::copy(&wxgridAdptr_end    , &fbgridAdptr[FBPart::END]    );

        auto chk = true;
        if(fb.get_number_rows(FBPart::HEADER)  != 1) {wxLogMessage("fb.get_number_rows(FBPart::HEADER)  != 1"); chk = false;}
        if(fb.get_number_rows(FBPart::DATA)    <= 0) {wxLogMessage("fb.get_number_rows(FBPart::DATA)    <= 0"); chk = false;}
        if(fb.get_number_rows(FBPart::TRAILER) != 1) {wxLogMessage("fb.get_number_rows(FBPart::TRAILER) != 1"); chk = false;}
        if(fb.get_number_rows(FBPart::END)     != 1) {wxLogMessage("fb.get_number_rows(FBPart::END)     != 1"); chk = false;}

        if(!chk)
        {
            wxLogMessage("不正なレコード数です");
            return;
        }

        auto path = fdialog.GetPath().ToStdString(wxCSConv("cp932"));

        if(!fb.save_file(path))
        {
            wxLogMessage("fb.save_file(path)");
            wxLogMessage("ファイルの保存に失敗しました");
        }
    };


    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnSaveAs() );

    }, ID_MENU_SAVEAS);

     // EXIT
    auto OnExit = [=, this]()
    {
        if(F()->is_edited_any())
        {
            wxMessageDialog mdialog(frame, "編集中のデータが失われます\r\n終了しますか？", "確認", wxOK | wxCANCEL);
            if(mdialog.ShowModal() == wxID_CANCEL) return;
        }

        frame->Destroy();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnExit() );

    }, ID_MENU_EXIT);

    F()->Bind(wxEVT_CLOSE_WINDOW, [=, this](wxCloseEvent& event)
    {
        _do_callback_( OnExit() );
    });

    // HEADER IMPORT
    auto OnHeaderImport = [=, this]()
    {
        auto grid = F()->get_grid_header();

        if(grid->GetNumberRows() != 1)
        {
            wxLogMessage("grid->GetNumberRows() != 1");
            return;
        }

        wxFileDialog fdialog(F(), "＠ヘッダ：読込（ファイル）", ".", wxEmptyString, "TXT files (*.txt)|*.txt|All files (*)|*");
        if(fdialog.ShowModal() == wxID_CANCEL) return;

        FBParser fb(m_attrs_array);
        auto path = fdialog.GetPath().ToStdString(wxCSConv("cp932"));

        if(!fb.open_file(path))
        {
            wxLogMessage("fb.open_file(path) : false");
            wxLogMessage("ファイルの読込に失敗しました");
            return;
        }

        if(fb.get_number_rows(FBPart::HEADER) != 1)
        {
            wxLogMessage("fb.get_number_rows(FBPart::HEADER) != 1");
            wxLogMessage("ヘッダーレコードが存在しない、または複数行存在します");
            return;
        }

        auto other_count = fb.get_number_rows(FBPart::DATA) + fb.get_number_rows(FBPart::TRAILER) + fb.get_number_rows(FBPart::END);
        auto message = 
            "ファイル上のレコードをヘッダレコードに読込します" +
            wxString(
                other_count > 0 ? 
                wxString::Format("\r\n※データ区分の異なるレコード「%d件」が存在しましたが無視されます", other_count) : 
                ""
            );
        
        wxMessageDialog mdialog(F(), message, "確認", wxOK | wxCANCEL);
        if(mdialog.ShowModal() == wxID_CANCEL) return;

        const int torikumibi_col = 5; 
        auto value = grid->GetCellValue(0, torikumibi_col);

        GridOps::Adapter fbgridAdptr(&fb);
        fbgridAdptr.set_current_part(FBPart::HEADER);
        GridOps::Adapter wxgridAdptr(grid);   

        GridOps::copy(&fbgridAdptr, &wxgridAdptr);

        grid->SetCellValue(0, torikumibi_col, value);

        grid->SelectRow(0, true);
        grid->GoToCell(0, 0);
        grid->SetFocus();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnHeaderImport() );

    }, ID_MENU_HEADER_IMPORT);

    F()->get_button_header_import()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnHeaderImport() );
    });

    // HEADER EXPORT
    auto OnHeaderExport = [=, this]()
    {
        auto grid = F()->get_grid_header();

        F()->save_editing_value();

        FBParser fb(m_attrs_array);
        fb.set_newline(FBNewLine::NONE);

        wxFileDialog fdialog(F(), "＠ヘッダ：出力（ファイル）", ".", wxEmptyString, "TXT files (*.txt)|*.txt|All files (*)|*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if(fdialog.ShowModal() == wxID_CANCEL) return;

        GridOps::Adapter fbgridAdptr(&fb);
        fbgridAdptr.set_current_part(FBPart::HEADER);
        GridOps::Adapter wxgridAdptr(grid);

        GridOps::copy(&wxgridAdptr, &fbgridAdptr);
        fb.assign_rows(0, FBPart::DATA);
        fb.assign_rows(0, FBPart::TRAILER);
        fb.assign_rows(0, FBPart::END);

        if(fb.get_number_rows(FBPart::HEADER)  != 1) {wxLogMessage("fb.get_number_rows(FBPart::HEADER)  != 1"); return;}
        if(fb.get_number_rows(FBPart::DATA)    != 0) {wxLogMessage("fb.get_number_rows(FBPart::DATA)    != 0"); return;}
        if(fb.get_number_rows(FBPart::TRAILER) != 0) {wxLogMessage("fb.get_number_rows(FBPart::TRAILER) != 0"); return;}
        if(fb.get_number_rows(FBPart::END)     != 0) {wxLogMessage("fb.get_number_rows(FBPart::END)     != 0"); return;}

        auto path = fdialog.GetPath().ToStdString(wxCSConv("cp932"));
        if(!fb.save_file(path))
        {
            wxLogMessage("fb.save_file(path) : false");
            wxLogMessage("ファイルの保存に失敗しました");
        }
    
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnHeaderExport() );

    }, ID_MENU_HEADER_EXPORT);

    F()->get_button_header_export()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnHeaderExport() );
    });

    // HEADER IMPORT FROM CLIPBOARD
    auto OnHeaderImportFromClipBoard = [=, this]()
    {
        auto grid = F()->get_grid_header();

        if(grid->GetNumberRows() != 1)
        {
            wxLogMessage("grid->GetNumberRows() != 1");
            return;
        }

        wxTextDataObject data;
        wxTheClipboard->GetData(data);
        auto text = data.GetText().ToStdString(wxCSConv("cp932"));
        wxTheClipboard->Close();

        FBParser fb(m_attrs_array);
        if(!fb.from_text(text))
        {
            wxLogMessage("fb.from_text(text) : false");
            wxLogMessage("クリップボードの取込に失敗しました");
            return;
        }       


        if(fb.get_number_rows(FBPart::HEADER) != 1)
        {
            wxLogMessage("fb.get_number_rows(FBPart::HEADER) != 1");
            wxLogMessage("ヘッダーレコードが存在しない、または複数行存在します");
            return;
        }

        auto other_count = fb.get_number_rows(FBPart::DATA) + fb.get_number_rows(FBPart::TRAILER) + fb.get_number_rows(FBPart::END);
        auto message = 
            "クリップボード上のレコードをヘッダレコードに読込します" +
            wxString(
                other_count > 0 ? 
                wxString::Format("\r\n※データ区分の異なるレコード「%d件」が存在しましたが無視されます", other_count) : 
                ""
            );
        
        wxMessageDialog mdialog(F(), message, "確認", wxOK | wxCANCEL);
        if(mdialog.ShowModal() == wxID_CANCEL) return;

        const int torikumibi_col = 5; 
        auto value = grid->GetCellValue(0, torikumibi_col);

        GridOps::Adapter fbgridAdptr(&fb);
        fbgridAdptr.set_current_part(FBPart::HEADER);
        GridOps::Adapter wxgridAdptr(grid);
        
        GridOps::copy(&fbgridAdptr, &wxgridAdptr);

        grid->SetCellValue(0, torikumibi_col, value);

        grid->SelectRow(0, true);
        grid->GoToCell(0, 0);
        grid->SetFocus();

    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnHeaderImportFromClipBoard() );

    }, ID_MENU_HEADER_IMPORT_FROM_CLIPBOARD);

    F()->get_button_header_import_from_clipboard()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnHeaderImportFromClipBoard() );
    });

    // DATA SELECT ALL
    auto OnDataSelectAll = [=, this]()
    {
        auto grid = F()->get_grid_data();
        auto selected = grid->GetSelectedRows();

        if(selected.size() == grid->GetNumberRows())
        {
            grid->ClearSelection();
        }
        else
        {
            grid->SelectAll();
        }

        grid->SetFocus();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataSelectAll() );

    }, ID_MENU_DATA_SELECT_ALL);

    F()->get_button_data_select_all()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataSelectAll() );
    });

    // DATA CLEAR SELECTED
    auto OnDataClearSelected = [=, this]()
    {
        auto grid = F()->get_grid_data();
        grid->ClearSelection();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataClearSelected() );

    }, ID_MENU_DATA_CLEAR_SELECTED);

    F()->get_button_data_clear_selected()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataClearSelected() );
    });

    // DATA MOVE UP
    auto OnDataMoveUp = [=, this]()
    {
        auto grid = F()->get_grid_data();
        grid->move_up_selected();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataMoveUp() );

    }, ID_MENU_DATA_MOVE_UP);

    F()->get_button_data_move_up()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataMoveUp() );
    });

    // DATA MOVE DOWN
    auto OnDataMoveDown = [=, this]()
    {
        auto grid = F()->get_grid_data();
        grid->move_down_selected();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataMoveDown() );

    }, ID_MENU_DATA_MOVE_DOWN);

    F()->get_button_data_move_down()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataMoveDown() );
    });

    // DATA ADD
    auto OnDataAdd = [=, this]()
    {
        auto grid = F()->get_grid_data();
        grid->insert_selected();

    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataAdd() );

    }, ID_MENU_DATA_ADD);

    F()->get_button_data_add()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataAdd() );
    });


    // DATA DELETE
    auto OnDataDelete = [=, this]()
    {
        auto grid = F()->get_grid_data();
        grid->delete_selected();

    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataDelete() ); 

    }, ID_MENU_DATA_DELETE);

    F()->get_button_data_delete()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataDelete() );          
    });

    // DATA ADD FROM FILE
    auto OnDataAddFromFile = [=, this]()
    {
        auto grid = F()->get_grid_data();
        auto selected = grid->GetSelectedRows();
        auto pos = grid->GetNumberRows();

        if(selected.size() > 1)
        {
            wxLogMessage("selected.size() > 1");
            wxLogMessage("挿入位置が複数選択されています");
            return;
        }

        if(selected.size() == 1)
        {
            pos = selected[0];
        }

        wxFileDialog fdialog(F(), "＠データ：読込（ファイル）", ".", wxEmptyString, "TXT files (*.txt)|*.txt|All files (*)|*");
        if(fdialog.ShowModal() == wxID_CANCEL) return;

        FBParser fb(m_attrs_array);
        auto path = fdialog.GetPath().ToStdString(wxCSConv("cp932"));

        if(!fb.open_file(path))
        {
            wxLogMessage("fb.open_file(path) : false");
            wxLogMessage("ファイルの読込に失敗しました");
            return;
        }

        if(fb.get_number_rows(FBPart::DATA) <= 0)
        {
            wxLogMessage("fb.get_number_rows(FBPart::DATA) <= 0");
            wxLogMessage("データレコードが存在しません");
            return;
        }

        auto other_count = fb.get_number_rows(FBPart::HEADER) + fb.get_number_rows(FBPart::TRAILER) + fb.get_number_rows(FBPart::END);
        auto message = other_count > 0 ? wxString::Format("※データ区分の異なるレコード「%d件」が存在しましたが無視されました", other_count) : wxString("");
        auto size = F()->GetSize();
        size.SetHeight(size.GetHeight()*2/3);

        GridDialog gdialog(m_attrs_array[(FBEnumInt)FBPart::DATA], message, F(), wxID_ANY, "レコードの選択", wxDefaultPosition, size);
        auto dlgrid = gdialog.get_grid();

        GridOps::Adapter fbgridAdptr(&fb); fbgridAdptr.set_current_part(FBPart::DATA);     
        GridOps::Adapter dlgridAdptr(dlgrid);
        GridOps::Adapter wxgridAdptr(grid);

        GridOps::copy(&fbgridAdptr, &dlgridAdptr);

        if(gdialog.ShowModal() == wxID_CANCEL) return;
        dlgrid->remain_selected();

        GridOps::insert(&dlgridAdptr, &wxgridAdptr, pos);
        
        grid->ClearSelection();

        auto begin = pos;
        auto end = begin + fbgridAdptr.GetNumberRows();
        for(auto row = begin; row < end; ++row)
        {
            grid->SelectRow(row, true);
        }

        grid->GoToCell(end - 1, 0);
        grid->SetFocus();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataAddFromFile() );

    }, ID_MENU_DATA_ADD_FROM_FILE);

    F()->get_button_data_add_from_file()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataAddFromFile() );
    });

    // DATA EXPORT
    auto OnDataExport = [=, this]()
    {
        auto grid = F()->get_grid_data();

        F()->save_editing_value();

        static wxString selection_labels[(FBEnumInt)FBNewLine::ITEM_NUM];
        for(FBEnumInt i = 0; i < (FBEnumInt)FBNewLine::ITEM_NUM; ++i)
        {
            selection_labels[i] = FBNewLineLabel[i];
        }
        
        wxSingleChoiceDialog scdialog(F(), "改行コードを選択してください", "選択", std::size(selection_labels), selection_labels);
        scdialog.SetSelection((FBEnumInt)FBNewLine::CRLF);

        if(scdialog.ShowModal() == wxID_CANCEL) return;

        auto newline = scdialog.GetSelection();
        FBParser fb(m_attrs_array);
        fb.set_newline((FBNewLine)newline);

        wxFileDialog fdialog(F(), "＠データ：出力（ファイル）", ".", wxEmptyString, "TXT files (*.txt)|*.txt|All files (*)|*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if(fdialog.ShowModal() == wxID_CANCEL) return;

        GridOps::Adapter fbgridAdptr(&fb);
        fbgridAdptr.set_current_part(FBPart::DATA);
        GridOps::Adapter wxgridAdptr(grid);

        fb.assign_rows(0, FBPart::HEADER);
        GridOps::copy(&wxgridAdptr , &fbgridAdptr);
        fb.assign_rows(0, FBPart::TRAILER);
        fb.assign_rows(0, FBPart::END);

        if(fb.get_number_rows(FBPart::HEADER)  != 0) {wxLogMessage("fb.get_number_rows(FBPart::HEADER)  != 0"); return;}
        if(fb.get_number_rows(FBPart::DATA)    <= 0) {wxLogMessage("fb.get_number_rows(FBPart::DATA)    <= 0"); return;}
        if(fb.get_number_rows(FBPart::TRAILER) != 0) {wxLogMessage("fb.get_number_rows(FBPart::TRAILER) != 0"); return;}
        if(fb.get_number_rows(FBPart::END)     != 0) {wxLogMessage("fb.get_number_rows(FBPart::END)     != 0"); return;}

        auto path = fdialog.GetPath().ToStdString(wxCSConv("cp932"));
        if(!fb.save_file(path))
        {
            wxLogMessage("fb.save_file(path) : false");
            wxLogMessage("ファイルの保存に失敗しました");
        }
    
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataExport() );

    }, ID_MENU_DATA_EXPORT);

    F()->get_button_data_export()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataExport() );
    });

    // DATA ADD FROM CRIPBOARD
    auto OnDataAddFromClipBoard = [=, this]()
    {
        auto grid = F()->get_grid_data();
        auto selected = grid->GetSelectedRows();
        auto pos = grid->GetNumberRows();

        if(selected.size() > 1)
        {
            wxLogMessage("selected.size() > 1");
            wxLogMessage("挿入位置が複数選択されています");
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
        auto text = data.GetText().ToStdString(wxCSConv("cp932"));
        wxTheClipboard->Close();

        FBParser fb(m_attrs_array);
        if(!fb.from_text(text))
        {
            wxLogMessage("fb.from_text(text) : false");
            wxLogMessage("クリップボードからの取込に失敗しました");
            return;
        }

        if(fb.get_number_rows(FBPart::DATA) <= 0)
        {
            wxLogMessage("fb.get_number_rows(FBPart::DATA) <= 0");
            wxLogMessage("データレコードが存在しません");
            return;
        }

        auto other_count = fb.get_number_rows(FBPart::HEADER) + fb.get_number_rows(FBPart::TRAILER) + fb.get_number_rows(FBPart::END);
        auto message = 
            wxString::Format("クリップボード上のレコード「%d行」を\r\nデータレコード「%d行目」の位置に挿入します", fb.get_number_rows(FBPart::DATA), pos + 1) +
            wxString(
                other_count > 0 ? 
                wxString::Format("\r\n※データ区分の異なるレコード「%d件」が存在しましたが無視されます", other_count) : 
                ""
            );

        wxMessageDialog mdialog(F(), message, "確認", wxOK | wxCANCEL);
        if(mdialog.ShowModal() == wxID_CANCEL) return;

        GridOps::Adapter fbgridAdptr(&fb);
        fbgridAdptr.set_current_part(FBPart::DATA);
        GridOps::Adapter wxgridAdptr(grid);

        GridOps::insert(&fbgridAdptr, &wxgridAdptr, pos);
        
        grid->ClearSelection();

        auto begin = pos;
        auto end = begin + fbgridAdptr.GetNumberRows();
        for(auto row = begin; row < end; ++row)
        {
            grid->SelectRow(row, true);
        }

        grid->GoToCell(end - 1, 0);
        grid->SetFocus();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataAddFromClipBoard() );

    }, ID_MENU_DATA_ADD_FROM_CLIPBOARD);

    F()->get_button_data_add_from_clipboard()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataAddFromClipBoard() );
    });


    // SEARCH BOX
    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            F()->get_searchctrl_data_search()->SetFocus()
        );

    }, ID_MENU_DATA_SEARCH_BOX);

    // SEARCH FORWARD
    auto OnDataSearchForward = [=, this]()
    {
        auto grid = F()->get_grid_data();
        auto search_value = F()->get_searchctrl_data_search()->GetValue();
        auto is_forward = true;
        
        grid->search_next_value(search_value, is_forward);
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataSearchForward() );

    }, ID_MENU_DATA_SEARCH_FORWARD);

    F()->get_searchctrl_data_search()->Bind(wxEVT_SEARCH, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataSearchForward() );
    });    

    F()->get_button_data_search_forward()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataSearchForward() );
    });

    // SEARCH BACKWARD
    auto OnDataSearchBackward = [=, this]()
    {
        auto grid = F()->get_grid_data();
        auto search_value = F()->get_searchctrl_data_search()->GetValue();
        auto is_forward = false;
        
        grid->search_next_value(search_value, is_forward);
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataSearchBackward() );

    }, ID_MENU_DATA_SEARCH_BACKWARD);

    F()->get_button_data_search_backward()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnDataSearchBackward() );
    });

    // TRAILER RECALCULATE
    auto OnTrailerRecalculate = [=, this]()
    {
        
        auto grid_data = F()->get_grid_data();
        auto attrs_data = m_attrs_array.at((FBEnumInt)FBPart::DATA);
        auto grid_trailer = F()->get_grid_trailer();
        auto attrs_trailer = m_attrs_array.at((FBEnumInt)FBPart::TRAILER);

        if(grid_trailer->GetNumberRows() != 1)
        {
            wxLogMessage("grid_trailer->GetNumberRows() != 1");
            return;
        }

        grid_data->SaveEditControlValue();

        long long sum_kensu = 0;
        long long sum_kingaku = 0;

        const int data_row_size = grid_data->GetNumberRows();
        const int data_kingaku_col = 9;

        for(int row = 0; row < data_row_size; row++)
        {
            long long value;
            grid_data->GetCellValue(row, data_kingaku_col).ToLongLong(&value);

            sum_kingaku += value;
            sum_kensu++;
        }

        const int trailer_kensu_col   = 1;
        const int trailer_kingaku_col = 2;

        long long sum_kensu_old;
        long long sum_kingaku_old;
        grid_trailer->GetCellValue(0, trailer_kensu_col).ToLongLong(&sum_kensu_old);
        grid_trailer->GetCellValue(0, trailer_kingaku_col).ToLongLong(&sum_kingaku_old);

        wxString massage;
        massage += "差分：　";
        massage += wxString::Format("%lld", sum_kensu - sum_kensu_old) + "件　";
        massage += wxString::Format("%lld", sum_kingaku - sum_kingaku_old) + "円　";

        wxMessageDialog mdialog(grid_trailer, massage, "情報", wxOK);
        mdialog.ShowModal();

        auto sum_kingaku_str = wxString::Format("%lld", sum_kingaku);
        auto sum_kensu_str   = wxString::Format("%lld", sum_kensu);

        {
            auto& value = sum_kensu_str;
            auto& attr = attrs_trailer[trailer_kensu_col];

            if(value.size() > attr.length)
            {
                wxLogMessage("value.size() > attr.length");
                return;
            }
            
            if(attr.pad_info[0] == 'R')
            {
                grid_trailer->SetCellValue(0, attr.order, value.append(attr.length - value.length(), attr.pad_info[1]));
            }
            else if(attr.pad_info[0] == 'L')
            {
                grid_trailer->SetCellValue(0, attr.order, value.insert(0, attr.length - value.length(), attr.pad_info[1]));
            }            
        }

        {
            auto& value = sum_kingaku_str;
            auto& attr = attrs_trailer[trailer_kingaku_col];

            if(value.size() > attr.length)
            {
                wxLogMessage("value.size() > attr.length");
                return;
            }
            
            if(attr.pad_info[0] == 'R')
            {
                grid_trailer->SetCellValue(0, attr.order, value.append(attr.length - value.length(), attr.pad_info[1]));
            }
            else if(attr.pad_info[0] == 'L')
            {
                grid_trailer->SetCellValue(0, attr.order, value.insert(0, attr.length - value.length(), attr.pad_info[1]));
            }            
        }

    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnTrailerRecalculate() );

    }, ID_MENU_TRAILER_RECALCULATE);

    F()->get_button_trailer_recalculated()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_( OnTrailerRecalculate() );
    });

    // SHOW ABOUT
    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( MyAboutDialogInfo().myAboutBox(F()) );

    }, ID_MENU_HELP_ABOUT);

    // SHOW MANUAL
    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( wxExecute("notepad.exe 簡易操作マニュアル.txt") );

    }, ID_MENU_HELP_MANUAL);

    // SHOW README
    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_( wxExecute("notepad.exe 最初にお読みください.txt") );

    }, ID_MENU_HELP_README);

}