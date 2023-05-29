#ifndef APP_H
#define APP_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


class App : public wxApp
{
public:
    virtual bool OnInit() override;
    virtual int  OnExit() override;

private:
    void* m_ctrl;
};

#endif //APP_H
