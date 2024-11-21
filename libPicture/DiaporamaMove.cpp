// ReSharper disable All
#include <header.h>
#include "DiaporamaMove.h"
#include <libPicture.h>
#include <ImageLoadingFormat.h>
using namespace cv;
using namespace Regards::Picture;

//#define EXPORT_DIAPORAMA_OPENCV


//**********************************************************************
//
//**********************************************************************
int CDiaporamaMove::ExecuteEffect(cv::Mat& pictureOne, cv::Mat& pictureTwo, const int& nbFrame,
                                  int width, int height, int effect)
{
	if (!pictureOne.empty())
	{
		CImageLoadingFormat img1;
		CImageLoadingFormat img2;
		img1.SetPicture(pictureOne);
		img2.SetPicture(pictureTwo);
		for (int k = 0; k < nbFrame; k++)
		{
			float alpha = static_cast<float>(k) / static_cast<float>(nbFrame);
			//float beta = 1.0 - alpha;

			int x = 1920 - (alpha * 1920);
			int x2 = -(alpha * 1920);

			CImageLoadingFormat imageLoad;
			cv::Mat dest;
			pBitmap = cv::Mat::zeros(pictureOne.size(), CV_8UC4);
			imageLoad.SetPicture(pBitmap);

			imageLoad.InsertBitmap(&img1, x2, 0, false);
			imageLoad.InsertBitmap(&img2, x, 0, false);

			cvtColor(imageLoad.GetMatrix().getMat(), dest, COLOR_BGRA2BGR);

			WritePicture(dest);
			SendMessageProgress();
			if (endProcess)
				break;
		}
	}

	return 0;
}


//**********************************************************************
//
//**********************************************************************
int CDiaporamaMove::ExecuteProcess(const wxString& outfile, vector<wxString>& listOfFile, int delay, int fps,
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
