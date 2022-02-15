#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <wx/wx.h>


class Controller
{
public:
    Controller() = default;
    virtual ~Controller() = default;

    void create_frame_and_binds();
    bool toggle_view();

protected:
    wxFrame *frame;

private:
    virtual void create_frame() = 0;
    virtual void create_binds() = 0;

    Controller(const Controller&) = delete;
    Controller& operator=(const Controller&) = delete;

    Controller(Controller&&) = delete;
    Controller& operator=(Controller&&) = delete;

};


#endif //CONTROLLER_H
