#ifndef INITIALCONTROLLER_H
#define INITIALCONTROLLER_H

#include "../Controller.h"
#include "../../view/initial/InitialFrame.h"
#include "../../model/FBParser.h"

class InitialController final : public Controller
{
public:
    InitialController() = default;
    ~InitialController() = default;

private:
    void create_frame() override;
    void create_binds() override;

    void reset_grid();
    bool is_edited();

    InitialFrame* get_casted_frame(){ return (InitialFrame*) frame; }
    FBParser fb;
};

#endif //INITIALCONTROLLER_H
