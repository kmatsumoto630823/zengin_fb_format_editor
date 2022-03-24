#ifndef INITIALCONTROLLER_H
#define INITIALCONTROLLER_H

#include "../Controller.h"
#include "../../view/initial/InitialFrame.h"
#include "../../model/FBParser.h"

#include <wx/fileconf.h>

class InitialController final : public Controller
{
public:
    InitialController() = default;
    ~InitialController() = default;

    virtual void initialize() override; 

private:
    void reset_grid();
    bool is_edited();
    void save_editing_value();

    enum zengin_format
    {
        SOHFURI = 0,
        KYUYO_SHOYO = 1,
        FURIKAE = 2,
    };
    
    void switch_format(zengin_format format);

    void create_frame(wxFileConfig &config);
    void create_binds(wxFileConfig &config);

    auto get_casted_frame(){return (InitialFrame*) frame;}
    FBParser fb;
};

#endif //INITIALCONTROLLER_H
