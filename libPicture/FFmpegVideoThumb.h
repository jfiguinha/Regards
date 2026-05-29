#pragma once
#include "VideoPlayerInterface.h"

// Remplacement de cv::VideoCapture par l'API FFmpeg directe.
// Raison : cv::VideoCapture + CAP_FFMPEG génère des fuites mémoire sur Linux
// car la couche d'abstraction OpenCV ne libère pas systématiquement tous les
// contextes AVFormatContext / AVCodecContext / AVFrame alloués par FFmpeg.
// En appelant FFmpeg directement on contrôle exactement le cycle de vie
// de chaque ressource.

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/display.h>
}

#include <opencv2/core.hpp>

namespace Regards
{
    namespace Video
    {
        class CFFmpegVideoThumb : public IVideoPlayer
        {
        public:
            explicit CFFmpegVideoThumb(const wxString& filename);
            ~CFFmpegVideoThumb() override;

            // Non copiable — les ressources FFmpeg ne se partagent pas
            CFFmpegVideoThumb(const CFFmpegVideoThumb&)            = delete;
            CFFmpegVideoThumb& operator=(const CFFmpegVideoThumb&) = delete;

            bool        isOpened()    override;
            bool        IsOk()        override;

            void        SeekToBegin() override;
            void        SeekToFrame(const int& frameIndex);          // FIX : méthode manquante pour ThumbnailDataSQL
            void        SkipFrame(const int& nbFrameRelative) override; // FIX : sémantique relative corrigée
            int         SeekToPos(const int& sec) override;

            int         GetDuration()   override;
            int         GetFps()        override;
            int         GetTotalFrame() override;
            int         GetWidth()      override;
            int         GetHeight()     override;
            int         GetOrientation() override;

            bool        SupportThumbnail() override { return false; }
            AspectRatio GetAspectRatio()   override;
            void        GetAspectRatio(int& num, int& den) override;

            // FIX : invertRotation est maintenant réellement utilisé
            cv::Mat     GetVideoFrame(const bool& applyOrientation = true,
                                      const bool& invertRotation   = false) override;
            cv::Mat     GetVideoThumbnailFrame(const int& thumbnailWidth,
                                               const int& thumbnailHeight) override;

        private:
            // Libère toutes les ressources FFmpeg dans le bon ordre
            void        Release();

            // Décode la prochaine frame vidéo dans frame_av_
            // Retourne true si une frame a été décodée
            bool        DecodeNextFrame();

            // Applique la rotation EXIF/metadata sur une cv::Mat
            cv::Mat     ApplyRotation(cv::Mat& src, const int& orientation,
                                      const bool& invert);

            std::string         filename_;

            // Contexte de démuxage
            AVFormatContext*    fmt_ctx_    = nullptr;

            // Contexte de décodage
            AVCodecContext*     codec_ctx_  = nullptr;

            // Conversion de pixel format vers BGR (OpenCV)
            SwsContext*         sws_ctx_    = nullptr;

            // Paquet lu depuis le flux
            AVPacket*           packet_     = nullptr;

            // Frame décodée côté FFmpeg
            AVFrame*            frame_av_   = nullptr;

            // Frame convertie en BGR pour OpenCV
            AVFrame*            frame_bgr_  = nullptr;

            int                 video_stream_index_ = -1;
            bool                is_open_    = false;

            // Cache des propriétés (évite des appels répétés à FFmpeg)
            int                 width_      = 0;
            int                 height_     = 0;
            double              fps_        = 0.0;
            int64_t             nb_frames_  = 0;
            int                 orientation_= 0;
            int                 sar_num_    = 1;
            int                 sar_den_    = 1;

            // Position courante en frames (pour SkipFrame relatif)
            int64_t             current_frame_ = 0;
        };
    }
}
