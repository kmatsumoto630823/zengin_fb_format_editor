#ifndef INITIAL_CONTROLLER_H
#define INITIAL_CONTROLLER_H

#include "common/FBAttrs.h"
#include "InitialFrame.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class InitialController
{
public:
    InitialController()  = default;
    ~InitialController() = default;

    InitialController(const InitialController&)            = delete;
    InitialController& operator=(const InitialController&) = delete; 
    InitialController(InitialController&&)                 = delete;   
    InitialController& operator=(InitialController&&)      = delete;

    void initialize();
    void finalize();
    bool toggle_view();

private:
    void create_frame();
    void create_binds();

    void switch_type(FBType type);
    inline auto get_frame(){ return m_frame; };

    InitialFrame* m_frame;
    
    FBType m_type;
    FBAttrsArray m_attrs_array;

    wxString m_app_name;
    int m_window_width;
    int m_window_height;

    std::string m_chars_kana;
    std::string m_padding_kana;
    std::string m_chars_num;
    std::string m_padding_num;
    std::string m_chars_dummy;
    std::string m_padding_dummy;
};

#endif //INITIAL_CONTROLLER_H
