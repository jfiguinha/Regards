#pragma once
#include <string>
using namespace std;
class LibRaw;
class CImageLoadingFormat;
class CDecodeRawParameter;

class CDecodeRawPicture
{
public:
	CDecodeRawPicture(const string & fileName);
	~CDecodeRawPicture();
	CImageLoadingFormat * DecodePicture(CDecodeRawParameter * decodeRawParameter);

private:
	LibRaw * rawProcessor;
	int result;
	unsigned char *buffer;

};