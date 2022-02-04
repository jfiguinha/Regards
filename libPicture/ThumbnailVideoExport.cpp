// ReSharper disable All
#include <header.h>
#include "ThumbnailVideoExport.h"
#include "RegardsBitmap.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "libPicture.h"
#include <wx/progdlg.h>
#include <effect_id.h>

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
	CRegardsBitmap* GenerateBitmapForVideo(const wxString& filename, int width, int height);
	int ExecuteEffect(const wxString& filename1, const wxString& filename2, const int& nbFrame, int width, int height,
	                  int effect);
	virtual void WritePicture(cv::Mat& dest) = 0;
	int SendMessageProgress();
	virtual int CopyPicture(const wxString& filename, const int& nbFrame, int width, int height) = 0;
	int ExecuteProcess(const wxString& outfile, vector<wxString>& listOfFile, int delay, int fps, int width, int height,
	                   int effect);



	wxProgressDialog* dialog;
	bool endProcess = false;
	int countNbFrame = 0;
	int numWriteFrame = 0;
	CRegardsBitmap* pBitmap = nullptr;
	map<wxString, CRegardsBitmap*> listOfPicture;

};

//**********************************************************************
//
//**********************************************************************
class CThumbnailVideoOpenCVExportImpl : public CThumbnailDiaporama
{
public:

	void WritePicture(cv::Mat & dest);
    int CopyPicture(const wxString& filename, const int& nbFrame, int width, int height);
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
int CThumbnailVideoOpenCVExportImpl::CopyPicture(const wxString& filename, const int& nbFrame, int width, int height)
{
	cv::Mat dest;
    CRegardsBitmap* src_bitmap = listOfPicture[filename]; //CThumbnailDiaporama::GenerateBitmapForVideo(filename, width, height);
    cvtColor(src_bitmap->GetMatrix(), dest, cv::COLOR_BGRA2BGR);

    for (int i = 0; i < nbFrame; i++)
    {
        outputVideo.write(dest);
        
        SendMessageProgress();
        if (endProcess)
            break;
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
CRegardsBitmap* CThumbnailDiaporama::GenerateBitmapForVideo(const wxString& filename, int width, int height)
{
	auto src_bitmap = new CRegardsBitmap(width, height);
	CLibPicture libPicture;
	bool pictureOK = true;

	CRegardsBitmap* pBitmap = libPicture.LoadPictureToBGRA(filename, pictureOK, width, height);
	if (pBitmap != nullptr)
	{
		src_bitmap->SetBackgroundColor(CRgbaquad(0, 0, 0, 0));
		int x = (width - pBitmap->GetBitmapWidth()) / 2;
		int y = (height - pBitmap->GetBitmapHeight()) / 2;
		src_bitmap->InsertBitmap(pBitmap, x, y, false);
		src_bitmap->VertFlipBuf();
	}

	delete pBitmap;
	return src_bitmap;
}

//**********************************************************************
//
//**********************************************************************
int CThumbnailDiaporama::ExecuteEffect(const wxString& filename1, const wxString& filename2, const int& nbFrame,
                                       int width, int height, int effect)
{
	CLibPicture libPicture;
	CRegardsBitmap* pBitmap1 = nullptr;
	if (!filename1.empty())
		pBitmap1 = listOfPicture[filename1]; // GenerateBitmapForVideo(filename1, width, height);


	CRegardsBitmap* pBitmap2 = nullptr;
	if (!filename2.empty())
		pBitmap2 = listOfPicture[filename2]; //GenerateBitmapForVideo(filename2, width, height);

	Mat src2;
	Mat src1;
	Mat dest;

	if (effect != IDM_DIAPORAMA_MOVE)
	{
		if (!filename1.empty())
		{
			dest = pBitmap1->GetMatrix();
			cvtColor(dest, src1, COLOR_BGRA2BGR);
		}

		if (!filename2.empty())
		{
			dest = pBitmap2->GetMatrix();
			cvtColor(dest, src2, COLOR_BGRA2BGR);
		}
	}

	switch (effect)
	{
	case IDM_DIAPORAMA_FUSION:
		{
			if (!filename1.empty())
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
				Rect myROI(x, y, 1920, 1080);
				dest = dest(myROI);
				WritePicture(dest);
				SendMessageProgress();
				if (endProcess)
					break;
			}
		}
		break;

	case IDM_DIAPORAMA_MOVE:
		{
			if (!filename1.empty())
			{
				for (int k = 0; k < nbFrame; k++)
				{
					float alpha = static_cast<float>(k) / static_cast<float>(nbFrame);
					//float beta = 1.0 - alpha;

					int x = 1920 - (alpha * 1920);
					int x2 = -(alpha * 1920);

					pBitmap->SetBackgroundColor(CRgbaquad(0, 0, 0, 0));
					pBitmap->InsertBitmap(pBitmap1, x2, 0, false);
					pBitmap->InsertBitmap(pBitmap2, x, 0, false);
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

	pBitmap = new CRegardsBitmap(width, height);

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

	for (int i = 0; i < picturefile.size(); i++)
	{
		//int position;
		if ((i == 0 || effect == IDM_DIAPORAMA_NONE) && effect != IDM_DIAPORAMA_TRANSITION)
		{
			CRegardsBitmap* src_bitmap = GenerateBitmapForVideo(listOfFile[i], width, height);
			listOfPicture[listOfFile[i]] = src_bitmap;

			CopyPicture(picturefile[i], nbFrameByPicture, width, height);
			if (endProcess)
				break;
		}
		else
		{
			CRegardsBitmap* src_bitmap = GenerateBitmapForVideo(listOfFile[i], width, height);
			listOfPicture[listOfFile[i]] = src_bitmap;

			switch (effect)
			{
			case IDM_DIAPORAMA_TRANSITION:
				{
					//int iStart = i * nbFrameByPicture;
					ExecuteEffect("", picturefile[i], nbFrameByPicture, width, height, effect);
					if (endProcess)
						break;
				}
				break;

			default:
				{
					int iStart = i * nbFrameByPicture + nbFrameEffect * (i - 1);
					ExecuteEffect(picturefile[i - 1], picturefile[i], nbFrameEffect, width, height, effect);
					iStart += nbFrameEffect;
					if (endProcess)
						break;

					CopyPicture(picturefile[i], nbFrameByPicture, width, height);
					if (endProcess)
						break;
				}
				break;
			}
		}

		if (i > 2)
		{
			for (int i1 = i1 - 2; i1 < i1 - 1; i1++)
			{
				CRegardsBitmap* src_bitmap = listOfPicture[listOfFile[i1]];
				if (src_bitmap != nullptr)
					delete src_bitmap;
				listOfPicture[listOfFile[i1]] = nullptr;
			}
		}

		if (endProcess)
			break;
	}

	if (endProcess)
		movie_duration = 0;

	for (int i = 0; i < listOfFile.size(); i++)
	{
		CRegardsBitmap* src_bitmap = listOfPicture[listOfFile[i]];
		if (src_bitmap != nullptr)
			delete src_bitmap;
	}
	listOfPicture.clear();
	delete pBitmap;
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
    thumbnailImpl.outputVideo.open(outfile.ToStdString(), fourcc, fps, S, true);

	if (thumbnailImpl.outputVideo.isOpened())
	{
		movie_duration = thumbnailImpl.ExecuteProcess(outfile, listOfFile, delay, fps, width, height, effect);

		thumbnailImpl.outputVideo.release();

		if (thumbnailImpl.endProcess)
			movie_duration = 0;
	}
    return movie_duration;
}
