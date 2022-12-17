#pragma once
class CxImage;
class FIBITMAP;

class CImageLoadingFormat
{
public:
	CImageLoadingFormat();
	~CImageLoadingFormat();
	//void SaveToJpeg(const wxString& filename);
	cv::Mat GetOpenCVPicture();
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
	void SetRotation(const int& rotation);
	bool Rotate180();
	bool Rotate270();
	int GetResolution();
	void SetOrientation(const int& orientation);
	void SetFilename(const wxString& filename);
	void SetPicture(CxImage* image);
	void SetPicture(wxImage& image);
	void SetPicture(cv::Mat& image);
	void SetPicture(cv::Mat & image, const int& orientation, const wxString& filename, const bool& convertToRGB24 = false);
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

	float CalculPictureRatio(const int& pictureWidth, const int& pictureHeight);

	cv::Mat _image;
	int format;
	wxString filename;
	int orientation;
	int resolution = 300;
};
