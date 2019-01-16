#pragma once
class LibRaw;
class CImageLoadingFormat;
class CDecodeRawParameter;

class CDecodeRawPicture
{
public:
	CDecodeRawPicture(const string & fileName);
	~CDecodeRawPicture();
	int DecodePicture(CDecodeRawParameter * decodeRawParameter);
	CImageLoadingFormat * GetPicture();

private:
	CImageLoadingFormat * imageLoadingFormat;
	LibRaw * rawProcessor;
	int result;
	unsigned char *buffer;

};