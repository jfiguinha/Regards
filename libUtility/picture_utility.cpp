#include <header.h>
#include "picture_utility.h"
#include <wx/wfstream.h>

std::vector<uint8_t> CPictureUtility::ReadFile(const wxString& fileName)
{
	std::vector<uint8_t> buffer;

	if (!wxFile::Exists(fileName))
		return buffer;

	wxFile file(fileName);

	if (!file.IsOpened())
		return buffer;

	const wxFileOffset length = file.Length();

	if (length <= 0)
		return buffer;

	buffer.resize(static_cast<size_t>(length));

	if (file.Read(buffer.data(), buffer.size()) != buffer.size())
	{
		buffer.clear();
	}

	return buffer;
}




float CPictureUtility::CalculPictureRatio(const int& pictureWidth, const int& pictureHeight, const int& screenWidth,
                                          const int& screenHeight)
{
	if (pictureWidth <= 0 || pictureHeight <= 0)
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
		flip(_image, _image, 1);
		break;
	case 3: // bottom right side
		flip(_image, _image, 1);
		flip(_image, _image, 0);
		break;
	case 4: // bottom left side
		flip(_image, _image, 0);
		break;
	case 5: //left side top
		Rotate90(_image);
		flip(_image, _image, 0);
		break;
	case 6: // right side top
		Rotate90(_image);
		flip(_image, _image, 1);
		flip(_image, _image, 0);
		break;
	case 7: // right side bottom
		Rotate90(_image);
		flip(_image, _image, 1);
		break;
	case 8: // left side bottom
		Rotate90(_image);
		break;
	default: ;
	}
}

cv::Mat  CPictureUtility::ApplyRotationVideo(cv::Mat& image, const int& rotation)
{
	cv::Mat mat;
	image.copyTo(mat);

	if (image.empty())
		return mat;

	if (rotation == 90)
		Rotate270(mat);
	else if (rotation == -90)
		Rotate90(mat);
	else if (rotation == -180)
		Rotate180(mat);
	else if (rotation == 180)
		Rotate180(mat);
	else if (rotation == -270)
		Rotate270(mat);
	else if (rotation == 270)
		Rotate90(mat);

	return mat;
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

void CPictureUtility::writefile(const wxString& fileName, uint8_t* data, const size_t& size)
{
	wxFile file;

	if (file.Create(fileName, true, wxFile::write))
		file.Write(data, size);
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

