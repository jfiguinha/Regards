#include "header.h"
#include "FFmpegTranscodingPimpl.h"
#include <RegardsBitmap.h>
#include <CompressVideo.h>
#include <ImageLoadingFormat.h>
#include "ffmpegToBitmap.h"
#include "VideoCompressOption.h"
#include <wx/progdlg.h>
#include <wx/filename.h>
#include <ConvertUtility.h>
#include <window_id.h>
#include <chrono>
#include "VideoCompressOption.h"


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


/**********************************************************************
 * hb_reduce
 **********************************************************************
 * Given a numerator (num) and a denominator (den), reduce them to an
 * equivalent fraction and store the result in x and y.
 *********************************************************************/
void hb_reduce(int *x, int *y, int num, int den)
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

void hb_limit_rational(int *x, int *y, int num, int den, int limit)
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


static int apply_vpx_preset(AVDictionary ** av_opts, const wxString & preset)
{
	if (preset == "")
	{
		// default "medium"
		av_dict_set(av_opts, "deadline", "good", 0);
		av_dict_set(av_opts, "cpu-used", "2", 0);
	}
	else if (preset == "veryfast")
	{
		av_dict_set(av_opts, "deadline", "good", 0);
		av_dict_set(av_opts, "cpu-used", "5", 0);
	}
	else if (preset == "faster")
	{
		av_dict_set(av_opts, "deadline", "good", 0);
		av_dict_set(av_opts, "cpu-used", "4", 0);
	}
	else if (preset == "fast")
	{
		av_dict_set(av_opts, "deadline", "good", 0);
		av_dict_set(av_opts, "cpu-used", "3", 0);
	}
	else if (preset == "medium")
	{
		av_dict_set(av_opts, "deadline", "good", 0);
		av_dict_set(av_opts, "cpu-used", "2", 0);
	}
	else if (preset == "slow")
	{
		av_dict_set(av_opts, "deadline", "good", 0);
		av_dict_set(av_opts, "cpu-used", "1", 0);
	}
	else if (preset == "slower")
	{
		av_dict_set(av_opts, "deadline", "good", 0);
		av_dict_set(av_opts, "cpu-used", "0", 0);
	}
	else if (preset == "veryslow")
	{
		av_dict_set(av_opts, "deadline", "best", 0);
		av_dict_set(av_opts, "cpu-used", "0", 0);
	}
	else
	{
		// default "medium"
		//hb_log("apply_vpx_preset: Unknown VPx encoder preset %s", preset);
		return -1;
	}

	return 0;
}

// VP8 and VP9 have some options in common and some different
static int apply_vp8_preset(AVDictionary ** av_opts, const wxString & preset)
{
	return apply_vpx_preset(av_opts, preset);
}

static int apply_vp9_preset(AVDictionary ** av_opts, const wxString & preset)
{
	av_dict_set(av_opts, "row-mt", "1", 0);
	return apply_vpx_preset(av_opts, preset);
}

static int apply_encoder_preset(AVCodecID vcodec, AVDictionary ** av_opts,
	const char * preset)
{
	switch (vcodec)
	{
	case AV_CODEC_ID_VP8:
		return apply_vp8_preset(av_opts, preset);
	case AV_CODEC_ID_VP9:
		return apply_vp9_preset(av_opts, preset);
		/*
		case AV_CODEC_ID_H264:
		case AV_CODEC_ID_H265:
			av_dict_set(av_opts, "preset", preset, 0);
			break;
		*/
	default:
		break;

	}

	return 0;
}



static enum AVPixelFormat hw_pix_fmt;

static wxString ConvertSecondToTime(int sec)
{

	int h = (sec / 3600);
	int m = (sec - (3600 * h)) / 60;
	int s = (sec - (3600 * h) - (m * 60));
	return wxString::Format("%02d:%02d:%02d\n", h, m, s);
}


void CFFmpegTranscodingPimpl::DisplayPreview(void * data)
{
	CFFmpegTranscodingPimpl * ffmpeg_trans = (CFFmpegTranscodingPimpl *)data;
	if (ffmpeg_trans != nullptr)
	{

		CffmpegToBitmap * ffmpegToBitmap = new CffmpegToBitmap(true);
		bool deleteData = false;

		ffmpeg_trans->muFrame.lock();

		int widthVideo = ffmpeg_trans->copyFrameBuffer->width;
		int heightVideo = ffmpeg_trans->copyFrameBuffer->height;
		ffmpegToBitmap->InitContext(ffmpeg_trans->copyFrameBuffer, 0, widthVideo, heightVideo);
		if (ffmpeg_trans->bitmapVideo == nullptr)
			ffmpeg_trans->bitmapVideo = ffmpegToBitmap->GetConvert(ffmpeg_trans->copyFrameBuffer, widthVideo, heightVideo);
		else
		{
			if (ffmpegToBitmap->GetConvert(ffmpeg_trans->bitmapVideo, ffmpeg_trans->copyFrameBuffer, widthVideo, heightVideo) == 0)
			{
				delete ffmpeg_trans->bitmapVideo;
				ffmpeg_trans->bitmapVideo = ffmpegToBitmap->GetConvert(ffmpeg_trans->copyFrameBuffer, widthVideo, heightVideo);
			}
		}
		if (ffmpeg_trans->copyFrameBuffer != nullptr)
			av_frame_free(&ffmpeg_trans->copyFrameBuffer);
		ffmpeg_trans->copyFrameBuffer = nullptr;
		ffmpeg_trans->muFrame.unlock();

		CImageLoadingFormat * imageLoadingFormat = new CImageLoadingFormat(false);
		imageLoadingFormat->SetPicture(ffmpeg_trans->bitmapVideo);
		ffmpeg_trans->m_dlgProgress->SetBitmap(imageLoadingFormat->GetwxImage());
		ffmpeg_trans->muWriteData.lock();

		double duration_total = ffmpeg_trans->duration_movie;

		int posVideo = (int)ffmpeg_trans->pos - ffmpeg_trans->videoCompressOption->startTime;

		if (ffmpeg_trans->videoCompressOption->endTime != 0 || ffmpeg_trans->videoCompressOption->startTime != 0)
			duration_total = ffmpeg_trans->videoCompressOption->endTime - ffmpeg_trans->videoCompressOption->startTime;

		ffmpeg_trans->m_dlgProgress->SetPos(duration_total, posVideo);
		ffmpeg_trans->m_dlgProgress->SetTextProgression(ffmpeg_trans->duration);



		double dif = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - ffmpeg_trans->begin).count();
		wxString frame = wxString::Format("%d fps", (int)(ffmpeg_trans->nbframe / dif));
		ffmpeg_trans->m_dlgProgress->SetTextProgression(frame, 2);

		double pourcentage = ((double)posVideo / (double)duration_total) * 100.0f;
		double timeMissing = (dif * 100.0f) / pourcentage;

		frame = ConvertSecondToTime((int)(timeMissing - dif));
		ffmpeg_trans->m_dlgProgress->SetTextProgression(frame, 3);

		frame = ConvertSecondToTime((int)dif);
		ffmpeg_trans->m_dlgProgress->SetTextProgression(frame, 1);

		ffmpeg_trans->muWriteData.unlock();
		delete imageLoadingFormat;

		ffmpegToBitmap->DeleteData();
		delete ffmpegToBitmap;
	}

	ffmpeg_trans->muEnding.lock();
	ffmpeg_trans->isend = true;
	ffmpeg_trans->muEnding.unlock();

}

int CFFmpegTranscodingPimpl::hw_decoder_init(AVCodecContext *ctx, const enum AVHWDeviceType type)
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

enum AVPixelFormat CFFmpegTranscodingPimpl::get_hw_format(AVCodecContext *ctx,
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

int CFFmpegTranscodingPimpl::open_input_file(const wxString & filename)
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


AVDictionary * CFFmpegTranscodingPimpl::setEncoderParam(const AVCodecID &codec_id, AVCodecContext * pCodecCtx, AVCodecContext * pSourceCodecCtx, const wxString &encoderName)
{
	int in_w = dst_width;
	int in_h = dst_height;

#ifdef HANDBRAKE
	pCodecCtx->codec_id = codec_id;
	pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
	pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
	pCodecCtx->width = in_w;
	pCodecCtx->height = in_h;
	pCodecCtx->time_base.num = 1;
	pCodecCtx->time_base.den = 30;
	pCodecCtx->bit_rate = dst_vbit_rate;
	pCodecCtx->gop_size = 30 * 1;
	pCodecCtx->max_b_frames = 0;
#else
	pCodecCtx->codec_id = codec_id;
	pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
	pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
	pCodecCtx->width = pSourceCodecCtx->width;
	pCodecCtx->height = pSourceCodecCtx->height;
	pCodecCtx->time_base = pSourceCodecCtx->time_base;
	//pCodecCtx->bit_rate = dst_vbit_rate;
	pCodecCtx->gop_size = (pSourceCodecCtx->framerate.num / pSourceCodecCtx->framerate.den) * 1;
	pCodecCtx->max_b_frames = 0;
	pCodecCtx->framerate = pSourceCodecCtx->framerate;
	pCodecCtx->sample_aspect_ratio = pSourceCodecCtx->sample_aspect_ratio;
#endif



	// some formats want stream headers to be separate
	if (pCodecCtx->flags & AVFMT_GLOBALHEADER)
		pCodecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

	AVDictionary *param = 0;

	if (pCodecCtx->codec_id == AV_CODEC_ID_H264 && videoCompressOption->videoPreset != "" && encoderName == "")
	{
		av_dict_set(&param, "start_time_realtime", 0, 0);
		av_opt_set(pCodecCtx->priv_data, "preset", videoCompressOption->videoPreset, 0);
		av_opt_set(pCodecCtx->priv_data, "tune", "zerolatency", 0);
	}

	/*
	if (pCodecCtx->codec_id == AV_CODEC_ID_H265 && encoderName == ""  && videoCompressOption->videoPreset != "")
	{
		//preset: ultrafast, superfast, veryfast, faster, fast,
			//medium, slow, slower, veryslow, placebo
		av_opt_set(pCodecCtx->priv_data, "preset", "fast", 0);
		//tune: psnr, ssim, zerolatency, fastdecode
		av_opt_set(pCodecCtx->priv_data, "tune", "zero-latency", 0);
		//profile: main, main10, mainstillpicture
		av_opt_set(pCodecCtx->priv_data, "profile", "main", 0);
	}
	*/

	if (videoCompressOption->videoQualityOrBitRate == 0)
	{
		/* Average bitrate */
		pCodecCtx->bit_rate = 1000 * videoCompressOption->videoBitRate;
		// ffmpeg's mpeg2 encoder requires that the bit_rate_tolerance be >=
		// bitrate * fps
		pCodecCtx->bit_rate_tolerance = videoCompressOption->videoBitRate * av_q2d(pCodecCtx->framerate) + 1;

		if ((codec_id == AV_CODEC_ID_H264 || codec_id == AV_CODEC_ID_H265) && encoderName == "nvenc")
			av_dict_set(&param, "rc", "vbr_hq", 0);

		if ((codec_id == AV_CODEC_ID_H264 || codec_id == AV_CODEC_ID_H265) && encoderName == "amf")
			av_dict_set(&param, "rc", "vbr_peak", 0);
	}
	else
	{
		if (codec_id == AV_CODEC_ID_VP8)
		{
			pCodecCtx->flags |= AV_CODEC_FLAG_QSCALE;
			pCodecCtx->global_quality = FF_QP2LAMBDA * videoCompressOption->videoCompressionValue + 0.5;
			pCodecCtx->bit_rate = (int64_t)pCodecCtx->width * pCodecCtx->height * pCodecCtx->framerate.num / pCodecCtx->framerate.den;
		}
		else if (codec_id == AV_CODEC_ID_VP9)
		{
			// These settings produce better image quality than
			// what was previously used
			pCodecCtx->flags |= AV_CODEC_FLAG_QSCALE;
			pCodecCtx->global_quality = FF_QP2LAMBDA * videoCompressOption->videoCompressionValue + 0.5;

			char quality[7];
			snprintf(quality, 7, "%.2f", videoCompressOption->videoCompressionValue);
			av_dict_set(&param, "crf", quality, 0);
			//This value was chosen to make the bitrate high enough
			//for libvpx to "turn off" the maximum bitrate feature
			//that is normally applied to constant quality.
			pCodecCtx->bit_rate = (int64_t)pCodecCtx->width * pCodecCtx->height * pCodecCtx->framerate.num / pCodecCtx->framerate.den;
			//hb_log("encavcodec: encoding at CQ %.2f", job->vquality);
		}
		//Set constant quality for nvenc
		else if ((codec_id == AV_CODEC_ID_H264 || codec_id == AV_CODEC_ID_H265) && encoderName == "nvenc")
		{
			char qualityI[7];
			char quality[7];
			char qualityB[7];

			double adjustedQualityI = videoCompressOption->videoCompressionValue - 2;
			double adjustedQualityB = videoCompressOption->videoCompressionValue + 2;
			if (adjustedQualityB > 51) {
				adjustedQualityB = 51;
			}

			if (adjustedQualityI < 0) {
				adjustedQualityI = 0;
			}

			snprintf(quality, 7, "%.2f", videoCompressOption->videoCompressionValue);
			snprintf(qualityI, 7, "%.2f", adjustedQualityI);
			snprintf(qualityB, 7, "%.2f", adjustedQualityB);

			pCodecCtx->bit_rate = 0;

			av_dict_set(&param, "rc", "vbr_hq", 0);
			av_dict_set(&param, "cq", quality, 0);

			// further Advanced Quality Settings in Constant Quality Mode
			av_dict_set(&param, "init_qpP", quality, 0);
			av_dict_set(&param, "init_qpB", qualityB, 0);
			av_dict_set(&param, "init_qpI", qualityI, 0);
			//hb_log("encavcodec: encoding at rc=vbr_hq %.2f", job->vquality);
		}
		else if ((codec_id == AV_CODEC_ID_H264 || codec_id == AV_CODEC_ID_H265) && encoderName == "amf")
		{
			char quality[7];
			char qualityB[7];
			double adjustedQualityB = videoCompressOption->videoCompressionValue + 2;

			snprintf(quality, 7, "%.2f", videoCompressOption->videoCompressionValue);
			snprintf(qualityB, 7, "%.2f", adjustedQualityB);

			if (adjustedQualityB > 51) {
				adjustedQualityB = 51;
			}

			av_dict_set(&param, "rc", "cqp", 0);

			av_dict_set(&param, "qp_i", quality, 0);
			av_dict_set(&param, "qp_p", quality, 0);

			if (codec_id != AV_CODEC_ID_H265)
			{
				av_dict_set(&param, "qp_b", qualityB, 0);
			}
			//hb_log("encavcodec: encoding at QP %.2f", job->vquality);
		}
		else if (codec_id == AV_CODEC_ID_H264 || codec_id == AV_CODEC_ID_H265)
		{
			pCodecCtx->flags |= AV_CODEC_FLAG_QSCALE;
			pCodecCtx->global_quality = videoCompressOption->videoCompressionValue;

			//hb_log("encavcodec: encoding at constant quality %d", context->global_quality);
		}
		else
		{
			// These settings produce better image quality than
			// what was previously used
			pCodecCtx->flags |= AV_CODEC_FLAG_QSCALE;
			pCodecCtx->global_quality = FF_QP2LAMBDA * videoCompressOption->videoCompressionValue + 0.5;

			//hb_log("encavcodec: encoding at constant quantizer %d",
			//	context->global_quality);
		}
	}



	/* take first format from list of supported formats */
	/*
	if (pCodecCtx->pix_fmts)
		pCodecCtx->pix_fmt = pSourceCodecCtx->pix_fmts[0];
	else
		pCodecCtx->pix_fmt = dec_ctx->pix_fmt;
	*/
	/* video time_base can be set to whatever is handy and supported by encoder */

	//pCodecCtx->time_base = dec_ctx->time_base;

	if (codec_id == AV_CODEC_ID_MPEG4)
	{
		// MPEG-4 Part 2 stores the PAR num/den as unsigned 8-bit fields,
		// and libavcodec's encoder fails to initialize if we don't
		// reduce it to fit 8-bits.
		hb_limit_rational(&pCodecCtx->sample_aspect_ratio.num,
			&pCodecCtx->sample_aspect_ratio.den,
			pCodecCtx->sample_aspect_ratio.num,
			pCodecCtx->sample_aspect_ratio.den, 255);
	}

	//hb_log("encavcodec: encoding with stored aspect %d/%d",
	//	job->par.num, job->par.den);

	// set colorimetry
	//context->color_primaries = hb_output_color_prim(job);
	//context->color_trc = hb_output_color_transfer(job);
	//context->colorspace = hb_output_color_matrix(job);

	/*
	if (!job->inline_parameter_sets)
	{
		context->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
	}
	if (job->grayscale)
	{
		context->flags |= AV_CODEC_FLAG_GRAY;
	}
	*/

	if (codec_id == AV_CODEC_ID_H264 && encoderName == "")
	{
		// Set profile and level
		if (videoCompressOption->encoder_profile != "")
		{
			if (videoCompressOption->encoder_profile == "baseline")
				av_dict_set(&param, "profile", "baseline", 0);
			else if (videoCompressOption->encoder_profile == "main")
				av_dict_set(&param, "profile", "main", 0);
			else if (videoCompressOption->encoder_profile == "high")
				av_dict_set(&param, "profile", "high", 0);
		}

		if (videoCompressOption->encoder_level != "")
		{
			int i = 1;
			while (hb_h264_level_names[i] != NULL)
			{
				if (videoCompressOption->encoder_level == hb_h264_level_names[i])
					av_dict_set(&param, "level", videoCompressOption->encoder_level, 0);
				++i;
			}
		}

		pCodecCtx->max_b_frames = 16;
	}

	if (codec_id == AV_CODEC_ID_H265 && encoderName == "")
	{
		// Set profile and level
		if (videoCompressOption->encoder_profile != "")
		{
			if (videoCompressOption->encoder_profile == "main")
				av_dict_set(&param, "profile", "main", 0);
			else if (videoCompressOption->encoder_profile == "main10")
				av_dict_set(&param, "profile", "main10", 0);
		}
		/*
		else
		{
			av_dict_set(&param, "profile", "main", 0);
		}
		*/
		pCodecCtx->max_b_frames = 16;
	}

	if (codec_id == AV_CODEC_ID_H264 && encoderName == "amf")
	{
		// Set profile and level
		pCodecCtx->profile = FF_PROFILE_UNKNOWN;
		if (videoCompressOption->encoder_profile != "")
		{
			if (videoCompressOption->encoder_profile == "baseline")
				pCodecCtx->profile = FF_PROFILE_H264_BASELINE;
			else if (videoCompressOption->encoder_profile == "main")
				pCodecCtx->profile = FF_PROFILE_H264_MAIN;
			else if (videoCompressOption->encoder_profile == "high")
				pCodecCtx->profile = FF_PROFILE_H264_HIGH;
		}
		pCodecCtx->level = FF_LEVEL_UNKNOWN;
		if (videoCompressOption->encoder_level != "")
		{
			int i = 1;
			while (hb_h264_level_names[i] != NULL)
			{
				if (videoCompressOption->encoder_level == hb_h264_level_names[i])
					pCodecCtx->level = hb_h264_level_values[i];
				++i;
			}
		}
	}

	if (codec_id == AV_CODEC_ID_H265 && encoderName == "amf")
	{
		// Set profile and level
		pCodecCtx->profile = FF_PROFILE_UNKNOWN;
		if (videoCompressOption->encoder_profile != "")
		{
			if (videoCompressOption->encoder_profile == "main")
				pCodecCtx->profile = FF_PROFILE_HEVC_MAIN;
		}
		pCodecCtx->level = FF_LEVEL_UNKNOWN;
		if (videoCompressOption->encoder_level != "")
		{
			int i = 1;
			while (hb_h265_level_names[i] != NULL)
			{
				if (videoCompressOption->encoder_level == hb_h264_level_names[i])
					pCodecCtx->level = hb_h265_level_values[i];
				++i;
			}
		}
		// FIXME
		//context->tier = FF_TIER_UNKNOWN;
	}

	if ((codec_id == AV_CODEC_ID_H264 || codec_id == AV_CODEC_ID_H265) && encoderName == "nvenc")
	{
		// Force IDR frames when we force a new keyframe for chapters
		av_dict_set(&param, "forced-idr", "1", 0);

		// Set profile and level
		if (videoCompressOption->encoder_profile != "")
		{
			if (videoCompressOption->encoder_profile == "baseline")
				av_dict_set(&param, "profile", "baseline", 0);
			else if (videoCompressOption->encoder_profile == "main")
				av_dict_set(&param, "profile", "main", 0);
			else if (videoCompressOption->encoder_profile == "high")
				av_dict_set(&param, "profile", "high", 0);
		}

		if (videoCompressOption->encoder_level != "")
		{
			int i = 1;
			while (hb_h264_level_names[i] != NULL)
			{
				if (videoCompressOption->encoder_level == hb_h264_level_names[i])
					av_dict_set(&param, "level", videoCompressOption->encoder_level, 0);
				++i;
			}
		}
	}

	// Make VCE h.265 encoder emit an IDR for every GOP
	if (codec_id == AV_CODEC_ID_H265 && encoderName == "nvenc")
	{
		av_dict_set(&param, "gops_per_idr", "1", 0);
	}

	//apply_encoder_preset(codec_id, &param, videoCompressOption->videoPreset);

	return param;

}


//h264_amf
//h264_nvenc

bool CFFmpegTranscodingPimpl::openHardEncoder(const AVCodecID &codec_id, const wxString &encoderName, AVCodecContext * pSourceCodecCtx)
{
	bool isSucceed = false;
	AVCodec * pCodec = nullptr;
	AVCodecContext * pCodecCtx = nullptr;
	wxString encoderHardName = GetCodecName(codec_id, encoderName);
	if (encoderName.size() > 0)
	{
		pCodec = avcodec_find_encoder_by_name(encoderHardName);

		if (pCodec != NULL)
		{
			pCodecCtx = avcodec_alloc_context3(pCodec);
			pCodecCtx->thread_count = 1;

			AVDictionary *param = setEncoderParam(codec_id, pCodecCtx, pSourceCodecCtx, encoderName);

			int ret = avcodec_open2(pCodecCtx, pCodec, &param);
			if (ret < 0)
			{
				avcodec_close(pCodecCtx);
				avcodec_free_context(&pCodecCtx);
				pCodecCtx = nullptr;
				isSucceed = false;

				//fprintf(stderr, "Could not open codec %s\n", hardWareDecoderName);
			}
			else
			{


				isSucceed = true;
				//fprintf(stderr, "open codec %s succeed! %d %d\n", hardWareDecoderName, pCodec->id, pCodecCtx->codec_id);
			}
		}
		else
		{
			//fprintf(stderr, "Codec %s not found.\n", hardWareDecoderName);
		}
	}
	avcodec_close(pCodecCtx);
	avcodec_free_context(&pCodecCtx);
	pCodecCtx = nullptr;
	return isSucceed;
}


bool CFFmpegTranscodingPimpl::openSoftEncoder(const AVCodecID &codec_id, AVCodecContext * pSourceCodecCtx)
{
	bool isSucceed = false;
	AVCodec * pCodec = nullptr;
	AVCodecContext * pCodecCtx = nullptr;
	pCodec = avcodec_find_encoder(codec_id);

	if (pCodec != NULL)
	{
		pCodecCtx = avcodec_alloc_context3(pCodec);
		pCodecCtx->thread_count = 1;

		AVDictionary *param = setEncoderParam(codec_id, pCodecCtx, pSourceCodecCtx, "");

		int ret = avcodec_open2(pCodecCtx, pCodec, &param);
		if (ret < 0)
		{
			avcodec_close(pCodecCtx);
			avcodec_free_context(&pCodecCtx);
			pCodecCtx = nullptr;
			isSucceed = false;

			//fprintf(stderr, "Could not open codec %s\n", hardWareDecoderName);
		}
		else
		{


			isSucceed = true;
			//fprintf(stderr, "open codec %s succeed! %d %d\n", hardWareDecoderName, pCodec->id, pCodecCtx->codec_id);
		}
	}

	avcodec_close(pCodecCtx);
	avcodec_free_context(&pCodecCtx);
	pCodecCtx = nullptr;
	return isSucceed;
}

wxString CFFmpegTranscodingPimpl::GetCodecName(AVCodecID codec_type, const wxString &encoderHardware)
{
	wxString codec_name;
	//if (encoderHardware != "")
	//	return GetCodecNameForEncoder(codec_type, encoderHardware);
	switch (codec_type)
	{
	case AV_CODEC_ID_MPEG4:
	{
		//hb_log("encavcodecInit: MPEG-4 ASP encoder");
		codec_name = "mpeg4";
	} break;
	case AV_CODEC_ID_MPEG2VIDEO:
	{
		if (encoderHardware == "qsv")
			codec_name = "mpeg2_qsv";
		else
			codec_name = "mpeg2video";
	} break;
	case AV_CODEC_ID_VP8:
	{
		// hb_log("encavcodecInit: VP8 encoder");
		codec_name = "libvpx";
	} break;
	case AV_CODEC_ID_VP9:
	{
		if (encoderHardware == "qsv")
			codec_name = "vp9_qsv";
		else
			codec_name = "libvpx-vp9";

	} break;
	case AV_CODEC_ID_H264:
	{
		if (encoderHardware == "nvenc")
			codec_name = "h264_nvenc";
		else if (encoderHardware == "amf")
			codec_name = "h264_amf";
		else if (encoderHardware == "mf")
			codec_name = "h264_mf";
		else if (encoderHardware == "videotoolbox")
			codec_name = "h264_videotoolbox";
		else if (encoderHardware == "qsv")
			codec_name = "h264_qsv";
		else
			codec_name = "libx264";
	}break;
	case AV_CODEC_ID_HEVC:
	{
		if (encoderHardware == "nvenc")
			codec_name = "hevc_nvenc";
		else if (encoderHardware == "amf")
			codec_name = "hevc_amf";
		else if (encoderHardware == "mf")
			codec_name = "hevc_mf";
		else if (encoderHardware == "videotoolbox")
			codec_name = "hevc_videotoolbox";
		else if (encoderHardware == "qsv")
			codec_name = "hevc_qsv";
		else
			codec_name = "libx265";
	}break;
	}
	return codec_name;
}

AVCodecID CFFmpegTranscodingPimpl::GetCodecID(AVMediaType codec_type)
{
	if (codec_type == AVMEDIA_TYPE_AUDIO)
	{
		if (videoCompressOption->audioCodec == "AAC")
		{
			return AV_CODEC_ID_AAC;
		}
		if (videoCompressOption->audioCodec == "MP3")
		{
			return AV_CODEC_ID_MP3;
		}
		if (videoCompressOption->audioCodec == "WAV")
		{
			return AV_CODEC_ID_PCM_S16LE;
		}
		if (videoCompressOption->audioCodec == "FLAC")
		{
			return AV_CODEC_ID_FLAC;
		}
		if (videoCompressOption->audioCodec == "VORBIS")
		{
			return AV_CODEC_ID_VORBIS;
		}
	}
	else
	{
		if (videoCompressOption->videoCodec == "H264")
		{
			return AV_CODEC_ID_H264;
		}
		if (videoCompressOption->videoCodec == "H265")
		{
			return AV_CODEC_ID_H265;
		}
		if (videoCompressOption->videoCodec == "VP8")
		{
			return  AV_CODEC_ID_VP8;
		}
		if (videoCompressOption->videoCodec == "VP9")
		{
			return  AV_CODEC_ID_VP9;
		}
		if (videoCompressOption->videoCodec == "MPEG4")
		{
			return AV_CODEC_ID_MPEG4;
		}
		if (videoCompressOption->videoCodec == "MPEG2")
		{
			return AV_CODEC_ID_MPEG2VIDEO;
		}
	}

	return AV_CODEC_ID_NONE;
}




wxString CFFmpegTranscodingPimpl::GetCodecNameForEncoder(AVCodecID vcodec, const wxString &nameEncoder)
{
	wxString nameCodecEncoder = "";
	switch (vcodec)
	{
	case AV_CODEC_ID_VP8:
		nameCodecEncoder = "vp8_";
		break;
	case AV_CODEC_ID_VP9:
		nameCodecEncoder = "vp9_";
		break;
	case AV_CODEC_ID_H264:
		nameCodecEncoder = "h264_";
		break;
	case AV_CODEC_ID_H265:
		nameCodecEncoder = "hevc_";
		break;
	default:
		break;
	}
	nameCodecEncoder += nameEncoder;
	return nameCodecEncoder;
}


int CFFmpegTranscodingPimpl::open_output_file(const wxString & filename)
{
	AVStream *out_stream;
	AVStream *in_stream;
	AVCodecContext *dec_ctx, *enc_ctx;
	AVCodec *encoder;
	int ret;
	wxString encoderHardware = "";
	unsigned int i;
	AVDictionary   * av_opts = nullptr;
	AVCodecID VIDEO_CODEC = GetCodecID(AVMEDIA_TYPE_VIDEO);
	AVCodecID AUDIO_CODEC = GetCodecID(AVMEDIA_TYPE_AUDIO);
	ofmt_ctx = NULL;

	//wxString filepath = filename;
	wxFileName filepath(filename);

	wxString extension = filepath.GetExt();

	//ret = avformat_alloc_output_context2(&mkvVideo, av_guess_format("matroska", "c:\\Users\\MPM\\Desktop\\test.mkv", NULL), "mkv", filename);

	if (extension == "mkv")
		avformat_alloc_output_context2(&ofmt_ctx, av_guess_format("matroska", CConvertUtility::ConvertToUTF8(filename), NULL), "mkv", CConvertUtility::ConvertToUTF8(filename));
	else if (extension == "webm")
		avformat_alloc_output_context2(&ofmt_ctx, av_guess_format("webm", CConvertUtility::ConvertToUTF8(filename), NULL), "webm", CConvertUtility::ConvertToUTF8(filename));
	else
		avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, CConvertUtility::ConvertToUTF8(filename));
	if (!ofmt_ctx) {
		av_log(NULL, AV_LOG_ERROR, "Could not create output context\n");
		return AVERROR_UNKNOWN;
	}

	for (i = 0; i < ifmt_ctx->nb_streams; i++) 
    {
		AVStream *stream = ifmt_ctx->streams[i];
        if(stream->codecpar->codec_id == AV_CODEC_ID_NONE)
            continue;
        
		out_stream = avformat_new_stream(ofmt_ctx, NULL);
		if (!out_stream) {
			av_log(NULL, AV_LOG_ERROR, "Failed allocating output stream\n");
			return AVERROR_UNKNOWN;
		}

		in_stream = ifmt_ctx->streams[i];
		dec_ctx = stream_ctx[i].dec_ctx;

		if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO
			|| dec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
			/* in this example, we choose transcoding to same codec */

			if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				enc_ctx = nullptr;
				encoder = nullptr;
#ifdef WIN32
				if (videoCompressOption->videoHardware)
				{
					bool success = false;
					encoderHardware = "nvenc"; //NVIDIA ENC
					success = openHardEncoder(VIDEO_CODEC, encoderHardware, dec_ctx);
					if (!success)
					{
						encoderHardware = "amf"; //AMD VCE
						success = openHardEncoder(VIDEO_CODEC, encoderHardware, dec_ctx);
					}
					if (!success)
					{
						encoderHardware = "qsv"; //Quicktime
						success = openHardEncoder(VIDEO_CODEC, encoderHardware, dec_ctx);
					}
					if (!success)
					{
						encoderHardware = "mf"; //MediaFoundation
						success = openHardEncoder(VIDEO_CODEC, encoderHardware, dec_ctx);

					}
					if (success)
					{
						printf("Hardware Encoder video use : %s \n", encoderHardware);
						encoder = avcodec_find_encoder_by_name(GetCodecName(VIDEO_CODEC, encoderHardware));
					}
					else
					{
						encoder = avcodec_find_encoder(VIDEO_CODEC);
						encoderHardware = "";
					}
				}
				else
				{
					encoderHardware = "";
					encoder = avcodec_find_encoder(VIDEO_CODEC);
				}
#elif defined(__APPLE__)
				if (videoCompressOption->videoHardware)
				{
					bool success = false;
					if (!success)
					{
						encoderHardware = "videotoolbox"; //Quicktime
						success = openHardEncoder(VIDEO_CODEC, encoderHardware, dec_ctx);
					}
					if (success)
					{
						encoder = avcodec_find_encoder_by_name(GetCodecName(VIDEO_CODEC, encoderHardware));
					}
					else
					{
						encoderHardware = "";
						encoder = avcodec_find_encoder(VIDEO_CODEC);
					}
				}
				else {
					encoderHardware = "";
					encoder = avcodec_find_encoder(VIDEO_CODEC);
				}
#else
				encoderHardware = "";
				encoder = avcodec_find_encoder(VIDEO_CODEC);
#endif

			}
			else
			{
				encoder = avcodec_find_encoder(AUDIO_CODEC);
			}
			if (!encoder) {
				av_log(NULL, AV_LOG_FATAL, "Necessary encoder not found\n");
				return AVERROR_INVALIDDATA;
			}
			enc_ctx = avcodec_alloc_context3(encoder);
			if (!enc_ctx) {
				av_log(NULL, AV_LOG_FATAL, "Failed to allocate the encoder context\n");
				return AVERROR(ENOMEM);
			}

			/* In this example, we transcode to same properties (picture size,
			 * sample rate etc.). These properties can be changed for output
			 * streams easily using filters */
			if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				AVDictionary *param = setEncoderParam(VIDEO_CODEC, enc_ctx, dec_ctx, encoderHardware);

				int ret = avcodec_open2(enc_ctx, encoder, &param);
				if (ret < 0) {
					av_log(NULL, AV_LOG_ERROR, "Cannot open video encoder for stream #%u\n", i);
					return ret;
				}
			}
			else {
				//enc_ctx->bit_rate = dst_abit_rate;
				enc_ctx->sample_rate = dec_ctx->sample_rate;
				enc_ctx->channel_layout = dec_ctx->channel_layout;
				enc_ctx->channels = av_get_channel_layout_nb_channels(enc_ctx->channel_layout);
				/* take first format from list of supported formats */
				enc_ctx->sample_fmt = encoder->sample_fmts[0];
				enc_ctx->time_base = { 1, enc_ctx->sample_rate };
				//enc_ctx->thread_count = FFMIN(8, std::thread::hardware_concurrency());
				if (videoCompressOption->audioQualityOrBitRate == 0)
				{
					enc_ctx->bit_rate = videoCompressOption->audioBitRate * 1000;
				}
				else if (videoCompressOption->audioQuality >= 0)
				{
					enc_ctx->global_quality = videoCompressOption->audioQuality * FF_QP2LAMBDA;
					enc_ctx->flags |= AV_CODEC_FLAG_QSCALE;
					if (AUDIO_CODEC == AV_CODEC_ID_AAC)
					{
						char vbr[8];
						snprintf(vbr, 8, "%.1g", videoCompressOption->audioQuality);
						av_dict_set(&av_opts, "vbr", vbr, 0);
					}
				}
				/*
				if (videoCompressOption-> >= 0)
				{
					context->compression_level = audio->config.out.compression_level;
				}
				*/
				// For some codecs, libav requires the following flag to be set
				// so that it fills extradata with global header information.
				// If this flag is not set, it inserts the data into each
				// packet instead.
				//enc_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

							/* Third parameter can be used to pass settings to encoder */
				ret = avcodec_open2(enc_ctx, encoder, &av_opts);
				if (ret < 0) {
					av_log(NULL, AV_LOG_ERROR, "Cannot open video encoder for stream #%u\n", i);
					return ret;
				}
			}





			ret = avcodec_parameters_from_context(out_stream->codecpar, enc_ctx);
			if (ret < 0) {
				av_log(NULL, AV_LOG_ERROR, "Failed to copy encoder parameters to output stream #%u\n", i);
				return ret;
			}

			out_stream->time_base = enc_ctx->time_base;
			stream_ctx[i].enc_ctx = enc_ctx;
		}
		else if (dec_ctx->codec_type == AVMEDIA_TYPE_UNKNOWN) {
			av_log(NULL, AV_LOG_FATAL, "Elementary stream #%d is of unknown type, cannot proceed\n", i);
			return AVERROR_INVALIDDATA;
		}
		else {
			/* if this stream must be remuxed */
			ret = avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar);
			if (ret < 0) {
				av_log(NULL, AV_LOG_ERROR, "Copying parameters for stream #%u failed\n", i);
				return ret;
			}
			out_stream->time_base = in_stream->time_base;
		}

	}
	av_dump_format(ofmt_ctx, 0, CConvertUtility::ConvertToUTF8(filename), 1);

	if (!(ofmt_ctx->oformat->flags & AVFMT_NOFILE)) {
		ret = avio_open(&ofmt_ctx->pb, CConvertUtility::ConvertToUTF8(filename), AVIO_FLAG_WRITE);
		if (ret < 0) {
			av_log(NULL, AV_LOG_ERROR, "Could not open output file '%s'", CConvertUtility::ConvertToUTF8(filename));
			return ret;
		}
	}

	/* init muxer, write output file header */
	ret = avformat_write_header(ofmt_ctx, NULL);
	if (ret < 0) {
		av_log(NULL, AV_LOG_ERROR, "Error occurred when opening output file\n");
		return ret;
	}

	return 0;
}

int CFFmpegTranscodingPimpl::init_filter(FilteringContext* fctx, AVCodecContext *dec_ctx,
	AVCodecContext *enc_ctx, const char *filter_spec)
{
	char args[512];
	int ret = 0;
	const AVFilter *buffersrc = NULL;
	const AVFilter *buffersink = NULL;
	AVFilterContext *buffersrc_ctx = NULL;
	AVFilterContext *buffersink_ctx = NULL;
	AVFilterInOut *outputs = avfilter_inout_alloc();
	AVFilterInOut *inputs = avfilter_inout_alloc();
	AVFilterGraph *filter_graph = avfilter_graph_alloc();

	if (!outputs || !inputs || !filter_graph) {
		ret = AVERROR(ENOMEM);
		goto end;
	}

	if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
		buffersrc = avfilter_get_by_name("buffer");
		buffersink = avfilter_get_by_name("buffersink");
		if (!buffersrc || !buffersink) {
			av_log(NULL, AV_LOG_ERROR, "filtering source or sink element not found\n");
			ret = AVERROR_UNKNOWN;
			goto end;
		}

		snprintf(args, sizeof(args),
			"video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:frame_rate=%d/%d:pixel_aspect=%d/%d",
			dec_ctx->width, dec_ctx->height, dec_ctx->pix_fmt,
			dec_ctx->time_base.num, dec_ctx->time_base.den,
			dec_ctx->framerate.num, dec_ctx->framerate.den,
			dec_ctx->sample_aspect_ratio.num,
			dec_ctx->sample_aspect_ratio.den);

		ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in",
			args, NULL, filter_graph);
		if (ret < 0) {
			av_log(NULL, AV_LOG_ERROR, "Cannot create buffer source\n");
			goto end;
		}

		ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out",
			NULL, NULL, filter_graph);
		if (ret < 0) {
			av_log(NULL, AV_LOG_ERROR, "Cannot create buffer sink\n");
			goto end;
		}

		ret = av_opt_set_bin(buffersink_ctx, "pix_fmts",
			(uint8_t*)&enc_ctx->pix_fmt, sizeof(enc_ctx->pix_fmt),
			AV_OPT_SEARCH_CHILDREN);
		if (ret < 0) {
			av_log(NULL, AV_LOG_ERROR, "Cannot set output pixel format\n");
			goto end;
		}
	}
	else if (dec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
		buffersrc = avfilter_get_by_name("abuffer");
		buffersink = avfilter_get_by_name("abuffersink");
		if (!buffersrc || !buffersink) {
			av_log(NULL, AV_LOG_ERROR, "filtering source or sink element not found\n");
			ret = AVERROR_UNKNOWN;
			goto end;
		}

		if (!dec_ctx->channel_layout)
			dec_ctx->channel_layout =
			av_get_default_channel_layout(dec_ctx->channels);
		snprintf(args, sizeof(args),
			"time_base=%d/%d:sample_rate=%d:sample_fmt=%s:channel_layout=0x%" PRIx64,
			dec_ctx->time_base.num, dec_ctx->time_base.den, dec_ctx->sample_rate,
			av_get_sample_fmt_name(dec_ctx->sample_fmt),
			dec_ctx->channel_layout);
		ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in",
			args, NULL, filter_graph);
		if (ret < 0) {
			av_log(NULL, AV_LOG_ERROR, "Cannot create audio buffer source\n");
			goto end;
		}

		ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out",
			NULL, NULL, filter_graph);
		if (ret < 0) {
			av_log(NULL, AV_LOG_ERROR, "Cannot create audio buffer sink\n");
			goto end;
		}

		ret = av_opt_set_bin(buffersink_ctx, "sample_fmts",
			(uint8_t*)&enc_ctx->sample_fmt, sizeof(enc_ctx->sample_fmt),
			AV_OPT_SEARCH_CHILDREN);
		if (ret < 0) {
			av_log(NULL, AV_LOG_ERROR, "Cannot set output sample format\n");
			goto end;
		}

		ret = av_opt_set_bin(buffersink_ctx, "channel_layouts",
			(uint8_t*)&enc_ctx->channel_layout,
			sizeof(enc_ctx->channel_layout), AV_OPT_SEARCH_CHILDREN);
		if (ret < 0) {
			av_log(NULL, AV_LOG_ERROR, "Cannot set output channel layout\n");
			goto end;
		}

		ret = av_opt_set_bin(buffersink_ctx, "sample_rates",
			(uint8_t*)&enc_ctx->sample_rate, sizeof(enc_ctx->sample_rate),
			AV_OPT_SEARCH_CHILDREN);
		if (ret < 0) {
			av_log(NULL, AV_LOG_ERROR, "Cannot set output sample rate\n");
			goto end;
		}
	}
	else {
		ret = AVERROR_UNKNOWN;
		goto end;
	}

	/* Endpoints for the filter graph. */
	outputs->name = av_strdup("in");
	outputs->filter_ctx = buffersrc_ctx;
	outputs->pad_idx = 0;
	outputs->next = NULL;

	inputs->name = av_strdup("out");
	inputs->filter_ctx = buffersink_ctx;
	inputs->pad_idx = 0;
	inputs->next = NULL;

	if (!outputs->name || !inputs->name) {
		ret = AVERROR(ENOMEM);
		goto end;
	}

	if ((ret = avfilter_graph_parse_ptr(filter_graph, filter_spec,
		&inputs, &outputs, NULL)) < 0)
		goto end;

	if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0)
		goto end;

	/* Fill FilteringContext */
	fctx->buffersrc_ctx = buffersrc_ctx;
	fctx->buffersink_ctx = buffersink_ctx;
	fctx->filter_graph = filter_graph;

end:
	avfilter_inout_free(&inputs);
	avfilter_inout_free(&outputs);

	return ret;
}

int CFFmpegTranscodingPimpl::init_filters(void)
{
	const char *filter_spec;
	unsigned int i;
	int ret;
	filter_ctx = (FilteringContext *)av_malloc_array(ifmt_ctx->nb_streams, sizeof(*filter_ctx));
	if (!filter_ctx)
		return AVERROR(ENOMEM);

	for (i = 0; i < ifmt_ctx->nb_streams; i++) {
		filter_ctx[i].buffersrc_ctx = NULL;
		filter_ctx[i].buffersink_ctx = NULL;
		filter_ctx[i].filter_graph = NULL;
		if (!(ifmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO
			|| ifmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO))
			continue;


		if (ifmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
			filter_spec = "null"; /* passthrough (dummy) filter for video */
		else
			filter_spec = "anull"; /* passthrough (dummy) filter for audio */
		ret = init_filter(&filter_ctx[i], stream_ctx[i].dec_ctx,
			stream_ctx[i].enc_ctx, filter_spec);
		if (ret)
			return ret;

		filter_ctx[i].filtered_frame = av_frame_alloc();
		if (!filter_ctx[i].filtered_frame)
			return AVERROR(ENOMEM);
	}
	return 0;
}

int CFFmpegTranscodingPimpl::encode_write_frame(AVFrame *filt_frame, unsigned int stream_index)
{
	StreamContext *stream = &stream_ctx[stream_index];
	int ret;
	AVPacket enc_pkt;

	av_log(NULL, AV_LOG_INFO, "Encoding frame\n");
	/* encode filtered frame */
	enc_pkt.data = NULL;
	enc_pkt.size = 0;
	av_init_packet(&enc_pkt);

	ret = avcodec_send_frame(stream->enc_ctx, filt_frame);

	if (ret < 0)
		return ret;

	while (ret >= 0) {
		ret = avcodec_receive_packet(stream->enc_ctx, &enc_pkt);

		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			return 0;

		/* prepare packet for muxing */
		enc_pkt.stream_index = stream_index;

		av_packet_rescale_ts(&enc_pkt,
			stream->enc_ctx->time_base,
			ofmt_ctx->streams[stream_index]->time_base);

		if (enc_pkt.duration > 0)
			enc_pkt.duration = av_rescale_q(enc_pkt.duration, ofmt_ctx->streams[stream_index]->time_base, stream->enc_ctx->time_base);


		av_log(NULL, AV_LOG_DEBUG, "Muxing frame\n");
		/* mux encoded frame */
		ret = av_interleaved_write_frame(ofmt_ctx, &enc_pkt);
	}

	return ret;
}

int CFFmpegTranscodingPimpl::filter_encode_write_frame(AVFrame *frame, unsigned int stream_index, CompressVideo * m_dlgProgress, const int &isvideo)
{
	FilteringContext *filter = &filter_ctx[stream_index];
	int ret;

	av_log(NULL, AV_LOG_INFO, "Pushing decoded frame to filters\n");
	/* push the decoded frame into the filtergraph */
	ret = av_buffersrc_add_frame_flags(filter->buffersrc_ctx,
		frame, 0);
	if (ret < 0) {
		av_log(NULL, AV_LOG_ERROR, "Error while feeding the filtergraph\n");
		return ret;
	}

	/* pull filtered frames from the filtergraph */
	while (1) {
		av_log(NULL, AV_LOG_INFO, "Pulling filtered frame from filters\n");
		ret = av_buffersink_get_frame(filter->buffersink_ctx,
			filter->filtered_frame);
		if (ret < 0) {
			/* if no more frames for output - returns AVERROR(EAGAIN)
			 * if flushed and no more frames for output - returns AVERROR_EOF
			 * rewrite retcode to 0 to show it as normal procedure completion
			 */
			if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
				ret = 0;
			break;
		}

		if (isvideo)
		{
			nbframe++;
			end = std::chrono::steady_clock::now();
			SetFrameData(filter->filtered_frame, m_dlgProgress);
		}

		filter->filtered_frame->pict_type = AV_PICTURE_TYPE_NONE;
		ret = encode_write_frame(filter->filtered_frame, stream_index);
		av_frame_unref(filter->filtered_frame);
		if (ret < 0)
			break;
	}

	return ret;
}


void CFFmpegTranscodingPimpl::SetFrameData(AVFrame * src_frame, CompressVideo * m_dlgProgress)
{
	if (processEnd)
		return;

	bool createFrame = true;

	if (bitmapShow == nullptr)
	{
		createFrame = true;
		isend = false;
	}
	else
	{
		bool threadEnd = false;
		muEnding.lock();
		threadEnd = isend;
		muEnding.unlock();
		if (threadEnd)
		{
			bitmapShow->join();
			delete bitmapShow;
			bitmapShow = nullptr;
			createFrame = true;
			isend = false;
		}
	}

	if (createFrame)
	{
		muFrame.lock();
		CopyFrame(src_frame);
		muFrame.unlock();
		bitmapShow = new thread(DisplayPreview, this);
	}

}


int CFFmpegTranscodingPimpl::flush_encoder(unsigned int stream_index)
{
    av_log(NULL, AV_LOG_INFO, "Flushing stream #%u encoder\n", stream_index);
	if (!(stream_ctx[stream_index].enc_ctx->codec->capabilities &
		AV_CODEC_CAP_DELAY))
		return 0;

	return encode_write_frame(NULL, stream_index);
}

int CFFmpegTranscodingPimpl::OpenFile(const wxString & input, const wxString & output)
{
	int ret;
	if ((ret = open_input_file(input)) < 0)
		return ret;
	if ((ret = open_output_file(output)) < 0)
		return ret;
	ret = init_filters();

	return ret;
}

int CFFmpegTranscodingPimpl::ProcessEncodeFile(AVFrame * dst, SwsContext* scaleContext)
{
	int ret = 0;
	int stream_index = 0;
	bool first = true;
	bool startEncoding = true;
	bool first_frame = true;
	
	if (videoCompressOption->startTime != 0)
	{
		int64_t timestamp = static_cast<int64_t>(videoCompressOption->startTime) * 1000 * 1000;
		//int64_t timestamp = (AV_TIME_BASE / 100) * static_cast<int64_t>(videoCompressOption->startTime);

		if (timestamp < 0)
		{
			timestamp = 0;
		}

	//	int64_t timestamp = (AV_TIME_BASE / 100) * static_cast<int64_t>(videoCompressOption->startTime);
		int64_t seek_target = timestamp;
		int64_t seek_rel = 0;
		int64_t seek_min = seek_rel > 0 ? seek_target - seek_rel + 2 : INT64_MIN;
		int64_t seek_max = seek_rel < 0 ? seek_target - seek_rel - 2 : INT64_MAX;
		int seek_flags = 0;
		seek_flags &= ~AVSEEK_FLAG_BYTE;
		// FIXME the +-2 is due to rounding being not done in the correct direction in generation
		//      of the seek_pos/seek_rel variables

		ret = avformat_seek_file(ifmt_ctx, -1, seek_min, seek_target, seek_max, seek_flags);

		/*
		ret = av_seek_frame(ifmt_ctx, videoStreamIndex, timestamp, 0);
		if (ret != 0)
			return ret;
		
		startEncoding = false;
		*/
	}
	


	/* read all packets */
	while (m_dlgProgress->IsOk())
	{


		if ((ret = av_read_frame(ifmt_ctx, &packet)) < 0)
			break;

		bool isVideo = false;
		stream_index = packet.stream_index;


		av_log(NULL, AV_LOG_DEBUG, "Demuxer gave frame of stream_index %u\n",
			stream_index);

		AVStream *st = ifmt_ctx->streams[packet.stream_index];
		switch (st->codecpar->codec_type)
		{
		case AVMEDIA_TYPE_AUDIO:
			printf("audio \n");
			break;

		case AVMEDIA_TYPE_VIDEO:
			printf("video \n");
			isVideo = true;
			//SetFrameData()
			break;
		}

        if(st->codecpar->codec_id == AV_CODEC_ID_NONE)
            goto continue_packet;

		if (filter_ctx[stream_index].filter_graph) {

			CFFmpegTranscodingPimpl::StreamContext *stream = &stream_ctx[stream_index];
			

			av_log(NULL, AV_LOG_DEBUG, "Going to reencode&filter the frame\n");

			av_packet_rescale_ts(&packet,
				ifmt_ctx->streams[stream_index]->time_base,
				stream->dec_ctx->time_base);

			pos = ((double)packet.pts * stream->dec_ctx->time_base.num / stream->dec_ctx->time_base.den);
			double dts = ((double)packet.dts * stream->dec_ctx->time_base.num / stream->dec_ctx->time_base.den);
			ret = avcodec_send_packet(stream->dec_ctx, &packet);
			if (ret < 0) {
				av_log(NULL, AV_LOG_ERROR, "Decoding failed\n");
				break;
			}
					   			 		  
			/*
			printf("pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
				av_ts2str(packet->pts), av_ts2timestr(packet->pts, time_base),
				av_ts2str(packet->dts), av_ts2timestr(packet->dts, time_base),
				av_ts2str(packet->duration), av_ts2timestr(packet->duration, time_base),
				pkt->stream_index);
			*/



			AVFrame *tmp_frame = NULL;
			int ret = 0;

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

						if (first)
						{



							av_opt_set_int(scaleContext, "srcw", sw_frame->width, 0);
							av_opt_set_int(scaleContext, "srch", sw_frame->height, 0);
							av_opt_set_int(scaleContext, "src_format", sw_frame->format, 0);
							av_opt_set_int(scaleContext, "dstw", sw_frame->width, 0);
							av_opt_set_int(scaleContext, "dsth", sw_frame->height, 0);
							av_opt_set_int(scaleContext, "dst_format", AV_PIX_FMT_YUV420P, 0);
							av_opt_set_int(scaleContext, "sws_flags", SWS_FAST_BILINEAR, 0);

							if (sws_init_context(scaleContext, nullptr, nullptr) < 0)
							{
								sws_freeContext(scaleContext);
								throw std::logic_error("Failed to initialise scale context");
							}

							first = false;
							dst->format = AV_PIX_FMT_YUV420P;
							dst->width = stream->dec_frame->width;
							dst->height = stream->dec_frame->height;
							dst->channels = stream->dec_frame->channels;
							dst->channel_layout = stream->dec_frame->channel_layout;
							dst->nb_samples = stream->dec_frame->nb_samples;
							int res = av_image_alloc(dst->data, dst->linesize, sw_frame->width, sw_frame->height, AV_PIX_FMT_YUV420P, 1);
						}
						av_frame_copy_props(dst, stream->dec_frame);

						sws_scale(scaleContext,
							(uint8_t const * const *)sw_frame->data, sw_frame->linesize, 0, (int)stream->dec_frame->height,
							dst->data, dst->linesize);

						tmp_frame = dst;
						av_frame_free(&sw_frame);
					}
					else
						tmp_frame = stream->dec_frame;
				}
				else
					tmp_frame = stream->dec_frame;

				tmp_frame->pts = tmp_frame->best_effort_timestamp;


				if (isVideo)
				{
					
					if (first_frame)
					{
						long startTime = videoCompressOption->startTime;
						videoCompressOption->startTime = pos;
						long diff = startTime - pos;
						videoCompressOption->endTime = videoCompressOption->endTime + diff;
						first_frame = false;
					}
					
					muWriteData.lock();
					double duration_total = duration_movie;

					if (videoCompressOption->endTime != 0 || videoCompressOption->startTime != 0)
						duration_total = videoCompressOption->endTime - videoCompressOption->startTime;

					int posVideo = (int)pos - videoCompressOption->startTime;
					pourcentage = (posVideo / duration_total) * 100.0f;
					this->nbframePerSecond = stream->dec_ctx->framerate.num / stream->dec_ctx->framerate.den;
					sprintf(duration, "Progress : %d percent - Total Second : %d / %d", (int)pourcentage, (int)posVideo, (int)duration_total);
					muWriteData.unlock();

				}

				ret = filter_encode_write_frame(tmp_frame, stream_index, m_dlgProgress, isVideo);
				if (ret < 0)
					return ret;

			}
		}
		else {
			/* remux this frame without reencoding */
			av_packet_rescale_ts(&packet,
				ifmt_ctx->streams[stream_index]->time_base,
				ofmt_ctx->streams[stream_index]->time_base);

			ret = av_interleaved_write_frame(ofmt_ctx, &packet);
			if (ret < 0)
				return ret;
		}

continue_packet:

		av_packet_unref(&packet);

		if (videoCompressOption->endTime != 0)
		{
			int posVideo = (int)pos;
			if (videoCompressOption->endTime == posVideo)
				break;
		}
	}

	/* flush filters and encoders */
	for (int i = 0; i < ifmt_ctx->nb_streams; i++) {
		/* flush filter */
		if (!filter_ctx[i].filter_graph)
			continue;
		ret = filter_encode_write_frame(NULL, i, m_dlgProgress, 0);
		if (ret < 0) {
			av_log(NULL, AV_LOG_ERROR, "Flushing filter failed\n");
			return ret;
		}

		/* flush encoder */
		ret = flush_encoder(i);
		if (ret < 0) {
			av_log(NULL, AV_LOG_ERROR, "Flushing encoder failed\n");
			return ret;
		}
	}

	av_write_trailer(ofmt_ctx);
	return ret;
}


int CFFmpegTranscodingPimpl::EncodeFile(const wxString & input, const wxString & output, CompressVideo * m_dlgProgress, CVideoOptionCompress * videoCompressOption)
{
	int ret;
	this->m_dlgProgress = m_dlgProgress;
	unsigned int stream_index;
	unsigned int i;
	nbframe = 0;
	bool first = true;
	cleanPacket = false;
	this->videoCompressOption = videoCompressOption;
	processEnd = false;

	if ((ret = OpenFile(input, output)) < 0)
		return ret;

	cleanPacket = true;
	begin = std::chrono::steady_clock::now();

	ProcessEncodeFile(dst, scaleContext);

	return ret ? 1 : 0;
}


void CFFmpegTranscodingPimpl::CopyFrame(AVFrame * frame)
{
	AVFrame * copyFrame = av_frame_alloc();
	copyFrame->format = frame->format;
	copyFrame->width = frame->width;
	copyFrame->height = frame->height;
	copyFrame->channels = frame->channels;
	copyFrame->channel_layout = frame->channel_layout;
	copyFrame->nb_samples = frame->nb_samples;

	av_frame_get_buffer(copyFrame, 32);
	av_frame_copy(copyFrame, frame);
	av_frame_copy_props(copyFrame, frame);


	copyFrameBuffer = copyFrame;

}

void CFFmpegTranscodingPimpl::Release()
{
	if (ifmt_ctx != nullptr)
	{
		for (int i = 0; i < ifmt_ctx->nb_streams; i++) {
			avcodec_free_context(&stream_ctx[i].dec_ctx);
			if (ofmt_ctx && ofmt_ctx->nb_streams > i && ofmt_ctx->streams[i] && stream_ctx[i].enc_ctx)
				avcodec_free_context(&stream_ctx[i].enc_ctx);
			if (filter_ctx && filter_ctx[i].filter_graph) {
				avfilter_graph_free(&filter_ctx[i].filter_graph);
				av_frame_free(&filter_ctx[i].filtered_frame);
			}

			av_frame_free(&stream_ctx[i].dec_frame);
		}
		av_free(filter_ctx);
		av_free(stream_ctx);
		avformat_close_input(&ifmt_ctx);
        
        filter_ctx = nullptr;
        stream_ctx = nullptr;
        ifmt_ctx = nullptr;
	}


	if (ofmt_ctx != nullptr)
	{
		if (ofmt_ctx && !(ofmt_ctx->oformat->flags & AVFMT_NOFILE))
			avio_closep(&ofmt_ctx->pb);
		avformat_free_context(ofmt_ctx);
        
        ofmt_ctx = nullptr;
	}

}
