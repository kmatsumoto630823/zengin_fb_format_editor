#ifndef INITIAL_CONTROLLER_H
#define INITIAL_CONTROLLER_H

#include "common/FBAttrs.h"
#include "../Controller.h"
#include "view/initial/InitialFrame.h"

#include <wx/fileconf.h>

class InitialController final : public Controller
{
public:
    InitialController() = default;
    ~InitialController() = default;

    virtual void initialize() override;
    virtual void finalize() override;

private:
    void switch_type(FBType type);
    
    void create_frame(wxFileConfig& config);
    void create_binds(wxFileConfig& config);

    auto get_casted_frame(){return (InitialFrame*) frame;}

    std::string m_chars_kana;
    std::string m_pad_kana;
    std::string m_chars_num;
    std::string m_pad_num;

    FBAttrsArray m_attrs_array;
};

#endif //INITIAL_CONTROLLER_H
