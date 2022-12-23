#include <header.h>
#include "picture_utility.h"
#include "ConvertUtility.h"
#include <wx/wfstream.h>
#include "FileUtility.h"
uint8_t* CPictureUtility::readfile(const wxString& fileName, size_t& _fileSize)
{
	//const char * fichier = CConvertUtility::ConvertFromwxString(fileName);
	uint8_t* _compressedImage = nullptr;
	if (wxFile::Exists(fileName))
	{
		wxFile file(fileName);
		_fileSize = file.Length();
		_compressedImage = new uint8_t[_fileSize];
		if (file.IsOpened())
			file.Read(_compressedImage, _fileSize);
		file.Close();
	}
	return _compressedImage;
}



float CPictureUtility::CalculPictureRatio(const int& pictureWidth, const int& pictureHeight, const int& screenWidth, const int& screenHeight)
{

	if (pictureWidth == 0 && pictureHeight == 0)
		return 1.0f;

	float new_ratio;

	//int tailleAffichageWidth = 0, tailleAffichageHeight = 0;

	if (pictureWidth > pictureHeight)
		new_ratio = static_cast<float>(screenWidth) / static_cast<float>(pictureWidth);
	else
		new_ratio = static_cast<float>(screenHeight) / static_cast<float>(pictureHeight);

	if ((pictureHeight * new_ratio) > screenHeight)
	{
		new_ratio = static_cast<float>(screenHeight) / static_cast<float>(pictureHeight);
	}
	if ((pictureWidth * new_ratio) > screenWidth)
	{
		new_ratio = static_cast<float>(screenWidth) / static_cast<float>(pictureWidth);
	}

	return new_ratio;
}


void CPictureUtility::RotateExif(cv::Mat& _image, const int& orientation)
{
	if (_image.empty())
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
		Rotate90(_image);
		cv::flip(_image, _image, 0);
		break;
	case 6: // right side top
		Rotate90(_image);
		cv::flip(_image, _image, 1);
		cv::flip(_image, _image, 0);
		break;
	case 7: // right side bottom
		Rotate90(_image);
		cv::flip(_image, _image, 1);
		break;
	case 8: // left side bottom
		Rotate90(_image);
		break;
	default:;
	}
}


void CPictureUtility::ApplyRotation(cv::Mat& image, const int& rotation)
{
	if (image.empty())
		return;

	if (rotation == 90)
		Rotate90(image);
	else if (rotation == -90)
		Rotate270(image);
	else if (rotation == -180)
		Rotate180(image);
	else if (rotation == 180)
		Rotate180(image);
	else if (rotation == -270)
		Rotate90(image);
	else if (rotation == 270)
		Rotate270(image);
}


//////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CPictureUtility::Rotate90(cv::Mat& image)
{
	if (image.empty())
		return false;

	cv::rotate(image, image, cv::ROTATE_90_COUNTERCLOCKWISE);
	return true;
}

bool CPictureUtility::Rotate270(cv::Mat& image)
{
	if (image.empty())
		return false;

	cv::rotate(image, image, cv::ROTATE_90_CLOCKWISE);
	return true;
}

bool CPictureUtility::Rotate180(cv::Mat& image)
{
	if (image.empty())
		return false;

	cv::rotate(image, image, cv::ROTATE_180);
	return true;
}

void CPictureUtility::ApplyTransform(cv::Mat& image)
{
	if (image.channels() == 3)
		cvtColor(image, image, cv::COLOR_RGB2BGRA);
	else if (image.channels() == 1)
		cvtColor(image, image, cv::COLOR_GRAY2BGRA);
	else
		cvtColor(image, image, cv::COLOR_RGBA2BGRA);
}

void CPictureUtility::writefile(const wxString& fileName, uint8_t* data, const size_t & size)
{
	uint8_t* _compressedImage = nullptr;
	wxFile file;
	if(file.Create(fileName, true, wxFile::write))
		file.Write(data, size);
	file.Close();
}

void CPictureUtility::VerifRectSize(cv::Rect& rect, cv::Mat& matSrc)
{
	rect.x = max(rect.x, 0);
	rect.y = max(rect.y, 0);

	bool is_inside = (rect & cv::Rect(0, 0, matSrc.cols, matSrc.rows)) == rect;
	if (!is_inside)
	{
		if ((rect.width + rect.x) > matSrc.cols)
		{
			rect.width = matSrc.cols - rect.x - 1;
		}

		if ((rect.height + rect.y) > matSrc.rows)
		{
			rect.height = matSrc.rows - rect.y - 1;
		}
	}
}


wxString CPictureUtility::GetPhotoCancel()
{
#ifdef WIN32
	return CFileUtility::GetResourcesFolderPath() + "\\photo_cancel.png";
#else
	return CFileUtility::GetResourcesFolderPath() + "/photo_cancel.png";
#endif
}