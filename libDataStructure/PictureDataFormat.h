#pragma once

class CxImage;
class CRegardsBitmap;

class CPictureDataFormat
{
public:
	CPictureDataFormat()
	{
		bitmap = nullptr;
		image = nullptr;
	};
	~CPictureDataFormat()
	{
		if(bitmap != nullptr)
			delete bitmap;

		if(image != nullptr)
			delete image;
	};

	int typePicture;
	CxImage * image;
	wxImage imageWx;
	CRegardsBitmap * bitmap;
};