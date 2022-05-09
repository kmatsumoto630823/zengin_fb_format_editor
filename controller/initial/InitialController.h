#ifndef INITIALCONTROLLER_H
#define INITIALCONTROLLER_H

#include "../Controller.h"
#include "view/initial/InitialFrame.h"
#include "model/FBParser.h"

#include <wx/fileconf.h>

class InitialController final : public Controller
{
public:
    InitialController() = default;
    ~InitialController() = default;

    virtual void initialize() override; 

private:
    void switch_fbtype(FBType type);
    bool is_edited_any();
    
    void create_frame(wxFileConfig& config);
    void create_binds(wxFileConfig& config);

    auto get_casted_frame(){return (InitialFrame*) frame;}

    FBParser m_fb;
    
    wxString m_app_name;
    wxString m_preset_path;
    wxString m_export_path;
    wxString m_fbdata_path;

    wxString m_chars_kana;
};

#endif //INITIALCONTROLLER_H
