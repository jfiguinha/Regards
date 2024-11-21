#include "header.h"
#include "PictureData.h"
#include "ImageLoadingFormat.h"


CPictureData::~CPictureData()
{
	bitmapMatrix.release();
}

int CPictureData::GetWidth()
{
	return bitmapMatrix.cols;
}

int CPictureData::GetHeight()
{
	return bitmapMatrix.rows;
}


wxString CPictureData::GetFilename()
{
	return filename;
}

void CPictureData::SetFilename(const wxString& filename)
{
	this->filename = filename;
}

uint8_t* CPictureData::GetData()
{
	return bitmapMatrix.data;
}

int CPictureData::GetSize()
{
	return size;
}

/*
std::vector<char> CPictureData::CopyData()
{
	std::vector<char> _data(data, data + size);
	return _data;
}
*/
void CPictureData::SetData(const int& width, const int& height, uint8_t* & extdata, const int& size)
{
	if (extdata != nullptr && size > 0)
	{
		bitmapMatrix = cv::Mat(height, width, CV_8UC4, extdata);
		this->size = size;
	}
}

void CPictureData::CopyData(CImageLoadingFormat* bitmap)
{
	if (bitmap != nullptr)
		bitmapMatrix = bitmap->GetMatrix().getMat();
}


float CPictureData::CalculPictureRatio(const int& pictureWidth, const int& pictureHeight, const int& widthOutput,
                                       const int& heightOutput)
{
	if (pictureWidth == 0 && pictureHeight == 0)
		return 1.0f;

	float newRatio;

	if (pictureWidth > pictureHeight)
		newRatio = static_cast<float>(widthOutput) / static_cast<float>(pictureWidth);
	else
		newRatio = static_cast<float>(heightOutput) / static_cast<float>(pictureHeight);

	if ((pictureHeight * newRatio) > heightOutput)
	{
		newRatio = static_cast<float>(heightOutput) / static_cast<float>(pictureHeight);
	}
	else
	{
		if ((pictureWidth * newRatio) > widthOutput)
		{
			newRatio = static_cast<float>(widthOutput) / static_cast<float>(pictureWidth);
		}
	}

	return newRatio;
}
