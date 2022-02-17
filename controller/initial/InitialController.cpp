#include "InitialController.h"
#include <wx/log.h>

#define F() (InitialController::get_casted_frame())

void InitialController::reset_grid()
{
    auto lambda_reset_grid = [](wxGrid * grid, const FBAttrs &fb_attrs)
    {
        if(auto h = grid->GetNumberRows(); h > 0) grid->DeleteRows(0, h);
        if(auto w = grid->GetNumberCols(); w > 0) grid->DeleteCols(0, w);
    
        if(grid->GetNumberRows() != 0 || grid->GetNumberCols() != 0)
        {
            wxLogMessage("grid->GetNumberRows() != 0 || grid->GetNumberCols() != 0");
            return;
        }

        grid->AppendCols(fb_attrs.size());

        for(const auto &attr : fb_attrs)
        {
            auto col = attr.num;

            grid->SetColLabelValue(col, attr.label);

            auto col_attr = new wxGridCellAttr;
            auto col_editor = new trimGridCellTextEditor(attr.length);

            if(attr.char_includes != nullptr)
                col_editor->SetValidString(attr.char_includes);
            if(attr.label != nullptr && attr.description != nullptr)
                col_editor->SetTipString(attr.label, attr.description);

            col_attr->SetEditor(col_editor);
            grid->SetColAttr(col, col_attr);    
        }

        grid->AppendRows();

        if(fb_attrs.size() > grid->GetNumberCols())
        {
            wxLogMessage("fb_attr.size() > grid->GetNumberCols()");
            return;
        }

        for(const auto &attr : fb_attrs)
        {
           auto col = attr.num;
           auto value = wxString(attr.length, attr.pad_info[1]);

            if(attr.initial_value != nullptr) value = attr.initial_value;

            grid->SetCellValue(grid->GetNumberRows() - 1, col, value);
        }

        auto row = grid->GetNumberRows() - 1;
        auto col = grid->GetGridCursorCol();

        grid->ClearSelection();
        grid->GoToCell(row, col);
        grid->SetFocus();        

        grid->AutoSize();
    };


    lambda_reset_grid(F()->get_grid_header(),  fb.get_fb_header_attrs());
    lambda_reset_grid(F()->get_grid_data(),    fb.get_fb_data_attrs());
    lambda_reset_grid(F()->get_grid_trailer(), fb.get_fb_trailer_attrs());
    lambda_reset_grid(F()->get_grid_end(),     fb.get_fb_end_attrs());
}

void InitialController::create_frame()
{
    this->frame = new InitialFrame();

    F()->get_button_header_export()->Hide();

    reset_grid();

    F()->SetSize(wxSize(1600, 900));
}

void InitialController::create_binds()
{
    auto LambdaFBPaserRead = [](wxGrid *grid, const FBAttrs &fb_attrs, auto fb, auto get_fb_value, auto get_fb_row_size)
    {
        if(grid->GetNumberRows() != 0) grid->DeleteRows(0, grid->GetNumberRows());
        grid->AppendRows((fb->*get_fb_row_size)());

        for(int row = 0; row < (fb->*get_fb_row_size)(); row++)
        {
            for(const auto &attr : fb_attrs)
            {
                auto col = attr.num;
                grid->SetCellValue(row, col, wxString((fb->*get_fb_value)(row, col).data(), attr.length));
            }
        }
    };

    auto LambdaFBPaserWrite = [](wxGrid *grid, const FBAttrs &fb_attrs, auto fb, auto assign_fb_line, auto set_fb_value)
    {
        auto row_size = grid->GetNumberRows();
        (fb->*assign_fb_line)(row_size);

        for(int row = 0; row < row_size; row++)
        {
            for(const auto &attr : fb_attrs)
            {
                auto col = attr.num;
                auto value = grid->GetCellValue(row, col);
                (fb->*set_fb_value)(row, col, value.ToStdString());
            }
        }
    };

    // NEW
    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        wxMessageDialog mdialog(F(), "�ҏW���̃f�[�^�������܂�\r\n���s���܂���", "�m�F", wxOK | wxCANCEL);
        if(mdialog.ShowModal() == wxID_CANCEL) return;
        
        reset_grid();
        
    }, ID_MENU_NEW);

    // OPEN
    auto LambdaOnOpen = [=](const wxString &default_path)
    {
        auto path = default_path;

        wxMessageDialog mdialog(F(), "�ҏW���̃f�[�^�������܂�\r\n���s���܂����H", "�m�F", wxOK | wxCANCEL);
        if(mdialog.ShowModal() == wxID_CANCEL) return;

        if(path.empty())
        {
            wxFileDialog fdialog(F(), "FB�f�[�^�̑I��",wxEmptyString, wxEmptyString, "TXT files (*.txt)|*.txt");
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

        LambdaFBPaserRead(F()->get_grid_header(),  fb.get_fb_header_attrs(),  &fb, &FBParser::get_fb_header_value,  &FBParser::get_fb_header_row_size);
        LambdaFBPaserRead(F()->get_grid_data(),    fb.get_fb_data_attrs(),    &fb, &FBParser::get_fb_data_value,    &FBParser::get_fb_data_row_size);
        LambdaFBPaserRead(F()->get_grid_trailer(), fb.get_fb_trailer_attrs(), &fb, &FBParser::get_fb_trailer_value, &FBParser::get_fb_trailer_row_size);
        LambdaFBPaserRead(F()->get_grid_end(),     fb.get_fb_end_attrs(),     &fb, &FBParser::get_fb_end_value,     &FBParser::get_fb_end_row_size);
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
            wxMessageDialog mdialog(F(), "�f�[�^���R�[�h�ƃg���[���[���R�[�h�̍��v�����A���v���z����v���܂���\r\n���s���܂����H", "�m�F", wxOK | wxCANCEL);
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
            "CRLF(�f�t�H���g)",
            "CR",
            "LF",
            "���s�Ȃ�"
        };

        
        wxSingleChoiceDialog scdialog(F(), "���s�R�[�h��I�����Ă�������", "�I��", std::size(newline_code_selection_strings) , newline_code_selection_strings);
        scdialog.SetSelection(NEWLINE_CRLF);

        if(scdialog.ShowModal() == wxID_CANCEL) return;

        auto &code = newline_codes[scdialog.GetSelection()];
        fb.set_fb_newline_code(code.ToStdString());
        
        wxFileDialog fdialog(F(), "FB�f�[�^�̕ۑ�", wxEmptyString, wxEmptyString, "TXT files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if(fdialog.ShowModal() == wxID_CANCEL) return;

        LambdaFBPaserWrite(F()->get_grid_header(),  fb.get_fb_header_attrs(),  &fb, &FBParser::assign_fb_header_line,  &FBParser::set_fb_header_value);
        LambdaFBPaserWrite(F()->get_grid_data(),    fb.get_fb_data_attrs(),    &fb, &FBParser::assign_fb_data_line,    &FBParser::set_fb_data_value);
        LambdaFBPaserWrite(F()->get_grid_trailer(), fb.get_fb_trailer_attrs(), &fb, &FBParser::assign_fb_trailer_line, &FBParser::set_fb_trailer_value);
        LambdaFBPaserWrite(F()->get_grid_end(),     fb.get_fb_end_attrs(),     &fb, &FBParser::assign_fb_end_line,     &FBParser::set_fb_end_value);

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
        wxMessageDialog mdialog(frame, "�ҏW���̃f�[�^�������܂�\r\n�I�����܂����H", "�m�F", wxOK | wxCANCEL);
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

        wxFileDialog fdialog(F(), "�v���Z�b�g�I��", "./preset/", wxEmptyString, "PRESET files (*.preset)|*.preset");
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

        const int torikumibi_col = 5; 
        auto value = F()->get_grid_header()->GetCellValue(0, torikumibi_col);
        LambdaFBPaserRead(F()->get_grid_header(),  fb.get_fb_header_attrs(),  &fb, &FBParser::get_fb_header_value,  &FBParser::get_fb_header_row_size);
        F()->get_grid_header()->SetCellValue(0, torikumibi_col, value);
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

        wxFileDialog fdialog(F(), "�v���Z�b�g�ۑ�", "./preset/", wxEmptyString, "PRESET files (*.preset)|*.preset", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if(fdialog.ShowModal() == wxID_CANCEL) return;

        LambdaFBPaserWrite(F()->get_grid_header(),  fb.get_fb_header_attrs(),  &fb, &FBParser::assign_fb_header_line,  &FBParser::set_fb_header_value);
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
    auto LambdaOnDataAdd = [](wxGrid *grid, const FBAttrs &fb_attrs)
    {
        grid->AppendRows();

        if(fb_attrs.size() > grid->GetNumberCols())
        {
            wxLogMessage("fb_attr.size() > grid->GetNumberCols()");
            return;
        }

        for(const auto &attr : fb_attrs)
        {
           auto col = attr.num;
           auto value = wxString(attr.length, attr.pad_info[1]);

            if(attr.initial_value != nullptr) value = attr.initial_value;

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
        LambdaOnDataAdd(F()->get_grid_data(), fb.get_fb_data_attrs());

    }, ID_MENU_DATA_ADD);

    F()->get_button_data_add()->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
    {
        LambdaOnDataAdd(F()->get_grid_data(), fb.get_fb_data_attrs());
    });

    auto LambdaOnDataDelete = [](wxGrid *grid, const FBAttrs &fb_attrs)
    {
        auto selected = grid->GetSelectedRows();

        if(selected.size() == 0)
        {
            wxMessageDialog mdialog(grid, "���R�[�h���I������Ă��܂���", "���", wxOK);
            mdialog.ShowModal();
            return;
        }

        wxString massage;        
        massage += "���R�[�h";
        for(const auto &i : selected) massage += '[' + wxString::Format(wxT("%i"), i + 1) + ']';      
        massage += "���폜���܂�";

        wxMessageDialog mdialog(grid, massage, "�m�F", wxOK | wxCANCEL);
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
        LambdaOnDataDelete(F()->get_grid_data(), fb.get_fb_data_attrs());

    }, ID_MENU_DATA_DELETE);

    F()->get_button_data_delete()->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
    {
        LambdaOnDataDelete(F()->get_grid_data(), fb.get_fb_data_attrs());
    });

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        F()->get_searchctrl_data_search()->SetFocus();

    }, ID_MENU_DATA_SEARCH_BOX);

    auto LambdaDataSearchForward = [](wxGrid *grid, const FBAttrs &fb_attrs, const wxString &value)
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

        wxMessageDialog mdialog(grid, "������܂���ł���", "���", wxOK);
        mdialog.ShowModal();

        grid->SetFocus();
        grid->GoToCell(grid->GetGridCursorRow(), grid->GetGridCursorCol());
    };

    auto LambdaDataSearchBackward = [](wxGrid *grid, const FBAttrs &fb_attrs, const wxString &value)
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

        wxMessageDialog mdialog(grid, "������܂���ł���", "���", wxOK);
        mdialog.ShowModal();

        grid->SetFocus();
        grid->GoToCell(grid->GetGridCursorRow(), grid->GetGridCursorCol());
    };

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        LambdaDataSearchForward(F()->get_grid_data(), fb.get_fb_data_attrs(), F()->get_searchctrl_data_search()->GetValue());

    }, ID_MENU_DATA_SEARCH_FORWARD);

    F()->get_searchctrl_data_search()->Bind(wxEVT_SEARCH, [=](wxCommandEvent& event)
    {
        LambdaDataSearchForward(F()->get_grid_data(), fb.get_fb_data_attrs(), F()->get_searchctrl_data_search()->GetValue());
    });    

    F()->get_button_data_search_forward()->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
    {
        LambdaDataSearchForward(F()->get_grid_data(), fb.get_fb_data_attrs(), F()->get_searchctrl_data_search()->GetValue());
    });

    F()->Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
        LambdaDataSearchBackward(F()->get_grid_data(), fb.get_fb_data_attrs(), F()->get_searchctrl_data_search()->GetValue());

    }, ID_MENU_DATA_SEARCH_BACKWARD);

    F()->get_button_data_search_backward()->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
    {
        LambdaDataSearchBackward(F()->get_grid_data(), fb.get_fb_data_attrs(), F()->get_searchctrl_data_search()->GetValue());
    });

    auto LambdaOnTrailerRecalculate = [](wxGrid *grid_data, FBAttrs fb_data_attrs, wxGrid *grid_trailer, FBAttrs fb_trailer_attrs)
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
        massage += "�����F�@";
        massage += wxString::Format(wxT("%lld"), sum_kensu - sum_kensu_old) + "���@";
        massage += wxString::Format(wxT("%lld"), sum_kingaku - sum_kingaku_old) + "�~�@";

        wxMessageDialog mdialog(grid_trailer, massage, "���", wxOK);
        mdialog.ShowModal();

        auto sum_kingaku_str = wxString::Format(wxT("%lld"), sum_kingaku);
        auto sum_kensu_str   = wxString::Format(wxT("%lld"), sum_kensu);

        {
            auto &value = sum_kensu_str;
            const auto &attr = fb_trailer_attrs[trailer_kensu_col];

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
            auto &value = sum_kingaku_str;
            const auto &attr = fb_trailer_attrs[trailer_kingaku_col];

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
        LambdaOnTrailerRecalculate(F()->get_grid_data(), fb.get_fb_data_attrs(), F()->get_grid_trailer(), fb.get_fb_trailer_attrs());
        
    }, ID_MENU_TRAILER_RECALCULATE);

    F()->get_button_trailer_recalculated()->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
    {
        LambdaOnTrailerRecalculate(F()->get_grid_data(), fb.get_fb_data_attrs(), F()->get_grid_trailer(), fb.get_fb_trailer_attrs());
    });    

    auto LambdaOnChangedGridCellValue = [](wxGridEvent& event, wxGrid *grid, const FBAttrs &fb_attrs)
    {
        auto row = event.GetRow();
        auto col = event.GetCol();

        const auto &attr = fb_attrs[col];
        auto value = grid->GetCellValue(row, col);

        if(attr.length - value.length() < 0)
        {
            wxLogMessage("length - value.length() < 0");
            return;
        }

        value.Trim(true);
        value.Trim(false);

        if(attr.initial_value != nullptr && 
            (value.length() == 0 || value.find_first_not_of(attr.char_includes) != wxNOT_FOUND))
        {
            value = attr.initial_value;
        }

        if(attr.pad_info[0] == 'R')      value.append(   attr.length - value.length(), attr.pad_info[1]);
        else if(attr.pad_info[0] == 'L') value.insert(0, attr.length - value.length(), attr.pad_info[1]);

        grid->SetCellValue(row, col, value);
        
    };

    F()->get_grid_header()->Bind(wxEVT_GRID_CELL_CHANGED, [=](wxGridEvent& event)
    {
        LambdaOnChangedGridCellValue(event, F()->get_grid_header(), fb.get_fb_header_attrs());
    });

    F()->get_grid_data()->Bind(wxEVT_GRID_CELL_CHANGED, [=](wxGridEvent& event)
    {
        LambdaOnChangedGridCellValue(event, F()->get_grid_data(), fb.get_fb_data_attrs());
    });   

    F()->get_grid_trailer()->Bind(wxEVT_GRID_CELL_CHANGED, [=](wxGridEvent& event)
    {
        LambdaOnChangedGridCellValue(event, F()->get_grid_trailer(), fb.get_fb_trailer_attrs());
    });

    F()->get_grid_end()->Bind(wxEVT_GRID_CELL_CHANGED, [=](wxGridEvent& event)
    {
        LambdaOnChangedGridCellValue(event, F()->get_grid_end(), fb.get_fb_end_attrs());
    });

}


