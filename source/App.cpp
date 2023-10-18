#include "App.h"
#include "InitialController.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    m_ctrl = new InitialController();

    auto ctrl = static_cast<InitialController*>(m_ctrl);
    ctrl->initialize();
    ctrl->toggle_view();

    return true;
}

int App::OnExit()
{
    auto ctrl = static_cast<InitialController*>(m_ctrl);
    ctrl->finalize();
    
    if(ctrl != nullptr) delete ctrl;
    
    return 0;
}