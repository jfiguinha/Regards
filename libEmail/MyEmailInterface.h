//
//  MyEmailInterface.h
//  Regards
//
#if defined(__APPLE__) || defined(__WXGTK__)
#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class MyEmailImpl
{
public:
    void SendEmail(const wxString& to, const wxString& subject, const wxString& body,const wxString& filePicture);
};

#endif