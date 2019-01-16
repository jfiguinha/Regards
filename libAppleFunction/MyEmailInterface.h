//
//  HelloInterface.h
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

class MyEmailImpl
{
public:
    MyEmailImpl ( void );
    ~MyEmailImpl( void );

    void SendEmail(const wxString &to, const wxString &subject, const wxString & filePicture);


};


#endif