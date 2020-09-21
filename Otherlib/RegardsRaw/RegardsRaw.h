#pragma once
#include <string>
using namespace std;

#define JPEGOUTPUT 1
#define BITMAPOUTPUT 2
#define NOTHUMBNAIL 3

class CxMemFile;
class CxImage;

class CRegardsRaw
{
public:
	CRegardsRaw();
	~CRegardsRaw();
	static CxImage * GetPicture(const string & fileName);
	static void GetDimensions(const string & fileName, int & width, int & height);
	static CxMemFile * GetThumbnail(const string & fileName, int &outputFormat);
};

