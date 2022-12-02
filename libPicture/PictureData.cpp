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

void CPictureData::CopyData(CImageLoadingFormat * bitmap)
{
	if (bitmap != nullptr)
		bitmapMatrix = bitmap->GetOpenCVPicture();
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
/*
void CPictureData::SetJpegData(uint8_t* & extdata, const int& outputsize)
{
	data = new uint8_t[outputsize];
	memcpy(data, extdata, outputsize);
	size = outputsize;
}

/*
CRegardsBitmap * CPictureData::LoadPictureToBGRA(const wxString &filename, bool &pictureOK, const int &resizeWidth, const int &resizeHeight)
{
	CRegardsBitmap * pictureData = nullptr;
	CLibPicture libPicture;
	
	CImageLoadingFormat * imageLoading = libPicture.LoadPicture(filename);

	if (imageLoading != nullptr)
	{
		if (resizeWidth != 0 && resizeHeight != 0)
		{
			float ratio = CalculPictureRatio(imageLoading->GetWidth(), imageLoading->GetHeight(), resizeWidth, resizeHeight);
			int width = imageLoading->GetWidth() * ratio;
			int height = imageLoading->GetHeight() * ratio;
			imageLoading->Resize(width, height, 1);
		}

		pictureOK = true;
		pictureData = imageLoading->GetRegardsBitmap(true);
	}
	else
		pictureOK = false;


	if (imageLoading != nullptr)
		delete imageLoading;

	imageLoading = nullptr;
	
	return pictureData;
}
*/
