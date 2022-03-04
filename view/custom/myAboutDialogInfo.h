#ifndef _MY_ABOUT_DIALOG_INFO_H_
#define _MY_ABOUT_DIALOG_INFO_H_

#include <wx/aboutdlg.h>

class myAboutDialogInfo
{
public:
    myAboutDialogInfo();
    ~myAboutDialogInfo() = default;

    void myAboutBox(wxWindow *parent);

private:
    wxAboutDialogInfo aboutinfo;
};

#endif // _MY_ABOUT_DIALOG_INFO_H_