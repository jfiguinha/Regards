#include "header.h"
#include "FFmpegDecodeFrameFilter.h"
#include <RegardsBitmap.h>
#include <ImageLoadingFormat.h>
#include <wx/progdlg.h>
#include <wx/filename.h>
#include <ConvertUtility.h>
#include <window_id.h>
#include <chrono>
#include <RotateByShearRGB.h>
#include <OpenCLFilter.h>
#include <FiltreEffet.h>
static const int dst_width = 1920;
static const int dst_height = 1080;
static const int dst_vbit_rate = 1500000;
static const int dst_abit_rate = 128000;
static const int64_t dst_ch_layout = AV_CH_LAYOUT_STEREO;
static const int dst_sample_rate = 44100;
//static const AVCodecID VIDEO_CODEC = AV_CODEC_ID_H265;
//static const AVCodecID AUDIO_CODEC = AV_CODEC_ID_AAC;

static const char * hb_h264_profile_names[] = { "auto", "high", "main", "baseline", NULL, };
static const char * hb_h264_level_names[] = { "auto", "1.0", "1b", "1.1", "1.2", "1.3", "2.0", "2.1", "2.2", "3.0", "3.1", "3.2", "4.0", "4.1", "4.2", "5.0", "5.1", "5.2",  NULL, };
static const int    hb_h264_level_values[] = { -1,    10,    9,    11,    12,    13,    20,    21,    22,    30,    31,    32,    40,    41,    42,    50,    51,    52,     0, };
static const int          hb_h265_level_values[] = {
	-1,  30,  60,  63,  90,  93, 120, 123,
	150, 153, 156, 180, 183, 186,   0, };
static const char * hb_h265_level_names[] = {
	"auto", "1.0", "2.0", "2.1", "3.0", "3.1", "4.0", "4.1",
	"5.0", "5.1", "5.2", "6.0", "6.1", "6.2",  NULL, };


void CFFmpegDecodeFrameFilter::ApplyFilter(CVideoEffectParameter * videoEffectParameter, AVFrame * sw_frame)
{
	AVFrame * out = nullptr;
	bool finalConvert = true;
	CRegardsBitmap * bitmapData = nullptr;
	int videoFrameOutputWidth = sw_frame->width;
	int videoFrameOutputHeight = sw_frame->height;
	uint8_t * convertedFrameBuffer_data = nullptr;
	if (bitmap == nullptr)
		bitmap = new CRegardsBitmap(videoFrameOutputWidth, videoFrameOutputHeight);

	if (openCLEngine != nullptr)
	{
		if (hardwareDecode)
		{
			int ysize = 0;
			int uvsize = 0;

			ysize = sw_frame->linesize[0] * sw_frame->height;
			uvsize = sw_frame->linesize[1] * (sw_frame->height / 2);
			openclEffectYUV->SetMemoryDataNV12(sw_frame->data[0], ysize, sw_frame->data[1], uvsize, sw_frame->width, sw_frame->height, sw_frame->linesize[0]);

		}
		else
		{
			int ysize = 0;
			int usize = 0;
			int vsize = 0;

			ysize = sw_frame->linesize[0] * sw_frame->height;
			usize = sw_frame->linesize[1] * (sw_frame->height / 2);
			vsize = sw_frame->linesize[2] * (sw_frame->height / 2);
			openclEffectYUV->SetMemoryData(sw_frame->data[0], ysize, sw_frame->data[1], usize, sw_frame->data[2], vsize, sw_frame->width, sw_frame->height, sw_frame->linesize[0]);

		}

		openclEffectYUV->TranscodePicture(videoFrameOutputWidth, videoFrameOutputHeight);

	}
	else
	{
		CRegardsBitmap * bitmapData = new CRegardsBitmap(videoFrameOutputWidth, videoFrameOutputHeight);
		
		if (localContext == nullptr)
		{
			localContext = sws_alloc_context();

			av_opt_set_int(localContext, "srcw", sw_frame->width, 0);
			av_opt_set_int(localContext, "srch", sw_frame->height, 0);
			av_opt_set_int(localContext, "src_format", sw_frame->format, 0);
			av_opt_set_int(localContext, "dstw", videoFrameOutputWidth, 0);
			av_opt_set_int(localContext, "dsth", videoFrameOutputHeight, 0);
			av_opt_set_int(localContext, "dst_format", AV_PIX_FMT_BGRA, 0);
			av_opt_set_int(localContext, "sws_flags", SWS_FAST_BILINEAR, 0);

			if (sws_init_context(localContext, nullptr, nullptr) < 0)
			{
				sws_freeContext(localContext);
				throw std::logic_error("Failed to initialise scale context");
				return;
			}
		}

		int numBytes = avpicture_get_size(AV_PIX_FMT_BGRA, videoFrameOutputWidth, videoFrameOutputHeight);
		if (numBytes != bitmapData->GetBitmapSize())
		{
			bitmapData->SetBitmap(videoFrameOutputWidth, videoFrameOutputHeight);
		}

		uint8_t * convertedFrameBuffer = bitmapData->GetPtBitmap();
		int linesize = videoFrameOutputWidth * 4;

		sws_scale(localContext, sw_frame->data, sw_frame->linesize, 0, sw_frame->height,
			&convertedFrameBuffer, &linesize);
	}

	if (videoEffectParameter != nullptr)
	{
		if (videoEffectParameter->effectEnable)
		{
			COpenCLParameterClMem * memOutput = new COpenCLParameterClMem(true);
			COpenCLFilter openclFilter(openclContext);
			COpenCLParameterClMem * data_mem = openclEffectYUV->GetPtData();
			//data_mem->SetNoDelete(true);
			if (openCLEngine != nullptr)
			{
				cl_mem output = openclFilter.Flip("FlipVertical", data_mem->GetValue(), videoFrameOutputWidth, videoFrameOutputHeight);
				memOutput->SetValue(output);

				CRgbaquad color;
				CFiltreEffet filtre(color, openclContext, memOutput, videoFrameOutputWidth, videoFrameOutputHeight);

				if (videoEffectParameter != nullptr)
				{
					if (videoEffectParameter->ColorBoostEnable)
					{
						filtre.RGBFilter(videoEffectParameter->color_boost[0], videoEffectParameter->color_boost[1], videoEffectParameter->color_boost[2]);
					}
					if (videoEffectParameter->bandcEnable)
					{
						filtre.BrightnessAndContrast(videoEffectParameter->brightness, videoEffectParameter->contrast);
					}
					if (videoEffectParameter->SharpenEnable)
					{
						filtre.SharpenMasking(videoEffectParameter->sharpness);
					}
					if (videoEffectParameter->denoiseEnable)
					{
						filtre.HQDn3D(videoEffectParameter->denoisingLevel, 4, 3, 3);
					}
					if (videoEffectParameter->sepiaEnable)
					{
						filtre.Sepia();
					}
					if (videoEffectParameter->grayEnable)
					{
						filtre.NiveauDeGris();
					}
					if (videoEffectParameter->filmgrainenable)
					{
						filtre.Noise();
					}
				}
				finalConvert = false;
				filtre.GetBitmap(bitmap, true);
			}
			else
			{
				CRgbaquad color;
				CImageLoadingFormat imageFormat;
				imageFormat.SetPicture(bitmapData);
				CFiltreEffet filtre(color, openclContext, &imageFormat);

				if (videoEffectParameter != nullptr)
				{
					if (videoEffectParameter->ColorBoostEnable)
					{
						filtre.RGBFilter(videoEffectParameter->color_boost[0], videoEffectParameter->color_boost[1], videoEffectParameter->color_boost[2]);
					}
					if (videoEffectParameter->bandcEnable)
					{
						filtre.BrightnessAndContrast(videoEffectParameter->brightness, videoEffectParameter->contrast);
					}
					if (videoEffectParameter->SharpenEnable)
					{
						filtre.SharpenMasking(videoEffectParameter->sharpness);
					}
					if (videoEffectParameter->denoiseEnable)
					{
						filtre.HQDn3D(videoEffectParameter->denoisingLevel, 4, 3, 3);
					}
					if (videoEffectParameter->sepiaEnable)
					{
						filtre.Sepia();
					}
					if (videoEffectParameter->grayEnable)
					{
						filtre.NiveauDeGris();
					}
					if (videoEffectParameter->filmgrainenable)
					{
						filtre.Noise();
					}
					if (videoEffectParameter->grayEnable)
					{
						filtre.NiveauDeGris();
					}
				}
				filtre.GetBitmap(bitmap, true);
				finalConvert = false;
			}


		}
	}

	if (finalConvert)
	{
		if (openCLEngine != nullptr)
		{
			openclEffectYUV->FlipVertical();
			if (bitmap != nullptr)
				delete bitmap;
			bitmap = openclEffectYUV->GetRgbaBitmap(true);
		}
	}
}


/**********************************************************************
 * hb_reduce
 **********************************************************************
 * Given a numerator (num) and a denominator (den), reduce them to an
 * equivalent fraction and store the result in x and y.
 *********************************************************************/
void CFFmpegDecodeFrameFilter::hb_reduce(int *x, int *y, int num, int den)
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

void CFFmpegDecodeFrameFilter::hb_limit_rational(int *x, int *y, int num, int den, int limit)
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
		double div = (double)limit / num;
		num = limit;
		den *= div;
	}
	else
	{
		double div = (double)limit / den;
		den = limit;
		num *= div;
	}
	*x = num;
	*y = den;
}


void CFFmpegDecodeFrameFilter::GetVideoInfos(int & width, int & height, int &rotation)
{
	width = widthVideo;
	height = heightVideo;
	rotation = this->rotation;
}

int CFFmpegDecodeFrameFilter::GetRotation()
{
	return rotation;
}

CFFmpegDecodeFrameFilter::CFFmpegDecodeFrameFilter(COpenCLEngine * openCLEngine, const wxString & fileName, const wxString &acceleratorHardware)
{
	dst = av_frame_alloc();
	scaleContext = sws_alloc_context();
	packet.data = NULL;
	packet.size = 0;
	duration_movie = 0;
	widthVideo = 0;
	heightVideo = 0;
	rotation = 0;
	first = true;
	this->acceleratorHardware = acceleratorHardware;
	int ret;
	if ((ret = open_input_file(fileName)) < 0)
		isOk = false;
	if (isOk)
	{
		m_allowSeek = (fileName != "-") && (fileName.find("rtsp://") != 0) && (fileName.find("udp://") != 0);
		dst = av_frame_alloc();
		scaleContext = sws_alloc_context();
		cleanPacket = true;
	}
	else
		cleanPacket = false;

	int supportOpenCL = 0;
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		supportOpenCL = config->GetIsOpenCLSupport();

	if (supportOpenCL)
	{
		this->openCLEngine = openCLEngine;
		if (openCLEngine != nullptr)
		{
			openclContext = openCLEngine->GetInstance();

			if (openclContext != nullptr)
				openclEffectYUV = new COpenCLEffectVideoYUV(openclContext);

		}
	}
};


CFFmpegDecodeFrameFilter::~CFFmpegDecodeFrameFilter()
{
	if (cleanPacket)
	{
		if (packet.data != NULL)
			av_packet_unref(&packet);
		Release();
		if (bitmapShow != nullptr)
		{
			bitmapShow->join();
			delete bitmapShow;
		}

	}

	if (copyFrameBuffer != nullptr)
		av_frame_free(&copyFrameBuffer);
	copyFrameBuffer = nullptr;

	if (dst != nullptr)
		av_frame_free(&dst);

	if (scaleContext != nullptr)
		sws_freeContext(scaleContext);

	if (bitmap != nullptr)
		delete bitmap;

	if (openclEffectYUV != nullptr)
		delete openclEffectYUV;
};


static enum AVPixelFormat hw_pix_fmt;

bool CFFmpegDecodeFrameFilter::IsOk()
{
	return isOk;
}


int CFFmpegDecodeFrameFilter::hw_decoder_init(AVCodecContext *ctx, const enum AVHWDeviceType type)
{
	int err = 0;

	if ((err = av_hwdevice_ctx_create(&hw_device_ctx, type,
		NULL, NULL, 0)) < 0) {
		fprintf(stderr, "Failed to create specified HW device.\n");
		return err;
	}
	ctx->hw_device_ctx = av_buffer_ref(hw_device_ctx);

	return err;
}

enum AVPixelFormat CFFmpegDecodeFrameFilter::get_hw_format(AVCodecContext *ctx,
	const enum AVPixelFormat *pix_fmts)
{
	const enum AVPixelFormat *p;

	for (p = pix_fmts; *p != -1; p++) {
		if (*p == hw_pix_fmt)
			return *p;
	}

	fprintf(stderr, "Failed to get HW surface format.\n");
	return AV_PIX_FMT_NONE;
}

int CFFmpegDecodeFrameFilter::open_input_file(const wxString & filename)
{
	int ret;
	unsigned int i;

	ifmt_ctx = NULL;
	if ((ret = avformat_open_input(&ifmt_ctx, CConvertUtility::ConvertToUTF8(filename), NULL, NULL)) < 0) {
		av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
		return ret;
	}

	if ((ret = avformat_find_stream_info(ifmt_ctx, NULL)) < 0) {
		av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
		return ret;
	}

	stream_ctx = (StreamContext *)av_mallocz_array(ifmt_ctx->nb_streams, sizeof(*stream_ctx));
	if (!stream_ctx)
		return AVERROR(ENOMEM);

	for (i = 0; i < ifmt_ctx->nb_streams; i++) {
		AVStream *stream = ifmt_ctx->streams[i];
        if(stream->codecpar->codec_id == AV_CODEC_ID_NONE)
            continue;
		AVCodec *dec = avcodec_find_decoder(stream->codecpar->codec_id);
		AVCodecContext *codec_ctx;
		if (!dec) {
			av_log(NULL, AV_LOG_ERROR, "Failed to find decoder for stream #%u\n", i);
			return AVERROR_DECODER_NOT_FOUND;
		}
		codec_ctx = avcodec_alloc_context3(dec);
		if (!codec_ctx) {
			av_log(NULL, AV_LOG_ERROR, "Failed to allocate the decoder context for stream #%u\n", i);
			return AVERROR(ENOMEM);
		}
		ret = avcodec_parameters_to_context(codec_ctx, stream->codecpar);
		if (ret < 0) {
			av_log(NULL, AV_LOG_ERROR, "Failed to copy decoder parameters to input decoder context "
				"for stream #%u\n", i);
			return ret;
		}
		/* Reencode video & audio and remux subtitles etc. */
		if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO
			|| codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
			if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
				codec_ctx->framerate = av_guess_frame_rate(ifmt_ctx, stream, NULL);

			if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
			{			/* Open decoder */
				videoStreamIndex = i;
				if (acceleratorHardware != "")
				{
					enum AVHWDeviceType type;

					type = av_hwdevice_find_type_by_name(acceleratorHardware);
					if (type == AV_HWDEVICE_TYPE_NONE) {
						fprintf(stderr, "Device type %s is not supported.\n", "dxva2");
						fprintf(stderr, "Available device types:");
						while ((type = av_hwdevice_iterate_types(type)) != AV_HWDEVICE_TYPE_NONE)
							fprintf(stderr, " %s", av_hwdevice_get_type_name(type));
						fprintf(stderr, "\n");
						return -1;
					}

					for (i = 0;; i++) {
						const AVCodecHWConfig *config = avcodec_get_hw_config(dec, i);
						if (!config) {
							fprintf(stderr, "Decoder %s does not support device type %s.\n",
								dec->name, av_hwdevice_get_type_name(type));
							return -1;
						}
						if (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX &&
							config->device_type == type) {
							hw_pix_fmt = config->pix_fmt;
							break;
						}
					}


					codec_ctx->get_format = get_hw_format;

					if (hw_decoder_init(codec_ctx, type) < 0)
						return -1;
				}

			}


			ret = avcodec_open2(codec_ctx, dec, NULL);
			if (ret < 0) {
				av_log(NULL, AV_LOG_ERROR, "Failed to open decoder for stream #%u\n", i);
				return ret;
			}

			if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
			{

				widthVideo = codec_ctx->width;
				heightVideo = codec_ctx->height;
				startTime = ifmt_ctx->start_time;

				int32_t* matrix = reinterpret_cast<int32_t*>(av_stream_get_side_data(stream, AV_PKT_DATA_DISPLAYMATRIX, nullptr));
				if (matrix)
					rotation = lround(av_display_rotation_get(matrix));
				else
					rotation = 0;

				duration_movie = double(stream->duration) * double(stream->time_base.num) / double(stream->time_base.den);
			}
		}
		stream_ctx[i].dec_ctx = codec_ctx;

		stream_ctx[i].dec_frame = av_frame_alloc();
		if (!stream_ctx[i].dec_frame)
			return AVERROR(ENOMEM);
	}

	av_dump_format(ifmt_ctx, 0, CConvertUtility::ConvertToUTF8(filename), 0);
	return 0;
}

double CFFmpegDecodeFrameFilter::GetTotalTime()
{
	return duration_movie;
}

void CFFmpegDecodeFrameFilter::GenerateBitmapFromFrame(AVFrame * sw_frame)
{
	int videoFrameOutputWidth = sw_frame->width;
	int videoFrameOutputHeight = sw_frame->height;

	if (bitmap == nullptr)
		bitmap = new CRegardsBitmap(videoFrameOutputWidth, videoFrameOutputHeight);

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
	}

	int numBytes = avpicture_get_size(AV_PIX_FMT_BGRA, videoFrameOutputWidth, videoFrameOutputHeight);
	if (numBytes != bitmap->GetBitmapSize())
	{
		bitmap->SetBitmap(videoFrameOutputWidth, videoFrameOutputHeight);
	}

	uint8_t * convertedFrameBuffer = bitmap->GetPtBitmap();
	int linesize = videoFrameOutputWidth * 4;

	sws_scale(scaleContext, sw_frame->data, sw_frame->linesize, 0, sw_frame->height,
		&convertedFrameBuffer, &linesize);

	//bitmap->VertFlipBuf();
}

int CFFmpegDecodeFrameFilter::GenerateFrameFromDecoder(CVideoEffectParameter * videoEffectParameter, bool & first, StreamContext *stream, const bool & decodeOpenCL)
{
	int ret = 0;

	AVFrame * sw_frame = av_frame_alloc();
	if (sw_frame == nullptr) {
		fprintf(stderr, "Can not alloc frame\n");
		ret = AVERROR(ENOMEM);
		av_frame_free(&sw_frame);
		return ret;
	}

	/* retrieve data from GPU to CPU */
	if ((ret = av_hwframe_transfer_data(sw_frame, stream->dec_frame, 0)) < 0) {
		fprintf(stderr, "Error transferring the data to system memory\n");
		av_frame_free(&sw_frame);
		return ret;
	}

	if (openCLEngine != nullptr && decodeOpenCL && videoEffectParameter->effectEnable)
	{
		hardwareDecode = true;
		ApplyFilter(videoEffectParameter, sw_frame);
		
	}
	else
	{
		GenerateBitmapFromFrame(sw_frame);
	}

	av_frame_free(&sw_frame);
	return ret;
}

int CFFmpegDecodeFrameFilter::GetFrameBitmapPosition(CVideoEffectParameter * videoEffectParameter, const long &timeInSeconds)
{
	if (!isOk)
		return -1;
	int videoFrameOutputWidth = 0;
	int videoFrameOutputHeight = 0;
	int ret = 0;
	int stream_index = 0;
	bool pictureFind = false;
	bool deleteMemory = false;

	if (!m_allowSeek)
	{
		ret = -22;
		return ret;
	}

	int64_t timestamp = timeInSeconds *1000 * 1000 + startTime;

	if (timestamp < 0)
	{
		timestamp = 0;
	}

	//int64_t timestamp = (AV_TIME_BASE / 100) * static_cast<int64_t>(videoCompressOption->startTime);
	int64_t seek_target = timestamp;
	int64_t seek_rel = 0;
	int64_t seek_min = seek_rel > 0 ? seek_target - seek_rel + 2 : INT64_MIN;
	int64_t seek_max = seek_rel < 0 ? seek_target - seek_rel - 2 : INT64_MAX;
	int seek_flags = 0;
	seek_flags &= ~AVSEEK_FLAG_BYTE;
	// FIXME the +-2 is due to rounding being not done in the correct direction in generation
	//      of the seek_pos/seek_rel variables

	ret = avformat_seek_file(ifmt_ctx, -1, seek_min, seek_target, seek_max, seek_flags);

	while (!pictureFind)
	{
		if ((ret = av_read_frame(ifmt_ctx, &packet)) < 0)
			return ret;

		if (packet.stream_index != videoStreamIndex)
		{
			av_free_packet(&packet);
			continue;
		}

		AVStream *st = ifmt_ctx->streams[packet.stream_index];
		if (st->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
			continue;

		if (st->codecpar->codec_id == AV_CODEC_ID_NONE)
			continue;

		CFFmpegDecodeFrameFilter::StreamContext *stream = &stream_ctx[stream_index];

		av_log(NULL, AV_LOG_DEBUG, "Going to reencode&filter the frame\n");

		av_packet_rescale_ts(&packet,
			ifmt_ctx->streams[stream_index]->time_base,
			stream->dec_ctx->time_base);


		ret = avcodec_send_packet(stream->dec_ctx, &packet);
		if (ret < 0) {
			av_log(NULL, AV_LOG_ERROR, "Decoding failed\n");
			return ret;
		}
		while (ret >= 0)
		{
			ret = avcodec_receive_frame(stream->dec_ctx, stream->dec_frame);
			if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN))
				break;
			else if (ret < 0)
				return ret;

			if (acceleratorHardware != "")
			{
				if (stream->dec_frame->format == hw_pix_fmt)
				{
					ret = GenerateFrameFromDecoder(videoEffectParameter, first, stream, true);
					pictureFind = true;
				}
						
			}
			else
			{
				if (!hardwareDecode && videoEffectParameter->effectEnable)
				{
					ApplyFilter(videoEffectParameter, stream->dec_frame);
					pictureFind = true;
				}
				else
				{
					GenerateBitmapFromFrame(stream->dec_frame);
					pictureFind = true;
				}
			}

		}

		av_packet_unref(&packet);
	}

	
	if (rotation != 0)
	{
		CRgbaquad * m_OriginalBitmapBits = (CRgbaquad *)bitmap->GetPtBitmap();
		wxSize m_size = wxSize(videoFrameOutputWidth, videoFrameOutputHeight);
		wxSize m_sizedst;
		CRgbaquad clrBack;
		CRotateByShearRGB Rot(CRotateByShearRGB::Progress);
		CRgbaquad * m_ScaledBitmapBits = Rot.AllocAndRotate(
			m_OriginalBitmapBits,
			m_size,
			rotation,
			&m_sizedst,
			clrBack
		);
		bitmap->SetBitmap((uint8_t *)m_ScaledBitmapBits, m_sizedst.GetWidth(), m_sizedst.GetHeight());
		delete[] m_ScaledBitmapBits;
	}
	if (pictureFind)
		ret = 0;
	return ret;
}

CRegardsBitmap * CFFmpegDecodeFrameFilter::GetBitmap()
{
	CRegardsBitmap * copyPicture = new CRegardsBitmap();
	*copyPicture = *bitmap;
	copyPicture->VertFlipBuf();
	return copyPicture;
}

void CFFmpegDecodeFrameFilter::Release()
{
	if (ifmt_ctx != nullptr)
	{
		for (int i = 0; i < ifmt_ctx->nb_streams; i++) {
			avcodec_free_context(&stream_ctx[i].dec_ctx);
			av_frame_free(&stream_ctx[i].dec_frame);
		}
		av_free(stream_ctx);
		avformat_close_input(&ifmt_ctx);
        stream_ctx = nullptr;
        ifmt_ctx = nullptr;
	}
}