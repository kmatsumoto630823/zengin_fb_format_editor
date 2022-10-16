#ifndef MY_ABOUT_DIALOG_INFO_H
#define MY_ABOUT_DIALOG_INFO_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/aboutdlg.h>

class MyAboutDialogInfo
{
public:
    MyAboutDialogInfo();
    ~MyAboutDialogInfo() = default;

    void myAboutBox(wxWindow* parent);

private:
    wxAboutDialogInfo aboutinfo;
};

#endif // MY_ABOUT_DIALOG_INFO_H