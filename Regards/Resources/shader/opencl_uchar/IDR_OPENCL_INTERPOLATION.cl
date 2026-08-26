#pragma OPENCL EXTENSION cl_khr_fp64 : disable

// ============================================================================
// Optimized OpenCL interpolation
//
// Input / output format:
//   uint = 0xAABBGGRR
//
// Supported filters:
//   1  BBox
//   2  Hermite
//   3  Hanning
//   4  Catmull-Rom
//   5  Mitchell
//   6  Triangle
//   7  Quadratic
//   8  Blackman
//   9  Hamming
//   10 Gaussian
//   11 Bilinear
//   12 Nearest
//   default = Cubic
//
// Bicubic filters use a 4x4 neighborhood.
// ============================================================================

#define FILTER_PI   3.14159265358979323846f
#define FILTER_2PI  6.28318530717958647692f
#define FILTER_4PI  12.56637061435917295384f

#define FILTER_EPSILON 1.0e-6f

// ============================================================================
// RGBA packing / unpacking
// ============================================================================

inline float4 rgbaUintToFloat4(const uint c)
{
    return convert_float4((uint4)(
        c & 0xFFu,
        (c >> 8) & 0xFFu,
        (c >> 16) & 0xFFu,
        (c >> 24) & 0xFFu));
}

inline uint rgbaFloat4ToUint(float4 rgba)
{
    rgba = clamp(rgba, 0.0f, 255.0f);

    const uint4 c = convert_uint4(rgba + 0.5f);

    return (c.x & 0xFFu) |
           ((c.y & 0xFFu) << 8) |
           ((c.z & 0xFFu) << 16) |
           ((c.w & 0xFFu) << 24);
}

// ============================================================================
// Source access
//
// Clamp coordinates to the image boundaries.
//
// This is preferable to returning black outside the image because it avoids
// dark borders when a 4x4 interpolation kernel reaches the edge.
// ============================================================================

inline uint GetPixel(
    const __global uint* input,
    const int x,
    const int y,
    const int width,
    const int height)
{
    const int px = clamp(x, 0, width - 1);
    const int py = clamp(y, 0, height - 1);

    return input[px + py * width];
}

inline float4 GetColorSrc(
    const int x,
    const int y,
    const __global uint* input,
    const int width,
    const int height)
{
    return rgbaUintToFloat4(
        GetPixel(input, x, y, width, height));
}

inline uint GetColorSrcShort(
    const int x,
    const int y,
    const __global uint* input,
    const int width,
    const int height)
{
    return GetPixel(input, x, y, width, height);
}

// ============================================================================
// Filter support
// ============================================================================

inline int GetFilterRadius(const int type)
{
    switch (type)
    {
        case 1:     // BBox
        case 3:     // Hanning
        case 6:     // Triangle
        case 9:     // Hamming
        case 11:    // Bilinear
            return 1;

        case 2:     // Hermite
        case 7:     // Quadratic
            return 2;

        case 4:     // Catmull-Rom
        case 5:     // Mitchell
        case 8:     // Blackman
        case 10:    // Gaussian
        default:    // Cubic
            return 2;
    }
}

// ============================================================================
// BBox
// ============================================================================

inline float BBoxFilter(const float x)
{
    return fabs(x) <= 0.5f ? 1.0f : 0.0f;
}

// ============================================================================
// Hermite
// ============================================================================

inline float HermiteFilter(const float x)
{
    const float ax = fabs(x);

    if (ax >= 2.0f)
        return 0.0f;

    if (ax >= 1.0f)
        return 0.0f;

    return (2.0f * ax - 3.0f) * ax * ax + 1.0f;
}

// ============================================================================
// Hanning
// ============================================================================

inline float HanningFilter(const float x)
{
    const float ax = fabs(x);

    if (ax >= 1.0f)
        return 0.0f;

    return 0.54f + 0.46f * cos(FILTER_PI * x);
}

// ============================================================================
// Catmull-Rom
//
// Standard Catmull-Rom cubic spline.
// Support = [-2, 2]
// ============================================================================

inline float CatmullRomFilter(const float x)
{
    const float ax = fabs(x);

    if (ax >= 2.0f)
        return 0.0f;

    if (ax < 1.0f)
    {
        return 1.5f * ax * ax * ax
             - 2.5f * ax * ax
             + 1.0f;
    }

    return -0.5f * ax * ax * ax
         + 2.5f * ax * ax
         - 4.0f * ax
         + 2.0f;
}

// ============================================================================
// Mitchell-Netravali
//
// B = 1/3
// C = 1/3
// ============================================================================

inline float MitchellFilter(const float x)
{
    const float ax = fabs(x);

    if (ax >= 2.0f)
        return 0.0f;

    const float B = 1.0f / 3.0f;
    const float C = 1.0f / 3.0f;

    if (ax < 1.0f)
    {
        return ((12.0f - 9.0f * B - 6.0f * C) * ax * ax * ax
              + (-18.0f + 12.0f * B + 6.0f * C) * ax * ax
              + (6.0f - 2.0f * B)) / 6.0f;
    }

    return ((-B - 6.0f * C) * ax * ax * ax
          + (6.0f * B + 30.0f * C) * ax * ax
          + (-12.0f * B - 48.0f * C) * ax
          + (8.0f * B + 24.0f * C)) / 6.0f;
}

// ============================================================================
// Triangle
// ============================================================================

inline float TriangleFilter(const float x)
{
    const float ax = fabs(x);

    return ax < 1.0f ? 1.0f - ax : 0.0f;
}

// ============================================================================
// Quadratic
// ============================================================================

inline float QuadraticFilter(const float x)
{
    const float ax = fabs(x);

    if (ax >= 1.5f)
        return 0.0f;

    if (ax < 0.5f)
        return 0.75f - ax * ax;

    const float t = 1.5f - ax;

    return 0.5f * t * t;
}

// ============================================================================
// Blackman
// ============================================================================

inline float BlackmanFilter(const float x)
{
    const float ax = fabs(x);

    if (ax >= 1.0f)
        return 0.0f;

    const float t = x;

    return 0.42f
         + 0.5f * cos(FILTER_2PI * t / 2.0f)
         + 0.08f * cos(FILTER_4PI * t / 2.0f);
}

// ============================================================================
// Hamming + sinc
// ============================================================================

inline float HammingFilter(const float x)
{
    const float ax = fabs(x);

    if (ax >= 1.0f)
        return 0.0f;

    const float window =
        0.54f + 0.46f * cos(FILTER_2PI * x);

    const float sinc =
        ax < FILTER_EPSILON
        ? 1.0f
        : sin(FILTER_PI * x) / (FILTER_PI * x);

    return window * sinc;
}

// ============================================================================
// Gaussian
// ============================================================================

inline float GaussianFilter(const float x)
{
    const float ax = fabs(x);

    if (ax >= 1.25f)
        return 0.0f;

    return exp(-0.5f * x * x) * 0.3989422804014327f;
}

// ============================================================================
// Bilinear
// ============================================================================

inline float BilinearFilter(const float x)
{
    const float ax = fabs(x);

    return ax < 1.0f ? 1.0f - ax : 0.0f;
}

// ============================================================================
// Cubic
//
// B = 0
// C = 1/3
//
// This corresponds to the classic cubic filter used by the original code.
// ============================================================================

inline float CubicFilter(const float x)
{
    const float ax = fabs(x);

    if (ax >= 2.0f)
        return 0.0f;

    if (ax < 1.0f)
    {
        return
            (4.0f
            - 6.0f * ax * ax
            + 3.0f * ax * ax * ax) / 6.0f;
    }

    const float t = 2.0f - ax;

    return t * t * t / 6.0f;
}

// ============================================================================
// Filter selection
// ============================================================================

inline float KernelFilterSelection(
    const float x,
    const int type)
{
    switch (type)
    {
        case 1:
            return BBoxFilter(x);

        case 2:
            return HermiteFilter(x);

        case 3:
            return HanningFilter(x);

        case 4:
            return CatmullRomFilter(x);

        case 5:
            return MitchellFilter(x);

        case 6:
            return TriangleFilter(x);

        case 7:
            return QuadraticFilter(x);

        case 8:
            return BlackmanFilter(x);

        case 9:
            return HammingFilter(x);

        case 10:
            return GaussianFilter(x);

        case 11:
            return BilinearFilter(x);

        default:
            return CubicFilter(x);
    }
}

// ============================================================================
// Compute 4 horizontal weights
// ============================================================================

inline void ComputeWeights4(
    const float fraction,
    const int type,
    __private float* w0,
    __private float* w1,
    __private float* w2,
    __private float* w3)
{
    *w0 = KernelFilterSelection(-1.0f - fraction, type);
    *w1 = KernelFilterSelection(-fraction, type);
    *w2 = KernelFilterSelection(1.0f - fraction, type);
    *w3 = KernelFilterSelection(2.0f - fraction, type);

    const float sum =
        *w0 + *w1 + *w2 + *w3;

    if (fabs(sum) > FILTER_EPSILON)
    {
        const float inv = 1.0f / sum;

        *w0 *= inv;
        *w1 *= inv;
        *w2 *= inv;
        *w3 *= inv;
    }
    else
    {
        // Fallback to the center pixel.
        *w0 = 0.0f;
        *w1 = 1.0f;
        *w2 = 0.0f;
        *w3 = 0.0f;
    }
}

// ============================================================================
// 4x4 separable interpolation
//
// This is the main interpolation path.
//
// Horizontal pass:
//   4 pixels x 4 rows
//
// Vertical pass:
//   4 horizontal results
// ============================================================================

inline uint KernelExecution(
    const float x,
    const float y,
    const __global uint* input,
    const int widthIn,
    const int heightIn,
    const int type)
{
    // Nearest neighbour
    if (type == 12)
    {
        const int ix = convert_int_rtn(x);
        const int iy = convert_int_rtn(y);

        return GetColorSrcShort(
            ix,
            iy,
            input,
            widthIn,
            heightIn);
    }

    const int baseX = convert_int_rtn(floor(x));
    const int baseY = convert_int_rtn(floor(y));

    const float fx = x - (float)baseX;
    const float fy = y - (float)baseY;

    float wx0, wx1, wx2, wx3;
    float wy0, wy1, wy2, wy3;

    ComputeWeights4(
        fx,
        type,
        &wx0,
        &wx1,
        &wx2,
        &wx3);

    ComputeWeights4(
        fy,
        type,
        &wy0,
        &wy1,
        &wy2,
        &wy3);

    const int x0 = baseX - 1;
    const int x1 = baseX;
    const int x2 = baseX + 1;
    const int x3 = baseX + 2;

    const int y0 = baseY - 1;
    const int y1 = baseY;
    const int y2 = baseY + 1;
    const int y3 = baseY + 2;

    // ------------------------------------------------------------------------
    // Horizontal interpolation
    // ------------------------------------------------------------------------

    const float4 row0 =
        GetColorSrc(x0, y0, input, widthIn, heightIn) * wx0 +
        GetColorSrc(x1, y0, input, widthIn, heightIn) * wx1 +
        GetColorSrc(x2, y0, input, widthIn, heightIn) * wx2 +
        GetColorSrc(x3, y0, input, widthIn, heightIn) * wx3;

    const float4 row1 =
        GetColorSrc(x0, y1, input, widthIn, heightIn) * wx0 +
        GetColorSrc(x1, y1, input, widthIn, heightIn) * wx1 +
        GetColorSrc(x2, y1, input, widthIn, heightIn) * wx2 +
        GetColorSrc(x3, y1, input, widthIn, heightIn) * wx3;

    const float4 row2 =
        GetColorSrc(x0, y2, input, widthIn, heightIn) * wx0 +
        GetColorSrc(x1, y2, input, widthIn, heightIn) * wx1 +
        GetColorSrc(x2, y2, input, widthIn, heightIn) * wx2 +
        GetColorSrc(x3, y2, input, widthIn, heightIn) * wx3;

    const float4 row3 =
        GetColorSrc(x0, y3, input, widthIn, heightIn) * wx0 +
        GetColorSrc(x1, y3, input, widthIn, heightIn) * wx1 +
        GetColorSrc(x2, y3, input, widthIn, heightIn) * wx2 +
        GetColorSrc(x3, y3, input, widthIn, heightIn) * wx3;

    // ------------------------------------------------------------------------
    // Vertical interpolation
    // ------------------------------------------------------------------------

    const float4 result =
        row0 * wy0 +
        row1 * wy1 +
        row2 * wy2 +
        row3 * wy3;

    return rgbaFloat4ToUint(result);
}

// ============================================================================
// Coordinate transformation
//
// Handles:
//   - scaling
//   - rotation 90 / 180 / 270
//   - horizontal flip
//   - vertical flip
// ============================================================================

inline uint CalculateInterpolation(
    const __global uint* input,
    const int widthIn,
    const int heightIn,
    const int widthOut,
    const int heightOut,
    const int flipH,
    const int flipV,
    const int angle,
    const int type,
    const float ratioX,
    const float ratioY,
    const int x,
    const int y,
    const float left,
    const float top)
{
    float posX =
        ((float)x + left) * ratioX;

    float posY =
        ((float)y + top) * ratioY;

    // ------------------------------------------------------------------------
    // Rotation
    // ------------------------------------------------------------------------

    if (angle == 90)
    {
        const float tmp = posX;

        posX = posY;
        posY = (float)heightIn - tmp - 1.0f;
    }
    else if (angle == 180)
    {
        posX = (float)widthIn - posX - 1.0f;
        posY = (float)heightIn - posY - 1.0f;
    }
    else if (angle == 270)
    {
        const float tmp = posX;

        posX = (float)widthIn - posY - 1.0f;
        posY = tmp;
    }

    // ------------------------------------------------------------------------
    // Flip
    //
    // For 90 / 270 degrees the axes are exchanged.
    // ------------------------------------------------------------------------

    if (angle == 90 || angle == 270)
    {
        if (flipV)
            posX = (float)widthIn - posX - 1.0f;

        if (flipH)
            posY = (float)heightIn - posY - 1.0f;
    }
    else
    {
        if (flipH)
            posX = (float)widthIn - posX - 1.0f;

        if (flipV)
            posY = (float)heightIn - posY - 1.0f;
    }

    // ------------------------------------------------------------------------
    // Nearest
    // ------------------------------------------------------------------------

    if (type == 12)
    {
        return GetColorSrcShort(
            convert_int_rtn(posX),
            convert_int_rtn(posY),
            input,
            widthIn,
            heightIn);
    }

    return KernelExecution(
        posX,
        posY,
        input,
        widthIn,
        heightIn,
        type);
}

// ============================================================================
// Interpolation
// ============================================================================

__kernel void Interpolation(
    __global uint* output,
    const __global uint* input,
    const int widthIn,
    const int heightIn,
    const int widthOut,
    const int heightOut,
    const int flipH,
    const int flipV,
    const int angle,
    const int type)
{
    const int x = get_global_id(0);
    const int y = get_global_id(1);

    if (x >= widthOut || y >= heightOut)
        return;

    // ------------------------------------------------------------------------
    // For rotations 90 / 270 the output dimensions are exchanged.
    // ------------------------------------------------------------------------

    float ratioX;
    float ratioY;

    if (angle == 90 || angle == 270)
    {
        ratioX = (float)widthIn / (float)heightOut;
        ratioY = (float)heightIn / (float)widthOut;
    }
    else
    {
        ratioX = (float)widthIn / (float)widthOut;
        ratioY = (float)heightIn / (float)heightOut;
    }

    const int position =
        x + y * widthOut;

    output[position] =
        CalculateInterpolation(
            input,
            widthIn,
            heightIn,
            widthOut,
            heightOut,
            flipH,
            flipV,
            angle,
            type,
            ratioX,
            ratioY,
            x,
            y,
            0.0f,
            0.0f);
}

// ============================================================================
// InterpolationZone
// ============================================================================

__kernel void InterpolationZone(
    __global uint* output,
    const __global uint* input,
    const int widthIn,
    const int heightIn,
    const int widthOut,
    const int heightOut,
    const float left,
    const float top,
    const float bitmapWidth,
    const float bitmapHeight,
    const int flipH,
    const int flipV,
    const int angle,
    const int type)
{
    const int x = get_global_id(0);
    const int y = get_global_id(1);

    if (x >= widthOut || y >= heightOut)
        return;

    float ratioX;
    float ratioY;

    if (angle == 90 || angle == 270)
    {
        ratioX =
            (float)widthIn / bitmapHeight;

        ratioY =
            (float)heightIn / bitmapWidth;
    }
    else
    {
        ratioX =
            (float)widthIn / bitmapWidth;

        ratioY =
            (float)heightIn / bitmapHeight;
    }

    const int position =
        x + y * widthOut;

    output[position] =
        CalculateInterpolation(
            input,
            widthIn,
            heightIn,
            widthOut,
            heightOut,
            flipH,
            flipV,
            angle,
            type,
            ratioX,
            ratioY,
            x,
            y,
            left,
            top);
}

// ============================================================================
// InterpolationDirect
// ============================================================================

__kernel void InterpolationDirect(
    __global uint* output,
    const __global uint* input,
    const int widthIn,
    const int heightIn,
    const int widthOut,
    const int heightOut,
    const int type)
{
    const int x = get_global_id(0);
    const int y = get_global_id(1);

    if (x >= widthOut || y >= heightOut)
        return;

    const float ratioX =
        (float)widthIn / (float)widthOut;

    const float ratioY =
        (float)heightIn / (float)heightOut;

    const float posX =
        (float)x * ratioX;

    const float posY =
        (float)y * ratioY;

    const int position =
        x + y * widthOut;

    if (type == 12)
    {
        output[position] =
            GetColorSrcShort(
                convert_int_rtn(posX),
                convert_int_rtn(posY),
                input,
                widthIn,
                heightIn);
    }
    else
    {
        output[position] =
            KernelExecution(
                posX,
                posY,
                input,
                widthIn,
                heightIn,
                type);
    }
}