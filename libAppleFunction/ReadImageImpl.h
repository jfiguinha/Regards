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

class ReadImageImpl
{
public:
    ReadImageImpl( void );
    ~ReadImageImpl( void );

    void GetDimension(const wxString & filePicture, int &width, int &height);
    unsigned char * ReadImage(const wxString & filePicture, int &width, int &height);


};


#endif