
#include <header.h>
#include <RGBAQuad.h>
#include <algorithm>
#include <cmath>
#include <cstdint>

#include "Color.h"

using namespace Regards::FiltreEffet;

namespace {
    constexpr float INV_255 = 1.0f / 255.0f;
    constexpr float RGB_MAX = 255.0f;

    inline int ClampByte(const float value) {
        return static_cast<int>(std::clamp(std::lround(value), 0L, 255L));
    }

    inline int ClampPercent(const long value) {
        return static_cast<int>(std::clamp(value, 0L, 100L));
    }

    inline long NormalizeHue(const long hue) {
        long result = hue % 360;

        if (result < 0) result += 360;

        return result;
    }
}  // namespace

CColor::CColor() = default;

CColor::~CColor() = default;

CRgbaquad CColor::YUVtoRGB(const CRgbaquad& yuvColor) {
    const float Y = yuvColor.GetFRed();

    const float U = static_cast<float>(yuvColor.GetGreen()) - 128.0f;

    const float V = static_cast<float>(yuvColor.GetBlue()) - 128.0f;

    // BT.601 full range
    const int R = ClampByte(Y + 1.403f * V);

    const int G = ClampByte(Y - 0.344f * U - 0.714f * V);

    const int B = ClampByte(Y + 1.770f * U);

    CRgbaquad rgb(static_cast<uint8_t>(R), static_cast<uint8_t>(G),
        static_cast<uint8_t>(B), yuvColor.GetAlpha());

    return rgb;
}

CRgbaquad CColor::RGBtoYUV(const CRgbaquad& color) {
    const float R = static_cast<float>(color.GetRed());

    const float G = static_cast<float>(color.GetGreen());

    const float B = static_cast<float>(color.GetBlue());

    const int Y = ClampByte(0.299f * R + 0.587f * G + 0.114f * B);

    const int U = ClampByte((B - static_cast<float>(Y)) * 0.565f + 128.0f);

    const int V = ClampByte((R - static_cast<float>(Y)) * 0.713f + 128.0f);

    CRgbaquad yuv(static_cast<uint8_t>(Y), static_cast<uint8_t>(U),
        static_cast<uint8_t>(V), color.GetAlpha());

    return yuv;
}

int CColor::HSBToRGB(HSB& HSBValue, CRgbaquad& rgb) {
    const long hue = NormalizeHue(HSBValue.Hue);

    const int saturation = ClampPercent(HSBValue.Saturation);

    const int brightness = ClampPercent(HSBValue.Brightness);

    const float s = static_cast<float>(saturation) / 100.0f;

    const float v = static_cast<float>(brightness) / 100.0f;

    if (s <= 0.0f) {
        const int value = ClampByte(v * RGB_MAX);

        rgb = CRgbaquad(static_cast<uint8_t>(value), static_cast<uint8_t>(value),
            static_cast<uint8_t>(value));

        return 0;
    }

    const float h = static_cast<float>(hue) / 60.0f;

    const int sector = static_cast<int>(h);

    const float fraction = h - static_cast<float>(sector);

    const float p = v * (1.0f - s);

    const float q = v * (1.0f - s * fraction);

    const float t = v * (1.0f - s * (1.0f - fraction));

    float red = 0.0f;
    float green = 0.0f;
    float blue = 0.0f;

    switch (sector) {
    case 0:
        red = v;
        green = t;
        blue = p;
        break;

    case 1:
        red = q;
        green = v;
        blue = p;
        break;

    case 2:
        red = p;
        green = v;
        blue = t;
        break;

    case 3:
        red = p;
        green = q;
        blue = v;
        break;

    case 4:
        red = t;
        green = p;
        blue = v;
        break;

    default:
        red = v;
        green = p;
        blue = q;
        break;
    }

    rgb = CRgbaquad(static_cast<uint8_t>(ClampByte(red * RGB_MAX)),
        static_cast<uint8_t>(ClampByte(green * RGB_MAX)),
        static_cast<uint8_t>(ClampByte(blue * RGB_MAX)));

    return 0;
}

int CColor::RGBToHSB(HSB& HSBValue, CRgbaquad& rgb) {
    const float r = static_cast<float>(rgb.GetRed());

    const float g = static_cast<float>(rgb.GetGreen());

    const float b = static_cast<float>(rgb.GetBlue());

    const float maxValue = std::max({ r, g, b });

    const float minValue = std::min({ r, g, b });

    const float delta = maxValue - minValue;

    HSBValue.Brightness = static_cast<long>((maxValue * 100.0f) / RGB_MAX);

    if (maxValue <= 0.0f) {
        HSBValue.Saturation = 0;
        HSBValue.Hue = 0;
        return 0;
    }

    HSBValue.Saturation = static_cast<long>((delta / maxValue) * 100.0f);

    if (delta <= 0.0f) {
        HSBValue.Hue = 0;
        return 0;
    }

    float hue;

    if (maxValue == r) {
        hue = 60.0f * ((g - b) / delta);

        if (hue < 0.0f) hue += 360.0f;
    }
    else if (maxValue == g) {
        hue = 60.0f * (2.0f + (b - r) / delta);
    }
    else {
        hue = 60.0f * (4.0f + (r - g) / delta);
    }

    if (hue >= 360.0f) hue = 0.0f;

    HSBValue.Hue = static_cast<long>(std::lround(hue));

    return 0;
}