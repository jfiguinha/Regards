#include <header.h>
#include "hqdn3d.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <vector>

#define HQDN3D_SPATIAL_LUMA_DEFAULT    4.0f
#define HQDN3D_SPATIAL_CHROMA_DEFAULT  3.0f
#define HQDN3D_TEMPORAL_LUMA_DEFAULT   6.0f

namespace
{
    constexpr int HQDN3D_COEF_OFFSET = 0x1000;
    constexpr int HQDN3D_COEF_SIZE = 0x2001;

    inline unsigned int hqdn3d_lowpass_mul(
        int prev_mul,
        int curr_mul,
        const short* coef) noexcept
    {
        const int d = (prev_mul - curr_mul) >> 4;
        return static_cast<unsigned int>(curr_mul + coef[d]);
    }
}

void Chqdn3d::hqdn3d_precalc_coef(short* ct, double dist25)
{
    if (ct == nullptr)
        return;

    const double gamma =
        std::log(0.25) /
        std::log(1.0 -
            std::min(dist25, 252.0) / 255.0 -
            0.00001);

    for (int i = -255 * 16; i <= 255 * 16; ++i)
    {
        // hqdn3d_lowpass_mul() truncates the difference.
        const double f =
            (static_cast<double>(i) + 15.0 / 32.0) / 16.0;

        const double simil =
            1.0 - std::abs(f) / 255.0;

        const double c =
            std::pow(simil, gamma) * 256.0 * f;

        ct[HQDN3D_COEF_OFFSET + i] =
            static_cast<short>(
                (c < 0.0) ? (c - 0.5) : (c + 0.5));
    }

    ct[0] = (dist25 != 0.0) ? 1 : 0;
}

void Chqdn3d::hqdn3d_denoise_temporal(
    unsigned char* frame_src,
    unsigned char* frame_dst,
    unsigned short* frame_ant,
    int w,
    int h,
    short* temporal)
{
    if (frame_src == nullptr ||
        frame_dst == nullptr ||
        frame_ant == nullptr ||
        temporal == nullptr ||
        w <= 0 ||
        h <= 0)
    {
        return;
    }

    temporal += HQDN3D_COEF_OFFSET;

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            const unsigned int tmp =
                hqdn3d_lowpass_mul(
                    frame_ant[x],
                    frame_src[x] << 8,
                    temporal);

            frame_ant[x] =
                static_cast<unsigned short>(tmp);

            frame_dst[x] =
                static_cast<unsigned char>((tmp + 0x7F) >> 8);
        }

        frame_src += w;
        frame_dst += w;
        frame_ant += w;
    }
}

void Chqdn3d::hqdn3d_denoise_spatial(
    unsigned char* frame_src,
    unsigned char* frame_dst,
    unsigned short* line_ant,
    unsigned short* frame_ant,
    int w,
    int h,
    short* spatial,
    short* temporal)
{
    if (frame_src == nullptr ||
        frame_dst == nullptr ||
        line_ant == nullptr ||
        frame_ant == nullptr ||
        spatial == nullptr ||
        temporal == nullptr ||
        w <= 0 ||
        h <= 0)
    {
        return;
    }

    spatial += HQDN3D_COEF_OFFSET;
    temporal += HQDN3D_COEF_OFFSET;

    // First line: no top neighbour.
    unsigned int pixel_ant =
        static_cast<unsigned int>(frame_src[0]) << 8;

    for (int x = 0; x < w; ++x)
    {
        unsigned int tmp =
            hqdn3d_lowpass_mul(
                static_cast<int>(pixel_ant),
                frame_src[x] << 8,
                spatial);

        line_ant[x] =
            static_cast<unsigned short>(tmp);

        pixel_ant = tmp;

        tmp =
            hqdn3d_lowpass_mul(
                frame_ant[x],
                static_cast<int>(tmp),
                temporal);

        frame_ant[x] =
            static_cast<unsigned short>(tmp);

        frame_dst[x] =
            static_cast<unsigned char>((tmp + 0x7F) >> 8);
    }

    for (int y = 1; y < h; ++y)
    {
        frame_src += w;
        frame_dst += w;
        frame_ant += w;

        pixel_ant =
            static_cast<unsigned int>(frame_src[0]) << 8;

        int x = 0;

        for (; x < w - 1; ++x)
        {
            unsigned int tmp =
                hqdn3d_lowpass_mul(
                    line_ant[x],
                    static_cast<int>(pixel_ant),
                    spatial);

            line_ant[x] =
                static_cast<unsigned short>(tmp);

            pixel_ant =
                hqdn3d_lowpass_mul(
                    static_cast<int>(pixel_ant),
                    frame_src[x + 1] << 8,
                    spatial);

            tmp =
                hqdn3d_lowpass_mul(
                    frame_ant[x],
                    static_cast<int>(tmp),
                    temporal);

            frame_ant[x] =
                static_cast<unsigned short>(tmp);

            frame_dst[x] =
                static_cast<unsigned char>((tmp + 0x7F) >> 8);
        }

        // Last pixel of the line.
        unsigned int tmp =
            hqdn3d_lowpass_mul(
                line_ant[x],
                static_cast<int>(pixel_ant),
                spatial);

        line_ant[x] =
            static_cast<unsigned short>(tmp);

        tmp =
            hqdn3d_lowpass_mul(
                frame_ant[x],
                static_cast<int>(tmp),
                temporal);

        frame_ant[x] =
            static_cast<unsigned short>(tmp);

        frame_dst[x] =
            static_cast<unsigned char>((tmp + 0x7F) >> 8);
    }
}

void Chqdn3d::hqdn3d_denoise(
    unsigned char* frame_src,
    unsigned char* frame_dst,
    unsigned short* line_ant,
    unsigned short* frame_ant,
    int w,
    int h,
    short* spatial,
    short* temporal)
{
    if (frame_src == nullptr ||
        frame_dst == nullptr ||
        line_ant == nullptr ||
        frame_ant == nullptr ||
        spatial == nullptr ||
        temporal == nullptr ||
        w <= 0 ||
        h <= 0)
    {
        return;
    }

    // Le buffer d'historique temporel (frame_ant) est desormais possede et
    // dimensionne par le std::vector<unsigned short> Frame (cf. EnsureBuffers),
    // qui le zero-initialise a la creation / au redimensionnement / au reset
    // des parametres. Il n'y a donc plus besoin d'allocation paresseuse "premiere
    // frame" ici : l'ancien mecanisme reposait sur un reinterpret_cast<unsigned short**>
    // d'un std::vector, ce qui est un type-punning non defini (suppose que le
    // premier membre interne du vector est son pointeur de donnees). Ca "marchait"
    // par coincidence en build release sur les ABI courantes, mais provoque un
    // acces memoire invalide des que la disposition interne du vector differe
    // (STL en mode debug, autre implementation), et fuyait le buffer alloue par
    // new[] dans la branche jamais liberee.
    hqdn3d_denoise_spatial(
        frame_src,
        frame_dst,
        line_ant,
        frame_ant,
        w,
        h,
        spatial,
        temporal);
}

Chqdn3d::Chqdn3d(
    const int& width,
    const int& height,
    const double& LumSpac,
    const double& temporalLumaDefault,
    const double& temporalSpatialLumaDefault)
    : spatial_luma(LumSpac),
    temporal_luma(
        temporalSpatialLumaDefault != 0.0
        ? temporalLumaDefault * LumSpac /
        temporalSpatialLumaDefault
        : temporalLumaDefault),
    h(width),
    w(height)
{
    // The original member initialization used w/h in the opposite
    // semantic order. Keep the public constructor order width,height.
    this->w = width;
    this->h = height;

    EnsureBuffers(width, height);

    hqdn3d_precalc_coef(
        hqdn3d_coef[0],
        spatial_luma);

    hqdn3d_precalc_coef(
        hqdn3d_coef[1],
        temporal_luma);
}

void Chqdn3d::EnsureBuffers(int width, int height)
{
    if (width <= 0 || height <= 0)
    {
        w = 0;
        h = 0;

        picture_y.clear();
        y_out.clear();
        Frame.clear();
        Line.clear();

        return;
    }

    if (w == width &&
        h == height &&
        picture_y.size() ==
        static_cast<size_t>(width) *
        static_cast<size_t>(height) &&
        y_out.size() == picture_y.size() &&
        Frame.size() == picture_y.size() &&
        Line.size() == static_cast<size_t>(width))
    {
        return;
    }

    w = width;
    h = height;

    const size_t pixelCount =
        static_cast<size_t>(width) *
        static_cast<size_t>(height);

    picture_y.resize(pixelCount);
    y_out.resize(pixelCount);
    Frame.resize(pixelCount);
    Line.resize(static_cast<size_t>(width));
}

void Chqdn3d::ResetTemporalState()
{
    std::fill(Frame.begin(), Frame.end(), 0);
    std::fill(Line.begin(), Line.end(), 0);
}

void Chqdn3d::UpdateParameter(
    const int& width,
    const int& height,
    const double& LumSpac,
    const double& temporalLumaDefault,
    const double& temporalSpatialLumaDefault)
{
    const bool dimensionsChanged =
        this->w != width ||
        this->h != height;

    if (dimensionsChanged)
    {
        EnsureBuffers(width, height);
        ResetTemporalState();
    }

    const double newSpatialLuma = LumSpac;

    const double newTemporalLuma =
        temporalSpatialLumaDefault != 0.0
        ? temporalLumaDefault *
        newSpatialLuma /
        temporalSpatialLumaDefault
        : temporalLumaDefault;

    const bool coefficientsChanged =
        newSpatialLuma != this->spatial_luma ||
        newTemporalLuma != this->temporal_luma;

    if (coefficientsChanged)
    {
        this->spatial_luma = newSpatialLuma;
        this->temporal_luma = newTemporalLuma;

        hqdn3d_precalc_coef(
            hqdn3d_coef[0],
            this->spatial_luma);

        hqdn3d_precalc_coef(
            hqdn3d_coef[1],
            this->temporal_luma);

        // A parameter change must not mix the previous filter history
        // with a different filter configuration.
        ResetTemporalState();
    }
}

Chqdn3d::~Chqdn3d()
{
    // Buffers are managed by std::vector.
}

uint8_t* Chqdn3d::ApplyDenoise3D(
    uint8_t* input,
    const int& width,
    const int& height)
{
    if (input == nullptr ||
        width <= 0 ||
        height <= 0)
    {
        return nullptr;
    }

    // The raw-buffer API has no parameter values available for a
    // reconfiguration, therefore the caller must use the same dimensions
    // as the configured filter.
    if (width != w || height != h)
        return nullptr;

    if (y_out.size() !=
        static_cast<size_t>(width) *
        static_cast<size_t>(height))
    {
        return nullptr;
    }

    hqdn3d_denoise(
        input,
        y_out.data(),
        Line.data(),
        Frame.data(),
        width,
        height,
        hqdn3d_coef[0],
        hqdn3d_coef[1]);

    return y_out.data();
}

int Chqdn3d::ApplyDenoise3D(cv::Mat& bitmapIn)
{
    if (bitmapIn.empty())
        return -1;

    if (bitmapIn.type() != CV_8UC3)
        return -1;

    const int width = bitmapIn.cols;
    const int height = bitmapIn.rows;

    if (width <= 0 || height <= 0)
        return -1;

    // Reconfigure buffers when the video/frame dimensions change.
    if (width != w || height != h)
    {
        EnsureBuffers(width, height);
        ResetTemporalState();
    }

    cv::Mat ycbcr;
    cv::Mat yChannel;

    cv::cvtColor(
        bitmapIn,
        ycbcr,
        cv::COLOR_BGR2YCrCb);

    cv::extractChannel(
        ycbcr,
        yChannel,
        0);

    if (yChannel.empty() ||
        yChannel.type() != CV_8UC1)
    {
        return -1;
    }

    const size_t rowBytes =
        static_cast<size_t>(width);

    // Handle both continuous and strided cv::Mat safely.
    if (yChannel.isContinuous())
    {
        std::memcpy(
            picture_y.data(),
            yChannel.data,
            rowBytes *
            static_cast<size_t>(height));
    }
    else
    {
        for (int y = 0; y < height; ++y)
        {
            std::memcpy(
                picture_y.data() +
                static_cast<size_t>(y) * rowBytes,
                yChannel.ptr<unsigned char>(y),
                rowBytes);
        }
    }

    hqdn3d_denoise(
        picture_y.data(),
        y_out.data(),
        Line.data(),
        Frame.data(),
        width,
        height,
        hqdn3d_coef[0],
        hqdn3d_coef[1]);

    if (yChannel.isContinuous())
    {
        std::memcpy(
            yChannel.data,
            y_out.data(),
            rowBytes *
            static_cast<size_t>(height));
    }
    else
    {
        for (int y = 0; y < height; ++y)
        {
            std::memcpy(
                yChannel.ptr<unsigned char>(y),
                y_out.data() +
                static_cast<size_t>(y) * rowBytes,
                rowBytes);
        }
    }

    cv::insertChannel(
        yChannel,
        ycbcr,
        0);

    cv::cvtColor(
        ycbcr,
        bitmapIn,
        cv::COLOR_YCrCb2BGR);

    return 0;
}