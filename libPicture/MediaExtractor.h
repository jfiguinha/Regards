#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/opt.h>
#include <libavutil/timestamp.h>
}

namespace Regards::Media
{
    // ─────────────────────────────────────────────────────────────
    // Timecode
    //
    // Format :
    //   HH:MM:SS
    //   HH:MM:SS.m
    //   HH:MM:SS.mm
    //   HH:MM:SS.mmm
    // ─────────────────────────────────────────────────────────────

    struct Timecode
    {
        int hours = 0;
        int minutes = 0;
        int seconds = 0;
        int millis = 0;

        /// Construit un Timecode depuis une chaîne.
        /// Lève std::invalid_argument si le format est incorrect.
        static Timecode FromString(
            std::string_view s);

        /// Conversion en secondes.
        [[nodiscard]]
        double ToSeconds() const noexcept
        {
            return
                hours * 3600.0 +
                minutes * 60.0 +
                seconds +
                millis / 1000.0;
        }

        /// Reconstitution sous la forme HH:MM:SS.mmm.
        [[nodiscard]]
        std::string ToString() const;

        /// Vérifie que les valeurs sont valides.
        [[nodiscard]]
        bool IsValid() const noexcept
        {
            return
                hours >= 0 &&
                minutes >= 0 &&
                minutes < 60 &&
                seconds >= 0 &&
                seconds < 60 &&
                millis >= 0 &&
                millis < 1000;
        }
    };


    // ─────────────────────────────────────────────────────────────
    // Période d'extraction
    //
    // start : début en secondes
    // end   : fin en secondes
    //         nullopt = jusqu'à la fin du fichier
    // ─────────────────────────────────────────────────────────────

    struct TimeRange
    {
        double start = 0.0;
        std::optional<double> end;

        [[nodiscard]]
        bool IsValid() const noexcept
        {
            return
                start >= 0.0 &&
                (!end.has_value() ||
                 *end > start);
        }

        [[nodiscard]]
        std::optional<double> Duration() const noexcept
        {
            if (!end.has_value())
                return std::nullopt;

            return *end - start;
        }
    };


    // ─────────────────────────────────────────────────────────────
    // Mode d'extraction
    // ─────────────────────────────────────────────────────────────

    enum class ExtractionMode
    {
        VideoOnly,
        AudioOnly,
        Both
    };


    // ─────────────────────────────────────────────────────────────
    // Options d'extraction
    // ─────────────────────────────────────────────────────────────

    struct ExtractionOptions
    {
        ExtractionMode mode =
            ExtractionMode::Both;

        // -1 = sélection automatique
        int videoIdx = -1;
        int audioIdx = -1;

        // Seek précis.
        bool accurate = true;

        // Callback de progression.
        //
        // progress : [0.0 ... 1.0]
        // retour false = annulation de l'opération
        std::function<bool(double)> progressCallback;
    };


    // ─────────────────────────────────────────────────────────────
    // Résultat d'une extraction
    // ─────────────────────────────────────────────────────────────

    struct ExtractionResult
    {
        bool success = false;

        std::string outputPath;

        double durationSecs = 0.0;

        std::int64_t bytesWritten = 0;

        std::string errorMessage;
    };


    // ─────────────────────────────────────────────────────────────
    // Wrappers RAII FFmpeg
    // ─────────────────────────────────────────────────────────────

    namespace detail
    {
        struct FormatContextDeleter
        {
            void operator()(
                AVFormatContext* ctx) const noexcept
            {
                if (ctx)
                    avformat_close_input(&ctx);
            }
        };


        struct OutputContextDeleter
        {
            void operator()(
                AVFormatContext* ctx) const noexcept
            {
                if (!ctx)
                    return;

                if (ctx->oformat &&
                    !(ctx->oformat->flags & AVFMT_NOFILE))
                {
                    avio_closep(&ctx->pb);
                }

                avformat_free_context(ctx);
            }
        };


        struct PacketDeleter
        {
            void operator()(
                AVPacket* pkt) const noexcept
            {
                if (pkt)
                    av_packet_free(&pkt);
            }
        };


        using UniqueInputFmt =
            std::unique_ptr<
                AVFormatContext,
                FormatContextDeleter>;


        using UniqueOutputFmt =
            std::unique_ptr<
                AVFormatContext,
                OutputContextDeleter>;


        using UniquePacket =
            std::unique_ptr<
                AVPacket,
                PacketDeleter>;
    }


    // ─────────────────────────────────────────────────────────────
    // Classe principale
    // ─────────────────────────────────────────────────────────────

    class MediaExtractor
    {
    public:

        explicit MediaExtractor(
            std::string inputPath);

        ~MediaExtractor() = default;


        // Non copiable.
        MediaExtractor(
            const MediaExtractor&) = delete;

        MediaExtractor& operator=(
            const MediaExtractor&) = delete;


        // Déplaçable.
        MediaExtractor(
            MediaExtractor&&) = default;

        MediaExtractor& operator=(
            MediaExtractor&&) = default;


        /// Extrait le média vers outputPath.
        [[nodiscard]]
        ExtractionResult Extract(
            const std::string& outputPath,
            const TimeRange& range,
            const ExtractionOptions& opts = {}) const;


        /// Durée totale du fichier source.
        [[nodiscard]]
        double TotalDuration() const noexcept
        {
            return m_durationSecs;
        }


        /// Nombre de flux vidéo.
        [[nodiscard]]
        int VideoStreamCount() const noexcept
        {
            return m_videoStreamCount;
        }


        /// Nombre de flux audio.
        [[nodiscard]]
        int AudioStreamCount() const noexcept
        {
            return m_audioStreamCount;
        }


        /// Chemin du fichier source.
        [[nodiscard]]
        const std::string& InputPath() const noexcept
        {
            return m_inputPath;
        }


    private:

        std::string m_inputPath;

        double m_durationSecs = 0.0;

        int m_videoStreamCount = 0;

        int m_audioStreamCount = 0;


        void ProbeInput();
    };


    // ─────────────────────────────────────────────────────────────
    // Fonctions utilitaires
    // ─────────────────────────────────────────────────────────────

    /// Coupe une vidéo entre timestart et timestop.
    ///
    /// Conserve les flux vidéo et audio.
    /// Aucun ré-encodage.
    [[nodiscard]]
    bool ExecuteFFmpegCutVideo(
        const std::string& fileIn,
        const std::string& timestart,
        const std::string& timestop,
        const std::string& fileOut);


    /// Extrait uniquement le flux vidéo.
    ///
    /// Aucun flux audio n'est écrit.
    /// Aucun ré-encodage.
    [[nodiscard]]
    bool ExecuteFFmpegExtractVideo(
        const std::string& fileIn,
        const std::string& timestart,
        const std::string& timestop,
        const std::string& fileOut);


    /// Extrait uniquement le flux audio.
    ///
    /// Aucun flux vidéo n'est écrit.
    /// Aucun ré-encodage.
    [[nodiscard]]
    bool ExecuteFFmpegExtractAudio(
        const std::string& fileIn,
        const std::string& timestart,
        const std::string& timestop,
        const std::string& fileOut);


    /// Combine le flux vidéo de fileVideo
    /// avec le flux audio de fileAudio.
    ///
    /// Aucun ré-encodage.
    [[nodiscard]]
    bool ExecuteFFmpegMuxVideoAudio(
        const std::string& fileVideo,
        const std::string& fileAudio,
        const std::string& fileOutput);


    /// Crée une piste audio répétée jusqu'à
    /// atteindre la durée demandée.
    [[nodiscard]]
    bool CreateLoopedAudio(
        const std::string& inputAudioFile,
        const std::string& outputFile,
        const std::string& timeVideo);


    /// Retourne la liste des accélérations matérielles
    /// disponibles parmi les périphériques FFmpeg.
    [[nodiscard]]
    std::vector<wxString> GetHardwareList();

} // namespace Regards::Media