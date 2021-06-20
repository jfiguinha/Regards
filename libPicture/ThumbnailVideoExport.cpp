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

#define FFMPEG_SAVING

class CThumbnailVideoExportImpl
{
public:

    wxString GenerateOutputFileName(int numStart);
    CRegardsBitmap* GenerateBitmapForVideo(const wxString& filename, int width, int height);
    int CopyPicture(const wxString& filename, const int& numStart, const int& nbFrame, int width, int height);
    int ExecuteEffect(const wxString& filename1, const wxString& filename2, const int& numStart, const int& nbFrame, int width, int height, int effect);
    
    
#ifndef FFMPEG_SAVING
    VideoWriter outputVideo;
#endif

#ifdef FFMPEG_SAVING 
    int GenerateFFmpegVideoFromList(const wxString& outfile, vector<wxString>& listOfFile, int delay, int fps, int width, int height, int effect);
    int WritePicture(CRegardsBitmap* bitmapData, int numFrame);
    int encode(AVCodecContext* enc_ctx, AVFrame* frame, AVPacket* pkt);
    int fps;

private:

    AVDictionary* opt = nullptr;
    SwsContext* convertCtx = NULL;
     AVOutputFormat* fmt;
    AVFormatContext* oc = NULL;
    AVStream* stream = NULL;
    AVCodec* codec = NULL;
    AVCodecContext* c = NULL;
    AVPacket* pkt;
    AVFrame* yuvpic;
#endif
};

//#define PICTURE_SAVING

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


#ifdef FFMPEG_SAVING 
int CThumbnailVideoExportImpl::WritePicture(CRegardsBitmap * bitmapData, int numFrame)
{
    int ret = 0;
    uint8_t* convertedFrameBuffer = bitmapData->GetPtBitmap();
    int linesize = bitmapData->GetBitmapWidth() * 4;
    
    sws_scale(convertCtx, &convertedFrameBuffer, &linesize, 0, bitmapData->GetBitmapHeight(),
        yuvpic->data, yuvpic->linesize);

    av_init_packet(pkt);
    pkt->data = NULL;
    pkt->size = 0;
    yuvpic->pts = numFrame; // The PTS of the frame are just in a reference unit, unrelated to the format we are using. We set them, for instance, as the corresponding frame number.
    /* encode the image */
    encode(c, yuvpic, pkt);

    return 0;
}
#endif

int CThumbnailVideoExportImpl::CopyPicture(const wxString &filename, const int &numStart, const int &nbFrame, int width, int height)
{

#ifdef FFMPEG_SAVING
    CRegardsBitmap* src_bitmap = GenerateBitmapForVideo(filename, width, height);
    src_bitmap->VertFlipBuf();

    int ret = 0;
    uint8_t* convertedFrameBuffer = src_bitmap->GetPtBitmap();
    int linesize = src_bitmap->GetBitmapWidth() * 4;

    sws_scale(convertCtx, &convertedFrameBuffer, &linesize, 0, src_bitmap->GetBitmapHeight(),
        yuvpic->data, yuvpic->linesize);

    for (int i = 0; i < nbFrame; i++)
    {
        av_init_packet(pkt);
        pkt->data = NULL;
        pkt->size = 0;
        yuvpic->pts = numStart + i;
        encode(c, yuvpic, pkt);
        //WritePicture(src_bitmap, numStart + i);
    }
    delete src_bitmap;
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
    
    return 0;
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

int CThumbnailVideoExportImpl::ExecuteEffect(const wxString& filename1, const wxString& filename2, const int& numStart, const int& nbFrame, int width, int height, int effect)
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
            CRegardsBitmap* pBitmap = new CRegardsBitmap(width, height);
            for (int k = 0; k < nbFrame; k++)
            {
                float alpha = (float)k / (float)nbFrame;
                float beta = 1.0 - alpha;
                addWeighted(src1, alpha, src2, beta, 0.0, dest);
#ifdef FFMPEG_SAVING
                pBitmap->SetBitmap(dest.data, width, height);        
                WritePicture(pBitmap, numStart + k);
#else
                outputVideo << dest;
#endif
            }
            delete pBitmap;
        }


    }
    break;

    case IDM_DIAPORAMA_TRANSITION:
    {
        float ratio = 1.0;

        CRegardsBitmap* pBitmap = new CRegardsBitmap(width, height);
        for (int k = 0; k < nbFrame; k++)
        {
            ratio = ratio + 0.0005;
            resize(src2, dest, Size(), ratio, ratio);
            cv::Size sDst = dest.size();
            cv::Size sSrc = src2.size();
            int y = (sDst.height - sSrc.height) / 2;
            int x = (sDst.width - sSrc.width) / 2;
            cv::Rect myROI(x, y, 1920, 1080);
#ifdef FFMPEG_SAVING
            dest = dest(myROI);
            pBitmap->SetBitmap(dest.data, width, height);
            WritePicture(pBitmap, numStart + k);
#else
            outputVideo << dest(myROI);
#endif
        }

        delete pBitmap;
    }
    break;

    case IDM_DIAPORAMA_MOVE:
    {
        if (filename1 != "")
        {
            CRegardsBitmap* pBitmap = new CRegardsBitmap(width, height);

#ifdef PICTURE_SAVING
#pragma omp parallel for 
#endif
            for (int k = 0; k < nbFrame; k++)
            {
                float alpha = (float)k / (float)nbFrame;
                float beta = 1.0 - alpha;

                int x = 1920 - (alpha * 1920);
                int x2 = -(alpha * 1920);

                pBitmap->SetBackgroundColor(CRgbaquad(0, 0, 0, 0));
                pBitmap->InsertBitmap(pBitmap1, x2, 0, false);
                pBitmap->InsertBitmap(pBitmap2, x, 0, false);


#ifdef FFMPEG_SAVING
                pBitmap->VertFlipBuf();
                WritePicture(pBitmap, numStart + k);
#else
                pBitmap->VertFlipBuf();
                cv::Mat dest = cv::Mat(pBitmap->GetBitmapHeight(), pBitmap->GetBitmapWidth(), CV_8UC4, pBitmap->GetPtBitmap());
                cvtColor(dest, dest, cv::COLOR_BGRA2BGR);
                outputVideo << dest;
                dest.release();
#endif
                

            }

            delete pBitmap;
        }

        break;
    }

    }

    if (pBitmap1)
        delete pBitmap1;

    if (pBitmap2)
        delete pBitmap2;

    src2.release();
    src1.release();
    dest.release();

    return 0;
}

#ifdef FFMPEG_SAVING 
int CThumbnailVideoExportImpl::encode(AVCodecContext* enc_ctx, AVFrame* frame, AVPacket* pkt)
{
    int ret;

    /* send the frame to the encoder */
    if (frame)
        printf("Send frame %d\n", frame->pts);

    ret = avcodec_send_frame(enc_ctx, frame);
    if (ret < 0) {
        //fprintf(stderr, "Error sending a frame for encoding\n");
        return 1;
    }

    while (ret >= 0) {
        ret = avcodec_receive_packet(enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return 1;
        else if (ret < 0) {
            //fprintf(stderr, "Error during encoding\n");
            return 1;
        }

        //pkt->pts = av_rescale_q(pkt->pts, { 1, fps }, stream->time_base);
        av_packet_rescale_ts(pkt, {1, fps }, stream->time_base); // We set the packet PTS and DTS taking in the account our FPS (second argument) and the time base that our selected format uses (third argument).
        pkt->stream_index = stream->index;
        //printf("Write frame %6d (size=%6d)\n", i, pkt->size);
        av_interleaved_write_frame(oc, pkt); // Write the encoded frame to the mp4 file.

        av_packet_unref(pkt);
    }

    return ret;
}

int CThumbnailVideoExportImpl::GenerateFFmpegVideoFromList(const wxString& outfile, vector<wxString>& listOfFile, int delay, int fps, int width, int height, int effect)
{

    int countNbFrame = 0;
    int movie_duration = 0;
    int ret = 0;
    convertCtx = sws_getContext(width, height, AV_PIX_FMT_BGRA, width, height, AV_PIX_FMT_YUV420P, SWS_FAST_BILINEAR, NULL, NULL, NULL); // Preparing to convert my generated RGB images to YUV frames.

    avformat_alloc_output_context2(&oc, NULL, NULL, outfile);
    stream = avformat_new_stream(oc, 0);

    codec = avcodec_find_encoder_by_name("libx264");
    if (!codec) {
        fprintf(stderr, "Codec '%s' not found\n", "libx264");
        return 0;
    }

    // Setting up the codec:
// Setting up the codec:
    avcodec_get_context_defaults3(stream->codec, codec);
    //c=avcodec_alloc_context3(codec);
    int videoBitRate = 2500;
    c = stream->codec;
    c->width = width;
    c->height = height;
    c->pix_fmt = AV_PIX_FMT_YUV420P;
    c->time_base = { 1, fps };
    c->framerate = { fps, 1 };
    c->bit_rate = 2500000;
    /* Average bitrate */
    c->bit_rate = 1000 * videoBitRate;
    // ffmpeg's mpeg2 encoder requires that the bit_rate_tolerance be >=
    // bitrate * fps
    c->bit_rate_tolerance = videoBitRate * av_q2d(c->framerate) + 1;
    // Setting up the format, its stream(s), linking with the codec(s) and write the header:
    if (oc->oformat->flags & AVFMT_GLOBALHEADER) // Some formats require a global header.
        c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;


    /* open it */
    ret = avcodec_open2(c, codec, NULL);
    if (ret < 0) {
       // fprintf(stderr, "Could not open codec: %s\n", av_err2str(ret));
        exit(1);
    }

    stream->time_base = { 1, fps };
    stream->codec = c; // Once the codec is set up, we need to let the container know which codec are the streams using, in this case the only (video) stream.
    av_dump_format(oc, 0, outfile, 1);
    avio_open(&oc->pb, outfile, AVIO_FLAG_WRITE);
    ret = avformat_write_header(oc, &opt);
    av_dict_free(&opt);

    pkt = av_packet_alloc();
    if (!pkt)
        return 0;

    // Allocating memory for each conversion output YUV frame:
    yuvpic = av_frame_alloc();
    yuvpic->format = AV_PIX_FMT_YUV420P;
    yuvpic->width = width;
    yuvpic->height = height;
    ret = av_frame_get_buffer(yuvpic, 1);

    CLibPicture libPicture;
    vector<wxString> picturefile;
    for (int i = 0; i < listOfFile.size(); i++)
    {
        if (libPicture.TestIsPicture(listOfFile[i]))
            picturefile.push_back(listOfFile[i]);
    }

    int position = 0;
    int nbFrameByPicture = delay * fps;
    int nbFrameEffect = fps * 2;
    int j = 0;
    countNbFrame = delay * fps * picturefile.size();

    if (effect != IDM_DIAPORAMA_TRANSITION)
        countNbFrame += nbFrameEffect * (picturefile.size() - 1);

    movie_duration = countNbFrame / fps;

    wxProgressDialog dialog("Export File", "Checking...", countNbFrame, nullptr, wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);

    for (int i = 0; i < picturefile.size(); i++)
    {
            
        if (i == 0 && effect != IDM_DIAPORAMA_TRANSITION)
        {
            CopyPicture(picturefile[i], 0, nbFrameByPicture, width, height);
            position = nbFrameByPicture;
        }
        else
        {
            switch (effect)
            {
            case IDM_DIAPORAMA_TRANSITION:
            {
                int iStart = i * nbFrameByPicture;
                ExecuteEffect("", picturefile[i], iStart, nbFrameByPicture, width, height, effect);
                position = iStart + nbFrameByPicture;
            }
            break;

            default:
            {
                int iStart = i * nbFrameByPicture + nbFrameEffect * (i - 1);
                ExecuteEffect(picturefile[i - 1], picturefile[i], iStart, nbFrameByPicture, width, height, effect);
                iStart += nbFrameEffect;
                CopyPicture(picturefile[i], iStart, nbFrameByPicture, width, height);
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

    /* flush the encoder */
    encode(c, NULL, pkt);

    av_write_trailer(oc); // Writing the end of the file.
    if (!(fmt->flags & AVFMT_NOFILE))
        avio_closep(&oc->pb); // Closing the file.
    avcodec_close(stream->codec);

    // Freeing all the allocated memory:
    sws_freeContext(convertCtx);
    avcodec_free_context(&c);
    av_frame_free(&yuvpic);
    av_packet_free(&pkt);

    return movie_duration;
}
#endif

int CThumbnailVideoExport::GenerateVideoFromList(const wxString& outfile, vector<wxString> & listOfFile, int delay, int fps, int width, int height, int effect)
{
#ifdef FFMPEG_SAVING
    CThumbnailVideoExportImpl thumbnailImpl;
    thumbnailImpl.fps = fps;
    return thumbnailImpl.GenerateFFmpegVideoFromList(outfile, listOfFile, delay, fps, width, height, effect);

#else
    CThumbnailVideoExportImpl thumbnailImpl;
    int countNbFrame = 0;
    int movie_duration = 0;
    //int codec = VideoWriter::fourcc('H', 'V', 'C', '1');
    int codec = VideoWriter::fourcc('M', 'P', '4', 'V'); 
    Size S = Size((int)width,    // Acquire input size
        (int)height);
                                   // Open the output
    thumbnailImpl.outputVideo.open(outfile.ToStdString(), codec, fps, S, true);

    if (thumbnailImpl.outputVideo.isOpened())
    {
        CLibPicture libPicture;
        vector<wxString> picturefile;
        for (int i = 0; i < listOfFile.size(); i++)
        {
            if (libPicture.TestIsPicture(listOfFile[i]))
                picturefile.push_back(listOfFile[i]);
        }

        int nbFrameByPicture = delay * fps;
        int nbFrameEffect = fps * 2;
        int j = 0;
        countNbFrame = delay * fps * picturefile.size();
        if(effect != IDM_DIAPORAMA_TRANSITION)
            countNbFrame += nbFrameEffect * (picturefile.size() - 1);

        movie_duration = countNbFrame / fps;

        wxProgressDialog dialog("Export File", "Checking...", countNbFrame, nullptr, wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);
       
        for (int i = 0; i < picturefile.size(); i++)
        {
            int position = 0;
            if (i == 0 && effect != IDM_DIAPORAMA_TRANSITION)
            {
                thumbnailImpl.CopyPicture(picturefile[i], 0, nbFrameByPicture, width, height);
                position = nbFrameByPicture;
            }
            else
            {
                switch (effect)
                {
                    case IDM_DIAPORAMA_TRANSITION:
                        {
                            int iStart = i * nbFrameByPicture;
                            thumbnailImpl.ExecuteEffect("", picturefile[i], iStart, nbFrameByPicture, width, height, effect);
                            position = iStart + nbFrameByPicture;
                        }
                    break;

                    default:
                        {
                            int iStart = i * nbFrameByPicture + nbFrameEffect * (i - 1);
                            thumbnailImpl.ExecuteEffect(picturefile[i - 1], picturefile[i], iStart, nbFrameByPicture, width, height, effect);
                            iStart += nbFrameEffect;
                            thumbnailImpl.CopyPicture(picturefile[i], iStart, nbFrameByPicture, width, height);
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

    thumbnailImpl.outputVideo.release();

    return movie_duration;

#endif
}
