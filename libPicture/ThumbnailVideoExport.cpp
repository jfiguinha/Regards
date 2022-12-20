// ReSharper disable All
#include <header.h>
#include "ThumbnailVideoExport.h"
#include <ImageLoadingFormat.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "libPicture.h"
#include <wx/progdlg.h>
#include <effect_id.h>
#include <ConvertUtility.h>
#include <picture_utility.h>
#include "RGBAQuad.h"
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/timestamp.h>
}
using namespace cv;
using namespace Regards::Picture;

//#define EXPORT_DIAPORAMA_OPENCV

//**********************************************************************
//
//**********************************************************************
class CThumbnailDiaporama
{
public:
	virtual ~CThumbnailDiaporama() = default;
	cv::Mat GenerateBitmapForVideo(const wxString& filename, int width, int height);
	int ExecuteEffect(cv::Mat& pictureOne, cv::Mat& pictureTwo, const int& nbFrame, int width, int height,
	                  int effect);
	virtual void WritePicture(cv::Mat& dest) = 0;
	int SendMessageProgress();
	virtual int CopyPicture(cv::Mat& dest, const int& nbFrame, int width, int height) = 0;
	int ExecuteProcess(const wxString& outfile, vector<wxString>& listOfFile, int delay, int fps, int width, int height,
	                   int effect);



	wxProgressDialog* dialog;
	bool endProcess = false;
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

	void WritePicture(cv::Mat & dest);
    int CopyPicture(cv::Mat& dest, const int& nbFrame, int width, int height);
    VideoWriter outputVideo;
};


CThumbnailVideoExport::CThumbnailVideoExport()
{
}

CThumbnailVideoExport::~CThumbnailVideoExport()
{
}


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
int CThumbnailVideoOpenCVExportImpl::CopyPicture(cv::Mat & dest, const int& nbFrame, int width, int height)
{
    //cvtColor(dest, dest, cv::COLOR_BGRA2BGR);
	if (!dest.empty())
	{
		cv::Mat video;
		if (dest.channels() == 1) { cvtColor(dest, video, cv::COLOR_GRAY2RGB); }
		else if (dest.channels() == 4) { cvtColor(dest, video, cv::COLOR_BGRA2BGR); }
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


    //delete src_bitmap;
    dest.release();
    return 0;
}

void CThumbnailVideoOpenCVExportImpl::WritePicture(cv::Mat& dest)
{
	outputVideo.write(dest);
}


//**********************************************************************
//
//**********************************************************************
cv::Mat CThumbnailDiaporama::GenerateBitmapForVideo(const wxString& filename, int width, int height)
{
	cv::Mat src_bitmap;
	CLibPicture libPicture;
	bool pictureOK = true;

	CImageLoadingFormat * bitmap = libPicture.LoadPictureToBGRA(filename, pictureOK, width, height);
	if (bitmap != nullptr)
	{
		cv::Mat pBitmap = bitmap->GetOpenCVPicture();
		src_bitmap = cv::Mat::zeros(cv::Size(width, height), CV_8UC4);
		int x = (width - bitmap->GetWidth()) / 2;
		int y = (height - bitmap->GetHeight()) / 2;
		try
		{
			pBitmap.copyTo(src_bitmap(cv::Rect(x, y, pBitmap.cols, pBitmap.rows)));
		}
		catch (cv::Exception& e)
		{
			const char* err_msg = e.what();
			std::cout << "exception caught: " << err_msg << std::endl;
			std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
		}

		
		//cv::flip(src_bitmap, src_bitmap, 0);

		/*
		src_bitmap.create(width, height);
		src_bitmap->SetBackgroundColor(CRgbaquad(0, 0, 0, 0));
		int x = (width - pBitmap->GetWidth()) / 2;
		int y = (height - pBitmap->GetHeight()) / 2;
		src_bitmap->InsertBitmap(pBitmap, x, y, false);
		src_bitmap->VertFlipBuf();
		*/
	}

	delete bitmap;
	return src_bitmap;
}

//**********************************************************************
//
//**********************************************************************
int CThumbnailDiaporama::ExecuteEffect(cv::Mat& pictureOne, cv::Mat& pictureTwo, const int& nbFrame,
                                       int width, int height, int effect)
{
	CLibPicture libPicture;

	Mat src2;
	Mat src1;
	Mat dest;

	if (IDM_DIAPORAMA_MOVE != effect)
	{
		if (!pictureOne.empty())
		{
			cvtColor(pictureOne, src1, COLOR_BGRA2BGR);
		}

		if (!pictureTwo.empty())
		{
			cvtColor(pictureTwo, src2, COLOR_BGRA2BGR);
		}
	}




	switch (effect)
	{
	case IDM_DIAPORAMA_FUSION:
		{
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
		}
		break;

	case IDM_DIAPORAMA_TRANSITION:
		{
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
		}
		break;

	case IDM_DIAPORAMA_MOVE:
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

					pBitmap = cv::Mat::zeros(pictureOne.size(), CV_8UC4);
					imageLoad.SetPicture(pBitmap);
					
					imageLoad.InsertBitmap(&img1, x2, 0, false);
					imageLoad.InsertBitmap(&img2, x, 0, false);

					cvtColor(imageLoad.GetOpenCVPicture(), dest, COLOR_BGRA2BGR);

					WritePicture(dest);
					SendMessageProgress();
					if (endProcess)
						break;
				}
			}
			break;
		}
	default: ;
	}

	src2.release();
	src1.release();
	dest.release();

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

	if (effect != IDM_DIAPORAMA_TRANSITION && effect != IDM_DIAPORAMA_NONE)
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
		if ((i == 0 || effect == IDM_DIAPORAMA_NONE) && effect != IDM_DIAPORAMA_TRANSITION)
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

			switch (effect)
			{
			case IDM_DIAPORAMA_TRANSITION:
				{
					//int iStart = i * nbFrameByPicture;
					cv::Mat mat;
					ExecuteEffect(mat, src_bitmap, nbFrameByPicture, width, height, effect);
					if (endProcess)
						break;
				}
				break;

			default:
				{
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
				break;
			}
			
		}

		/*
		if (i > 2)
		{
			for (int i1 = i1 - 2; i1 < i1 - 1; i1++)
			{
				cv::Mat src_bitmap = listOfPicture[listOfFile[i1]];
				listOfPicture[listOfFile[i1]] = src_bitmap;
			}
		}
		*/
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
int CThumbnailVideoExport::GenerateVideoFromList(const wxString& outfile, vector<wxString>& listOfFile, int delay,
                                                 int fps, int width, int height, int effect)
{
    CThumbnailVideoOpenCVExportImpl thumbnailImpl;
    int movie_duration = 0;
	int fourcc = VideoWriter::fourcc('H', '2', '6', '4');
    Size S = Size((int)width,    // Acquire input size
        (int)height);



    // Open the output
    thumbnailImpl.outputVideo.open(CConvertUtility::ConvertToStdString(outfile), fourcc, fps, S, true);

	if (thumbnailImpl.outputVideo.isOpened())
	{
		movie_duration = thumbnailImpl.ExecuteProcess(outfile, listOfFile, delay, fps, width, height, effect);

		thumbnailImpl.outputVideo.release();

		if (thumbnailImpl.endProcess)
			movie_duration = 0;
	}
    return movie_duration;
}
