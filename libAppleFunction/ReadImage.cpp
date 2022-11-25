//
//  Hello.cpp
//  Regards
//
//  Created by figuinha jacques on 10/08/2015.
//
//

#ifdef __APPLE__
#include "ReadImage.h"
#include "ReadImageImpl.h"


CReadMacOSImage::CReadMacOSImage( void )
{
    _impl = new ReadImageImpl();
}


CReadMacOSImage::~CReadMacOSImage( void )
{
    if ( _impl ) { delete _impl; _impl = NULL; }
}

void CReadMacOSImage::GetDimension(const wxString & filePicture, int &width, int &height)
{
     std::cout << "CReadMacOSImage::GetDimension " << std::endl;
     if ( _impl )
        return _impl->GetDimension(filePicture, width, height);   
}



unsigned char * CReadMacOSImage::ReadImage(const wxString & filePicture, int &width, int &height)
{
     std::cout << "CReadMacOSImage::ReadImage " << std::endl;
    if ( _impl )
        return _impl->ReadImage(filePicture, width, height);
        
    return nullptr;
}
#endif