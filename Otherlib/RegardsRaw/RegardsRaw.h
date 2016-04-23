#pragma once
#include <stdint.h>
#include <string>
#include <vector>
using namespace std;


#define JPEGOUTPUT 1
#define BITMAPOUTPUT 2

class CRegardsRaw
{
public:
	CRegardsRaw();
	~CRegardsRaw();

	virtual void GetDimensions(const string & fileName, int & width, int & height);
	virtual std::vector<uint8_t> GetPicture(const string & fileName, int & width, int & height);
	virtual std::vector<uint8_t> GetThumbnail(const string & fileName, int &outputFormat);
};

