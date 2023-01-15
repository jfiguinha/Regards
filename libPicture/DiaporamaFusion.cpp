// ReSharper disable All
#include <header.h>
#include "DiaporamaFusion.h"
#include <libPicture.h>
using namespace cv;
using namespace Regards::Picture;

//#define EXPORT_DIAPORAMA_OPENCV


//**********************************************************************
//
//**********************************************************************
int CDiaporamaFusion::ExecuteEffect(cv::Mat& pictureOne, cv::Mat& pictureTwo, const int& nbFrame,
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

	if (!pictureOne.empty())
	{
		for (int k = 0; k < nbFrame; k++)
		{
			float alpha = static_cast<float>(k) / static_cast<float>(nbFrame);
			float beta = 1.0 - alpha;
			addWeighted(src2, alpha, src1, beta, 0.0, dest);
			WritePicture(dest);
			SendMessageProgress();
			if (endProcess)
				break;
		}
	}


	src2.release();
	src1.release();
	dest.release();

	return 0;
}


//**********************************************************************
//
//**********************************************************************
int CDiaporamaFusion::ExecuteProcess(const wxString& outfile, vector<wxString>& listOfFile, int delay, int fps,
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
	countNbFrame += nbFrameEffect * (picturefile.size() - 1);

	int movie_duration = countNbFrame / fps;

	wxProgressDialog dialog("Export File", "Checking...", countNbFrame, nullptr,
	                        wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);

	this->dialog = &dialog;
	cv::Mat old_bitmap;
	cv::Mat src_bitmap;
	for (int i = 0; i < picturefile.size(); i++)
	{
		//int position;
		if (i == 0)
		{
			src_bitmap = GenerateBitmapForVideo(listOfFile[i], width, height);
			src_bitmap.copyTo(old_bitmap);
			CopyPicture(src_bitmap, nbFrameByPicture, width, height);
			if (endProcess)
				break;
		}
		else
		{
			src_bitmap = GenerateBitmapForVideo(listOfFile[i], width, height);

			int iStart = i * nbFrameByPicture + nbFrameEffect * (i - 1);
			ExecuteEffect(old_bitmap, src_bitmap, nbFrameEffect, width, height, effect);
			iStart += nbFrameEffect;
			if (endProcess)
				break;

			src_bitmap.copyTo(old_bitmap);
			CopyPicture(src_bitmap, nbFrameByPicture, width, height);
			if (endProcess)
				break;
		}

		if (endProcess)
			break;
	}

	if (endProcess)
		movie_duration = 0;


	return movie_duration;
}
