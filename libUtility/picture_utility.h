#pragma once

class CPictureUtility
{
public:
	static uint8_t* readfile(const wxString& fileName, size_t& _fileSize);
	static void writefile(const wxString& fileName, uint8_t* data, const size_t& size);
	static wxString GetPhotoCancel();
	static void VerifRectSize(cv::Rect& rect, cv::Mat& matSrc);
	static void ApplyTransform(cv::Mat& image);
	static void ApplyRotation(cv::Mat& image, const int& rotation);
	static cv::Mat ApplyRotationVideo(cv::Mat& image, const int& rotation);
	static void RotateExif(cv::Mat& image, const int& orientation);
	static bool Rotate90(cv::Mat& image);
	static bool Rotate180(cv::Mat& image);
	static bool Rotate270(cv::Mat& image);
	static float CalculPictureRatio(const int& pictureWidth, const int& pictureHeight, const int& screenWidth,
	                                const int& screenHeight);

	

};
