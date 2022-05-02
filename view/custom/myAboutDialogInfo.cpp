#include "myAboutDialogInfo.h"

myAboutDialogInfo::myAboutDialogInfo()
{
    aboutinfo.SetName("Zengin FB Format Editor");
    aboutinfo.SetVersion("1.0.4");
    aboutinfo.SetDescription("全銀協が定めるFBデータ（固定長フォーマット）を編集するためのアプリです");
    aboutinfo.SetCopyright("(C) 2022-2022 kmatsumoto630823");
    aboutinfo.SetWebSite("https://github.com/kmatsumoto630823/zengin_fb_format_editor/");
    aboutinfo.AddDeveloper("kmatsumoto630823(k.matsumoto.s630823@gmail.com)");
    aboutinfo.SetLicence(
R"***(MIT License

Copyright (c) 2022 kmatsumoto630823

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.)***");

}

void myAboutDialogInfo::myAboutBox(wxWindow *parent)
{
    wxAboutBox(aboutinfo, parent);
}