#include "InitialController.h"
#include "view/custom/myAboutDialogInfo.h"

#include <wx/progdlg.h>
#include <wx/log.h>

#define F() (InitialController::get_casted_frame())
#define _do_callback_(collback) do {F()->Freeze(); (collback); F()->force_refresh(); F()->Thaw();} while(0)

class FBGrid
{
public:
    FBGrid(FBParser& fb){ set_fb(fb); };
    ~FBGrid(){};

    FBGrid& set_fbpart(FBPart part)
    {
        m_fbpart = part;
        return *this;
    }

    FBGrid& set_fb(FBParser& fb)
    {
        m_fb = &fb;
        m_fbpart = fb.get_fbpart();
        return *this;
    }    

    //Adapter Function (like wxGridTableBase)
    int GetNumberRows () const { return m_fb->get_number_rows(m_fbpart); };
    int GetNumberCols () const { return m_fb->get_number_cols(m_fbpart); };

    const wxString& GetValue(int row, int col)
    {
        auto value = m_fb->get_value(row, col, m_fbpart);
        m_wxstring_buff.assign(value.data(), value.size());

        return m_wxstring_buff;
    };

    void SetValue(int row, int col, const wxString& value)
    {
        m_string_buff = value;
        m_fb->set_value(row, col, m_string_buff, m_fbpart);
    };

    bool AppendRows(int numRows = 1){ return m_fb->append_rows(numRows, m_fbpart); };
    bool InsertRows(int pos = 0, int numRows = 1){ return m_fb->insert_rows(pos, numRows, m_fbpart); };
    bool DeleteRows(int pos = 0, int numRows = 1){ return m_fb->delete_rows(pos, numRows, m_fbpart); };

private:
    FBParser* m_fb;
    FBPart m_fbpart;

    wxString m_wxstring_buff;
    std::string m_string_buff;
};

template<typename T, typename U>
inline void grid2grid(T* src, U* dst)
{
    if constexpr (std::is_same_v<T, U>)
    {
        if(src == dst)
        {
            wxLogMessage("srt == dst");
            return;      
        }
    }

    if(src->GetNumberCols() != dst->GetNumberCols())
    {
        wxLogMessage("src->GetNumberCols() != dst->GetNumberCols()");
        return;
    }

    auto num_rows = src->GetNumberRows();
    auto num_cols = src->GetNumberCols();

    if(dst->GetNumberRows() != 0) dst->DeleteRows(0, dst->GetNumberRows());
    dst->AppendRows(num_rows);

    for(decltype(num_rows) row = 0; row < num_rows; row++)  
    {
        for(decltype(num_cols) col = 0; col < num_cols; col++)
        {
            auto&& value = src->GetValue(row, col);
            dst->SetValue(row, col, value);
        }
    }
}

template<typename T, typename U>
inline void grid2grid(T* src, U* dst, wxProgressDialog* dlg)
{
    if constexpr (std::is_same_v<T, U>)
    {
        if(src == dst)
        {
            wxLogMessage("srt == dst");
            return;      
        }
    }

    if(src->GetNumberCols() != dst->GetNumberCols())
    {
        wxLogMessage("src->GetNumberCols() != dst->GetNumberCols()");
        return;
    }

    auto num_rows = src->GetNumberRows();
    auto num_cols = src->GetNumberCols();

    if(dst->GetNumberRows() != 0) dst->DeleteRows(0, dst->GetNumberRows());
    dst->AppendRows(num_rows);

    wxString msg;
    auto update_msg = [&msg, &dlg](auto row, auto num_rows)
    {
        msg.Empty();
        msg <<  row * 100 / num_rows << "% ";
        msg << "( " << row  << " / " << num_rows << " )";
        dlg->SetRange(num_rows);
        dlg->Update(row, msg);
    };

    for(decltype(num_rows) row = 0; row < num_rows; row++)  
    {
       
        if(row % (num_rows / 100 + 1) == 0)
        {
            update_msg(row, num_rows);
        }

        for(decltype(num_cols) col = 0; col < num_cols; col++)
        {
            auto&& value = src->GetValue(row, col);
            dst->SetValue(row, col, value);
        }
    }

    update_msg(num_rows, num_rows);
}


void InitialController::switch_fbtype(FBType type)
{

    wxString status_string;
    switch(type)
    {
        case FBType::SOHFURI:
            status_string = "フォーマット；総合振込";
            break;

        case FBType::KYUYO_SHOYO:
            status_string = "フォーマット：給与・賞与振込";
            break;

        case FBType::FURIKAE:
            status_string = "フォーマット：預金口座振替";
            break;

        default:
            return;
    }

    m_fb.set_fbtype(type, m_chars_kana.ToStdString(wxCSConv("cp932")));

    F()->get_grid_header() ->reset(m_fb.get_attrs(FBPart::HEADER ));
    F()->get_grid_data()   ->reset(m_fb.get_attrs(FBPart::DATA   ));
    F()->get_grid_trailer()->reset(m_fb.get_attrs(FBPart::TRAILER)); 
    F()->get_grid_end()    ->reset(m_fb.get_attrs(FBPart::END    )); 

    F()->SetStatusText(status_string);
    F()->force_refresh();
}

bool InitialController::is_edited_any()
{

    if(F()->get_grid_header() ->is_edited()) return true;
    if(F()->get_grid_data()   ->is_edited()) return true;
    if(F()->get_grid_trailer()->is_edited()) return true;
    if(F()->get_grid_end()    ->is_edited()) return true;          

    return false; 
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

    m_fbdata_path = config.Read("FBDATA_PATH", wxEmptyString);
    m_preset_path = config.Read("PRESET_PATH", wxEmptyString);
    m_export_path = config.Read("EXPORT_PATH", wxEmptyString);

    m_chars_kana = config.Read("CHARS_KANA", wxEmptyString);

    create_frame(config);
    create_binds(config);
}

void InitialController::create_frame(wxFileConfig& config)
{
    this->frame = new InitialFrame();

    m_app_name = config.Read("APP_NAME", wxEmptyString);
    this->frame->SetTitle(m_app_name);

    if(config.Read("BUTTTON_HEADER_IMPORT", wxEmptyString) == "HIDE")
    {
        F()->get_button_header_import()->Hide();
    }

    if(config.Read("BUTTTON_HEADER_EXPORT", wxEmptyString) == "HIDE")
    {
        F()->get_button_header_export()->Hide();
    }

    auto type = (FBType)config.Read("FBTYPE", (long)FBType::SOHFURI);
    switch_fbtype(type);

    auto w = config.Read("WINDOW_WIDTH", 1600);
    auto h = config.Read("WINDOW_HEIGHT", 900);
    F()->SetSize(wxSize(w, h));
}

void InitialController::finalize()
{

}


void InitialController::create_binds(wxFileConfig& config)
{

    // NEW
    auto OnNew = [=]()
    {
        if(is_edited_any())
        {
            wxMessageDialog mdialog(F(), "編集中のデータが失われます\r\n続行しますか", "確認", wxOK | wxCANCEL);
            if(mdialog.ShowModal() == wxID_CANCEL) return;
        }

        wxString format_selection_strings[3];
        format_selection_strings[(int)FBType::SOHFURI] = "総合振込";
        format_selection_strings[(int)FBType::KYUYO_SHOYO] = "給与・賞与振込";
        format_selection_strings[(int)FBType::FURIKAE] = "預金口座振替";

        wxSingleChoiceDialog scdialog(F(), "フォーマットを選択してください", "選択", std::size(format_selection_strings) , format_selection_strings);
        scdialog.SetSelection((int)FBType::SOHFURI);

        if(scdialog.ShowModal() == wxID_CANCEL) return;

        auto type = (FBType) scdialog.GetSelection();
        switch_fbtype(type);
    };

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnNew()
        );

    }, ID_MENU_NEW);

    // OPEN
    auto OnOpen = [=](const wxString& default_path)
    {
        auto path = default_path;

        if(is_edited_any())
        {
            wxMessageDialog mdialog(F(), "編集中のデータが失われます\r\n続行しますか？", "確認", wxOK | wxCANCEL);
            if(mdialog.ShowModal() == wxID_CANCEL) return;
        }

        if(path.empty())
        {
            wxFileDialog fdialog(F(), "FBデータの選択", m_fbdata_path, wxEmptyString, "TXT files (*.txt)|*.txt|All files (*)|*");
            if(fdialog.ShowModal() == wxID_CANCEL) return;

            path = fdialog.GetPath();
        }

        if(!m_fb.open_file(path.ToStdString(wxCSConv("cp932"))))
        {
            wxLogMessage("m_fb.open_file(fdialog.GetPath().ToStdString(wxCSConv(\"cp932\"))) : false");
            return;
        }

        if(m_fb.get_number_rows(FBPart::HEADER)  != 1) {wxLogMessage("m_fb.get_rows_num(FBPart::HEADER)  != 1"); return;}
        if(m_fb.get_number_rows(FBPart::DATA)    <= 0) {wxLogMessage("m_fb.get_rows_num(FBPart::DATA)    <= 0"); return;}
        if(m_fb.get_number_rows(FBPart::TRAILER) != 1) {wxLogMessage("m_fb.get_rows_num(FBPart::TRAILER) != 1"); return;}
        if(m_fb.get_number_rows(FBPart::END)     != 1) {wxLogMessage("m_fb.get_rows_num(FBPart::END)     != 1"); return;}

        FBGrid fbgrid(m_fb);  
        wxProgressDialog pdialog("進捗", " ", 100, F());

        grid2grid(&fbgrid.set_fbpart(FBPart::HEADER)  ,F()->get_grid_header()->GetTable() );
        grid2grid(&fbgrid.set_fbpart(FBPart::DATA)    ,F()->get_grid_data()->GetTable()   , &pdialog);
        grid2grid(&fbgrid.set_fbpart(FBPart::TRAILER) ,F()->get_grid_trailer()->GetTable());
        grid2grid(&fbgrid.set_fbpart(FBPart::END)     ,F()->get_grid_end()->GetTable()    );

    };

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnOpen("")
        );
        
    }, ID_MENU_OPEN);

    F()->DragAcceptFiles(true);
    F()->Bind(wxEVT_DROP_FILES, [=](wxDropFilesEvent& event)
    {
        if(event.GetNumberOfFiles() != 1)
        {
            wxLogMessage("event.GetNumberOfFiles() != 1");
            return;
        };
        
        _do_callback_
        (
            OnOpen(event.GetFiles()[0])
        );
    });

    // SAVEAS
    auto OnSaveAs = [=]()
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
            wxMessageDialog mdialog(F(), "データレコードとトレーラーレコードの合計件数、合計金額が一致しません\r\n続行しますか？", "警告", wxOK | wxCANCEL | wxICON_WARNING);
            if(mdialog.ShowModal() == wxID_CANCEL) return;
        }

        static const wxString newline_code_selection_strings[] =
        {
            "CRLF(デフォルト)",
            "CR",
            "LF",
            "改行なし"
        };

        
        wxSingleChoiceDialog scdialog(F(), "改行コードを選択してください", "選択", std::size(newline_code_selection_strings) , newline_code_selection_strings);
        scdialog.SetSelection(FBNewLine::CRLF);

        if(scdialog.ShowModal() == wxID_CANCEL) return;

        auto newline = scdialog.GetSelection();
        m_fb.set_newline((FBNewLine)newline);
        
        wxFileDialog fdialog(F(), "FBデータの保存", m_export_path, wxEmptyString, "TXT files (*.txt)|*.txt|All files (*)|*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if(fdialog.ShowModal() == wxID_CANCEL) return;

        FBGrid fbgrid(m_fb);
        wxProgressDialog pdialog("進捗", " ", 100, F());

        grid2grid(F()->get_grid_header()->GetTable() , &fbgrid.set_fbpart(FBPart::HEADER) );
        grid2grid(F()->get_grid_data()->GetTable()   , &fbgrid.set_fbpart(FBPart::DATA)   , &pdialog);
        grid2grid(F()->get_grid_trailer()->GetTable(), &fbgrid.set_fbpart(FBPart::TRAILER));
        grid2grid(F()->get_grid_end()->GetTable()    , &fbgrid.set_fbpart(FBPart::END)    );

        if(m_fb.get_number_rows(FBPart::HEADER)  != 1) {wxLogMessage("m_fb.get_rows_num(FBPart::HEADER)  != 1"); return;}
        if(m_fb.get_number_rows(FBPart::DATA)    <= 0) {wxLogMessage("m_fb.get_rows_num(FBPart::DATA)    <= 0"); return;}
        if(m_fb.get_number_rows(FBPart::TRAILER) != 1) {wxLogMessage("m_fb.get_rows_num(FBPart::TRAILER) != 1"); return;}
        if(m_fb.get_number_rows(FBPart::END)     != 1) {wxLogMessage("m_fb.get_rows_num(FBPart::END)     != 1"); return;}

        if(!m_fb.save_file(fdialog.GetPath().ToStdString(wxCSConv("cp932"))))
        {
            wxLogMessage("m_fb.save_file(fdialog.GetPath().ToStdString(wxCSConv(\"cp932\"))) : false");
        }
    };

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnSaveAs()
        );

    }, ID_MENU_SAVEAS);

     // EXIT
    auto OnExit = [=]()
    {
        if(is_edited_any())
        {
            wxMessageDialog mdialog(frame, "編集中のデータが失われます\r\n終了しますか？", "確認", wxOK | wxCANCEL);
            if(mdialog.ShowModal() == wxID_CANCEL) return;
        }

        frame->Destroy();
    };

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnExit()
        );

    }, ID_MENU_EXIT);

    F()->Bind(wxEVT_CLOSE_WINDOW, [=](wxCloseEvent& event)
    {
        _do_callback_
        (
            OnExit()
        );
    });


    auto OnHeaderImport = [=]()
    {
        if(F()->get_grid_header()->GetNumberRows() != 1)
        {
            wxLogMessage("F()->get_grid_header()->GetNumberRows() != 1");
            return;
        }

        wxFileDialog fdialog(F(), "プリセット選択", m_preset_path, wxEmptyString, "PRESET files (*.preset)|*.preset");
        if(fdialog.ShowModal() == wxID_CANCEL) return;

        if(!m_fb.open_file(fdialog.GetPath().ToStdString(wxCSConv("cp932"))))
        {
            wxLogMessage("m_fb.open_file(fdialog.GetPath().ToStdString(wxCSConv(\"cp932\"))) : false");
            return;
        }

        if(m_fb.get_number_rows(FBPart::HEADER)  != 1) {wxLogMessage("m_fb.get_rows_num(FBPart::HEADER)  != 1"); return;}
        if(m_fb.get_number_rows(FBPart::DATA)    != 0) {wxLogMessage("m_fb.get_rows_num(FBPart::DATA)    != 0"); return;}
        if(m_fb.get_number_rows(FBPart::TRAILER) != 0) {wxLogMessage("m_fb.get_rows_num(FBPart::TRAILER) != 0"); return;}
        if(m_fb.get_number_rows(FBPart::END)     != 0) {wxLogMessage("m_fb.get_rows_num(FBPart::END)     != 0"); return;}

        const int torikumibi_col = 5; 
        auto value = F()->get_grid_header()->GetCellValue(0, torikumibi_col);

        FBGrid fbgrid(m_fb);
        grid2grid(&fbgrid.set_fbpart(FBPart::HEADER) ,F()->get_grid_header()->GetTable() );

        F()->get_grid_header()->SetCellValue(0, torikumibi_col, value);

    };

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnHeaderImport()
        );

    }, ID_MENU_HEADER_IMPORT);

    F()->get_button_header_import()->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnHeaderImport()
        );
    });

    auto OnHeaderExport = [=]()
    {
        F()->save_editing_value();

        m_fb.set_newline(FBNewLine::NONE);

        wxFileDialog fdialog(F(), "プリセット保存", m_preset_path, wxEmptyString, "PRESET files (*.preset)|*.preset", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if(fdialog.ShowModal() == wxID_CANCEL) return;

        FBGrid fbgrid(m_fb);
        grid2grid(F()->get_grid_header()->GetTable() , &fbgrid.set_fbpart(FBPart::HEADER) );
        m_fb.assign_rows(0, FBPart::DATA);
        m_fb.assign_rows(0, FBPart::TRAILER);
        m_fb.assign_rows(0, FBPart::END);

        if(m_fb.get_number_rows(FBPart::HEADER)  != 1) {wxLogMessage("m_fb.get_rows_num(FBPart::HEADER)  != 1"); return;}
        if(m_fb.get_number_rows(FBPart::DATA)    != 0) {wxLogMessage("m_fb.get_rows_num(FBPart::DATA)    != 0"); return;}
        if(m_fb.get_number_rows(FBPart::TRAILER) != 0) {wxLogMessage("m_fb.get_rows_num(FBPart::TRAILER) != 0"); return;}
        if(m_fb.get_number_rows(FBPart::END)     != 0) {wxLogMessage("m_fb.get_rows_num(FBPart::END)     != 0"); return;}

        if(!m_fb.save_file(fdialog.GetPath().ToStdString(wxCSConv("cp932"))))
        {
            wxLogMessage("m_fb.save_file(fdialog.GetPath().ToStdString(wxCSConv(\"cp932\"))) : false");
        }
    
    };

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnHeaderExport()
        );

    }, ID_MENU_HEADER_EXPORT);

    F()->get_button_header_export()->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnHeaderExport()
            
        );
    });


    // DATA ADD
    auto DataAdd = [=]()
    {
        auto grid = F()->get_grid_data();
        grid->insert_selected();

    };

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        _do_callback_
        (
            DataAdd()
        );

    }, ID_MENU_DATA_ADD);

    F()->get_button_data_add()->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
    {
        _do_callback_
        (
            DataAdd()
        );
    });


    // DATA DELETE
    auto DataDelete = [=]()
    {
        auto grid = F()->get_grid_data();
        grid->delete_selected();

    };

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        _do_callback_
        (
            DataDelete()
        ); 

    }, ID_MENU_DATA_DELETE);

    F()->get_button_data_delete()->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
    {
        _do_callback_
        (
            DataDelete()
        );          
    });

    // SEARCH BOX
    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        _do_callback_
        (
            F()->get_searchctrl_data_search()->SetFocus()
        );

    }, ID_MENU_DATA_SEARCH_BOX);

    // SEARCH FORWARD
    auto DataSearchForward = [=]()
    {
        auto grid = F()->get_grid_data();
        auto search_value = F()->get_searchctrl_data_search()->GetValue();
        auto is_forward = true;
        
        grid->search_next_value(search_value, is_forward);
    };

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        _do_callback_
        (
            DataSearchForward()
        );

    }, ID_MENU_DATA_SEARCH_FORWARD);

    F()->get_searchctrl_data_search()->Bind(wxEVT_SEARCH, [=](wxCommandEvent& event)
    {
        _do_callback_
        (
            DataSearchForward()
        );
    });    

    F()->get_button_data_search_forward()->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
    {
        _do_callback_
        (
            DataSearchForward()
        );
    });

    // SEARCH BACKWARD
    auto DataSearchBackward = [=]()
    {
        auto grid = F()->get_grid_data();
        auto search_value = F()->get_searchctrl_data_search()->GetValue();
        auto is_forward = false;
        
        grid->search_next_value(search_value, is_forward);
    };

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        _do_callback_
        (
            DataSearchBackward()
        );

    }, ID_MENU_DATA_SEARCH_BACKWARD);

    F()->get_button_data_search_backward()->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
    {
        _do_callback_
        (
            DataSearchBackward()
        );
    });

    // TRAILER RECALCULATE
    auto OnTrailerRecalculate = [=](wxGrid* grid_data, FBAttrs fb_data_attrs, wxGrid* grid_trailer, FBAttrs fb_trailer_attrs)
    {
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
            auto& attr = fb_trailer_attrs[trailer_kensu_col];

            if(value.size() > attr.length)
            {
                wxLogMessage("value.size() > length");
                return;
            }
            
            if(attr.pad_info[0] == 'R')
            {
                grid_trailer->SetCellValue(0, attr.num, value.append(attr.length - value.length(), attr.pad_info[1]));
            }
            else if(attr.pad_info[0] == 'L')
            {
                grid_trailer->SetCellValue(0, attr.num, value.insert(0, attr.length - value.length(), attr.pad_info[1]));
            }            
        }

        {
            auto& value = sum_kingaku_str;
            auto& attr = fb_trailer_attrs[trailer_kingaku_col];

            if(value.size() > attr.length)
            {
                wxLogMessage("value.size() > length");
                return;
            }
            
            if(attr.pad_info[0] == 'R')
            {
                grid_trailer->SetCellValue(0, attr.num, value.append(attr.length - value.length(), attr.pad_info[1]));
            }
            else if(attr.pad_info[0] == 'L')
            {
                grid_trailer->SetCellValue(0, attr.num, value.insert(0, attr.length - value.length(), attr.pad_info[1]));
            }            
        }

    };

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnTrailerRecalculate(F()->get_grid_data(), m_fb.get_attrs(FBPart::DATA), F()->get_grid_trailer(), m_fb.get_attrs(FBPart::TRAILER))
        );

    }, ID_MENU_TRAILER_RECALCULATE);

    F()->get_button_trailer_recalculated()->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnTrailerRecalculate(F()->get_grid_data(), m_fb.get_attrs(FBPart::DATA), F()->get_grid_trailer(), m_fb.get_attrs(FBPart::TRAILER))
        );
    });

    // ABOUT
    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        _do_callback_
        (
            myAboutDialogInfo().myAboutBox(F())
        );

    }, ID_MENU_HELP_ABOUT);

}


