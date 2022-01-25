#pragma once
#define TYPE_IMAGE_CXIMAGE 0
#define TYPE_IMAGE_WXIMAGE 1
#define TYPE_IMAGE_REGARDSIMAGE 2
#define TYPE_IMAGE_REGARDSFLOATIMAGE 3
#define TYPE_IMAGE_REGARDSJPEGIMAGE 4
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
	int GetNbPage();
	CxImage* GetCxImage();
	uint8_t* GetJpegData(unsigned long& outputsize);
	wxImage* GetwxImage();
	CRegardsFloatBitmap* GetFloatBitmap();
	CRegardsBitmap* GetRegardsBitmap(const bool& copy = true);
	int Resize(const int& width, const int& height, const int& method);
	void Flip();
	void Mirror();
	void Rotate90();
	int GetWidth();
	int GetHeight();
	int GetOrientation();
	wxString GetFilename();

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
	void ApplyExifOrientation(const int& exifMethod);
	void ApplyExifOrientation();
protected:
	void DeletePicture();
	float CalculPictureRatio(const int& pictureWidth, const int& pictureHeight);
	uint8_t* CompressRegardsBitmapToJpeg(unsigned char* buffer, int width, int height, unsigned long& outputsize);
	CRegardsBitmap* _image;
	wxString filename;
	int orientation;
	bool convertToRGB24;
	bool memoryDelete;
	bool convertToBGR;
	int resolution = 300;
};
