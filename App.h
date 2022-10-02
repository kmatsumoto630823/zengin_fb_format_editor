#ifndef APP_H
#define APP_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


class App : public wxApp
{
    virtual bool OnInit() override;
};

#endif //APP_H
