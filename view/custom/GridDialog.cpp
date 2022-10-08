#include "GridDialog.h"

#include <wx/button.h>
#include <wx/srchctrl.h>
#include <wx/stattext.h>
#include <wx/sizer.h>

GridDialog::GridDialog
(
    const FBAttrs& attrs,
    const wxString& message,
    wxWindow* parent,
    wxWindowID id,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name    
) : wxDialog(parent, id, title, pos, size, style, name)
{
    m_grid = new CustomGrid(this, wxID_ANY);
    m_grid->reset(attrs);
    m_grid->EnableEditing(false);

    auto searchctrl = new wxSearchCtrl(this, wxID_ANY);
    auto button_search_forward = new wxButton(this, wxID_ANY, "次検索");
    auto button_search_backward = new wxButton(this, wxID_ANY, "前検索");

    auto button_select_all = new wxButton(this, wxID_ANY, "全選択");
    auto button_clear_selected = new wxButton(this, wxID_ANY, "全解除");
    
    auto sizer_top = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer_top);
        sizer_top->AddSpacer(10);

        auto sizer_head = new wxBoxSizer(wxHORIZONTAL);
        sizer_top->Add(sizer_head, 0, wxALIGN_LEFT);
            sizer_head->Add(new wxStaticText(this, wxID_ANY, message), 0, wxALIGN_BOTTOM | wxRIGHT, 50);
            sizer_head->Add(searchctrl, 0, wxALIGN_BOTTOM);
            sizer_head->Add(button_search_forward, 0, wxALIGN_BOTTOM);
            sizer_head->Add(button_search_backward, 0, wxALIGN_BOTTOM);

        sizer_top->Add(m_grid, 1, wxALIGN_LEFT | wxEXPAND | wxLEFT, 1);

        sizer_top->AddSpacer(10);

        auto sizer_foot = new wxBoxSizer(wxHORIZONTAL);
        sizer_top->Add(sizer_foot, 0, wxALIGN_CENTER);
            sizer_foot->Add(button_select_all, 0, wxALIGN_CENTER | wxRIGHT, 10);
            sizer_foot->Add(button_clear_selected, 0, wxALIGN_CENTER | wxRIGHT, 10);
            sizer_foot->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_CENTER);

        sizer_top->AddSpacer(10);

    searchctrl->Bind(wxEVT_SEARCH, [=, this](wxCommandEvent& event)
    {
        auto search_value = searchctrl->GetValue();
        auto is_forward = true;
        
        m_grid->search_next_value(search_value, is_forward);
    });

    button_search_forward->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        auto search_value = searchctrl->GetValue();
        auto is_forward = true;
        
        m_grid->search_next_value(search_value, is_forward);
    });

    button_search_backward->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        auto search_value = searchctrl->GetValue();
        auto is_forward = false;
        
        m_grid->search_next_value(search_value, is_forward);
    });


    button_select_all->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        m_grid->SelectAll();
        m_grid->SetFocus();
    });

    button_clear_selected->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent& event)
    {
        m_grid->ClearSelection();
        m_grid->SetFocus();
    });
}

GridDialog::~GridDialog()
{

}

int GridDialog::ShowModal()
{
    m_grid->SelectAll();
    m_grid->SetFocus();
    return wxDialog::ShowModal();
}