// ReSharper disable All
#include <header.h>
#include "ImageLoadingFormat.h"
#include <FreeImage.h>
#include <libPicture.h>
#include <ConvertUtility.h>
#include <ximage.h>
#include <wx/mstream.h>
//#include <wx/wfstream.h>

#ifdef ROTDETECT
#include <rotdetect.h>
#endif
#if defined(EXIV2)
#include <MetadataExiv2.h>
using namespace Regards::exiv2;
#endif
using namespace Regards::Picture;

extern float value[256];

CImageLoadingFormat::CImageLoadingFormat(): format(0), orientation(0)
{
}

CImageLoadingFormat::~CImageLoadingFormat()
{
}

void CImageLoadingFormat::ReadFile(const wxString& filename)
{
	_image = cv::imread(CConvertUtility::ConvertToStdString(filename));
	if (!_image.empty())
		cv::cvtColor(_image, _image, cv::COLOR_BGR2BGRA);
}

void CImageLoadingFormat::WriteFile(const wxString& filename)
{
	if (!_image.empty())
		cv::imwrite(CConvertUtility::ConvertToStdString(filename), _image);
}

int CImageLoadingFormat::GetFormat()
{
	return format;
}

int CImageLoadingFormat::GetWidth()
{
	if (!IsOk())
		return 0;

	return _image.cols;
}

int CImageLoadingFormat::GetHeight()
{
	if (!IsOk())
		return 0;

	return _image.rows;
}

void CImageLoadingFormat::SetRotation(const int& rotation)
{
	if (rotation == 90)
		Rotate90();
	else if (rotation == -90)
		Rotate270();
	else if (rotation == -180)
		Rotate180();
	else if (rotation == 180)
		Rotate180();
	else if (rotation == -270)
		Rotate90();
	else if (rotation == 270)
		Rotate270();
}

bool CImageLoadingFormat::Rotate270()
{
	if (_image.empty())
		return false;

	cv::rotate(_image, _image, cv::ROTATE_90_CLOCKWISE);
	return true;
}

bool CImageLoadingFormat::Rotate180()
{
	if (_image.empty())
		return false;

	cv::rotate(_image, _image, cv::ROTATE_180);
	return true;
}

void CImageLoadingFormat::Mirror()
{
	if (!IsOk())
		return;
	cv::flip(_image, _image, 1);
}

void CImageLoadingFormat::Rotate90()
{
	if (!IsOk())
		return;

	cv::rotate(_image, _image, cv::ROTATE_90_COUNTERCLOCKWISE);
}

void CImageLoadingFormat::Flip()
{
	if (!IsOk())
		return;
	cv::flip(_image, _image, 0);
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

cv::Mat CImageLoadingFormat::GetOpenCVPicture()
{
	return _image;
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
void CImageLoadingFormat::RotateExif(const int& orientation)
{
	if (!IsOk())
		return;

	switch (orientation)
	{
	case 1: // top left side
		break;
	case 2: // top right side
		cv::flip(_image, _image, 1);
		break;
	case 3: // bottom right side
		cv::flip(_image, _image, 1);
		cv::flip(_image, _image, 0);
		break;
	case 4: // bottom left side
		cv::flip(_image, _image, 0);
		break;
	case 5: //left side top
		this->Rotate90();
		cv::flip(_image, _image, 0);
		break;
	case 6: // right side top
		this->Rotate90();
		cv::flip(_image, _image, 1);
		cv::flip(_image, _image, 0);
		break;
	case 7: // right side bottom
		this->Rotate90();
		cv::flip(_image, _image, 1);
		break;
	case 8: // left side bottom
		this->Rotate90();
		break;
	default:;
	}
}

wxImage CImageLoadingFormat::GetwxImage()
{
	if (!IsOk())
		return wxImage();

	cv::Mat im2;
	if (_image.channels() == 1) { cvtColor(_image, im2, cv::COLOR_GRAY2RGB); }
	else if (_image.channels() == 4) { cvtColor(_image, im2, cv::COLOR_BGRA2RGB); }
	else { cvtColor(_image, im2, cv::COLOR_BGR2RGB); }

	long imsize = im2.rows * im2.cols * im2.channels();
	wxImage wx(im2.cols, im2.rows, (unsigned char*)malloc(imsize), false);
	unsigned char* s = im2.data;
	unsigned char* d = wx.GetData();
	memcpy(d, s, imsize);
	/*
	for (long i = 0; i < imsize; i++)
	{
		d[i] = s[i];
	}
	*/
	return wx;

}

CxImage CImageLoadingFormat::GetCxImage()
{
	if (!IsOk())
		return CxImage();

	CxImage image;
	image.CreateFromArray(_image.data, _image.rows * _image.cols * 4, _image.size().width, _image.size().height, 32, _image.size().width * 4, false, false);
	image.Flip();
	return image;

}


void CImageLoadingFormat::SetOrientation(const int& orientation)
{
	RotateExif(orientation);
	this->orientation = 0;
}


void CImageLoadingFormat::SetFilename(const wxString& filename)
{
	this->filename = wxString(filename);
}

void CImageLoadingFormat::UpdatePicture(cv::Mat& image)
{
	image.copyTo(_image);
}

void CImageLoadingFormat::SetPicture(CxImage* image)
{
	_image.create(image->GetHeight(), image->GetWidth(), CV_8UC4);
	image->Encode2BGRA(_image.data, image->GetHeight() * image->GetWidth() * 4, false);
	cv::flip(_image, _image, 0);
}

FIBITMAP * CImageLoadingFormat::GetFreeImage()
{
	int pitch = GetWidth() * 4;
	return FreeImage_ConvertFromRawBits(_image.data, GetWidth(),
		GetHeight(), pitch, 32, FI_RGBA_RED_MASK,
		FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);
}

void CImageLoadingFormat::SetPicture(wxImage & image)
{
	_image = CLibPicture::mat_from_wx(image);
	if (_image.channels() == 1)
		cvtColor(_image, _image, cv::COLOR_GRAY2BGRA);
	
}

cv::Mat CImageLoadingFormat::GetFloatImage()
{
	cv::Mat img2;
	_image.convertTo(img2, CV_32FC4, 1.0 / 255);
	return img2;
}

void CImageLoadingFormat::SetPicture(cv::Mat& image)
{
	if (image.channels() == 3)
		cvtColor(image, _image, cv::COLOR_BGR2BGRA);
	else if (image.channels() == 1)
		cvtColor(image, _image, cv::COLOR_GRAY2BGRA);
	else
		image.copyTo(_image);

}

void CImageLoadingFormat::SetPicture(cv::Mat& image, const int & exif, const wxString & fileIn, const bool& convertToRGB24)
{
	SetPicture(image);

	if (convertToRGB24)
		cvtColor(_image, _image, cv::COLOR_RGBA2BGRA);

	RotateExif(exif);
	orientation = 0;// image->GetOrientation();
	if (orientation < 0)
		orientation = 0;
	filename = fileIn;
}

/*
void CImageLoadingFormat::SaveToJpeg(const wxString& filename)
{
	int compressMethod = 0;
	unsigned long outputsize = 0;
	std::vector<uchar> data = GetJpegData();
	wxFileOutputStream outputStream(filename);
	outputStream.Write(data.data(), data.size());
	outputStream.Close();
}
*/
void CImageLoadingFormat::ConvertToBGR()
{
	cvtColor(_image, _image, cv::COLOR_RGBA2BGRA);
}


std::vector<uchar> CImageLoadingFormat::GetJpegData()
{
	std::vector<uchar> buff;

	if (!IsOk())
	{
		return buff;
	}

	
	std::vector<int> param(2);
	param[0] = cv::IMWRITE_JPEG_QUALITY;
	param[1] = 80; //default(95) 0-100
	cv::imencode(".jpg", _image, buff, param);
	return buff;
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
	return !_image.empty();

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
		cv::resize(_image, _image,cv::Size(thumbnailWidth, thumbnailHeight), cv::INTER_CUBIC);
	}
	return 0;
}
