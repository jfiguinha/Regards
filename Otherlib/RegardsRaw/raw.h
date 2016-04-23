#pragma once
#ifdef LIBRAW
#include <RegardsRaw.h>
#include <RegardsBitmap.h>
#include <string>
using namespace std;

class CRegardsBitmap;
class CxImage;

class CRaw
{
public:

    static void GetDimensions(const wxString & fileName, int & width, int & height);
    static CRegardsBitmap * GetPicture(const wxString & fileName);
    static CxImage * GetThumbnail(const wxString & fileName);
    
private:
    
    static void Init();
#ifdef __APPLE__
    static CRegardsRaw * libRegardsRaw;
    static void * lib_handle;
#else
	static CRegardsRaw * libRegardsRaw;
#endif

};

#endif