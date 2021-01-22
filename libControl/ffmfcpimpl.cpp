#include <header.h>
#include "ffmfcpimpl.h"
#include <WindowMain.h>
#include <mutex>
using namespace std;

bool exit_video = false;
std::mutex abortMutex;

//Calcul du pourcentage
using namespace Regards::Window;
CVideoControlInterface * CFFmfcPimpl::dlg = nullptr;
AVPixelFormat CFFmfcPimpl::hw_pix_fmt;

//-----------------------------------------------------------------------------------------
//Code
//-----------------------------------------------------------------------------------------

//#include "ColorSpaceConverter.h"
//#include <DShow.h>
//const char program_name[] = "ffplaymfc";
//const int program_birth_year = 2013;

#ifdef WIN32
AVPixelFormat CFFmfcPimpl::GetHwFormat(AVCodecContext *s, const AVPixelFormat *pix_fmts)
{
	InputStream* ist = (InputStream*)s->opaque;
	ist->active_hwaccel_id = HWACCEL_DXVA2;
	ist->hwaccel_pix_fmt = AV_PIX_FMT_DXVA2_VLD;
	return ist->hwaccel_pix_fmt;
}
#endif

inline int compute_mod(int a, int b)
{
	return a < 0 ? a % b + b : a % b;
}

AVCodecContext * GetCodec(AVStream * avStream)
{
	AVCodec * pCodec = avcodec_find_decoder(avStream->codecpar->codec_id);
	AVCodecContext * pCodecCtx = avcodec_alloc_context3(pCodec);
	avcodec_parameters_to_context(pCodecCtx, avStream->codecpar);
	return pCodecCtx;
}

int CFFmfcPimpl::percentageToDb(int p, int maxValue) {
	if (p == 0)
		return 0;
	float value = (float)p / 100.0f;
	return maxValue * (1 + log(value));
}

int CFFmfcPimpl::packet_queue_put_private(PacketQueue *q, AVPacket *pkt)
{
	AVPacketList *pkt1;

	bool abort = false;
	abortMutex.lock();
	abort = exit_video;
	abortMutex.unlock();
	if (abort)
		return -1;

	if (q->abort_request)
		return -1;
	//PacketList½«Packet×é³ÉÒ»¸öÁ´±í
	pkt1 = (AVPacketList*)av_malloc(sizeof(AVPacketList));
	if (!pkt1)
		return -1;
	pkt1->pkt = *pkt;
	pkt1->next = nullptr;

	if (!q->last_pkt)
		//µÚÒ»¸ö
		q->first_pkt = pkt1;
	else
		//ÆäËûÇé¿ö
		q->last_pkt->next = pkt1;
	q->last_pkt = pkt1;
	q->nb_packets++;
	q->size += pkt1->pkt.size + sizeof(*pkt1);
	/* XXX: should duplicate packet data in DV case */
	SDL_CondSignal(q->cond);
	return 0;
}
//Íù¶ÓÁÐÀïÌí¼ÓPacket
int CFFmfcPimpl::packet_queue_put(PacketQueue *q, AVPacket *pkt)
{
	int ret;

	/* duplicate the packet */
	if (pkt != &flush_pkt && av_dup_packet(pkt) < 0)
		return -1;

	SDL_LockMutex(q->mutex);
	//Íù¶ÓÁÐÀïÌí¼ÓPacket(Á½²ã)?
	ret = packet_queue_put_private(q, pkt);
	SDL_UnlockMutex(q->mutex);

	if (pkt != &flush_pkt && ret < 0)
		av_packet_unref(pkt);

	return ret;
}

/* packet queue handling */
void CFFmfcPimpl::packet_queue_init(PacketQueue *q)
{
	memset(q, 0, sizeof(PacketQueue));
	q->mutex = SDL_CreateMutex();
	q->cond = SDL_CreateCond();
	q->abort_request = 1;
}

void CFFmfcPimpl::packet_queue_flush(PacketQueue *q)
{
	AVPacketList *pkt, *pkt1;

	SDL_LockMutex(q->mutex);
	for (pkt = q->first_pkt; pkt != nullptr; pkt = pkt1) {
		pkt1 = pkt->next;
		av_packet_unref(&pkt->pkt);
		av_freep(&pkt);
	}
	q->last_pkt = nullptr;
	q->first_pkt = nullptr;
	q->nb_packets = 0;
	q->size = 0;
	SDL_UnlockMutex(q->mutex);
}

void CFFmfcPimpl::packet_queue_destroy(PacketQueue *q)
{
	packet_queue_flush(q);
	SDL_DestroyMutex(q->mutex);
	SDL_DestroyCond(q->cond);
}

void CFFmfcPimpl::packet_queue_abort(PacketQueue *q)
{
	SDL_LockMutex(q->mutex);

	q->abort_request = 1;

	SDL_CondSignal(q->cond);

	SDL_UnlockMutex(q->mutex);
}

void CFFmfcPimpl::packet_queue_start(PacketQueue *q)
{
	SDL_LockMutex(q->mutex);
	q->abort_request = 0;
	packet_queue_put_private(q, &flush_pkt);
	SDL_UnlockMutex(q->mutex);
}

/* return < 0 if aborted, 0 if no packet and > 0 if packet.  */
int CFFmfcPimpl::packet_queue_get(PacketQueue *q, AVPacket *pkt, int block)
{
	AVPacketList *pkt1;
	int ret;

	SDL_LockMutex(q->mutex);

	for (;;) {

		bool abort = false;
		abortMutex.lock();
		abort = exit_video;
		abortMutex.unlock();
		if (abort)
		{
			ret = -1;
			break;
		}

		if (q->abort_request) {
			ret = -1;
			break;
		}

		pkt1 = q->first_pkt;
		if (pkt1) {
			q->first_pkt = pkt1->next;
			if (!q->first_pkt)
				q->last_pkt = nullptr;
			q->nb_packets--;
			q->size -= pkt1->pkt.size + sizeof(*pkt1);
			*pkt = pkt1->pkt;
			av_free(pkt1);
			ret = 1;
			break;
		}
		else if (!block) {
			ret = 0;
			break;
		}
		else {
			SDL_CondWait(q->cond, q->mutex);
		}
	}
	SDL_UnlockMutex(q->mutex);
	return ret;
}


void CFFmfcPimpl::free_subpicture(SubPicture *sp)
{
	avsubtitle_free(&sp->sub);
}

void CFFmfcPimpl::video_audio_display(VideoState *s)
{
	int i, i_start, x, delay, n, nb_display_channels;//, y1, y, ys;
	int channels, h;//, h2, ch, bgcolor, fgcolor;
	int16_t time_diff;
	int rdft_bits, nb_freq;

	for (rdft_bits = 1; (1 << rdft_bits) < 2 * s->height; rdft_bits++)
		;
	nb_freq = 1 << (rdft_bits - 1);

	/* compute display index : center on currently output samples */
	channels = s->audio_tgt.channels;
	nb_display_channels = channels;
	if (!s->paused) {
		int data_used = s->show_mode == SHOW_MODE_WAVES ? s->width : (2 * nb_freq);
		n = 2 * channels;
		delay = s->audio_write_buf_size;
		delay /= n;

		/* to be more precise, we take into account the time spent since
		the last buffer computation */
		if (audio_callback_time) {
			time_diff = av_gettime() - audio_callback_time;
			delay -= (time_diff * s->audio_tgt.freq) / 1000000;
		}

		delay += 2 * data_used;
		if (delay < data_used)
			delay = data_used;

		i_start = x = compute_mod(s->sample_array_index - delay * channels, SAMPLE_ARRAY_SIZE);
		if (s->show_mode == SHOW_MODE_WAVES) {
			h = INT_MIN;
			for (i = 0; i < 1000; i += channels) {
				int idx = (SAMPLE_ARRAY_SIZE + x - i) % SAMPLE_ARRAY_SIZE;
				int a = s->sample_array[idx];
				int b = s->sample_array[(idx + 4 * channels) % SAMPLE_ARRAY_SIZE];
				int c = s->sample_array[(idx + 5 * channels) % SAMPLE_ARRAY_SIZE];
				int d = s->sample_array[(idx + 9 * channels) % SAMPLE_ARRAY_SIZE];
				int score = a - d;
				if (h < score && (b ^ c) < 0) {
					h = score;
					i_start = idx;
				}
			}
		}

		s->last_i_start = i_start;
	}
	else {
		i_start = s->last_i_start;
	}
}

void CFFmfcPimpl::StopStream()
{
	abortMutex.lock();
	exit_video = 1;
	abortMutex.unlock();
}

void CFFmfcPimpl::stream_close(VideoState *is)
{
	VideoPicture *vp;
	int i;
	/* XXX: use a special url_shutdown call to abort parse cleanly */
	is->abort_request = 1;


	if (is->read_tid != nullptr)
	{
		if (is->read_tid->joinable())
			is->read_tid->join();

		delete is->read_tid;
		is->read_tid = nullptr;
	}

	if (is->refresh_tid != nullptr)
	{
		if (is->refresh_tid->joinable())
			is->refresh_tid->join();

		delete is->refresh_tid;
		is->refresh_tid = nullptr;
	}

	/* close each stream */
	if (is->audio_stream >= 0)
		is->_pimpl->stream_component_close(is, is->audio_stream);
	if (is->video_stream >= 0)
		is->_pimpl->stream_component_close(is, is->video_stream);
	if (is->subtitle_stream >= 0)
		is->_pimpl->stream_component_close(is, is->subtitle_stream);
	if (is->ic) {
		avformat_close_input(&is->ic);
	}

	//SDL_WaitThread(is->read_tid, nullptr);
	//SDL_WaitThread(is->refresh_tid, nullptr);
	packet_queue_destroy(&is->videoq);
	packet_queue_destroy(&is->audioq);
	packet_queue_destroy(&is->subtitleq);

	/* free all pictures */
	for (i = 0; i < VIDEO_PICTURE_QUEUE_SIZE; i++)
	{
		vp = &is->pictq[i];
	}

	SDL_DestroyMutex(is->pictq_mutex);
	SDL_DestroyCond(is->pictq_cond);
	SDL_DestroyMutex(is->subpq_mutex);
	SDL_DestroyCond(is->subpq_cond);
	SDL_DestroyCond(is->continue_read_thread);
	av_free(is);

	
	wxCommandEvent evt(FF_QUIT_EVENT);
	parent->GetEventHandler()->AddPendingEvent(evt);
	
	abortMutex.lock();
	exit_video = 0;
	abortMutex.unlock();
}

//ÍË³ö
void CFFmfcPimpl::do_exit(VideoState *is)
{

	exit_remark = 1;
	if (is) {
		stream_close(is);
	}
	av_lockmgr_register(nullptr);
	uninit_opts();
	//avformat_network_deinit();
	if (show_status)
		printf("\n");

	av_log(nullptr, AV_LOG_QUIET, "%s", "");

}

/*
static void sigterm_handler(int sig)
{
	exit(123);
}
*/
/* display the current picture, if any */
void CFFmfcPimpl::video_display(VideoState *is)
{
#ifdef DISPLAY_VIDEO
	if (!dlg->screen)
		video_open(is, 0);
#endif
	if (!CMasterWindow::endProgram)
	{

		if (is->audio_st && is->show_mode != SHOW_MODE_VIDEO)
			video_audio_display(is);
		else if (is->video_st)
		{
			VideoPicture *vp;
			SubPicture *sp;
			AVPicture pict;
			SDL_Rect rect;
			int i;

			if (is->subtitle_st)
			{
				if (is->subpq_size > 0)
				{
					sp = &is->subpq[is->subpq_rindex];
					vp = &is->pictq[is->pictq_rindex];

					if (vp->pts >= sp->pts + ((float)sp->sub.start_display_time / 1000))
					{
						for (i = 0; i < sp->sub.num_rects; i++)
						{
							AVSubtitleRect *rect = sp->sub.rects[i];
							AVPicture picture = rect->pict;
							CRegardsBitmap * bitmap = new CRegardsBitmap(rect->w, rect->h);
							uint8_t * data = picture.data[0];
							uint8_t * colorIndex = picture.data[1];
							uint8_t * ptBitmap = bitmap->GetPtBitmap();
							for (int y = 0; y < rect->h; y++)
							{
								for (int x = 0; x < rect->w; x++)
								{
									int r, g, b, a;
									int j = *data++;
									RGBA_IN(r, g, b, a, (uint32_t*)picture.data[1] + j);
									CRgbaquad color(r, g, b, a);
									bitmap->SetColorValue(x, y, color);
								}
							}
							dlg->SetSubtitulePicture(bitmap);
						}
					}
				}

			}
			else
			{
				dlg->DeleteSubtitulePicture();
			}

			//dlg->Refresh();
		}

	}

	//video_image_display(is);
}

int CFFmfcPimpl::refresh_thread(void *opaque)
{
	VideoState *is = (VideoState *)opaque;
	while (1) 
	{
		bool abort = false;
		abortMutex.lock();
		abort = exit_video;
		abortMutex.unlock();
		if (abort)
			break;

		wxCommandEvent evt(FF_REFRESH_EVENT);
		evt.SetClientData(opaque);
		if (is->ic != nullptr)
		{
			if (!is->refresh && (!is->paused || is->force_refresh)) {
				is->refresh = 1;
				is->_pimpl->parent->GetEventHandler()->AddPendingEvent(evt);
			}
		}

		if (CMasterWindow::endProgram)
			return 0;
		//FIXME ideally we should wait the correct time but SDLs event passing is so slow it would be silly
		av_usleep(is->audio_st && is->show_mode != SHOW_MODE_VIDEO ? is->_pimpl->rdftspeed * 1000 : 5000);
	}
	return 0;
}

/* get the current audio clock value */
double CFFmfcPimpl::get_audio_clock(VideoState *is)
{
	if (is->paused) {
		return is->audio_current_pts;
	}
	else {
		return is->audio_current_pts_drift + av_gettime() / 1000000.0;
	}
}

/* get the current video clock value */
double CFFmfcPimpl::get_video_clock(VideoState *is)
{
	if (is->paused) {
		return is->video_current_pts;
	}
	else {
		return is->video_current_pts_drift + av_gettime() / 1000000.0;
	}
}

/* get the current external clock value */
double CFFmfcPimpl::get_external_clock(VideoState *is)
{
	//int64_t ti;
	//int64_t ti = av_gettime();
	return is->external_clock + ((av_gettime() - is->external_clock_time) * 1e-6);
}

/* get the current master clock value */
double CFFmfcPimpl::get_master_clock(VideoState *is)
{
	double val = 0;

	if (is->av_sync_type == AV_SYNC_VIDEO_MASTER) {
		if (is->video_st)
			val = get_video_clock(is);
		else
			val = get_audio_clock(is);
	}
	else if (is->av_sync_type == AV_SYNC_AUDIO_MASTER) {
		if (is->audio_st)
			val = get_audio_clock(is);
		else
			val = get_video_clock(is);
	}
	else {
		val = get_external_clock(is);
	}
	return val;
}

/* seek in the stream */
void CFFmfcPimpl::stream_seek(VideoState *is, int64_t pos, int64_t rel, int seek_by_bytes)
{
	if (!is->seek_req) {
		is->seek_pos = pos;
		is->seek_rel = rel;
		is->seek_flags &= ~AVSEEK_FLAG_BYTE;
		if (seek_by_bytes)
			is->seek_flags |= AVSEEK_FLAG_BYTE;
		is->seek_req = 1;
	}
}

/* pause or resume the video */
void CFFmfcPimpl::stream_toggle_pause(VideoState *is)
{
	//bool pause = GetPausedValue();
	if (is->paused) {
		is->frame_timer += av_gettime() / 1000000.0 + is->video_current_pts_drift - is->video_current_pts;
		if (is->read_pause_return != AVERROR(ENOSYS)) {
			is->video_current_pts = is->video_current_pts_drift + av_gettime() / 1000000.0;
		}
		is->video_current_pts_drift = is->video_current_pts - av_gettime() / 1000000.0;
	}
	//SetPausedValue(!pause);
	is->paused = !is->paused;

}

double CFFmfcPimpl::compute_target_delay(double delay, VideoState *is)
{
	double sync_threshold, diff;

	/* update delay to follow master synchronisation source */
	if (((is->av_sync_type == AV_SYNC_AUDIO_MASTER && is->audio_st) ||
		is->av_sync_type == AV_SYNC_EXTERNAL_CLOCK)) {
		/* if video is slave, we try to correct big delays by
		duplicating or deleting a frame */
		diff = get_video_clock(is) - get_master_clock(is);

		/* skip or repeat frame. We take into account the
		delay to compute the threshold. I still don't know
		if it is the best guess */
		sync_threshold = FFMAX(AV_SYNC_THRESHOLD, delay);
		if (fabs(diff) < AV_NOSYNC_THRESHOLD) {
			if (diff <= -sync_threshold)
				delay = 0;
			else if (diff >= sync_threshold)
				delay = 2 * delay;
		}
	}

	//av_dlog(nullptr, "video: delay=%0.3f A-V=%f\n",       delay, -diff);

	return delay;
}

void CFFmfcPimpl::pictq_next_picture(VideoState *is) {
	/* update queue size and signal for next picture */
	if (++is->pictq_rindex == VIDEO_PICTURE_QUEUE_SIZE)
		is->pictq_rindex = 0;

	SDL_LockMutex(is->pictq_mutex);
	is->pictq_size--;
	SDL_CondSignal(is->pictq_cond);
	SDL_UnlockMutex(is->pictq_mutex);
}

void CFFmfcPimpl::pictq_prev_picture(VideoState *is) {
	VideoPicture *prevvp;
	/* update queue size and signal for the previous picture */
	prevvp = &is->pictq[(is->pictq_rindex + VIDEO_PICTURE_QUEUE_SIZE - 1) % VIDEO_PICTURE_QUEUE_SIZE];
	if (prevvp->allocated && !prevvp->skip) {
		SDL_LockMutex(is->pictq_mutex);
		if (is->pictq_size < VIDEO_PICTURE_QUEUE_SIZE - 1) {
			if (--is->pictq_rindex == -1)
				is->pictq_rindex = VIDEO_PICTURE_QUEUE_SIZE - 1;
			is->pictq_size++;
		}
		SDL_CondSignal(is->pictq_cond);
		SDL_UnlockMutex(is->pictq_mutex);
	}
}

void CFFmfcPimpl::update_video_pts(VideoState *is, double pts, int64_t pos) {
	double time = av_gettime() / 1000000.0;
	/* update current video pts */
	is->video_current_pts = pts;
	is->video_current_pts_drift = is->video_current_pts - time;
	is->video_current_pos = pos;
	is->frame_last_pts = pts;
}

void CFFmfcPimpl::video_refresh(VideoState *is)
{
	VideoPicture *vp;
	double time;

	SubPicture *sp, *sp2;

	if (g_is != is)
		return;

	if (is->ic == nullptr)
		return;

	if (is->video_st) {
		if (is->force_refresh)
			pictq_prev_picture(is);
	retry:
		if (is->pictq_size == 0) {
			SDL_LockMutex(is->pictq_mutex);
			if (is->frame_last_dropped_pts != AV_NOPTS_VALUE && is->frame_last_dropped_pts > is->frame_last_pts) {
				update_video_pts(is, is->frame_last_dropped_pts, is->frame_last_dropped_pos);
				is->frame_last_dropped_pts = AV_NOPTS_VALUE;
			}
			SDL_UnlockMutex(is->pictq_mutex);
			// nothing to do, no picture to display in the que
		}
		else {
			double last_duration, duration, delay;
			// dequeue the picture
			vp = &is->pictq[is->pictq_rindex];

			if (vp->skip) {
				pictq_next_picture(is);
				goto retry;
			}

			if (is->paused)
				goto display;

			// compute nominal last_duration
			last_duration = vp->pts - is->frame_last_pts;
			if (last_duration > 0 && last_duration < 10.0) {
				// if duration of the last frame was sane, update last_duration in video state 
				is->frame_last_duration = last_duration;
			}
			delay = compute_target_delay(is->frame_last_duration, is);

			time = av_gettime() / 1000000.0;
			if (time < is->frame_timer + delay)
				return;

			if (delay > 0)
				is->frame_timer += delay * FFMAX(1, floor((time - is->frame_timer) / delay));

			SDL_LockMutex(is->pictq_mutex);
			update_video_pts(is, vp->pts, vp->pos);
			SDL_UnlockMutex(is->pictq_mutex);

			if (is->pictq_size > 1) {
				VideoPicture *nextvp = &is->pictq[(is->pictq_rindex + 1) % VIDEO_PICTURE_QUEUE_SIZE];
				duration = nextvp->pts - vp->pts;
				if ((framedrop > 0 || (framedrop && is->audio_st)) && time > is->frame_timer + duration) {
					is->frame_drops_late++;
					pictq_next_picture(is);
					goto retry;
				}
			}

			if (is->subtitle_st) {
				if (is->subtitle_stream_changed) {
					SDL_LockMutex(is->subpq_mutex);

					while (is->subpq_size) {
						free_subpicture(&is->subpq[is->subpq_rindex]);

						// update queue size and signal for next picture 
						if (++is->subpq_rindex == SUBPICTURE_QUEUE_SIZE)
							is->subpq_rindex = 0;

						is->subpq_size--;
					}
					is->subtitle_stream_changed = 0;

					SDL_CondSignal(is->subpq_cond);
					SDL_UnlockMutex(is->subpq_mutex);
				}
				else {
					if (is->subpq_size > 0) {
						sp = &is->subpq[is->subpq_rindex];

						if (is->subpq_size > 1)
							sp2 = &is->subpq[(is->subpq_rindex + 1) % SUBPICTURE_QUEUE_SIZE];
						else
							sp2 = nullptr;

						if ((is->video_current_pts > (sp->pts + ((float)sp->sub.end_display_time / 1000)))
							|| (sp2 && is->video_current_pts > (sp2->pts + ((float)sp2->sub.start_display_time / 1000))))
						{
							CFFmfcPimpl::free_subpicture(sp);

							// update queue size and signal for next picture
							if (++is->subpq_rindex == SUBPICTURE_QUEUE_SIZE)
								is->subpq_rindex = 0;

							SDL_LockMutex(is->subpq_mutex);
							is->subpq_size--;
							SDL_CondSignal(is->subpq_cond);
							SDL_UnlockMutex(is->subpq_mutex);
						}
					}
				}
			}

		display:
			//display picture 
			if (!display_disable)
				video_display(is);

			pictq_next_picture(is);
		}
	}
	else if (is->audio_st) {
		// draw the next audio frame 

		// if only audio stream, then display the audio bars (better
		//than nothing, just to test the implementation 

		//display picture 
		if (!display_disable)
			video_display(is);
	}
	is->force_refresh = 0;
	/*
	wxString currentclockstr;
	int tns, thh, tmm, tss;
	tns  = get_master_clock(is);
	thh  = tns / 3600;
	tmm  = (tns % 3600) / 60;
	tss  = (tns % 60);

	double avgFrame = av_q2d(g_is->ic->streams[g_is->video_stream]->avg_frame_rate);
	currentclockstr = wxString::Format(_T("%02d:%02d:%02d FPS : %0.3f"),thh,tmm,tss, avgFrame);

	dlg->SetCurrentclock(currentclockstr);*/

	if(!is->paused)
		dlg->SetPos(get_master_clock(is) * 1000);

	/*
	if (show_status)
	{
		static int64_t last_time;
		int64_t cur_time;
		int aqsize, vqsize, sqsize;
		double av_diff;

		cur_time = av_gettime();
		if (!last_time || (cur_time - last_time) >= 30000) {
			aqsize = 0;
			vqsize = 0;
			sqsize = 0;
			if (is->audio_st)
				aqsize = is->audioq.size;
			if (is->video_st)
				vqsize = is->videoq.size;
			if (is->subtitle_st)
				sqsize = is->subtitleq.size;
			av_diff = 0;
			if (is->audio_st && is->video_st)
				av_diff = get_audio_clock(is) - get_video_clock(is);
#ifndef WIN32
			printf("%7.2f A-V:%7.3f fd=%4d aq=%5dKB vq=%5dKB sq=%5dB f=%" PRId64"/%" PRId64"   \r",
				get_master_clock(is),
				av_diff,
				is->frame_drops_early + is->frame_drops_late,
				aqsize / 1024,
				vqsize / 1024,
				sqsize,
				is->video_st ? is->videoCtx->pts_correction_num_faulty_dts : 0,
				is->video_st ? is->videoCtx->pts_correction_num_faulty_pts : 0);
#endif
			//int pos=1000*get_master_clock(is)/(is->ic->duration/1000000);
			dlg->SetPos(get_master_clock(is) * 1000);

			wxString currentclockstr;
			int tns, thh, tmm, tss;
			tns  = get_master_clock(is);
			thh  = tns / 3600;
			tmm  = (tns % 3600) / 60;
			tss  = (tns % 60);
			currentclockstr.Format(_T("%02d:%02d:%02d"),thh,tmm,tss);
			dlg->SetCurrentclock(currentclockstr);

			fflush(stdout);
			last_time = cur_time;
		}

	}
	else
	{
		wxString currentclockstr;
		int tns, thh, tmm, tss;
		tns  = get_master_clock(is);
		thh  = tns / 3600;
		tmm  = (tns % 3600) / 60;
		tss  = (tns % 60);
		currentclockstr = wxString::Format(_T("%02d:%02d:%02d"),thh,tmm,tss);
		dlg->SetCurrentclock(currentclockstr);
		dlg->SetPos(get_master_clock(is) * 1000);
	}*/
}

/* allocate a picture (needs to do that in main thread to avoid
potential locking problems */
void CFFmfcPimpl::alloc_picture(VideoState *is)
{
	VideoPicture * vp = &is->pictq[is->pictq_windex];

	SDL_LockMutex(is->pictq_mutex);
	vp->allocated = 1;
	SDL_CondSignal(is->pictq_cond);
	SDL_UnlockMutex(is->pictq_mutex);
}
//½âÂë³É¹¦ºóÓÃÓÚÏÔÊ¾£¬Ò²ÊÇ·Åµ½ÁíÒ»¸ö¶ÓÁÐÖÐ£¿
int CFFmfcPimpl::queue_picture(VideoState *is, AVFrame *src_frame, double pts1, int64_t pos)
{
	VideoPicture *vp;
	double frame_delay, pts = pts1;

	/* compute the exact PTS for the picture if it is omitted in the stream
	* pts1 is the dts of the pkt / pts of the frame */
	if (pts != 0) {
		/* update video clock with pts, if present */
		is->video_clock = pts;
	}
	else {
		pts = is->video_clock;
	}
	/* update video clock for next frame */
	frame_delay = av_q2d(is->video_st->time_base);
	/* for MPEG2, the frame can be repeated, so we update the
	clock accordingly */
	frame_delay += src_frame->repeat_pict * (frame_delay * 0.5);
	is->video_clock += frame_delay;

#if defined(DEBUG_SYNC) && 0
	printf("frame_type=%c clock=%0.3f pts=%0.3f\n",
		av_get_picture_type_char(src_frame->pict_type), pts, pts1);
#endif

	/* wait until we have space to put a new picture */
	SDL_LockMutex(is->pictq_mutex);

	/* keep the last already displayed picture in the queue */
	while (is->pictq_size >= VIDEO_PICTURE_QUEUE_SIZE - 2 &&
		!is->videoq.abort_request) {

		bool abort = false;
		abortMutex.lock();
		abort = exit_video;
		abortMutex.unlock();

		if (abort)
			break;

		SDL_CondWait(is->pictq_cond, is->pictq_mutex);
	}
	SDL_UnlockMutex(is->pictq_mutex);

	if (is->videoq.abort_request)
		return -1;

	vp = &is->pictq[is->pictq_windex];

	vp->sample_aspect_ratio = av_guess_sample_aspect_ratio(is->ic, is->video_st, src_frame);


#ifdef YUV
	if (is->videoq.abort_request)
		return -1;

	if (TRUE)
	{
		float video_aspect_ratio = 0;
		if (vp->sample_aspect_ratio.num == 0)
			video_aspect_ratio = 0;
		else
			video_aspect_ratio = av_q2d(vp->sample_aspect_ratio);
#ifdef WIN32
		if (dlg->GetDXVA2Compatible())
			dlg->SetData(src_frame, video_aspect_ratio, dxva2);
		else
			dlg->SetData(src_frame, video_aspect_ratio, nullptr);
#else
		dlg->SetData(src_frame, video_aspect_ratio, nullptr);
#endif
		//av_frame_unref(src_frame);
#else


	/* if the frame is not skipped, then display it */
	if (vp->bmp)
	{
		AVPicture pict = { { 0 } };

		/* get a pointer on the bitmap */
		SDL_LockSurface(vp->bmp);
		//ÏÔÊ¾YUVÊý¾Ý
		//ÉèÖÃÊä³ö¸ñÊ½-----------------

		is->v_show_mode = SHOW_MODE_RGB24;

		switch (is->v_show_mode)
		{
		case SHOW_MODE_RGB24:
			pict.data[0] = (uint8_t *)vp->bmp->pixels;
			pict.linesize[0] = vp->bmp->pitch;
			break;
		}

		if (videoOutputMode == 24)
		{
			is->img_convert_ctx = sws_getCachedContext(is->img_convert_ctx,
				vp->width, vp->height, (AVPixelFormat)(src_frame->format), vp->width, vp->height,
				AV_PIX_FMT_RGB24, sws_flags, nullptr, nullptr, nullptr);
		}
		else
		{
			is->img_convert_ctx = sws_getCachedContext(is->img_convert_ctx,
				vp->width, vp->height, (AVPixelFormat)(src_frame->format), vp->width, vp->height,
				AV_PIX_FMT_RGBA, sws_flags, nullptr, nullptr, nullptr);
		}
		if (is->img_convert_ctx == nullptr) {
			fprintf(stderr, "Cannot initialize the conversion context\n");
			exit(1);
		}
		sws_scale(is->img_convert_ctx, src_frame->data, src_frame->linesize,
			0, vp->height, pict.data, pict.linesize);

#endif
		/* update the bitmap content */
		//SDL_UnlockSurface(vp->bmp);

		vp->pts = pts;
		vp->pos = pos;
		vp->skip = 0;

		/* now we can update the picture count */
		if (++is->pictq_windex == VIDEO_PICTURE_QUEUE_SIZE)
			is->pictq_windex = 0;
		SDL_LockMutex(is->pictq_mutex);
		is->pictq_size++;
		SDL_UnlockMutex(is->pictq_mutex);
	}
	return 0;
}

AVFrame * CFFmfcPimpl::CopyFrame(AVFrame * frame)
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
	return copyFrame;
}

void CFFmfcPimpl::CopyFrameToDest(AVFrame * frame)
{
	dst->format = frame->format;
	dst->width = frame->width;
	dst->height = frame->height;
	dst->channels = frame->channels;
	dst->channel_layout = frame->channel_layout;
	dst->nb_samples = frame->nb_samples;
	av_frame_get_buffer(dst, 32);
	av_frame_copy(dst, frame);
	av_frame_copy_props(dst, frame);
}

//½âÂëÒ»Ö¡ÊÓÆµ
int CFFmfcPimpl::get_video_frame(VideoState *is, AVFrame *frame, int64_t *pts, AVPacket *pkt, bool & frame_destination)
{
	int got_picture, i;
	//´ÓPacket¶ÓÁÐÖÐ»ñÈ¡Packet
	if (packet_queue_get(&is->videoq, pkt, 1) < 0)
		return -1;

	if (pkt->data == flush_pkt.data) {
		avcodec_flush_buffers(is->videoCtx);

		SDL_LockMutex(is->pictq_mutex);
		// Make sure there are no long delay timers (ideally we should just flush the que but thats harder)
		for (i = 0; i < VIDEO_PICTURE_QUEUE_SIZE; i++) {
			is->pictq[i].skip = 1;
		}
		while (is->pictq_size && !is->videoq.abort_request)
		{
			bool abort = false;
			abortMutex.lock();
			abort = exit_video;
			abortMutex.unlock();

			if (abort)
				break;

			SDL_CondWait(is->pictq_cond, is->pictq_mutex);
		}
		is->video_current_pos = -1;
		is->frame_last_pts = AV_NOPTS_VALUE;
		is->frame_last_duration = 0;
		is->frame_timer = (double)av_gettime() / 1000000.0;
		is->frame_last_dropped_pts = AV_NOPTS_VALUE;
		SDL_UnlockMutex(is->pictq_mutex);

		return 0;
	}

	frame_destination = false;
	got_picture = false;
	
	int ret = avcodec_receive_frame(is->videoCtx, frame);
	if (ret == 0)
		got_picture = true;
	if (ret == AVERROR(EAGAIN))
		ret = 0;

	if (acceleratorHardware != "" && (acceleratorHardware == "dxva2" && !isOpenGLDecoding))
	{
		if (frame->format == hw_pix_fmt)
		{
			/*
			AVFrame * sw_frame = av_frame_alloc();
			if (sw_frame == nullptr) {
				fprintf(stderr, "Can not alloc frame\n");
				ret = AVERROR(ENOMEM);
				av_frame_free(&sw_frame);
				return ret;
			}
			/* retrieve data from GPU to CPU */
			if ((ret = av_hwframe_transfer_data(dst, frame, 0)) < 0) {
				fprintf(stderr, "Error transferring the data to system memory\n");
				av_frame_free(&dst);
				return ret;
			}
			
			/*
			if (first)
			{

				scaleContext = sws_alloc_context();

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
				dst->width = frame->width;
				dst->height = frame->height;
				dst->channels = frame->channels;
				dst->channel_layout = frame->channel_layout;
				dst->nb_samples = frame->nb_samples;
				int res = av_image_alloc(dst->data, dst->linesize, sw_frame->width, sw_frame->height, AV_PIX_FMT_YUV420P, 1);
			}

			*/
			av_frame_copy_props(dst, frame);

			/*/
			sws_scale(scaleContext,
				(uint8_t const * const *)sw_frame->data, sw_frame->linesize, 0, (int)frame->height,
				dst->data, dst->linesize);
*/
			frame_destination = true;
			//av_frame_free(&sw_frame);
		}
	}

	if (ret == 0)
		ret = avcodec_send_packet(is->videoCtx, pkt);
	if (ret == AVERROR(EAGAIN))
		ret = 0;
	else if (ret < 0)
		return ret;
	//else
	//	len1 = pkt->size;
	

	if (got_picture) {
		//×¢Òâ£º´Ë´¦ÉèÖÃMFC²ÎÊý£¡
		//ffmfc_param_vframe(is,frame,pkt);
		//--------------------------
		int ret = 1;

		if (decoder_reorder_pts == -1) {
			*pts = av_frame_get_best_effort_timestamp(frame);
		}
		else if (decoder_reorder_pts) {
			*pts = frame->pkt_pts;
		}
		else {
			*pts = frame->pkt_dts;
		}

		if (*pts == AV_NOPTS_VALUE) {
			*pts = 0;
		}

		if (((is->av_sync_type == AV_SYNC_AUDIO_MASTER && is->audio_st) || is->av_sync_type == AV_SYNC_EXTERNAL_CLOCK) &&
			(framedrop > 0 || (framedrop && is->audio_st))) {
			SDL_LockMutex(is->pictq_mutex);
			if (is->frame_last_pts != AV_NOPTS_VALUE && *pts) {
				double clockdiff = get_video_clock(is) - get_master_clock(is);
				double dpts = av_q2d(is->video_st->time_base) * *pts;
				double ptsdiff = dpts - is->frame_last_pts;
				if (fabs(clockdiff) < AV_NOSYNC_THRESHOLD &&
					ptsdiff > 0 && ptsdiff < AV_NOSYNC_THRESHOLD &&
					clockdiff + ptsdiff - is->frame_last_filter_delay < 0) {
					is->frame_last_dropped_pos = pkt->pos;
					is->frame_last_dropped_pts = dpts;
					is->frame_drops_early++;
					ret = 0;
				}
			}
			SDL_UnlockMutex(is->pictq_mutex);
		}

		return ret;
	}
	return 0;
}

//½âÂëÊÓÆµ
int CFFmfcPimpl::video_thread(void *arg)
{
	AVPacket pkt = { 0 };
	VideoState *is = (VideoState *)arg;
	//AVFrame *frame = avcodec_alloc_frame();
	AVFrame *frame = av_frame_alloc();
	int64_t pts_int = AV_NOPTS_VALUE, pos = -1;
	double pts;
	int ret;
	//is->videoCtx = GetCodec(is->video_st);

	for (;;)
	{
		bool abort = false;
		abortMutex.lock();
		abort = exit_video;
		abortMutex.unlock();
		if (abort)
			goto the_end;

		while (is->paused)
		{
			abortMutex.lock();
			abort = exit_video;
			abortMutex.unlock();
			if (abort)
				goto the_end;

			SDL_Delay(10);

		}
		//avcodec_get_frame_defaults(frame);
		av_frame_unref(frame);
		av_packet_unref(&pkt);
		bool frame_destination = false;
		//½âÂëÒ»Ö¡ÊÓÆµ
		ret = is->_pimpl->get_video_frame(is, frame, &pts_int, &pkt, frame_destination);
		if (ret < 0)
			goto the_end;

		if (!ret)
			continue;

		pts = pts_int * av_q2d(is->video_st->time_base);
		//½âÂë³É¹¦ºóÓÃÓÚÏÔÊ¾£¬Ò²ÊÇ·Åµ½ÁíÒ»¸ö¶ÓÁÐÖÐ£¿

		if(frame_destination)
			ret = is->_pimpl->queue_picture(is, is->_pimpl->dst, pts, pkt.pos);
		else
			ret = is->_pimpl->queue_picture(is, frame, pts, pkt.pos);

		if (ret < 0)
			goto the_end;

		if (is->step)
			is->_pimpl->stream_toggle_pause(is);

		SDL_Delay(10);
	}
the_end:
	avcodec_flush_buffers(is->videoCtx);
	av_packet_unref(&pkt);

	//av_freep(&frame->data[0]);
	av_frame_free(&frame);

	return 0;
}

int CFFmfcPimpl::subtitle_thread(void *arg)
{
	VideoState *is = (VideoState *)arg;
	SubPicture *sp;
	AVPacket pkt1, *pkt = &pkt1;
	int got_subtitle;
	double pts;
	int i, j;
	//is->subtitleCtx = GetCodec(is->subtitle_st);
	//int r, g, b, y, u, v, a;

	for (;;) {

		bool abort = false;
		abortMutex.lock();
		abort = exit_video;
		abortMutex.unlock();
		if (abort)
			break;

		while (is->paused && !is->subtitleq.abort_request) {
			SDL_Delay(10);
		}
		if (is->_pimpl->packet_queue_get(&is->subtitleq, pkt, 1) < 0)
			break;

		if (pkt->data == is->_pimpl->flush_pkt.data) {
			avcodec_flush_buffers(is->subtitleCtx);
			continue;
		}
		SDL_LockMutex(is->subpq_mutex);
		while (is->subpq_size >= SUBPICTURE_QUEUE_SIZE &&
			!is->subtitleq.abort_request) {

			bool abort = false;
			abortMutex.lock();
			abort = exit_video;
			abortMutex.unlock();
			if (abort)
				break;

			SDL_CondWait(is->subpq_cond, is->subpq_mutex);
		}
		SDL_UnlockMutex(is->subpq_mutex);

		if (is->subtitleq.abort_request)
			return 0;

		sp = &is->subpq[is->subpq_windex];

		/* NOTE: ipts is the PTS of the _first_ picture beginning in
		this packet, if any */
		pts = 0;
		if (pkt->pts != AV_NOPTS_VALUE)
			pts = av_q2d(is->subtitle_st->time_base) * pkt->pts;

		avcodec_decode_subtitle2(is->subtitleCtx, &sp->sub,
			&got_subtitle, pkt);
		if (got_subtitle && sp->sub.format == 0) {
			if (sp->sub.pts != AV_NOPTS_VALUE)
				pts = sp->sub.pts / (double)AV_TIME_BASE;
			sp->pts = pts;

			/*
			for (i = 0; i < sp->sub.num_rects; i++)
			{
				for (j = 0; j < sp->sub.rects[i]->nb_colors; j++)
				{
					RGBA_IN(r, g, b, a, (uint32_t*)sp->sub.rects[i]->pict.data[1] + j);
					y = RGB_TO_Y_CCIR(r, g, b);
					u = RGB_TO_U_CCIR(r, g, b, 0);
					v = RGB_TO_V_CCIR(r, g, b, 0);
					YUVA_OUT((uint32_t*)sp->sub.rects[i]->pict.data[1] + j, y, u, v, a);
				}
			}
			*/
			/* now we can update the picture count */
			if (++is->subpq_windex == SUBPICTURE_QUEUE_SIZE)
				is->subpq_windex = 0;
			SDL_LockMutex(is->subpq_mutex);
			is->subpq_size++;
			SDL_UnlockMutex(is->subpq_mutex);
		}
		av_packet_unref(pkt);
	}
	return 0;
}
//²¥·ÅÒôÆµµÄÊ±ºòSDLµÄÏÔÊ¾
/* copy samples for viewing in editor window */
void CFFmfcPimpl::update_sample_display(VideoState *is, short *samples, int samples_size)
{
	int size, len;

	size = samples_size / sizeof(short);
	while (size > 0) {
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
int CFFmfcPimpl::synchronize_audio(VideoState *is, int nb_samples)
{
	int wanted_nb_samples = nb_samples;

	/* if not master, then we try to remove or add samples to correct the clock */
	if (((is->av_sync_type == AV_SYNC_VIDEO_MASTER && is->video_st) ||
		is->av_sync_type == AV_SYNC_EXTERNAL_CLOCK)) {
		double diff, avg_diff;
		int min_nb_samples, max_nb_samples;

		diff = get_audio_clock(is) - get_master_clock(is);

		if (diff < AV_NOSYNC_THRESHOLD) {
			is->audio_diff_cum = diff + is->audio_diff_avg_coef * is->audio_diff_cum;
			if (is->audio_diff_avg_count < AUDIO_DIFF_AVG_NB) {
				/* not enough measures to have a correct estimate */
				is->audio_diff_avg_count++;
			}
			else {
				/* estimate the A-V difference */
				avg_diff = is->audio_diff_cum * (1.0 - is->audio_diff_avg_coef);

				if (fabs(avg_diff) >= is->audio_diff_threshold) {
					wanted_nb_samples = nb_samples + (int)(diff * is->audio_src.freq);
					min_nb_samples = ((nb_samples * (100 - SAMPLE_CORRECTION_PERCENT_MAX) / 100));
					max_nb_samples = ((nb_samples * (100 + SAMPLE_CORRECTION_PERCENT_MAX) / 100));
					wanted_nb_samples = FFMIN(FFMAX(wanted_nb_samples, min_nb_samples), max_nb_samples);
				}
				/*
				av_dlog(nullptr, "diff=%f adiff=%f sample_diff=%d apts=%0.3f vpts=%0.3f %f\n",
					diff, avg_diff, wanted_nb_samples - nb_samples,
					is->audio_clock, is->video_clock, is->audio_diff_threshold);
					 * */
				printf("diff=%f adiff=%f sample_diff=%d apts=%0.3f vpts=%0.3f %f\n",
					diff, avg_diff, wanted_nb_samples - nb_samples,
					is->audio_clock, is->video_clock, is->audio_diff_threshold);
			}
		}
		else {
			/* too big difference : may be initial PTS errors, so
			reset A-V filter */
			is->audio_diff_avg_count = 0;
			is->audio_diff_cum = 0;
		}
	}

	return wanted_nb_samples;
}

/* decode one audio frame and returns its uncompressed size */
int CFFmfcPimpl::audio_decode_frame(VideoState *is, double *pts_ptr)
{
	AVCodecContext *audio_ctx = is->audioCtx;
	AVPacket *pkt_temp = &is->audio_pkt_temp;
	AVPacket *pkt = &is->audio_pkt;
	AVCodecContext *dec = is->audioCtx;
	int len2, data_size, resampled_data_size;
	int64_t dec_channel_layout;
	double pts;
	int new_packet = 0;
	int flush_complete = 0;
	int wanted_nb_samples;

	for (;;) {
		/* NOTE: the audio packet can contain several frames */
		while (new_packet)
		{
			int ret = 0;
			if (!is->frame) {
				if (!(is->frame = av_frame_alloc()))//avcodec_alloc_frame()))
					return AVERROR(ENOMEM);
			}
			else
            {
                 av_frame_unref(is->frame);
            }
				

			if (is->paused)
				return -1;

			if (flush_complete)
				break;
			new_packet = 0;

			ret = avcodec_send_packet(audio_ctx, pkt);
			if (ret < 0) {
				//Debug4("codec: sending audio packet failed");
				fprintf(stderr, "codec: sending audio packet failed\n");
				return ret;
			}
			ret = avcodec_receive_frame(audio_ctx, is->frame);
			if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
				//Debug4("codec: receiving audio frame failed");
				fprintf(stderr, "codec: receiving audio frame failed\n");
				return ret;
			}

			data_size = av_samples_get_buffer_size(nullptr, dec->channels,
				is->frame->nb_samples,
				dec->sample_fmt, 1);

			dec_channel_layout =
				(dec->channel_layout && dec->channels == av_get_channel_layout_nb_channels(dec->channel_layout)) ?
				dec->channel_layout : av_get_default_channel_layout(dec->channels);
			wanted_nb_samples = synchronize_audio(is, is->frame->nb_samples);

			if (dec->sample_fmt != is->audio_src.fmt ||
				dec_channel_layout != is->audio_src.channel_layout ||
				dec->sample_rate != is->audio_src.freq ||
				(wanted_nb_samples != is->frame->nb_samples && !is->swr_ctx)) {
				swr_free(&is->swr_ctx);
				is->swr_ctx = swr_alloc_set_opts(nullptr,
					is->audio_tgt.channel_layout, is->audio_tgt.fmt, is->audio_tgt.freq,
					dec_channel_layout, dec->sample_fmt, dec->sample_rate,
					0, nullptr);
				if (!is->swr_ctx || swr_init(is->swr_ctx) < 0) {
					fprintf(stderr, "Cannot create sample rate converter for conversion of %d Hz %s %d channels to %d Hz %s %d channels!\n",
						dec->sample_rate, av_get_sample_fmt_name(dec->sample_fmt), dec->channels,
						is->audio_tgt.freq, av_get_sample_fmt_name(is->audio_tgt.fmt), is->audio_tgt.channels);
					break;
				}
				is->audio_src.channel_layout = dec_channel_layout;
				is->audio_src.channels = dec->channels;
				is->audio_src.freq = dec->sample_rate;
				is->audio_src.fmt = dec->sample_fmt;
			}

			if (is->swr_ctx) {
				const uint8_t **in = (const uint8_t **)is->frame->extended_data;
				uint8_t *out[] = { is->audio_buf2 };
				int out_count = sizeof(is->audio_buf2) / is->audio_tgt.channels / av_get_bytes_per_sample(is->audio_tgt.fmt);
				if (wanted_nb_samples != is->frame->nb_samples) {
					if (swr_set_compensation(is->swr_ctx, (wanted_nb_samples - is->frame->nb_samples) * is->audio_tgt.freq / dec->sample_rate,
						wanted_nb_samples * is->audio_tgt.freq / dec->sample_rate) < 0) {
						fprintf(stderr, "swr_set_compensation() failed\n");
						break;
					}
				}
				len2 = swr_convert(is->swr_ctx, out, out_count, in, is->frame->nb_samples);
				if (len2 < 0) {
					fprintf(stderr, "swr_convert() failed\n");
					break;
				}
				if (len2 == out_count) {
					fprintf(stderr, "warning: audio buffer is probably too small\n");
					swr_init(is->swr_ctx);
				}
				is->audio_buf = is->audio_buf2;
				resampled_data_size = len2 * is->audio_tgt.channels * av_get_bytes_per_sample(is->audio_tgt.fmt);
			}
			else {
				is->audio_buf = is->frame->data[0];
				resampled_data_size = data_size;
			}

			/* if no pts, then compute it */
			pts = is->audio_clock;
			*pts_ptr = pts;
			is->audio_clock += (double)data_size /
				(dec->channels * dec->sample_rate * av_get_bytes_per_sample(dec->sample_fmt));
#ifdef DEBUG
			{
				static double last_clock;
				printf("audio: delay=%0.3f clock=%0.3f pts=%0.3f\n",
					is->audio_clock - last_clock,
					is->audio_clock, pts);
				last_clock = is->audio_clock;
			}
#endif
			return resampled_data_size;
		}

		/* free the current packet */
		if (pkt->data)
			av_packet_unref(pkt);

		if (is->paused || is->audioq.abort_request) {
			return -1;
		}

		if (is->audioq.nb_packets == 0)
			SDL_CondSignal(is->continue_read_thread);

		/* read next packet */
		if ((new_packet = CFFmfcPimpl::packet_queue_get(&is->audioq, pkt, 1)) < 0)
			return -1;

		if (pkt->data == flush_pkt.data) {
			avcodec_flush_buffers(dec);
			flush_complete = 0;
		}

		/* if update the audio clock with the pts */
		if (pkt->pts != AV_NOPTS_VALUE) {
			is->audio_clock = av_q2d(is->audio_st->time_base)*pkt->pts;
		}
	}

}

/* prepare a new audio buffer */
void CFFmfcPimpl::sdl_audio_callback(void *opaque, Uint8 *stream, int len)
{
	VideoState *is = (VideoState *)opaque;
	int audio_size, len1;
	int bytes_per_sec;
	int frame_size = av_samples_get_buffer_size(nullptr, is->audio_tgt.channels, 1, is->audio_tgt.fmt, 1);
	double pts = 0;

	is->_pimpl->audio_callback_time = av_gettime();

	while (len > 0) {
		if (is->audio_buf_index >= is->audio_buf_size) {
			audio_size = is->_pimpl->audio_decode_frame(is, &pts);
			if (audio_size < 0) {
				/* if error, just output silence */
				is->audio_buf = is->silence_buf;
				is->audio_buf_size = sizeof(is->silence_buf) / frame_size * frame_size;
			}
			else {
				if (is->show_mode != SHOW_MODE_VIDEO)
					is->_pimpl->update_sample_display(is, (int16_t *)is->audio_buf, audio_size);
				is->audio_buf_size = audio_size;
			}
			is->audio_buf_index = 0;
		}
		len1 = is->audio_buf_size - is->audio_buf_index;
		if (len1 > len)
			len1 = len;
            
         //   printf("Volume index : %d \n",is->_pimpl->percentVolume);
		//memcpy(stream, (uint8_t *)is->audio_buf + is->audio_buf_index, len1);
		SDL_memset(stream, 0, len1);
		float volume = (((float)is->_pimpl->percentVolume / 100.0f) * SDL_MIX_MAXVOLUME);
		SDL_MixAudio(stream, (uint8_t *)is->audio_buf + is->audio_buf_index, len1, volume);

		len -= len1;
		stream += len1;
		is->audio_buf_index += len1;
	}
	bytes_per_sec = is->audio_tgt.freq * is->audio_tgt.channels * av_get_bytes_per_sample(is->audio_tgt.fmt);
	is->audio_write_buf_size = is->audio_buf_size - is->audio_buf_index;
	/* Let's assume the audio driver that is used by SDL has two periods. */
	is->audio_current_pts = is->audio_clock - (double)(2 * is->audio_hw_buf_size + is->audio_write_buf_size) / bytes_per_sec;
	is->audio_current_pts_drift = is->audio_current_pts - is->_pimpl->audio_callback_time / 1000000.0;
}

int CFFmfcPimpl::audio_open(void *opaque, int64_t wanted_channel_layout, int wanted_nb_channels, int wanted_sample_rate, AudioParams *audio_hw_params)
{
	// Make sure sound is shut down first.
	SDL_PauseAudio(1);

	// Shut down SDL audio.
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	// Attempt to initialize SDL audio.
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
		return -1;


	/*
#ifdef _MSC_VER > 1900
	SDL_AudioSpec wanted_spec, spec;
	const char *env;
	const int next_nb_channels[] = { 0, 0, 1, 6, 2, 6, 4, 6 };

	env = SDL_getenv("SDL_AUDIO_CHANNELS");
	if (env) {
		wanted_nb_channels = atoi(env);
		wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
	}
	if (!wanted_channel_layout || wanted_nb_channels != av_get_channel_layout_nb_channels(wanted_channel_layout)) {
		wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
		wanted_channel_layout &= ~AV_CH_LAYOUT_STEREO_DOWNMIX;
	}
	wanted_spec.channels = av_get_channel_layout_nb_channels(wanted_channel_layout);
	wanted_spec.freq = wanted_sample_rate;
	if (wanted_spec.freq <= 0 || wanted_spec.channels <= 0) {
		fprintf(stderr, "Invalid sample rate or channel count!\n");
		return -1;
	}
	wanted_spec.format = AUDIO_F32SYS;
	wanted_spec.silence = 0;
	wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;
	wanted_spec.callback = sdl_audio_callback;
	wanted_spec.userdata = opaque;
	while (SDL_OpenAudio(&wanted_spec, &spec) < 0) {
		fprintf(stderr, "SDL_OpenAudio (%d channels): %s\n", wanted_spec.channels, SDL_GetError());
		wanted_spec.channels = next_nb_channels[FFMIN(7, wanted_spec.channels)];
		if (!wanted_spec.channels) {
			fprintf(stderr, "No more channel combinations to try, audio open failed\n");
			return -1;
		}
		wanted_channel_layout = av_get_default_channel_layout(wanted_spec.channels);
	}
	if (spec.format != AUDIO_F32SYS) {
		fprintf(stderr, "SDL advised audio format %d is not supported!\n", spec.format);
		return -1;
	}
	if (spec.channels != wanted_spec.channels) {
		wanted_channel_layout = av_get_default_channel_layout(spec.channels);
		if (!wanted_channel_layout) {
			fprintf(stderr, "SDL advised channel count %d is not supported!\n", spec.channels);
			return -1;
		}
	}

	audio_hw_params->fmt = AV_SAMPLE_FMT_FLT;
	audio_hw_params->freq = spec.freq;
	audio_hw_params->channel_layout = wanted_channel_layout;
	audio_hw_params->channels = spec.channels;
	return spec.size;
#else

	SDL_AudioSpec wanted_spec, spec;
	const char *env;
	const int next_nb_channels[] = { 0, 0, 1, 6, 2, 6, 4, 6 };

	env = SDL_getenv("SDL_AUDIO_CHANNELS");
	if (env) {
		wanted_nb_channels = atoi(env);
		wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
	}
	if (!wanted_channel_layout || wanted_nb_channels != av_get_channel_layout_nb_channels(wanted_channel_layout)) {
		wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
		wanted_channel_layout &= ~AV_CH_LAYOUT_STEREO_DOWNMIX;
	}
	wanted_spec.channels = av_get_channel_layout_nb_channels(wanted_channel_layout);
	wanted_spec.freq = wanted_sample_rate;
	if (wanted_spec.freq <= 0 || wanted_spec.channels <= 0) {
		fprintf(stderr, "Invalid sample rate or channel count!\n");
		return -1;
	}
	wanted_spec.format = AUDIO_S16SYS;
	wanted_spec.silence = 0;
	wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;
	wanted_spec.callback = sdl_audio_callback;
	wanted_spec.userdata = opaque;
	while (SDL_OpenAudio(&wanted_spec, &spec) < 0) {
		fprintf(stderr, "SDL_OpenAudio (%d channels): %s\n", wanted_spec.channels, SDL_GetError());
		wanted_spec.channels = next_nb_channels[FFMIN(7, wanted_spec.channels)];
		if (!wanted_spec.channels) {
			fprintf(stderr, "No more channel combinations to try, audio open failed\n");
			return -1;
		}
		wanted_channel_layout = av_get_default_channel_layout(wanted_spec.channels);
	}
	if (spec.format != AUDIO_S16SYS) {
		fprintf(stderr, "SDL advised audio format %d is not supported!\n", spec.format);
		return -1;
	}
	if (spec.channels != wanted_spec.channels) {
		wanted_channel_layout = av_get_default_channel_layout(spec.channels);
		if (!wanted_channel_layout) {
			fprintf(stderr, "SDL advised channel count %d is not supported!\n", spec.channels);
			return -1;
		}
	}

	audio_hw_params->fmt = AV_SAMPLE_FMT_S16;
	audio_hw_params->freq = spec.freq;
	audio_hw_params->channel_layout = wanted_channel_layout;
	audio_hw_params->channels = spec.channels;
	return spec.size;
#endif
*/
	SDL_AudioSpec wanted_spec, spec;
	const char *env;
	const int next_nb_channels[] = { 0, 0, 1, 6, 2, 6, 4, 6 };

	env = SDL_getenv("SDL_AUDIO_CHANNELS");
	if (env) {
		wanted_nb_channels = atoi(env);
		wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
	}
	if (!wanted_channel_layout || wanted_nb_channels != av_get_channel_layout_nb_channels(wanted_channel_layout)) {
		wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
		wanted_channel_layout &= ~AV_CH_LAYOUT_STEREO_DOWNMIX;
	}
	wanted_spec.channels = av_get_channel_layout_nb_channels(wanted_channel_layout);
	wanted_spec.freq = wanted_sample_rate;
	if (wanted_spec.freq <= 0 || wanted_spec.channels <= 0) {
		fprintf(stderr, "Invalid sample rate or channel count!\n");
		return -1;
	}
	wanted_spec.format = AUDIO_S16SYS;
	wanted_spec.silence = 0;
	wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;
	wanted_spec.callback = sdl_audio_callback;
	wanted_spec.userdata = opaque;
	while (SDL_OpenAudio(&wanted_spec, &spec) < 0) {
		fprintf(stderr, "SDL_OpenAudio (%d channels): %s\n", wanted_spec.channels, SDL_GetError());
		wanted_spec.channels = next_nb_channels[FFMIN(7, wanted_spec.channels)];
		if (!wanted_spec.channels) {
			fprintf(stderr, "No more channel combinations to try, audio open failed\n");
			return -1;
		}
		wanted_channel_layout = av_get_default_channel_layout(wanted_spec.channels);
	}
	if (spec.format != AUDIO_S16SYS) {
		fprintf(stderr, "SDL advised audio format %d is not supported!\n", spec.format);
		return -1;
	}
	if (spec.channels != wanted_spec.channels) {
		wanted_channel_layout = av_get_default_channel_layout(spec.channels);
		if (!wanted_channel_layout) {
			fprintf(stderr, "SDL advised channel count %d is not supported!\n", spec.channels);
			return -1;
		}
	}

	audio_hw_params->fmt = AV_SAMPLE_FMT_S16;
	audio_hw_params->freq = spec.freq;
	audio_hw_params->channel_layout = wanted_channel_layout;
	audio_hw_params->channels = spec.channels;
	return spec.size;
}

#ifndef CMDUTILS

int CFFmfcPimpl::check_stream_specifier(AVFormatContext *s, AVStream *st, const char *spec)
{
	int ret = avformat_match_stream_specifier(s, st, spec);
	if (ret < 0)
		av_log(s, AV_LOG_ERROR, "Invalid stream specifier: %s.\n", spec);
	return ret;
}

AVDictionary * CFFmfcPimpl::filter_codec_opts(AVDictionary *opts, enum AVCodecID codec_id,
	AVFormatContext *s, AVStream *st, AVCodec *codec)
{
	AVDictionary    *ret = nullptr;
	AVDictionaryEntry *t = nullptr;
	int            flags = s->oformat ? AV_OPT_FLAG_ENCODING_PARAM
		: AV_OPT_FLAG_DECODING_PARAM;
	char          prefix = 0;
	const AVClass    *cc = avcodec_get_class();

	if (!codec)
		codec = s->oformat ? avcodec_find_encoder(codec_id)
		: avcodec_find_decoder(codec_id);
	if (!codec)
		return nullptr;

	switch (codec->type) {
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

	while (t = av_dict_get(opts, "", t, AV_DICT_IGNORE_SUFFIX)) {
		char *p = strchr(t->key, ':');

		/* check stream specification in opt name */
		if (p)
			switch (check_stream_specifier(s, st, p + 1)) {
			case  1: *p = 0; break;
			case  0:         continue;
			default:         return nullptr;
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

AVDictionary ** CFFmfcPimpl::setup_find_stream_info_opts(AVFormatContext *s,
	AVDictionary *codec_opts)
{
	int i;
	AVDictionary **opts;

	if (!s->nb_streams)
		return nullptr;
	opts = (AVDictionary **)av_mallocz(s->nb_streams * sizeof(*opts));
	if (!opts) {
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

const char * CFFmfcPimpl::getExt(const char *fspec) {
	const char *e = strrchr(fspec, '.');
	if (e == NULL)
		e = ""; // fast method, could also use &(fspec[strlen(fspec)]).
	return e;
}


enum AVPixelFormat CFFmfcPimpl::get_hw_format(AVCodecContext *ctx,
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

int CFFmfcPimpl::hw_decoder_init(AVCodecContext *ctx, const enum AVHWDeviceType type)
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

/* open a given stream. Return 0 if OK */
//´ò¿ªÒ»¸öStream£¬ÊÓÆµ»òÒôÆµ
int CFFmfcPimpl::stream_component_open(VideoState *is, int stream_index)
{
	AVFormatContext *ic = is->ic;
	AVCodecContext *avctx;
	AVCodec *codec;
	AVDictionary *opts;
	AVDictionaryEntry *t = nullptr;
	bool dvxa2 = false;

	if (stream_index < 0 || stream_index >= ic->nb_streams)
		return -1;
	avctx = GetCodec(ic->streams[stream_index]);
	// ÎªÊÓÆµÁ÷Ñ°ÕÒ½âÂëÆ÷
	//×¢Òâ£º´Ë´¦¿ÉÒÔÖ¸¶¨½âÂëÆ÷
	codec = avcodec_find_decoder(avctx->codec_id);
	//#ifdef CMDUTILS
	opts = filter_codec_opts(codec_opts, avctx->codec_id, ic, ic->streams[stream_index], codec);
	//#endif



	switch (avctx->codec_type) {
	case AVMEDIA_TYPE_AUDIO:
		is->last_audio_stream = stream_index;
		if (audio_codec_name)
			codec = avcodec_find_decoder_by_name(audio_codec_name);
		break;
	case AVMEDIA_TYPE_SUBTITLE: is->last_subtitle_stream = stream_index; if (subtitle_codec_name) codec = avcodec_find_decoder_by_name(subtitle_codec_name); break;
	case AVMEDIA_TYPE_VIDEO:
		is->last_video_stream = stream_index;
		if (video_codec_name)
			codec = avcodec_find_decoder_by_name(video_codec_name);

#ifdef __APPLE__

		//if(avctx->codec_id == AV_CODEC_ID_H264)
		//    codec= avcodec_find_decoder_by_name("h264_vda");

#endif

#ifdef WIN32
		if (acceleratorHardware == "dxva2" && isOpenGLDecoding)
		{
			if (dlg->GetDXVA2HardwareCompatible())
			{
				avctx->thread_count = 1;  // Multithreading is apparently not compatible with hardware decoding
				InputStream *ist = new InputStream();
				ist->hwaccel_id = HWACCEL_AUTO;
				ist->hwaccel_device = "dxva2";
				ist->dec = codec;
				ist->dec_ctx = avctx;
				avctx->coded_height = ic->streams[stream_index]->codecpar->height;
				avctx->coded_width = ic->streams[stream_index]->codecpar->width;


				//printf("ÊÓÆµ¸ß£º%d\n",ic->streams[video_index]->codec->height);

				avctx->opaque = ist;
				if (dxva2_init(avctx, dlg, avctx->coded_width, avctx->coded_height) == 0)
				{
					InputStream *ist = (InputStream *)avctx->opaque;
					dxva2 = (DXVA2Context *)ist->hwaccel_ctx;
					//dlg->SetOpenCLDevice((void *)ist);
					dlg->SetDXVA2Compatible(true);
					avctx->get_buffer2 = ist->hwaccel_get_buffer;
					avctx->get_format = GetHwFormat;
					avctx->thread_safe_callbacks = 1;

					avctx->workaround_bugs = workaround_bugs;
					avctx->lowres = lowres;
					if (avctx->lowres > codec->max_lowres) {
						av_log(avctx, AV_LOG_WARNING, "The maximum value for lowres supported by the decoder is %d\n",
							codec->max_lowres);
						avctx->lowres = codec->max_lowres;
					}
					avctx->idct_algo = idct;
					avctx->skip_frame = skip_frame;
					avctx->skip_idct = skip_idct;
					avctx->skip_loop_filter = skip_loop_filter;
					avctx->error_concealment = error_concealment;

					/*
					if(avctx->lowres) avctx->flags |= CODEC_FLAG_EMU_EDGE;
					if (fast)   avctx->flags2 |= CODEC_FLAG2_FAST;
					if(codec->capabilities & CODEC_CAP_DR1)
						avctx->flags |= CODEC_FLAG_EMU_EDGE;
					*/
					//MetaData?
					if (!av_dict_get(opts, "threads", nullptr, 0))
						av_dict_set(&opts, "threads", "auto", 0);
					// ´ò¿ª½âÂëÆ÷£¬¶þÕßÖ®¼ä½¨Á¢ÁªÏµ

					if (avcodec_open2(avctx, codec, nullptr) < 0)
					{
						std::cout << "Video codec open error" << std::endl;
						return false;
					}
					dvxa2 = true;
				}
				else
				{
					dlg->SetDXVA2Compatible(false);
				}
			}
		}
		else if (acceleratorHardware != "")
#else
        if (acceleratorHardware != "")
#endif
		{
			bool success = true;
			enum AVHWDeviceType type;

			type = av_hwdevice_find_type_by_name(acceleratorHardware);
			if (type == AV_HWDEVICE_TYPE_NONE) {
				fprintf(stderr, "Device type %s is not supported.\n", acceleratorHardware.ToStdString().c_str());
				fprintf(stderr, "Available device types:");
				while ((type = av_hwdevice_iterate_types(type)) != AV_HWDEVICE_TYPE_NONE)
					fprintf(stderr, " %s", av_hwdevice_get_type_name(type));
				fprintf(stderr, "\n");
				success = false;
			}

			if (success)
			{
				for (int i = 0;; i++) {
					const AVCodecHWConfig *config = avcodec_get_hw_config(codec, i);
					if (!config) 
					{
						fprintf(stderr, "Decoder %s does not support device type %s.\n",
							codec->name, av_hwdevice_get_type_name(type));
						success = false;
						break;
					}
					if (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX &&
						config->device_type == type) {
						hw_pix_fmt = config->pix_fmt;
						break;
					}
				}
			}

			if (success)
			{
				avctx->get_format = get_hw_format;
				if (hw_decoder_init(avctx, type) < 0)
					success = false;
			}


		}
		break;
	}
	if (!codec)
		return -1;

	if (!dvxa2)
	{

		avctx->workaround_bugs = workaround_bugs;
		avctx->lowres = lowres;
		if (avctx->lowres > codec->max_lowres) {
			av_log(avctx, AV_LOG_WARNING, "The maximum value for lowres supported by the decoder is %d\n",
				codec->max_lowres);
			avctx->lowres = codec->max_lowres;
		}
		avctx->idct_algo = idct;
		avctx->skip_frame = skip_frame;
		avctx->skip_idct = skip_idct;
		avctx->skip_loop_filter = skip_loop_filter;
		avctx->error_concealment = error_concealment;

		//if(avctx->lowres) 
		//    avctx->flags |= CODEC_FLAG_EMU_EDGE;
		//if (fast)   
		//    avctx->flags2 |= AV_CODEC_FLAG2_FAST;
		//if(codec->capabilities & AV_CODEC_CAP_DR1)
		//	avctx->flags |= CODEC_FLAG_EMU_EDGE;
		//MetaData?
#ifdef CMDUTILS
		if (!av_dict_get(opts, "threads", nullptr, 0))
			av_dict_set(&opts, "threads", "auto", 0);
#else
		av_dict_set(&opts, "threads", "auto", 0);
#endif
		// ´ò¿ª½âÂëÆ÷£¬¶þÕßÖ®¼ä½¨Á¢ÁªÏµ
		if (!codec ||
			avcodec_open2(avctx, codec, &opts) < 0)
			return -1;
		if ((t = av_dict_get(opts, "", nullptr, AV_DICT_IGNORE_SUFFIX))) {
			av_log(nullptr, AV_LOG_ERROR, "Option %s not found.\n", t->key);
			return AVERROR_OPTION_NOT_FOUND;
		}
	}

	/* prepare audio output */
	//×¼±¸SDLÒôÆµÊä³ö
	if (avctx->codec_type == AVMEDIA_TYPE_AUDIO) {
		int audio_hw_buf_size = audio_open(is, avctx->channel_layout, avctx->channels, avctx->sample_rate, &is->audio_src);
		if (audio_hw_buf_size < 0)
			return -1;
		is->audio_hw_buf_size = audio_hw_buf_size;
		is->audio_tgt = is->audio_src;
		
	}

	ic->streams[stream_index]->discard = AVDISCARD_DEFAULT;

	switch (avctx->codec_type)
	{
		//ÉèÖÃis½á¹¹Ìå
	case AVMEDIA_TYPE_AUDIO:
		is->audioCtx = avctx;
		is->audio_stream = stream_index;
		is->audio_st = ic->streams[stream_index];
		is->audio_buf_size = 0;
		is->audio_buf_index = 0;

		/* init averaging filter */
		is->audio_diff_avg_coef = exp(log(0.01) / AUDIO_DIFF_AVG_NB);
		is->audio_diff_avg_count = 0;
		/* since we do not have a precise anough audio fifo fullness,
		we correct audio sync only if larger than this threshold */
		is->audio_diff_threshold = 2.0 * is->audio_hw_buf_size / av_samples_get_buffer_size(nullptr, is->audio_tgt.channels, is->audio_tgt.freq, is->audio_tgt.fmt, 1);

		memset(&is->audio_pkt, 0, sizeof(is->audio_pkt));
		memset(&is->audio_pkt_temp, 0, sizeof(is->audio_pkt_temp));
		//³õÊ¼»¯Packet¶ÓÁÐ
		CFFmfcPimpl::packet_queue_start(&is->audioq);
		//¿ª²¥
		SDL_PauseAudio(0);
		break;


	case AVMEDIA_TYPE_SUBTITLE:
		is->subtitleCtx = avctx;
		is->subtitle_stream = stream_index;
		is->subtitle_st = ic->streams[stream_index];
		CFFmfcPimpl::packet_queue_start(&is->subtitleq);
		/*
#ifdef SDL2
		is->subtitle_tid = SDL_CreateThread(subtitle_thread, "Subtitle Thread", is);
#else
		is->subtitle_tid = SDL_CreateThread(subtitle_thread, is);
#endif
*/
		is->subtitle_tid = new std::thread(subtitle_thread, is);

		break;



	case AVMEDIA_TYPE_VIDEO:
	{
		is->videoCtx = avctx;
		is->video_stream = stream_index;
		is->video_st = ic->streams[stream_index];

		CFFmfcPimpl::packet_queue_start(&is->videoq);
		//ÊÓÆµÏß³Ì
		/*
#ifdef SDL2
		is->video_tid = SDL_CreateThread(video_thread, "Video Thread", is);
#else
		is->video_tid = SDL_CreateThread(video_thread, is);
#endif
		*/
		is->video_tid = new std::thread(video_thread, is);

		int32_t* matrix = reinterpret_cast<int32_t*>(av_stream_get_side_data(is->video_st, AV_PKT_DATA_DISPLAYMATRIX, nullptr));
		if (matrix)
		{
			long rotation = lround(av_display_rotation_get(matrix));
			dlg->SetRotation(rotation);
		}
	}
	break;


	default:
		break;

	}
	return 0;
}

void CFFmfcPimpl::stream_component_close(VideoState *is, int stream_index)
{
	AVFormatContext *ic = is->ic;
	//AVCodecContext *avctx;


	if (stream_index < 0 || stream_index >= ic->nb_streams)
		return;
	//avctx = ic->streams[stream_index]->codec;



	switch (ic->streams[stream_index]->codecpar->codec_type)
	{
	case AVMEDIA_TYPE_AUDIO:
		packet_queue_abort(&is->audioq);

		SDL_CloseAudio();

		packet_queue_flush(&is->audioq);
		av_packet_unref(&is->audio_pkt);
		swr_free(&is->swr_ctx);
		av_freep(&is->audio_buf1);
		is->audio_buf = nullptr;
		av_frame_free(&is->frame);
		//avcodec_free_frame(&is->frame);

		if (is->rdft) {
			av_rdft_end(is->rdft);
			av_freep(&is->rdft_data);
			is->rdft = nullptr;
			is->rdft_bits = 0;
		}
		avcodec_close(is->audioCtx);
		break;
	case AVMEDIA_TYPE_VIDEO:
		packet_queue_abort(&is->videoq);

		/* note: we also signal this mutex to make sure we deblock the
		video thread in all cases */
		SDL_LockMutex(is->pictq_mutex);
		SDL_CondSignal(is->pictq_cond);
		SDL_UnlockMutex(is->pictq_mutex);

		if (is->video_tid != nullptr)
		{
			if (is->video_tid->joinable())
				is->video_tid->join();

			delete is->video_tid;
			is->video_tid = nullptr;
		}

		packet_queue_flush(&is->videoq);

#ifdef WIN32

		if (dlg->GetDXVA2Compatible())
			dxva2_uninit(is->videoCtx);
#endif
		avcodec_close(is->videoCtx);
		break;
	case AVMEDIA_TYPE_SUBTITLE:
		packet_queue_abort(&is->subtitleq);

		/* note: we also signal this mutex to make sure we deblock the
		video thread in all cases */
		SDL_LockMutex(is->subpq_mutex);
		is->subtitle_stream_changed = 1;

		SDL_CondSignal(is->subpq_cond);
		SDL_UnlockMutex(is->subpq_mutex);

		//SDL_WaitThread(is->subtitle_tid, nullptr);
		if (is->subtitle_tid != nullptr)
		{
			if (is->subtitle_tid->joinable())
				is->subtitle_tid->join();

			delete is->subtitle_tid;
			is->subtitle_tid = nullptr;
		}

		packet_queue_flush(&is->subtitleq);
		avcodec_close(is->subtitleCtx);
		break;
	default:
		break;
	}

	ic->streams[stream_index]->discard = AVDISCARD_ALL;
	

	switch (ic->streams[stream_index]->codecpar->codec_type) {
	case AVMEDIA_TYPE_AUDIO:
		is->audio_st = nullptr;
		is->audio_stream = -1;
		break;
	case AVMEDIA_TYPE_VIDEO:
		is->video_st = nullptr;
		is->video_stream = -1;
		break;
	case AVMEDIA_TYPE_SUBTITLE:
		is->subtitle_st = nullptr;
		is->subtitle_stream = -1;
		break;
	default:
		break;
	}
}


int CFFmfcPimpl::decode_interrupt_cb(void *ctx)
{
	VideoState *is = (VideoState *)ctx;
	bool abort = false;
	abortMutex.lock();
	abort = is->abort_request;
	abortMutex.unlock();
	return abort;
}

int CFFmfcPimpl::is_realtime(AVFormatContext *s)
{
	if (!strcmp(s->iformat->name, "rtp")
		|| !strcmp(s->iformat->name, "rtsp")
		|| !strcmp(s->iformat->name, "sdp")
		)
		return 1;

	if (s->pb && (!strncmp(s->filename, "rtp:", 4)
		|| !strncmp(s->filename, "udp:", 4)
		)
		)
		return 1;
	return 0;
}

/* this thread gets the stream from the disk or the network */
//½âÂëÏß³Ì£¬»ñµÃÊÓÒôÆµPacket²¢·ÅÈë¶ÓÁÐ
int CFFmfcPimpl::read_thread(void *arg)
{
	bool cleanup = false;
	VideoState *is = (VideoState *)arg;
	AVFormatContext *ic = nullptr;
	int err, i, ret;
	int st_index[AVMEDIA_TYPE_NB];
	AVPacket pkt1, *pkt = &pkt1;
	int eof = 0;
	int pkt_in_play_range = 0;
	AVDictionaryEntry *t;
	AVDictionary **opts;
	int orig_nb_streams;
	SDL_mutex *wait_mutex = SDL_CreateMutex();

	memset(st_index, -1, sizeof(st_index));
	is->last_video_stream = is->video_stream = -1;
	is->last_audio_stream = is->audio_stream = -1;
	is->last_subtitle_stream = is->subtitle_stream = -1;

	ic = avformat_alloc_context();
	ic->interrupt_callback.callback = decode_interrupt_cb;
	ic->interrupt_callback.opaque = is;
	// ´ò¿ªÊÓÆµÎÄ¼þ£¨Ã»ÓÐ´ò¿ª±à½âÂëÆ÷£©£¬³õÊ¼»¯AVFormatContext
	// ÐÂ°æÖÐÒÑ¾­½«Æä·ÏÆú
	//if(av_open_input_file(&pFormatCtx, filename, nullptr, 0, nullptr)!=0)
	//{
	//	printf("Couldn't open file.\n");
	//	return -1;
	//}
	//¸ÄÎªavformat_open_input()
	//--------------------------
	//if(avformat_open_input(&pFormatCtx,filename,nullptr,nullptr)!=0){
	//char rtspurl[100]="rtsp://169.254.197.35:8554/sh1943.mpg";
	//ÎªÁËÊ¹FFMPEG¿ÉÒÔÖ±½Ó´ÓÄÚ´æÖÐ¶ÁÈ¡Êý¾Ý
	//×¥°üµÄÊ±ºòÖ±½Ó´ÓÄÚ´æÖÐ¶ÁÈ¡Êý¾Ý

	err = avformat_open_input(&ic, is->filename, is->iformat, &is->_pimpl->format_opts);

	//if(avformat_open_input(&pFormatCtx,"sprink_12M.ts",nullptr,nullptr)!=0){
	////AfxMessageBox()ÊÇÈ«¾ÖµÄº¯Êý£¬¼Ç×¡ÔÚVCÀïÓÐAFX¿ª¹ØµÄ¶¼ÊÇÈ«¾Öº¯Êý¡£
	//¶øMessageBox()ÊÇCWndµÄ³ÉÔ±º¯Êý£¬Ò²¾ÍÊÇËµÔÚÄÄ¶¼¿ÉÒÔµ÷ÓÃAfxMessageBox£¨£©£¬
	//µ«ÊÇMessageBox()Ö»ÄÜÔÚ»ùÀàÊÇCWndµÄÀàÀïÃæÖ±½Óµ÷ÓÃ¡£ 
	//printf("ÊäÈë¸ñÊ½:%s\n",ic->iformat->name);
	//printf("IO»º´æ´óÐ¡:%d\n",ic->pb.buffer_size);
	if (err < 0) {
#ifdef CMDUTILS
		print_error(is->filename, err);
#endif
		ret = -1;
		goto fail;
	}

	if ((t = av_dict_get(is->_pimpl->format_opts, "", nullptr, AV_DICT_IGNORE_SUFFIX))) {
		av_log(nullptr, AV_LOG_ERROR, "Option %s not found.\n", t->key);
		ret = AVERROR_OPTION_NOT_FOUND;
		goto fail;
	}

	is->ic = ic;

	if (is->_pimpl->genpts)
		ic->flags |= AVFMT_FLAG_GENPTS;

	opts = is->_pimpl->setup_find_stream_info_opts(ic, is->_pimpl->codec_opts);
	orig_nb_streams = ic->nb_streams;
	// »ñÈ¡Á÷ÐÅÏ¢£¬¼ÓÈëAVFormatContextÖÐ
	//This is useful for file formats with no headers such as MPEG. 
	err = avformat_find_stream_info(ic, opts);
	if (err < 0) {
		fprintf(stderr, "%s: could not find codec parameters\n", is->filename);
		ret = -1;
		goto fail;
	}


	for (i = 0; i < orig_nb_streams; i++)
	{
		av_dict_free(&opts[i]);
	}

	av_freep(&opts);

	if (ic->pb)
		ic->pb->eof_reached = 0; // FIXME hack, ffplay maybe should not use url_feof() to test for the end

	if (is->_pimpl->seek_by_bytes < 0)
		is->_pimpl->seek_by_bytes = !!(ic->iformat->flags & AVFMT_TS_DISCONT);

	/* if seeking requested, we execute it */
	if (is->_pimpl->start_time != AV_NOPTS_VALUE) {
		int64_t timestamp;

		timestamp = is->_pimpl->start_time;
		/* add the stream start time */
		if (ic->start_time != AV_NOPTS_VALUE)
			timestamp += ic->start_time;
		ret = avformat_seek_file(ic, -1, INT64_MIN, timestamp, INT64_MAX, 0);
		if (ret < 0) {
			fprintf(stderr, "%s: could not seek to position %0.3f\n",
				is->filename, (double)timestamp / AV_TIME_BASE);
		}
	}

	// »ñÈ¡µÚ¸öÊÓÆµÁ÷£¬ÒôÆµÁ÷Ë÷Òý
	for (i = 0; i < ic->nb_streams; i++)
	{
		AVDictionaryEntry * tag = nullptr;

		const char * stream_type = av_get_media_type_string(ic->streams[i]->codecpar->codec_type);


		if (ic->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			while ((tag = av_dict_get(ic->streams[i]->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
			{
				printf("STREAM_%s_%s=%s\n", stream_type, tag->key, tag->value);
			}
		}
		else if (ic->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			while ((tag = av_dict_get(ic->streams[i]->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
			{
				printf("STREAM_%s_%s=%s\n", stream_type, tag->key, tag->value);
			}
		}
		else if (ic->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE)
		{
			while ((tag = av_dict_get(ic->streams[i]->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
			{
				printf("STREAM_%s_%s=%s\n", stream_type, tag->key, tag->value);
			}
		}

		ic->streams[i]->discard = AVDISCARD_ALL;
	}

	float zhenlv;
	if (!is->_pimpl->video_disable)
		st_index[AVMEDIA_TYPE_VIDEO] = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, is->_pimpl->wanted_stream[AVMEDIA_TYPE_VIDEO], -1, nullptr, 0);


	//int video_index = st_index[AVMEDIA_TYPE_VIDEO];
	//printf("ÊÓÆµ±àÂëÆ÷Ãû³Æ£º%s\n",ic->streams[video_index]->codec->codec_name);
	//printf("ÊÓÆµ¿í£º%d\n",ic->streams[video_index]->codec->width);
	//printf("ÊÓÆµ¸ß£º%d\n",ic->streams[video_index]->codec->height);
	//zhenlv=(ic->streams[video_index]->codec->time_base.den)/(ic->streams[video_index]->codec->time_base.num);
	//printf("ÊÓÆµÖ¡ÂÊ£º%f\n",zhenlv);


	if (!is->_pimpl->audio_disable)
		st_index[AVMEDIA_TYPE_AUDIO] = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, is->_pimpl->wanted_stream[AVMEDIA_TYPE_AUDIO], st_index[AVMEDIA_TYPE_VIDEO], nullptr, 0);

	//int audio_index = st_index[AVMEDIA_TYPE_AUDIO];
	//printf("ÒôÆµ±àÂëÆ÷Ãû³Æ£º%s\n",ic->streams[audio_index]->codec->codec_name);
	//printf("²ÉÑùÂÊ£º%d\n",ic->streams[audio_index]->codec->sample_rate);
	//printf("Ö¡´óÐ¡£º%d\n",ic->streams[audio_index]->codec->frame_size);
	//printf("ÉùµÀÊý£º%d\n",ic->streams[audio_index]->codec->channels);
	if (!is->_pimpl->video_disable)
		st_index[AVMEDIA_TYPE_SUBTITLE] =
		av_find_best_stream(ic, AVMEDIA_TYPE_SUBTITLE,
			is->_pimpl->wanted_stream[AVMEDIA_TYPE_SUBTITLE],
			(st_index[AVMEDIA_TYPE_AUDIO] >= 0 ?
				st_index[AVMEDIA_TYPE_AUDIO] :
				st_index[AVMEDIA_TYPE_VIDEO]),
			nullptr, 0);
	if (is->_pimpl->show_status) {
		av_dump_format(ic, 0, is->filename, 0);
	}

	is->show_mode = (VideoState::ShowMode)is->_pimpl->show_mode;

	/* open the streams */
	//´ò¿ªStream£¬ÊÓÆµ»òÒôÆµ
	if (st_index[AVMEDIA_TYPE_AUDIO] >= 0) {
		//´ò¿ª
		is->_pimpl->stream_component_open(is, st_index[AVMEDIA_TYPE_AUDIO]);
	}

	ret = -1;
	if (st_index[AVMEDIA_TYPE_VIDEO] >= 0) {
		ret = is->_pimpl->stream_component_open(is, st_index[AVMEDIA_TYPE_VIDEO]);
	}

	/*
#ifdef SDL2
	is->refresh_tid = SDL_CreateThread(refresh_thread, "refresh thread", is);
#else
	is->refresh_tid = SDL_CreateThread(refresh_thread, is);
#endif
*/
	is->refresh_tid = new std::thread(refresh_thread, is);

	if (is->show_mode == SHOW_MODE_NONE)
		is->show_mode = ret >= 0 ? (VideoState::ShowMode)SHOW_MODE_VIDEO : (VideoState::ShowMode)SHOW_MODE_RDFT;

	if (st_index[AVMEDIA_TYPE_SUBTITLE] >= 0) {
		is->_pimpl->stream_component_open(is, st_index[AVMEDIA_TYPE_SUBTITLE]);
	}

	if (is->video_stream < 0 && is->audio_stream < 0) {
		//AfxMessageBox(_T("could not open codecs"));
		ret = -1;
		goto fail;
	}

	if (is->_pimpl->infinite_buffer < 0 && is->_pimpl->is_realtime(ic))
		is->_pimpl->infinite_buffer = 1;
	//»ñÈ¡Ò»Ð©ÏµÍ³ÐÅÏ¢

	//×¢Òâ£ºÔÚ´Ë´¦ÉèÖÃMFC²ÎÊý
	//ffmfc_param_global(is);
	is->_pimpl->dlg->SetVideoDuration(is->ic->duration, is->ic->start_time);

	for (;;) {

		bool abort = false;
		abortMutex.lock();
		abort = exit_video;
		abortMutex.unlock();

		if (abort)
			break;

		if (is->paused != is->last_paused) {
			is->last_paused = is->paused;
			if (is->paused)
				is->read_pause_return = av_read_pause(ic);
			else
				av_read_play(ic);
		}
#if CONFIG_RTSP_DEMUXER || CONFIG_MMSH_PROTOCOL
		if (is->paused &&
			(!strcmp(ic->iformat->name, "rtsp") ||
			(ic->pb && !strncmp(input_filename.c_str(), "mmsh:", 5)))) {
			/* wait 10 ms to avoid trying to get another packet */
			/* XXX: horrible */
			SDL_Delay(10);
			continue;
		}
#endif


		//µ±µ÷Õû²¥·Å½ø¶ÈÒÔºó
		if (is->seek_req) {
			int64_t seek_target = is->seek_pos;
			int64_t seek_min = is->seek_rel > 0 ? seek_target - is->seek_rel + 2 : INT64_MIN;
			int64_t seek_max = is->seek_rel < 0 ? seek_target - is->seek_rel - 2 : INT64_MAX;
			// FIXME the +-2 is due to rounding being not done in the correct direction in generation
			//      of the seek_pos/seek_rel variables

			ret = avformat_seek_file(is->ic, -1, seek_min, seek_target, seek_max, is->seek_flags);
			if (ret < 0) {
				fprintf(stderr, "%s: error while seeking\n", is->ic->filename);
			}
			else {
				if (is->audio_stream >= 0) {
					//Çå¿Õµ±Ç°µÄPAcket¶ÓÁÐ
					is->_pimpl->packet_queue_flush(&is->audioq);
					is->_pimpl->packet_queue_put(&is->audioq, &is->_pimpl->flush_pkt);
				}
				if (is->subtitle_stream >= 0) {
					is->_pimpl->packet_queue_flush(&is->subtitleq);
					is->_pimpl->packet_queue_put(&is->subtitleq, &is->_pimpl->flush_pkt);
				}
				if (is->video_stream >= 0) {
					is->_pimpl->packet_queue_flush(&is->videoq);
					is->_pimpl->packet_queue_put(&is->videoq, &is->_pimpl->flush_pkt);
				}
			}
			is->seek_req = 0;
			eof = 0;
		}
		if (is->que_attachments_req) {
			avformat_queue_attached_pictures(ic);
			is->que_attachments_req = 0;
		}

		/* if the queue are full, no need to read more */
		//Èç¹ûPacket¶ÓÁÐÂúÁË£¬ÔòµÈ´ý
		if (is->_pimpl->infinite_buffer < 1 &&
			(is->audioq.size + is->videoq.size + is->subtitleq.size > MAX_QUEUE_SIZE
				|| ((is->audioq.nb_packets > MIN_FRAMES || is->audio_stream < 0 || is->audioq.abort_request)
					&& (is->videoq.nb_packets > MIN_FRAMES || is->video_stream < 0 || is->videoq.abort_request)
					&& (is->subtitleq.nb_packets > MIN_FRAMES || is->subtitle_stream < 0 || is->subtitleq.abort_request)))) {
			/* wait 10 ms */
			SDL_LockMutex(wait_mutex);
			SDL_CondWaitTimeout(is->continue_read_thread, wait_mutex, 10);
			SDL_UnlockMutex(wait_mutex);
			continue;
		}
		if (eof) {
			if (is->video_stream >= 0) {
				av_init_packet(pkt);
				pkt->data = nullptr;
				pkt->size = 0;
				pkt->stream_index = is->video_stream;
				is->_pimpl->packet_queue_put(&is->videoq, pkt);
			}
			if (is->audio_stream >= 0 &&
				is->audioCtx->codec->capabilities & AV_CODEC_CAP_DELAY) {
				av_init_packet(pkt);
				pkt->data = nullptr;
				pkt->size = 0;
				pkt->stream_index = is->audio_stream;
				is->_pimpl->packet_queue_put(&is->audioq, pkt);
			}
			SDL_Delay(10);
			if (is->audioq.size + is->videoq.size + is->subtitleq.size == 0) {
				if (is->_pimpl->loop != 1 && (!is->_pimpl->loop || --is->_pimpl->loop)) {
					is->_pimpl->stream_seek(is, is->_pimpl->start_time != AV_NOPTS_VALUE ? is->_pimpl->start_time : 0, 0, 0);
				}
				else if (is->_pimpl->autoexit) {
					ret = AVERROR_EOF;
					//is->_pimpl->dlg->OnBnClickedStop();
					goto fail;
				}
			}
			eof = 0;

			wxCommandEvent evt(FF_STOP_EVENT);
			is->_pimpl->parent->GetEventHandler()->AddPendingEvent(evt);
			is->paused = true;
			is->seek_pos = 0;
			cleanup = true;
			goto fail;
			continue;
		}

		//¶ÁÈ¡Ò»¸öPacket
		ret = av_read_frame(ic, pkt);
		//printf("Packet dts£º%d\n",pkt->dts);
		//printf("Packet pts£º%d\n",pkt->pts);
		//printf("Packet Size£º%d\n",pkt->size);
		//printf("Packet dts£º%d\n",pkt->dts);
		//´Ë´¦ÉèÖÃÍ¼±í²ÎÊý

		//ffmfc_param_packet(is,pkt);
		//--------------------
		if (ret < 0) {
			if (ret == AVERROR_EOF || ic->pb->eof_reached)//url_feof(ic->pb))
				eof = 1;
			if (ic->pb && ic->pb->error)
				break;
			SDL_LockMutex(wait_mutex);
			SDL_CondWaitTimeout(is->continue_read_thread, wait_mutex, 10);
			SDL_UnlockMutex(wait_mutex);
			continue;
		}
		/* check if packet is in play range specified by user, then queue, otherwise discard */
		pkt_in_play_range = is->_pimpl->duration == AV_NOPTS_VALUE ||
			(pkt->pts - ic->streams[pkt->stream_index]->start_time) *
			av_q2d(ic->streams[pkt->stream_index]->time_base) -
			(double)(is->_pimpl->start_time != AV_NOPTS_VALUE ? is->_pimpl->start_time : 0) / 1000000
			<= ((double)is->_pimpl->duration / 1000000);
		if (pkt->stream_index == is->audio_stream && pkt_in_play_range) {
			is->_pimpl->packet_queue_put(&is->audioq, pkt);
			//printf("ÒôÆµPacket£¬¼ÓÈëÒôÆµ¶ÓÁÐ\n");
			//---------------------------------
		}
		else if (pkt->stream_index == is->video_stream && pkt_in_play_range) {
			is->_pimpl->packet_queue_put(&is->videoq, pkt);
			//printf("ÊÓÆµPacket£¬¼ÓÈëÊÓÆµ¶ÓÁÐ\n");
		}
		else if (pkt->stream_index == is->subtitle_stream && pkt_in_play_range) {
			is->_pimpl->packet_queue_put(&is->subtitleq, pkt);
		}
		else {
			av_packet_unref(pkt);
		}
	}
	/* wait until the end */
	while (1) 
	{
		bool abort = false;
		abortMutex.lock();
		abort = exit_video;
		abortMutex.unlock();
		if (abort)
			break;
		SDL_Delay(100);
	}

	ret = 0;



fail:

	//wxCommandEvent evt(CLOSESTREAM_EVENT);
	//is->_pimpl->parent->GetEventHandler()->AddPendingEvent(evt);

	if (cleanup)
	{
		// close each stream 
		if (is->audio_stream >= 0)
			is->_pimpl->stream_component_close(is, is->audio_stream);
		if (is->video_stream >= 0)
			is->_pimpl->stream_component_close(is, is->video_stream);
		if (is->subtitle_stream >= 0)
			is->_pimpl->stream_component_close(is, is->subtitle_stream);
		if (is->ic) {
			avformat_close_input(&is->ic);
		}
	}



	SDL_DestroyMutex(wait_mutex);
	return 0;
}

void CFFmfcPimpl::CloseStream(VideoState *is)
{
	/* close each stream */
	if (is->audio_stream >= 0)
		is->_pimpl->stream_component_close(is, is->audio_stream);
	if (is->video_stream >= 0)
		is->_pimpl->stream_component_close(is, is->video_stream);
	if (is->subtitle_stream >= 0)
		is->_pimpl->stream_component_close(is, is->subtitle_stream);
	if (is->ic) {
		avformat_close_input(&is->ic);
	}

	//av_freep(&is->_pimpl->dst->data[0]);
	av_frame_free(&is->_pimpl->dst);
	if (is->_pimpl->scaleContext != nullptr)
		sws_freeContext(is->_pimpl->scaleContext);
	is->_pimpl->scaleContext = nullptr;
	is->_pimpl->dst = nullptr;
	is->_pimpl->first = true;
}

//ÉèÖÃ¸÷ÖÖSDLÐÅºÅ£¬¿ªÊ¼½âÂëÏß³Ì
CFFmfcPimpl::VideoState * CFFmfcPimpl::stream_open(const char *filename, AVInputFormat *iformat)
{
	VideoState *is;

	is = (VideoState *)av_mallocz(sizeof(VideoState));
	if (!is)
		return nullptr;
	av_strlcpy(is->filename, filename, sizeof(is->filename));
	is->iformat = iformat;
	is->ytop = 0;
	is->xleft = 0;
	is->_pimpl = this;

	/* start video display */
	//³õÊ¼»¯¸÷ÖÖ±äÁ¿
	is->pictq_mutex = SDL_CreateMutex();
	is->pictq_cond = SDL_CreateCond();

	is->subpq_mutex = SDL_CreateMutex();
	is->subpq_cond = SDL_CreateCond();
	//³õÊ¼»¯Packet¶ÓÁÐ
	packet_queue_init(&is->videoq);
	packet_queue_init(&is->audioq);
	packet_queue_init(&is->subtitleq);

	is->continue_read_thread = SDL_CreateCond();

	is->av_sync_type = av_sync_type;
	//½âÂëÏß³Ì

	/*
#ifdef SDL2
	is->read_tid = SDL_CreateThread(read_thread, "Read Thread", is);
#else
	is->read_tid = SDL_CreateThread(read_thread, is);
#endif
*/

	is->read_tid = new std::thread(read_thread, is);

	if (!is->read_tid) {
		av_free(is);
		return nullptr;
	}
	return is;
}
//ÒÔÏÂ¼¸¸öº¯Êý¶¼ÊÇ´¦Àíevent_loop()ÖÐµÄ¸÷ÖÖ²Ù×÷µÄ
void CFFmfcPimpl::stream_cycle_channel(VideoState *is, int codec_type)
{
	AVFormatContext *ic = is->ic;
	int start_index, stream_index;
	int old_index;
	AVStream *st;

	if (codec_type == AVMEDIA_TYPE_VIDEO) {
		start_index = is->last_video_stream;
		old_index = is->video_stream;
	}
	else if (codec_type == AVMEDIA_TYPE_AUDIO) {
		start_index = is->last_audio_stream;
		old_index = is->audio_stream;
	}
	else {
		start_index = is->last_subtitle_stream;
		old_index = is->subtitle_stream;
	}
	stream_index = start_index;
	for (;;) {
		if (++stream_index >= is->ic->nb_streams)
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
		st = ic->streams[stream_index];
		if (st->codecpar->codec_id == codec_type) {
			/* check that parameters are OK */
			switch (codec_type) {
			case AVMEDIA_TYPE_AUDIO:
				if (st->codecpar->sample_rate != 0 &&
					st->codecpar->channels != 0)
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
	stream_component_close(is, old_index);
	stream_component_open(is, stream_index);
	if (codec_type == AVMEDIA_TYPE_VIDEO)
		is->que_attachments_req = 1;
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
void CFFmfcPimpl::stream_change_stream(VideoState *is, int codec_type, int newIndex)
{
	//AVFormatContext *ic = is->ic;
	int start_index, stream_index = newIndex;
	int old_index;
	//AVStream *st;

	if (codec_type == AVMEDIA_TYPE_VIDEO)
	{
		start_index = newIndex;//is->last_video_stream;
		old_index = is->video_stream;
	}
	else if (codec_type == AVMEDIA_TYPE_AUDIO)
	{
		start_index = newIndex;//is->last_audio_stream;
		old_index = is->audio_stream;
	}
	else
	{
		start_index = newIndex;//is->last_subtitle_stream;
		old_index = is->subtitle_stream;
	}
	stream_component_close(is, old_index);
	stream_component_open(is, stream_index);
	if (codec_type == AVMEDIA_TYPE_VIDEO)
		is->que_attachments_req = 1;
}

void CFFmfcPimpl::toggle_pause(VideoState *is)
{
	stream_toggle_pause(is);
	is->step = 0;
}

void CFFmfcPimpl::toggle_play(VideoState *is)
{
	if (is->paused)
	{
		stream_toggle_pause(is);
		is->step = 0;
	}
}

void CFFmfcPimpl::step_to_next_frame(VideoState *is)
{
	/* if the stream is paused unpause it, then step */
	if (is->paused)
		stream_toggle_pause(is);
	is->step = 1;
}

void CFFmfcPimpl::toggle_audio_display(VideoState *is, int mode)
{
#ifdef DISPLAY_VIDEO
	int bgcolor = SDL_MapRGB(dlg->screen->format, 0x00, 0x00, 0x00);
	//is->show_mode = (VideoState::ShowMode)((is->show_mode + 1) % SHOW_MODE_NB);
	switch (mode) {
	case SHOW_MODE_VIDEO:is->show_mode = (VideoState::ShowMode)SHOW_MODE_VIDEO; break;
	case SHOW_MODE_WAVES:is->show_mode = (VideoState::ShowMode)SHOW_MODE_WAVES; break;
	case SHOW_MODE_RDFT:is->show_mode = (VideoState::ShowMode)SHOW_MODE_RDFT; break;
	default:is->show_mode = (VideoState::ShowMode)SHOW_MODE_VIDEO; break;
	}
	fill_rectangle(dlg->screen,
		is->xleft, is->ytop, is->width, is->height,
		bgcolor);
	SDL_UpdateRect(dlg->screen, is->xleft, is->ytop, is->width, is->height);
#endif
}


int CFFmfcPimpl::lockmgr(void **mtx, enum AVLockOp op)
{
	switch (op) {
	case AV_LOCK_CREATE:
		*mtx = SDL_CreateMutex();
		if (!*mtx)
			return 1;
		return 0;
	case AV_LOCK_OBTAIN:
		return !!SDL_LockMutex((SDL_mutex *)*mtx);
	case AV_LOCK_RELEASE:
		return !!SDL_UnlockMutex((SDL_mutex *)*mtx);
	case AV_LOCK_DESTROY:
		SDL_DestroyMutex((SDL_mutex *)*mtx);
		return 0;
	}
	return 1;
}