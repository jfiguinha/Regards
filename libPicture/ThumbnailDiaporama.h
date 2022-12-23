#pragma once
#include <wx/progdlg.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
//**********************************************************************
//
//**********************************************************************
class CThumbnailDiaporama
{
public:
	virtual ~CThumbnailDiaporama() = default;
	cv::Mat GenerateBitmapForVideo(const wxString& filename, int width, int height);
	virtual int ExecuteEffect(cv::Mat& pictureOne, cv::Mat& pictureTwo, const int& nbFrame, int width, int height,
		int effect);
	virtual void WritePicture(cv::Mat& dest) = 0;
	int SendMessageProgress();
	virtual int CopyPicture(cv::Mat& dest, const int& nbFrame, int width, int height) = 0;
	virtual int ExecuteProcess(const wxString& outfile, vector<wxString>& listOfFile, int delay, int fps, int width, int height,
		int effect);

	bool endProcess = false;

protected:

	wxProgressDialog* dialog;
	
	int countNbFrame = 0;
	int numWriteFrame = 0;
	cv::Mat pBitmap;

};

//**********************************************************************
//
//**********************************************************************
class CThumbnailVideoOpenCVExportImpl : public CThumbnailDiaporama
{
public:

	void WritePicture(cv::Mat& dest);
	int CopyPicture(cv::Mat& dest, const int& nbFrame, int width, int height);
	cv::VideoWriter outputVideo;
};
