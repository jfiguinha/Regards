#pragma OPENCL EXTENSION cl_khr_fp64 : disable

#define FILTER_PI   3.1415926536f
#define FILTER_2PI  6.2831853072f
#define FILTER_4PI  12.5663706144f
#define FILTER_EPSILON 1.0e-6f

// Unpacking / Packing RGBA optimiss
inline float4 rgbaUintToFloat4(const uint c)
{
    return (float4)(
        (float)(c & 0xFFu),
        (float)((c >> 8) & 0xFFu),
        (float)((c >> 16) & 0xFFu),
        (float)((c >> 24) & 0xFFu));
}

inline uint rgbaFloat4ToUint(float4 rgba)
{
    rgba = clamp(rgba, 0.0f, 255.0f) + 0.5f;
    return ((uint)rgba.x & 0xFFu) |
           (((uint)rgba.y & 0xFFu) << 8) |
           (((uint)rgba.z & 0xFFu) << 16) |
           (((uint)rgba.w & 0xFFu) << 24);
}

// Accs mmoire globale scuris
inline uint GetPixel(const __global uint* input, int x, int y, int width, int height)
{
    return input[clamp(x, 0, width - 1) + clamp(y, 0, height - 1) * width];
}

// valuation de filtres allge (utilisation de constantes directes multiplicatives au lieu de divisions)
inline float KernelFilterSelection(const float x, const int type)
{
    const float ax = fabs(x);

    if (type == 1)  return (ax <= 0.5f) ? 1.0f : 0.0f; // BBox
    if (type == 2)  return (ax >= 1.0f) ? 0.0f : ((2.0f * ax - 3.0f) * ax * ax + 1.0f); // Hermite
    if (type == 3)  return (ax >= 1.0f) ? 0.0f : (0.54f + 0.46f * cos(FILTER_PI * ax)); // Hanning
    if (type == 4) { // Catmull-Rom
        if (ax >= 2.0f) return 0.0f;
        if (ax < 1.0f)  return 1.5f * ax * ax * ax - 2.5f * ax * ax + 1.0f;
        return -0.5f * ax * ax * ax + 2.5f * ax * ax - 4.0f * ax + 2.0f;
    }
    if (type == 5) { // Mitchell-Netravali (B=1/3, C=1/3 pr-calcul  la compilation)
        if (ax >= 2.0f) return 0.0f;
        if (ax < 1.0f)  return (7.0f * ax * ax * ax - 12.0f * ax * ax + 5.33333333f) * 0.16666667f;
        return (-2.33333333f * ax * ax * ax + 12.0f * ax * ax - 20.0f * ax + 10.66666667f) * 0.16666667f;
    }
    if (type == 6 || type == 11) return (ax < 1.0f) ? (1.0f - ax) : 0.0f; // Triangle / Bilinear
    if (type == 7) { // Quadratic
        if (ax >= 1.5f) return 0.0f;
        if (ax < 0.5f)  return 0.75f - ax * ax;
        float t = 1.5f - ax;
        return 0.5f * t * t;
    }
    if (type == 8)  return (ax >= 1.0f) ? 0.0f : (0.42f + 0.5f * cos(FILTER_2PI * ax * 0.5f) + 0.08f * cos(FILTER_4PI * ax * 0.5f)); // Blackman
    if (type == 9) { // Hamming
        if (ax >= 1.0f) return 0.0f;
        return (0.54f + 0.46f * cos(FILTER_2PI * ax)) * ((ax < FILTER_EPSILON) ? 1.0f : (sin(FILTER_PI * ax) / (FILTER_PI * ax)));
    }
    if (type == 10) return (ax >= 1.25f) ? 0.0f : (exp(-0.5f * ax * ax) * 0.3989422804f); // Gaussian
    
    // Default: Cubic (B=0, C=1/3)
    if (ax >= 2.0f) return 0.0f;
    if (ax < 1.0f)  return (4.0f - 6.0f * ax * ax + 3.0f * ax * ax * ax) * 0.16666667f;
    float t = 2.0f - ax;
    return (t * t * t) * 0.16666667f;
}

inline void ComputeWeights4(const float fraction, const int type, __private float* w)
{
    w[0] = KernelFilterSelection(-1.0f - fraction, type);
    w[1] = KernelFilterSelection(-fraction, type);
    w[2] = KernelFilterSelection(1.0f - fraction, type);
    w[3] = KernelFilterSelection(2.0f - fraction, type);

    float sum = w[0] + w[1] + w[2] + w[3];
    if (fabs(sum) > FILTER_EPSILON)
    {
        float inv = 1.0f / sum;
        w[0] *= inv; w[1] *= inv; w[2] *= inv; w[3] *= inv;
    }
    else
    {
        w[0] = 0.0f; w[1] = 1.0f; w[2] = 0.0f; w[3] = 0.0f;
    }
}

inline uint KernelExecution(const float x, const float y, const __global uint* input, const int widthIn, const int heightIn, const int type)
{
    if (type == 12) // Nearest
    {
        return input[clamp((int)floor(x), 0, widthIn - 1) + clamp((int)floor(y), 0, heightIn - 1) * widthIn];
    }

    const int baseX = (int)floor(x);
    const int baseY = (int)floor(y);
    const float fx = x - (float)baseX;
    const float fy = y - (float)baseY;

    // Allocation sous forme de vecteurs de registres (Tableaux privs ultra-rapides)
    float wx[4];
    float wy[4];

    ComputeWeights4(fx, type, wx);
    ComputeWeights4(fy, type, wy);

    // Pr-calcul des lignes d'index pour conomiser des multiplications dans la boucle
    int idxY[4];
    idxY[0] = clamp(baseY - 1, 0, heightIn - 1) * widthIn;
    idxY[1] = clamp(baseY,     0, heightIn - 1) * widthIn;
    idxY[2] = clamp(baseY + 1, 0, heightIn - 1) * widthIn;
    idxY[3] = clamp(baseY + 2, 0, heightIn - 1) * widthIn;

    int idxX[4];
    idxX[0] = clamp(baseX - 1, 0, widthIn - 1);
    idxX[1] = clamp(baseX,     0, widthIn - 1);
    idxX[2] = clamp(baseX + 1, 0, widthIn - 1);
    idxX[3] = clamp(baseX + 2, 0, widthIn - 1);

    float4 row0 = rgbaUintToFloat4(input[idxX[0] + idxY[0]]) * wx[0] +
                  rgbaUintToFloat4(input[idxX[1] + idxY[0]]) * wx[1] +
                  rgbaUintToFloat4(input[idxX[2] + idxY[0]]) * wx[2] +
                  rgbaUintToFloat4(input[idxX[3] + idxY[0]]) * wx[3];

    float4 row1 = rgbaUintToFloat4(input[idxX[0] + idxY[1]]) * wx[0] +
                  rgbaUintToFloat4(input[idxX[1] + idxY[1]]) * wx[1] +
                  rgbaUintToFloat4(input[idxX[2] + idxY[1]]) * wx[2] +
                  rgbaUintToFloat4(input[idxX[3] + idxY[1]]) * wx[3];

    float4 row2 = rgbaUintToFloat4(input[idxX[0] + idxY[2]]) * wx[0] +
                  rgbaUintToFloat4(input[idxX[1] + idxY[2]]) * wx[1] +
                  rgbaUintToFloat4(input[idxX[2] + idxY[2]]) * wx[2] +
                  rgbaUintToFloat4(input[idxX[3] + idxY[2]]) * wx[3];

    float4 row3 = rgbaUintToFloat4(input[idxX[0] + idxY[3]]) * wx[0] +
                  rgbaUintToFloat4(input[idxX[1] + idxY[3]]) * wx[1] +
                  rgbaUintToFloat4(input[idxX[2] + idxY[3]]) * wx[2] +
                  rgbaUintToFloat4(input[idxX[3] + idxY[3]]) * wx[3];

    return rgbaFloat4ToUint(row0 * wy[0] + row1 * wy[1] + row2 * wy[2] + row3 * wy[3]);
}

inline uint CalculateInterpolation(const __global uint* input, const int widthIn, const int heightIn, const int widthOut, const int heightOut, const int flipH, const int flipV, const int angle, const int type, const float ratioX, const float ratioY, const int x, const int y, const float left, const float top)
{
    float posX = ((float)x + left) * ratioX;
    float posY = ((float)y + top) * ratioY;

    // Transformation de coordonnes optimise (Regroupement des axes)
    if (angle == 90) {
        float tmp = posX; posX = posY; posY = (float)heightIn - tmp - 1.0f;
    } else if (angle == 180) {
        posX = (float)widthIn - posX - 1.0f; posY = (float)heightIn - posY - 1.0f;
    } else if (angle == 270) {
        float tmp = posX; posX = (float)widthIn - posY - 1.0f; posY = tmp;
    }

    if (angle == 90 || angle == 270) {
        if (flipV) posX = (float)widthIn - posX - 1.0f;
        if (flipH) posY = (float)heightIn - posY - 1.0f;
    } else {
        if (flipH) posX = (float)widthIn - posX - 1.0f;
        if (flipV) posY = (float)heightIn - posY - 1.0f;
    }

    return KernelExecution(posX, posY, input, widthIn, heightIn, type);
}

__kernel void Interpolation(__global uint* output, const __global uint* input, const int widthIn, const int heightIn, const int widthOut, const int heightOut, const int flipH, const int flipV, const int angle, const int type)
{
    const int x = get_global_id(0);
    const int y = get_global_id(1);

    if (x >= widthOut || y >= heightOut) return;

    float ratioX = (angle == 90 || angle == 270) ? ((float)widthIn / (float)heightOut) : ((float)widthIn / (float)widthOut);
    float ratioY = (angle == 90 || angle == 270) ? ((float)heightIn / (float)widthOut) : ((float)heightIn / (float)heightOut);

    output[x + y * widthOut] = CalculateInterpolation(input, widthIn, heightIn, widthOut, heightOut, flipH, flipV, angle, type, ratioX, ratioY, x, y, 0.0f, 0.0f);
}

__kernel void InterpolationZone(__global uint* output, const __global uint* input, const int widthIn, const int heightIn, const int widthOut, const int heightOut, const float left, const float top, const float bitmapWidth, const float bitmapHeight, const int flipH, const int flipV, const int angle, const int type)
{
    const int x = get_global_id(0);
    const int y = get_global_id(1);

    if (x >= widthOut || y >= heightOut) return;

    float ratioX = (angle == 90 || angle == 270) ? ((float)widthIn / bitmapHeight) : ((float)widthIn / bitmapWidth);
    float ratioY = (angle == 90 || angle == 270) ? ((float)heightIn / bitmapWidth)  : ((float)heightIn / bitmapHeight);

    output[x + y * widthOut] = CalculateInterpolation(input, widthIn, heightIn, widthOut, heightOut, flipH, flipV, angle, type, ratioX, ratioY, x, y, left, top);
}

__kernel void InterpolationDirect(__global uint* output, const __global uint* input, const int widthIn, const int heightIn, const int widthOut, const int heightOut, const int type)
{
    const int x = get_global_id(0);
    const int y = get_global_id(1);

    if (x >= widthOut || y >= heightOut) return;

    const float posX = (float)x * ((float)widthIn / (float)widthOut);
    const float posY = (float)y * ((float)heightIn / (float)heightOut);

    output[x + y * widthOut] = KernelExecution(posX, posY, input, widthIn, heightIn, type);
}
