// ReSharper disable All
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
#include <chrono>
#include <FiltreEffet.h>
#include <OpenCLFilter.h>
#include <OpenCLEffectVideo.h>
#include <FiltreEffetCPU.h>
#include <ConvertUtility.h>
#include <MediaInfo.h>
#include <VideoPlayer.h>

extern "C"
{
	#include <libavcodec/avcodec.h>
	#include <libavcodec/packet.h>
	#include <libavutil/opt.h>
	#include <libavutil/imgutils.h>
	#include <libavutil/display.h>
	#include <libavutil/channel_layout.h>
}


#include <opencv2/core/core.hpp>
#include <ffmpeg_application.h>
using namespace cv;
using namespace Regards::OpenCL;
using namespace Regards::Video;

static const char* ROTATE = "rotate";
static const char* hb_h264_level_names[] = {
	"auto", "1.0", "1b", "1.1", "1.2", "1.3", "2.0", "2.1", "2.2", "3.0", "3.1", "3.2", "4.0", "4.1", "4.2", "5.0",
	"5.1", "5.2", nullptr,
};
static const int hb_h264_level_values[] = { -1, 10, 9, 11, 12, 13, 20, 21, 22, 30, 31, 32, 40, 41, 42, 50, 51, 52, 0, };
static const int hb_h265_level_values[] = {
	-1, 30, 60, 63, 90, 93, 120, 123,
	150, 153, 156, 180, 183, 186, 0,
};
static const char* hb_h265_level_names[] = {
	"auto", "1.0", "2.0", "2.1", "3.0", "3.1", "4.0", "4.1",
	"5.0", "5.1", "5.2", "6.0", "6.1", "6.2", nullptr,
};

const char* extract_metadata_internal(AVFormatContext* ic, AVStream* audio_st, AVStream* video_st, const char* key) {
	char* value = NULL;

	if (!ic) {
		return value;
	}

	if (key) {
		if (av_dict_get(ic->metadata, key, NULL, AV_DICT_MATCH_CASE)) {
			value = av_dict_get(ic->metadata, key, NULL, AV_DICT_MATCH_CASE)->value;
		}
		else if (audio_st && av_dict_get(audio_st->metadata, key, NULL, AV_DICT_MATCH_CASE)) {
			value = av_dict_get(audio_st->metadata, key, NULL, AV_DICT_MATCH_CASE)->value;
		}
		else if (video_st && av_dict_get(video_st->metadata, key, NULL, AV_DICT_MATCH_CASE)) {
			value = av_dict_get(video_st->metadata, key, NULL, AV_DICT_MATCH_CASE)->value;
		}
	}

	return value;
}

void set_rotation(AVFormatContext* ic, AVStream* audio_st, AVStream* video_st) {
	if (!extract_metadata_internal(ic, audio_st, video_st, ROTATE) && video_st && video_st->metadata) {
		AVDictionaryEntry* entry = av_dict_get(video_st->metadata, ROTATE, NULL, AV_DICT_MATCH_CASE);

		if (entry && entry->value) {
			av_dict_set(&ic->metadata, ROTATE, entry->value, 0);
		}
		else {
			av_dict_set(&ic->metadata, ROTATE, "0", 0);
		}
	}
}

/**********************************************************************
 * hb_reduce
 **********************************************************************
 * Given a numerator (num) and a denominator (den), reduce them to an
 * equivalent fraction and store the result in x and y.
 *********************************************************************/
void hb_reduce(int* x, int* y, int num, int den)
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

void hb_limit_rational(int* x, int* y, int num, int den, int limit)
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
		const double div = static_cast<double>(limit) / num;
		num = limit;
		den *= div;
	}
	else
	{
		const double div = static_cast<double>(limit) / den;
		den = limit;
		num *= div;
	}
	*x = num;
	*y = den;
}

/*
static int apply_vpx_preset(AVDictionary** av_opts, const wxString& preset)
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
/*
static int apply_vp8_preset(AVDictionary** av_opts, const wxString& preset)
{
	return apply_vpx_preset(av_opts, preset);
}

static int apply_vp9_preset(AVDictionary** av_opts, const wxString& preset)
{
	av_dict_set(av_opts, "row-mt", "1", 0);
	return apply_vpx_preset(av_opts, preset);
}
*/

#ifdef WIN32
wxString listencoderHardware[] = { "nvenc", "amf", "qsv","mf", "libmfx", "opencl" };
int sizeListEncoderHardware = 6;
#elif defined(__APPLE__)
wxString listencoderHardware[] = { "videotoolbox", "opencl" };
int sizeListEncoderHardware = 1;
#else
wxString listencoderHardware[] = { "nvenc", "vaapi", "libmfx", "opencl" };
int sizeListEncoderHardware = 4;
#endif



static enum AVPixelFormat hw_pix_fmt;

static wxString ConvertSecondToTime(int sec)
{
	int h = (sec / 3600);
	int m = (sec - (3600 * h)) / 60;
	int s = (sec - (3600 * h) - (m * 60));
	return wxString::Format("%02d:%02d:%02d\n", h, m, s);
}

CFFmpegTranscodingPimpl::CFFmpegTranscodingPimpl() : stream_ctx(nullptr),
m_dlgProgress(nullptr),
videoCompressOption(nullptr), duration{}
{
	//dst = av_frame_alloc();
	scaleContext = sws_alloc_context();
	packet.data = nullptr;
	packet.size = 0;
	bitmapOut = new CRegardsBitmap();

}

void CFFmpegTranscodingPimpl::EndTreatment()
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


int CFFmpegTranscodingPimpl::IsSupportOpenCL()
{
	int supportOpenCL = 0;
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		supportOpenCL = config->GetIsOpenCLSupport();

	if (!cv::ocl::haveOpenCL())
		supportOpenCL = 0;

	return supportOpenCL;
}


CFFmpegTranscodingPimpl::~CFFmpegTranscodingPimpl()
{
	EndTreatment();


	if (openCVStabilization != nullptr)
		delete openCVStabilization;

	if (dst_hardware != nullptr)
	{
		av_freep(&dst_hardware->data[0]);
		av_frame_free(&dst_hardware);
	}

	if (scaleContext != nullptr)
		sws_freeContext(scaleContext);

	if (localContext != nullptr)
		sws_freeContext(localContext);

	if (capture != nullptr)
		delete capture;

	if (bitmapOut != nullptr)
		delete bitmapOut;

};

void CFFmpegTranscodingPimpl::DisplayPreview(void* data)
{

	CFFmpegTranscodingPimpl* ffmpeg_trans = static_cast<CFFmpegTranscodingPimpl*>(data);
	if (ffmpeg_trans != nullptr)
	{
		char duration[255];
		//auto imageLoadingFormat = new CImageLoadingFormat(false);
		ffmpeg_trans->muFrame.lock();
		ffmpeg_trans->m_dlgProgress->SetBitmap(ffmpeg_trans->bmp);
		delete ffmpeg_trans->bmp;
		ffmpeg_trans->muFrame.unlock();
		ffmpeg_trans->muWriteData.lock();

		double percent = ((double)ffmpeg_trans->nbFrameEncoded / (double)ffmpeg_trans->totalFrame);
		double pos = percent * ffmpeg_trans->duration;

		sprintf(duration, "Progress : %d percent - Total Second : %d / %d", static_cast<int>(percent * 100), static_cast<int>(pos), static_cast<int>(ffmpeg_trans->duration));

		ffmpeg_trans->m_dlgProgress->SetPos(static_cast<int>(ffmpeg_trans->duration), static_cast<int>(pos));
		ffmpeg_trans->m_dlgProgress->SetTextProgression(duration);


		double dif = std::chrono::duration_cast<std::chrono::seconds>(
			std::chrono::steady_clock::now() - ffmpeg_trans->begin).count();

		try
		{

			// dif = dif / 100.0;
			int nbFpsPerSecond = 1;
			if (dif > 0)
				nbFpsPerSecond = static_cast<int>(ffmpeg_trans->nbFrameEncoded / dif);
			int missingFrame = ffmpeg_trans->totalFrame - ffmpeg_trans->nbFrameEncoded;

			double timeMissing = 1;
			if (nbFpsPerSecond > 0)
				timeMissing = missingFrame / nbFpsPerSecond;

			wxString frame = wxString::Format("%d fps", nbFpsPerSecond);
			ffmpeg_trans->m_dlgProgress->SetTextProgression(frame, 2);

			frame = ConvertSecondToTime(timeMissing);
			ffmpeg_trans->m_dlgProgress->SetTextProgression(frame, 3);

			frame = ConvertSecondToTime(static_cast<int>(dif));
			ffmpeg_trans->m_dlgProgress->SetTextProgression(frame, 1);



		}
		catch (...)
		{
		}
		ffmpeg_trans->muWriteData.unlock();



	}

	ffmpeg_trans->muEnding.lock();
	ffmpeg_trans->isend = true;
	ffmpeg_trans->muEnding.unlock();
}

int CFFmpegTranscodingPimpl::hw_decoder_init(AVCodecContext* ctx, const enum AVHWDeviceType type)
{
	int err;

	if ((err = av_hwdevice_ctx_create(&hw_device_ctx, type,
		nullptr, nullptr, 0)) < 0)
	{
		fprintf(stderr, "Failed to create specified HW device.\n");
		return err;
	}
	ctx->hw_device_ctx = av_buffer_ref(hw_device_ctx);

	return err;
}

enum AVPixelFormat CFFmpegTranscodingPimpl::get_hw_format(AVCodecContext* ctx,
	const enum AVPixelFormat* pix_fmts)
{
	const enum AVPixelFormat* p;

	for (p = pix_fmts; *p != -1; p++)
	{
		if (*p == hw_pix_fmt)
			return *p;
	}

	fprintf(stderr, "Failed to get HW surface format.\n");
	return AV_PIX_FMT_NONE;
}

double CFFmpegTranscodingPimpl::get_rotation(AVStream* st)
{
	uint8_t* displaymatrix = av_stream_get_side_data(st,
		AV_PKT_DATA_DISPLAYMATRIX, NULL);
	double theta = 0;
	if (displaymatrix)
		theta = -av_display_rotation_get((int32_t*)displaymatrix);

	theta -= 360 * floor(theta / 360 + 0.9 / 360);

	if (fabs(theta - 90 * round(theta / 90)) > 2)
		av_log(NULL, AV_LOG_WARNING, "Odd rotation angle.\n"
			"If you want to help, upload a sample "
			"of this file to https://streams.videolan.org/upload/ "
			"and contact the ffmpeg-devel mailing list. (ffmpeg-devel@ffmpeg.org)");

	return theta;
}

int CFFmpegTranscodingPimpl::open_input_file(const wxString& filename)
{
	int ret;
	unsigned int i;

	ifmt_ctx = nullptr;
	if ((ret = avformat_open_input(&ifmt_ctx, CConvertUtility::ConvertToUTF8(filename), nullptr, nullptr)) < 0)
	{
		av_log(nullptr, AV_LOG_ERROR, "Cannot open input file\n");
		return ret;
	}

	if ((ret = avformat_find_stream_info(ifmt_ctx, nullptr)) < 0)
	{
		av_log(nullptr, AV_LOG_ERROR, "Cannot find stream information\n");
		return ret;
	}

	stream_ctx = static_cast<StreamContext*>(av_mallocz_array(ifmt_ctx->nb_streams, sizeof(*stream_ctx)));
	if (!stream_ctx)
		return AVERROR(ENOMEM);

	for (i = 0; i < ifmt_ctx->nb_streams; i++)
	{
		AVStream* stream = ifmt_ctx->streams[i];
		if (stream->codecpar->codec_id == AV_CODEC_ID_NONE)
			continue;
		const AVCodec* dec = avcodec_find_decoder(stream->codecpar->codec_id);
		AVCodecContext* codec_ctx;
		if (!dec)
		{
			av_log(nullptr, AV_LOG_ERROR, "Failed to find decoder for stream #%u\n", i);
			return AVERROR_DECODER_NOT_FOUND;
		}
		codec_ctx = avcodec_alloc_context3(dec);
		if (!codec_ctx)
		{
			av_log(nullptr, AV_LOG_ERROR, "Failed to allocate the decoder context for stream #%u\n", i);
			return AVERROR(ENOMEM);
		}
		ret = avcodec_parameters_to_context(codec_ctx, stream->codecpar);
		if (ret < 0)
		{
			av_log(nullptr, AV_LOG_ERROR, "Failed to copy decoder parameters to input decoder context "
				"for stream #%u\n", i);
			return ret;
		}
		/* Reencode video & audio and remux subtitles etc. */
		if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO
			|| codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
				codec_ctx->framerate = av_guess_frame_rate(ifmt_ctx, stream, nullptr);

			if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				videoStreamIndex = i;
			}


			ret = avcodec_open2(codec_ctx, dec, nullptr);
			if (ret < 0)
			{
				av_log(nullptr, AV_LOG_ERROR, "Failed to open decoder for stream #%u\n", i);
				return ret;
			}

			if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				startTime = ifmt_ctx->start_time;

				rotation = get_rotation(stream);
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


int CFFmpegTranscodingPimpl::open_input_file(const wxString& filename, const wxString& decodeHardware)
{
	//if(decodeHardware == "")
	//{
	int ret;
	unsigned int i;

	enum AVHWDeviceType type = AV_HWDEVICE_TYPE_NONE;
	this->decodeHardware = decodeHardware;
	if (decodeHardware != "" || decodeHardware != "none")
	{
		type = av_hwdevice_find_type_by_name(CConvertUtility::ConvertToUTF8(decodeHardware));
		if (type == AV_HWDEVICE_TYPE_NONE) {
			fprintf(stderr, "Device type %s is not supported.\n", CConvertUtility::ConvertToUTF8(decodeHardware));
			fprintf(stderr, "Available device types:");
			while ((type = av_hwdevice_iterate_types(type)) != AV_HWDEVICE_TYPE_NONE)
				fprintf(stderr, " %s", av_hwdevice_get_type_name(type));
			fprintf(stderr, "\n");
		}
	}

	ifmt_ctx = nullptr;
	if ((ret = avformat_open_input(&ifmt_ctx, CConvertUtility::ConvertToUTF8(filename), nullptr, nullptr)) < 0)
	{
		av_log(nullptr, AV_LOG_ERROR, "Cannot open input file\n");
		return ret;
	}

	if ((ret = avformat_find_stream_info(ifmt_ctx, nullptr)) < 0)
	{
		av_log(nullptr, AV_LOG_ERROR, "Cannot find stream information\n");
		return ret;
	}

	stream_ctx = static_cast<StreamContext*>(av_mallocz_array(ifmt_ctx->nb_streams, sizeof(*stream_ctx)));
	if (!stream_ctx)
		return AVERROR(ENOMEM);

	for (i = 0; i < ifmt_ctx->nb_streams; i++)
	{
		AVStream* stream = ifmt_ctx->streams[i];
		if (stream->codecpar->codec_id == AV_CODEC_ID_NONE)
			continue;
		const AVCodec* dec = avcodec_find_decoder(stream->codecpar->codec_id);
		AVCodecContext* codec_ctx;
		if (!dec)
		{
			av_log(nullptr, AV_LOG_ERROR, "Failed to find decoder for stream #%u\n", i);
			return AVERROR_DECODER_NOT_FOUND;
		}
		codec_ctx = avcodec_alloc_context3(dec);
		if (!codec_ctx)
		{
			av_log(nullptr, AV_LOG_ERROR, "Failed to allocate the decoder context for stream #%u\n", i);
			return AVERROR(ENOMEM);
		}
		ret = avcodec_parameters_to_context(codec_ctx, stream->codecpar);
		if (ret < 0)
		{
			av_log(nullptr, AV_LOG_ERROR, "Failed to copy decoder parameters to input decoder context "
				"for stream #%u\n", i);
			return ret;
		}
		/* Reencode video & audio and remux subtitles etc. */
		if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO
			|| codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
			{

				if (decodeHardware != "" || decodeHardware != "none")
				{
					for (int j = 0;; j++) {
						const AVCodecHWConfig* config = avcodec_get_hw_config(dec, j);
						if (!config) {
							fprintf(stderr, "Decoder %s does not support device type %s.\n",
								dec->name, av_hwdevice_get_type_name(type));
							return -1;
						}
						if (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX &&
							config->device_type == type) {
							hw_pix_fmt = config->pix_fmt;
							pixelFormatInput = AV_PIX_FMT_NV12;
							break;
						}
					}

				}
				else
					pixelFormatInput = AV_PIX_FMT_YUV420P;

				codec_ctx->framerate = av_guess_frame_rate(ifmt_ctx, stream, nullptr);
				videoStreamIndex = i;

				if (decodeHardware != "" || decodeHardware != "none")
				{

					codec_ctx->get_format = get_hw_format;

					ret = hw_decoder_init(codec_ctx, type);
					if (ret < 0)
						return ret;
				}

			}


			ret = avcodec_open2(codec_ctx, dec, nullptr);
			if (ret < 0)
			{
				av_log(nullptr, AV_LOG_ERROR, "Failed to open decoder for stream #%u\n", i);
				return ret;
			}

			if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				startTime = ifmt_ctx->start_time;

				rotation = get_rotation(stream);

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


int CFFmpegTranscodingPimpl::EncodeFrame(const int& stream_index, int& positionMovie, const bool& isVideo, const bool& write)
{
	int ret = 0;

	StreamContext* stream = &stream_ctx[stream_index];

	av_log(nullptr, AV_LOG_DEBUG, "Going to reencode&filter the frame\n");

	int outStreamIndex = streamInNumberInOut[stream_index];

	if (ofmt_ctx->streams[outStreamIndex]->time_base.den == ifmt_ctx->streams[stream_index]->time_base.den
		&& ofmt_ctx->streams[outStreamIndex]->time_base.num == ifmt_ctx->streams[stream_index]->time_base.
		num)
	{
		av_packet_rescale_ts(&packet,
			ifmt_ctx->streams[stream_index]->time_base,
			ofmt_ctx->streams[outStreamIndex]->time_base);
	}
	else
	{
		av_packet_rescale_ts(&packet,
			ifmt_ctx->streams[stream_index]->time_base,
			stream->dec_ctx->time_base);
	}

	ret = avcodec_send_packet(stream->dec_ctx, &packet);
	if (ret < 0)
	{
		av_log(nullptr, AV_LOG_ERROR, "Decoding failed\n");
		return ret;
	}

	if (packet.buf == nullptr)
	{
		av_packet_unref(&packet);
		return 0;
	}


	AVFrame* tmp_frame = nullptr;
	AVFrame* sw_frame = NULL;
	while (ret >= 0)
	{
		if (!(sw_frame = av_frame_alloc())) {
			fprintf(stderr, "Can not alloc frame\n");
			ret = AVERROR(ENOMEM);
			goto fail;
		}

		ret = avcodec_receive_frame(stream->dec_ctx, stream->dec_frame);
		if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN))
			break;
		if (ret < 0)
			goto fail;

		if (stream->dec_frame->format == hw_pix_fmt && (decoderHardware != "" && decoderHardware != "none")) {
			/* retrieve data from GPU to CPU */
			if ((ret = av_hwframe_transfer_data(sw_frame, stream->dec_frame, 0)) < 0)
			{
				fprintf(stderr, "Error transferring the data to system memory\n");
				goto fail;
			}
			tmp_frame = sw_frame;

			ret = av_frame_copy_props(tmp_frame, stream->dec_frame);
			if (ret < 0) {
				goto fail;
			}

			//tmp_frame->pts = positionMovie++;
		}

		else
		{
			tmp_frame = stream->dec_frame;

		}
		tmp_frame->pts = tmp_frame->best_effort_timestamp;


		if (isVideo)
		{
			if (videoCompressOption != nullptr)
				if (videoCompressOption->videoEffectParameter.effectEnable)
					VideoTreatment(tmp_frame, stream);

			VideoInfos(stream);

			nbFrameEncoded++;
		}


		ret = filter_encode_write_frame(tmp_frame, stream_index, m_dlgProgress, isVideo, write);
		if (ret < 0)
			goto fail;

	fail:
		av_frame_free(&sw_frame);
	}

	return ret;
}



AVDictionary* CFFmpegTranscodingPimpl::setEncoderParam(const AVCodecID& codec_id, AVCodecContext* pCodecCtx, const wxString& encoderName)
{

	// some formats want stream headers to be separate
	if (pCodecCtx->flags & AVFMT_GLOBALHEADER)
		pCodecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

	AVDictionary* param = nullptr;

	if (pCodecCtx->codec_id == AV_CODEC_ID_MPEG2VIDEO)
	{
		/* just for testing, we also add B frames */
		pCodecCtx->max_b_frames = 2;
	}
	if (pCodecCtx->codec_id == AV_CODEC_ID_MPEG1VIDEO)
	{
		/* needed to avoid using macroblocks in which some coeffs overflow
		   this doesnt happen with normal video, it just happens here as the
		   motion of the chroma plane doesnt match the luma plane */
		pCodecCtx->mb_decision = 2;
	}


	if (pCodecCtx->codec_id == AV_CODEC_ID_H264 && videoCompressOption->videoPreset != "" && encoderName == "")
	{
		av_dict_set(&param, "start_time_realtime", nullptr, 0);
		av_opt_set(pCodecCtx->priv_data, "preset", videoCompressOption->videoPreset, 0);
		av_opt_set(pCodecCtx->priv_data, "tune", "zerolatency", 0);
		av_opt_set(pCodecCtx->priv_data, "profile", videoCompressOption->encoder_profile, 0);
	}
	/*
	if (pCodecCtx->codec_id == AV_CODEC_ID_H265 && encoderName == ""  && videoCompressOption->videoPreset != "")
	{
		//preset: ultrafast, superfast, veryfast, faster, fast,
			//medium, slow, slower, veryslow, placebo
		av_opt_set(pCodecCtx->priv_data, "preset", videoCompressOption->videoPreset, 0);
		//tune: psnr, ssim, zerolatency, fastdecode
		//av_opt_set(pCodecCtx->priv_data, "tune", "zero-latency", 0);
		//profile: main, main10, mainstillpicture
		//av_opt_set(pCodecCtx->priv_data, "profile", videoCompressOption->encoder_profile, 0);
	}
	*/
	if (videoCompressOption->videoQualityOrBitRate == 0)
	{
		/* Average bitrate */
		pCodecCtx->bit_rate = 1000 * videoCompressOption->videoBitRate;
		// ffmpeg's mpeg2 encoder requires that the bit_rate_tolerance be >=
		// bitrate * fps
		pCodecCtx->bit_rate_tolerance = videoCompressOption->videoBitRate * av_q2d(pCodecCtx->framerate) + 1;

		//if ((codec_id == AV_CODEC_ID_H264 || codec_id == AV_CODEC_ID_H265) && encoderName == "nvenc")
		//	av_dict_set(&param, "rc", "vbr_hq", 0);

		if ((codec_id == AV_CODEC_ID_H264 || codec_id == AV_CODEC_ID_H265) && encoderName == "amf")
			av_dict_set(&param, "rc", "vbr_peak", 0);
	}
	else
	{
		if (codec_id == AV_CODEC_ID_VP8)
		{
			pCodecCtx->flags |= AV_CODEC_FLAG_QSCALE;
			pCodecCtx->global_quality = FF_QP2LAMBDA * videoCompressOption->videoCompressionValue + 0.5;
			pCodecCtx->bit_rate = static_cast<int64_t>(pCodecCtx->width) * pCodecCtx->height * pCodecCtx->framerate.num
				/ pCodecCtx->framerate.den;
		}
		else if (codec_id == AV_CODEC_ID_VP9)
		{
			// These settings produce better image quality than
			// what was previously used
			pCodecCtx->flags |= AV_CODEC_FLAG_QSCALE;
			pCodecCtx->global_quality = FF_QP2LAMBDA * videoCompressOption->videoCompressionValue + 0.5;

			char quality[7];
			snprintf(quality, 7, "%.2d", videoCompressOption->videoCompressionValue);
			av_dict_set(&param, "crf", quality, 0);
			//This value was chosen to make the bitrate high enough
			//for libvpx to "turn off" the maximum bitrate feature
			//that is normally applied to constant quality.
			pCodecCtx->bit_rate = static_cast<int64_t>(pCodecCtx->width) * pCodecCtx->height * pCodecCtx->framerate.num
				/ pCodecCtx->framerate.den;
			//hb_log("encavcodec: encoding at CQ %.2f", job->vquality);
		}
		else if ((codec_id == AV_CODEC_ID_H264 || codec_id == AV_CODEC_ID_H265) && encoderName == "nvenc")
		{
			char qualityI[7];
			char quality[7];
			char qualityB[7];

			double adjustedQualityI = videoCompressOption->videoCompressionValue - 2;
			double adjustedQualityB = videoCompressOption->videoCompressionValue + 2;
			if (adjustedQualityB > 51)
			{
				adjustedQualityB = 51;
			}

			if (adjustedQualityI < 0)
			{
				adjustedQualityI = 0;
			}

			snprintf(quality, 7, "%.2d", videoCompressOption->videoCompressionValue);
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

			snprintf(quality, 7, "%.2d", videoCompressOption->videoCompressionValue);
			snprintf(qualityB, 7, "%.2f", adjustedQualityB);

			if (adjustedQualityB > 51)
			{
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
			while (hb_h264_level_names[i] != nullptr)
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
			if (videoCompressOption->encoder_profile.Lower() == "main")
				av_dict_set(&param, "profile", "main", 0);
			else if (videoCompressOption->encoder_profile.Lower() == "main10")
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
			while (hb_h264_level_names[i] != nullptr)
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
			while (hb_h265_level_names[i] != nullptr)
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
			while (hb_h264_level_names[i] != nullptr)
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

	return param;
}



wxString CFFmpegTranscodingPimpl::GetCodecName(AVCodecID codec_type, const wxString& encoderHardware)
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
	}
	break;
	case AV_CODEC_ID_MPEG2VIDEO:
	{
		if (encoderHardware == "qsv")
			codec_name = "mpeg2_qsv";
		else
			codec_name = "mpeg2video";
	}
	break;
	case AV_CODEC_ID_VP8:
	{
		// hb_log("encavcodecInit: VP8 encoder");
		codec_name = "libvpx";
	}
	break;
	case AV_CODEC_ID_VP9:
	{
		if (encoderHardware == "qsv")
			codec_name = "vp9_qsv";
		else
			codec_name = "libvpx-vp9";
	}
	break;
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
	}
	break;
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
	}
	break;
	default:;
	}
	return codec_name;
}

AVCodecID CFFmpegTranscodingPimpl::GetCodecID(AVMediaType codec_type) const
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
			return AV_CODEC_ID_VP8;
		}
		if (videoCompressOption->videoCodec == "VP9")
		{
			return AV_CODEC_ID_VP9;
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


wxString CFFmpegTranscodingPimpl::GetCodecNameForEncoder(AVCodecID vcodec, const wxString& nameEncoder)
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

int CFFmpegTranscodingPimpl::write_packet(void* opaque, uint8_t* buf, int buf_size)
{
	auto dataOutput = static_cast<wxMemoryOutputStream*>(opaque);
	dataOutput->WriteAll(buf, buf_size);
	return buf_size;
}

void CFFmpegTranscodingPimpl::SetParamFromVideoCodec(AVCodecContext* c, AVCodecContext* pSourceCodecCtx)
{
	c->codec_type = AVMEDIA_TYPE_VIDEO;
	c->pix_fmt = AV_PIX_FMT_YUV420P;
	c->width = pSourceCodecCtx->width;
	c->height = pSourceCodecCtx->height;
	c->framerate = pSourceCodecCtx->framerate;
	c->time_base = pSourceCodecCtx->time_base;
	framerate = (c->framerate.num / c->framerate.den) * 1;
	c->bit_rate = 1000 * videoCompressOption->videoBitRate;
	c->gop_size = framerate;
	c->max_b_frames = 1;
	c->sample_aspect_ratio = pSourceCodecCtx->sample_aspect_ratio;

}

int CFFmpegTranscodingPimpl::open_output_file(const wxString& filename)
{
	AVStream* out_stream;
	AVStream* in_stream;
	AVCodecContext* dec_ctx, * enc_ctx;
	const AVCodec* encoder;
	int ret;
	wxString encoderHardware = "";
	unsigned int i;
	AVDictionary* av_opts = nullptr;
	AVCodecID VIDEO_CODEC = GetCodecID(AVMEDIA_TYPE_VIDEO);
	AVCodecID AUDIO_CODEC = GetCodecID(AVMEDIA_TYPE_AUDIO);
	ofmt_ctx = nullptr;

	//wxString filepath = filename;
	wxFileName filepath(filename);

	wxString extension = filepath.GetExt();


	if (extension == "mkv")
		avformat_alloc_output_context2(
			&ofmt_ctx, av_guess_format("matroska", CConvertUtility::ConvertToUTF8(filename), nullptr), "mkv",
			CConvertUtility::ConvertToUTF8(filename));
	else if (extension == "webm")
		avformat_alloc_output_context2(
			&ofmt_ctx, av_guess_format("webm", CConvertUtility::ConvertToUTF8(filename), nullptr), "webm",
			CConvertUtility::ConvertToUTF8(filename));
	else if (extension == "mpeg")
		avformat_alloc_output_context2(
			&ofmt_ctx, av_guess_format("mpeg", CConvertUtility::ConvertToUTF8(filename), nullptr), "mpeg",
			CConvertUtility::ConvertToUTF8(filename));
	else
		avformat_alloc_output_context2(&ofmt_ctx, nullptr, nullptr, CConvertUtility::ConvertToUTF8(filename));
	if (!ofmt_ctx)
	{
		av_log(nullptr, AV_LOG_ERROR, "Could not create output context\n");
		return AVERROR_UNKNOWN;
	}

	//
	AVStream* streamAudio = nullptr;
	AVStream* streamVideo = nullptr;
	int nbStream = 0;
	int outStream = 0;
	for (i = 0; i < ifmt_ctx->nb_streams; i++)
	{
		AVStream* stream = ifmt_ctx->streams[i];
		if (stream->codecpar->codec_id == AV_CODEC_ID_NONE)
			continue;

		streamCorrespondant.insert(std::make_pair(i, nbStream++));

		dec_ctx = stream_ctx[i].dec_ctx;

		out_stream = avformat_new_stream(ofmt_ctx, nullptr);
		if (!out_stream)
		{
			av_log(nullptr, AV_LOG_ERROR, "Failed allocating output stream\n");
			return AVERROR_UNKNOWN;
		}



		streamInNumberInOut[i] = outStream++;

		in_stream = ifmt_ctx->streams[i];

		if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO
			|| dec_ctx->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			/* in this example, we choose transcoding to same codec */

			if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				streamVideo = out_stream;
				enc_ctx = nullptr;

				width = dec_ctx->width;
				height = dec_ctx->height;


				if (videoCompressOption->videoHardware)
				{
					CRegardsConfigParam* config = CParamInit::getInstance();
					if (config != nullptr)
					{
						wxString encoderHardware = config->GetHardwareEncoder();
						if (encoderHardware != "")
							enc_ctx = OpenFFmpegEncoder(VIDEO_CODEC, dec_ctx, streamVideo, encoderHardware);

					}
					if (enc_ctx != nullptr)
					{
						for (int i = 0; i < sizeListEncoderHardware; i++)
						{
							enc_ctx = OpenFFmpegEncoder(VIDEO_CODEC, dec_ctx, streamVideo, listencoderHardware[i]);
							if (enc_ctx)
							{
								encoderHardware = listencoderHardware[i];
								break;
							}
						}
					}
				}

				if (!enc_ctx)
				{
					encoderHardware = ""; //MediaFoundation
					enc_ctx = OpenFFmpegEncoder(VIDEO_CODEC, dec_ctx, streamVideo, encoderHardware);
				}
			}
			else
			{
				encoder = avcodec_find_encoder(AUDIO_CODEC);
				enc_ctx = avcodec_alloc_context3(encoder);
				if (!enc_ctx)
				{
					av_log(nullptr, AV_LOG_FATAL, "Failed to allocate the encoder context\n");
					return AVERROR(ENOMEM);
				}
			}
			if (!enc_ctx)
			{
				av_log(nullptr, AV_LOG_FATAL, "Necessary encoder not found\n");
				return AVERROR_INVALIDDATA;
			}

			if (dec_ctx->codec_type == AVMEDIA_TYPE_AUDIO)
			{
				streamAudio = out_stream;
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
				if (ret < 0)
				{
					av_log(nullptr, AV_LOG_ERROR, "Cannot open video encoder for stream #%u\n", i);
					return ret;
				}
			}

			ret = avcodec_parameters_from_context(out_stream->codecpar, enc_ctx);
			if (ret < 0)
			{
				av_log(nullptr, AV_LOG_ERROR, "Failed to copy encoder parameters to output stream #%u\n", i);
				return ret;
			}

			//out_stream->time_base = enc_ctx->time_base;
			stream_ctx[i].enc_ctx = enc_ctx;
		}
		else if (dec_ctx->codec_type == AVMEDIA_TYPE_UNKNOWN)
		{
			av_log(nullptr, AV_LOG_FATAL, "Elementary stream #%d is of unknown type, cannot proceed\n", i);
			return AVERROR_INVALIDDATA;
		}
		else
		{
			/* if this stream must be remuxed */
			ret = avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar);
			if (ret < 0)
			{
				av_log(nullptr, AV_LOG_ERROR, "Copying parameters for stream #%u failed\n", i);
				return ret;
			}
			//out_stream->time_base = in_stream->time_base;
		}
	}





	//AVDictionaryEntry* rotate_tag = av_dict_get(ifmt_ctx->metadata, "rotate", NULL, 0);
	//if (rotate_tag != nullptr)
	//	av_dict_set(&ofmt_ctx->metadata, rotate_tag->key, rotate_tag->value, 0);

	av_dump_format(ofmt_ctx, 0, CConvertUtility::ConvertToUTF8(filename), 1);

	if (!(ofmt_ctx->oformat->flags & AVFMT_NOFILE))
	{
		ret = avio_open(&ofmt_ctx->pb, CConvertUtility::ConvertToUTF8(filename), AVIO_FLAG_WRITE);
		if (ret < 0)
		{
			av_log(nullptr, AV_LOG_ERROR, "Could not open output file '%s'",
				CConvertUtility::ConvertToUTF8(filename));
			return ret;
		}
	}

	av_dict_copy(&ofmt_ctx->metadata, ifmt_ctx->metadata, AV_DICT_DONT_OVERWRITE);
	set_rotation(ofmt_ctx, ifmt_ctx->streams[videoStreamIndex], streamAudio);

	/* init muxer, write output file header */
	ret = avformat_write_header(ofmt_ctx, nullptr);
	if (ret < 0)
	{
		av_log(nullptr, AV_LOG_ERROR, "Error occurred when opening output file\n");
		return ret;
	}



	return 0;
}


int CFFmpegTranscodingPimpl::encode_write_frame_withoutpos(AVFrame* filt_frame, unsigned int stream_index)
{
	StreamContext* stream = &stream_ctx[stream_index];
	int ret;
	AVPacket enc_pkt;

	av_log(nullptr, AV_LOG_INFO, "Encoding frame\n");
	/* encode filtered frame */
	enc_pkt.data = nullptr;
	enc_pkt.size = 0;
	av_init_packet(&enc_pkt);

	ret = avcodec_send_frame(stream->enc_ctx, filt_frame);

	if (ret >= 0)
	{
		while (ret >= 0)
		{
			ret = avcodec_receive_packet(stream->enc_ctx, &enc_pkt);

			if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			{
				ret = 0;
				break;
			}

			av_write_frame(ofmt_ctx, &enc_pkt);
			//av_packet_unref(&enc_pkt);
		}
	}

	av_packet_unref(&enc_pkt);

	return ret;
}

int CFFmpegTranscodingPimpl::encode_write_frame(AVFrame* filt_frame, unsigned int stream_index)
{
	StreamContext* stream = &stream_ctx[stream_index];
	int ret;
	AVPacket enc_pkt;

	av_log(nullptr, AV_LOG_INFO, "Encoding frame\n");
	/* encode filtered frame */
	enc_pkt.data = nullptr;
	enc_pkt.size = 0;
	av_init_packet(&enc_pkt);

	ret = avcodec_send_frame(stream->enc_ctx, filt_frame);

	if (ret >= 0)
	{
		while (ret >= 0)
		{
			ret = avcodec_receive_packet(stream->enc_ctx, &enc_pkt);

			if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			{
				ret = 0;
				break;
			}
			/* prepare packet for muxing */
			enc_pkt.stream_index = stream_index;

			int outputIndex = streamCorrespondant[stream_index];

			if (ofmt_ctx->streams[outputIndex]->time_base.den == ifmt_ctx->streams[outputIndex]->time_base.den
				&& ofmt_ctx->streams[outputIndex]->time_base.num == ifmt_ctx->streams[outputIndex]->time_base.num)
			{
				av_packet_rescale_ts(&enc_pkt,
					ifmt_ctx->streams[outputIndex]->time_base,
					ofmt_ctx->streams[outputIndex]->time_base);
			}
			else
			{
				av_packet_rescale_ts(&enc_pkt,
					stream->enc_ctx->time_base,
					ofmt_ctx->streams[outputIndex]->time_base);
			}

			if (enc_pkt.duration > 0)
				enc_pkt.duration = av_rescale_q(enc_pkt.duration, ofmt_ctx->streams[outputIndex]->time_base,
					stream->enc_ctx->time_base);

			av_log(nullptr, AV_LOG_DEBUG, "Muxing frame\n");
			/* mux encoded frame */
			av_write_frame(ofmt_ctx, &enc_pkt);
			//av_packet_unref(&enc_pkt);
		}
	}

	av_packet_unref(&enc_pkt);

	return ret;
}

int CFFmpegTranscodingPimpl::filter_encode_write_frame(AVFrame* frame, unsigned int stream_index, CompressVideo* m_dlgProgress, const int& isvideo, const bool& write)
{
	int ret = 0;

	if (isvideo && m_dlgProgress != nullptr)
	{
		SetFrameData(frame, m_dlgProgress);
	}

	if (write)
		ret = encode_write_frame(frame, stream_index);

	return ret;
}


void CFFmpegTranscodingPimpl::SetFrameData(AVFrame* src_frame, CompressVideo* m_dlgProgress)
{
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
		bmp = GetBitmapRGBA(src_frame);
		//bmp->VertFlipBuf();
		muFrame.unlock();
		bitmapShow = new thread(DisplayPreview, this);
	}
}


int CFFmpegTranscodingPimpl::flush_encoder(unsigned int stream_index)
{
	if (stream_ctx[stream_index].enc_ctx != nullptr)
	{

		av_log(nullptr, AV_LOG_INFO, "Flushing stream #%u encoder\n", stream_index);
		if (!(stream_ctx[stream_index].enc_ctx->codec->capabilities &
			AV_CODEC_CAP_DELAY))
			return 0;

		return encode_write_frame(nullptr, stream_index);
	}
	return -1;
}

int CFFmpegTranscodingPimpl::OpenFile(const wxString& input, const wxString& output)
{
	int ret;

	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
	{
		decoderHardware = config->GetHardwareDecoder();
	}

	if (decoderHardware == "" || decoderHardware == "none")
	{
		if ((ret = open_input_file(input)) < 0)
			return ret;
	}
	else if ((ret = open_input_file(input, decoderHardware)) < 0)
		return ret;
	if ((ret = open_output_file(output)) < 0)
		return ret;



	return ret;
}



CRegardsBitmap* CFFmpegTranscodingPimpl::GetBitmapRGBA(AVFrame* tmp_frame)
{
	CRegardsBitmap* bitmapData = new CRegardsBitmap(tmp_frame->width, tmp_frame->height);

	if (localContext == nullptr)
	{
		localContext = sws_alloc_context();

		av_opt_set_int(localContext, "srcw", tmp_frame->width, 0);
		av_opt_set_int(localContext, "srch", tmp_frame->height, 0);
		av_opt_set_int(localContext, "src_format", tmp_frame->format, 0);
		av_opt_set_int(localContext, "dstw", tmp_frame->width, 0);
		av_opt_set_int(localContext, "dsth", tmp_frame->height, 0);
		av_opt_set_int(localContext, "dst_format", AV_PIX_FMT_BGRA, 0);
		av_opt_set_int(localContext, "sws_flags", SWS_FAST_BILINEAR, 0);

		if (sws_init_context(localContext, nullptr, nullptr) < 0)
		{
			sws_freeContext(localContext);
			throw std::logic_error("Failed to initialise scale context");
		}
	}

	int numBytes = av_image_get_buffer_size(AV_PIX_FMT_BGRA, tmp_frame->width, tmp_frame->height, 16);
	if (numBytes != bitmapData->GetBitmapSize())
	{
		bitmapData->SetBitmap(tmp_frame->width, tmp_frame->height);
	}

	uint8_t* convertedFrameBuffer = bitmapData->GetPtBitmap();
	int linesize = tmp_frame->width * 4;

	sws_scale(localContext, tmp_frame->data, tmp_frame->linesize, 0, tmp_frame->height,
		&convertedFrameBuffer, &linesize);

	return bitmapData;
}

void CFFmpegTranscodingPimpl::VideoTreatment(AVFrame*& tmp_frame, StreamContext* stream)
{
	//bool decodeBitmap = false;
	cv::Mat mat;

	if (IsSupportOpenCL())
	{
		cv::UMat bgr;
		int nWidth = tmp_frame->width;
		int nHeight = tmp_frame->height;
		COpenCLEffectVideo openclEffectVideo;

		if (tmp_frame->format == AV_PIX_FMT_NV12)
		{
			try
			{
				int sizeData = (nHeight + nHeight / 2) * tmp_frame->linesize[0];
				if (sizeData != sizesrc && src != nullptr)
				{
					delete[] src;
					src = nullptr;
				}

				if (src == nullptr)
				{
					src = new uint8_t[sizeData];
					sizesrc = sizeData;
				}

				int size = nHeight * tmp_frame->linesize[0];
				memcpy(src, tmp_frame->data[0], size);
				memcpy(src + size, tmp_frame->data[1], (tmp_frame->linesize[0] * (nHeight / 2)));
				cv::Mat yuv = cv::Mat(nHeight + nHeight / 2, tmp_frame->linesize[0], CV_8UC1, src);
				openclEffectVideo.SetNV12(yuv, tmp_frame->linesize[0], nWidth, nHeight);


			}
			catch (cv::Exception& e)
			{
				const char* err_msg = e.what();
				std::cout << "exception caught: " << err_msg << std::endl;
				std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
			}


		}
		else if (tmp_frame->format == AV_PIX_FMT_YUV420P)
		{
			try
			{
				cv::Mat y = cv::Mat(cv::Size(tmp_frame->linesize[0], nHeight), CV_8UC1, tmp_frame->data[0]);
				cv::Mat u = cv::Mat(cv::Size(tmp_frame->linesize[1], nHeight / 2), CV_8UC1, tmp_frame->data[1]);
				cv::Mat v = cv::Mat(cv::Size(tmp_frame->linesize[2], nHeight / 2), CV_8UC1, tmp_frame->data[2]);
				openclEffectVideo.SetYUV420P(y, u, v, tmp_frame->linesize[0], nWidth, nHeight);
			}
			catch (cv::Exception& e)
			{
				const char* err_msg = e.what();
				std::cout << "exception caught: " << err_msg << std::endl;
				std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
			}

		}



		bool stabilizeFrame = videoCompressOption->videoEffectParameter.stabilizeVideo;
		bool correctedContrast = videoCompressOption->videoEffectParameter.autoConstrast;

		if (stabilizeFrame && openCVStabilization == nullptr)
			openCVStabilization = new Regards::OpenCV::COpenCVStabilization(videoCompressOption->videoEffectParameter.stabilizeImageBuffere);

		//COpenCLEffectVideo openclEffectVideo;
		//openclEffectVideo.SetMatrix(bgr);

		if (stabilizeFrame || correctedContrast)
		{
			openclEffectVideo.ApplyOpenCVEffect(&videoCompressOption->videoEffectParameter, openCVStabilization);
		}

		openclEffectVideo.ApplyVideoEffect(&videoCompressOption->videoEffectParameter);

		if (dst_hardware == nullptr)
		{
			dst_hardware = av_frame_alloc();
			dst_hardware->format = AV_PIX_FMT_YUV420P;
			dst_hardware->width = stream->dec_frame->width;
			dst_hardware->height = stream->dec_frame->height;
			av_image_alloc(dst_hardware->data, dst_hardware->linesize, tmp_frame->width, tmp_frame->height,
				AV_PIX_FMT_YUV420P, 1);
		}
		av_frame_copy_props(dst_hardware, tmp_frame);

		openclEffectVideo.GetYUV420P(dst_hardware->data[0], dst_hardware->data[1], dst_hardware->data[2], tmp_frame->width, tmp_frame->height);

		tmp_frame = dst_hardware;

		nbFrame++;
	}
	else
	{
		CRegardsBitmap* bitmap = GetBitmapRGBA(tmp_frame);
		mat = ApplyProcess(bitmap);
		delete bitmap;


		if (dst_hardware == nullptr)
		{
			dst_hardware = av_frame_alloc();

			av_opt_set_int(scaleContext, "srcw", tmp_frame->width, 0);
			av_opt_set_int(scaleContext, "srch", tmp_frame->height, 0);
			av_opt_set_int(scaleContext, "src_format", AV_PIX_FMT_BGRA, 0);
			av_opt_set_int(scaleContext, "dstw", tmp_frame->width, 0);
			av_opt_set_int(scaleContext, "dsth", tmp_frame->height, 0);
			av_opt_set_int(scaleContext, "dst_format", AV_PIX_FMT_YUV420P, 0);
			av_opt_set_int(scaleContext, "sws_flags", SWS_FAST_BILINEAR, 0);

			if (sws_init_context(scaleContext, nullptr, nullptr) < 0)
			{
				sws_freeContext(scaleContext);
				throw std::logic_error("Failed to initialise scale context");
			}


			dst_hardware->format = AV_PIX_FMT_YUV420P;
			dst_hardware->width = stream->dec_frame->width;
			dst_hardware->height = stream->dec_frame->height;
			av_image_alloc(dst_hardware->data, dst_hardware->linesize, tmp_frame->width, tmp_frame->height,
				AV_PIX_FMT_YUV420P, 1);
		}
		av_frame_copy_props(dst_hardware, tmp_frame);


		int linesize = mat.cols * 4;

		sws_scale(scaleContext, &mat.data, &linesize, 0, mat.rows,
			dst_hardware->data, dst_hardware->linesize);

		tmp_frame = dst_hardware;

		nbFrame++;
	}


}

void CFFmpegTranscodingPimpl::VideoInfos(StreamContext* stream)
{
	end = std::chrono::steady_clock::now();

	if (first_frame)
	{
		first_frame = false;
	}

}


cv::Mat CFFmpegTranscodingPimpl::ApplyProcess(CRegardsBitmap* bitmap)
{
	bool stabilizeFrame = videoCompressOption->videoEffectParameter.stabilizeVideo;
	bool correctedContrast = videoCompressOption->videoEffectParameter.autoConstrast;
	cv::Mat mat = bitmap->GetMatrix();

	if (stabilizeFrame && openCVStabilization == nullptr)
		openCVStabilization = new Regards::OpenCV::COpenCVStabilization(videoCompressOption->videoEffectParameter.stabilizeImageBuffere);

	if (IsSupportOpenCL())
	{

		COpenCLEffectVideo openclEffectVideo;
		openclEffectVideo.SetMatrix(mat);

		if (stabilizeFrame || correctedContrast)
		{
			openclEffectVideo.ApplyOpenCVEffect(&videoCompressOption->videoEffectParameter, openCVStabilization);
		}

		openclEffectVideo.ApplyVideoEffect(&videoCompressOption->videoEffectParameter);

		mat = openclEffectVideo.GetMatrix();
	}
	else
	{

		bool frameStabilized = false;

		if (videoCompressOption->videoEffectParameter.stabilizeVideo)
		{
			openCVStabilization->SetNbFrameBuffer(videoCompressOption->videoEffectParameter.stabilizeImageBuffere);

			if (openCVStabilization->GetNbFrameBuffer() == 0)
			{
				openCVStabilization->BufferFrame(mat);
			}
			else
			{
				frameStabilized = true;
				openCVStabilization->AddFrame(mat);
			}

			if (frameStabilized)
			{
				openCVStabilization->CorrectFrame(mat);
			}

			bitmap->SetMatrix(mat);
		}

		CImageLoadingFormat image;
		image.SetPicture(bitmap);
		const CRgbaquad colorLocal;
		CFiltreEffetCPU filtre(colorLocal, &image);

		if (videoCompressOption != nullptr)
		{
			if (correctedContrast)
				filtre.BrightnessAndContrastAuto(1);

			if (videoCompressOption->videoEffectParameter.effectEnable)
			{
				if (videoCompressOption->videoEffectParameter.ColorBoostEnable)
				{
					filtre.RGBFilter(videoCompressOption->videoEffectParameter.color_boost[0],
						videoCompressOption->videoEffectParameter.color_boost[1],
						videoCompressOption->videoEffectParameter.color_boost[2]);
				}
				if (videoCompressOption->videoEffectParameter.bandcEnable)
				{
					filtre.BrightnessAndContrast(videoCompressOption->videoEffectParameter.brightness,
						videoCompressOption->videoEffectParameter.contrast);
				}
				if (videoCompressOption->videoEffectParameter.SharpenEnable)
				{
					filtre.SharpenMasking(videoCompressOption->videoEffectParameter.sharpness);
				}
				if (videoCompressOption->videoEffectParameter.denoiseEnable)
				{
					filtre.HQDn3D(videoCompressOption->videoEffectParameter.denoisingLevel, 4, 3, 3);
				}
				if (videoCompressOption->videoEffectParameter.sepiaEnable)
				{
					filtre.Sepia();
				}
				if (videoCompressOption->videoEffectParameter.grayEnable)
				{
					filtre.NiveauDeGris();
				}
				if (videoCompressOption->videoEffectParameter.filmgrainenable)
				{
					filtre.Noise();
				}
				if (videoCompressOption->videoEffectParameter.grayEnable)
				{
					filtre.NiveauDeGris();
				}
			}
		}


		CRegardsBitmap* bitmap = filtre.GetBitmap(true);
		mat = bitmap->GetMatrix();
		delete bitmap;
	}
	return mat;
}

static void encode(AVCodecContext* enc_ctx, AVFrame* frame, AVPacket* pkt,
	FILE* outfile)
{
	int ret;
	ret = avcodec_send_frame(enc_ctx, frame);
	if (ret < 0) {
		fprintf(stderr, "Error sending a frame for encoding\n");
		return;
	}

	while (ret >= 0) {
		ret = avcodec_receive_packet(enc_ctx, pkt);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			return;
		else if (ret < 0) {
			fprintf(stderr, "Error during encoding\n");
			exit(1);
		}
		fwrite(pkt->data, 1, pkt->size, outfile);
		av_packet_unref(pkt);
	}
}

int CFFmpegTranscodingPimpl::ProcessEncodeOneFrameFile(AVFrame* dst, const int64_t& timeInSeconds)
{
	int ret = 0;
	int stream_index = 0;

	//bool firstPos = true;
	double fps = 0;
	
	{
		fps = capture->GetFps();
		width = capture->GetWidth();
		height = capture->GetHeight();
		bool success = capture->SeekToPos(timeInSeconds);
		frameOutput = capture->GetVideoFrame(false);
		cvtColor(frameOutput, frameOutput, cv::COLOR_RGB2BGRA);
	}
	
	bitmapOut->SetMatrix(frameOutput);
	bitmapOut->ApplyRotation(rotation);
	bitmapOut->VertFlipBuf();
	frameOutput = ApplyProcess(bitmapOut);

	AVFrame* frame = av_frame_alloc();
	if (!frame) {
		fprintf(stderr, "Could not allocate video frame\n");
		exit(1);
	}

	StreamContext* stream = &stream_ctx[videoStreamIndex];
	frame->format = AV_PIX_FMT_YUV420P;
	frame->width = stream->dec_ctx->width;
	frame->height = stream->dec_ctx->height;

	/* the image can be allocated by any means and av_image_alloc() is
	 * just the most convenient way if av_malloc() is to be used */
	ret = av_image_alloc(frame->data, frame->linesize, frame->width, frame->height, AV_PIX_FMT_YUV420P, 32);
	if (ret < 0) {
		fprintf(stderr, "Could not allocate raw picture buffer\n");
		exit(1);
	}

	if (dst_hardware == nullptr)
	{
		//dst_hardware = av_frame_alloc();

		av_opt_set_int(scaleContext, "srcw", frame->width, 0);
		av_opt_set_int(scaleContext, "srch", frame->height, 0);
		av_opt_set_int(scaleContext, "src_format", AV_PIX_FMT_BGRA, 0);
		av_opt_set_int(scaleContext, "dstw", frame->width, 0);
		av_opt_set_int(scaleContext, "dsth", frame->height, 0);
		av_opt_set_int(scaleContext, "dst_format", AV_PIX_FMT_YUV420P, 0);
		av_opt_set_int(scaleContext, "sws_flags", SWS_FAST_BILINEAR, 0);

		if (sws_init_context(scaleContext, nullptr, nullptr) < 0)
		{
			sws_freeContext(scaleContext);
			throw std::logic_error("Failed to initialise scale context");
		}
	}


	//int got_output = 0;
	int linesize = frameOutput.cols * 4;

	sws_scale(scaleContext, &frameOutput.data, &linesize, 0, frameOutput.rows,
		frame->data, frame->linesize);

	//ret = av_frame_get_buffer(frame, 0);

	for (int i = 0; i < fps; i++) {
		frame->pts = i;
		ret = encode_write_frame(frame, stream_index);
		if (ret < 0)
			break;
	}

	// flush encoder
	while (true)
	{
		ret = flush_encoder(0);
		if (ret < 0)
		{
			break;
		}
	}

	ret = av_write_trailer(ofmt_ctx);

	av_freep(&frame->data[0]);
	av_frame_free(&frame);

	return ret;
}


int CFFmpegTranscodingPimpl::ProcessEncodeFile(AVFrame* dst)
{
	int ret = 0;
	int stream_index = 0;
	int positionMovie = 0;

	/* read all packets */
	while (m_dlgProgress->IsOk())
	{
		if ((ret = av_read_frame(ifmt_ctx, &packet)) < 0)
			break;

		bool isVideo = false;
		stream_index = packet.stream_index;


		av_log(nullptr, AV_LOG_DEBUG, "Demuxer gave frame of stream_index %u\n",
			stream_index);

		AVStream* st = ifmt_ctx->streams[packet.stream_index];
		switch (st->codecpar->codec_type)
		{
		case AVMEDIA_TYPE_AUDIO:
			break;

		case AVMEDIA_TYPE_VIDEO:
			//printf("video \n");
			isVideo = true;

			break;
		}

		if (st->codecpar->codec_id != AV_CODEC_ID_NONE)
		{
			bool copyDirectPacket = false;
			bool showPreviewFrame = false;
			if (videoCompressOption->audioDirectCopy && !isVideo)
			{
				copyDirectPacket = true;
			}
			if (videoCompressOption->videoDirectCopy && isVideo)
			{
				copyDirectPacket = true;

				bool threadEnd = false;
				muEnding.lock();
				threadEnd = isend;
				muEnding.unlock();

				nbFrameEncoded++;
				end = std::chrono::steady_clock::now();

				if (threadEnd)
					showPreviewFrame = true;
			}

			if (showPreviewFrame)
			{
				ret = EncodeFrame(stream_index, positionMovie, isVideo, false);
			}

			if (!showPreviewFrame && !copyDirectPacket)
			{
				ret = EncodeFrame(stream_index, positionMovie, isVideo, true);
			}
			else
			{
				/* remux this frame without reencoding */
				int outStreamIndex = streamInNumberInOut[stream_index];
				av_packet_rescale_ts(&packet,
					ifmt_ctx->streams[stream_index]->time_base,
					ofmt_ctx->streams[outStreamIndex]->time_base);

				ret = av_interleaved_write_frame(ofmt_ctx, &packet);
				if (ret < 0)
					return ret;
			}
			int outStreamIndex = streamInNumberInOut[stream_index];
			printf("decoder -> type:video "
				"pkt_pts:%s pkt_pts_time:%s pkt_dts:%s pkt_dts_time:%s\n",
				av_ts2str(packet.pts), av_ts2timestr(packet.pts, &ifmt_ctx->streams[stream_index]->time_base),
				av_ts2str(packet.dts), av_ts2timestr(packet.dts, &ifmt_ctx->streams[stream_index]->time_base));

			printf("encoder -> type:video "
				"pkt_pts:%s pkt_pts_time:%s pkt_dts:%s pkt_dts_time:%s\n",
				av_ts2str(packet.pts), av_ts2timestr(packet.pts, &ofmt_ctx->streams[outStreamIndex]->time_base),
				av_ts2str(packet.dts), av_ts2timestr(packet.dts, &ofmt_ctx->streams[outStreamIndex]->time_base));
		}

		av_packet_unref(&packet);
	}

	/* flush filters and encoders */
	for (int i = 0; i < ifmt_ctx->nb_streams; i++)
	{
		/* flush filter */
		AVStream* st = ifmt_ctx->streams[packet.stream_index];
		if (st->codecpar->codec_id == AV_CODEC_ID_NONE)
			continue;

#ifdef USE_FILTER
		if (!filter_ctx[i].filter_graph)
			continue;

		ret = filter_encode_write_frame(NULL, i, m_dlgProgress, 0);
		if (ret < 0) {
			av_log(NULL, AV_LOG_ERROR, "Flushing filter failed\n");
			return ret;
		}
#endif
		// flush encoder
		while (true)
		{
			ret = flush_encoder(i);
			if (ret < 0)
			{
				break;
			}
		}
	}

	ret = av_write_trailer(ofmt_ctx);
	return ret;
}

int CFFmpegTranscodingPimpl::EncodeOneFrame(CompressVideo* m_dlgProgress, const wxString& input,
	const wxString& output, const long& time,
	CVideoOptionCompress* videoCompressOption)
{
	int ret;
	this->m_dlgProgress = m_dlgProgress;
	totalFrame = 0;
	encodeOneFrame = true;
	cleanPacket = false;
	this->videoCompressOption = videoCompressOption;
	showpreview = false;
	this->outputFile = output;
	input_file = input;

	if (capture != nullptr)
		delete capture;

	capture = new CVideoPlayer(input_file);
	if (!capture->isOpened())
		throw "Error when reading steam_avi";

	rotate = capture->GetOrientation();

	orientation = rotate;

	orientation = 360 - orientation;

	/*
	if ((ret = OpenFile(input, output)) < 0)
		return ret;

	*/
	cleanPacket = true;
	begin = std::chrono::steady_clock::now();

	//ret = ProcessEncodeOneFrameFile(dst, time);

	EncodeOneFrameFFmpeg(output, dst, time);

	return ret;
}



int CFFmpegTranscodingPimpl::EncodeFile(const wxString& input, const wxString& output, CompressVideo* m_dlgProgress,
	CVideoOptionCompress* videoCompressOption)
{
	int ret;
	this->m_dlgProgress = m_dlgProgress;
	//unsigned int stream_index;
	//unsigned int i;
	input_file = input;
	totalFrame = 0;
	encodeOneFrame = false;
	//bool first = true;
	cleanPacket = false;
	this->videoCompressOption = videoCompressOption;

	if (capture != nullptr)
		delete capture;

	capture = new CVideoPlayer(input_file);
	if (!capture->isOpened())
		throw "Error when reading steam_avi";


	{
		rotate = capture->GetOrientation();
		fps = capture->GetFps();
		totalFrame = capture->GetTotalFrame();
		width = capture->GetWidth();
		height = capture->GetHeight();
		duration = capture->GetDuration();
	}

	if ((ret = OpenFile(input, output)) < 0)
		return ret;

	cleanPacket = true;
	begin = std::chrono::steady_clock::now();


	ret = ProcessEncodeFile(dst);
	return ret;
}

void CFFmpegTranscodingPimpl::Release()
{
	if (ifmt_ctx != nullptr)
	{
		for (int i = 0; i < ifmt_ctx->nb_streams; i++)
		{
			if (stream_ctx[i].dec_ctx != nullptr)
				avcodec_free_context(&stream_ctx[i].dec_ctx);
			if (ofmt_ctx && ofmt_ctx->nb_streams > i && ofmt_ctx->streams[i] && stream_ctx[i].enc_ctx)
			{
				if (stream_ctx[i].enc_ctx != nullptr)
				{
					avcodec_close(stream_ctx[i].enc_ctx);
					avcodec_free_context(&stream_ctx[i].enc_ctx);
				}
			}

			if (stream_ctx[i].dec_frame != nullptr)
			{
				av_frame_free(&stream_ctx[i].dec_frame);
			}
		}

		av_free(stream_ctx);
		avformat_close_input(&ifmt_ctx);


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


void CFFmpegTranscodingPimpl::EncodeOneFrame(AVCodecContext* enc_ctx, AVFrame* frame, FILE* outfile)
{
	int ret;
	AVPacket enc_pkt;
	enc_pkt.data = nullptr;
	enc_pkt.size = 0;
	av_init_packet(&enc_pkt);

	ret = avcodec_send_frame(enc_ctx, frame);
	if (ret < 0) {
		exit(1);
	}

	while (ret >= 0) {
		ret = avcodec_receive_packet(enc_ctx, &enc_pkt);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			return;
		else if (ret < 0) {
			fprintf(stderr, "Error during encoding\n");
			return;
		}

		fwrite(enc_pkt.data, 1, enc_pkt.size, outfile);

	}

	av_packet_unref(&enc_pkt);
}

AVCodecContext* CFFmpegTranscodingPimpl::OpenFFmpegEncoder(AVCodecID codec_id, AVCodecContext* pSourceCodecCtx, AVStream* streamVideo, wxString encoderName)
{
	AVCodecContext* c = nullptr;
	wxString encoderHardName = "";
	const AVCodec* p_codec;

	if (encoderName != "")
	{
		encoderHardName = GetCodecName(codec_id, encoderName);
		p_codec = avcodec_find_encoder_by_name(encoderHardName);
	}
	else
	{
		p_codec = avcodec_find_encoder(codec_id);
	}

	if (p_codec != nullptr)
	{
		c = avcodec_alloc_context3(p_codec);
		if (pSourceCodecCtx != nullptr)
		{
			// Set things in context that we will allow the user to
			// override with advanced settings.
			//AVRational time_base_from_framerate;
			//time_base_from_framerate.num = pSourceCodecCtx->framerate.den;
			//time_base_from_framerate.den = pSourceCodecCtx->framerate.num;
			framerate = (pSourceCodecCtx->framerate.num / pSourceCodecCtx->framerate.den) * 1;
			c->codec_id = codec_id;
			c->codec_type = AVMEDIA_TYPE_VIDEO;
			c->pix_fmt = AV_PIX_FMT_YUV420P;
			c->width = pSourceCodecCtx->width;
			c->height = pSourceCodecCtx->height;
			c->framerate = pSourceCodecCtx->framerate;
			c->time_base = pSourceCodecCtx->time_base;
			c->bit_rate = videoCompressOption->videoBitRate;
			c->gop_size = framerate;
			c->max_b_frames = 0;
			c->sample_aspect_ratio = pSourceCodecCtx->sample_aspect_ratio;

			if (videoCompressOption->videoQualityOrBitRate == 0)
				c->compression_level = videoCompressOption->videoCompressionValue;
		}
		else
		{
			c->codec_type = AVMEDIA_TYPE_VIDEO;
			c->pix_fmt = AV_PIX_FMT_YUV420P;
			c->width = width;
			c->height = height;
			c->time_base = { 1,  (int)fps };
			c->framerate = { (int)fps, 1 };
			c->bit_rate = 1000 * videoCompressOption->videoBitRate;
			c->gop_size = fps;
			c->max_b_frames = 0;
			if (videoCompressOption->videoQualityOrBitRate == 0)
				c->compression_level = videoCompressOption->videoCompressionValue;
		}
		AVDictionary* param = setEncoderParam(codec_id, c, encoderHardName);

		if (rotate != 0 && streamVideo != nullptr)
		{
			uint8_t* sd = av_stream_new_side_data(streamVideo, AV_PKT_DATA_DISPLAYMATRIX,
				sizeof(int32_t) * 9);
			if (sd)
				av_display_rotation_set((int32_t*)sd, rotate);
		}

		const int ret = avcodec_open2(c, p_codec, &param);
		if (ret < 0)
		{

			char str_err[256];
			if (av_strerror(ret, str_err, 256) == 0)
			{
				printf("Error (%s) returned from encoded video", str_err);
			}

			avcodec_close(c);
			avcodec_free_context(&c);
			c = nullptr;
		}
	}
	return c;
}
CRegardsBitmap * CFFmpegTranscodingPimpl::GetFrameOutput()
{
	return bitmapOut;
}

int CFFmpegTranscodingPimpl::EncodeOneFrameFFmpeg(const char* filename, AVFrame* dst, const int64_t& timeInSeconds)
{
	cv::Mat decodeFrame;
	AVCodecID codec_name;
	//const AVCodec* codec;
	AVCodecContext* c = NULL;
	int i, ret, x, y;
	FILE* f;
	AVFrame* frame;
	uint8_t endcode[] = { 0, 0, 1, 0xb7 };

	int stream_index = 0;
	try
	{
		//bool firstPos = true;
		//double fps = 0;
		
		{
			fps = capture->GetFps();
			width = capture->GetWidth();
			height = capture->GetHeight();
			bool success = capture->SeekToPos(timeInSeconds);
			decodeFrame = capture->GetVideoFrame(false);
			//cv::imwrite("d:\\test.jpg", decodeFrame);
			cvtColor(decodeFrame, decodeFrame, cv::COLOR_RGB2BGRA);
		}


		bitmapOut->SetMatrix(decodeFrame);
		bitmapOut->ApplyRotation(rotation);
		bitmapOut->VertFlipBuf();
		frameOutput = ApplyProcess(bitmapOut);

		//*bitmapOut = bitmap;

		codec_name = GetCodecID(AVMEDIA_TYPE_VIDEO);

		bool isSuccess = false;
		wxString encoderHardware = "";

		if (videoCompressOption->videoHardware)
		{
			for (int i = 0; i < sizeListEncoderHardware; i++)
			{
				c = OpenFFmpegEncoder(codec_name, nullptr, nullptr, listencoderHardware[i]);
				if (c)
				{
					encoderHardware = listencoderHardware[i];
					break;
				}
			}
		}

		if (!c)
		{
			c = OpenFFmpegEncoder(codec_name, nullptr, nullptr, encoderHardware);
		}


		if (!c)
			throw "can't open";

		f = fopen(filename, "wb");
		if (!f) {
			fprintf(stderr, "Could not open %s\n", filename);
			throw "can't open";
		}

		frame = av_frame_alloc();
		if (!frame) {
			fprintf(stderr, "Could not allocate video frame\n");
			throw "can't alloc";
		}
		frame->format = c->pix_fmt;
		frame->width = c->width;
		frame->height = c->height;

		ret = av_frame_get_buffer(frame, 0);
		if (ret < 0) {
			fprintf(stderr, "Could not allocate the video frame data\n");
			throw "can't alloc";
		}

		if (dst_hardware == nullptr)
		{
			//dst_hardware = av_frame_alloc();

			av_opt_set_int(scaleContext, "srcw", frame->width, 0);
			av_opt_set_int(scaleContext, "srch", frame->height, 0);
			av_opt_set_int(scaleContext, "src_format", AV_PIX_FMT_BGRA, 0);
			av_opt_set_int(scaleContext, "dstw", frame->width, 0);
			av_opt_set_int(scaleContext, "dsth", frame->height, 0);
			av_opt_set_int(scaleContext, "dst_format", AV_PIX_FMT_YUV420P, 0);
			av_opt_set_int(scaleContext, "sws_flags", SWS_FAST_BILINEAR, 0);

			if (sws_init_context(scaleContext, nullptr, nullptr) < 0)
			{
				sws_freeContext(scaleContext);
				throw std::logic_error("Failed to initialise scale context");
			}
		}



		//int got_output = 0;
		int linesize = frameOutput.cols * 4;


		sws_scale(scaleContext, &frameOutput.data, &linesize, 0, frameOutput.rows,
			frame->data, frame->linesize);



		/* encode 1 second of video */
		for (i = 0; i < fps; i++) {


			frame->pts = i;

			/* encode the image */
			EncodeOneFrame(c, frame, f);
		}

		/* flush the encoder */
		EncodeOneFrame(c, NULL, f);

		/* Add sequence end code to have a real MPEG file.
		   It makes only sense because this tiny examples writes packets
		   directly. This is called "elementary stream" and only works for some
		   codecs. To create a valid file, you usually need to write packets
		   into a proper file format or protocol; see muxing.c.
		 */
		//if (codec->id == AV_CODEC_ID_MPEG1VIDEO || codec->id == AV_CODEC_ID_MPEG2VIDEO)
		//	fwrite(endcode, 1, sizeof(endcode), f);


		avcodec_free_context(&c);
		av_frame_free(&frame);

		fclose(f);

	}
	catch (...)
	{

	}

	return 0;
}