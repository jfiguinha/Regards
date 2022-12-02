#pragma once

class CImageLoadingFormat;

class CPictureData
{
public:
	CPictureData()
	{
		size = 0;
	}

	~CPictureData();

	//static CPictureData * LoadPictureToJpeg(const wxString &filename, bool &pictureOK, const int &resizeWidth = 0, const int &resizeHeight = 0);
	//static CRegardsBitmap * LoadPictureToBGRA(const wxString &filename, bool &pictureOK, const int &resizeWidth = 0, const int &resizeHeight = 0);
	static float CalculPictureRatio(const int& pictureWidth, const int& pictureHeight, const int& widthOutput,
	                                const int& heightOutput);

	int GetWidth();
	int GetHeight();
	wxString GetFilename();
	void SetFilename(const wxString& filename);
	void SetData(const int &width, const int &height, uint8_t* & extdata, const int& size);
	void CopyData(CImageLoadingFormat * bitmap);
	uint8_t* GetData();
	int GetSize();

	//void SetJpegData(uint8_t* & extdata, const int& outputsize);

private:
	wxString filename;
	cv::Mat bitmapMatrix;
	int size;
};
