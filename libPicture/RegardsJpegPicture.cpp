#include <header.h>
#include "RegardsJpegPicture.h"
#include <turbojpeg.h>

CRegardsJpegPicture::CRegardsJpegPicture(uint8_t * buffer, const int &width, const int &height, const bool &flip)
{
	isValid = true;
	m_lSize = 0;
	this->data = CompressRegardsBitmapToJpeg(buffer, width, height, m_lSize, flip);
	m_iWidth = width;
	m_iHeight = height;

}

uint8_t * CRegardsJpegPicture::CompressRegardsBitmapToJpeg(uint8_t * buffer, int width, int height, unsigned long & outputsize, const bool &flip)
{
	uint8_t * data = nullptr;
	//bool result = true;
	const int JPEG_QUALITY = 75;
	//const int COLOR_COMPONENTS = 3;

	tjhandle _jpegCompressor = tjInitCompress();

	if(flip)
		tjCompress2(_jpegCompressor, buffer, width, 0, height, TJPF_BGRX, &data, &outputsize, TJSAMP_444, JPEG_QUALITY, TJFLAG_FASTDCT | TJFLAG_BOTTOMUP);
	else
		tjCompress2(_jpegCompressor, buffer, width, 0, height, TJPF_BGRX, &data, &outputsize, TJSAMP_444, JPEG_QUALITY, TJFLAG_FASTDCT);

	tjDestroy(_jpegCompressor);

	return data;
}

CRegardsJpegPicture::~CRegardsJpegPicture()
{
	if(data != nullptr)
		tjFree(data);
}


bool CRegardsJpegPicture::IsValid()
{
	return isValid;
}

uint8_t * CRegardsJpegPicture::GetData()
{
	return data;
}

unsigned long CRegardsJpegPicture::GetSize()
{
	return m_lSize;
}

const int CRegardsJpegPicture::GetWidth()
{
	return m_iWidth;
}

const int CRegardsJpegPicture::GetHeight()
{
	return m_iHeight;
}