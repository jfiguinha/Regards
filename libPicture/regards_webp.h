#pragma once
class CRegardsBitmap;

class CRegardsWebp
{
public:
	static cv::Mat GetPicture(const wxString& filename);
	static int GetNbFrame(const wxString& filename);
	static void GetPictureDimension(const wxString& filename, int& width, int& height);
	static void SavePicture(const wxString& filenameOut, cv::Mat& source, const int& compression = 50);
	static vector<cv::Mat> GetAllPicture(const wxString& filename, int& delay);

private:
	static void ApplyTransform(cv::Mat& image);
};
