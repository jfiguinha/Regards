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
#include "ImageLoadingFormat.h"
#include <FileUtility.h>
using namespace cv;
using namespace Regards::Picture;

class CThumbnailVideoExportImpl
{
public:

    static wxString GenerateOutputFileName(int numStart);
    static CRegardsBitmap* GenerateBitmapForVideo(const wxString& filename, int width, int height);
#ifdef PICTURE_SAVING
    static int CopyPicture(const wxString& filename, const int& numStart, const int& nbFrame, int width, int height);
    static int ExecuteEffect(const wxString& filename1, const wxString& filename2, const int& numStart, const int& nbFrame, int width, int height, int effect);

#else
    static int CopyPicture(const wxString& filename, const int& numStart, const int& nbFrame, int width, int height, VideoWriter & outputVideo);
    static int ExecuteEffect(const wxString& filename1, const wxString& filename2, const int& numStart, const int& nbFrame, int width, int height, int effect, VideoWriter & outputVideo);
#endif
};

//#define PICTURE_SAVING 1

CThumbnailVideoExport::CThumbnailVideoExport()
{

}

CThumbnailVideoExport::~CThumbnailVideoExport()
{

}

wxString CThumbnailVideoExportImpl::GenerateOutputFileName(int numStart)
{
    wxString fileOutName;
    wxString documentPath = CFileUtility::GetDocumentFolderPath();
#ifdef WIN32
    wxString tempFolder = documentPath + "\\temp";
    if (!wxMkDir(tempFolder)) {
#else
    wxString tempFolder = documentPath + "/temp";
    if (!wxMkDir(tempFolder, wxS_DIR_DEFAULT)) {
#endif
        // handle the error here
    }
    else
    {
#ifdef WIN32
        fileOutName = tempFolder + "\\picture";
#else
        fileOutName = tempFolder + "/picture";
#endif

    }
    fileOutName.append(to_string(numStart));
    fileOutName.append(".jpg");

    return fileOutName;
}

int CThumbnailVideoExportImpl::CopyPicture(const wxString &filename, const int &numStart, const int &nbFrame, int width, int height, VideoWriter & outputVideo)
{

#ifdef PICTURE_SAVING
    wxString fileOut = GenerateOutputFileName(numStart);

    CRegardsBitmap* src_bitmap = GenerateBitmapForVideo(filename, width, height);
    CImageLoadingFormat loadingFormat;
    loadingFormat.SetPicture(src_bitmap);
    loadingFormat.SaveToJpeg(fileOut);

    for (int i = 1; i < nbFrame; i++)
    {
        wxString fileOutCopy = GenerateOutputFileName(numStart + i);
        wxCopyFile(fileOut, fileOutCopy);
    }
#else

    CRegardsBitmap* src_bitmap = GenerateBitmapForVideo(filename, width, height);
    src_bitmap->VertFlipBuf();
    Mat dest = cv::Mat(src_bitmap->GetBitmapHeight(), src_bitmap->GetBitmapWidth(), CV_8UC4, src_bitmap->GetPtBitmap());
    cvtColor(dest, dest, cv::COLOR_BGRA2BGR);

    for (int i = 0; i < nbFrame; i++)
    {
        outputVideo << dest;
    }

    delete src_bitmap;
    dest.release();

#endif
}

CRegardsBitmap* CThumbnailVideoExportImpl::GenerateBitmapForVideo(const wxString& filename, int width, int height)
{
    CRegardsBitmap* src_bitmap = new CRegardsBitmap(width, height);
    CLibPicture libPicture;
    bool pictureOK = true;

    CRegardsBitmap* pBitmap = libPicture.LoadPictureToBGRA(filename, pictureOK, width, height);
    if (pBitmap != nullptr)
    {
        src_bitmap->SetBackgroundColor(CRgbaquad(0, 0, 0, 0));
        int x = (width - pBitmap->GetBitmapWidth()) / 2;
        int y = (height - pBitmap->GetBitmapHeight()) / 2;
        src_bitmap->InsertBitmap(pBitmap, x, y, false);
    }

    delete pBitmap;
    return src_bitmap;
}

int CThumbnailVideoExportImpl::ExecuteEffect(const wxString& filename1, const wxString& filename2, const int& numStart, const int& nbFrame, int width, int height, int effect, VideoWriter & outputVideo)
{
    CLibPicture libPicture;
    bool pictureOK = true;
    CRegardsBitmap* pBitmap1 = nullptr;
    if (filename1 != "")
        pBitmap1 = GenerateBitmapForVideo(filename1, width, height);


    CRegardsBitmap* pBitmap2 = nullptr;
    if (filename2 != "")
        pBitmap2 = GenerateBitmapForVideo(filename2, width, height);

    Mat src2;
    Mat src1;
    Mat dest;

    if (effect != IDM_DIAPORAMA_MOVE)
    {
        if (filename1 != "")
        {
            pBitmap1->VertFlipBuf();
            dest = cv::Mat(pBitmap1->GetBitmapHeight(), pBitmap1->GetBitmapWidth(), CV_8UC4, pBitmap1->GetPtBitmap());
            cvtColor(dest, src1, cv::COLOR_BGRA2BGR);
            dest.release();
        }

        if (filename2 != "")
        {
            pBitmap2->VertFlipBuf();
            dest = cv::Mat(pBitmap2->GetBitmapHeight(), pBitmap2->GetBitmapWidth(), CV_8UC4, pBitmap2->GetPtBitmap());
            cvtColor(dest, src2, cv::COLOR_BGRA2BGR);
            dest.release();
        }
    }

    switch (effect)
    {
        case IDM_DIAPORAMA_FUSION:
        {
            if (filename1 != "")
            {
                for (int k = 0; k < nbFrame; k++)
                {
                    float alpha = (float)k / (float)nbFrame;
                    float beta = 1.0 - alpha;
                    addWeighted(src1, alpha, src2, beta, 0.0, dest);
#ifdef PICTURE_SAVING
                    wxString fileOutCopy = GenerateOutputFileName(numStart + k);
                    imwrite(fileOutCopy.ToStdString(), dest);
#else
                    outputVideo << dest;
#endif
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
                cv::Size sDst = dest.size();
                cv::Size sSrc = src2.size();
                int y = (sDst.height - sSrc.height) / 2;
                int x = (sDst.width - sSrc.width) / 2;
                cv::Rect myROI(x, y, 1920, 1080);
#ifdef PICTURE_SAVING
                wxString fileOutCopy = GenerateOutputFileName(numStart + k);
                imwrite(fileOutCopy.ToStdString(), dest(myROI));
#else
                outputVideo << dest(myROI);
#endif
            }
        }
        break;

        case IDM_DIAPORAMA_MOVE:
        {
            if (filename1 != "")
            {
                CRegardsBitmap* pBitmap = new CRegardsBitmap(width, height);

                for (int k = 0; k < nbFrame; k++)
                {
                    float alpha = (float)k / (float)nbFrame;
                    float beta = 1.0 - alpha;

                    int x = 1920 - (alpha * 1920);
                    int x2 = -(alpha * 1920);
                    
                    pBitmap->SetBackgroundColor(CRgbaquad(0, 0, 0, 0));
                    pBitmap->InsertBitmap(pBitmap1, x2, 0, false);
                    pBitmap->InsertBitmap(pBitmap2, x, 0, false);
                    pBitmap->VertFlipBuf();
                    cv::Mat dest = cv::Mat(pBitmap->GetBitmapHeight(), pBitmap->GetBitmapWidth(), CV_8UC4, pBitmap->GetPtBitmap());
                    cvtColor(dest, dest, cv::COLOR_BGRA2BGR);

#ifdef PICTURE_SAVING
                    wxString fileOutCopy = GenerateOutputFileName(numStart + k);
                    imwrite(fileOutCopy.ToStdString(), dest);
                    
#else
                    outputVideo << dest;
#endif
                    dest.release();
                   
                }

                delete pBitmap;
            }

            break;
        }

    }

    if(pBitmap1)
        delete pBitmap1;

    if (pBitmap2)
        delete pBitmap2;

    src2.release();
    src1.release();
    dest.release();
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

        int nbFrameByPicture = delay * fps;
        Mat src1;
        Mat src2;
        int nbFrameEffect = fps * 2;
        int j = 0;
        int countNbFrame = delay * fps * picturefile.size();
        if(effect != IDM_DIAPORAMA_TRANSITION)
            countNbFrame += nbFrameEffect * (picturefile.size() - 1);

        movie_duration = countNbFrame / fps;

        wxProgressDialog dialog("Export File", "Checking...", countNbFrame, nullptr, wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);
        for (int i = 0; i < picturefile.size(); i++)
        {
            int position = 0;
            if (i == 0 && effect != IDM_DIAPORAMA_TRANSITION)
            {
                CThumbnailVideoExportImpl::CopyPicture(picturefile[i], 0, nbFrameByPicture, width, height, outputVideo);
                position = nbFrameByPicture;
            }
            else
            {
                switch (effect)
                {
                    case IDM_DIAPORAMA_TRANSITION:
                        {
                            int iStart = i * nbFrameByPicture;
                            CThumbnailVideoExportImpl::ExecuteEffect("", picturefile[i], iStart, nbFrameByPicture, width, height, effect, outputVideo);
                            position = iStart + nbFrameByPicture;
                        }
                    break;

                    default:
                        {
                            int iStart = i * nbFrameByPicture + nbFrameEffect * (i - 1);
                            CThumbnailVideoExportImpl::ExecuteEffect(picturefile[i - 1], picturefile[i], iStart, nbFrameByPicture, width, height, effect, outputVideo);
                            iStart += nbFrameEffect;
                            CThumbnailVideoExportImpl::CopyPicture(picturefile[i], iStart, nbFrameByPicture, width, height, outputVideo);
                            position = iStart + nbFrameByPicture;
                        }
                    break;
                }
            }

            wxString message = "In progress : " + to_string(position) + "/" + to_string(countNbFrame);
            if (false == dialog.Update(position, message))
            {
                break;
            }
        }
    }

    outputVideo.release();

    return movie_duration;
}