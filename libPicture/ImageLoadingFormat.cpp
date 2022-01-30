// ReSharper disable All
#include <header.h>
#include "ImageLoadingFormat.h"
#include <libPicture.h>
#include <ximage.h>
#include "RegardsBitmap.h"
#include <wx/mstream.h>
#include <wx/wfstream.h>
#include <InterpolationBicubic.h>
#include <InterpolationFloatBicubic.h>
#include <turbojpeg.h>
#include "RegardsJpegPicture.h"
#ifdef ROTDETECT
#include <rotdetect.h>
#endif
#if defined(EXIV2)
#include <MetadataExiv2.h>
using namespace Regards::exiv2;
#endif
using namespace Regards::Picture;
#include "RegardsFloatBitmap.h"

extern float value[256];

void CImageLoadingFormat::SetMemoryDelete(const bool& memoryDelete)
{
	this->memoryDelete = memoryDelete;
}

CImageLoadingFormat::CImageLoadingFormat(const bool& memoryDelete): format(0), orientation(0)
{
	_image = nullptr;

	convertToRGB24 = false;
	convertToBGR = false;
	this->memoryDelete = memoryDelete;
}

CImageLoadingFormat::~CImageLoadingFormat()
{
	if (memoryDelete)
	{
		DeletePicture();
	}
}

int CImageLoadingFormat::GetFormat()
{
	return format;
}

void CImageLoadingFormat::DeletePicture()
{
	if (_image != nullptr)
		delete _image;
	_image = nullptr;
}

int CImageLoadingFormat::GetWidth()
{
	if (!IsOk())
		return 0;

	return _image->GetBitmapWidth();
}

int CImageLoadingFormat::GetHeight()
{
	if (!IsOk())
		return 0;

	return _image->GetBitmapHeight();
}

void CImageLoadingFormat::Mirror()
{
	if (!IsOk())
		return;
	_image->HorzFlipBuf();
}

void CImageLoadingFormat::Rotate90()
{
	if (!IsOk())
		return;

	_image->Rotate90();
}

void CImageLoadingFormat::Flip()
{
	if (!IsOk())
		return;
	_image->VertFlipBuf();
}


int CImageLoadingFormat::GetOrientation()
{
	if (!IsOk())
		return 0;

	return orientation;
}

wxString CImageLoadingFormat::GetFilename()
{
	return filename;
}

int CImageLoadingFormat::GetResolution()
{
	return resolution;
}

/*
From exiftool documentation

1 = Horizontal (normal)
2 = Mirror horizontal
3 = Rotate 180
4 = Mirror vertical
5 = Mirror horizontal and rotate 270 CW
6 = Rotate 90 CW
7 = Mirror horizontal and rotate 90 CW
8 = Rotate 270 CW

*/

void CImageLoadingFormat::SetOrientation(const int& orientation)
{
	_image->RotateExif(orientation);
	this->orientation = 0;
}


void CImageLoadingFormat::SetFilename(const wxString& filename)
{
	this->filename = wxString(filename);
}

/*
void CImageLoadingFormat::SetPicturToJpeg(CRegardsBitmap* image, const bool& flip)
{
	if (image != nullptr)
	{
		format = TYPE_IMAGE_REGARDSJPEGIMAGE;
		_jpegImage = new CRegardsJpegPicture(image->GetPtBitmap(), image->GetBitmapWidth(), image->GetBitmapHeight(),
		                                     flip);
		filename = image->GetFilename();
	}
}
*/

void CImageLoadingFormat::UpdatePicture(CRegardsBitmap* image)
{
	DeletePicture();
	_image = image;
}

void CImageLoadingFormat::SetPicture(CRegardsBitmap* image, const bool& convertToRGB24)
{
	if (image != nullptr)
	{
		_image = image;
		_image->RotateExif(image->GetOrientation());
		orientation = 0;// image->GetOrientation();
		if (orientation < 0)
			orientation = 0;
		filename = _image->GetFilename();
		this->convertToRGB24 = convertToRGB24;
	}
}

void CImageLoadingFormat::SetPicture(CxImage* image)
{
	if (image != nullptr)
	{
		if (_image != nullptr)
			delete _image;

		CLibPicture libPicture;
		_image = libPicture.ConvertCXImageToRegardsBitmap(image, 0);
	}
}

void CImageLoadingFormat::SetPicture(wxImage* image)
{
	if (image != nullptr)
	{
		if (_image != nullptr)
			delete _image;

		CLibPicture libPicture;
		_image = libPicture.ConvertwxImageToRegardsBitmap(*image);
	}
}

void CImageLoadingFormat::SetPicture(CRegardsFloatBitmap* image)
{
	if (image != nullptr)
	{
		if (_image != nullptr)
			delete _image;

		_image = new CRegardsBitmap(GetWidth(), GetHeight());
		uint8_t* buffer = _image->GetPtBitmap();
		float* data = image->GetData();
		for (long y = 0; y < GetHeight(); y++)
		{
			for (long x = 0; x < GetWidth(); x++)
			{
				int position = x * 4 + y * GetWidth() * 4;
				*buffer++ = max(0, min(static_cast<int32_t>(data[position + 2] * 255), 255));
				*buffer++ = max(0, min(static_cast<int32_t>(data[position + 1] * 255), 255));
				*buffer++ = max(0, min(static_cast<int32_t>(data[position] * 255), 255));
				*buffer++ = max(0, min(static_cast<int32_t>(data[position + 3] * 255), 255));
			}
		}

	}
}

CRegardsFloatBitmap* CImageLoadingFormat::GetFloatBitmap(const bool& copy)
{
	if (!IsOk())
	{
		return nullptr;
	}

	CLibPicture libPicture;
	CRegardsFloatBitmap* _local = nullptr;

	_local = new CRegardsFloatBitmap(GetWidth(), GetHeight());
	float* buffer = _local->GetData();
	for (long y = 0; y < _image->GetBitmapHeight(); y++)
	{
		for (long x = 0; x < _image->GetBitmapWidth(); x++)
		{
			CRgbaquad color = _image->GetColorValue(x, y);
			*buffer++ = value[color.GetRed()] / 255.0f;
			*buffer++ = value[color.GetGreen()] / 255.0f;
			*buffer++ = value[color.GetBlue()] / 255.0f;
			*buffer++ = value[color.GetAlpha()] / 255.0f;
		}
	}

	return _local;
}

CRegardsBitmap* CImageLoadingFormat::GetRegardsBitmap(const bool& copy)
{
	if (!IsOk())
	{
		return nullptr;
	}

	if (copy)
	{
		auto _local = new CRegardsBitmap();
		*_local = *_image;
		return _local;
	}
	return _image;
}

void CImageLoadingFormat::DestroyJpegData(uint8_t* & data)
{
#ifdef TURBOJPEG
	tjFree(data);
#else
	delete[] data;
#endif

}

void CImageLoadingFormat::SaveToJpeg(const wxString& filename)
{
	int compressMethod = 0;
	unsigned long outputsize = 0;
	convertToRGB24 = true;
	uint8_t* data = GetJpegData(outputsize);
	wxFileOutputStream outputStream(filename);
	outputStream.Write(data, outputsize);
	outputStream.Close();
	DestroyJpegData(data);
}

void CImageLoadingFormat::ConvertToBGR(const bool& convert)
{
	convertToBGR = convert;
}

void CImageLoadingFormat::ConvertToRGB24(const bool& convert)
{
	convertToRGB24 = convert;
}

uint8_t* CImageLoadingFormat::CompressRegardsBitmapToJpeg(unsigned char* buffer, int width, int height,
                                                          unsigned long& outputsize)
{
	uint8_t* data = nullptr;
	//bool result = true;
	const int JPEG_QUALITY = 75;
	//const int COLOR_COMPONENTS = 3;

	tjhandle _jpegCompressor = tjInitCompress();

	if (convertToRGB24)
	{
		tjCompress2(_jpegCompressor, buffer, width, 0, height, TJPF_BGRX,
		            &data, &outputsize, TJSAMP_444, JPEG_QUALITY,
		            TJFLAG_FASTDCT | TJFLAG_BOTTOMUP);
	}
	else
	{
		tjCompress2(_jpegCompressor, buffer, width, 0, height, TJPF_RGBX,
		            &data, &outputsize, TJSAMP_444, JPEG_QUALITY,
		            TJFLAG_FASTDCT);
	}
	tjDestroy(_jpegCompressor);

	return data;
}


uint8_t* CImageLoadingFormat::GetJpegData(unsigned long& outputsize)
{
	if (!IsOk())
	{
		outputsize = 0;
		return nullptr;
	}

	uint8_t* data = nullptr;

	data = CompressRegardsBitmapToJpeg(_image->GetPtBitmap(), _image->GetBitmapWidth(),
			                            _image->GetBitmapHeight(), outputsize);
	
	return data;
}


float CImageLoadingFormat::CalculPictureRatio(const int& pictureWidth, const int& pictureHeight)
{
	if (pictureWidth == 0 && pictureHeight == 0)
		return 1.0f;

	float newRatio = 1;

	//int tailleAffichageWidth = 0, tailleAffichageHeight = 0;

	if (pictureWidth > pictureHeight)
		newRatio = static_cast<float>(pictureWidth) / static_cast<float>(GetWidth());
	else
		newRatio = static_cast<float>(pictureHeight) / static_cast<float>(GetHeight());

	if ((GetHeight() * newRatio) > pictureHeight)
	{
		newRatio = static_cast<float>(pictureHeight) / static_cast<float>(GetHeight());
	}

	if ((GetWidth() * newRatio) > pictureWidth)
	{
		newRatio = static_cast<float>(pictureWidth) / static_cast<float>(GetWidth());
	}

	return newRatio;
}


bool CImageLoadingFormat::IsOk()
{

	if (_image == nullptr)
		return false;
	return _image->IsValid();

}


CImageLoadingFormat* CImageLoadingFormat::GetPage(const int& numPage)
{
	CLibPicture libPicture;
	CImageLoadingFormat* imageLoading = libPicture.LoadPicture(filename, false, numPage);
	return imageLoading;
}

int CImageLoadingFormat::GetNbPage()
{
	CLibPicture libPicture;
	int nbPage = libPicture.GetNbImage(filename);
	return nbPage;
}

int CImageLoadingFormat::Resize(const int& pictureWidth, const int& pictureHeight, const int& method)
{
	if (!IsOk())
		return -1;

	float newRatio = CalculPictureRatio(pictureWidth, pictureHeight);
	int thumbnailWidth = this->GetWidth() * newRatio;
	int thumbnailHeight = this->GetHeight() * newRatio;

	if (thumbnailWidth > 0 && thumbnailHeight > 0)
	{
		CInterpolationBicubic interpolation;
		auto bitmapOut = new CRegardsBitmap(thumbnailWidth, thumbnailHeight);
		interpolation.Execute(_image, bitmapOut);
		delete _image;
		_image = bitmapOut;

	}
	return 0;
}

wxImage* CImageLoadingFormat::GetwxImage(const bool& copy)
{
	if (!IsOk())
		return nullptr;

	CLibPicture libPicture;
	return libPicture.ConvertRegardsBitmapToWXImage(_image);
}

CxImage* CImageLoadingFormat::GetCxImage(const bool& copy)
{
	if (!IsOk())
		return nullptr;

	CLibPicture libPicture;
	return libPicture.ConvertRegardsBitmapToCXImage(_image);
}
