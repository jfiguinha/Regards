#include <header.h>
#include "ThumbnailVideoExport.h"
#include "FFmpegDecodeFrame.h"
#include "RegardsBitmap.h"
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/core.hpp>
#include "libPicture.h"
#include <wx/progdlg.h>
#include <effect_id.h>
using namespace cv;
using namespace Regards::Picture;

CThumbnailVideoExport::CThumbnailVideoExport()
{

}

CThumbnailVideoExport::~CThumbnailVideoExport()
{

}

int CThumbnailVideoExport::GenerateVideoFromList(const wxString& outfile, vector<wxString> & listOfFile, int delay, int fps, int width, int height, int effect)
{
    int movie_duration = 0;
    //int codec = VideoWriter::fourcc('H', 'V', 'C', '1');
    int codec = VideoWriter::fourcc('M', 'P', '4', 'V'); 
    Size S = Size((int)width,    // Acquire input size
        (int)height);
    VideoWriter outputVideo;                                        // Open the output
    outputVideo.open(outfile.ToStdString(), codec, fps, S, true);

    if (outputVideo.isOpened())
    {
        CLibPicture libPicture;
        vector<wxString> picturefile;
        for (int i = 0; i < listOfFile.size(); i++)
        {
            if (libPicture.TestIsPicture(listOfFile[i]))
                picturefile.push_back(listOfFile[i]);
        }

        movie_duration = delay * picturefile.size();
        int countNbFrame = delay * fps * picturefile.size();
        int nbFrameByPicture = delay * fps;
        Mat src1;
        Mat src2;
        int nbFrameEffect = fps * 2;
        int j = 0;

        wxProgressDialog dialog("Export File", "Checking...", countNbFrame, nullptr, wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);
       
        float ratio = 1.0;
        for (int i = 0; i < countNbFrame; i++) //Show the image captured in the window and repeat
        {
            if (i % nbFrameByPicture == 0)
            {
                ratio = 1.0;

                if (i != 0)
                    src1.copyTo(src2);

                CRegardsBitmap* src_bitmap = new CRegardsBitmap(width, height);
               
                bool pictureOK = true;

                CRegardsBitmap* pBitmap = libPicture.LoadPictureToBGRA(picturefile[j++], pictureOK, width, height);
                if (pBitmap != nullptr)
                {
                    pBitmap->VertFlipBuf();
                    src_bitmap->SetBackgroundColor(CRgbaquad(0, 0, 0, 0));
                    int x = (width - pBitmap->GetBitmapWidth()) / 2;
                    int y = (height - pBitmap->GetBitmapHeight()) / 2;
                    src_bitmap->InsertBitmap(pBitmap, x, y, false);

                    Mat dest = cv::Mat(src_bitmap->GetBitmapHeight(), src_bitmap->GetBitmapWidth(), CV_8UC4, src_bitmap->GetPtBitmap());
                    cvtColor(dest, src1, cv::COLOR_BGRA2BGR);
                    dest.release();
                }

                
                delete pBitmap;
                delete src_bitmap;

                if (i != 0)
                {
                    CRegardsBitmap* bitmap1;
                    CRegardsBitmap* bitmap2;
                    if (effect == IDM_DIAPORAMA_MOVE)
                    {
                        cv::Mat dst1;
                        cv::Mat dst2;
                        bitmap1 = new CRegardsBitmap(width, height);
                        bitmap2 = new CRegardsBitmap(width, height);

                        cvtColor(src2, dst1, cv::COLOR_BGR2BGRA);

                        bitmap1->SetBitmap(dst1.data, width, height);

                        cvtColor(src1, dst2, cv::COLOR_BGR2BGRA);

                        bitmap2->SetBitmap(dst2.data, width, height);

                        dst1.release();
                        dst2.release();
                    }

                    if (effect != IDM_DIAPORAMA_NONE && effect != IDM_DIAPORAMA_TRANSITION)
                    {
                        movie_duration += 2;

                        for (int k = 0; k < nbFrameEffect; k++)
                        {
                            cv::Mat dst;
                            float alpha = (float)k / (float)nbFrameEffect;
                            float beta = 1.0 - alpha;
                            //Apply Transition Effect
                            switch (effect)
                            {
                                case IDM_DIAPORAMA_FUSION:
                                {
                                    addWeighted(src1, alpha, src2, beta, 0.0, dst);
                                }
                                break;

                                case IDM_DIAPORAMA_MOVE:
                                    {
                                        int x = 1920 - (alpha * 1920);
                                        int x2 = -(alpha * 1920);
                                        CRegardsBitmap* pBitmap = new CRegardsBitmap(width, height);
                                        pBitmap->SetBackgroundColor(CRgbaquad(0, 0, 0, 0));
                                        pBitmap->InsertBitmap(bitmap1, x2, 0);
                                        pBitmap->InsertBitmap(bitmap2, x, 0);

                                        Mat dest = cv::Mat(pBitmap->GetBitmapHeight(), pBitmap->GetBitmapWidth(), CV_8UC4, pBitmap->GetPtBitmap());
                                        cvtColor(dest, dst, cv::COLOR_BGRA2BGR);
                                        dest.release();
                                        delete pBitmap;

                                        break;
                                    }
                            
                            }
                            outputVideo << dst;
                        }
                    }

                    if (effect == IDM_DIAPORAMA_MOVE)
                    {
                        delete bitmap1;
                        delete bitmap2;
                    }
                }
            }

            wxString message = "In progress : " + to_string(i) + "/" + to_string(countNbFrame);
            if (false == dialog.Update(i, message))
            {
                break;
            }

            if (effect == IDM_DIAPORAMA_TRANSITION)
            {
                cv::Mat dst;
                ratio = ratio + 0.0005;
                resize(src1, dst, Size(), ratio, ratio);
                cv::Size sDst = dst.size();
                cv::Size sSrc = src1.size();
                int y = (sDst.height - sSrc.height) / 2;
                int x = (sDst.width - sSrc.width) / 2;
                cv::Rect myROI(x, y, 1920, 1080);
                dst = dst(myROI);
                outputVideo << dst;
            }
            else
                outputVideo << src1;
        }
    }

    outputVideo.release();

    return movie_duration;
}