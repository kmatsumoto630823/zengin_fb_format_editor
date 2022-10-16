#include "Controller.h"

bool Controller::toggle_view()
{
    return frame->IsFullScreen() ? frame->Show(false) : frame->Show(true);
}
