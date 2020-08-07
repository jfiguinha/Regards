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

#include "RegardsFloatBitmap.h"

extern float value[256];

void CImageLoadingFormat::SetMemoryDelete(const bool &memoryDelete)
{
	this->memoryDelete = memoryDelete;
}

CImageLoadingFormat::CImageLoadingFormat(const bool &memoryDelete)
{
	_image = nullptr;
	_cxImage = nullptr;
	_wxImage = nullptr;
	_floatImage = nullptr;
	_jpegImage = nullptr;
	convertToRGB24 = false;
	convertToBGR = false;
	this->memoryDelete = memoryDelete;
}

CImageLoadingFormat::~CImageLoadingFormat()
{
	if(memoryDelete)
	{
		if(_image != nullptr)
			delete _image;
		_image = nullptr;

		if(_cxImage != nullptr)
			delete _cxImage;
		_cxImage = nullptr;

		if(_wxImage != nullptr)
			delete _wxImage;
		_wxImage = nullptr;

		if(_floatImage != nullptr)
			delete _floatImage;
		_floatImage = nullptr;

		if (_jpegImage != nullptr)
			delete _jpegImage;
		_jpegImage = nullptr;
	}
}

int CImageLoadingFormat::GetFormat()
{
	return format;
}



int CImageLoadingFormat::GetWidth()
{
	if(!IsOk())
		return 0;

	switch(format)
	{
	case TYPE_IMAGE_CXIMAGE:
		{
			return _cxImage->GetWidth();
		}
		break;

	case TYPE_IMAGE_WXIMAGE:
		{
			return _wxImage->GetWidth();
		}
		break;

	case TYPE_IMAGE_REGARDSIMAGE:
		{
			return _image->GetBitmapWidth();
		}
		break;

	case TYPE_IMAGE_REGARDSFLOATIMAGE:
		{
			return _floatImage->GetWidth();
		}
		break;
	case TYPE_IMAGE_REGARDSJPEGIMAGE:
		{
			return _jpegImage->GetWidth();
		}
		break;
	}
	return 0;
}

int CImageLoadingFormat::GetHeight()
{
	if(!IsOk())
		return 0;

	switch(format)
	{
	case TYPE_IMAGE_CXIMAGE:
		{
			return _cxImage->GetHeight();
		}
		break;

	case TYPE_IMAGE_WXIMAGE:
		{
			return _wxImage->GetHeight();
		}
		break;

	case TYPE_IMAGE_REGARDSIMAGE:
		{
			return _image->GetBitmapHeight();
		}
		break;

	case TYPE_IMAGE_REGARDSFLOATIMAGE:
		{
			return _floatImage->GetHeight();
		}
		break;
	case TYPE_IMAGE_REGARDSJPEGIMAGE:
		{
			return _jpegImage->GetHeight();
		}
		break;
	}
	return 0;
}

void CImageLoadingFormat::Mirror()
{
	if(!IsOk())
		return;

	switch(format)
	{
	case TYPE_IMAGE_CXIMAGE:
		{
			_cxImage->Mirror();
		}
		break;

	case TYPE_IMAGE_WXIMAGE:
		{
			_wxImage->Mirror();
		}
		break;

	case TYPE_IMAGE_REGARDSIMAGE:
		{
			_image->HorzFlipBuf();
		}
		break;

	case TYPE_IMAGE_REGARDSFLOATIMAGE:
		{
			_floatImage->HorzFlipBuf();
		}
		break;
	}
}

void CImageLoadingFormat::Rotate90()
{
	if(!IsOk())
		return;

	switch(format)
	{
	case TYPE_IMAGE_CXIMAGE:
		{
			_cxImage->Rotate(90);
		}
		break;

	case TYPE_IMAGE_WXIMAGE:
		{
			_wxImage->Rotate90();
		}
		break;

	case TYPE_IMAGE_REGARDSIMAGE:
		{
			_image->Rotation90();
		}
		break;

	case TYPE_IMAGE_REGARDSFLOATIMAGE:
		{
			_floatImage->Rotate90();
		}
		break;
	}
}

void CImageLoadingFormat::Flip()
{
	if(!IsOk())
		return;

	switch(format)
	{
	case TYPE_IMAGE_CXIMAGE:
		{
			_cxImage->Flip();
		}
		break;

	case TYPE_IMAGE_WXIMAGE:
		{
			_wxImage->Mirror(false);
		}
		break;

	case TYPE_IMAGE_REGARDSIMAGE:
		{
			_image->VertFlipBuf();
		}
		break;

	case TYPE_IMAGE_REGARDSFLOATIMAGE:
		{
			_floatImage->VertFlipBuf();
		}
		break;
	}
}


int CImageLoadingFormat::GetOrientation()
{
	if(!IsOk())
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

void CImageLoadingFormat::SetOrientation(const int &orientation)
{
	this->orientation = orientation;
}


void CImageLoadingFormat::SetFilename(const wxString &filename)
{
	this->filename = wxString(filename);
}

void CImageLoadingFormat::SetPicturToJpeg(CRegardsBitmap * image)
{
	if (image != nullptr)
	{
		format = TYPE_IMAGE_REGARDSJPEGIMAGE;
		_jpegImage = new CRegardsJpegPicture(image->GetPtBitmap(), image->GetBitmapWidth(), image->GetBitmapHeight());
		filename = image->GetFilename();
	}
}


void CImageLoadingFormat::SetPicture(CRegardsBitmap * image, const bool &convertToRGB24)
{
	if(image != nullptr)
	{
		format = TYPE_IMAGE_REGARDSIMAGE;
		_image = image;
		orientation = image->GetOrientation();
		if (orientation < 0)
			orientation = 0;
		filename = _image->GetFilename();
		this->convertToRGB24 = convertToRGB24;
	}
}

void CImageLoadingFormat::SetPicture(CxImage * image)
{
	if(image != nullptr)
	{
		_cxImage = image;
		format = TYPE_IMAGE_CXIMAGE;
	}
}

void CImageLoadingFormat::SetPicture(wxImage * image)
{
	if(image != nullptr)
	{
		_wxImage = image;
		format = TYPE_IMAGE_WXIMAGE;
	}
}

void CImageLoadingFormat::SetPicture(CRegardsFloatBitmap * image)
{
	if(image != nullptr)
	{
		_floatImage = image;
		format = TYPE_IMAGE_REGARDSFLOATIMAGE;
	}
}

CRegardsFloatBitmap * CImageLoadingFormat::GetFloatBitmap(const bool &copy)
{
	if(!IsOk())
	{
		return nullptr;
	}

	CLibPicture libPicture;
	CRegardsFloatBitmap * _local = nullptr;
	switch(format)
	{
		case TYPE_IMAGE_WXIMAGE:
			{
				_local = new CRegardsFloatBitmap(GetWidth(), GetHeight());
				CxImage * _image = libPicture.ConvertwxImageToCxImage(*_wxImage);
				_image->Encode2BGRAFloat(_local->GetData(), _local->GetSize(), true);
			}
			break;

		case TYPE_IMAGE_CXIMAGE:
			{
				_local = new CRegardsFloatBitmap(GetWidth(), GetHeight());
				_cxImage->Encode2BGRAFloat(_local->GetData(), _local->GetSize(), true);
			}
			break;

		case TYPE_IMAGE_REGARDSIMAGE:
			{
				_local = new CRegardsFloatBitmap(GetWidth(), GetHeight());
				float * buffer = _local->GetData();
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
			}
			break;

		case TYPE_IMAGE_REGARDSFLOATIMAGE:
			{
				if(copy)
				{
					CRegardsFloatBitmap * _local = new CRegardsFloatBitmap(this->GetWidth(), this->GetHeight());
					memcpy(_local->GetData(), _floatImage->GetData(), _floatImage->GetSize());
					return _local;
				}
				else
					return _floatImage;
			}
			break;
	}
	return _local;
}

CRegardsBitmap * CImageLoadingFormat::GetRegardsBitmap(const bool &copy)
{
	if(!IsOk())
	{
		return nullptr;
	}

	CLibPicture libPicture;
	switch(format)
	{
	case TYPE_IMAGE_CXIMAGE:
		{
			CRegardsBitmap * _local = libPicture.ConvertCXImageToRegardsBitmap(_cxImage,0);
			return _local;
		}

	case TYPE_IMAGE_WXIMAGE:
		{
			return libPicture.ConvertwxImageToRegardsBitmap(*_wxImage);
		}
		break;

	case TYPE_IMAGE_REGARDSIMAGE:
		{
			if(copy)
			{
				CRegardsBitmap * _local = new CRegardsBitmap();
				*_local = *_image;
				return _local;
			}
			else
				return _image;
		}
		break;

	case TYPE_IMAGE_REGARDSFLOATIMAGE:
		{
			CRegardsBitmap * _local = new CRegardsBitmap(GetWidth(), GetHeight());
			uint8_t * buffer = _local->GetPtBitmap();
			float * data = _floatImage->GetData();
			for (long y = 0; y < GetHeight(); y++)
			{
				for (long x = 0; x < GetWidth(); x++)
				{
                    int position = x * 4 + y * GetWidth() * 4;
                    *buffer++ = max(0,min(data[position + 2] * 255,255));
					*buffer++ = max(0,min(data[position + 1] * 255,255));
					*buffer++ = max(0,min(data[position] * 255,255));
					*buffer++ = max(0,min(data[position + 3] * 255,255));
				}
			}
            return _local;
		}
		break;
	}
	return nullptr;
}

void CImageLoadingFormat::DestroyJpegData(uint8_t * data, const int & compressMethod)
{
	if (compressMethod == 1)
	{
	#ifdef TURBOJPEG
			tjFree(data);
	#else
			delete[] data;
	#endif	
	}
	else
	{

		switch (format)
		{
		case TYPE_IMAGE_CXIMAGE:
			_cxImage->FreeMemory(data);
			break;
		case TYPE_IMAGE_REGARDSIMAGE:
#ifdef TURBOJPEG
			tjFree(data);
#else
			delete[] data;
#endif
			break;
		default:
			if (data != nullptr)
				delete[] data;
			break;
		}
	}
	data = nullptr;
}

void CImageLoadingFormat::SaveToJpeg(const wxString &filename)
{
	int compressMethod = 0;
	unsigned long outputsize = 0;
	convertToRGB24 = true;
	uint8_t * data = GetJpegData(outputsize, compressMethod);
	wxFileOutputStream outputStream(filename);
	outputStream.Write(data, outputsize);
	outputStream.Close();
	DestroyJpegData(data, compressMethod);
}

void CImageLoadingFormat::ConvertToBGR(const bool &convert)
{
	convertToBGR = convert;
}

void CImageLoadingFormat::ConvertToRGB24(const bool &convert)
{
	convertToRGB24 = convert;
}

uint8_t * CImageLoadingFormat::CompressRegardsBitmapToJpeg(unsigned char * buffer, int width, int height, unsigned long & outputsize)
{
	uint8_t * data = nullptr;
	//bool result = true;
	const int JPEG_QUALITY = 75;
	const int COLOR_COMPONENTS = 3;

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


uint8_t * CImageLoadingFormat::GetJpegData(unsigned long & outputsize, int &compressMethod)
{
	if(!IsOk())
	{
		outputsize = 0;
		return nullptr;
	}

	compressMethod = 0;
	uint8_t * data = nullptr;
	switch(format)
	{
	case TYPE_IMAGE_REGARDSJPEGIMAGE:
	{
		if (_jpegImage != nullptr)
		{
			outputsize = _jpegImage->GetSize();
			data = new uint8_t[outputsize];
			compressMethod = 0;
			memcpy(data, _jpegImage->GetData(), outputsize);
			return data;
		}
	}
	break;
	case TYPE_IMAGE_CXIMAGE:
		{
			data = NULL;
			int32_t size = 0;
			if (_cxImage->IsIndexed() || _cxImage->IsGrayScale())
			{

				CRegardsBitmap * bitmap = GetRegardsBitmap();
				data = CompressRegardsBitmapToJpeg(bitmap->GetPtBitmap(), bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), outputsize);
				compressMethod = 1;
				delete bitmap;
			}
			else
			{
				compressMethod = 2;
				_cxImage->Encode(data, size, CxImage::GetTypeIdFromName("jpg"));
				outputsize = size;
			}
		}
		break;

	case TYPE_IMAGE_WXIMAGE:
		{
			wxMemoryOutputStream outputStream;
			_wxImage->SaveFile(outputStream, wxBITMAP_TYPE_JPEG);
			outputsize = outputStream.GetSize();
			data = new uint8_t[outputsize];
			compressMethod = 0;
			outputStream.CopyTo(data, outputStream.GetSize());
		}
		break;


	case TYPE_IMAGE_REGARDSIMAGE:
		{
			data =  CompressRegardsBitmapToJpeg(_image->GetPtBitmap(), _image->GetBitmapWidth(), _image->GetBitmapHeight(), outputsize);
			compressMethod = 1;
		}
		break;
        
	case TYPE_IMAGE_REGARDSFLOATIMAGE:
		{
            int width =  _floatImage->GetWidth();
            int height =  _floatImage->GetHeight();
            uint8_t * inputBuffer = new uint8_t[width * height * 4];
            float * buffer = _floatImage->GetData();
            int k = 0;
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++, k+=4)
                {
                    inputBuffer[k] = (int)(buffer[k+2] * 255.0);
                    inputBuffer[k+1] = (int)(buffer[k+1] * 255.0);
                    inputBuffer[k+2] = (int)(buffer[k] * 255.0);
                    inputBuffer[k+3] = (int)(buffer[k+3] * 255.0);    
                }
            }
			data = CompressRegardsBitmapToJpeg(inputBuffer,width, height, outputsize);
            delete[] inputBuffer;
			compressMethod = 1;
		}
		break;        
	}
	return data;
}


float CImageLoadingFormat::CalculPictureRatio(const int &pictureWidth, const int &pictureHeight)
{
	if (pictureWidth == 0 && pictureHeight == 0)
		return 1.0f;

	float newRatio = 1;

	//int tailleAffichageWidth = 0, tailleAffichageHeight = 0;

	if (pictureWidth > pictureHeight)
		newRatio = (float)(pictureWidth) / (float)GetWidth();
	else
		newRatio = (float)(pictureHeight) / (float)GetHeight();

	if ((GetHeight() * newRatio) > pictureHeight)
	{
		newRatio = (float)(pictureHeight) / (float)GetHeight();
	}

	if ((GetWidth() * newRatio) > pictureWidth)
	{
		newRatio =  (float)(pictureWidth) / (float)GetWidth();
	}

	return newRatio;
}


bool CImageLoadingFormat::IsOk()
{
		switch(format)
		{
		case TYPE_IMAGE_CXIMAGE:
			{
				if(_cxImage == nullptr)
					return false;
				return _cxImage->IsValid();
			}
			break;

		case TYPE_IMAGE_WXIMAGE:
			{
				if(_wxImage == nullptr)
					return false;
				return _wxImage->IsOk();

			}
			break;

		case TYPE_IMAGE_REGARDSIMAGE:
			{
				if(_image == nullptr)
					return false;
				return _image->IsValid();
			}
			break;

		case TYPE_IMAGE_REGARDSFLOATIMAGE:
			{
				if(_floatImage == nullptr)
					return false;
				return _floatImage->IsValid();
			}
			break;

		case TYPE_IMAGE_REGARDSJPEGIMAGE:
			{
				if (_jpegImage == nullptr)
					return false;
				return _jpegImage->IsValid();
			}
			break;
		}
	return false;
}

void CImageLoadingFormat::ApplyExifOrientation()
{
	if(!IsOk())
		return;

	
	int exifOrientation = 0;
	CLibPicture libPicture;
	if(libPicture.TestIsExifCompatible(filename))
	{
		CMetadataExiv2 metadata(filename);
		exifOrientation = metadata.GetOrientation();
	}
#ifdef ROTDETECT
	if (orientation == -1 || orientation == 1)
	{
		float result[4];
		CRotDetect rotDetect;
		rotDetect.rotdetect(this, result, false);

		CRotDetect::rotation rot = rotDetect.analyzeResult(result);
		//printf("Orientation of %s is ", argv[optIndex]);
		switch (rot)
		{
		case CRotDetect::NOT_ROTATED:
			exifOrientation = 0;
			break;
		case CRotDetect::ROTATED90CW:
			exifOrientation = 6;
			break;
		case CRotDetect::ROTATED90CCW:
			exifOrientation = 3;
			break;
		}
	}
#endif
	ApplyExifOrientation(exifOrientation);
}


void CImageLoadingFormat::ApplyExifOrientation(const int &exifMethod)
{
	if (!IsOk())
		return;

	switch (format)
	{
	case TYPE_IMAGE_CXIMAGE:
	{
		printf("TYPE_IMAGE_CXIMAGE Exif Method : %d \n", exifMethod);
		_cxImage->RotateExif(exifMethod);
	}
	break;

	case TYPE_IMAGE_WXIMAGE:
	{
		printf("TYPE_IMAGE_WXIMAGE Exif Method : %d \n", exifMethod);
		switch (exifMethod)
		{
		case 1:// top left side
			break;
		case 2:// top right side
			break;
		case 3:// bottom right side
		{
			wxImage image = _wxImage->Rotate180();
			delete _wxImage;
			_wxImage = new wxImage(image);
		}
		break;
		case 4:// bottom left side
			break;
		case 5://left side top
			break;
		case 6:// right side top
		{
			wxImage image = _wxImage->Rotate180();
			image = image.Rotate90();
			delete _wxImage;
			_wxImage = new wxImage(image);
		}
		break;
		case 7:// right side bottom
			break;
		case 8:// left side bottom
		{
			wxImage image = image.Rotate90();
			delete _wxImage;
			_wxImage = new wxImage(image);
		}
		break;
		}
	}
	break;

	case TYPE_IMAGE_REGARDSIMAGE:
	{

		printf("TYPE_IMAGE_REGARDSIMAGE Exif Method : %d \n", exifMethod);
		//if(exifMethod == 0)
		//	_image->RotateRawExif(exifOrientation);
		//else
		_image->RotateExif(exifMethod);
	}
	break;
	}
}

CImageLoadingFormat * CImageLoadingFormat::GetPage(const int &numPage)
{
	CLibPicture libPicture;
	CImageLoadingFormat * imageLoading = libPicture.LoadPicture(filename, false, numPage);
	return imageLoading;
}

int CImageLoadingFormat::GetNbPage()
{
	CLibPicture libPicture;
	int nbPage = libPicture.GetNbImage(filename);
	return nbPage;
}

int CImageLoadingFormat::Resize(const int &pictureWidth, const int &pictureHeight, const int &method)
{
	if(!IsOk())
		return -1;

	float newRatio = CalculPictureRatio(pictureWidth, pictureHeight);
	int thumbnailWidth = this->GetWidth() * newRatio;
	int thumbnailHeight =  this->GetHeight() * newRatio;

	if(thumbnailWidth > 0 && thumbnailHeight > 0)
	{	
		switch(format)
		{
		case TYPE_IMAGE_CXIMAGE:
			{
				if(method == 0)
				{
					//RGBQUAD color = {0,0,0,0};
					_image = new CRegardsBitmap(thumbnailWidth, thumbnailHeight);
					uint8_t * data = _image->GetPtBitmap();
					if(convertToBGR)
						_cxImage->InterpolationBicubicBGR(data, thumbnailWidth, thumbnailHeight);
					else
						_cxImage->InterpolationBicubicRGB(data, thumbnailWidth, thumbnailHeight);
					delete _cxImage;
					_cxImage = nullptr;
					format = TYPE_IMAGE_REGARDSIMAGE;
				}
				else if(method == 1)
				{
					_cxImage->Resample(thumbnailWidth, thumbnailHeight);
				}
			}
			break;

		case TYPE_IMAGE_WXIMAGE:
			{
				if(method == 0)
				{
					CInterpolationBicubic interpolation;
					_image = new CRegardsBitmap(thumbnailWidth, thumbnailHeight);
					interpolation.Execute(_wxImage, _image);
					delete _wxImage;
					_wxImage = nullptr;
					format = TYPE_IMAGE_REGARDSIMAGE;
				}
				else if(method == 1)
				{
					wxImage image = _wxImage->ResampleBicubic(thumbnailWidth, thumbnailHeight);
					delete _wxImage;
					_wxImage = new wxImage(image);
				}				
			}
			break;

		case TYPE_IMAGE_REGARDSIMAGE:
			{	
				CInterpolationBicubic interpolation;
				CRegardsBitmap * bitmapOut = new CRegardsBitmap(thumbnailWidth, thumbnailHeight);
				interpolation.Execute(_image, bitmapOut);
				delete _image;
				_image = bitmapOut;
			}
			break;
            
		case TYPE_IMAGE_REGARDSFLOATIMAGE:
			{	
				CInterpolationFloatBicubic interpolation;
				CRegardsFloatBitmap * bitmapOut = new CRegardsFloatBitmap(thumbnailWidth, thumbnailHeight);
				interpolation.Execute(_floatImage, bitmapOut);
				delete _floatImage;
				_floatImage = bitmapOut;
			}
			break;            
		}
	}
	return 0;
}

wxImage * CImageLoadingFormat::GetwxImage(const bool &copy)
{
	if(!IsOk())
		return nullptr;

	CLibPicture libPicture;
	switch(format)
	{
	case TYPE_IMAGE_CXIMAGE:
		{
			CRegardsBitmap * _local = libPicture.ConvertCXImageToRegardsBitmap(_cxImage, 0);
			wxImage * _wxImage = libPicture.ConvertRegardsBitmapToWXImage(_local);
			delete _local;
			return _wxImage;
		}

	case TYPE_IMAGE_WXIMAGE:
		{
			if(copy)
			{
				wxImage * local = new wxImage();
				*local = *_wxImage;
				return local;
			}
			else
				return  _wxImage;
		}
		break;

	case TYPE_IMAGE_REGARDSIMAGE:
		return libPicture.ConvertRegardsBitmapToWXImage(_image);
		break;

	case TYPE_IMAGE_REGARDSJPEGIMAGE:
		{
			if (_jpegImage != nullptr)
			{
				wxImage * _wxImage = new wxImage();
				wxMemoryInputStream jpegStream(_jpegImage->GetData(), _jpegImage->GetSize());
				_wxImage->LoadFile(jpegStream, wxBITMAP_TYPE_JPEG);
				return _wxImage;
			}
		}
		break;
	}
	return nullptr;
}

CxImage * CImageLoadingFormat::GetCxImage(const bool &copy)
{
	if(!IsOk())
		return nullptr;

	CLibPicture libPicture;
	switch(format)
	{
	case TYPE_IMAGE_CXIMAGE:
		{
			if(copy)
			{
				CxImage * local = new CxImage();
				*local = *_cxImage;
				return local;
			}
			else
				return _cxImage;
		}
		break;

	case TYPE_IMAGE_WXIMAGE:
		{
			wxMemoryOutputStream outputStream;
			_wxImage->SaveFile(outputStream, wxBITMAP_TYPE_PNG);
			uint8_t * data = new uint8_t[outputStream.GetSize()];
			outputStream.CopyTo(data, outputStream.GetSize());
			CxMemFile memFile(data, outputStream.GetSize());
			return new CxImage(&memFile, CxImage::GetTypeIdFromName("png"));
		}
		break;

	case TYPE_IMAGE_REGARDSIMAGE:
		return libPicture.ConvertRegardsBitmapToCXImage(_image);
		break;
	}
	return nullptr;
}