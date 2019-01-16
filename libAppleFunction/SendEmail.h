//
//  Hello.h
//  Regards
//
//  Created by figuinha jacques on 10/08/2015.
//
//



#ifdef __APPLE__
#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class MyEmailImpl;

class CSendEmail
{
public:
    CSendEmail ( void );
    ~CSendEmail( void );
    
    void SendEmail(const wxString &to,const wxString &subject, const wxString & filePicture);
    
private:
    MyEmailImpl * _impl;
};

#endif