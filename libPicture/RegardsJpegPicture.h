#pragma once

class CRegardsJpegPicture
{
public:

	CRegardsJpegPicture(uint8_t * buffer, const int &width, const int &height);
	virtual ~CRegardsJpegPicture();

	bool IsValid();

	uint8_t * GetData();
	unsigned long GetSize();

	const int GetWidth();
	const int GetHeight();

	

protected:

	uint8_t * CompressRegardsBitmapToJpeg(uint8_t * buffer, int width, int height, unsigned long & outputsize);
	int m_iWidth;
	int m_iHeight;
	unsigned long m_lSize;
	uint8_t * data = nullptr;
	bool isValid = false;

};
