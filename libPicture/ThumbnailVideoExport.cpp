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
	virtual void WriteVideoFrame(Mat* dest, CRegardsBitmap* pBitmap, int width, int height) = 0;
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
class CThumbnailVideoExportImpl final : public CThumbnailDiaporama
{
public:
	int CopyPicture(const wxString& filename, const int& nbFrame, int width, int height) override;
	void WriteVideoFrame(Mat* dest, CRegardsBitmap* pBitmap, int width, int height) override;

	int GenerateFFmpegVideoFromList(const wxString& outfile, vector<wxString>& listOfFile, int delay, int fps,
	                                int width, int height, int effect);
	int WritePicture(CRegardsBitmap* bitmapData);
	int encode(AVCodecContext* enc_ctx, AVFrame* frame, AVPacket* pkt);

	int fps;

private:
	AVDictionary* opt = nullptr;
	SwsContext* convertCtx = nullptr;
	//AVOutputFormat* fmt_ = nullptr;
	AVFormatContext* oc = nullptr;
	AVStream* stream = nullptr;
	const AVCodec* codec = nullptr;
	AVCodecContext* c = nullptr;
	AVPacket* pkt = nullptr;
	AVFrame* yuvpic = nullptr;
};

#ifdef EXPORT_DIAPORAMA_OPENCV
//**********************************************************************
//
//**********************************************************************
class CThumbnailVideoOpenCVExportImpl : public CThumbnailDiaporama
{
public:
    
    wxString GenerateOutputFileName(int numStart);
    int CopyPicture(const wxString& filename, const int& nbFrame, int width, int height);
    void WriteVideoFrame(Mat* dest, CRegardsBitmap* pBitmap, int width, int height);

    VideoWriter outputVideo;
};

#endif
//#define PICTURE_SAVING

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
int CThumbnailVideoExportImpl::WritePicture(CRegardsBitmap* bitmapData)
{
	uint8_t* convertedFrameBuffer = bitmapData->GetPtBitmap();
	int linesize = bitmapData->GetBitmapWidth() * 4;

	sws_scale(convertCtx, &convertedFrameBuffer, &linesize, 0, bitmapData->GetBitmapHeight(),
	          yuvpic->data, yuvpic->linesize);

	av_init_packet(pkt);
	pkt->data = nullptr;
	pkt->size = 0;
	yuvpic->pts = numWriteFrame;
	// The PTS of the frame are just in a reference unit, unrelated to the format we are using. We set them, for instance, as the corresponding frame number.
	/* encode the image */
	encode(c, yuvpic, pkt);

	return 0;
}
#ifdef EXPORT_DIAPORAMA_OPENCV
//**********************************************************************
//
//**********************************************************************
int CThumbnailVideoOpenCVExportImpl::CopyPicture(const wxString& filename, const int& nbFrame, int width, int height)
{
    CRegardsBitmap* src_bitmap = listOfPicture[filename]; //CThumbnailDiaporama::GenerateBitmapForVideo(filename, width, height);
    Mat dest = cv::Mat(src_bitmap->GetBitmapHeight(), src_bitmap->GetBitmapWidth(), CV_8UC4, src_bitmap->GetPtBitmap());
    cvtColor(dest, dest, cv::COLOR_BGRA2BGR);

    for (int i = 0; i < nbFrame; i++)
    {
        outputVideo << dest;
        
        SendMessageProgress();
        if (endProcess)
            break;
    }

    //delete src_bitmap;
    dest.release();
    return 0;
}
#endif
//**********************************************************************
//
//**********************************************************************
int CThumbnailVideoExportImpl::CopyPicture(const wxString& filename, const int& nbFrame, int width, int height)
{
	CRegardsBitmap* src_bitmap = listOfPicture[filename]; //GenerateBitmapForVideo(filename, width, height);
	uint8_t* convertedFrameBuffer = src_bitmap->GetPtBitmap();
	int linesize = src_bitmap->GetBitmapWidth() * 4;

	sws_scale(convertCtx, &convertedFrameBuffer, &linesize, 0, src_bitmap->GetBitmapHeight(),
	          yuvpic->data, yuvpic->linesize);

	for (int i = 0; i < nbFrame; i++)
	{
		av_init_packet(pkt);
		pkt->data = nullptr;
		pkt->size = 0;
		yuvpic->pts = numWriteFrame;
		encode(c, yuvpic, pkt);
		SendMessageProgress();
		if (endProcess)
			break;
	}
	//delete src_bitmap;

	return 0;
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

#ifdef EXPORT_DIAPORAMA_OPENCV
//**********************************************************************
//
//**********************************************************************
void CThumbnailVideoOpenCVExportImpl::WriteVideoFrame(Mat * dest, CRegardsBitmap* pBitmap, int width, int height)
{
    if (dest == nullptr)
    {
        cv::Mat dest = cv::Mat(pBitmap->GetBitmapHeight(), pBitmap->GetBitmapWidth(), CV_8UC4, pBitmap->GetPtBitmap());
        cvtColor(dest, dest, cv::COLOR_BGRA2BGR);
        outputVideo << dest;
        dest.release();
    }
    else
        outputVideo << *dest;
}
#endif
//**********************************************************************
//
//**********************************************************************
auto CThumbnailVideoExportImpl::WriteVideoFrame(Mat* dest, CRegardsBitmap* pBitmap, int width, int height) -> void
{
	if (dest == nullptr)
	{
		WritePicture(pBitmap);
	}
	else
	{
		Mat destination;
		cvtColor(*dest, destination, COLOR_BGR2BGRA);
		pBitmap->SetMatrix(destination);
		WritePicture(pBitmap);
		destination.release();
	}
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
					WriteVideoFrame(&dest, pBitmap, width, height);
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
				WriteVideoFrame(&dest, pBitmap, width, height);

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
					WriteVideoFrame(nullptr, pBitmap, width, height);
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
int CThumbnailVideoExportImpl::encode(AVCodecContext* enc_ctx, AVFrame* frame, AVPacket* pkt)
{

	int ret = avcodec_send_frame(enc_ctx, frame);
	if (ret < 0)
	{
		//fprintf(stderr, "Error sending a frame for encoding\n");
		return 1;
	}

	while (ret >= 0)
	{
		ret = avcodec_receive_packet(enc_ctx, pkt);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			return 1;
		if (ret < 0)
		{
			//fprintf(stderr, "Error during encoding\n");
			return 1;
		}

		//pkt->pts = av_rescale_q(pkt->pts, { 1, fps }, stream->time_base);
		av_packet_rescale_ts(pkt, {1, fps}, stream->time_base);
		// We set the packet PTS and DTS taking in the account our FPS (second argument) and the time base that our selected format uses (third argument).
		pkt->stream_index = stream->index;
		//printf("Write frame %6d (size=%6d)\n", i, pkt->size);
		av_interleaved_write_frame(oc, pkt); // Write the encoded frame to the mp4 file.

		av_packet_unref(pkt);
	}

	return ret;
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
int CThumbnailVideoExportImpl::GenerateFFmpegVideoFromList(const wxString& outfile, vector<wxString>& listOfFile,
                                                           int delay, int fps, int width, int height, int effect)
{
	int ret = 0;
	convertCtx = sws_getContext(width, height, AV_PIX_FMT_BGRA, width, height, AV_PIX_FMT_YUV420P, SWS_FAST_BILINEAR,
	                            nullptr, nullptr, nullptr);
	// Preparing to convert my generated RGB images to YUV frames.
	avformat_alloc_output_context2(&oc, nullptr, nullptr, outfile);
	stream = avformat_new_stream(oc, nullptr);

	codec = avcodec_find_encoder(AV_CODEC_ID_H265);
	if (!codec)
	{
		fprintf(stderr, "Codec '%s' not found\n", "AV_CODEC_ID_H265");
		return 0;
	}

	// Setting up the codec:
	// Setting up the codec:
	//avcodec_get_context_defaults3(stream->codec, codec);
	c = avcodec_alloc_context3(codec);
	int videoBitRate = 1500;
	//c = stream->codec;
	c->width = width;
	c->height = height;
	c->pix_fmt = AV_PIX_FMT_YUV420P;
	c->time_base = {1, fps};
	c->framerate = {fps, 1};
	//c->bit_rate = 2500000;
	/* Average bitrate */
	c->bit_rate = 1000 * videoBitRate;
	// ffmpeg's mpeg2 encoder requires that the bit_rate_tolerance be >=
	// bitrate * fps
	c->bit_rate_tolerance = videoBitRate * av_q2d(c->framerate) + 1;
	// Setting up the format, its stream(s), linking with the codec(s) and write the header:
	if (oc->oformat->flags & AVFMT_GLOBALHEADER) // Some formats require a global header.
		c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;


	/* open it */
	ret = avcodec_open2(c, codec, nullptr);
	if (ret < 0)
	{
		// fprintf(stderr, "Could not open codec: %s\n", av_err2str(ret));
		return 0;
	}

	stream->time_base = {1, fps};
	//stream->codec = c;
	// Once the codec is set up, we need to let the container know which codec are the streams using, in this case the only (video) stream.
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

	int movie_duration = ExecuteProcess(outfile, listOfFile, delay, fps, width, height, effect);

	/* flush the encoder */
	if (!endProcess)
		encode(c, nullptr, pkt);

	av_write_trailer(oc); // Writing the end of the file.
	avio_closep(&oc->pb); // Closing the file.
	avcodec_close(c);

	// Freeing all the allocated memory:
	sws_freeContext(convertCtx);
	avcodec_free_context(&c);
	av_frame_free(&yuvpic);
	av_packet_free(&pkt);

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
#ifdef EXPORT_DIAPORAMA_OPENCV
    CThumbnailVideoOpenCVExportImpl thumbnailImpl;
    int movie_duration = 0;
    int codec = VideoWriter::fourcc('M', 'P', '4', 'V'); 
    Size S = Size((int)width,    // Acquire input size
        (int)height);

    

    // Open the output
    thumbnailImpl.outputVideo.open(outfile.ToStdString(), codec, fps, S, true);

    if (thumbnailImpl.outputVideo.isOpened())
    {
        movie_duration = thumbnailImpl.ExecuteProcess(outfile, listOfFile, delay, fps, width, height, effect);

        thumbnailImpl.outputVideo.release();

        if (thumbnailImpl.endProcess)
            movie_duration = 0;
    }
    else
    {
#endif
	CThumbnailVideoExportImpl thumbnailImpl;
	thumbnailImpl.fps = fps;
	return thumbnailImpl.GenerateFFmpegVideoFromList(outfile, listOfFile, delay, fps, width, height, effect);
#ifdef EXPORT_DIAPORAMA_OPENCV
        movie_duration = thumbnailImpl.GenerateFFmpegVideoFromList(outfile, listOfFile, delay, fps, width, height, effect);
    }
    return movie_duration;
#endif
}
