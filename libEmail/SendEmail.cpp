#include <header.h>
//
//  SendEmail.cpp
//  Regards
//
//  Created by figuinha jacques on 10/08/2015.
//

#if defined(__APPLE__) || defined(__WXGTK__)

#include "SendEmail.h"
#include "MyEmailInterface.h"

CSendEmail::CSendEmail(void)
{
    _impl = new MyEmailImpl();
}

CSendEmail::~CSendEmail(void)
{
    if (_impl) { delete _impl; _impl = NULL; }
}

void CSendEmail::SendEmail(const wxString& to, const wxString& subject, const wxString& body, const wxString& filePicture)
{
    if (_impl)
        _impl->SendEmail(to, subject, body, filePicture);
}

#endif