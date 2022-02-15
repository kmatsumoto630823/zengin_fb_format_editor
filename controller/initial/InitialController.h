#ifndef INITIALCONTROLLER_H
#define INITIALCONTROLLER_H

#include "../Controller.h"
#include "../../view/initial/InitialFrame.h"
#include "../../model/FBDataParser.h"

class InitialController final : public Controller
{
public:
    InitialController() = default;
    ~InitialController() = default;

private:
    void create_frame() override;
    void create_binds() override;

    void reset_grid();

    InitialFrame* get_casted_frame(){ return (InitialFrame*) frame; }

    FBDataParser fb;

    InitialController(const InitialController&) = delete;
    InitialController& operator=(const InitialController&) = delete;

    InitialController(InitialController&&) = delete;
    InitialController& operator=(InitialController&&) = delete;
};


#endif //INITIALCONTROLLER_H
