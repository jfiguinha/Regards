#include <header.h>
//
//  Hello.cpp
//  Regards
//
//  Created by figuinha jacques on 10/08/2015.
//
//

#ifdef __APPLE__
#include "SendEmail.h"
#include "MyEmailInterface.h"

CSendEmail::CSendEmail( void )
{
    _impl = new MyEmailImpl();
}


CSendEmail::~CSendEmail( void )
{
    if ( _impl ) { delete _impl; _impl = NULL; }
}


void CSendEmail::SendEmail(const wxString &to,const wxString &subject, const wxString & filePicture)
{
    if ( _impl )
        _impl->SendEmail(to, subject, filePicture);
}
#endif
