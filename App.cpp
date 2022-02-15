#include "App.h"
#include "controller/initial/InitialController.h"


wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    Controller *ctrl = (Controller*) new InitialController();
    ctrl->create_frame_and_binds();
    ctrl->toggle_view();

    return true;
}

