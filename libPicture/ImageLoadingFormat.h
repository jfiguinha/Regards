#pragma once
class CxImage;
class FIBITMAP;
class CRgbaquad;

class CImageLoadingFormat
{
public:
	CImageLoadingFormat();
	~CImageLoadingFormat();
	//void SaveToJpeg(const wxString& filename);
	cv::Mat GetOpenCVPicture();
	void RotateExif_withoutflip(const int& orientation);
	CImageLoadingFormat* GetPage(const int& numPage);
	int GetNbPage();
	int Resize(const int& width, const int& height, const int& method);
	void Flip();
	void Mirror();
	void Rotate90();
	int GetWidth();
	int GetHeight();
	int GetOrientation();
	wxString GetFilename();
	int GetFormat();
	int InsertBitmap(CImageLoadingFormat* bitmap, const int& xPos, const int& yPos, const bool& withalpha = true);
	void SetRotation(const int& rotation);
	bool Rotate180();
	bool Rotate270();
	int GetResolution();
	void SetOrientation(const int& orientation);
	void SetFilename(const wxString& filename);
	void SetPicture(CxImage* image);
	void SetPicture(wxImage& image);
	void SetPicture(cv::Mat& image);
	void SetPicture(cv::Mat& image, const int& orientation, const wxString& filename,
	                const bool& convertToRGB24 = false);
	void UpdatePicture(cv::Mat& image);
	bool IsOk();
	void ConvertToBGR();
	void RotateExif(const int& orientation);
	std::vector<uchar> GetJpegData();
	wxImage GetwxImage();
	CxImage GetCxImage();
	cv::Mat GetFloatImage();
	FIBITMAP* GetFreeImage();
	void ReadFile(const wxString& filename);
	void WriteFile(const wxString& filename);

protected:
	CRgbaquad* GetPtColorValue(const int& x, const int& y);
	CRgbaquad GetColorValue(const int& x, const int& y);
	float CalculPictureRatio(const int& pictureWidth, const int& pictureHeight);
	int GetPosition(const int& x, const int& y);
	cv::Mat _image;
	int format;
	wxString filename;
	int orientation;
	int resolution = 300;
};
