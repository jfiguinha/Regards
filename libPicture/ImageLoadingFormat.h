#pragma once
class CRegardsFloatBitmap;
class CRegardsBitmap;
class CRegardsJpegPicture;
class CxImage;


class CImageLoadingFormat
{
public:
	CImageLoadingFormat(const bool& memoryDelete = true);
	~CImageLoadingFormat();
	void SetMemoryDelete(const bool& memoryDelete);
	void DestroyJpegData(uint8_t* & data);
	void SaveToJpeg(const wxString& filename);

	CImageLoadingFormat* GetPage(const int& numPage);
	//void SetPicturToJpeg(CRegardsBitmap* image, const bool& flip = true);
	CxImage* GetCxImage(const bool& copy = true);
	wxImage GetwxImage(const bool& copy = true);
	CRegardsBitmap* GetRegardsBitmap(const bool& copy = true);
	CRegardsFloatBitmap* GetFloatBitmap(const bool& copy = true);

	int GetNbPage();

	uint8_t* GetJpegData(unsigned long& outputsize);

	int Resize(const int& width, const int& height, const int& method);
	void Flip();
	void Mirror();
	void Rotate90();
	int GetWidth();
	int GetHeight();
	int GetOrientation();
	wxString GetFilename();
	int GetFormat();
	int GetResolution();
	void SetOrientation(const int& orientation);
	void SetFilename(const wxString& filename);

	void SetPicture(CRegardsBitmap* image, const bool& convertToRGB24 = false);
	void SetPicture(CxImage* image);
	void SetPicture(wxImage* image);
	void UpdatePicture(CRegardsBitmap* image);
	void SetPicture(CRegardsFloatBitmap* image);
	bool IsOk();
	void ConvertToRGB24(const bool& convert);
	void ConvertToBGR(const bool& convert);

protected:
	void DeletePicture();
	float CalculPictureRatio(const int& pictureWidth, const int& pictureHeight);
	uint8_t* CompressRegardsBitmapToJpeg(unsigned char* buffer, int width, int height, unsigned long& outputsize);
	CRegardsBitmap* _image;
	int format;
	wxString filename;
	int orientation;
	bool convertToRGB24;
	bool memoryDelete;
	bool convertToBGR;
	int resolution = 300;
};
