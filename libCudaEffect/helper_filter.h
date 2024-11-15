#pragma once
#include "cuda_runtime.h"


__host__ __device__ struct ColorRef
{
    float red;
    float green;
    float blue;
    float alpha;
};

inline  __host__ __device__ ColorRef GetfColor(uchar* input, const int color_tid)
{
    ColorRef color;
    color.blue = input[color_tid];
    color.green = input[color_tid + 1];
    color.red = input[color_tid + 2];
    color.alpha = input[color_tid + 3];
    return color;
}



inline  __host__ __device__ float4 GetfloatColor(uchar* input, const int color_tid)
{
    float4 color;
    color.x = input[color_tid];
    color.y = input[color_tid + 1];
    color.z = input[color_tid + 2];
    color.w = input[color_tid + 3];
    return color;
}

inline  __host__ __device__ float4 GetColorSrc(const int x, const int y, uchar* input, const int colorWidthStep, int width, int height)
{
    if ((x < width) && (y < height))
    {
        const int color_tid = y * colorWidthStep + (4 * x);
        return GetfloatColor(input, color_tid);
    }
    return make_float4(0);
}

inline  __host__ __device__ ColorRef clamp(ColorRef colorIn, const float min, const float max)
{
    ColorRef color;
    color.blue = clamp(colorIn.blue, min, max);
    color.green = clamp(colorIn.green, min, max);
    color.red = clamp(colorIn.red, min, max);
    color.alpha = clamp(colorIn.alpha, min, max);
    return color;
}

//----------------------------------------------------
// Conversion du NV12 vers du 32 bits
//----------------------------------------------------
inline  __host__ __device__ void rgbaFloat4ToUchar4(uchar* output, int position, float4 rgba, float fScale)
{
    output[position] = (uchar)(rgba.x * fScale);
    output[position + 1] = (uchar)(rgba.y * fScale);
    output[position + 2] = (uchar)(rgba.z * fScale);
    output[position + 3] = (uchar)(rgba.w * fScale);
    output[position] = clamp(output[position], 0, 255);
    output[position + 1] = clamp(output[position + 1], 0, 255);
    output[position + 2] = clamp(output[position + 2], 0, 255);
    output[position + 3] = clamp(output[position + 3], 0, 255);
}


inline  __host__ __device__ void SetColor(uchar* output, const int color_tid, ColorRef color)
{
    output[color_tid] = static_cast<unsigned char>(color.blue);
    output[color_tid + 1] = static_cast<unsigned char>(color.green);
    output[color_tid + 2] = static_cast<unsigned char>(color.red);
    output[color_tid + 3] = static_cast<unsigned char>(color.alpha);
}

inline __host__ __device__ ColorRef operator+(ColorRef a, ColorRef b)
{
    ColorRef color;
    color.blue = a.blue + b.blue;
    color.green = a.green + b.green;
    color.red = a.red + b.red;
    color.alpha = a.alpha + b.alpha;
    return color;
}

inline __host__ __device__ ColorRef operator-(ColorRef a, ColorRef b)
{
    ColorRef color;
    color.blue = a.blue - b.blue;
    color.green = a.green - b.green;
    color.red = a.red - b.red;
    color.alpha = a.alpha - b.alpha;
    return color;
}

inline __host__ __device__ ColorRef operator*(ColorRef a, ColorRef b)
{
    ColorRef color;
    color.blue = a.blue * b.blue;
    color.green = a.green * b.green;
    color.red = a.red * b.red;
    color.alpha = a.alpha * b.alpha;
    return color;
}

inline __host__ __device__ ColorRef operator*(ColorRef a, float b)
{
    ColorRef color;
    color.blue = a.blue * b;
    color.green = a.green * b;
    color.red = a.red * b;
    color.alpha = a.alpha * b;
    return color;
}