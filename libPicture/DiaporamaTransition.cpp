
// ReSharper disable All
#include <header.h>
#include "DiaporamaTransition.h"
#include <libPicture.h>
#include <picture_utility.h>
#include <ImageLoadingFormat.h>
using namespace cv;
using namespace Regards::Picture;

//#define EXPORT_DIAPORAMA_OPENCV

//**********************************************************************
//
//**********************************************************************
int CDiaporamaTransition::ExecuteEffect(cv::Mat& pictureOne, cv::Mat& pictureTwo, const int& nbFrame,
	int width, int height, int effect)
{
	CLibPicture libPicture;

	Mat src2;
	Mat src1;
	Mat dest;


	if (!pictureOne.empty())
	{
		cvtColor(pictureOne, src1, COLOR_BGRA2BGR);
	}

	if (!pictureTwo.empty())
	{
		cvtColor(pictureTwo, src2, COLOR_BGRA2BGR);
	}


	float ratio = 1.0;
	for (int k = 0; k < nbFrame; k++)
	{
		ratio = ratio + 0.0005;
		resize(src2, dest, Size(), ratio, ratio);
		Size sDst = dest.size();
		Size sSrc = src2.size();
		int y = (sDst.height - sSrc.height) / 2;
		int x = (sDst.width - sSrc.width) / 2;
		Rect rect(x, y, 1920, 1080);
		//bool is_inside = (rect & cv::Rect(0, 0, dest.cols, dest.rows)) == rect;
		CPictureUtility::VerifRectSize(rect, dest);
		dest = dest(rect);
		WritePicture(dest);
		SendMessageProgress();
		if (endProcess)
			break;
	}


	return 0;
}


//**********************************************************************
//
//**********************************************************************
int CDiaporamaTransition::ExecuteProcess(const wxString& outfile, vector<wxString>& listOfFile, int delay, int fps,
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
	cv::Mat old_bitmap;
	cv::Mat src_bitmap;
	for (int i = 0; i < picturefile.size(); i++)
	{
		//int iStart = i * nbFrameByPicture;
		cv::Mat mat;
		src_bitmap = GenerateBitmapForVideo(listOfFile[i], width, height);
		ExecuteEffect(mat, src_bitmap, nbFrameByPicture, width, height, effect);
		if (endProcess)
			break;

	}

	if (endProcess)
		movie_duration = 0;


	return movie_duration;
}

