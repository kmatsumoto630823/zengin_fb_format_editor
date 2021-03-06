#include "App.h"
#include "controller/initial/InitialController.h"


wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    Controller* ctrl = (Controller*) new InitialController();
    ctrl->initialize();
    ctrl->toggle_view();

    return true;
}

