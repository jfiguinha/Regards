#include "header.h"
#include "FFmpegDecodeFrame.h"
#include <RegardsBitmap.h>
#include <wx/progdlg.h>
#include <wx/filename.h>
#include <ConvertUtility.h>
#include <chrono>
#include <LibResource.h>
#include <RotateByShearRGB.h>


extern "C" {
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libavutil/display.h>
}


/**********************************************************************
 * hb_reduce
 **********************************************************************
 * Given a numerator (num) and a denominator (den), reduce them to an
 * equivalent fraction and store the result in x and y.
 *********************************************************************/
void CFFmpegDecodeFrame::hb_reduce(int* x, int* y, int num, int den)
{
	// find the greatest common divisor of num & den by Euclid's algorithm
	int n = num, d = den;
	while (d)
	{
		int t = d;
		d = n % d;
		n = t;
	}

	// at this point n is the gcd. if it's non-zero remove it from num
	// and den. Otherwise just return the original values.
	if (n)
	{
		*x = num / n;
		*y = den / n;
	}
	else
	{
		*x = num;
		*y = den;
	}
}

void CFFmpegDecodeFrame::hb_limit_rational(int* x, int* y, int num, int den, int limit)
{
	hb_reduce(&num, &den, num, den);
	if (num < limit && den < limit)
	{
		*x = num;
		*y = den;
		return;
	}

	if (num > den)
	{
		double div = static_cast<double>(limit) / num;
		num = limit;
		den *= div;
	}
	else
	{
		double div = static_cast<double>(limit) / den;
		den = limit;
		num *= div;
	}
	*x = num;
	*y = den;
}


void CFFmpegDecodeFrame::GetVideoInfos(int& width, int& height, int& rotation)
{
	width = widthVideo;
	height = heightVideo;
	rotation = this->rotation;
}

int CFFmpegDecodeFrame::GetRotation()
{
	return rotation;
}

void CFFmpegDecodeFrame::OpenFile(wxMemoryOutputStream* dataOutput, const wxString& filename)
{
	isBuffer = true;
	size_t buffer_size = dataOutput->GetSize();
	//size_t buffer_size = dataOutput->CopyTo(nullptr, 0);
	//wxStreamBuffer* theBuffer = dataOutput->GetOutputStreamBuffer();
	//theBuffer->
	bd.size = buffer_size;
	bd.data = static_cast<uint8_t*>(malloc(bd.size + 1));
	dataOutput->CopyTo(bd.data, bd.size);
	bd.ptr = bd.data;

	int ret = open_input_file(filename);
	if (ret < 0)
	{
		char message[255];
		av_make_error_string(message, AV_ERROR_MAX_STRING_SIZE, ret);
		wxString infos = CLibResource::LoadStringFromResource("LBLINFORMATIONS", 1);
		wxMessageBox(message, infos, wxICON_ERROR);

		isOk = false;
		cleanPacket = false;
	}
	else
	{
		isOk = true;
		m_allowSeek = (filename != "-") && (filename.find("rtsp://") != 0) && (filename.find("udp://") != 0);
		cleanPacket = true;
	}
}

void CFFmpegDecodeFrame::OpenFile(const wxString& filename)
{
	int ret = open_input_file(filename);
	if (ret < 0)
	{
		isOk = false;
		cleanPacket = false;
	}
	else
	{
		isOk = true;
		m_allowSeek = (filename != "-") && (filename.find("rtsp://") != 0) && (filename.find("udp://") != 0);
		cleanPacket = true;
	}
}

CFFmpegDecodeFrame::CFFmpegDecodeFrame(): stream_ctx(nullptr)
{
	isOk = true;
	dst = av_frame_alloc();
	scaleContext = sws_alloc_context();
	packet.data = nullptr;
	packet.size = 0;
	duration_movie = 0;
	widthVideo = 0;
	heightVideo = 0;
	rotation = 0;
	first = true;
	image = new CRegardsBitmap();
}


CFFmpegDecodeFrame::~CFFmpegDecodeFrame()
{
	EndTreatment();

	if (copyFrameBuffer != nullptr)
	{
		av_freep(&copyFrameBuffer->data[0]);
		av_frame_free(&copyFrameBuffer);
	}

	copyFrameBuffer = nullptr;

	if (dst != nullptr)
	{
		av_freep(&dst->data[0]);
		av_frame_free(&dst);
	}

	if (scaleContext != nullptr)
		sws_freeContext(scaleContext);

	if (image != nullptr)
		delete image;
}

void CFFmpegDecodeFrame::EndTreatment()
{
	if (cleanPacket)
	{
		if (packet.data != nullptr)
			av_packet_unref(&packet);
		Release();
		if (bitmapShow != nullptr)
		{
			bitmapShow->join();
			delete bitmapShow;
		}
		cleanPacket = false;
	}
}




static enum AVPixelFormat hw_pix_fmt;

bool CFFmpegDecodeFrame::IsOk()
{
	return isOk;
}



int CFFmpegDecodeFrame::read_packet(void* opaque, uint8_t* buf, int buf_size)
{
	auto bd = static_cast<buffer_data*>(opaque);
	buf_size = FFMIN(buf_size, bd->size);

	if (!buf_size)
		return AVERROR_EOF;
	printf("ptr:%p size:%zu\n", bd->ptr, bd->size);

	// copy internal buffer data to buf 
	memcpy(buf, bd->ptr, buf_size);
	bd->ptr += buf_size;
	bd->size -= buf_size;

	return buf_size;
}

int CFFmpegDecodeFrame::open_input_file(const wxString& filename)
{
	int ret;
	//unsigned int i;

	if (isBuffer)
	{
		if (!((ifmt_ctx = avformat_alloc_context())))
		{
			ret = AVERROR(ENOMEM);
			return ret;
		}

		avio_ctx_buffer = static_cast<uint8_t*>(av_malloc(avio_ctx_buffer_size));
		if (!avio_ctx_buffer)
		{
			ret = AVERROR(ENOMEM);
			return ret;
		}
		avio_ctx = avio_alloc_context(avio_ctx_buffer, avio_ctx_buffer_size,
		                              0, &bd, &read_packet, nullptr, nullptr);
		if (!avio_ctx)
		{
			ret = AVERROR(ENOMEM);
			return ret;
		}
		ifmt_ctx->pb = avio_ctx;

		ret = avformat_open_input(&ifmt_ctx, nullptr, nullptr, nullptr);
		if (ret < 0)
		{
			fprintf(stderr, "Could not open input\n");
			return ret;
		}
	}
	else
	{
		ifmt_ctx = nullptr;
		if ((ret = avformat_open_input(&ifmt_ctx, CConvertUtility::ConvertToUTF8(filename), nullptr, nullptr)) < 0)
		{
			av_log(nullptr, AV_LOG_ERROR, "Cannot open input file\n");
			return ret;
		}
	}

	if ((ret = avformat_find_stream_info(ifmt_ctx, nullptr)) < 0)
	{
		av_log(nullptr, AV_LOG_ERROR, "Cannot find stream information\n");
		return ret;
	}

	stream_ctx = static_cast<StreamContext*>(av_mallocz_array(ifmt_ctx->nb_streams, sizeof(*stream_ctx)));
	if (!stream_ctx)
		return AVERROR(ENOMEM);

	videoStreamIndex = av_find_best_stream(ifmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
	if (videoStreamIndex < 0)
	{
		av_log(nullptr, AV_LOG_ERROR, "Failed to copy decoder parameters to input decoder context "
			"for stream #%u\n", videoStreamIndex);
		return videoStreamIndex;
	}

	//for (int i = 0; i < ifmt_ctx->nb_streams; i++) {
	AVStream* stream = ifmt_ctx->streams[videoStreamIndex];
	//    if(stream->codecpar->codec_id == AV_CODEC_ID_NONE)
	//	   continue;
	AVCodec* dec = avcodec_find_decoder(stream->codecpar->codec_id);
	
	if (!dec)
	{
		av_log(nullptr, AV_LOG_ERROR, "Failed to find decoder for stream #%u\n", videoStreamIndex);
		return AVERROR_DECODER_NOT_FOUND;
	}
	codec_ctx = avcodec_alloc_context3(dec);
	if (!codec_ctx)
	{
		av_log(nullptr, AV_LOG_ERROR, "Failed to allocate the decoder context for stream #%u\n", videoStreamIndex);
		return AVERROR(ENOMEM);
	}
	ret = avcodec_parameters_to_context(codec_ctx, stream->codecpar);
	if (ret < 0)
	{
		av_log(nullptr, AV_LOG_ERROR, "Failed to copy decoder parameters to input decoder context "
		       "for stream #%u\n", videoStreamIndex);
		return ret;
	}
	/* Reencode video & audio and remux subtitles etc. */
	if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO
		|| codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO)
	{
		if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
			codec_ctx->framerate = av_guess_frame_rate(ifmt_ctx, stream, nullptr);

		ret = avcodec_open2(codec_ctx, dec, nullptr);
		if (ret < 0)
		{
			av_log(nullptr, AV_LOG_ERROR, "Failed to open decoder for stream #%u\n", videoStreamIndex);
			return ret;
		}

		if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			widthVideo = codec_ctx->width;
			heightVideo = codec_ctx->height;
			startTime = ifmt_ctx->start_time;
			duration_movie = ifmt_ctx->duration / 1000000;
			auto matrix = reinterpret_cast<int32_t*>(
				av_stream_get_side_data(stream, AV_PKT_DATA_DISPLAYMATRIX, nullptr));
			if (matrix)
				rotation = lround(av_display_rotation_get(matrix));
			else
				rotation = 0;

			//duration_movie = static_cast<double>(stream->duration) * static_cast<double>(stream->time_base.num) /
			//	static_cast<double>(stream->time_base.den);
		}
	}
	stream_ctx[videoStreamIndex].dec_ctx = codec_ctx;

	stream_ctx[videoStreamIndex].dec_frame = av_frame_alloc();
	if (!stream_ctx[videoStreamIndex].dec_frame)
		return AVERROR(ENOMEM);
	//}

	av_dump_format(ifmt_ctx, 0, CConvertUtility::ConvertToUTF8(filename), 0);
	return 0;
}

double CFFmpegDecodeFrame::GetTotalTime()
{
	return duration_movie;
}

void CFFmpegDecodeFrame::FrameToBitmap(AVFrame* sw_frame, const int & widthThumbnail, const int& heightThumbnail)
{
	videoFrameOutputWidth = sw_frame->width;
	videoFrameOutputHeight = sw_frame->height;
	float ratio = static_cast<float>(videoFrameOutputWidth) / static_cast<float>(videoFrameOutputHeight);
	if (widthThumbnail != 0 && heightThumbnail != 0)
	{
		videoFrameOutputWidth = widthThumbnail;
		videoFrameOutputHeight = heightThumbnail;

		//Calcul Ratio
		if (widthThumbnail >= heightThumbnail)
			videoFrameOutputHeight = static_cast<int>(static_cast<float>(widthThumbnail) / ratio);
		else
			videoFrameOutputWidth = static_cast<int>(static_cast<float>(videoFrameOutputHeight) * ratio);
	}

	if (first)
	{
		av_opt_set_int(scaleContext, "srcw", sw_frame->width, 0);
		av_opt_set_int(scaleContext, "srch", sw_frame->height, 0);
		av_opt_set_int(scaleContext, "src_format", sw_frame->format, 0);
		av_opt_set_int(scaleContext, "dstw", videoFrameOutputWidth, 0);
		av_opt_set_int(scaleContext, "dsth", videoFrameOutputHeight, 0);
		av_opt_set_int(scaleContext, "dst_format", AV_PIX_FMT_BGRA, 0);
		av_opt_set_int(scaleContext, "sws_flags", SWS_FAST_BILINEAR, 0);

		if (sws_init_context(scaleContext, nullptr, nullptr) < 0)
		{
			sws_freeContext(scaleContext);
			throw std::logic_error("Failed to initialise scale context");
		}

		first = false;
	}

	int numBytes = av_image_get_buffer_size(AV_PIX_FMT_BGRA, videoFrameOutputWidth, videoFrameOutputHeight, 1);
	if (numBytes != image->GetBitmapSize())
	{
		image->SetBitmap(videoFrameOutputWidth, videoFrameOutputHeight);
	}

	uint8_t* convertedFrameBuffer = image->GetPtBitmap();
	int linesize = videoFrameOutputWidth * 4;

	sws_scale(scaleContext, sw_frame->data, sw_frame->linesize, 0, sw_frame->height,
		&convertedFrameBuffer, &linesize);
}

int CFFmpegDecodeFrame::CalculVideoSecondStabilization(COpenCVStabilization* opencvStabilization, const int& nbFrame,
                                                       const bool& isBuffer)
{
	if (!isOk)
		return -1;

	int ret = 0;
	int numFrame = 0;
	bool toTheEnd = false;

	if (nbFrame == -1)
		toTheEnd = true;

	while (TRUE)
	{
		if (!toTheEnd)
		{
			if (!(numFrame < nbFrame))
				break;
		}

		if ((ret = av_read_frame(ifmt_ctx, &packet)) < 0)
			return ret;

		//AVStream *st = ifmt_ctx->streams[packet.stream_index];
		if (packet.stream_index != videoStreamIndex)
		{
			av_packet_unref(&packet);
			continue;
		}

		StreamContext* stream = &stream_ctx[videoStreamIndex];

		if (stream->dec_ctx == nullptr)
		{
			continue;
		}

		av_log(nullptr, AV_LOG_DEBUG, "Going to reencode&filter the frame\n");

		ret = avcodec_send_packet(stream->dec_ctx, &packet);
		if (ret < 0)
		{
			av_log(nullptr, AV_LOG_ERROR, "Decoding failed\n");
			return ret;
		}

		if (packet.buf == nullptr)
		{
			av_packet_unref(&packet);
			continue;
		}

		while (ret >= 0)
		{
			ret = avcodec_receive_frame(stream->dec_ctx, stream->dec_frame);
			if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN))
				break;
			if (ret < 0)
				return ret;

			ret = avcodec_receive_frame(stream->dec_ctx, stream->dec_frame);
			if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN))
				break;
			if (ret < 0)
				return ret;

			AVFrame* sw_frame = stream->dec_frame;
			FrameToBitmap(sw_frame, 0, 0);

			if (isBuffer)
				opencvStabilization->BufferFrame(image);
			else
				opencvStabilization->AddFrame(image);

			numFrame++;

			av_frame_unref(sw_frame);
		}

		av_packet_unref(&packet);
	}

	return ret;
}

int CFFmpegDecodeFrame::GetFrameBitmapPosition(const long& timeInSeconds, const int& widthThumbnail,
                                               const int& heightThumbnail)
{
	if (!isOk)
		return -1;

	int ret = -22;
	bool pictureFind = false;

	bool deleteMemory = false;

	int try_seek = 0;

	if (!m_allowSeek)
	{
		ret = -22;
		return ret;
	}

	if (timeInSeconds > 0)
	{
		//ret = SetVideoPosition(timeInSeconds);
		ret = av_seek_frame(ifmt_ctx, -1, timeInSeconds * AV_TIME_BASE + startTime, 0);
	}
	
	if(ret >= 0)
		avcodec_flush_buffers(codec_ctx);


	while (!pictureFind)
	{

		if ((ret = av_read_frame(ifmt_ctx, &packet)) < 0)
			return ret;

		//	AVStream *st = ifmt_ctx->streams[packet.stream_index];
		if (packet.stream_index != videoStreamIndex)
		{
			av_packet_unref(&packet);
			continue;
		}

		//checkPosition(ifmt_ctx);

		StreamContext* stream = &stream_ctx[videoStreamIndex];

		if (stream->dec_ctx == nullptr)
		{
			continue;
		}

		av_log(nullptr, AV_LOG_DEBUG, "Going to reencode&filter the frame\n");

		ret = avcodec_send_packet(stream->dec_ctx, &packet);
		if (ret < 0)
		{
			av_log(nullptr, AV_LOG_ERROR, "Decoding failed\n");
			return ret;
		}

		if (packet.buf == nullptr)
		{
			av_packet_unref(&packet);
			continue;
		}

		while (ret >= 0)
		{
			AVFrame* sw_frame = nullptr;

			ret = avcodec_receive_frame(stream->dec_ctx, stream->dec_frame);
			if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN))
				break;
			if (ret < 0)
				return ret;
			sw_frame = stream->dec_frame;

			int64_t videoPosition = sw_frame->pts * 1000;
			videoPosition = videoPosition - startTime;
			videoPosition = videoPosition / AV_TIME_BASE;

			if (videoPosition >= timeInSeconds || try_seek > 2)
			{
				FrameToBitmap(sw_frame, widthThumbnail, heightThumbnail);

				pictureFind = true;
			}
			else
			{
				ret = av_seek_frame(ifmt_ctx, -1, timeInSeconds * AV_TIME_BASE + startTime, 0);

				if (ret >= 0)
					avcodec_flush_buffers(codec_ctx);

				try_seek++;
			}


			if (deleteMemory)
			{
				av_freep(&sw_frame->data[0]);
				av_frame_free(&sw_frame);
			}
			else
				av_frame_unref(sw_frame);
		}

		av_packet_unref(&packet);
	}


	if (rotation != 0)
	{
		auto m_OriginalBitmapBits = (CRgbaquad*)image->GetPtBitmap();
		auto m_size = wxSize(videoFrameOutputWidth, videoFrameOutputHeight);
		wxSize m_sizedst;
		CRgbaquad clrBack;
		CRotateByShearRGB Rot(CRotateByShearRGB::Progress);
		CRgbaquad* m_ScaledBitmapBits = Rot.AllocAndRotate(
			m_OriginalBitmapBits,
			m_size,
			rotation,
			&m_sizedst,
			clrBack
		);
		image->SetBitmap((uint8_t*)m_ScaledBitmapBits, m_sizedst.GetWidth(), m_sizedst.GetHeight());
		delete[] m_ScaledBitmapBits;
	}
	if (pictureFind)
		ret = 0;
	return ret;
}

CRegardsBitmap* CFFmpegDecodeFrame::GetBitmap(const bool& copy)
{
	if (!isOk)
		return nullptr;

	if (copy && image != nullptr)
	{
		auto copyPicture = new CRegardsBitmap();
		*copyPicture = *image;
		return copyPicture;
	}
	return image;
}

void CFFmpegDecodeFrame::Release()
{
	if (NULL != codec_ctx)
		avcodec_close(codec_ctx);

	if (ifmt_ctx != nullptr)
	{
		for (int i = 0; i < ifmt_ctx->nb_streams; i++)
		{
			if (stream_ctx[i].dec_ctx != nullptr)
				avcodec_free_context(&stream_ctx[i].dec_ctx);
			if (stream_ctx[i].dec_frame != nullptr)
			{
				av_freep(&stream_ctx[i].dec_frame->data[0]);
				av_frame_free(&stream_ctx[i].dec_frame);
			}
		}
		av_free(stream_ctx);
		avformat_close_input(&ifmt_ctx);
		stream_ctx = nullptr;
		ifmt_ctx = nullptr;
	}

	if (isBuffer && bd.data != nullptr)
	{
		/* note: the internal buffer could have changed, and be != avio_ctx_buffer */
		if (avio_ctx)
			av_freep(&avio_ctx->buffer);
		avio_context_free(&avio_ctx);

		free(bd.data);
		bd.data = nullptr;
		bd.ptr = nullptr;
	}
}
