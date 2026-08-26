//
//  Hello.h
//  Regards
//
//  Created by figuinha jacques on 10/08/2015.
//
//



#ifdef __APPLE__

class ReadImageImpl;

class CReadMacOSImage
{
public:
    CReadMacOSImage ( void );
    ~CReadMacOSImage( void );
    void GetDimension(const wxString & filePicture, int &width, int &height);
    unsigned char * ReadImage(const wxString & filePicture, int &width, int &height);
    
private:
    ReadImageImpl * _impl;
};


#endif