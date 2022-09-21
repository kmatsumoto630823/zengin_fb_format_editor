#include "InitialController.h"

#include "view/custom/myAboutDialogInfo.h"
#include "model/FBParser.h"

#include <wx/clipbrd.h>
#include <wx/progdlg.h>
#include <wx/log.h>


#define F() (InitialController::get_casted_frame())
#define _do_callback_(collback) do {F()->Freeze(); (collback); F()->force_refresh(); F()->Thaw();} while(0)


class FBGrid
{
public:
    FBGrid(FBParser& fb) : m_fb(fb){};
    ~FBGrid(){};

    FBGrid& set_current_part(FBPart part)
    {
        m_fb.set_current_part(part);
        return *this;
    }

    //Adapter Function (like wxGridTableBase)
    int GetNumberRows() const { return m_fb.get_number_rows(); };
    int GetNumberCols() const { return m_fb.get_number_cols(); };

    const wxString& GetValue(int row, int col) const
    {
        auto value = m_fb.get_value(row, col);
        m_wxstring_buff.assign(value.data(), value.size());

        return m_wxstring_buff;
    };

    bool SetValue(int row, int col, const wxString& value)
    {
        m_string_buff = value;
        return m_fb.set_value(row, col, m_string_buff);
    };

    bool AppendRows(int numRows = 1){ return m_fb.append_rows(numRows); };
    bool InsertRows(int pos = 0, int numRows = 1){ return m_fb.insert_rows(pos, numRows); };
    bool DeleteRows(int pos = 0, int numRows = 1){ return m_fb.delete_rows(pos, numRows); };

private:
    FBParser& m_fb;

    static wxString m_wxstring_buff;
    static std::string m_string_buff;
};

wxString FBGrid::m_wxstring_buff;
std::string FBGrid::m_string_buff;

template<typename T, typename U>
inline void copy_grid_to_grid(T* src, U* dst)
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

    if(dst->GetNumberRows() != 0)
    {
        dst->DeleteRows(0, dst->GetNumberRows());
    }

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

/*
template<typename T, typename U>
inline void copy_grid_to_grid(T* src, U* dst, wxProgressDialog* dlg)
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

    if(dst->GetNumberRows() != 0)
    {
        dst->DeleteRows(0, dst->GetNumberRows());
    }
    
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
*/

/*
template<typename T, typename U>
inline void add_grid_to_grid(T* src, U* dst)
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
    auto offset_rows = dst->GetNumberRows();

    dst->AppendRows(num_rows);

    for(decltype(num_rows) row = 0; row < num_rows; row++)  
    {
        for(decltype(num_cols) col = 0; col < num_cols; col++)
        {
            auto&& value = src->GetValue(row, col);
            dst->SetValue(row + offset_rows, col, value);
        }
    }
}
*/

template<typename T, typename U, typename N>
inline void insert_grid_to_grid(T* src, U* dst, N pos)
{
    static_assert(std::is_integral_v<N>, "Integral required.");

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

    if(pos > dst->GetNumberRows())
    {
        wxLogMessage("pos > dst->GetNumberRows()");
        return;
    }

    auto offset_rows = pos;

    dst->InsertRows(offset_rows, num_rows);

    for(decltype(num_rows) row = 0; row < num_rows; row++)  
    {
        for(decltype(num_cols) col = 0; col < num_cols; col++)
        {
            auto&& value = src->GetValue(row, col);
            dst->SetValue(row + offset_rows, col, value);
        }
    }
}

void InitialController::switch_type(FBType type)
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

    m_attrs_array = make_attrs_array
    (
        type,
        m_chars_kana.data(),
        m_pad_kana.data(),
        m_chars_num.data(),
        m_pad_num.data()
    );

    using enum_int_t = std::underlying_type_t<FBPart>;

    F()->get_grid_header() ->reset(m_attrs_array.at((enum_int_t)FBPart::HEADER ));
    F()->get_grid_data()   ->reset(m_attrs_array.at((enum_int_t)FBPart::DATA   ));
    F()->get_grid_trailer()->reset(m_attrs_array.at((enum_int_t)FBPart::TRAILER)); 
    F()->get_grid_end()    ->reset(m_attrs_array.at((enum_int_t)FBPart::END    )); 

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

    //m_fbdata_path = config.Read("FBDATA_PATH", wxEmptyString);
    //m_preset_path = config.Read("PRESET_PATH", wxEmptyString);
    //m_export_path = config.Read("EXPORT_PATH", wxEmptyString);

    m_chars_kana = config.Read("CHARS_KANA", wxEmptyString).ToStdString(wxCSConv("cp932"));
    m_pad_kana   = config.Read("PAD_KANA",   wxEmptyString).ToStdString(wxCSConv("cp932"));
    m_chars_num  = config.Read("CHARS_NUM",  wxEmptyString).ToStdString(wxCSConv("cp932"));
    m_pad_num    = config.Read("PAD_NUM",    wxEmptyString).ToStdString(wxCSConv("cp932"));

    create_frame(config);
    create_binds(config);

    switch_type((FBType)config.Read("FBTYPE", (long)FBType::SOHFURI));
}

void InitialController::create_frame(wxFileConfig& config)
{
    this->frame = new InitialFrame();

    this->frame->SetTitle(config.Read("APP_NAME", wxEmptyString));

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
    auto OnNew = [=, this]()
    {
        if(is_edited_any())
        {
            wxMessageDialog mdialog(F(), "編集中のデータが失われます\r\n続行しますか", "確認", wxOK | wxCANCEL);
            if(mdialog.ShowModal() == wxID_CANCEL) return;
        }

        using enum_int_t = std::underlying_type_t<FBType>;

        wxString selection_strings[(enum_int_t)FBType::ITEM_NUM];
        selection_strings[(enum_int_t)FBType::SOHFURI] = "総合振込";
        selection_strings[(enum_int_t)FBType::KYUYO_SHOYO] = "給与・賞与振込";
        selection_strings[(enum_int_t)FBType::FURIKAE] = "預金口座振替";

        wxSingleChoiceDialog scdialog(F(), "フォーマットを選択してください", "選択", std::size(selection_strings) , selection_strings);
        scdialog.SetSelection((enum_int_t)FBType::SOHFURI);

        if(scdialog.ShowModal() == wxID_CANCEL) return;

        auto type = (FBType) scdialog.GetSelection();
        switch_type(type);
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_(
            OnNew()
        );

    }, ID_MENU_NEW);

    // OPEN
    auto OnOpen = [=, this](const wxString& default_path)
    {
        auto path = default_path;

        if(is_edited_any())
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

        FBGrid fbgrid(fb);  
        //wxProgressDialog pdialog("進捗", " ", 100, F());

        copy_grid_to_grid(&fbgrid.set_current_part(FBPart::HEADER)  ,F()->get_grid_header()->GetTable() );
        //copy_grid_to_grid(&fbgrid.set_current_part(FBPart::DATA)    ,F()->get_grid_data()->GetTable()   , &pdialog);
        copy_grid_to_grid(&fbgrid.set_current_part(FBPart::DATA)    ,F()->get_grid_data()->GetTable()   );
        copy_grid_to_grid(&fbgrid.set_current_part(FBPart::TRAILER) ,F()->get_grid_trailer()->GetTable());
        copy_grid_to_grid(&fbgrid.set_current_part(FBPart::END)     ,F()->get_grid_end()->GetTable()    );

    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnOpen("")
        );
        
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
        
        _do_callback_
        (
            OnOpen(event.GetFiles()[0])
        );
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

        static const wxString selection_strings[] =
        {
            "CRLF(デフォルト)",
            "CR",
            "LF",
            "改行なし"
        };

        using enum_int_t = std::underlying_type_t<FBNewLine>;
        
        wxSingleChoiceDialog scdialog(F(), "改行コードを選択してください", "選択", std::size(selection_strings), selection_strings);
        scdialog.SetSelection((enum_int_t)FBNewLine::CRLF);

        if(scdialog.ShowModal() == wxID_CANCEL) return;

        auto newline = scdialog.GetSelection();
        FBParser fb(m_attrs_array);
        fb.set_newline((FBNewLine)newline);
        
        wxFileDialog fdialog(F(), "FBデータの保存", ".", wxEmptyString, "TXT files (*.txt)|*.txt|All files (*)|*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if(fdialog.ShowModal() == wxID_CANCEL) return;

        FBGrid fbgrid(fb);

        copy_grid_to_grid(F()->get_grid_header()->GetTable() , &fbgrid.set_current_part(FBPart::HEADER) );
        copy_grid_to_grid(F()->get_grid_data()->GetTable()   , &fbgrid.set_current_part(FBPart::DATA)   );
        copy_grid_to_grid(F()->get_grid_trailer()->GetTable(), &fbgrid.set_current_part(FBPart::TRAILER));
        copy_grid_to_grid(F()->get_grid_end()->GetTable()    , &fbgrid.set_current_part(FBPart::END)    );

        if(fb.get_number_rows(FBPart::HEADER)  != 1) {wxLogMessage("fb.get_number_rows(FBPart::HEADER)  != 1"); return;}
        if(fb.get_number_rows(FBPart::DATA)    <= 0) {wxLogMessage("fb.get_number_rows(FBPart::DATA)    <= 0"); return;}
        if(fb.get_number_rows(FBPart::TRAILER) != 1) {wxLogMessage("fb.get_number_rows(FBPart::TRAILER) != 1"); return;}
        if(fb.get_number_rows(FBPart::END)     != 1) {wxLogMessage("fb.get_number_rows(FBPart::END)     != 1"); return;}

        auto path = fdialog.GetPath().ToStdString(wxCSConv("cp932"));

        if(!fb.save_file(path))
        {
            wxLogMessage("fb.save_file(path)");
            wxLogMessage("ファイルの保存に失敗しました");
        }
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnSaveAs()
        );

    }, ID_MENU_SAVEAS);

     // EXIT
    auto OnExit = [=, this]()
    {
        if(is_edited_any())
        {
            wxMessageDialog mdialog(frame, "編集中のデータが失われます\r\n終了しますか？", "確認", wxOK | wxCANCEL);
            if(mdialog.ShowModal() == wxID_CANCEL) return;
        }

        frame->Destroy();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnExit()
        );

    }, ID_MENU_EXIT);

    F()->Bind(wxEVT_CLOSE_WINDOW, [=, this](wxCloseEvent& event)
    {
        _do_callback_
        (
            OnExit()
        );
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

        wxFileDialog fdialog(F(), "＠ヘッダ：ファイル読込", ".", wxEmptyString, "TXT files (*.txt)|*.txt|All files (*)|*");
        if(fdialog.ShowModal() == wxID_CANCEL) return;

        FBParser fb(m_attrs_array);
        auto path = fdialog.GetPath().ToStdString(wxCSConv("cp932"));

        if(!fb.open_file(path))
        {
            wxLogMessage("fb.open_file(path) : false");
            wxLogMessage("ファイルの読込に失敗しました");
            return;
        }

        if(fb.get_number_rows(FBPart::HEADER)  != 1) {wxLogMessage("fb.get_number_rows(FBPart::HEADER)  != 1"); return;}
        if(fb.get_number_rows(FBPart::DATA)    != 0) {wxLogMessage("fb.get_number_rows(FBPart::DATA)    != 0"); return;}
        if(fb.get_number_rows(FBPart::TRAILER) != 0) {wxLogMessage("fb.get_number_rows(FBPart::TRAILER) != 0"); return;}
        if(fb.get_number_rows(FBPart::END)     != 0) {wxLogMessage("fb.get_number_rows(FBPart::END)     != 0"); return;}

        const int torikumibi_col = 5; 
        auto value = grid->GetCellValue(0, torikumibi_col);

        FBGrid fbgrid(fb);
        fbgrid.set_current_part(FBPart::HEADER);

        copy_grid_to_grid(&fbgrid , grid->GetTable());

        grid->SetCellValue(0, torikumibi_col, value);

        grid->SelectRow(0, true);
        grid->GoToCell(0, 0);
        grid->SetFocus();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnHeaderImport()
        );

    }, ID_MENU_HEADER_IMPORT);

    F()->get_button_header_import()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnHeaderImport()
        );
    });

    // HEADER EXPORT
    auto OnHeaderExport = [=, this]()
    {
        auto grid = F()->get_grid_header();

        F()->save_editing_value();

        FBParser fb(m_attrs_array);
        fb.set_newline(FBNewLine::NONE);

        wxFileDialog fdialog(F(), "＠ヘッダ：ファイル出力", ".", wxEmptyString, "TXT files (*.txt)|*.txt|All files (*)|*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if(fdialog.ShowModal() == wxID_CANCEL) return;

        FBGrid fbgrid(fb);
        fbgrid.set_current_part(FBPart::HEADER);

        copy_grid_to_grid(grid->GetTable() , &fbgrid);
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
        _do_callback_
        (
            OnHeaderExport()
        );

    }, ID_MENU_HEADER_EXPORT);

    F()->get_button_header_export()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnHeaderExport()
        );
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

        if(fb.get_number_rows(FBPart::HEADER)  != 1) {wxLogMessage("fb.get_number_rows(FBPart::HEADER)  != 1"); return;}
        if(fb.get_number_rows(FBPart::DATA)    != 0) {wxLogMessage("fb.get_number_rows(FBPart::DATA)    != 0"); return;}
        if(fb.get_number_rows(FBPart::TRAILER) != 0) {wxLogMessage("fb.get_number_rows(FBPart::TRAILER) != 0"); return;}
        if(fb.get_number_rows(FBPart::END)     != 0) {wxLogMessage("fb.get_number_rows(FBPart::END)     != 0"); return;}

        wxMessageDialog mdialog(F(), "クリップボードの内容をヘッダレコードに読込します", "確認", wxOK | wxCANCEL);
        if(mdialog.ShowModal() == wxID_CANCEL) return;

        const int torikumibi_col = 5; 
        auto value = grid->GetCellValue(0, torikumibi_col);

        FBGrid fbgrid(fb);
        fbgrid.set_current_part(FBPart::HEADER);
        
        copy_grid_to_grid(&fbgrid, grid->GetTable() );

        grid->SetCellValue(0, torikumibi_col, value);

        grid->SelectRow(0, true);
        grid->GoToCell(0, 0);
        grid->SetFocus();

    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnHeaderImportFromClipBoard()
        );

    }, ID_MENU_HEADER_IMPORT_FROM_CLIPBOARD);

    F()->get_button_header_import_from_clipboard()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnHeaderImportFromClipBoard()
        );
    });

    // DATA SELECT ALL
    auto OnDataSelectAll = [=, this]()
    {
        auto grid = F()->get_grid_data();
        grid->SelectAll();
        grid->SetFocus();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnDataSelectAll()
        );

    }, ID_MENU_DATA_SELECT_ALL);

    F()->get_button_data_select_all()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnDataSelectAll()
        );
    });

    // DATA CLEAR SELECTED
    auto OnDataClearSelected = [=, this]()
    {
        auto grid = F()->get_grid_data();
        grid->ClearSelection();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnDataClearSelected()
        );

    }, ID_MENU_DATA_CLEAR_SELECTED);

    F()->get_button_data_clear_selected()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnDataClearSelected()
        );
    });

    // DATA ADD
    auto OnDataAdd = [=, this]()
    {
        auto grid = F()->get_grid_data();
        grid->insert_selected();

    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnDataAdd()
        );

    }, ID_MENU_DATA_ADD);

    F()->get_button_data_add()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnDataAdd()
        );
    });


    // DATA DELETE
    auto OnDataDelete = [=, this]()
    {
        auto grid = F()->get_grid_data();
        grid->delete_selected();

    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnDataDelete()
        ); 

    }, ID_MENU_DATA_DELETE);

    F()->get_button_data_delete()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnDataDelete()
        );          
    });

    // DATA ADD FROM CRIPBOARD
    auto OnDataAddFromClipBoard = [=, this]()
    {
        auto grid = F()->get_grid_data();

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

        if(fb.get_number_rows(FBPart::HEADER)  != 0) {wxLogMessage("fb.get_number_rows(FBPart::HEADER)  != 0"); return;}
        if(fb.get_number_rows(FBPart::DATA)    <= 0) {wxLogMessage("fb.get_number_rows(FBPart::DATA)    <= 0"); return;}
        if(fb.get_number_rows(FBPart::TRAILER) != 0) {wxLogMessage("fb.get_number_rows(FBPart::TRAILER) != 0"); return;}
        if(fb.get_number_rows(FBPart::END)     != 0) {wxLogMessage("fb.get_number_rows(FBPart::END)     != 0"); return;}

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
 
        auto message = wxString::Format("クリップボードの内容（%d行）をレコード【%d】の位置に挿入します", fb.get_number_rows(FBPart::DATA), pos + 1);
        wxMessageDialog mdialog(F(), message, "確認", wxOK | wxCANCEL);
        if(mdialog.ShowModal() == wxID_CANCEL) return;

        FBGrid fbgrid(fb);
        fbgrid.set_current_part(FBPart::DATA);

        insert_grid_to_grid(&fbgrid, grid->GetTable(), pos);
        
        grid->ClearSelection();

        auto begin = pos;
        auto end = begin + fbgrid.GetNumberRows();
        for(auto row = begin; row < end; ++row)
        {
            grid->SelectRow(row, true);
        }

        grid->GoToCell(end - 1, 0);
        grid->SetFocus();
    };

    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnDataAddFromClipBoard()
        );

    }, ID_MENU_DATA_ADD_FROM_CLIPBOARD);

    F()->get_button_data_add_from_clipboard()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnDataAddFromClipBoard()
        );
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
        _do_callback_
        (
            OnDataSearchForward()
        );

    }, ID_MENU_DATA_SEARCH_FORWARD);

    F()->get_searchctrl_data_search()->Bind(wxEVT_SEARCH, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnDataSearchForward()
        );
    });    

    F()->get_button_data_search_forward()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnDataSearchForward()
        );
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
        _do_callback_
        (
            OnDataSearchBackward()
        );

    }, ID_MENU_DATA_SEARCH_BACKWARD);

    F()->get_button_data_search_backward()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnDataSearchBackward()
        );
    });

    // TRAILER RECALCULATE
    auto OnTrailerRecalculate = [=, this]()
    {
        using enum_int_t = std::underlying_type_t<FBPart>;
        
        auto grid_data = F()->get_grid_data();
        auto attrs_data = m_attrs_array.at((enum_int_t)FBPart::DATA);
        auto grid_trailer = F()->get_grid_trailer();
        auto attrs_trailer = m_attrs_array.at((enum_int_t)FBPart::TRAILER);

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
        _do_callback_
        (
            OnTrailerRecalculate()
        );

    }, ID_MENU_TRAILER_RECALCULATE);

    F()->get_button_trailer_recalculated()->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            OnTrailerRecalculate()
        );
    });

    // SHOW ABOUT
    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            myAboutDialogInfo().myAboutBox(F())
        );

    }, ID_MENU_HELP_ABOUT);

    // SHOW MANUAL
    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            wxExecute("notepad.exe 簡易操作マニュアル.txt")
        );

    }, ID_MENU_HELP_MANUAL);

    // SHOW README
    F()->Bind(wxEVT_MENU, [=, this](wxCommandEvent& event)
    {
        _do_callback_
        (
            wxExecute("notepad.exe 最初にお読みください.txt")
        );

    }, ID_MENU_HELP_README);

}