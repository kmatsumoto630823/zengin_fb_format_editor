#include "InitialController.h"

#include <wx/log.h>

#define F() (get_casted_frame())

void InitialController::reset_grid()
{
    auto lambda_reset_grid = [](wxGrid * grid, const FBInfo &fb_info)
    {
        if(auto h = grid->GetNumberRows(); h > 0) grid->DeleteRows(0, h);
        if(auto w = grid->GetNumberCols(); w > 0) grid->DeleteCols(0, w);
    
        if(grid->GetNumberRows() != 0 || grid->GetNumberCols() != 0)
        {
            wxLogMessage("grid->GetNumberRows() != 0 || grid->GetNumberCols() != 0");
            return;
        }

        grid->AppendCols(fb_info.size());

        for(auto &[num, label, offset, length, char_includes, pad_info, initial_value, description] : fb_info)
        {
            auto col = num;

            grid->SetColLabelValue(col, label);
            auto col_attr = new wxGridCellAttr;
            auto col_editor = new trimGridCellTextEditor(length);

            if(char_includes != nullptr)col_editor->SetValidString(char_includes);
            if(label != nullptr && description != nullptr) col_editor->SetTipString(label, description);

            col_attr->SetEditor(col_editor);
            grid->SetColAttr(col, col_attr);    
        }

        grid->AppendRows();

        if(fb_info.size() > grid->GetNumberCols())
        {
            wxLogMessage("fb_info.size() > grid->GetNumberCols()");
            return;
        }

        for(auto &[num, label, offset, length, char_includes, pad_info, initial_value, description] : fb_info)
        {
           auto col = num;
           auto value = wxString(length, pad_info[1]);

            if(initial_value != nullptr) value = initial_value;

            grid->SetCellValue(grid->GetNumberRows() - 1, col, value);
        }

        auto row = grid->GetNumberRows() - 1;
        auto col = grid->GetGridCursorCol();

        grid->ClearSelection();
        grid->GoToCell(row, col);
        grid->SetFocus();        

        grid->AutoSize();
    };


    lambda_reset_grid(F()->get_grid_header(),  fb.get_fb_header_info());
    lambda_reset_grid(F()->get_grid_data(),    fb.get_fb_data_info());
    lambda_reset_grid(F()->get_grid_trailer(), fb.get_fb_trailer_info());
    lambda_reset_grid(F()->get_grid_end(),     fb.get_fb_end_info());
}

void InitialController::create_frame()
{
    this->frame = new InitialFrame();

    F()->get_button_header_export()->Hide();

    F()->get_grid_header()->CreateGrid (0, 0, wxGrid::wxGridSelectRowsOrColumns);
    F()->get_grid_data()->CreateGrid   (0, 0, wxGrid::wxGridSelectRowsOrColumns);
    F()->get_grid_trailer()->CreateGrid(0, 0, wxGrid::wxGridSelectRowsOrColumns);
    F()->get_grid_end()->CreateGrid    (0, 0, wxGrid::wxGridSelectRowsOrColumns);

    reset_grid();

    Controller::frame->SetSize(wxSize(1600, 900));
}

void InitialController::create_binds()
{
    auto LambdaFBPaserRead = [](wxGrid *grid, const FBInfo &fb_info, auto fb, auto get_fb_value, auto get_fb_row_size)
    {
        if(grid->GetNumberRows() != 0) grid->DeleteRows(0, grid->GetNumberRows());
        grid->AppendRows((fb->*get_fb_row_size)());

        for(int row = 0; row < (fb->*get_fb_row_size)(); row++)
        {
            for(auto &[num, label, offset, length, char_includes, pad_info, initial_value, description] : fb_info)
            {
                auto col = num;
                grid->SetCellValue(row, col, wxString((fb->*get_fb_value)(row, col).data(), length));
            }
        }
    };

    auto LambdaFBPaserWrite = [](wxGrid *grid, const FBInfo &fb_info, auto fb, auto assign_fb_line, auto set_fb_value)
    {
        auto row_size = grid->GetNumberRows();
        (fb->*assign_fb_line)(row_size);

        for(int row = 0; row < row_size; row++)
        {
            for(auto &[num, label, offset, length, char_includes, pad_info, initial_value, description] : fb_info)
            {
                auto col = num;
                auto value = grid->GetCellValue(row, col);
                (fb->*set_fb_value)(row, col, value.ToStdString());
            }
        }
    };

    // NEW
    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        wxMessageDialog mdialog(F(), "編集中のデータが失われます\r\n続行しますか", "確認", wxOK | wxCANCEL);
        if(mdialog.ShowModal() == wxID_CANCEL) return;
        
        reset_grid();
        
    }, ID_MENU_NEW);

    // OPEN
    auto LambdaOnOpen = [=](const wxString &default_path)
    {
        auto path = default_path;

        wxMessageDialog mdialog(F(), "編集中のデータが失われます\r\n続行しますか？", "確認", wxOK | wxCANCEL);
        if(mdialog.ShowModal() == wxID_CANCEL) return;

        if(path.empty())
        {
            wxFileDialog fdialog(F(), "FBデータの選択",wxEmptyString, wxEmptyString, "TXT files (*.txt)|*.txt");
            if(fdialog.ShowModal() == wxID_CANCEL) return;

            path = fdialog.GetPath();
        }

        if(!fb.open_fb_file(path.ToStdString()))
        {
            wxLogMessage("fb.open_fb_file(fdialog.GetPath().ToStdString()) : false");
            return;
        }

        if(fb.get_fb_header_row_size()  != 1) {wxLogMessage("fb.get_fb_header_row_size()  != 1"); return;}
        if(fb.get_fb_data_row_size()    <= 0) {wxLogMessage("fb.get_fb_data_row_size()    <= 0"); return;}
        if(fb.get_fb_trailer_row_size() != 1) {wxLogMessage("fb.get_fb_trailer_row_size() != 1"); return;}
        if(fb.get_fb_end_row_size()     != 1) {wxLogMessage("fb.get_fb_end_row_size()     != 1"); return;}

        LambdaFBPaserRead(F()->get_grid_header(),  fb.get_fb_header_info(),  &fb, &FBDataParser::get_fb_header_value,  &FBDataParser::get_fb_header_row_size);
        LambdaFBPaserRead(F()->get_grid_data(),    fb.get_fb_data_info(),    &fb, &FBDataParser::get_fb_data_value,    &FBDataParser::get_fb_data_row_size);
        LambdaFBPaserRead(F()->get_grid_trailer(), fb.get_fb_trailer_info(), &fb, &FBDataParser::get_fb_trailer_value, &FBDataParser::get_fb_trailer_row_size);
        LambdaFBPaserRead(F()->get_grid_end(),     fb.get_fb_end_info(),     &fb, &FBDataParser::get_fb_end_value,     &FBDataParser::get_fb_end_row_size);
    };

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        LambdaOnOpen("");

    }, ID_MENU_OPEN);

    F()->DragAcceptFiles(true);
    F()->Bind(wxEVT_DROP_FILES, [=](wxDropFilesEvent& event)
    {
        if(event.GetNumberOfFiles() != 1){
            wxLogMessage("event.GetNumberOfFiles() != 1");
            return;
        }

        LambdaOnOpen(event.GetFiles()[0]);
    });

    // SAVEAS
    auto LambdaOnSaveAs = [=]()
    {
        F()->get_grid_header()->SaveEditControlValue();
        F()->get_grid_data()->SaveEditControlValue();
        F()->get_grid_trailer()->SaveEditControlValue();
        F()->get_grid_end()->SaveEditControlValue();

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
            wxMessageDialog mdialog(F(), "データレコードとトレーラーレコードの合計件数、合計金額が一致しません\r\n続行しますか？", "確認", wxOK | wxCANCEL);
            if(mdialog.ShowModal() == wxID_CANCEL) return;
        }
        
        enum {
            NEWLINE_CRLF = 0,
            NEWLINE_CR = 1,
            NEWLINE_LF = 2,
            NEWLINE_NONE =3
        };
        
        const wxString newline_codes[] = {
            "\r\n",
            "\r",
            "\n",
            ""
        };

        const wxString newline_code_selection_strings[] = {
            "CRLF(デフォルト)",
            "CR",
            "LF",
            "改行なし"
        };

        
        wxSingleChoiceDialog scdialog(F(), "改行コードを選択してください", "選択", std::size(newline_code_selection_strings) , newline_code_selection_strings);
        scdialog.SetSelection(NEWLINE_CRLF);

        if(scdialog.ShowModal() == wxID_CANCEL) return;

        auto &code = newline_codes[scdialog.GetSelection()];
        fb.set_fb_newline_code(code.ToStdString());
        
        wxFileDialog fdialog(F(), "FBデータの保存", wxEmptyString, wxEmptyString, "TXT files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if(fdialog.ShowModal() == wxID_CANCEL) return;

        LambdaFBPaserWrite(F()->get_grid_header(),  fb.get_fb_header_info(),  &fb, &FBDataParser::assign_fb_header_line,  &FBDataParser::set_fb_header_value);
        LambdaFBPaserWrite(F()->get_grid_data(),    fb.get_fb_data_info(),    &fb, &FBDataParser::assign_fb_data_line,    &FBDataParser::set_fb_data_value);
        LambdaFBPaserWrite(F()->get_grid_trailer(), fb.get_fb_trailer_info(), &fb, &FBDataParser::assign_fb_trailer_line, &FBDataParser::set_fb_trailer_value);
        LambdaFBPaserWrite(F()->get_grid_end(),     fb.get_fb_end_info(),     &fb, &FBDataParser::assign_fb_end_line,     &FBDataParser::set_fb_end_value);

        if(fb.get_fb_header_row_size()  != 1) {wxLogMessage("fb.get_fb_header_row_size()  != 1"); return;}
        if(fb.get_fb_data_row_size()    <= 0) {wxLogMessage("fb.get_fb_data_row_size()    <= 0"); return;}
        if(fb.get_fb_trailer_row_size() != 1) {wxLogMessage("fb.get_fb_trailer_row_size() != 1"); return;}
        if(fb.get_fb_end_row_size()     != 1) {wxLogMessage("fb.get_fb_end_row_size()     != 1"); return;}

        if(!fb.save_fb_file(fdialog.GetPath().ToStdString()))
        {
            wxLogMessage("fb.save_fb_file(fdialog.GetPath().ToStdString()) : false");
        }
    };

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        LambdaOnSaveAs();
        F()->Refresh();

    }, ID_MENU_SAVEAS);

     // EXIT
    auto LambdaOnExit = [](wxFrame *frame)
    {
        wxMessageDialog mdialog(frame, "編集中のデータが失われます\r\n終了しますか？", "確認", wxOK | wxCANCEL);
        if(mdialog.ShowModal() == wxID_CANCEL) return;

        frame->Destroy();
    };

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        LambdaOnExit(F());

    }, ID_MENU_EXIT);

    F()->Bind(wxEVT_CLOSE_WINDOW, [=](wxCloseEvent& event)
    {
        LambdaOnExit(F());
    });


    auto LambdaOnHeaderImport = [=]()
    {
        if(F()->get_grid_header()->GetNumberRows() != 1)
        {
            wxLogMessage("F()->get_grid_header()->GetNumberRows() != 1");
            return;
        }

        wxFileDialog fdialog(F(), "プリセット選択", "./preset/", wxEmptyString, "PRESET files (*.preset)|*.preset");
        if(fdialog.ShowModal() == wxID_CANCEL) return;

        if(!fb.open_fb_file(fdialog.GetPath().ToStdString()))
        {
            wxLogMessage("fb.open_fb_file(fdialog.GetPath().ToStdString()) : false");
            return;
        }

        if(fb.get_fb_header_row_size()  != 1) {wxLogMessage("fb.get_fb_header_row_size()  != 1"); return;}
        if(fb.get_fb_data_row_size()    != 0) {wxLogMessage("fb.get_fb_data_row_size()    <= 0"); return;}
        if(fb.get_fb_trailer_row_size() != 0) {wxLogMessage("fb.get_fb_trailer_row_size() != 0"); return;}
        if(fb.get_fb_end_row_size()     != 0) {wxLogMessage("fb.get_fb_end_row_size()     != 0"); return;}

        const int header_torikumibi_col = 5; 
        auto value = F()->get_grid_header()->GetCellValue(0, header_torikumibi_col);
        LambdaFBPaserRead(F()->get_grid_header(),  fb.get_fb_header_info(),  &fb, &FBDataParser::get_fb_header_value,  &FBDataParser::get_fb_header_row_size);
        F()->get_grid_header()->SetCellValue(0, header_torikumibi_col, value);
    };

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        LambdaOnHeaderImport();

    }, ID_MENU_HEADER_IMPORT);

    F()->get_button_header_import()->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
    {
        LambdaOnHeaderImport();
    });

    auto LambdaOnHeaderExport = [=]()
    {
        F()->get_grid_header()->SaveEditControlValue();

        fb.set_fb_newline_code("");

        wxFileDialog fdialog(F(), "プリセット保存", "./preset/", wxEmptyString, "PRESET files (*.preset)|*.preset", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if(fdialog.ShowModal() == wxID_CANCEL) return;

        LambdaFBPaserWrite(F()->get_grid_header(),  fb.get_fb_header_info(),  &fb, &FBDataParser::assign_fb_header_line,  &FBDataParser::set_fb_header_value);
        fb.assign_fb_data_line(0);
        fb.assign_fb_trailer_line(0);
        fb.assign_fb_end_line(0);

        if(fb.get_fb_header_row_size()  != 1) {wxLogMessage("fb.get_fb_header_row_size()  != 1"); return;}
        if(fb.get_fb_data_row_size()    != 0) {wxLogMessage("fb.get_fb_data_row_size()    != 0"); return;}
        if(fb.get_fb_trailer_row_size() != 0) {wxLogMessage("fb.get_fb_trailer_row_size() != 0"); return;}
        if(fb.get_fb_end_row_size()     != 0) {wxLogMessage("fb.get_fb_end_row_size()     != 0"); return;}

        if(!fb.save_fb_file(fdialog.GetPath().ToStdString()))
        {
            wxLogMessage("fb.save_fb_file(fdialog.GetPath().ToStdString()) : false");
        }
    
    };

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        LambdaOnHeaderExport(); 
        F()->Refresh();

    }, ID_MENU_HEADER_EXPORT);

    F()->get_button_header_export()->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
    {
        LambdaOnHeaderExport();
        F()->Refresh();    
    });


    // ADD DATA
    auto LambdaOnDataAdd = [](wxGrid *grid, const FBInfo &fb_info)
    {
        grid->AppendRows();

        if(fb_info.size() > grid->GetNumberCols())
        {
            wxLogMessage("fb_info.size() > grid->GetNumberCols()");
            return;
        }

        for(auto &[num, label, offset, length, char_includes, pad_info, initial_value, description] : fb_info)
        {
           auto col = num;
           auto value = wxString(length, pad_info[1]);

            if(initial_value != nullptr) value = initial_value;

            grid->SetCellValue(grid->GetNumberRows() - 1, col, value);
        }

        auto row = grid->GetNumberRows() - 1;
        auto col = grid->GetGridCursorCol();

        grid->ClearSelection();
        grid->GoToCell(row, col);
        grid->SetFocus();
    };

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        LambdaOnDataAdd(F()->get_grid_data(), fb.get_fb_data_info());

    }, ID_MENU_DATA_ADD);

    F()->get_button_data_add()->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
    {
        LambdaOnDataAdd(F()->get_grid_data(), fb.get_fb_data_info());
    });

    auto LambdaOnDataDelete = [](wxGrid *grid, const FBInfo &fb_info)
    {
        auto selected = grid->GetSelectedRows();

        if(selected.size() == 0)
        {
            wxMessageDialog mdialog(grid, "レコードが選択されていません", "情報", wxOK);
            mdialog.ShowModal();
            return;
        }

        wxString massage;        
        massage += "レコード";
        for(const auto &i : selected) massage += '[' + wxString::Format(wxT("%i"), i + 1) + ']';      
        massage += "を削除します";

        wxMessageDialog mdialog(grid, massage, "確認", wxOK | wxCANCEL);
        if(mdialog.ShowModal() == wxID_CANCEL) return;

        selected.Sort([](auto lhs, auto rhs){return rhs - lhs;});
        for(const auto &i : selected)
        {
           grid->DeleteRows(i);
        }

        grid->ClearSelection();
    };

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        LambdaOnDataDelete(F()->get_grid_data(), fb.get_fb_data_info());

    }, ID_MENU_DATA_DELETE);

    F()->get_button_data_delete()->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
    {
        LambdaOnDataDelete(F()->get_grid_data(), fb.get_fb_data_info());
    });

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        F()->get_searchctrl_data_search()->SetFocus();

    }, ID_MENU_DATA_SEARCH_BOX);

    auto LambdaDataSearchForward = [](wxGrid *grid, const FBInfo &fb_info, const wxString &value)
    {
        if(value.empty()) return;

        if(grid->GetNumberCols() == 0 || grid->GetNumberRows() == 0)
        {
            wxLogMessage("grid->GetNumberCols() == 0 || grid->GetNumberRows() == 0");
            return;
        }

        if(grid->GetGridCursorRow() < 0 || grid->GetGridCursorCol() < 0)
        {
            wxLogMessage("grid->GetGridCursorRow() < 0 || grid->GetGridCursorCol() < 0");
            return;
        }

        auto width = grid->GetNumberCols();
        auto height = grid->GetNumberRows();
        auto current_pos = grid->GetGridCursorRow() * width + grid->GetGridCursorCol();

        if(grid->GetCellValue(current_pos / width, current_pos % width).find(value) != wxNOT_FOUND)
        {
            current_pos++; 
        }
        else
        {
            current_pos = 0;
        }

        while(true)
        {
            if(current_pos >= width * height) break;
            if(current_pos <  0             ) break;
            
            auto row = current_pos / width;
            auto col = current_pos % width;

            if(grid->GetCellValue(row, col).find(value) != wxNOT_FOUND)
            {
                grid->SetFocus();
                grid->GoToCell(row, col);
                return;
            }

            current_pos++;
        };

        wxMessageDialog mdialog(grid, "見つかりませんでした", "情報", wxOK);
        mdialog.ShowModal();

        grid->SetFocus();
        grid->GoToCell(grid->GetGridCursorRow(), grid->GetGridCursorCol());
    };

    auto LambdaDataSearchBackward = [](wxGrid *grid, const FBInfo &fb_info, const wxString &value)
    {
        if(value.empty()) return;

        if(grid->GetNumberCols() == 0 || grid->GetNumberRows() == 0)
        {
            wxLogMessage("grid->GetNumberCols() == 0 || grid->GetNumberRows() == 0");
            return;
        }

        if(grid->GetGridCursorRow() < 0 || grid->GetGridCursorCol() < 0)
        {
            wxLogMessage("grid->GetGridCursorRow() < 0 || grid->GetGridCursorCol() < 0");
            return;
        }

        auto width = grid->GetNumberCols();
        auto height = grid->GetNumberRows();
        auto current_pos = grid->GetGridCursorRow() * width + grid->GetGridCursorCol();


        if(grid->GetCellValue(current_pos / width, current_pos % width).find(value) != wxNOT_FOUND)
        {
            current_pos--; 
        }
        else
        {
            current_pos = width * height - 1;
        }

        while(true)
        {
            if(current_pos >= width * height) break;
            if(current_pos <  0             ) break;
            
            auto row = current_pos / width;
            auto col = current_pos % width;

            if(grid->GetCellValue(row, col).find(value) != wxNOT_FOUND)
            {
                grid->SetFocus();
                grid->GoToCell(row, col);
                return;
            }

            current_pos--;
        };

        wxMessageDialog mdialog(grid, "見つかりませんでした", "情報", wxOK);
        mdialog.ShowModal();

        grid->SetFocus();
        grid->GoToCell(grid->GetGridCursorRow(), grid->GetGridCursorCol());
    };

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        LambdaDataSearchForward(F()->get_grid_data(), fb.get_fb_data_info(), F()->get_searchctrl_data_search()->GetValue());

    }, ID_MENU_DATA_SEARCH_FORWARD);

    F()->get_searchctrl_data_search()->Bind(wxEVT_SEARCH, [=](wxCommandEvent& event)
    {
        LambdaDataSearchForward(F()->get_grid_data(), fb.get_fb_data_info(), F()->get_searchctrl_data_search()->GetValue());
    });    

    F()->get_button_data_search_forward()->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
    {
        LambdaDataSearchForward(F()->get_grid_data(), fb.get_fb_data_info(), F()->get_searchctrl_data_search()->GetValue());
    });

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        LambdaDataSearchBackward(F()->get_grid_data(), fb.get_fb_data_info(), F()->get_searchctrl_data_search()->GetValue());

    }, ID_MENU_DATA_SEARCH_BACKWARD);

    F()->get_button_data_search_backward()->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
    {
        LambdaDataSearchBackward(F()->get_grid_data(), fb.get_fb_data_info(), F()->get_searchctrl_data_search()->GetValue());
    });

    auto LambdaOnTrailerRecalculate = [](wxGrid *grid_data, FBInfo fb_data_info, wxGrid *grid_trailer, FBInfo fb_trailer_info)
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
        massage += wxString::Format(wxT("%lld"), sum_kensu - sum_kensu_old) + "件　";
        massage += wxString::Format(wxT("%lld"), sum_kingaku - sum_kingaku_old) + "円　";

        wxMessageDialog mdialog(grid_trailer, massage, "情報", wxOK);
        mdialog.ShowModal();

        auto sum_kingaku_str = wxString::Format(wxT("%lld"), sum_kingaku);
        auto sum_kensu_str   = wxString::Format(wxT("%lld"), sum_kensu);

        {
            auto &value = sum_kensu_str;
            auto &[num, label, offset, length, char_includes, pad_info, initial_value, description] = fb_trailer_info[trailer_kensu_col];

            if(value.size() > length)
            {
                wxLogMessage("value.size() > length");
                return;
            }
            
            if(pad_info[0] == 'R')
            {
                grid_trailer->SetCellValue(0, num, value.append(length - value.length(), pad_info[1]));
            }
            else if(pad_info[0] == 'L')
            {
                grid_trailer->SetCellValue(0, num, value.insert(0, length - value.length(), pad_info[1]));
            }            
        }

        {
            auto &value = sum_kingaku_str;
            auto &[num, label, offset, length, char_includes, pad_info, initial_value, description] = fb_trailer_info[trailer_kingaku_col];

            if(value.size() > length)
            {
                wxLogMessage("value.size() > length");
                return;
            }
            
            if(pad_info[0] == 'R')
            {
                grid_trailer->SetCellValue(0, num, value.append(length - value.length(), pad_info[1]));
            }
            else if(pad_info[0] == 'L')
            {
                grid_trailer->SetCellValue(0, num, value.insert(0, length - value.length(), pad_info[1]));
            }            
        }

    };

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        LambdaOnTrailerRecalculate(F()->get_grid_data(), fb.get_fb_data_info(), F()->get_grid_trailer(), fb.get_fb_trailer_info());
        
    }, ID_MENU_TRAILER_RECALCULATE);

    F()->get_button_trailer_recalculated()->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
    {
        LambdaOnTrailerRecalculate(F()->get_grid_data(), fb.get_fb_data_info(), F()->get_grid_trailer(), fb.get_fb_trailer_info());
    });    

    auto LambdaOnChangedGridCellValue = [](wxGridEvent& event, wxGrid *grid, const FBInfo &fb_info)
    {
        auto row = event.GetRow();
        auto col = event.GetCol();

        auto &[num, label, offset, length, char_includes, pad_info, initial_value, description] = fb_info[col];
        auto value = grid->GetCellValue(row, col);

        if(length - value.length() < 0)
        {
            wxLogMessage("length - value.length() < 0");
            return;
        }

        value.Trim(true);
        value.Trim(false);

        if(initial_value != nullptr && 
            (value.length() == 0 || value.find_first_not_of(char_includes) != wxNOT_FOUND))
        {
            value = initial_value;
        }

        if(pad_info[0] == 'R') value.append(length - value.length(), pad_info[1]);
        else if(pad_info[0] == 'L') value.insert(0, length - value.length(), pad_info[1]);

        grid->SetCellValue(row, col, value);
        
    };

    F()->get_grid_header()->Bind(wxEVT_GRID_CELL_CHANGED, [=](wxGridEvent& event)
    {
        LambdaOnChangedGridCellValue(event, F()->get_grid_header(), fb.get_fb_header_info());
    });

    F()->get_grid_data()->Bind(wxEVT_GRID_CELL_CHANGED, [=](wxGridEvent& event)
    {
        LambdaOnChangedGridCellValue(event, F()->get_grid_data(), fb.get_fb_data_info());
    });   

    F()->get_grid_trailer()->Bind(wxEVT_GRID_CELL_CHANGED, [=](wxGridEvent& event)
    {
        LambdaOnChangedGridCellValue(event, F()->get_grid_trailer(), fb.get_fb_trailer_info());
    });

    F()->get_grid_end()->Bind(wxEVT_GRID_CELL_CHANGED, [=](wxGridEvent& event)
    {
        LambdaOnChangedGridCellValue(event, F()->get_grid_end(), fb.get_fb_end_info());
    });

}


