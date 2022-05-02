#include "InitialFrame.h"

#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/button.h>
#include <wx/grid.h>
#include <wx/srchctrl.h>

#include <wx/menu.h>
#include <wx/stattext.h>
#include <wx/sizer.h>

#include <wx/artprov.h>

#include <wx/log.h>

InitialFrame::InitialFrame() : wxFrame(NULL, wxID_ANY, "")
{

    // MENU FILE
    menu_file = new wxMenu;
    menu_file->Append(ID_MENU_NEW, "新規（フォーマット選択）\tCtrl+N")->SetBitmap(wxArtProvider::GetBitmap(wxART_NEW, wxART_MENU));
    menu_file->Append(ID_MENU_OPEN, "FBデータを開く")->SetBitmap(wxArtProvider::GetBitmap(wxART_FOLDER_OPEN, wxART_MENU));
    menu_file->Append(ID_MENU_SAVEAS, "名前をつけて保存")->SetBitmap(wxArtProvider::GetBitmap(wxART_FLOPPY, wxART_MENU));
    menu_file->AppendSeparator();
    menu_file->Append(ID_MENU_EXIT, "終了\tCtrl+Q")->SetBitmap(wxArtProvider::GetBitmap(wxART_CLOSE, wxART_MENU));

    // MENU EDIT
    menu_edit = new wxMenu;
    menu_edit->Append(ID_MENU_HEADER_IMPORT, "＠ヘッダ：プリセット選択")->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_MENU));
    menu_edit->Append(ID_MENU_HEADER_EXPORT, "＠ヘッダ：プリセット出力")->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_MENU));
    menu_edit->AppendSeparator();
    menu_edit->Append(ID_MENU_DATA_ADD, "＠データ：行追")->SetBitmap(wxArtProvider::GetBitmap(wxART_PLUS, wxART_MENU));
    menu_edit->Append(ID_MENU_DATA_DELETE,"＠データ：行削\tDelete")->SetBitmap(wxArtProvider::GetBitmap(wxART_MINUS, wxART_MENU));
    menu_edit->AppendSeparator();
    menu_edit->Append(ID_MENU_TRAILER_RECALCULATE,"＠トレーラ：再計算")->SetBitmap(wxArtProvider::GetBitmap(wxART_EXECUTABLE_FILE, wxART_MENU));

    // MENU SEARCH
    menu_search = new wxMenu;
    menu_search->Append(ID_MENU_DATA_SEARCH_BOX,"＠データ：検索ボックス\tCtrl+F")->SetBitmap(wxArtProvider::GetBitmap(wxART_FIND, wxART_MENU));
    menu_search->AppendSeparator();
    menu_search->Append(ID_MENU_DATA_SEARCH_FORWARD, "＠データ：次検索\tF3")->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_DOWN, wxART_MENU));
    menu_search->Append(ID_MENU_DATA_SEARCH_BACKWARD, "＠データ：前検索\tShift+F3")->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_UP, wxART_MENU));;

    // MENU HELP
    menu_help = new wxMenu;
    menu_help->Append(ID_MENU_HELP_ABOUT, "このアプリについて")->SetBitmap(wxArtProvider::GetBitmap(wxART_INFORMATION, wxART_MENU));

    // MENU BAR
    menu_bar = new wxMenuBar;
    menu_bar->Append(menu_file, "ファイル(&F)");
    menu_bar->Append(menu_edit, "編集(&E)");
    menu_bar->Append(menu_search, "検索(&S)");
    menu_bar->Append(menu_help, "情報(&H)");
    SetMenuBar(menu_bar);

    // TOP PANEL
    panel_top = new wxPanel(this, wxID_ANY);

    // CELL FONT
    auto font_grid_cell = this->GetFont();
    font_grid_cell.SetPointSize(12);
    font_grid_cell.SetFamily(wxFONTFAMILY_TELETYPE);

    //GRID_HEADER
    grid_header = new wxGrid(panel_top, wxID_ANY);
    grid_header->SetGridLineColour(wxColour(0,0,0));
    grid_header->SetDefaultCellFont(font_grid_cell);
    grid_header->SetTabBehaviour(wxGrid::Tab_Wrap);
    grid_header->SetCellHighlightColour(grid_header->GetSelectionBackground());
    grid_header->DisableDragRowSize();
    grid_header->CreateGrid(0, 0, wxGrid::wxGridSelectRowsOrColumns);

    button_header_import = new wxButton(panel_top, wxID_ANY, "プリセット選択"); button_header_import->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_BUTTON));
    button_header_export = new wxButton(panel_top, wxID_ANY, "プリセット出力");

    //GRID_DATA
    grid_data = new wxGrid(panel_top, wxID_ANY);
    grid_data->SetGridLineColour(wxColour(0,0,0));
    grid_data->SetDefaultCellFont(font_grid_cell);
    grid_data->SetTabBehaviour(wxGrid::Tab_Wrap);
    grid_data->SetCellHighlightColour(grid_data->GetSelectionBackground());
    grid_data->DisableDragRowSize();
    grid_data->SetScrollLineY(grid_data->GetScrollLineY()/2);
    grid_data->CreateGrid(0, 0, wxGrid::wxGridSelectRowsOrColumns);

    button_data_add = new wxButton(panel_top, wxID_ANY, "行追"); button_data_add->SetBitmap(wxArtProvider::GetBitmap(wxART_PLUS, wxART_BUTTON));
    button_data_delete = new wxButton(panel_top, wxID_ANY, "行削"); button_data_delete->SetBitmap(wxArtProvider::GetBitmap(wxART_MINUS, wxART_BUTTON));
    searchctrl_data_search = new wxSearchCtrl(panel_top, wxID_ANY);
    button_data_search_forward = new wxButton(panel_top, wxID_ANY, "次検索"); button_data_search_forward->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_DOWN, wxART_BUTTON));
    button_data_search_backward = new wxButton(panel_top, wxID_ANY, "前検索"); button_data_search_backward->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_UP, wxART_BUTTON));

    //GRID_TRAILER
    grid_trailer = new wxGrid(panel_top, wxID_ANY);
    grid_trailer->SetGridLineColour(wxColour(0,0,0));
    grid_trailer->SetDefaultCellFont(font_grid_cell);
    grid_trailer->SetTabBehaviour(wxGrid::Tab_Wrap);
    grid_trailer->SetCellHighlightColour(grid_trailer->GetSelectionBackground());
    grid_trailer->DisableDragRowSize();
    grid_trailer->CreateGrid(0, 0, wxGrid::wxGridSelectRowsOrColumns);

    button_trailer_recalculate = new wxButton(panel_top, wxID_ANY, "再計算"); button_trailer_recalculate->SetBitmap(wxArtProvider::GetBitmap(wxART_EXECUTABLE_FILE, wxART_BUTTON));

    //GRID_END
    grid_end = new wxGrid(panel_top, wxID_ANY);
    grid_end->SetGridLineColour(wxColour(0,0,0));
    grid_end->SetDefaultCellFont(font_grid_cell);
    grid_end->SetTabBehaviour(wxGrid::Tab_Wrap);
    grid_end->SetCellHighlightColour(grid_end->GetSelectionBackground());
    grid_end->DisableDragRowSize();
    grid_end->CreateGrid(0, 0, wxGrid::wxGridSelectRowsOrColumns);

    //SIZER
    auto sizer_top = new wxBoxSizer(wxVERTICAL);
    panel_top->SetSizer(sizer_top);
        auto sizer_header_label = new wxBoxSizer(wxHORIZONTAL);
        sizer_top->Add(sizer_header_label, 0, wxALIGN_LEFT);
            sizer_header_label->Add(new wxStaticText(panel_top, wxID_ANY, "＠ヘッダレコード：　"), 0, wxALIGN_BOTTOM);
            sizer_header_label->Add(button_header_import, 0, wxALIGN_BOTTOM);
            sizer_header_label->Add(button_header_export, 0, wxALIGN_BOTTOM);
        sizer_top->Add(grid_header, 0, wxALIGN_LEFT | wxBOTTOM, 10);

        auto sizer_data_label = new wxBoxSizer(wxHORIZONTAL);
        sizer_top->Add(sizer_data_label, 0, wxALIGN_LEFT);
            sizer_data_label->Add(new wxStaticText(panel_top, wxID_ANY, "＠データレコード：　"), 0, wxALIGN_BOTTOM);
            sizer_data_label->Add(button_data_add, 0, wxALIGN_BOTTOM);
            sizer_data_label->Add(button_data_delete, 0, wxALIGN_BOTTOM | wxRIGHT, 50);
            sizer_data_label->Add(searchctrl_data_search, 0, wxALIGN_BOTTOM);
            sizer_data_label->Add(button_data_search_forward, 0, wxALIGN_BOTTOM);
            sizer_data_label->Add(button_data_search_backward, 0, wxALIGN_BOTTOM);
        sizer_top->Add(grid_data, 1, wxALIGN_LEFT | wxEXPAND | wxBOTTOM, 10);

        auto sizer_trailer_label = new wxBoxSizer(wxHORIZONTAL);
        sizer_top->Add(sizer_trailer_label, 0, wxALIGN_LEFT);
            sizer_trailer_label->Add(new wxStaticText(panel_top, wxID_ANY, "＠トレーラレコード：　"), 0, wxALIGN_BOTTOM);
            sizer_trailer_label->Add(button_trailer_recalculate, 0, wxALIGN_BOTTOM);
        sizer_top->Add(grid_trailer, 0, wxALIGN_LEFT | wxBOTTOM, 10);

        auto sizer_end_label = new wxBoxSizer(wxHORIZONTAL);
        sizer_top->Add(sizer_end_label, 0, wxALIGN_LEFT);
            sizer_end_label->Add(new wxStaticText(panel_top, wxID_ANY, "＠エンドレコード：　"), 0, wxALIGN_BOTTOM);
        sizer_top->Add(grid_end, 0, wxALIGN_LEFT | wxBOTTOM, 10);


    // STATUS
    CreateStatusBar();
}

wxGridCellCoords InitialFrame::search_next_value(wxGrid *grid, const wxString &search_value, bool is_forward)
{
    if(search_value.empty()) return {-1, -1};

    if(grid->GetNumberCols() <= 0 || grid->GetNumberRows() <= 0)
    {
        wxLogMessage("grid->GetNumberCols() <= 0 || grid->GetNumberRows() <= 0");
        return {-1, -1};
    }

    if(grid->GetGridCursorRow() < 0 || grid->GetGridCursorCol() < 0)
    {
        wxLogMessage("grid->GetGridCursorRow() < 0 || grid->GetGridCursorCol() < 0");
        return {-1, -1};
    }

    auto width = grid->GetNumberCols();
    auto height = grid->GetNumberRows();

    auto first_row = grid->GetGridCursorRow();
    auto first_col = grid->GetGridCursorCol();
    
    auto first_value = grid->GetCellValue(first_row, first_col);

    auto next = [d = (is_forward ? 1 : -1)](auto &pos)
    {
        pos += d;
    };

    auto current_pos = first_row * width + first_col;
    if(first_value.find(search_value) != wxNOT_FOUND)
    {
        next(current_pos);
    }
    else
    {
        if(is_forward)
        {
            current_pos = 0;
        }
        else
        {
            current_pos = width * height - 1;
        }
        
    }

    while(true)
    {
        if(current_pos >= width * height) break;
        if(current_pos <  0             ) break;
            
        auto current_row = current_pos / width;
        auto current_col = current_pos % width;

        auto current_value = grid->GetCellValue(current_row, current_col);
        if(current_value.find(search_value) != wxNOT_FOUND)
        {
            grid->SetFocus();
            grid->GoToCell(current_row, current_col);
            return {current_row, current_col};
        }

        next(current_pos);
    };

    wxMessageDialog mdialog(grid, "見つかりませんでした", "情報", wxOK);
    mdialog.ShowModal();

    grid->SetFocus();
    grid->GoToCell(first_row, first_col);
    return {first_row, first_col};
}

void InitialFrame::reset_grid(wxGrid *grid, const FBAttrs &attrs)
{
    if(auto h = grid->GetNumberRows(); h > 0) grid->DeleteRows(0, h);
    if(auto w = grid->GetNumberCols(); w > 0) grid->DeleteCols(0, w);
    
    if(grid->GetNumberRows() != 0 || grid->GetNumberCols() != 0)
    {
        wxLogMessage("grid->GetNumberRows() != 0 || grid->GetNumberCols() != 0");
        return;
    }

    grid->AppendCols(attrs.size());

    for(auto &attr : attrs)
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

    if(attrs.size() > grid->GetNumberCols())
    {
        wxLogMessage("fb_attr.size() > grid->GetNumberCols()");
        return;
    }

    for(auto &attr : attrs)
    {
        auto col = attr.num;
        auto row = grid->GetNumberRows() - 1;
        auto value = wxString(attr.length, attr.pad_info[1]);

        if(attr.initial_value != nullptr) value = attr.initial_value;

        grid->SetCellValue(row, col, value);
    }

    auto row = grid->GetNumberRows() - 1;
    auto col = grid->GetGridCursorCol();

    grid->ClearSelection();
    grid->GoToCell(row, col);
    grid->SetFocus();        

    grid->AutoSize();   
}

/*
void InitialFrame::reset_grid(const FBAttrsArray &attrs_array)
{
    reset_grid(grid_header , attrs_array.at((int)FBPart::HEADER) );
    reset_grid(grid_data   , attrs_array.at((int)FBPart::DATA)   );
    reset_grid(grid_trailer, attrs_array.at((int)FBPart::TRAILER));
    reset_grid(grid_end    , attrs_array.at((int)FBPart::END)    );
}
*/

bool InitialFrame::is_edited(wxGrid *grid, const FBAttrs &attrs)
{
    if(grid->GetNumberRows() != 1) return false;

    for(auto &attr : attrs)
    {
        auto col = attr.num;
        auto row = grid->GetNumberRows() - 1;
        auto value = grid->GetCellValue(row, col);

        auto initial_value = wxString(attr.length, attr.pad_info[1]);
        if(attr.initial_value != nullptr) initial_value = attr.initial_value;

        if(value != initial_value) return true;
    }

    return false;   
}

/*
bool InitialFrame::is_edited(const FBAttrsArray &attrs_array)
{
    if(is_edited(grid_header , attrs_array.at((int)FBPart::HEADER) )) return true;
    if(is_edited(grid_data   , attrs_array.at((int)FBPart::DATA)   )) return true;
    if(is_edited(grid_trailer, attrs_array.at((int)FBPart::TRAILER))) return true;
    if(is_edited(grid_end    , attrs_array.at((int)FBPart::END)    )) return true;

    return false; 
}
*/

void InitialFrame::save_editing_value()
{
    grid_header->SaveEditControlValue();
    grid_data->SaveEditControlValue();
    grid_trailer->SaveEditControlValue();
    grid_end->SaveEditControlValue();   
}

void InitialFrame::insert_selected(wxGrid *grid, const FBAttrs &attrs)
{
    if(attrs.size() > grid->GetNumberCols())
    {
        wxLogMessage("attr.size() > grid->GetNumberCols()");
        return;
    }

    auto selected = grid->GetSelectedRows();
    if(selected.empty())
    {
        auto bottom = grid->GetNumberRows();
        selected.Add(bottom);
    }

    grid->ClearSelection();
    selected.Sort([](auto lhs, auto rhs){return rhs - lhs;});

    for(int i = 0; i < selected.size(); ++i)
    {
        auto last = selected[i];
        while(i < selected.size() - 1)
        {
            auto diff = selected[i] - selected[i+1];
            if(diff == 1) i++;
            else break;
        }
        auto first = selected[i];

        auto insert_pos = first;
        auto insert_num = last - first + 1;
            
        for(int j = 0; j < insert_num; ++j)
        {
            grid->InsertRows(insert_pos);

            for(auto &attr : attrs)
            {
                auto row = insert_pos;
                auto col = attr.num;
                auto value = wxString(attr.length, attr.pad_info[1]);

                if(attr.initial_value != nullptr) value = attr.initial_value;

                grid->SetCellValue(row, col, value);                    
            }

            grid->SelectRow(insert_pos, true);
        }
    }

    auto row = selected[0];
    auto col = grid->GetGridCursorCol();

    grid->GoToCell(row, col);
    grid->SetFocus();

}

void InitialFrame::delete_selected(wxGrid *grid, const FBAttrs &attrs)
{
    auto selected = grid->GetSelectedRows();

    if(selected.empty())
    {
        wxMessageDialog mdialog(grid, "レコードが選択されていません", "情報", wxOK);
        mdialog.ShowModal();
        return;
    }

    wxString massage;        
    massage += "レコード";
    for(auto &i : selected) massage += '[' + wxString::Format("%d", i + 1) + ']';      
    massage += "を削除します";

    wxMessageDialog mdialog(grid, massage, "確認", wxOK | wxCANCEL);
    if(mdialog.ShowModal() == wxID_CANCEL) return;

    selected.Sort([](auto lhs, auto rhs){return rhs - lhs;});
    for(auto &insert_pos : selected)
    {
       grid->DeleteRows(insert_pos);
    }

    grid->ClearSelection();
}

wxMenuBar *InitialFrame::get_menu_bar()
{
    return menu_bar;
}

wxMenu *InitialFrame::get_menu_file()
{
    return menu_file;
}

wxMenu *InitialFrame::get_menu_edit()
{
    return menu_edit;
}

wxMenu *InitialFrame::get_menu_search()
{
    return menu_search;
}

wxPanel *InitialFrame::get_panel_top()
{
    return panel_top;
}

wxGrid *InitialFrame::get_grid_header()
{
    return grid_header;
}

wxButton *InitialFrame::get_button_header_import()
{
    return button_header_import;   
}

wxButton *InitialFrame::get_button_header_export()
{
    return button_header_export;
}

wxGrid *InitialFrame::get_grid_data()
{
    return grid_data;
}

wxButton *InitialFrame::get_button_data_add()
{
    return button_data_add;
}

wxButton *InitialFrame::get_button_data_delete()
{
    return button_data_delete;
}

wxSearchCtrl *InitialFrame::get_searchctrl_data_search()
{
    return searchctrl_data_search;
}

wxButton *InitialFrame::get_button_data_search_forward()
{
    return button_data_search_forward;
}

wxButton *InitialFrame::get_button_data_search_backward()
{
    return button_data_search_backward;
}

wxGrid *InitialFrame::get_grid_trailer()
{
    return grid_trailer;
}

wxButton *InitialFrame::get_button_trailer_recalculated()
{
    return button_trailer_recalculate;
}

wxGrid *InitialFrame::get_grid_end()
{
    return grid_end;
}


