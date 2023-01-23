#include <header.h>
#include "ThumbnailDiaporama.h"
#include <libPicture.h>
#include <ImageLoadingFormat.h>
using namespace cv;
using namespace Regards::Picture;

//**********************************************************************
//
//**********************************************************************
int CThumbnailDiaporama::SendMessageProgress()
{
	const wxString message = "In progress : " + to_string(numWriteFrame) + "/" + to_string(countNbFrame);
	if (false == dialog->Update(numWriteFrame, message))
	{
		endProcess = true;
		return 1;
	}
	numWriteFrame++;
	return 0;
}


//**********************************************************************
//
//**********************************************************************
int CThumbnailDiaporama::ExecuteEffect(Mat& pictureOne, Mat& pictureTwo, const int& nbFrame,
                                       int width, int height, int effect)
{
	return 0;
}


//**********************************************************************
//
//**********************************************************************
int CThumbnailDiaporama::ExecuteProcess(const wxString& outfile, vector<wxString>& listOfFile, int delay, int fps,
                                        int width, int height, int effect)
{
	CLibPicture libPicture;
	vector<wxString> picturefile;

	//pBitmap = new CRegardsBitmap(width, height);

	for (auto& i : listOfFile)
	{
		if (libPicture.TestIsPicture(i))
		{
			picturefile.push_back(i);
		}
	}

	const int nbFrameByPicture = delay * fps;
	const int nbFrameEffect = fps * 2;

	countNbFrame = delay * fps * picturefile.size();

	int movie_duration = countNbFrame / fps;

	wxProgressDialog dialog("Export File", "Checking...", countNbFrame, nullptr,
	                        wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);
	this->dialog = &dialog;
	Mat old_bitmap;
	Mat src_bitmap;
	for (int i = 0; i < picturefile.size(); i++)
	{
		src_bitmap = GenerateBitmapForVideo(listOfFile[i], width, height);
		src_bitmap.copyTo(old_bitmap);
		CopyPicture(src_bitmap, nbFrameByPicture, width, height);
		if (endProcess)
			break;
	}

	if (endProcess)
		movie_duration = 0;


	return movie_duration;
}


//**********************************************************************
//
//**********************************************************************
int CThumbnailVideoOpenCVExportImpl::CopyPicture(Mat& dest, const int& nbFrame, int width, int height)
{
	//cvtColor(dest, dest, cv::COLOR_BGRA2BGR);
	if (!dest.empty())
	{
		Mat video;
		if (dest.channels() == 1) { cvtColor(dest, video, COLOR_GRAY2RGB); }
		else if (dest.channels() == 4) { cvtColor(dest, video, COLOR_BGRA2BGR); }
		else
			dest.copyTo(video);

		for (int i = 0; i < nbFrame; i++)
		{
			outputVideo.write(video);

			SendMessageProgress();
			if (endProcess)
				break;
		}
	}
	else
	{
		printf("Error");
	}
	return 0;
}

void CThumbnailVideoOpenCVExportImpl::WritePicture(Mat& dest)
{
	outputVideo.write(dest);
}


//**********************************************************************
//
//**********************************************************************
Mat CThumbnailDiaporama::GenerateBitmapForVideo(const wxString& filename, int width, int height)
{
	Mat src_bitmap;
	CLibPicture libPicture;
	bool pictureOK = true;

	CImageLoadingFormat* bitmap = libPicture.LoadPictureToBGRA(filename, pictureOK, width, height);
	if (bitmap != nullptr)
	{
		Mat pBitmap = bitmap->GetOpenCVPicture();
		src_bitmap = Mat::zeros(Size(width, height), CV_8UC4);
		int x = (width - bitmap->GetWidth()) / 2;
		int y = (height - bitmap->GetHeight()) / 2;
		try
		{
			pBitmap.copyTo(src_bitmap(Rect(x, y, pBitmap.cols, pBitmap.rows)));
		}
		catch (Exception& e)
		{
			const char* err_msg = e.what();
			std::cout << "exception caught: " << err_msg << std::endl;
			std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
		}
	}

	delete bitmap;
	return src_bitmap;
}
