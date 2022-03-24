#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <wx/wx.h>


class Controller
{
public:
    Controller() = default;
    virtual ~Controller() = default;

    virtual void initialize() = 0;
    bool toggle_view();

protected:
    wxFrame *frame;

};


#endif //CONTROLLER_H
