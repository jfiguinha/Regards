// ReSharper disable All
#include <header.h>
#include <new>
#include "ffmfcpimpl.h"
#include <WindowMain.h>
#include "DataAVFrame.h"
#include <ConvertUtility.h>
#include <RGBAQuad.h>
#include <RegardsConfigParam.h>
#include <OpenCLEffectVideo.h>
#include <ParamInit.h>
#include <MediaInfo.h>
#include <appcontext.h>
extern AppContext application_context;

using namespace std;
using namespace Regards::OpenCL;

#ifdef WIN32
wxString listHardware[] = { "cuda", "qsv", "d3d11va", "dxva2", "opencl" };
int sizeList = 5;
#elif defined(__APPLE__)
wxString listHardware[] = { "videotoolbox", "opencl" };
int sizeList = 2;
#else
wxString listHardware[] = { "vdpau", "cuda", "vaapi", "opencl", "qsv" };
int sizeList = 5;
#endif

std::atomic_bool CFFmfcPimpl::exit_video{ false };

#define HW_DEFAULT_SW_FORMAT    AV_PIX_FMT_NV12
//#define INTERPOLATION_METHOD SWS_FAST_BILINEAR

//Calcul du pourcentage
using namespace Regards::Window;
CVideoControlInterface* CFFmfcPimpl::dlg = nullptr;
AVPixelFormat CFFmfcPimpl::hw_pix_fmt;
/* polls for possible required screen refresh at least this often, should be less than 1/fps */
#define REFRESH_RATE 0.01
static int filter_nbthreads = 0;
static char* afilters = NULL;
static int interpolation_method = SWS_BICUBIC; //SWS_FAST_BILINEAR

//-----------------------------------------------------------------------------------------
//Code
//-----------------------------------------------------------------------------------------

//#include "ColorSpaceConverter.h"
//#include <DShow.h>
//const char program_name[] = "ffplaymfc";
//const int program_birth_year = 2013;
/*
AVRational GetAvRational(int den, int num)
{
	AVRational value;
	value.num = num;
	value.den = den;
	return value;
}
*/

inline int compute_mod(int a, int b)
{
	return a < 0 ? a % b + b : a % b;
}

AVCodecContext* GetCodec(AVStream* avStream)
{
	AVCodec* pCodec = (AVCodec*)avcodec_find_decoder(avStream->codecpar->codec_id);
	AVCodecContext* pCodecCtx = avcodec_alloc_context3(pCodec);
	avcodec_parameters_to_context(pCodecCtx, avStream->codecpar);
	return pCodecCtx;
}

int CFFmfcPimpl::percentageToDb(int p, int maxValue)
{
	if (p == 0)
		return 0;
	float value = static_cast<float>(p) / 100.0f;
	return maxValue * (1 + log(value));
}

int CFFmfcPimpl::packet_queue_put_private(PacketQueue* q, AVPacket* pkt)
{
	MyAVPacketList pkt1;
	int ret;

	if (q->abort_request)
		return -1;


	pkt1.pkt = pkt;
	pkt1.serial = q->serial;

	ret = av_fifo_write(q->pkt_list, &pkt1, 1);
	if (ret < 0)
		return ret;
	q->nb_packets++;
	q->size += pkt1.pkt->size + sizeof(pkt1);
	q->duration += pkt1.pkt->duration;
	/* XXX: should duplicate packet data in DV case */
	q->cond.notify_one();
	return 0;
}

//Íù¶ÓÁÐÀïÌí¼ÓPacket
int CFFmfcPimpl::packet_queue_put(PacketQueue* q, AVPacket* pkt)
{
	AVPacket* pkt1 = av_packet_alloc();
	if (!pkt1) {
		av_packet_unref(pkt);
		return -1;
	}
	av_packet_move_ref(pkt1, pkt);

	{
		std::lock_guard<std::mutex> lock(q->mutex); // Remplacé : SDL_LockMutex / SDL_UnlockMutex
		int ret = packet_queue_put_private(q, pkt1);
		if (ret >= 0) return ret;
	}

	av_packet_free(&pkt1);
	return -1;
}

int CFFmfcPimpl::packet_queue_put_nullpacket(PacketQueue* q, AVPacket* pkt, int stream_index)
{
	pkt->stream_index = stream_index;
	return packet_queue_put(q, pkt);
}


/* packet queue handling */
int CFFmfcPimpl::packet_queue_init(PacketQueue* q)
{
	if (!q)
		return AVERROR(EINVAL);

	// PacketQueue contient mutex/condition_variable : pas de memset().
	q->pkt_list = nullptr;
	q->nb_packets = 0;
	q->size = 0;
	q->duration = 0;
	q->abort_request = 1;
	q->serial = 0;

	q->pkt_list = av_fifo_alloc2(1, sizeof(MyAVPacketList), AV_FIFO_FLAG_AUTO_GROW);
	if (!q->pkt_list)
		return AVERROR(ENOMEM);

	// Plus besoin de SDL_CreateMutex() ni de SDL_CreateCond()
	q->abort_request = 1;
	return 0;
}

void CFFmfcPimpl::packet_queue_flush(PacketQueue* q)
{
	MyAVPacketList pkt1;
	std::lock_guard<std::mutex> lock(q->mutex);
	while (av_fifo_read(q->pkt_list, &pkt1, 1) >= 0)
		av_packet_free(&pkt1.pkt);
	q->nb_packets = 0;
	q->size = 0;
	q->duration = 0;
	q->serial++;
}

void CFFmfcPimpl::packet_queue_destroy(PacketQueue* q)
{
	packet_queue_flush(q);
	av_fifo_freep2(&q->pkt_list);
	// Plus besoin de SDL_DestroyMutex et SDL_DestroyCond
}

void CFFmfcPimpl::packet_queue_abort(PacketQueue* q)
{
	std::lock_guard<std::mutex> lock(q->mutex);
	q->abort_request = 1;
	q->cond.notify_all(); // Remplacé : SDL_CondSignal(q->cond);
}

void CFFmfcPimpl::packet_queue_start(PacketQueue* q)
{
	std::lock_guard<std::mutex> lock(q->mutex);
	q->abort_request = 0;
	q->serial++;
}

int CFFmfcPimpl::packet_queue_get(PacketQueue* q, AVPacket* pkt, int block, int* serial)
{
	MyAVPacketList pkt1;
	int ret;

	std::unique_lock<std::mutex> lock(q->mutex); // Nécessaire pour condition_variable.wait

	for (;;) {
		if (q->abort_request) {
			ret = -1;
			break;
		}

		if (av_fifo_read(q->pkt_list, &pkt1, 1) >= 0) {
			q->nb_packets--;
			q->size -= pkt1.pkt->size + sizeof(pkt1);
			q->duration -= pkt1.pkt->duration;
			av_packet_move_ref(pkt, pkt1.pkt);
			if (serial)
				*serial = pkt1.serial;
			av_packet_free(&pkt1.pkt);
			ret = 1;
			break;
		}
		else if (!block) {
			ret = 0;
			break;
		}
		else {
			q->cond.wait(lock); // Remplacé : SDL_CondWait(q->cond, q->mutex);
		}
	}
	return ret;
}

void CFFmfcPimpl::free_subpicture(SubPicture* sp)
{
	avsubtitle_free(&sp->sub);
}

void CFFmfcPimpl::StopStream()
{
	exit_video.store(true, std::memory_order_release);
}

void CFFmfcPimpl::stream_close(VideoState* is)
{
	is->abort_request = 1;

	// 1. Avorter immédiatement toutes les files pour débloquer les threads en attente (Condition Variables)
	packet_queue_abort(&is->videoq);
	packet_queue_abort(&is->audioq);
	packet_queue_abort(&is->subtitleq);

	// Réveiller explicitement les files d'attente de frames
	frame_queue_signal(&is->pictq);
	frame_queue_signal(&is->sampq);
	frame_queue_signal(&is->subpq);

	// Réveiller le thread de lecture (is->continue_read_thread n'est plus un pointeur mais une instance C++ standard)
	is->continue_read_thread.notify_all();

	// 2. Maintenant, on peut attendre le thread de lecture en toute sécurité avec la méthode join standard
	if (is->read_tid.joinable())
		is->read_tid.join();

	if (is->refresh_tid != nullptr)
	{
		if (is->refresh_tid->joinable())
			is->refresh_tid->join();

		delete is->refresh_tid;
		is->refresh_tid = nullptr;
	}

	/* close each stream */
	if (is->audio_stream >= 0)
		stream_component_close(is, is->audio_stream);
	if (is->video_stream >= 0)
		stream_component_close(is, is->video_stream);
	if (is->subtitle_stream >= 0)
		stream_component_close(is, is->subtitle_stream);

	avformat_close_input(&is->ic);

	packet_queue_destroy(&is->videoq);
	packet_queue_destroy(&is->audioq);
	packet_queue_destroy(&is->subtitleq);

	/* free all pictures */
	frame_queue_destory(&is->pictq);
	frame_queue_destory(&is->sampq);
	frame_queue_destory(&is->subpq);

	// L'instruction SDL_DestroyCond a été supprimée (géré automatiquement par le destructeur C++ standard)

	sws_freeContext(is->img_convert_ctx);
	sws_freeContext(is->sub_convert_ctx);

	sws_freeContext(localContext);
	localContext = nullptr;
	localWidth = 0;
	localHeight = 0;
	localFormat = AV_PIX_FMT_NONE;

	av_free(is->filename);
	delete is;

	if (parent)
	{
		wxCommandEvent evt(FF_QUIT_EVENT);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}

	exit_video.store(false, std::memory_order_release);
}


//ÍË³ö
void CFFmfcPimpl::do_exit(VideoState* is)
{
	if (exit_remark == 0)
	{
		exit_remark = 1;
		if (is)
		{
			stream_close(is);
		}

		uninit_opts();
		//avformat_network_deinit();
		if (show_status)
			printf("\n");
		av_freep(&video_codec_name);
		av_freep(&audio_codec_name);
		av_freep(&subtitle_codec_name);
		av_log(nullptr, AV_LOG_QUIET, "%s", "");

		// CORRECTION : L'appel SDL_CloseAudioDevice a été retiré. 
		// Le périphérique et le contexte OpenAL sont déjà correctement libérés dans stream_close().
	}
}


int CFFmfcPimpl::IsSupportOpenCL()
{
	int supportOpenCL = 0;
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		supportOpenCL = config->GetIsOpenCLSupport();

	//if (cv::ocl::Context::getDefault(false).empty())
	//	return 0;

	return supportOpenCL;
}


AVFrame* CFFmfcPimpl::CopyFrame(AVFrame* src)
{
	if (!src)
		return nullptr;

	AVFrame* dst = av_frame_alloc();
	if (!dst)
		return nullptr;

	if (av_frame_ref(dst, src) < 0)
	{
		av_frame_free(&dst);
		return nullptr;
	}

	return dst;
}

bool CFFmfcPimpl::EnsureVideoConversionContext(const AVFrame* frame)
{
	if (!frame || frame->width <= 0 || frame->height <= 0)
		return false;

	const auto format = static_cast<AVPixelFormat>(frame->format);

	if (localContext &&
		localWidth == frame->width &&
		localHeight == frame->height &&
		localFormat == format)
	{
		return true;
	}

	sws_freeContext(localContext);
	localContext = sws_getContext(
		frame->width, frame->height, format,
		frame->width, frame->height, AV_PIX_FMT_BGRA,
		application_context.GetInterpolationMethod(), nullptr, nullptr, nullptr);

	if (!localContext)
	{
		localWidth = 0;
		localHeight = 0;
		localFormat = AV_PIX_FMT_NONE;
		return false;
	}

	localWidth = frame->width;
	localHeight = frame->height;
	localFormat = format;
	return true;
}

void CFFmfcPimpl::ConvertSubtitleBitmap(const AVSubtitleRect* rect, cv::Mat& bitmap)
{
	if (!rect || rect->w <= 0 || rect->h <= 0 || !rect->data[0] || !rect->data[1])
		return;

	bitmap.create(rect->h, rect->w, CV_8UC4);

	for (int y = 0; y < rect->h; ++y)
	{
		const uint8_t* src = rect->data[0] + y * rect->linesize[0];
		auto* dst = bitmap.ptr<CRgbaquad>(y);

		for (int x = 0; x < rect->w; ++x)
		{
			int r, g, b, a;
			const int index = src[x];
			RGBA_IN(r, g, b, a, reinterpret_cast<const uint32_t*>(rect->data[1]) + index);
			dst[x] = CRgbaquad(r, g, b, a);
		}
	}
}

void CFFmfcPimpl::PostSubtitleImage(const cv::Mat& bitmap)
{
	if (!dlg)
		return;

	auto* image = new cv::Mat(bitmap.clone());
	wxCommandEvent event(wxEVENT_SETSUBTITLEIMAGE);
	event.SetClientData(image);
	wxPostEvent(dlg->GetMainWindow(), event);
}

void CFFmfcPimpl::PostSubtitleText(const wxString& text, int endDisplayTime)
{
	if (!dlg)
		return;

	auto* subtitle = new wxString(text);
	wxCommandEvent event(wxEVENT_SETSUBTITLETEXT);
	event.SetClientData(subtitle);
	event.SetInt(endDisplayTime);
	wxPostEvent(dlg->GetMainWindow(), event);
}

/* display the current picture, if any */
void CFFmfcPimpl::video_display(VideoState* is)
{
	//printf("CFFmfcPimpl::video_display \n");


	int ret = 0;
	Frame* vp;
	Frame* sp = NULL;
	vp = frame_queue_peek_last(&is->pictq);

	if (!CMasterWindow::endProgram)
	{
		float video_aspect_ratio = 0;
		if (vp->sample_aspect_ratio.num == 0)
			video_aspect_ratio = 0;
		else
			video_aspect_ratio = av_q2d(vp->sample_aspect_ratio);

		if (dlg != nullptr)
			if (!is->paused)
			{

				int64_t pos = vp->pts * 1000;
				wxCommandEvent evt(wxEVENT_SETFRAMEPOS);
				evt.SetExtraLong(pos);
				wxPostEvent(dlg->GetMainWindow(), evt);

				//is->_pimpl->start_time
				//dlg->SetPos(vp->pts * 1000);
			}


		if (dlg != nullptr && dlg->IsAvailable())
		{
			auto tmp_frame = static_cast<AVFrame*>(vp->frame);

			/*
			CDataAVFrame* dataFrame = new CDataAVFrame();
			dataFrame->width = tmp_frame->width;
			dataFrame->height = tmp_frame->height;
			dataFrame->ratioVideo = static_cast<float>(tmp_frame->width) / static_cast<float>(tmp_frame->height);
			dataFrame->sample_aspect_ratio = video_aspect_ratio;
			dataFrame->isHardwareDecoding = isHardwareDecoding;

			if (IsSupportOpenCL() && !isHardwareDecoding && dlg->ApplyVideoEffect() && (tmp_frame->format == AV_PIX_FMT_NV12 || tmp_frame->format == AV_PIX_FMT_YUV420P))
			{
				dataFrame->dst = CopyFrame(vp->frame);
			}
			else
			{

				dataFrame->matFrame = cv::Mat(tmp_frame->height, tmp_frame->width, CV_8UC4);


				if (EnsureVideoConversionContext(tmp_frame))
				{
					uint8_t* convertedFrameBuffer = dataFrame->matFrame.data;
					int linesize = tmp_frame->width * 4;

					sws_scale(localContext, tmp_frame->data, tmp_frame->linesize, 0, tmp_frame->height,
						&convertedFrameBuffer, &linesize);
				}
			}
			*/

			// Remplacer la logique d'allocation par une affectation exclusive :
			CDataAVFrame* dataFrame = new CDataAVFrame();
			dataFrame->width = tmp_frame->width;
			dataFrame->height = tmp_frame->height;
			dataFrame->ratioVideo = static_cast<float>(tmp_frame->width) / static_cast<float>(tmp_frame->height);
			dataFrame->sample_aspect_ratio = video_aspect_ratio;
			dataFrame->isHardwareDecoding = isHardwareDecoding;

			if (IsSupportOpenCL() && !isHardwareDecoding && dlg->ApplyVideoEffect() &&
				(tmp_frame->format == AV_PIX_FMT_NV12 || tmp_frame->format == AV_PIX_FMT_YUV420P))
			{
				dataFrame->dst = CopyFrame(vp->frame);
			}
			else
			{
				dataFrame->dst = nullptr; // Évite les pointeurs sauvages
				dataFrame->matFrame = cv::Mat(tmp_frame->height, tmp_frame->width, CV_8UC4);

				if (EnsureVideoConversionContext(tmp_frame))
				{
					uint8_t* convertedFrameBuffer = dataFrame->matFrame.data;
					int linesize = tmp_frame->width * 4;

					sws_scale(localContext, tmp_frame->data, tmp_frame->linesize, 0, tmp_frame->height,
						&convertedFrameBuffer, &linesize);
				}
			}

			wxCommandEvent event(wxEVENT_UPDATEFRAME);
			event.SetClientData(dataFrame);
			wxPostEvent(dlg->GetMainWindow(), event);

		}

		//}

		if (is->subtitle_st)
		{
			if (frame_queue_nb_remaining(&is->subpq) > 0)
			{
				sp = frame_queue_peek(&is->subpq);

				if (TRUE)
				{
					if (sp->sub.format == 0 && (vp->pts >= sp->pts + (static_cast<float>(sp->sub.start_display_time) / 1000)))
					{
						for (int i = 0; i < sp->sub.num_rects; i++)
						{
							AVSubtitleRect* rect = sp->sub.rects[i];
							//AVPicture picture = rect->pict;
							cv::Mat bitmap;
							ConvertSubtitleBitmap(rect, bitmap);
							PostSubtitleImage(bitmap);
						}
					}
					else if (sp->sub.format == 1 && (vp->pts >= sp->pts - (static_cast<float>(sp->sub.end_display_time))))
					{
						wxString text = "";
						for (int i = 0; i < sp->sub.num_rects; i++)
						{

							AVSubtitleRect* rect = sp->sub.rects[i];
							if (rect->ass != nullptr)
							{
								wxString mystring = wxString::FromUTF8(rect->ass, strlen(rect->ass));
								text += mystring;
							}
							else if (rect->text != nullptr)
							{
								wxString mystring = wxString::FromUTF8(rect->text, strlen(rect->text));
								text += mystring;
							}

						}
						if (dlg != nullptr)
						{
							wxString* _textSub = new wxString(text);
							wxCommandEvent event(wxEVENT_SETSUBTITLETEXT);
							event.SetClientData(_textSub);
							event.SetInt(sp->sub.end_display_time);
							wxPostEvent(dlg->GetMainWindow(), event);
						}
					}
				}
			}
		}
		else if (dlg != nullptr)
		{
			wxCommandEvent event(wxEVENT_DELETESUBTITLEIMAGE);
			wxPostEvent(dlg->GetMainWindow(), event);
		}
	}
}


/* seek in the stream */
void CFFmfcPimpl::stream_seek(VideoState* is, int64_t pos, int64_t rel, int seek_by_bytes)
{
	if (!is->seek_req)
	{
		is->seek_pos = pos;
		is->seek_rel = rel;
		is->seek_flags &= ~AVSEEK_FLAG_BYTE;
		if (seek_by_bytes)
			is->seek_flags |= AVSEEK_FLAG_BYTE;
		is->seek_req = 1;
	}
}

/* pause or resume the video */
void CFFmfcPimpl::stream_toggle_pause(VideoState* is)
{
	if (is->paused)
	{
		is->frame_timer += av_gettime_relative() / 1000000.0 - is->vidclk.last_updated;
		if (is->read_pause_return != AVERROR(ENOSYS))
		{
			is->vidclk.paused = 0;
		}
		set_clock(&is->vidclk, get_clock(&is->vidclk), is->vidclk.serial);
	}
	set_clock(&is->extclk, get_clock(&is->extclk), is->extclk.serial);
	is->paused = is->audclk.paused = is->vidclk.paused = is->extclk.paused = !is->paused;
}

/* get the current master clock value */
double CFFmfcPimpl::get_master_clock(VideoState* is)
{
	double val;

	switch (get_master_sync_type(is))
	{
	case AV_SYNC_VIDEO_MASTER:
		val = get_clock(&is->vidclk);
		break;
	case AV_SYNC_AUDIO_MASTER:
		val = get_clock(&is->audclk);
		break;
	default:
		val = get_clock(&is->extclk);
		break;
	}
	return val;
}


double CFFmfcPimpl::compute_target_delay(double delay, VideoState* is)
{
	double sync_threshold, diff = 0;


	/* update delay to follow master synchronisation source */
	if (get_master_sync_type(is) != AV_SYNC_VIDEO_MASTER)
	{
		/* if video is slave, we try to correct big delays by
		   duplicating or deleting a frame */
		   //if(disable_framedelay)
		diff = get_clock(&is->vidclk) - get_master_clock(is);

		/* skip or repeat frame. We take into account the
		   delay to compute the threshold. I still don't know
		   if it is the best guess */
		sync_threshold = FFMAX(AV_SYNC_THRESHOLD_MIN, FFMIN(AV_SYNC_THRESHOLD_MAX, delay));
		if (!isnan(diff) && fabs(diff) < is->max_frame_duration)
		{
			if (diff <= -sync_threshold)
				delay = FFMAX(0, delay + diff);
			else if (diff >= sync_threshold && delay > AV_SYNC_FRAMEDUP_THRESHOLD)
				delay = delay + diff;
			else if (diff >= sync_threshold)
				delay = 2 * delay;
		}
	}

	av_log(NULL, AV_LOG_TRACE, "video: delay=%0.3f A-V=%f\n",
		delay, -diff);

	return delay;
}

void CFFmfcPimpl::set_clock_at(Clock* c, double pts, int serial, double time)
{
	c->pts = pts;
	c->last_updated = time;
	c->pts_drift = c->pts - time;
	c->serial = serial;
}

double CFFmfcPimpl::get_clock(Clock* c)
{
	if (*c->queue_serial != c->serial)
		return NAN;
	if (c->paused)
	{
		return c->pts;
	}
	else
	{
		double time = av_gettime_relative() / 1000000.0;
		return c->pts_drift + time - (time - c->last_updated) * (1.0 - c->speed);
	}
}


void CFFmfcPimpl::set_clock(Clock* c, double pts, int serial)
{
	double time = av_gettime_relative() / 1000000.0;
	set_clock_at(c, pts, serial, time);
}

void CFFmfcPimpl::set_clock_speed(Clock* c, double speed)
{
	set_clock(c, get_clock(c), c->serial);
	c->speed = speed;
}

void CFFmfcPimpl::sync_clock_to_slave(Clock* c, Clock* slave)
{
	double clock = get_clock(c);
	double slave_clock = get_clock(slave);
	if (!isnan(slave_clock) && (isnan(clock) || fabs(clock - slave_clock) > AV_NOSYNC_THRESHOLD))
		set_clock(c, slave_clock, slave->serial);
}

void CFFmfcPimpl::update_video_pts(VideoState* is, double pts, int64_t pos, int serial)
{
	/* update current video pts */
	set_clock(&is->vidclk, pts, serial);
	sync_clock_to_slave(&is->extclk, &is->vidclk);
}


void CFFmfcPimpl::check_external_clock_speed(VideoState* is)
{
	if (is->video_stream >= 0 && is->videoq.nb_packets <= EXTERNAL_CLOCK_MIN_FRAMES ||
		is->audio_stream >= 0 && is->audioq.nb_packets <= EXTERNAL_CLOCK_MIN_FRAMES)
	{
		set_clock_speed(&is->extclk, FFMAX(EXTERNAL_CLOCK_SPEED_MIN, is->extclk.speed - EXTERNAL_CLOCK_SPEED_STEP));
	}
	else if ((is->video_stream < 0 || is->videoq.nb_packets > EXTERNAL_CLOCK_MAX_FRAMES) &&
		(is->audio_stream < 0 || is->audioq.nb_packets > EXTERNAL_CLOCK_MAX_FRAMES))
	{
		set_clock_speed(&is->extclk, FFMIN(EXTERNAL_CLOCK_SPEED_MAX, is->extclk.speed + EXTERNAL_CLOCK_SPEED_STEP));
	}
	else
	{
		double speed = is->extclk.speed;
		if (speed != 1.0)
			set_clock_speed(&is->extclk, speed + EXTERNAL_CLOCK_SPEED_STEP * (1.0 - speed) / fabs(1.0 - speed));
	}
}


double CFFmfcPimpl::vp_duration(VideoState* is, Frame* vp, Frame* nextvp)
{
	if (vp->serial == nextvp->serial)
	{
		double duration = nextvp->pts - vp->pts;
		if (isnan(duration) || duration <= 0 || duration > is->max_frame_duration)
			return vp->duration;
		else
			return duration;
	}
	else
	{
		return 0.0;
	}
}

/* called to display each frame */
void CFFmfcPimpl::video_refresh(void* opaque, double* remaining_time)
{
	auto is = static_cast<VideoState*>(opaque);
	double time;

	Frame* sp, * sp2;

	if (!is->paused && get_master_sync_type(is) == AV_SYNC_EXTERNAL_CLOCK && is->realtime)
		check_external_clock_speed(is);

	if (!display_disable && is->show_mode != SHOW_MODE_VIDEO && is->audio_st)
	{
		time = av_gettime_relative() / 1000000.0;
		if (is->force_refresh || is->last_vis_time + rdftspeed < time)
		{
			video_display(is);
			is->last_vis_time = time;
		}
		*remaining_time = FFMIN(*remaining_time, is->last_vis_time + rdftspeed - time);
	}

	if (is->video_st)
	{
	retry:
		if (frame_queue_nb_remaining(&is->pictq) == 0)
		{
			// nothing to do, no picture to display in the queue
		}
		else
		{
			double last_duration, duration, delay;
			Frame* vp, * lastvp;

			/* dequeue the picture */
			lastvp = frame_queue_peek_last(&is->pictq);
			vp = frame_queue_peek(&is->pictq);

			if (vp->serial != is->videoq.serial)
			{
				frame_queue_next(&is->pictq);
				goto retry;
			}

			if (lastvp->serial != vp->serial)
				is->frame_timer = av_gettime_relative() / 1000000.0;

			if (is->paused)
				goto display;

			/* compute nominal last_duration */
			last_duration = vp_duration(is, lastvp, vp);
			delay = compute_target_delay(last_duration, is);

			time = av_gettime_relative() / 1000000.0;
			if (time < is->frame_timer + delay)
			{
				*remaining_time = FFMIN(is->frame_timer + delay - time, *remaining_time);
				goto display;
			}

			is->frame_timer += delay;
			if (delay > 0 && time - is->frame_timer > AV_SYNC_THRESHOLD_MAX)
				is->frame_timer = time;

			// CORRECTION ICI : Remplacement des primitives SDL par std::unique_lock (RAII)
			{
				std::unique_lock<std::mutex> lock(is->pictq.mutex);
				if (!isnan(vp->pts))
					update_video_pts(is, vp->pts, vp->pos, vp->serial);
			}

			if (frame_queue_nb_remaining(&is->pictq) > 1)
			{
				Frame* nextvp = frame_queue_peek_next(&is->pictq);
				duration = vp_duration(is, vp, nextvp);
				if (!is->step && (framedrop > 0 || (framedrop && get_master_sync_type(is) != AV_SYNC_VIDEO_MASTER)) &&
					time > is->frame_timer + duration)
				{
					is->frame_drops_late++;
					frame_queue_next(&is->pictq);
					goto retry;
				}
			}

			//Subtitle
			if (is->subtitle_st)
			{
				while (frame_queue_nb_remaining(&is->subpq) > 0)
				{
					sp = frame_queue_peek(&is->subpq);

					if (frame_queue_nb_remaining(&is->subpq) > 1)
						sp2 = frame_queue_peek_next(&is->subpq);
					else
						sp2 = NULL;

					if (sp->sub.format == 0 && (sp->serial != is->subtitleq.serial
						|| (is->vidclk.pts > (sp->pts + ((float)sp->sub.end_display_time / 1000)))
						|| (sp2 && is->vidclk.pts > (sp2->pts + ((float)sp2->sub.start_display_time / 1000)))))
					{
						if (sp->uploaded)
						{
							int i;
							for (i = 0; i < sp->sub.num_rects; i++)
							{
								AVSubtitleRect* sub_rect = sp->sub.rects[i];

								AVSubtitleRect* rect = sp->sub.rects[i];
								cv::Mat* bitmap = new cv::Mat(rect->h, rect->w, CV_8UC4);
								uint8_t* data = rect->data[0];
								for (int y = 0; y < rect->h; y++)
								{
									for (int x = 0; x < rect->w; x++)
									{
										int r, g, b, a;
										int j = *data++;
										RGBA_IN(r, g, b, a, (uint32_t*)rect->data[1] + j);
										CRgbaquad color(r, g, b, a);
										int i = (x << 2) + (y * (bitmap->cols << 2));
										memcpy(bitmap->data + i, &color, sizeof(CRgbaquad));
									}
								}
								if (dlg != nullptr)
								{
									wxCommandEvent event(wxEVENT_SETSUBTITLEIMAGE);
									event.SetClientData(bitmap);
									wxPostEvent(dlg->GetMainWindow(), event);
								}
								else {
									delete bitmap;
								}
							}

						}
						frame_queue_next(&is->subpq);
					}
					else if (sp->sub.format == 1 && (sp->serial != is->subtitleq.serial
						|| (is->vidclk.pts > (sp->pts + ((float)sp->sub.end_display_time)))
						|| (sp2 && is->vidclk.pts > (sp2->pts + ((float)sp2->sub.start_display_time)))))
					{
						if (sp->uploaded)
						{
							wxString text = "";
							for (int i = 0; i < sp->sub.num_rects; i++)
							{
								AVSubtitleRect* rect = sp->sub.rects[i];
								if (rect != nullptr)
									text += rect->ass;
							}
							PostSubtitleText(text, sp->sub.end_display_time);

						}
						frame_queue_next(&is->subpq);
					}
					else
					{
						break;
					}
				}
			}

			frame_queue_next(&is->pictq);
			is->force_refresh = 1;

			if (is->step && !is->paused)
				stream_toggle_pause(is);
		}

	display:
		if (!display_disable && is->force_refresh && is->show_mode == SHOW_MODE_VIDEO && is->pictq.rindex_shown)
			video_display(is);
	}
	is->force_refresh = 0;
	is->refresh = 0;
}


int CFFmfcPimpl::GetPosition(VideoState* is)
{
	return get_master_clock(is) * 1000;
}


int CFFmfcPimpl::queue_picture(VideoState* is, AVFrame* src_frame, double pts, double duration, int64_t pos, int serial)
{
	Frame* vp;
	int ret = 0;

#if defined(DEBUG_SYNC)
	printf("frame_type=%c pts=%0.3f\n",
		av_get_picture_type_char(src_frame->pict_type), pts);
#endif

	if (!(vp = frame_queue_peek_writable(&is->pictq)))
		return -1;

	vp->sar = src_frame->sample_aspect_ratio;
	vp->uploaded = 0;

	vp->width = src_frame->width;
	vp->height = src_frame->height;
	vp->format = src_frame->format;

	vp->pts = pts;
	vp->duration = duration;
	vp->pos = pos;
	vp->serial = serial;

	vp->sample_aspect_ratio = av_guess_sample_aspect_ratio(is->ic, is->video_st, src_frame);


	av_frame_move_ref(vp->frame, src_frame);
	frame_queue_push(&is->pictq);
	return 0;
}



int CFFmfcPimpl::get_master_sync_type(VideoState* is)
{
	if (is->av_sync_type == AV_SYNC_VIDEO_MASTER)
	{
		if (is->video_st)
			return AV_SYNC_VIDEO_MASTER;
		else
			return AV_SYNC_AUDIO_MASTER;
	}
	else if (is->av_sync_type == AV_SYNC_AUDIO_MASTER)
	{
		if (is->audio_st)
			return AV_SYNC_AUDIO_MASTER;
		else
			return AV_SYNC_EXTERNAL_CLOCK;
	}
	else
	{
		return AV_SYNC_EXTERNAL_CLOCK;
	}
}


int CFFmfcPimpl::get_video_frame(VideoState* is, AVFrame* frame)
{
	int got_picture;

	if ((got_picture = decoder_decode_frame(is, &is->viddec, frame, NULL)) < 0)
		return -1;

	if (got_picture)
	{
		double dpts = NAN;

		if (frame->pts != AV_NOPTS_VALUE)
			dpts = av_q2d(is->video_st->time_base) * frame->pts;

		frame->sample_aspect_ratio = av_guess_sample_aspect_ratio(is->ic, is->video_st, frame);

		if (framedrop > 0 || (framedrop && get_master_sync_type(is) != AV_SYNC_VIDEO_MASTER))
		{
			if (frame->pts != AV_NOPTS_VALUE)
			{
				double diff = dpts - get_master_clock(is);
				if (!isnan(diff) && fabs(diff) < AV_NOSYNC_THRESHOLD &&
					diff - is->frame_last_filter_delay < 0 &&
					is->viddec.pkt_serial == is->vidclk.serial &&
					is->videoq.nb_packets)
				{
					is->frame_drops_early++;
					av_frame_unref(frame);
					got_picture = 0;
				}
			}
		}
	}

	return got_picture;
}

//½âÂëÊÓÆµ
int CFFmfcPimpl::video_thread(void* arg)
{
	auto is = static_cast<VideoState*>(arg);
	AVFrame* frame = av_frame_alloc();
	double pts;
	double duration;
	int ret;
	AVRational tb = is->video_st->time_base;
	AVRational frame_rate = av_guess_frame_rate(is->ic, is->video_st, NULL);



	if (!frame)
		return AVERROR(ENOMEM);

	for (;;)
	{
		FrameData* fd;

		ret = is->_pimpl->get_video_frame(is, frame);
		if (ret < 0)
			goto the_end;
		if (!ret)
			continue;

		fd = frame->opaque_ref ? (FrameData*)frame->opaque_ref->data : NULL;

		AVRational tb_frame = { frame_rate.den, frame_rate.num };
		duration = (frame_rate.num && frame_rate.den ? av_q2d(tb_frame) : 0);
		pts = (frame->pts == AV_NOPTS_VALUE) ? NAN : frame->pts * av_q2d(tb);

		ret = is->_pimpl->queue_picture(is, frame, pts, duration, fd ? fd->pkt_pos : -1, is->viddec.pkt_serial);

		av_frame_unref(frame);


		if (ret < 0)
			goto the_end;
	}


the_end:
	av_frame_free(&frame);


	return 0;
}


void CFFmfcPimpl::frame_queue_unref_item(Frame* vp)
{
	av_frame_unref(vp->frame);
	avsubtitle_free(&vp->sub);
}

int CFFmfcPimpl::frame_queue_init(FrameQueue* f, PacketQueue* pktq, int max_size, int keep_last)
{
	int i;
	if (!f || !pktq)
		return AVERROR(EINVAL);

	// FrameQueue contient mutex/condition_variable : pas de memset().
	f->rindex = 0;
	f->windex = 0;
	f->size = 0;
	f->max_size = FFMIN(max_size, FRAME_QUEUE_SIZE);
	f->keep_last = !!keep_last;
	f->rindex_shown = 0;
	f->pktq = pktq;

	// Supprimé : SDL_CreateMutex et SDL_CreateCond
	for (i = 0; i < f->max_size; i++)
		if (!(f->queue[i].frame = av_frame_alloc()))
			return AVERROR(ENOMEM);
	return 0;
}

void CFFmfcPimpl::frame_queue_destory(FrameQueue* f)
{
	int i;
	for (i = 0; i < f->max_size; i++) {
		Frame* vp = &f->queue[i];
		frame_queue_unref_item(vp);
		av_frame_free(&vp->frame);
	}
	// Supprimé : SDL_DestroyMutex et SDL_DestroyCond
}

void CFFmfcPimpl::frame_queue_signal(FrameQueue* f)
{
	std::lock_guard<std::mutex> lock(f->mutex);
	f->cond.notify_one(); // Remplacé : SDL_CondSignal(f->cond);
}

CFFmfcPimpl::Frame* CFFmfcPimpl::frame_queue_peek(FrameQueue* f)
{
	return &f->queue[(f->rindex + f->rindex_shown) % f->max_size];
}

CFFmfcPimpl::Frame* CFFmfcPimpl::frame_queue_peek_next(FrameQueue* f)
{
	return &f->queue[(f->rindex + f->rindex_shown + 1) % f->max_size];
}

CFFmfcPimpl::Frame* CFFmfcPimpl::frame_queue_peek_last(FrameQueue* f)
{
	return &f->queue[f->rindex];
}

CFFmfcPimpl::Frame* CFFmfcPimpl::frame_queue_peek_writable(FrameQueue* f)
{
	/* wait until we have space to put a new frame */
	std::unique_lock<std::mutex> lock(f->mutex);
	while (f->size >= f->max_size &&
		!f->pktq->abort_request)
	{
		f->cond.wait(lock);
	}
	lock.unlock();

	if (f->pktq->abort_request)
		return NULL;

	return &f->queue[f->windex];
}

CFFmfcPimpl::Frame* CFFmfcPimpl::frame_queue_peek_readable(FrameQueue* f)
{
	/* wait until we have a readable a new frame */
	std::unique_lock<std::mutex> lock(f->mutex);
	while (f->size - f->rindex_shown <= 0 &&
		!f->pktq->abort_request)
	{
		f->cond.wait(lock);
	}
	lock.unlock();

	if (f->pktq->abort_request)
		return NULL;

	return &f->queue[(f->rindex + f->rindex_shown) % f->max_size];
}

void CFFmfcPimpl::frame_queue_push(FrameQueue* f)
{
	std::unique_lock<std::mutex> lock(f->mutex);
	if (++f->windex == f->max_size)
		f->windex = 0;
	f->size++;
	f->cond.notify_one();
}

void CFFmfcPimpl::frame_queue_next(FrameQueue* f)
{
	if (f->keep_last && !f->rindex_shown) {
		f->rindex_shown = 1;
		return;
	}
	frame_queue_unref_item(&f->queue[f->rindex]);
	if (++f->rindex == f->max_size)
		f->rindex = 0;
	std::lock_guard<std::mutex> lock(f->mutex);
	f->size--;
	f->cond.notify_one(); // Remplacé : SDL_CondSignal

}

/* return the number of undisplayed frames in the queue */
int CFFmfcPimpl::frame_queue_nb_remaining(FrameQueue* f)
{
	return f->size - f->rindex_shown;
}

/* return last shown position */
int64_t CFFmfcPimpl::frame_queue_last_pos(FrameQueue* f)
{
	Frame* fp = &f->queue[f->rindex];
	if (f->rindex_shown && fp->serial == f->pktq->serial)
		return fp->pos;
	else
		return -1;
}

void CFFmfcPimpl::decoder_abort(Decoder* d, FrameQueue* fq)
{
	packet_queue_abort(d->queue);
	frame_queue_signal(fq);

	if (d->decoder_thread.joinable()) {
		d->decoder_thread.join(); // Remplacé : SDL_WaitThread
	}
	packet_queue_flush(d->queue);
}

int CFFmfcPimpl::subtitle_thread(void* arg)
{
	auto is = static_cast<VideoState*>(arg);
	Frame* sp;
	int got_subtitle;
	double pts;

	for (;;)
	{
		if (exit_video.load(std::memory_order_acquire))
			break;

		if (!(sp = is->_pimpl->frame_queue_peek_writable(&is->subpq)))
			return 0;

		if ((got_subtitle = is->_pimpl->decoder_decode_frame(is, &is->subdec, NULL, &sp->sub)) < 0)
			break;

		pts = 0;

		if (got_subtitle && sp->sub.format == 0)
		{
			if (sp->sub.pts != AV_NOPTS_VALUE)
				pts = sp->sub.pts / (double)AV_TIME_BASE;
			sp->pts = pts;
			sp->serial = is->subdec.pkt_serial;
			sp->width = is->subdec.avctx->width;
			sp->height = is->subdec.avctx->height;
			sp->uploaded = 0;

			/* now we can update the picture count */
			is->_pimpl->frame_queue_push(&is->subpq);
		}
		else if (got_subtitle && sp->sub.format == 1)
		{
			if (sp->sub.pts != AV_NOPTS_VALUE)
				pts = sp->sub.pts / (double)AV_TIME_BASE;
			sp->pts = pts;
			sp->serial = is->subdec.pkt_serial;
			sp->width = is->subdec.avctx->width;
			sp->height = is->subdec.avctx->height;
			sp->uploaded = 0;

			/* now we can update the picture count */
			is->_pimpl->frame_queue_push(&is->subpq);

			/*
			wxString text = "";
			for (int i = 0; i < sp->sub.num_rects; i++)
			{
				AVSubtitleRect* rect = sp->sub.rects[i];
				text += rect->ass;
			}
			if (dlg != nullptr)
				dlg->SetSubtituleText(text, sp->sub.end_display_time);


			avsubtitle_free(&sp->sub);
			*/
		}
		else if (got_subtitle)
		{
			avsubtitle_free(&sp->sub);
		}
	}
	return 0;
}

//²¥·ÅÒôÆµµÄÊ±ºòSDLµÄÏÔÊ¾
/* copy samples for viewing in editor window */
void CFFmfcPimpl::update_sample_display(VideoState* is, short* samples, int samples_size)
{
	int size, len;

	size = samples_size / sizeof(short);
	while (size > 0)
	{
		len = SAMPLE_ARRAY_SIZE - is->sample_array_index;
		if (len > size)
			len = size;
		memcpy(is->sample_array + is->sample_array_index, samples, len * sizeof(short));
		samples += len;
		is->sample_array_index += len;
		if (is->sample_array_index >= SAMPLE_ARRAY_SIZE)
			is->sample_array_index = 0;
		size -= len;
	}
}

/* return the wanted number of samples to get better sync if sync_type is video
* or external master clock */
int CFFmfcPimpl::synchronize_audio(VideoState* is, int nb_samples)
{
	int wanted_nb_samples = nb_samples;

	/* if not master, then we try to remove or add samples to correct the clock */
	if (get_master_sync_type(is) != AV_SYNC_AUDIO_MASTER)
	{
		double diff, avg_diff;
		int min_nb_samples, max_nb_samples;

		diff = get_clock(&is->audclk) - get_master_clock(is);

		if (!isnan(diff) && fabs(diff) < AV_NOSYNC_THRESHOLD)
		{
			is->audio_diff_cum = diff + is->audio_diff_avg_coef * is->audio_diff_cum;
			if (is->audio_diff_avg_count < AUDIO_DIFF_AVG_NB)
			{
				/* not enough measures to have a correct estimate */
				is->audio_diff_avg_count++;
			}
			else
			{
				/* estimate the A-V difference */
				avg_diff = is->audio_diff_cum * (1.0 - is->audio_diff_avg_coef);

				if (fabs(avg_diff) >= is->audio_diff_threshold)
				{
					wanted_nb_samples = nb_samples + (int)(diff * is->audio_src.freq);
					min_nb_samples = ((nb_samples * (100 - SAMPLE_CORRECTION_PERCENT_MAX) / 100));
					max_nb_samples = ((nb_samples * (100 + SAMPLE_CORRECTION_PERCENT_MAX) / 100));
					wanted_nb_samples = av_clip(wanted_nb_samples, min_nb_samples, max_nb_samples);
				}
				av_log(NULL, AV_LOG_TRACE, "diff=%f adiff=%f sample_diff=%d apts=%0.3f %f\n",
					diff, avg_diff, wanted_nb_samples - nb_samples,
					is->audio_clock, is->audio_diff_threshold);
			}
		}
		else
		{
			/* too big difference : may be initial PTS errors, so
			   reset A-V filter */
			is->audio_diff_avg_count = 0;
			is->audio_diff_cum = 0;
		}
	}

	return wanted_nb_samples;
}

/**
 * Decode one audio frame and return its uncompressed size.
 *
 * The processed audio frame is decoded, converted if required, and
 * stored in is->audio_buf, with size in bytes given by the return
 * value.
 */
int  CFFmfcPimpl::audio_decode_frame(VideoState* is)
{
	int data_size, resampled_data_size;
	av_unused double audio_clock0;
	int wanted_nb_samples;
	Frame* af;

	if (is->paused)
		return -1;

	do {
#if defined(_WIN32)
		while (frame_queue_nb_remaining(&is->sampq) == 0) {
			if ((av_gettime_relative() - audio_callback_time) > 1000000LL * is->audio_hw_buf_size / is->audio_tgt.bytes_per_sec / 2)
				return -1;
			av_usleep(1000);
		}
#endif
		if (!(af = frame_queue_peek_readable(&is->sampq)))
			return -1;
		frame_queue_next(&is->sampq);
	} while (af->serial != is->audioq.serial);

	data_size = av_samples_get_buffer_size(NULL, af->frame->ch_layout.nb_channels,
		af->frame->nb_samples,
		(AVSampleFormat)af->frame->format, 1);

	wanted_nb_samples = synchronize_audio(is, af->frame->nb_samples);

	if (af->frame->format != is->audio_src.fmt ||
		av_channel_layout_compare(&af->frame->ch_layout, &is->audio_src.ch_layout) ||
		af->frame->sample_rate != is->audio_src.freq ||
		(wanted_nb_samples != af->frame->nb_samples && !is->swr_ctx)) {
		int ret;
		swr_free(&is->swr_ctx);
		ret = swr_alloc_set_opts2(&is->swr_ctx,
			&is->audio_tgt.ch_layout, is->audio_tgt.fmt, is->audio_tgt.freq,
			&af->frame->ch_layout, (AVSampleFormat)af->frame->format, af->frame->sample_rate,
			0, NULL);
		if (ret < 0 || swr_init(is->swr_ctx) < 0) {
			av_log(NULL, AV_LOG_ERROR,
				"Cannot create sample rate converter for conversion of %d Hz %s %d channels to %d Hz %s %d channels!\n",
				af->frame->sample_rate, av_get_sample_fmt_name((AVSampleFormat)af->frame->format), af->frame->ch_layout.nb_channels,
				is->audio_tgt.freq, av_get_sample_fmt_name(is->audio_tgt.fmt), is->audio_tgt.ch_layout.nb_channels);
			swr_free(&is->swr_ctx);
			return -1;
		}
		if (av_channel_layout_copy(&is->audio_src.ch_layout, &af->frame->ch_layout) < 0)
			return -1;
		is->audio_src.freq = af->frame->sample_rate;
		is->audio_src.fmt = (AVSampleFormat)af->frame->format;
	}

	if (is->swr_ctx) {
		const uint8_t** in = (const uint8_t**)af->frame->extended_data;
		uint8_t** out = &is->audio_buf1;
		int out_count = (int64_t)wanted_nb_samples * is->audio_tgt.freq / af->frame->sample_rate + 256;
		int out_size = av_samples_get_buffer_size(NULL, is->audio_tgt.ch_layout.nb_channels, out_count, is->audio_tgt.fmt, 0);
		int len2;
		if (out_size < 0) {
			av_log(NULL, AV_LOG_ERROR, "av_samples_get_buffer_size() failed\n");
			return -1;
		}
		if (wanted_nb_samples != af->frame->nb_samples) {
			if (swr_set_compensation(is->swr_ctx, (wanted_nb_samples - af->frame->nb_samples) * is->audio_tgt.freq / af->frame->sample_rate,
				wanted_nb_samples * is->audio_tgt.freq / af->frame->sample_rate) < 0) {
				av_log(NULL, AV_LOG_ERROR, "swr_set_compensation() failed\n");
				return -1;
			}
		}
		av_fast_malloc(&is->audio_buf1, &is->audio_buf1_size, out_size);
		if (!is->audio_buf1)
			return AVERROR(ENOMEM);
		len2 = swr_convert(is->swr_ctx, out, out_count, in, af->frame->nb_samples);
		if (len2 < 0) {
			av_log(NULL, AV_LOG_ERROR, "swr_convert() failed\n");
			return -1;
		}
		if (len2 == out_count) {
			av_log(NULL, AV_LOG_WARNING, "audio buffer is probably too small\n");
			if (swr_init(is->swr_ctx) < 0)
				swr_free(&is->swr_ctx);
		}
		is->audio_buf = is->audio_buf1;
		resampled_data_size = len2 * is->audio_tgt.ch_layout.nb_channels * av_get_bytes_per_sample(is->audio_tgt.fmt);
	}
	else {
		is->audio_buf = af->frame->data[0];
		resampled_data_size = data_size;
	}

	audio_clock0 = is->audio_clock;
	/* update the audio clock with the pts */
	if (!isnan(af->pts))
		is->audio_clock = af->pts + (double)af->frame->nb_samples / af->frame->sample_rate;
	else
		is->audio_clock = NAN;
	is->audio_clock_serial = af->serial;
#ifdef DEBUG
	{
		static double last_clock;
		printf("audio: delay=%0.3f clock=%0.3f clock0=%0.3f\n",
			is->audio_clock - last_clock,
			is->audio_clock, audio_clock0);
		last_clock = is->audio_clock;
	}
#endif
	return resampled_data_size;
}


int CFFmfcPimpl::audio_open(void* opaque, AVChannelLayout* wanted_channel_layout, int wanted_sample_rate, AudioParams* audio_hw_params)
{
	VideoState* is = (VideoState*)opaque;

	// 1. Initialisation du périphérique OpenAL
	is->al_device = alcOpenDevice(NULL); // Périphérique par défaut
	if (!is->al_device) {
		av_log(NULL, AV_LOG_ERROR, "OpenAL : Impossible d'ouvrir le périphérique par défaut.\n");
		return -1;
	}

	is->al_context = alcCreateContext(is->al_device, NULL);
	if (!is->al_context) {
		alcCloseDevice(is->al_device);
		return -1;
	}
	alcMakeContextCurrent(is->al_context);

	// 2. Génération de la source audio.
	alGenSources(1, &is->al_source);
	if (alGetError() != AL_NO_ERROR)
	{
		av_log(nullptr, AV_LOG_ERROR, "OpenAL : alGenSources() a échoué.\n");
		return -1;
	}

	alGenBuffers(4, is->al_buffers);
	if (alGetError() != AL_NO_ERROR)
	{
		av_log(nullptr, AV_LOG_ERROR, "OpenAL : alGenBuffers() a échoué.\n");
		return -1;
	}

	// 3. Sortie PCM S16 interleavée.
	// OpenAL ici est limité au mono/stéréo.
	audio_hw_params->fmt = AV_SAMPLE_FMT_S16;
	audio_hw_params->freq = wanted_sample_rate;

	av_channel_layout_uninit(&audio_hw_params->ch_layout);

	if (wanted_channel_layout->nb_channels <= 1)
		av_channel_layout_default(&audio_hw_params->ch_layout, 1);
	else
		av_channel_layout_default(&audio_hw_params->ch_layout, 2);

	if (audio_hw_params->ch_layout.nb_channels <= 0)
		return -1;

	audio_hw_params->frame_size = av_samples_get_buffer_size(NULL, audio_hw_params->ch_layout.nb_channels, 1, audio_hw_params->fmt, 1);
	audio_hw_params->bytes_per_sec = av_samples_get_buffer_size(NULL, audio_hw_params->ch_layout.nb_channels, audio_hw_params->freq, audio_hw_params->fmt, 1);

	if (audio_hw_params->bytes_per_sec <= 0 || audio_hw_params->frame_size <= 0) {
		av_log(NULL, AV_LOG_ERROR, "av_samples_get_buffer_size failed\n");
		return -1;
	}

	// Retourne une taille de tampon cible simulée (ex: 2048 échantillons)
	return 2048 * audio_hw_params->frame_size;
}


#ifndef CMDUTILS

int CFFmfcPimpl::check_stream_specifier(AVFormatContext* s, AVStream* st, const char* spec)
{
	int ret = avformat_match_stream_specifier(s, st, spec);
	if (ret < 0)
		av_log(s, AV_LOG_ERROR, "Invalid stream specifier: %s.\n", spec);
	return ret;
}

AVDictionary* CFFmfcPimpl::filter_codec_opts(AVDictionary* opts, enum AVCodecID codec_id,
	AVFormatContext* s, AVStream* st, AVCodec* codec)
{
	AVDictionary* ret = nullptr;
	AVDictionaryEntry* t = nullptr;
	int flags = s->oformat
		? AV_OPT_FLAG_ENCODING_PARAM
		: AV_OPT_FLAG_DECODING_PARAM;
	char prefix = 0;
	const AVClass* cc = avcodec_get_class();

	if (!codec)
		codec = s->oformat
		? (AVCodec*)avcodec_find_encoder(codec_id)
		: (AVCodec*)avcodec_find_decoder(codec_id);
	if (!codec)
		return nullptr;

	switch (codec->type)
	{
	case AVMEDIA_TYPE_VIDEO:
		prefix = 'v';
		flags |= AV_OPT_FLAG_VIDEO_PARAM;
		break;
	case AVMEDIA_TYPE_AUDIO:
		prefix = 'a';
		flags |= AV_OPT_FLAG_AUDIO_PARAM;
		break;
	case AVMEDIA_TYPE_SUBTITLE:
		prefix = 's';
		flags |= AV_OPT_FLAG_SUBTITLE_PARAM;
		break;
	}

	while (t = av_dict_get(opts, "", t, AV_DICT_IGNORE_SUFFIX))
	{
		char* p = strchr(t->key, ':');

		/* check stream specification in opt name */
		if (p)
			switch (check_stream_specifier(s, st, p + 1))
			{
			case 1: *p = 0;
				break;
			case 0: continue;
			default: return nullptr;
			}

		if (av_opt_find(&cc, t->key, nullptr, flags, AV_OPT_SEARCH_FAKE_OBJ) ||
			(codec && codec->priv_class &&
				av_opt_find(&codec->priv_class, t->key, nullptr, flags,
					AV_OPT_SEARCH_FAKE_OBJ)))
			av_dict_set(&ret, t->key, t->value, 0);
		else if (t->key[0] == prefix &&
			av_opt_find(&cc, t->key + 1, nullptr, flags,
				AV_OPT_SEARCH_FAKE_OBJ))
			av_dict_set(&ret, t->key + 1, t->value, 0);

		if (p)
			*p = ':';
	}
	return ret;
}

void CFFmfcPimpl::uninit_opts(void)
{
	/*
#if CONFIG_SWSCALE
	sws_freeContext(sws_opts);
	sws_opts = nullptr;
#endif

	if(CONFIG_SWRESAMPLE)
		swr_free(&swr_opts);
	*/
	av_dict_free(&format_opts);
	av_dict_free(&codec_opts);
}

AVDictionary** CFFmfcPimpl::setup_find_stream_info_opts(AVFormatContext* s,
	AVDictionary* codec_opts)
{
	int i;
	AVDictionary** opts;

	if (!s->nb_streams)
		return nullptr;
	opts = static_cast<AVDictionary**>(av_mallocz(s->nb_streams * sizeof(*opts)));
	if (!opts)
	{
		av_log(nullptr, AV_LOG_ERROR,
			"Could not alloc memory for stream options.\n");
		return nullptr;
	}
	for (i = 0; i < s->nb_streams; i++)
		opts[i] = filter_codec_opts(codec_opts, s->streams[i]->codecpar->codec_id,
			s, s->streams[i], nullptr);
	return opts;
}
#endif

const char* CFFmfcPimpl::getExt(const char* fspec)
{
	const char* e = strrchr(fspec, '.');
	if (e == nullptr)
		e = ""; // fast method, could also use &(fspec[strlen(fspec)]).
	return e;
}

int CFFmfcPimpl::hw_decoder_init(AVCodecContext* ctx, const enum AVHWDeviceType type)
{
	int err = 0;

	if (hw_device_ctx == NULL)
	{
		if ((err = av_hwdevice_ctx_create(&hw_device_ctx, type,
			nullptr, nullptr, 0)) < 0)
		{
			fprintf(stderr, "Failed to create specified HW device.\n");
			return err;
		}
	}
	ctx->hw_device_ctx = av_buffer_ref(hw_device_ctx);

	return err;
}


enum AVPixelFormat CFFmfcPimpl::get_hw_format(AVCodecContext* ctx,
	const enum AVPixelFormat* pix_fmts)
{
	const enum AVPixelFormat* p;

	for (p = pix_fmts; *p != -1; p++)
	{
		if (*p == hw_pix_fmt)
			return *p;
	}

	hw_pix_fmt = AV_PIX_FMT_NONE;
	fprintf(stderr, "Failed to get HW surface format.\n");

	if (dlg != nullptr)
	{
		wxCommandEvent event(wxEVENT_ERRORDECODINGFRAME);
		wxPostEvent(dlg->GetMainWindow(), event);
	}

	return AV_PIX_FMT_NONE;
}


int CFFmfcPimpl::decoder_init(Decoder* d, AVCodecContext* avctx, PacketQueue* queue, std::condition_variable * empty_queue_cond)
{
	if (!d || !avctx || !queue)
		return AVERROR(EINVAL);

	// Decoder contient std::thread : pas de memset().
	d->pkt = nullptr;
	d->queue = queue;
	d->avctx = avctx;
	d->pkt_serial = -1;
	d->finished = 0;
	d->packet_pending = 0;
	d->empty_queue_cond = empty_queue_cond;
	d->start_pts = AV_NOPTS_VALUE;
	d->start_pts_tb = AVRational{ 0, 1 };
	d->next_pts = AV_NOPTS_VALUE;
	d->next_pts_tb = AVRational{ 0, 1 };

	d->pkt = av_packet_alloc();
	if (!d->pkt)
		return AVERROR(ENOMEM);
	return 0;
}

int CFFmfcPimpl::decoder_decode_frame(VideoState* is, Decoder* d, AVFrame* frame, AVSubtitle* sub)
{
	int ret = AVERROR(EAGAIN);

	for (;;)
	{
		if (d->queue->serial == d->pkt_serial)
		{
			do
			{
				if (d->queue->abort_request)
					return -1;

				switch (d->avctx->codec_type)
				{
				case AVMEDIA_TYPE_VIDEO:
					ret = avcodec_receive_frame(d->avctx, frame);

					if (isHardwareDecoding && hw_pix_fmt != AV_PIX_FMT_NONE)
					{
						if (is->hwaccel_retrieve_data && frame->format == hw_pix_fmt)
						{
							ret = is->hwaccel_retrieve_data(d->avctx, frame);
						}

						if (ret >= 0)
							is->hwaccel_retrieved_pix_fmt = (AVPixelFormat)frame->format;
					}
					else if (ret >= 0)
					{
						if (decoder_reorder_pts == -1)
						{
							frame->pts = frame->best_effort_timestamp;
						}
						else if (!decoder_reorder_pts)
						{
							frame->pts = frame->pkt_dts;
						}
					}
					/*
					if (ret < 0)
					{
						acceleratorHardware = "none";
						if (dlg != nullptr)
							dlg->ErrorDecodingFrame();
					}
					*/
					break;
				case AVMEDIA_TYPE_AUDIO:
					ret = avcodec_receive_frame(d->avctx, frame);
					if (ret >= 0)
					{
						AVRational tb = { 1, frame->sample_rate };
						if (frame->pts != AV_NOPTS_VALUE)
							frame->pts = av_rescale_q(frame->pts, d->avctx->pkt_timebase, tb);
						else if (d->next_pts != AV_NOPTS_VALUE)
							frame->pts = av_rescale_q(d->next_pts, d->next_pts_tb, tb);
						if (frame->pts != AV_NOPTS_VALUE)
						{
							d->next_pts = frame->pts + frame->nb_samples;
							d->next_pts_tb = tb;
						}
					}
					break;
				}
				if (ret == AVERROR_EOF)
				{
					d->finished = d->pkt_serial;
					avcodec_flush_buffers(d->avctx);
					return 0;
				}
				if (ret >= 0)
					return 1;
			} while (ret != AVERROR(EAGAIN));
		}

		do
		{
			if (d->queue->nb_packets == 0 && d->empty_queue_cond != nullptr)
				d->empty_queue_cond->notify_one();
			if (d->packet_pending)
			{
				d->packet_pending = 0;
			}
			else
			{
				int old_serial = d->pkt_serial;
				if (packet_queue_get(d->queue, d->pkt, 1, &d->pkt_serial) < 0)
					return -1;
				if (old_serial != d->pkt_serial)
				{
					avcodec_flush_buffers(d->avctx);
					d->finished = 0;
					d->next_pts = d->start_pts;
					d->next_pts_tb = d->start_pts_tb;
				}
			}
			if (d->queue->serial == d->pkt_serial)
				break;
			av_packet_unref(d->pkt);
		} while (1);

		if (d->avctx->codec_type == AVMEDIA_TYPE_SUBTITLE)
		{
			int got_frame = 0;
			ret = avcodec_decode_subtitle2(d->avctx, sub, &got_frame, d->pkt);
			if (ret < 0)
			{
				ret = AVERROR(EAGAIN);
			}
			else
			{
				if (got_frame && !d->pkt->data)
				{
					d->packet_pending = 1;
				}
				ret = got_frame ? 0 : (d->pkt->data ? AVERROR(EAGAIN) : AVERROR_EOF);
			}
			av_packet_unref(d->pkt);
		}
		else
		{
			if (avcodec_send_packet(d->avctx, d->pkt) == AVERROR(EAGAIN))
			{
				av_packet_unref(d->pkt);
				av_log(d->avctx, AV_LOG_ERROR,
					"Receive_frame and send_packet both returned EAGAIN, which is an API violation.\n");
				d->packet_pending = 1;
			}
			else
			{
				av_packet_unref(d->pkt);
			}
		}
	}
}

void CFFmfcPimpl::decoder_destroy(Decoder* d)
{
	av_packet_free(&d->pkt);
	avcodec_free_context(&d->avctx);
}

int CFFmfcPimpl::decoder_start(Decoder* d, int (*fn)(void*), const char* thread_name, void* arg)
{
	packet_queue_start(d->queue);
	d->decoder_thread = std::thread(fn, arg);
	if (!d->decoder_thread.joinable())
	{
		av_log(NULL, AV_LOG_ERROR, "std::thread(): %s\n", "Failed to create thread");
		return AVERROR(ENOMEM);
	}
	return 0;
}


int CFFmfcPimpl::configure_filtergraph(AVFilterGraph* graph, const char* filtergraph,
	AVFilterContext* source_ctx, AVFilterContext* sink_ctx)
{
	int ret, i;
	int nb_filters = graph->nb_filters;
	AVFilterInOut* outputs = NULL, * inputs = NULL;

	if (filtergraph) {
		outputs = avfilter_inout_alloc();
		inputs = avfilter_inout_alloc();
		if (!outputs || !inputs) {
			ret = AVERROR(ENOMEM);
			goto fail;
		}

		outputs->name = av_strdup("in");
		outputs->filter_ctx = source_ctx;
		outputs->pad_idx = 0;
		outputs->next = NULL;

		inputs->name = av_strdup("out");
		inputs->filter_ctx = sink_ctx;
		inputs->pad_idx = 0;
		inputs->next = NULL;

		if ((ret = avfilter_graph_parse_ptr(graph, filtergraph, &inputs, &outputs, NULL)) < 0)
			goto fail;
	}
	else {
		if ((ret = avfilter_link(source_ctx, 0, sink_ctx, 0)) < 0)
			goto fail;
	}

	/* Reorder the filters to ensure that inputs of the custom filters are merged first */
	for (i = 0; i < graph->nb_filters - nb_filters; i++)
		FFSWAP(AVFilterContext*, graph->filters[i], graph->filters[i + nb_filters]);

	ret = avfilter_graph_config(graph, NULL);
fail:
	avfilter_inout_free(&outputs);
	avfilter_inout_free(&inputs);
	return ret;
}


int CFFmfcPimpl::configure_audio_filters(VideoState* is, const char* afilters, int force_output_format)
{
	static const enum AVSampleFormat sample_fmts[] = { AV_SAMPLE_FMT_S16, AV_SAMPLE_FMT_NONE };
	int sample_rates[2] = { 0, -1 };
	AVFilterContext* filt_asrc = NULL, * filt_asink = NULL;
	char aresample_swr_opts[512] = "";
	const AVDictionaryEntry* e = NULL;
	AVBPrint bp;
	char asrc_args[256];
	int ret;

	avfilter_graph_free(&is->agraph);
	if (!(is->agraph = avfilter_graph_alloc()))
		return AVERROR(ENOMEM);
	is->agraph->nb_threads = filter_nbthreads;

	av_bprint_init(&bp, 0, AV_BPRINT_SIZE_AUTOMATIC);

	//while ((e = av_dict_iterate(swr_opts, e)))
	//	av_strlcatf(aresample_swr_opts, sizeof(aresample_swr_opts), "%s=%s:", e->key, e->value);
	if (strlen(aresample_swr_opts))
		aresample_swr_opts[strlen(aresample_swr_opts) - 1] = '\0';
	av_opt_set(is->agraph, "aresample_swr_opts", aresample_swr_opts, 0);

	av_channel_layout_describe_bprint(&is->audio_filter_src.ch_layout, &bp);

	ret = snprintf(asrc_args, sizeof(asrc_args),
		"sample_rate=%d:sample_fmt=%s:time_base=%d/%d:channel_layout=%s",
		is->audio_filter_src.freq, av_get_sample_fmt_name(is->audio_filter_src.fmt),
		1, is->audio_filter_src.freq, bp.str);

	ret = avfilter_graph_create_filter(&filt_asrc,
		avfilter_get_by_name("abuffer"), "ffplay_abuffer",
		asrc_args, NULL, is->agraph);
	if (ret < 0)
		goto end;


	// 1. Allouer SANS initialiser
	filt_asink = avfilter_graph_alloc_filter(
		is->agraph,
		avfilter_get_by_name("abuffersink"),
		"ffplay_abuffersink");

	if (!filt_asink) {
		ret = AVERROR(ENOMEM);
		goto end;
	}

	if ((ret = av_opt_set(
			filt_asink,
			"sample_formats",
			"s16",
			AV_OPT_SEARCH_CHILDREN)) < 0)
		goto end;

	if (force_output_format) {
		if ((ret = av_opt_set_array(
				filt_asink,
				"channel_layouts",
				AV_OPT_SEARCH_CHILDREN,
				0,
				1,
				AV_OPT_TYPE_CHLAYOUT,
				&is->audio_tgt.ch_layout)) < 0)
			goto end;

		if ((ret = av_opt_set_array(
				filt_asink,
				"samplerates",
				AV_OPT_SEARCH_CHILDREN,
				0,
				1,
				AV_OPT_TYPE_INT,
				&is->audio_tgt.freq)) < 0)
			goto end;
	}

	if ((ret = avfilter_init_str(filt_asink, NULL)) < 0)
		goto end;

	if ((ret = configure_filtergraph(
			is->agraph,
			afilters,
			filt_asrc,
			filt_asink)) < 0)
		goto end;

	is->in_audio_filter = filt_asrc;
	is->out_audio_filter = filt_asink;

end:
	if (ret < 0)
		avfilter_graph_free(&is->agraph);
	av_bprint_finalize(&bp, NULL);

	return ret;
}

static inline int cmp_audio_fmts(enum AVSampleFormat fmt1, int64_t channel_count1,
	enum AVSampleFormat fmt2, int64_t channel_count2)
{
	/* If channel count == 1, planar and non-planar formats are the same */
	if (channel_count1 == 1 && channel_count2 == 1)
		return av_get_packed_sample_fmt(fmt1) != av_get_packed_sample_fmt(fmt2);
	else
		return channel_count1 != channel_count2 || fmt1 != fmt2;
}

// -----------------------------------------------------------------------------
// Thread de décodage audio : audioq -> sampq.
// Il ne touche jamais au contexte OpenAL.
// -----------------------------------------------------------------------------
int CFFmfcPimpl::audio_decoder_thread(void* arg)
{
	VideoState* is = static_cast<VideoState*>(arg);
	if (!is)
		return AVERROR(EINVAL);

	AVFrame* frame = av_frame_alloc();
	if (!frame)
		return AVERROR(ENOMEM);

	for (;;)
	{
		if (is->abort_request || exit_video.load(std::memory_order_acquire))
			break;

		Frame* af = is->_pimpl->frame_queue_peek_writable(&is->sampq);
		if (!af)
			break;

		const int ret = is->_pimpl->decoder_decode_frame(
			is, &is->auddec, frame, nullptr);

		if (ret < 0)
			break;

		if (!ret)
			continue;

		af->pts = (frame->pts != AV_NOPTS_VALUE && frame->sample_rate > 0)
			? static_cast<double>(frame->pts) /
			static_cast<double>(frame->sample_rate)
			: NAN;

		af->duration = (frame->sample_rate > 0)
			? static_cast<double>(frame->nb_samples) /
			static_cast<double>(frame->sample_rate)
			: 0.0;

		af->serial = is->auddec.pkt_serial;

		av_frame_move_ref(af->frame, frame);
		is->_pimpl->frame_queue_push(&is->sampq);

	}

	av_frame_free(&frame);
	return 0;
}

int CFFmfcPimpl::audio_thread(void* arg)
{
    VideoState* is = static_cast<VideoState*>(arg);
    if (!is || !is->_pimpl)
        return AVERROR(EINVAL);

    // 1. OpenAL est thread-local : le contexte DOIT être rendu courant 
    // dans le thread exact qui effectue les opérations OpenAL.
    if (!is->al_context || !alcMakeContextCurrent(is->al_context))
    {
        av_log(nullptr, AV_LOG_ERROR, "OpenAL : Impossible d'activer le contexte audio dans ce thread.\n");
        return -1;
    }

    const int channels = is->audio_tgt.ch_layout.nb_channels;
    const ALenum al_format = (channels == 1) ? AL_FORMAT_MONO16 :
                             (channels == 2) ? AL_FORMAT_STEREO16 : 0;

    if (al_format == 0)
    {
        av_log(nullptr, AV_LOG_ERROR, "OpenAL : Nombre de canaux non supporté : %d\n", channels);
        alcMakeContextCurrent(nullptr);
        return -1;
    }

    if (is->audio_tgt.fmt != AV_SAMPLE_FMT_S16)
    {
        av_log(nullptr, AV_LOG_ERROR, "OpenAL : Format cible attendu invalide (S16 requis).\n");
        alcMakeContextCurrent(nullptr);
        return -1;
    }

    // Gestion initiale du volume
    is->audio_volume = static_cast<float>(av_clip(is->_pimpl->percentVolume, 0, 100)) / 100.0f;
    if (is->muted)
        is->audio_volume = 0.0f;
    alSourcef(is->al_source, AL_GAIN, is->audio_volume);

    int queued_count = 0;
    int retry_attempts = 0;

    // 2. Remplissage initial de la file d'attente OpenAL (4 tampons)
    for (int i = 0; i < 4 && !is->abort_request;)
    {
        const int audio_size = is->_pimpl->audio_decode_frame(is);

        if (audio_size <= 0 || !is->audio_buf)
        {
            // PROTECTION ANTI-UNDERFLOW : Si la sampq est momentanément vide au démarrage,
            // on attend quelques millisecondes que le décodeur audio produise du PCM.
            if (frame_queue_nb_remaining(&is->sampq) == 0 && retry_attempts < 100)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                retry_attempts++;
                continue; // On réessaye pour le même index de tampon 'i'
            }
            break;
        }

        // Réinitialisation du compteur si un décodage a réussi
        retry_attempts = 0;

        alBufferData(is->al_buffers[i], al_format, is->audio_buf, audio_size, is->audio_tgt.freq);
        ALenum error = alGetError();
        if (error != AL_NO_ERROR)
        {
            av_log(nullptr, AV_LOG_ERROR, "OpenAL : alBufferData() initial a échoué (0x%04X)\n", error);
            break;
        }

        alSourceQueueBuffers(is->al_source, 1, &is->al_buffers[i]);
        error = alGetError();
        if (error != AL_NO_ERROR)
        {
            av_log(nullptr, AV_LOG_ERROR, "OpenAL : alSourceQueueBuffers() initial a échoué (0x%04X)\n", error);
            break;
        }

        ++queued_count;
        ++i; // On passe au tampon OpenAL suivant
    }

    if (queued_count == 0)
    {
        av_log(nullptr, AV_LOG_ERROR, "OpenAL : Aucun buffer audio disponible au démarrage.\n");
        alcMakeContextCurrent(nullptr);
        return -1;
    }

    // Lancement de la lecture audio
    alSourcePlay(is->al_source);
    ALenum error = alGetError();
    if (error != AL_NO_ERROR)
    {
        av_log(nullptr, AV_LOG_ERROR, "OpenAL : alSourcePlay() a échoué (0x%04X)\n", error);
        alcMakeContextCurrent(nullptr);
        return -1;
    }

    // 3. Boucle principale de streaming
    while (!is->abort_request && !exit_video.load(std::memory_order_acquire))
    {
        if (is->paused)
        {
            ALint state = AL_INITIAL;
            alGetSourcei(is->al_source, AL_SOURCE_STATE, &state);

            if (state == AL_PLAYING)
                alSourcePause(is->al_source);

            av_usleep(10000); // 10ms d'attente passive si en pause
            continue;
        }

        // Mise à jour dynamique du volume
        is->audio_volume = static_cast<float>(av_clip(is->_pimpl->percentVolume, 0, 100)) / 100.0f;
        if (is->muted)
            is->audio_volume = 0.0f;
        alSourcef(is->al_source, AL_GAIN, is->audio_volume);

        // Vérification des buffers OpenAL déjà consommés par la carte son
        ALint processed = 0;
        alGetSourcei(is->al_source, AL_BUFFERS_PROCESSED, &processed);

        while (processed > 0 && !is->abort_request)
        {
            ALuint buffer = 0;
            alSourceUnqueueBuffers(is->al_source, 1, &buffer);
            error = alGetError();
            if (error != AL_NO_ERROR)
            {
                av_log(nullptr, AV_LOG_ERROR, "OpenAL : alSourceUnqueueBuffers() a échoué (0x%04X)\n", error);
                break;
            }

            // Décode la frame suivante
            const int audio_size = is->_pimpl->audio_decode_frame(is);

            if (audio_size > 0 && is->audio_buf)
            {
                alBufferData(buffer, al_format, is->audio_buf, audio_size, is->audio_tgt.freq);
                error = alGetError();
                if (error != AL_NO_ERROR)
                {
                    av_log(nullptr, AV_LOG_ERROR, "OpenAL : alBufferData() en flux a échoué (0x%04X)\n", error);
                    break;
                }

                // Ré-injection du buffer rempli dans la file OpenAL
                alSourceQueueBuffers(is->al_source, 1, &buffer);
                error = alGetError();
                if (error != AL_NO_ERROR)
                {
                    av_log(nullptr, AV_LOG_ERROR, "OpenAL : alSourceQueueBuffers() en flux a échoué (0x%04X)\n", error);
                    break;
                }
            }

            --processed;
        }

        // Sécurité en cas d'underflow critique (si la carte son a consommé plus vite que le décodeur)
        ALint state = AL_STOPPED;
        ALint queued = 0;
        alGetSourcei(is->al_source, AL_SOURCE_STATE, &state);
        alGetSourcei(is->al_source, AL_BUFFERS_QUEUED, &queued);

        if (state != AL_PLAYING && queued > 0 && !is->paused)
        {
            alSourcePlay(is->al_source);
        }

        // 4. Gestion de la synchronisation temporelle (Master Clock)
        is->_pimpl->audio_callback_time = av_gettime_relative();

        if (!isnan(is->audio_clock))
        {
            // On met à jour l'horloge audio de référence basée sur la progression d'OpenAL
            is->_pimpl->set_clock_at(
                &is->audclk,
                is->audio_clock,
                is->audio_clock_serial,
                is->_pimpl->audio_callback_time / 1000000.0
            );

            // Synchronisation de l'horloge externe globale
            is->_pimpl->sync_clock_to_slave(&is->extclk, &is->audclk);
        }

        av_usleep(5000); // Latence de boucle de 5ms pour libérer le processeur
    }

    // 5. Nettoyage du thread
    alSourceStop(is->al_source);
    alcMakeContextCurrent(nullptr);

    return 0;
}


bool CFFmfcPimpl::TestHardware(const wxString& acceleratorHardware, AVHWDeviceType& type, AVCodecContext* avctx,
	AVCodec* codec, AVDictionary*& opts, VideoState* is, AVStream* video)
{
	bool isSuccess = false;
	bool error = false;
	int ret = 0;

	type = av_hwdevice_find_type_by_name(acceleratorHardware);
	if (type == AV_HWDEVICE_TYPE_NONE)
	{
		fprintf(stderr, "Device type %s is not supported.\n", CConvertUtility::ConvertToStdString(acceleratorHardware).c_str());
		fprintf(stderr, "Available device types:");
		while ((type = av_hwdevice_iterate_types(type)) != AV_HWDEVICE_TYPE_NONE)
			fprintf(stderr, " %s", av_hwdevice_get_type_name(type));
		fprintf(stderr, "\n");

		error = true;
	}

	if (error)
		return false;

	for (int i = 0;; i++)
	{
		const AVCodecHWConfig* config = avcodec_get_hw_config(codec, i);
		if (!config)
		{
			fprintf(stderr, "Decoder %s does not support device type %s.\n",
				codec->name, av_hwdevice_get_type_name(type));
			return false;
		}
		if (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX &&
			config->device_type == type)
		{
			hw_pix_fmt = config->pix_fmt;
			break;
		}
	}

	is->avctx = avctx;

	//if (!(is->avctx = avcodec_alloc_context3(codec)))
	//	return AVERROR(ENOMEM);

	if (avcodec_parameters_to_context(is->avctx, video->codecpar) < 0)
		return false;

	//is->hwaccel_get_buffer = dxva2_get_buffer;
	is->avctx->get_format = get_hw_format;
	//	is->avctx->get_buffer2 = get_buffer;
	is->avctx->opaque = is;
	is->codec = codec;
	is->hwaccel_id = HWACCEL_AUTO;
	printf("Success for hardware decoding : %s ! \n", CConvertUtility::ConvertToStdString(acceleratorHardware).c_str());

	if (!error)
	{
		if (hw_decoder_init(avctx, type) < 0)
			return false;
	}
	if (!error)
	{
		if ((ret = avcodec_open2(avctx, codec, &opts)) < 0)
		{
			return false;
		}
	}

	isSuccess = true;

	if (isSuccess)
	{
		printf("Success for hardware decoding : %s ! \n", CConvertUtility::ConvertToStdString(acceleratorHardware).c_str());
	}
	return isSuccess;
}

double CFFmfcPimpl::get_rotation(AVStream* st)
{


	int32_t* displaymatrix = 0;
	const AVPacketSideData* psd = av_packet_side_data_get(st->codecpar->coded_side_data,
		st->codecpar->nb_coded_side_data,
		AV_PKT_DATA_DISPLAYMATRIX);
	if (psd)
		displaymatrix = (int32_t*)psd->data;

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

/* open a given stream. Return 0 if OK */
//´ò¿ªÒ»¸öStream£¬ÊÓÆµ»òÒôÆµ
int CFFmfcPimpl::stream_component_open(VideoState* is, int stream_index)
{
	AVFormatContext* ic = is->ic;
	AVCodecContext* avctx;
	AVCodec* codec = nullptr;
	const char* forced_codec_name = NULL;
	AVDictionary* opts = NULL;
	AVDictionaryEntry* t = NULL;
	int sample_rate;// , nb_channels;
	AVChannelLayout ch_layout;
	//AVChannelLayout* channel_layout;
	int ret = 0;
	int stream_lowres = lowres;
	enum AVHWDeviceType type;
	if (stream_index < 0 || stream_index >= ic->nb_streams)
		return -1;

	memset(&ch_layout, 0, sizeof(AVChannelLayout));
	bool isSuccess = false;

	avctx = avcodec_alloc_context3(NULL);
	if (!avctx)
		return AVERROR(ENOMEM);

	ret = avcodec_parameters_to_context(avctx, ic->streams[stream_index]->codecpar);
	if (ret < 0)
		goto fail;
	avctx->pkt_timebase = ic->streams[stream_index]->time_base;


	if (acceleratorHardware != "")
	{
		if (avctx->codec_id == AV_CODEC_ID_AV1 && acceleratorHardware == "cuda")
		{
			codec = (AVCodec*)avcodec_find_decoder_by_name("av1_cuvid");
		}
		else if (avctx->codec_id == AV_CODEC_ID_AV1 && acceleratorHardware == "qsv")
		{
			codec = (AVCodec*)avcodec_find_decoder_by_name("av1_qsv");
		}
	}

	if (codec == nullptr)
		codec = (AVCodec*)avcodec_find_decoder(avctx->codec_id);

	switch (avctx->codec_type)
	{
	case AVMEDIA_TYPE_AUDIO: is->last_audio_stream = stream_index;
		forced_codec_name = audio_codec_name;
		break;
	case AVMEDIA_TYPE_SUBTITLE: is->last_subtitle_stream = stream_index;
		forced_codec_name = subtitle_codec_name;
		break;
	case AVMEDIA_TYPE_VIDEO: is->last_video_stream = stream_index;
		forced_codec_name = video_codec_name;
		break;
	}

	if (forced_codec_name)
		codec = (AVCodec*)avcodec_find_decoder_by_name(forced_codec_name);

	if (!codec)
	{
		if (forced_codec_name)
			av_log(NULL, AV_LOG_WARNING,
				"No codec could be found with name '%s'\n", forced_codec_name);
		else
			av_log(NULL, AV_LOG_WARNING,
				"No decoder could be found for codec %s\n", avcodec_get_name(avctx->codec_id));
		ret = AVERROR(EINVAL);
	}

	if (ret >= 0)
	{
		avctx->codec_id = codec->id;
		if (stream_lowres > codec->max_lowres)
		{
			av_log(avctx, AV_LOG_WARNING, "The maximum value for lowres supported by the decoder is %d\n",
				codec->max_lowres);
			stream_lowres = codec->max_lowres;
		}
		avctx->lowres = stream_lowres;

		if (fast)
			avctx->flags2 |= AV_CODEC_FLAG2_FAST;

		opts = filter_codec_opts(codec_opts, avctx->codec_id, ic, ic->streams[stream_index], codec);
		if (!av_dict_get(opts, "threads", NULL, 0))
			av_dict_set(&opts, "threads", "auto", 0);
		if (stream_lowres)
			av_dict_set_int(&opts, "lowres", stream_lowres, 0);


		if (avctx->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			if (acceleratorHardware != "" && acceleratorHardware != "none")
			{
				printf("Test hardware decoding : %s ! \n", acceleratorHardware.utf8_string().c_str());
				AVStream* video = ic->streams[stream_index];
				isSuccess = TestHardware(acceleratorHardware, type, avctx, codec, opts, is, video);
			}
		}

		if (!isSuccess)
		{
			isHardwareDecoding = false;
			printf("No success for hardware decoding ! \n");
			ret = avcodec_open2(avctx, codec, &opts);

			if (ret < 0)
			{
				goto fail;
			}
		}
		else
		{
			isHardwareDecoding = true;
			is->hwaccel_retrieve_data = &hwaccel_retrieve_data;
		}

		if ((t = av_dict_get(opts, "", NULL, AV_DICT_IGNORE_SUFFIX)))
		{
			av_log(NULL, AV_LOG_ERROR, "Option %s not found.\n", t->key);
			ret = AVERROR_OPTION_NOT_FOUND;
			goto fail;
		}

		is->eof = 0;
		ic->streams[stream_index]->discard = AVDISCARD_DEFAULT;
		switch (avctx->codec_type)
		{
		case AVMEDIA_TYPE_AUDIO:
		{
			{
				AVFilterContext* sink;

				is->audio_filter_src.freq = avctx->sample_rate;
				ret = av_channel_layout_copy(&is->audio_filter_src.ch_layout, &avctx->ch_layout);
				if (ret < 0)
					goto fail;
				is->audio_filter_src.fmt = avctx->sample_fmt;
				if ((ret = configure_audio_filters(is, afilters, 0)) < 0)
					goto fail;
				sink = is->out_audio_filter;
				sample_rate = av_buffersink_get_sample_rate(sink);
				ret = av_buffersink_get_ch_layout(sink, &ch_layout);
				if (ret < 0)
					goto fail;
			}

			/* prepare audio output */
			if ((ret = audio_open(is, &ch_layout, sample_rate, &is->audio_tgt)) < 0)
				goto fail;
			is->audio_hw_buf_size = ret;
			is->audio_src = is->audio_tgt;
			is->audio_buf_size = 0;
			is->audio_buf_index = 0;

			/* init averaging filter */
			is->audio_diff_avg_coef = exp(log(0.01) / AUDIO_DIFF_AVG_NB);
			is->audio_diff_avg_count = 0;
			/* since we do not have a precise anough audio FIFO fullness,
			   we correct audio sync only if larger than this threshold */
			is->audio_diff_threshold = (double)(is->audio_hw_buf_size) / is->audio_tgt.bytes_per_sec;

			is->audio_stream = stream_index;
			is->audio_st = ic->streams[stream_index];

			if ((ret = decoder_init(&is->auddec, avctx, &is->audioq, &is->continue_read_thread)) < 0)
				goto fail;
			if (is->ic->iformat->flags & AVFMT_NOTIMESTAMPS) {
				is->auddec.start_pts = is->audio_st->start_time;
				is->auddec.start_pts_tb = is->audio_st->time_base;
			}
			// Thread 1 : décodage FFmpeg audioq -> sampq.
			if ((ret = decoder_start(&is->auddec, audio_decoder_thread, "audio_decoder", is)) < 0)
				goto out;

			// Thread 2 : lecture OpenAL sampq -> périphérique audio.
		try
		{
			is->audio_tid = std::thread(audio_thread, is);
		}
		catch (const std::system_error& e)
		{
			av_log(nullptr, AV_LOG_ERROR,
				"Impossible de créer le thread audio OpenAL : %s\n", e.what());
			decoder_abort(&is->auddec, &is->sampq);
			ret = AVERROR(ENOMEM);
			goto out;
		}
		}
		break;

		case AVMEDIA_TYPE_VIDEO:
		{
			is->video_stream = stream_index;
			is->video_st = ic->streams[stream_index];

			long rotation = get_rotation(is->video_st);
			if (dlg != nullptr)
			{
				wxCommandEvent event(EVENT_VIDEOROTATION);
				event.SetExtraLong(rotation);
				wxPostEvent(dlg->GetMainWindow(), event);
			}


			/*
			auto matrix = reinterpret_cast<int32_t*>(av_stream_get_side_data(
				is->video_st, AV_PKT_DATA_DISPLAYMATRIX, nullptr));

			if (matrix)
			{
				long rotation = lround(av_display_rotation_get(matrix));
				if (dlg != nullptr)
					dlg->SetRotation(rotation);
			}
			*/

			if ((ret = decoder_init(&is->viddec, avctx, &is->videoq, &is->continue_read_thread)) < 0)
				goto fail;

			if ((ret = decoder_start(&is->viddec, video_thread, "video_decoder", is)) < 0)
				goto out;
			is->queue_attachments_req = 1;
			break;
		}


		case AVMEDIA_TYPE_SUBTITLE:
			is->subtitle_stream = stream_index;
			is->subtitle_st = ic->streams[stream_index];

			if ((ret = decoder_init(&is->subdec, avctx, &is->subtitleq, &is->continue_read_thread)) < 0)
				goto fail;
			if ((ret = decoder_start(&is->subdec, subtitle_thread, "subtitle_decoder", is)) < 0)
				goto out;
			break;

		default:
			break;
		}
		goto out;
	}


fail:
	avcodec_free_context(&avctx);
out:
	av_dict_free(&opts);

	return ret;
}
void CFFmfcPimpl::stream_component_close(VideoState* is, int stream_index)
{
	AVFormatContext* ic = is->ic;
	AVCodecParameters* codecpar;

	if (stream_index < 0 || stream_index >= ic->nb_streams)
		return;
	codecpar = ic->streams[stream_index]->codecpar;

	switch (codecpar->codec_type)
	{
	case AVMEDIA_TYPE_AUDIO:
		// 1. Arrêt du décodage : abort_request réveille sampq et stoppe le thread de décodage FFmpeg.
		decoder_abort(&is->auddec, &is->sampq);

		// 2. Le thread OpenAL peut maintenant sortir de sa boucle et de audio_decode_frame().
		if (is->audio_tid.joinable())
			is->audio_tid.join();

		// NETTOYAGE OPENAL SÉCURISÉ
		if (is->al_source) {
			alSourceStop(is->al_source);
			
			// CORRECTION : Détacher impérativement tous les buffers de la source 
			// avant de tenter de les supprimer, sinon OpenAL ignorera alDeleteBuffers.
			alSourcei(is->al_source, AL_BUFFER, 0);
			
			alDeleteSources(1, &is->al_source);
			is->al_source = 0;
		}
		
		if (is->al_buffers[0] != 0) {
			alDeleteBuffers(4, is->al_buffers);
			memset(is->al_buffers, 0, sizeof(is->al_buffers));
		}
		
		if (is->al_context) {
			alcMakeContextCurrent(NULL);
			alcDestroyContext(is->al_context);
			is->al_context = nullptr;
		}
		
		if (is->al_device) {
			alcCloseDevice(is->al_device);
			is->al_device = nullptr;
		}

		// Nettoyage FFmpeg Audio
		decoder_destroy(&is->auddec);
		swr_free(&is->swr_ctx);
		av_freep(&is->audio_buf1);
		is->audio_buf1_size = 0;
		is->audio_buf = NULL;
		break;

	case AVMEDIA_TYPE_VIDEO:
		if (is->hwaccel_uninit)
			is->hwaccel_uninit(is->viddec.avctx);

		if (is->viddec.avctx->hw_device_ctx != nullptr)
			av_buffer_unref(&is->viddec.avctx->hw_device_ctx);

		decoder_abort(&is->viddec, &is->pictq);
		decoder_destroy(&is->viddec);
		break;

	case AVMEDIA_TYPE_SUBTITLE:
		decoder_abort(&is->subdec, &is->subpq);
		decoder_destroy(&is->subdec);
		break;
		
	default:
		break;
	}

	ic->streams[stream_index]->discard = AVDISCARD_ALL;
	switch (codecpar->codec_type)
	{
	case AVMEDIA_TYPE_AUDIO:
		is->audio_st = NULL;
		is->audio_stream = -1;
		break;
	case AVMEDIA_TYPE_VIDEO:
		is->video_st = NULL;
		is->video_stream = -1;
		break;
	case AVMEDIA_TYPE_SUBTITLE:
		is->subtitle_st = NULL;
		is->subtitle_stream = -1;
		break;
	default:
		break;
	}
}


//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
void CFFmfcPimpl::stream_change_stream(VideoState* is, int codec_type, int newIndex)
{
	//AVFormatContext *ic = is->ic;
	int start_index, stream_index = newIndex;
	int old_index;
	//AVStream *st;

	if (codec_type == AVMEDIA_TYPE_VIDEO)
	{
		start_index = newIndex; //is->last_video_stream;
		old_index = is->video_stream;
	}
	else if (codec_type == AVMEDIA_TYPE_AUDIO)
	{
		start_index = newIndex; //is->last_audio_stream;
		old_index = is->audio_stream;
	}
	else
	{
		start_index = newIndex; //is->last_subtitle_stream;
		old_index = is->subtitle_stream;
	}
	stream_component_close(is, old_index);
	stream_component_open(is, stream_index);

	//if (codec_type == AVMEDIA_TYPE_VIDEO)
	//	is->que_attachments_req = 1;
}

void CFFmfcPimpl::init_clock(Clock* c, int* queue_serial)
{
	c->speed = 1.0;
	c->paused = 0;
	c->queue_serial = queue_serial;
	set_clock(c, NAN, -1);
}


int CFFmfcPimpl::decode_interrupt_cb(void* ctx)
{
	const auto* is = static_cast<const VideoState*>(ctx);
	return !is || is->abort_request != 0;
}

int CFFmfcPimpl::is_realtime(AVFormatContext* s, char* filename)
{
	if (!strcmp(s->iformat->name, "rtp")
		|| !strcmp(s->iformat->name, "rtsp")
		|| !strcmp(s->iformat->name, "sdp")
		)
		return 1;

	if (s->pb && (!strncmp(filename, "rtp:", 4)
		|| !strncmp(filename, "udp:", 4)
		)
		)
		return 1;
	return 0;
}



int CFFmfcPimpl::stream_has_enough_packets(AVStream* st, int stream_id, PacketQueue* queue)
{
	return stream_id < 0 ||
		queue->abort_request ||
		(st->disposition & AV_DISPOSITION_ATTACHED_PIC) ||
		queue->nb_packets > MIN_FRAMES && (!queue->duration || av_q2d(st->time_base) * queue->duration > 1.0);
}


/* this thread gets the stream from the disk or the network */
//½âÂëÏß³Ì£¬»ñµÃÊÓÒôÆµPacket²¢·ÅÈë¶ÓÁÐ
int CFFmfcPimpl::read_thread(void* arg)
{
	auto is = static_cast<VideoState*>(arg);
	AVFormatContext* ic = NULL;
	int err, i, ret;
	int st_index[AVMEDIA_TYPE_NB];
	AVPacket* pkt = NULL;
	int64_t stream_start_time;
	int pkt_in_play_range = 0;
	AVDictionaryEntry* t;
	std::mutex wait_mutex;
	int scan_all_pmts_set = 0;
	int64_t pkt_ts;

	memset(st_index, -1, sizeof(st_index));
	is->eof = 0;

	pkt = av_packet_alloc();
	if (!pkt)
	{
		av_log(NULL, AV_LOG_FATAL, "Could not allocate packet.\n");
		ret = AVERROR(ENOMEM);
		goto fail;
	}
	ic = avformat_alloc_context();
	if (!ic)
	{
		av_log(NULL, AV_LOG_FATAL, "Could not allocate context.\n");
		ret = AVERROR(ENOMEM);
		goto fail;
	}
	ic->interrupt_callback.callback = decode_interrupt_cb;
	ic->interrupt_callback.opaque = is;
	if (!av_dict_get(is->_pimpl->format_opts, "scan_all_pmts", NULL, AV_DICT_MATCH_CASE))
	{
		av_dict_set(&is->_pimpl->format_opts, "scan_all_pmts", "1", AV_DICT_DONT_OVERWRITE);
		scan_all_pmts_set = 1;
	}
	err = avformat_open_input(&ic, is->filename, is->iformat, &is->_pimpl->format_opts);
	if (err < 0)
	{
		//print_error(is->filename, err);
		ret = -1;
		goto fail;
	}
	if (scan_all_pmts_set)
		av_dict_set(&is->_pimpl->format_opts, "scan_all_pmts", NULL, AV_DICT_MATCH_CASE);

	if ((t = av_dict_get(is->_pimpl->format_opts, "", NULL, AV_DICT_IGNORE_SUFFIX)))
	{
		av_log(NULL, AV_LOG_ERROR, "Option %s not found.\n", t->key);
		ret = AVERROR_OPTION_NOT_FOUND;
		goto fail;
	}
	is->ic = ic;

	if (is->_pimpl->genpts)
		ic->flags |= AVFMT_FLAG_GENPTS;

	if (is->_pimpl->find_stream_info)
	{
		AVDictionary** opts = is->_pimpl->setup_find_stream_info_opts(ic, is->_pimpl->codec_opts);
		int orig_nb_streams = ic->nb_streams;

		err = avformat_find_stream_info(ic, opts);

		for (i = 0; i < orig_nb_streams; i++)
			av_dict_free(&opts[i]);
		av_freep(&opts);

		if (err < 0)
		{
			av_log(NULL, AV_LOG_WARNING,
				"%s: could not find codec parameters\n", is->filename);
			ret = -1;
			goto fail;
		}
	}

	if (ic->pb)
		ic->pb->eof_reached = 0; // FIXME hack, ffplay maybe should not use avio_feof() to test for the end

	if (is->_pimpl->seek_by_bytes < 0)
		is->_pimpl->seek_by_bytes = !!(ic->iformat->flags & AVFMT_TS_DISCONT) && strcmp(
			"ogg", ic->iformat->name);

	is->max_frame_duration = (ic->iformat->flags & AVFMT_TS_DISCONT) ? 10.0 : 3600.0;

	//if (!window_title && (t = av_dict_get(ic->metadata, "title", NULL, 0)))
	//	window_title = av_asprintf("%s - %s", t->value, input_filename);

	/* if seeking requested, we execute it */
	if (is->_pimpl->start_time != AV_NOPTS_VALUE)
	{
		int64_t timestamp;

		timestamp = is->_pimpl->start_time;
		/* add the stream start time */
		if (ic->start_time != AV_NOPTS_VALUE)
			timestamp += ic->start_time;
		ret = avformat_seek_file(ic, -1, INT64_MIN, timestamp, INT64_MAX, 0);
		if (ret < 0)
		{
			av_log(NULL, AV_LOG_WARNING, "%s: could not seek to position %0.3f\n",
				is->filename, (double)timestamp / AV_TIME_BASE);
		}
	}

	is->realtime = is->_pimpl->is_realtime(ic, is->filename);

	if (is->_pimpl->show_status)
		av_dump_format(ic, 0, is->filename, 0);

	for (i = 0; i < ic->nb_streams; i++)
	{
		AVStream* st = ic->streams[i];
		enum AVMediaType type = st->codecpar->codec_type;
		st->discard = AVDISCARD_ALL;
		if (type >= 0 && is->_pimpl->wanted_stream_spec[type] && st_index[type] == -1)
			if (avformat_match_stream_specifier(ic, st, is->_pimpl->wanted_stream_spec[type]) > 0)
				st_index[type] = i;
	}
	for (i = 0; i < AVMEDIA_TYPE_NB; i++)
	{
		if (is->_pimpl->wanted_stream_spec[i] && st_index[i] == -1)
		{
			//av_log(NULL, AV_LOG_ERROR, "Stream specifier %s does not match any %s stream\n", is->_pimpl->wanted_stream_spec[i], av_get_media_type_string(i));
			st_index[i] = INT_MAX;
		}
	}

	if (!is->_pimpl->video_disable)
		st_index[AVMEDIA_TYPE_VIDEO] =
		av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO,
			st_index[AVMEDIA_TYPE_VIDEO], -1, NULL, 0);
	if (!is->_pimpl->audio_disable)
		st_index[AVMEDIA_TYPE_AUDIO] =
		av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO,
			st_index[AVMEDIA_TYPE_AUDIO],
			st_index[AVMEDIA_TYPE_VIDEO],
			NULL, 0);
	if (!is->_pimpl->video_disable && !is->_pimpl->subtitle_disable)
		st_index[AVMEDIA_TYPE_SUBTITLE] =
		av_find_best_stream(ic, AVMEDIA_TYPE_SUBTITLE,
			st_index[AVMEDIA_TYPE_SUBTITLE],
			(st_index[AVMEDIA_TYPE_AUDIO] >= 0
				? st_index[AVMEDIA_TYPE_AUDIO]
				: st_index[AVMEDIA_TYPE_VIDEO]),
			NULL, 0);


	is->show_mode = (VideoState::ShowMode)SHOW_MODE_NONE;
	/*
	if (st_index[AVMEDIA_TYPE_VIDEO] >= 0) {
		AVStream* st = ic->streams[st_index[AVMEDIA_TYPE_VIDEO]];
		AVCodecParameters* codecpar = st->codecpar;
		AVRational sar = av_guess_sample_aspect_ratio(ic, st, NULL);
		if (codecpar->width)
			is->_pimpl->set_default_window_size(codecpar->width, codecpar->height, sar);
	}*/

	/* open the streams */
	if (st_index[AVMEDIA_TYPE_AUDIO] >= 0)
	{
		is->_pimpl->stream_component_open(is, st_index[AVMEDIA_TYPE_AUDIO]);
		is->_pimpl->disable_framedelay = 0;
	}
	else
		is->_pimpl->disable_framedelay = 1;

	ret = -1;
	if (st_index[AVMEDIA_TYPE_VIDEO] >= 0)
	{
		ret = is->_pimpl->stream_component_open(is, st_index[AVMEDIA_TYPE_VIDEO]);
	}
	if (is->show_mode == SHOW_MODE_NONE)
		is->show_mode = (VideoState::ShowMode)(ret >= 0 ? SHOW_MODE_VIDEO : SHOW_MODE_RDFT);

	if (st_index[AVMEDIA_TYPE_SUBTITLE] >= 0)
	{
		is->_pimpl->stream_component_open(is, st_index[AVMEDIA_TYPE_SUBTITLE]);
	}

	if (is->video_stream < 0 && is->audio_stream < 0)
	{
		av_log(NULL, AV_LOG_FATAL, "Failed to open file '%s' or configure filtergraph\n",
			is->filename);
		ret = -1;
		goto fail;
	}

	if (is->_pimpl->infinite_buffer < 0 && is->realtime)
		is->_pimpl->infinite_buffer = 1;

	is->refresh_tid = new std::thread(refresh_thread, is);

	if (is->_pimpl->dlg != nullptr)
	{
		//is->_pimpl->dlg->SetVideoDuration(is->ic->duration, is->ic->start_time);
		DataTimeDuration* dtTime = new DataTimeDuration();
		dtTime->duration = is->ic->duration;
		dtTime->startTime = is->ic->start_time;
		wxCommandEvent evt(wxEVENT_UPDATEMOVIETIME);
		evt.SetClientData(dtTime);
		wxPostEvent(is->_pimpl->dlg->GetMainWindow(), evt);// is->_pimpl->dlg->GetMainWindow()->GetEventHandler()->AddPendingEvent(evt);
	}


	for (;;)
	{
		if (is->abort_request)
			break;
		if (is->paused != is->last_paused)
		{
			is->last_paused = is->paused;
			if (is->paused)
				is->read_pause_return = av_read_pause(ic);
			else
				av_read_play(ic);
		}
#if CONFIG_RTSP_DEMUXER || CONFIG_MMSH_PROTOCOL
		if (is->paused &&
			(!strcmp(ic->iformat->name, "rtsp") ||
				(ic->pb && !strncmp(input_filename, "mmsh:", 5)))) {
			/* wait 10 ms to avoid trying to get another packet */
			/* XXX: horrible */
			SDL_Delay(10);
			continue;
		}
#endif
		if (is->seek_req)
		{
			int64_t seek_target = is->seek_pos;
			int64_t seek_min = is->seek_rel > 0 ? seek_target - is->seek_rel + 2 : INT64_MIN;
			int64_t seek_max = is->seek_rel < 0 ? seek_target - is->seek_rel - 2 : INT64_MAX;
			// FIXME the +-2 is due to rounding being not done in the correct direction in generation
			//      of the seek_pos/seek_rel variables

			ret = avformat_seek_file(is->ic, -1, seek_min, seek_target, seek_max, is->seek_flags);
			if (ret < 0)
			{
				av_log(NULL, AV_LOG_ERROR,
					"%s: error while seeking\n", is->ic->url);
			}
			else
			{
				if (is->audio_stream >= 0)
					is->_pimpl->packet_queue_flush(&is->audioq);
				if (is->subtitle_stream >= 0)
					is->_pimpl->packet_queue_flush(&is->subtitleq);
				if (is->video_stream >= 0)
					is->_pimpl->packet_queue_flush(&is->videoq);
				if (is->seek_flags & AVSEEK_FLAG_BYTE)
				{
					is->_pimpl->set_clock(&is->extclk, NAN, 0);
				}
				else
				{
					is->_pimpl->set_clock(&is->extclk, seek_target / (double)AV_TIME_BASE, 0);
				}
			}
			is->seek_req = 0;
			is->queue_attachments_req = 1;
			is->eof = 0;
			if (is->paused)
				is->_pimpl->step_to_next_frame(is);
		}
		if (is->queue_attachments_req)
		{
			if (is->video_st && is->video_st->disposition & AV_DISPOSITION_ATTACHED_PIC)
			{
				if ((ret = av_packet_ref(pkt, &is->video_st->attached_pic)) < 0)
					goto fail;
				is->_pimpl->packet_queue_put(&is->videoq, pkt);
				is->_pimpl->packet_queue_put_nullpacket(&is->videoq, pkt, is->video_stream);
			}
			is->queue_attachments_req = 0;
		}

		/* if the queue are full, no need to read more */
		if (is->_pimpl->infinite_buffer < 1 &&
			(is->audioq.size + is->videoq.size + is->subtitleq.size > MAX_QUEUE_SIZE
				|| (is->_pimpl->stream_has_enough_packets(is->audio_st, is->audio_stream, &is->audioq) &&
					is->_pimpl->stream_has_enough_packets(is->video_st, is->video_stream, &is->videoq) &&
					is->_pimpl->stream_has_enough_packets(is->subtitle_st, is->subtitle_stream, &is->subtitleq))))
		{

			/* wait 10 ms avec unique_lock au lieu de lock_guard */
			std::unique_lock<std::mutex> lock(wait_mutex);
			is->continue_read_thread.wait_for(lock, std::chrono::milliseconds(10));

			continue;
		}
		if (!is->paused &&
			(!is->audio_st || (is->auddec.finished == is->audioq.serial && is->_pimpl->
				frame_queue_nb_remaining(&is->sampq) ==
				0)) &&
			(!is->video_st || (is->viddec.finished == is->videoq.serial && is->_pimpl->
				frame_queue_nb_remaining(&is->pictq) ==
				0)))
		{
			if (is->_pimpl->loop != 1 && (!is->_pimpl->loop || --is->_pimpl->loop))
			{
				is->_pimpl->stream_seek(
					is, is->_pimpl->start_time != AV_NOPTS_VALUE ? is->_pimpl->start_time : 0, 0, 0);
			}
			else if (is->_pimpl->autoexit)
			{
				ret = AVERROR_EOF;
				goto fail;
			}
		}
		ret = av_read_frame(ic, pkt);
		if (ret < 0)
		{
			if ((ret == AVERROR_EOF || avio_feof(ic->pb)) && !is->eof)
			{
				if (is->video_stream >= 0)
					is->_pimpl->packet_queue_put_nullpacket(&is->videoq, pkt, is->video_stream);
				if (is->audio_stream >= 0)
					is->_pimpl->packet_queue_put_nullpacket(&is->audioq, pkt, is->audio_stream);
				if (is->subtitle_stream >= 0)
					is->_pimpl->packet_queue_put_nullpacket(&is->subtitleq, pkt, is->subtitle_stream);
				is->eof = 1;
			}
			if (ic->pb && ic->pb->error)
			{
				if (is->_pimpl->autoexit)
					goto fail;
				else
					break;
			}
			std::unique_lock<std::mutex> lock(wait_mutex);
			is->continue_read_thread.wait_for(lock, std::chrono::milliseconds(10));
			continue;
		}
		else
		{
			is->eof = 0;
		}
		/* check if packet is in play range specified by user, then queue, otherwise discard */
		stream_start_time = ic->streams[pkt->stream_index]->start_time;
		pkt_ts = pkt->pts == AV_NOPTS_VALUE ? pkt->dts : pkt->pts;
		pkt_in_play_range = is->_pimpl->duration == AV_NOPTS_VALUE ||
			(pkt_ts - (stream_start_time != AV_NOPTS_VALUE ? stream_start_time : 0)) *
			av_q2d(ic->streams[pkt->stream_index]->time_base) -
			(double)(is->_pimpl->start_time != AV_NOPTS_VALUE ? is->_pimpl->start_time : 0) / 1000000
			<= ((double)is->_pimpl->duration / 1000000);
		if (pkt->stream_index == is->audio_stream && pkt_in_play_range)
		{
			is->_pimpl->packet_queue_put(&is->audioq, pkt);
		}
		else if (pkt->stream_index == is->video_stream && pkt_in_play_range
			&& !(is->video_st->disposition & AV_DISPOSITION_ATTACHED_PIC))
		{
			is->_pimpl->packet_queue_put(&is->videoq, pkt);
		}
		else if (pkt->stream_index == is->subtitle_stream && pkt_in_play_range)
		{
			is->_pimpl->packet_queue_put(&is->subtitleq, pkt);
		}
		else
		{
			av_packet_unref(pkt);
		}
	}

	ret = 0;
fail:
	if (ic && !is->ic)
		avformat_close_input(&ic);

	av_packet_free(&pkt);
	wxCommandEvent evt(FF_STOP_EVENT);
	is->_pimpl->parent->GetEventHandler()->AddPendingEvent(evt);

	return 0;
}


CFFmfcPimpl::VideoState* CFFmfcPimpl::stream_open(const char* filename, AVInputFormat* iformat)
{
	VideoState* is;

	is = new (std::nothrow) VideoState();
	if (!is)
		return NULL;
	is->last_video_stream = is->video_stream = -1;
	is->last_audio_stream = is->audio_stream = -1;
	is->last_subtitle_stream = is->subtitle_stream = -1;
	is->filename = av_strdup(filename);
	is->_pimpl = this;
	if (!is->filename)
		return NULL;

	is->iformat = iformat;
	is->ytop = 0;
	is->xleft = 0;

	sws_freeContext(localContext);
	localContext = nullptr;
	localWidth = 0;
	localHeight = 0;
	localFormat = AV_PIX_FMT_NONE;

	colorRange = CMediaInfo::GetColorRange(filename);
	colorSpace = CMediaInfo::GetColorSpace(filename);

	/* start video display */
	if (frame_queue_init(&is->pictq, &is->videoq, VIDEO_PICTURE_QUEUE_SIZE, 1) < 0)
	{
		stream_close(is);
		return NULL;
	}
	if (frame_queue_init(&is->subpq, &is->subtitleq, SUBPICTURE_QUEUE_SIZE, 0) < 0)
	{
		stream_close(is);
		return NULL;
	}
	if (frame_queue_init(&is->sampq, &is->audioq, SAMPLE_QUEUE_SIZE, 1) < 0)
	{
		stream_close(is);
		return NULL;
	}

	if (packet_queue_init(&is->videoq) < 0 ||
		packet_queue_init(&is->audioq) < 0 ||
		packet_queue_init(&is->subtitleq) < 0)
	{
		stream_close(is);
		return NULL;
	}

	// Note : is->continue_read_thread étant un objet std::condition_variable standard,
	// elle s'initialise automatiquement sans allocation manuelle (plus besoin de SDL_CreateCond).

	init_clock(&is->vidclk, &is->videoq.serial);
	init_clock(&is->audclk, &is->audioq.serial);
	init_clock(&is->extclk, &is->extclk.serial);

	is->audio_clock_serial = -1;

	// CORRECTION ICI : Conversion du pourcentage (0-100) en gain OpenAL (0.0f - 1.0f)
	int clamped_percent = av_clip(percentVolume, 0, 100);
	is->audio_volume = static_cast<float>(clamped_percent) / 100.0f;

	is->muted = 0;
	is->av_sync_type = av_sync_type;

	// Démarrage du thread de lecture avec std::thread à la place de SDL_CreateThread
	try
	{
		is->read_tid = std::thread(read_thread, is);
	}
	catch (const std::system_error& e)
	{
		av_log(NULL, AV_LOG_FATAL, "std::thread execution failed: %s\n", e.what());
		stream_close(is);
		return NULL;
	}
	return is;
}


//ÒÔÏÂ¼¸¸öº¯Êý¶¼ÊÇ´¦Àíevent_loop()ÖÐµÄ¸÷ÖÖ²Ù×÷µÄ
void CFFmfcPimpl::stream_cycle_channel(VideoState* is, int codec_type)
{
	AVFormatContext* ic = is->ic;
	int start_index, stream_index;
	int old_index;
	AVStream* st;
	AVProgram* p = NULL;
	int nb_streams = is->ic->nb_streams;

	if (codec_type == AVMEDIA_TYPE_VIDEO)
	{
		start_index = is->last_video_stream;
		old_index = is->video_stream;
	}
	else if (codec_type == AVMEDIA_TYPE_AUDIO)
	{
		start_index = is->last_audio_stream;
		old_index = is->audio_stream;
	}
	else
	{
		start_index = is->last_subtitle_stream;
		old_index = is->subtitle_stream;
	}
	stream_index = start_index;

	if (codec_type != AVMEDIA_TYPE_VIDEO && is->video_stream != -1)
	{
		p = av_find_program_from_stream(ic, NULL, is->video_stream);
		if (p)
		{
			nb_streams = p->nb_stream_indexes;
			for (start_index = 0; start_index < nb_streams; start_index++)
				if (p->stream_index[start_index] == stream_index)
					break;
			if (start_index == nb_streams)
				start_index = -1;
			stream_index = start_index;
		}
	}

	for (;;)
	{
		if (++stream_index >= nb_streams)
		{
			if (codec_type == AVMEDIA_TYPE_SUBTITLE)
			{
				stream_index = -1;
				is->last_subtitle_stream = -1;
				goto the_end;
			}
			if (start_index == -1)
				return;
			stream_index = 0;
		}
		if (stream_index == start_index)
			return;
		st = is->ic->streams[p ? p->stream_index[stream_index] : stream_index];
		if (st->codecpar->codec_type == codec_type)
		{
			/* check that parameters are OK */
			switch (codec_type)
			{
			case AVMEDIA_TYPE_AUDIO:
				if (st->codecpar->sample_rate != 0 &&
					st->codecpar->ch_layout.nb_channels != 0)
					goto the_end;
				break;
			case AVMEDIA_TYPE_VIDEO:
			case AVMEDIA_TYPE_SUBTITLE:
				goto the_end;
			default:
				break;
			}
		}
	}
the_end:
	if (p && stream_index != -1)
		stream_index = p->stream_index[stream_index];
	/*
	av_log(NULL, AV_LOG_INFO, "Switch %s stream from #%d to #%d\n",
		av_get_media_type_string(codec_type),
		old_index,
		stream_index);

	*/
	stream_component_close(is, old_index);
	stream_component_open(is, stream_index);
}

void CFFmfcPimpl::toggle_pause(VideoState* is)
{
	stream_toggle_pause(is);
	is->step = 0;
}

void CFFmfcPimpl::toggle_play(VideoState* is)
{
	if (is->paused)
	{
		stream_toggle_pause(is);
		is->step = 0;
	}
}

void CFFmfcPimpl::step_to_next_frame(VideoState* is)
{
	/* if the stream is paused unpause it, then step */
	if (is->paused)
		stream_toggle_pause(is);
	is->step = 1;
}


int CFFmfcPimpl::refresh_thread(void* opaque)
{
	auto* is = static_cast<VideoState*>(opaque);
	if (!is || !is->_pimpl)
		return 0;

	double remaining_time = 0.0;

	while (!exit_video.load(std::memory_order_acquire))
	{
		if (remaining_time > 0.0)
			av_usleep(static_cast<unsigned>(remaining_time * 1000000.0));

		remaining_time = REFRESH_RATE;

		if (is->show_mode != SHOW_MODE_NONE &&
			(!is->paused || is->force_refresh))
		{
			is->_pimpl->video_refresh(is, &remaining_time);
		}

		if (CMasterWindow::endProgram)
			break;
	}

	return 0;
}

int CFFmfcPimpl::hwaccel_retrieve_data(AVCodecContext* avctx, AVFrame* input)
{
	VideoState* ist = (VideoState*)avctx->opaque;
	AVFrame* output = NULL;
	//enum AVPixelFormat output_format = ist->hwaccel_output_format;
	int err;

	output = av_frame_alloc();
	if (!output)
		return AVERROR(ENOMEM);

	//output->format = output_format;

	err = av_hwframe_transfer_data(output, input, 0);
	if (err < 0)
	{
		av_log(avctx, AV_LOG_ERROR, "Failed to transfer data to "
			"output frame: %d.\n", err);
		goto fail;
	}

	//output->pts = input->pkt_dts;

	av_frame_copy_props(output, input);
	output->pts = input->pts;
	ist->hwaccel_retrieved_pix_fmt = (AVPixelFormat)output->format;
	/*
	err = av_frame_copy_props(output, input);
	if (err < 0) {
		av_frame_unref(output);
		goto fail;
	}
	*/
	/*
	int ret = 0;
	int  size = av_image_get_buffer_size((AVPixelFormat)output->format, output->width,
		output->height, 1);
	uint8_t  * buffer =(uint8_t *) av_malloc(size);
	if (!buffer) {
		fprintf(stderr, "Can not alloc buffer\n");
		ret = AVERROR(ENOMEM);
		goto fail;
	}
	ret = av_image_copy_to_buffer(buffer, size,
		(const uint8_t* const*)output->data,
		(const int*)output->linesize, (AVPixelFormat)output->format,
		output->width, output->height, 1);

	if (ret < 0) {
		fprintf(stderr, "Can not copy image to buffer\n");
		goto fail;
	}



	delete[] buffer;
	*/

	av_frame_unref(input);
	av_frame_move_ref(input, output);
	av_frame_free(&output);





	return 0;

fail:
	av_frame_free(&output);
	return err;
}


int CFFmfcPimpl::get_buffer(AVCodecContext* s, AVFrame* frame, int flags)
{
	VideoState* ist = (VideoState*)s->opaque;

	if (ist->hwaccel_get_buffer && frame->format == hw_pix_fmt)
		return ist->hwaccel_get_buffer(s, frame, flags);

	return avcodec_default_get_buffer2(s, frame, flags);
}
