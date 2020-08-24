#include "header.h"
#include "PictureData.h"
#include <ImageLoadingFormat.h>
#include "libPicture.h"
#include <RegardsBitmap.h>

CPictureData::~CPictureData()
{
	if (data != nullptr)
		delete[] data;

	data = nullptr;
}

int  CPictureData::GetWidth()
{
	return width;
}

void CPictureData::SetWidth(const int &width)
{
	this->width = width;
}

int  CPictureData::GetHeight()
{
	return height;
}
void  CPictureData::SetHeight(const int &height)
{
	this->height = height;
}

wxString  CPictureData::GetFilename()
{
	return filename;
}

void  CPictureData::SetFilename(const wxString &filename)
{
	this->filename = filename;
}

uint8_t * CPictureData::GetData()
{
	return data;
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
void CPictureData::SetData(uint8_t * & extdata, const int &size)
{
	if (extdata != nullptr && size > 0)
	{
		if (data != nullptr)
			delete[] data;
		data = new uint8_t[size];
		memcpy(data, extdata, size * sizeof(uint8_t));
		this->size = size;
	}
}

void CPictureData::CopyData(CRegardsBitmap * & bitmap)
{
	if(bitmap != nullptr)
		bitmap->SetBitmap(data, width, height, false, true);
}
		

float CPictureData::CalculPictureRatio(const int &pictureWidth, const int &pictureHeight, const int &widthOutput, const int &heightOutput)
{
	if (pictureWidth == 0 && pictureHeight == 0)
		return 1.0f;

	float newRatio = 1;

	if (pictureWidth > pictureHeight)
		newRatio = (float)widthOutput / (float)(pictureWidth);
	else
		newRatio = (float)heightOutput / (float)(pictureHeight);

	if ((pictureHeight * newRatio) > heightOutput)
	{
		newRatio = (float)heightOutput / (float)(pictureHeight);
	}
	else
	{
		if ((pictureWidth * newRatio) > widthOutput)
		{
			newRatio = (float)widthOutput / (float)(pictureWidth);
		}
	}

	return newRatio;
}

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

CPictureData * CPictureData::LoadPictureToJpeg(const wxString &filename, bool &pictureOK, const int &resizeWidth, const int &resizeHeight)
{
	CPictureData * pictureData = nullptr;
	CLibPicture libPicture;
	CImageLoadingFormat * imageLoading = libPicture.LoadPicture(filename);

	if (imageLoading != nullptr)
	{
		pictureOK = imageLoading->IsOk();
		if (pictureOK)
		{
			imageLoading->ApplyExifOrientation(1);
			imageLoading->ConvertToRGB24(true);
			//Calcul Resize Size
			pictureData = new CPictureData();
			if (resizeWidth != 0 && resizeHeight != 0)
			{
				float ratio = CalculPictureRatio(imageLoading->GetWidth(), imageLoading->GetHeight(), resizeWidth, resizeHeight);
				pictureData->width = imageLoading->GetWidth() * ratio;
				pictureData->height = imageLoading->GetHeight() * ratio;
				imageLoading->Resize(pictureData->width, pictureData->height, 1);
			}
			else
			{
				pictureData->width = imageLoading->GetWidth();
				pictureData->height = imageLoading->GetHeight();
			}

			unsigned long outputsize = 0;
			int compressMethod = 0;
			uint8_t * data = imageLoading->GetJpegData(outputsize, compressMethod);
			pictureData->data = new uint8_t[outputsize];
			memcpy(pictureData->data, data, outputsize);
			imageLoading->DestroyJpegData(data, compressMethod);

			pictureData->size = outputsize;

		}
	}

	if (imageLoading != nullptr)
		delete imageLoading;

	imageLoading = nullptr;

	return pictureData;
}
