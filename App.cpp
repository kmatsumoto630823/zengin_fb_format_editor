#include "App.h"
#include "controller/initial/InitialController.h"

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
    
    delete ctrl;
    return 0;
}