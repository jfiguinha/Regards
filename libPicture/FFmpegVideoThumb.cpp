#include "header.h"
#include "FFmpegVideoThumb.h"
#include <ConvertUtility.h>
#include <opencv2/imgproc.hpp>

using namespace Regards::Video;

// ---------------------------------------------------------------------------
// Constructeur
// ---------------------------------------------------------------------------
CFFmpegVideoThumb::CFFmpegVideoThumb(const wxString& fileName)
    : IVideoPlayer(fileName)
{
    filename_ = CConvertUtility::ConvertToStdString(fileName);

    // --- Ouverture du fichier ---
    if (avformat_open_input(&fmt_ctx_, filename_.c_str(), nullptr, nullptr) < 0)
        return; // is_open_ reste false

    if (avformat_find_stream_info(fmt_ctx_, nullptr) < 0)
    {
        avformat_close_input(&fmt_ctx_); // fmt_ctx_ mis à nullptr par FFmpeg
        return;
    }

    // --- Recherche du meilleur flux vidéo ---
    const AVCodec* codec = nullptr;
    video_stream_index_ = av_find_best_stream(fmt_ctx_, AVMEDIA_TYPE_VIDEO,
                                               -1, -1, &codec, 0);
    if (video_stream_index_ < 0 || codec == nullptr)
    {
        avformat_close_input(&fmt_ctx_);
        return;
    }

    AVStream* stream = fmt_ctx_->streams[video_stream_index_];

    // --- Contexte de décodage ---
    codec_ctx_ = avcodec_alloc_context3(codec);
    if (!codec_ctx_)
    {
        avformat_close_input(&fmt_ctx_);
        return;
    }

    if (avcodec_parameters_to_context(codec_ctx_, stream->codecpar) < 0)
    {
        avcodec_free_context(&codec_ctx_);
        avformat_close_input(&fmt_ctx_);
        return;
    }

    // Active le multithreading FFmpeg
    codec_ctx_->thread_count = 0; // auto
    codec_ctx_->thread_type  = FF_THREAD_FRAME;

    if (avcodec_open2(codec_ctx_, codec, nullptr) < 0)
    {
        avcodec_free_context(&codec_ctx_);
        avformat_close_input(&fmt_ctx_);
        return;
    }

    // --- Cache des propriétés ---
    width_     = codec_ctx_->width;
    height_    = codec_ctx_->height;
    fps_       = av_q2d(stream->avg_frame_rate);
    nb_frames_ = stream->nb_frames;

    // Ratio d'aspect (SAR)
    AVRational sar = av_guess_sample_aspect_ratio(fmt_ctx_, stream, nullptr);
    sar_num_ = (sar.num > 0) ? sar.num : 1;
    sar_den_ = (sar.den > 0) ? sar.den : 1;

    // --- Orientation depuis les métadonnées du stream ---
    // FIX : lecture propre de la rotation via av_display_rotation_get
    const AVPacketSideData* sd = av_packet_side_data_get(stream->codecpar->coded_side_data,
                                                          stream->codecpar->nb_coded_side_data,
                                                          AV_PKT_DATA_DISPLAYMATRIX);
    if (sd)
    {
        double angle = -av_display_rotation_get(reinterpret_cast<const int32_t*>(sd->data));
        // Normalise en 0/90/180/270
        angle = std::fmod(angle + 360.0, 360.0);
        if      (angle > 315.0 || angle <=  45.0) orientation_ = 0;
        else if (angle >  45.0 && angle <= 135.0) orientation_ = 90;
        else if (angle > 135.0 && angle <= 225.0) orientation_ = 180;
        else                                       orientation_ = 270;
    }

    // --- Allocation des frames et du paquet ---
    packet_    = av_packet_alloc();
    frame_av_  = av_frame_alloc();
    frame_bgr_ = av_frame_alloc();

    if (!packet_ || !frame_av_ || !frame_bgr_)
    {
        // Échec d'allocation : on nettoie proprement
        av_packet_free(&packet_);
        av_frame_free(&frame_av_);
        av_frame_free(&frame_bgr_);
        avcodec_free_context(&codec_ctx_);
        avformat_close_input(&fmt_ctx_);
        return;
    }

    // --- Allocation du buffer BGR pour OpenCV ---
    int buf_size = av_image_get_buffer_size(AV_PIX_FMT_BGR24, width_, height_, 1);
    uint8_t* buf = static_cast<uint8_t*>(av_malloc(buf_size));
    if (!buf)
    {
        av_packet_free(&packet_);
        av_frame_free(&frame_av_);
        av_frame_free(&frame_bgr_);
        avcodec_free_context(&codec_ctx_);
        avformat_close_input(&fmt_ctx_);
        return;
    }
    av_image_fill_arrays(frame_bgr_->data, frame_bgr_->linesize,
                         buf, AV_PIX_FMT_BGR24, width_, height_, 1);

    // --- Contexte de conversion de format de pixel ---
    sws_ctx_ = sws_getContext(width_, height_, codec_ctx_->pix_fmt,
                               width_, height_, AV_PIX_FMT_BGR24,
                               SWS_BILINEAR, nullptr, nullptr, nullptr);
    if (!sws_ctx_)
    {
        av_freep(&buf); // buf est géré manuellement ici
        av_packet_free(&packet_);
        av_frame_free(&frame_av_);
        av_frame_free(&frame_bgr_);
        avcodec_free_context(&codec_ctx_);
        avformat_close_input(&fmt_ctx_);
        return;
    }

    is_open_ = true;
}

// ---------------------------------------------------------------------------
// Destructeur — libération complète et ordonnée de toutes les ressources
// FIX [critique #3] : plus de raw pointer, chaque ressource FFmpeg est
// libérée via sa propre fonction de destruction, dans le bon ordre.
// ---------------------------------------------------------------------------
CFFmpegVideoThumb::~CFFmpegVideoThumb()
{
    Release();
}

void CFFmpegVideoThumb::Release()
{
    if (sws_ctx_)
    {
        sws_freeContext(sws_ctx_);
        sws_ctx_ = nullptr;
    }

    // Le buffer BGR a été alloué avec av_malloc et rattaché à frame_bgr_->data[0]
    if (frame_bgr_)
    {
        av_freep(&frame_bgr_->data[0]);
        av_frame_free(&frame_bgr_);
    }

    if (frame_av_)
        av_frame_free(&frame_av_);

    if (packet_)
        av_packet_free(&packet_);

    if (codec_ctx_)
        avcodec_free_context(&codec_ctx_);

    if (fmt_ctx_)
        avformat_close_input(&fmt_ctx_); // met fmt_ctx_ à nullptr

    is_open_       = false;
    current_frame_ = 0;
}

// ---------------------------------------------------------------------------
// État
// ---------------------------------------------------------------------------
bool CFFmpegVideoThumb::isOpened() { return is_open_; }
bool CFFmpegVideoThumb::IsOk()     { return is_open_; }

// ---------------------------------------------------------------------------
// Propriétés
// ---------------------------------------------------------------------------

// FIX [critique #1] : vérification isOpen EN PREMIER
// FIX [critique #2] : garde contre fps == 0
int CFFmpegVideoThumb::GetDuration()
{
    if (!is_open_)
        return 0;
    if (fps_ <= 0.0)
        return 0;
    return static_cast<int>(static_cast<double>(nb_frames_) / fps_);
}

int CFFmpegVideoThumb::GetFps()
{
    if (!is_open_)
        return 0;
    return static_cast<int>(fps_);
}

int CFFmpegVideoThumb::GetTotalFrame()
{
    if (!is_open_)
        return 0;
    return static_cast<int>(nb_frames_);
}

// FIX [mineur #11] : width_ et height_ sont initialisés dans le constructeur
// depuis les paramètres du codec — ils ne sont plus jamais 0 si is_open_ est vrai.
int CFFmpegVideoThumb::GetWidth()
{
    return width_;
}

int CFFmpegVideoThumb::GetHeight()
{
    return height_;
}

int CFFmpegVideoThumb::GetOrientation()
{
    return orientation_;
}

AspectRatio CFFmpegVideoThumb::GetAspectRatio()
{
    AspectRatio ar;
    ar.num = sar_num_;
    ar.den = sar_den_;
    return ar;
}

void CFFmpegVideoThumb::GetAspectRatio(int& num, int& den)
{
    num = sar_num_;
    den = sar_den_;
}

// ---------------------------------------------------------------------------
// Navigation
// ---------------------------------------------------------------------------

void CFFmpegVideoThumb::SeekToBegin()
{
    if (!is_open_)
        return;
    av_seek_frame(fmt_ctx_, video_stream_index_, 0, AVSEEK_FLAG_BACKWARD);
    avcodec_flush_buffers(codec_ctx_);
    current_frame_ = 0;
}

// FIX [sérieux #7] : SeekToFrame ajouté pour ThumbnailDataSQL::SetMouseOn
void CFFmpegVideoThumb::SeekToFrame(const int& frameIndex)
{
    if (!is_open_ || fps_ <= 0.0)
        return;

    // Conversion frame → timestamp dans la time_base du stream
    AVStream* stream    = fmt_ctx_->streams[video_stream_index_];
    int64_t   timestamp = av_rescale_q(
        static_cast<int64_t>(frameIndex),
        AVRational{stream->avg_frame_rate.den, stream->avg_frame_rate.num},
        stream->time_base);

    av_seek_frame(fmt_ctx_, video_stream_index_, timestamp, AVSEEK_FLAG_BACKWARD);
    avcodec_flush_buffers(codec_ctx_);
    current_frame_ = frameIndex;
}

// FIX [sérieux #6] : SkipFrame est maintenant réellement RELATIF
void CFFmpegVideoThumb::SkipFrame(const int& nbFrameRelative)
{
    if (!is_open_)
        return;
    SeekToFrame(static_cast<int>(current_frame_) + nbFrameRelative);
}

// FIX [sérieux #5] : sec == 0 n'est plus ignoré
int CFFmpegVideoThumb::SeekToPos(const int& sec)
{
    if (!is_open_)
        return -1;
    if (fps_ <= 0.0)
        return -1;

    int64_t target_frame = static_cast<int64_t>(fps_ * sec);
    SeekToFrame(static_cast<int>(target_frame));
    return 0;
}

// ---------------------------------------------------------------------------
// Décodage interne
// ---------------------------------------------------------------------------
bool CFFmpegVideoThumb::DecodeNextFrame()
{
    while (av_read_frame(fmt_ctx_, packet_) >= 0)
    {
        if (packet_->stream_index != video_stream_index_)
        {
            av_packet_unref(packet_);
            continue;
        }

        if (avcodec_send_packet(codec_ctx_, packet_) < 0)
        {
            av_packet_unref(packet_);
            continue;
        }
        av_packet_unref(packet_);

        int ret = avcodec_receive_frame(codec_ctx_, frame_av_);
        if (ret == AVERROR(EAGAIN))
            continue;   // besoin de plus de paquets
        if (ret < 0)
            return false;

        // Conversion vers BGR24
        sws_scale(sws_ctx_,
                  frame_av_->data,    frame_av_->linesize,    0, height_,
                  frame_bgr_->data,   frame_bgr_->linesize);

        av_frame_unref(frame_av_);
        current_frame_++;
        return true;
    }
    return false; // fin du flux
}

// ---------------------------------------------------------------------------
// Lecture de frame
// FIX [critique #4] : vérification is_open_ en premier
// FIX [mineur #9]   : invertRotation est maintenant réellement utilisé
// ---------------------------------------------------------------------------
cv::Mat CFFmpegVideoThumb::GetVideoFrame(const bool& applyOrientation,
                                           const bool& invertRotation)
{
    if (!is_open_)
        return cv::Mat();

    if (!DecodeNextFrame())
        return cv::Mat();

    // Copie du buffer BGR dans un cv::Mat (sans partager la mémoire FFmpeg)
    cv::Mat frame(height_, width_, CV_8UC3,
                  frame_bgr_->data[0],
                  static_cast<size_t>(frame_bgr_->linesize[0]));
    cv::Mat result = frame.clone(); // propriété exclusive pour l'appelant

    if (applyOrientation && orientation_ != 0)
        result = ApplyRotation(result, orientation_, invertRotation);

    return result;
}

cv::Mat CFFmpegVideoThumb::ApplyRotation(cv::Mat& src, const int& orientation,
                                           const bool& invert)
{
    cv::Mat dst;
    int effective = invert ? (360 - orientation) % 360 : orientation;
    printf("CFFmpegVideoThumb::ApplyRotation : %d \n", orientation);
    switch (effective)
    {
    case 90:
        cv::rotate(src, dst, cv::ROTATE_90_CLOCKWISE);
        break;
    case 180:
        cv::rotate(src, dst, cv::ROTATE_180);
        break;
    case 270:
        cv::rotate(src, dst, cv::ROTATE_90_COUNTERCLOCKWISE);
        break;
    default:
        dst = src;
        break;
    }
    return dst;
}