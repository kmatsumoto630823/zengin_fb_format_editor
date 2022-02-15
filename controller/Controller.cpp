#include "Controller.h"

void Controller::create_frame_and_binds()
{
    create_frame();
    create_binds();
}

bool Controller::toggle_view()
{
    return frame->IsFullScreen() ? frame->Show(false) : frame->Show(true);
}
