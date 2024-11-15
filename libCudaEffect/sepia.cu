//
// CUDA implementation of Image Sharpening Filter
//

#include "sepia.h"
#include <cuda.h>
#include "cuda_runtime.h"
#include "helper_math.h"
#include "helper_filter.h"
#include <opencv2/core/utility.hpp>
#include <opencv2/core/cuda/common.hpp>
#define BLOCK_SIZE      16
#define FILTER_WIDTH    3       
#define FILTER_HEIGHT   3       

using namespace std;

//---------------------------------------------------------------------
//Noise Filter
//---------------------------------------------------------------------

inline  __host__ __device__ float Noise2d(int x, int y)
{
    int n = ((x + (y << 6)) << 13) ^ (x + (y << 6));
    return 0.2f * (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

inline  __host__ __device__ float CalculPosValue(int Xint, int Yint)
{
    int m = Xint + ((Yint) << 6);
    int n = (m << 13) ^ (m);
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

inline  __host__ __device__ float GetValue(float x, float y)
{
    int Xint = (int)x;
    int Yint = (int)y;

    float Xfrac = x - (float)Xint;
    float Yfrac = y - (float)Yint;

    float x0y0, x1y0, x0y1, x1y1;

    if (Xint != 0 || Yint != 0)
    {
        x0y0 = CalculPosValue(Xint, Yint);
        x1y0 = CalculPosValue(Xint + 1, Yint);
        x0y1 = CalculPosValue(Xint, Yint + 1);
        x1y1 = CalculPosValue(Xint + 1, Yint + 1);
    }
    else
    {
        x0y0 = Noise2d(0, 0);
        x1y0 = Noise2d(1, 0);
        x0y1 = Noise2d(0, 1);
        x1y1 = Noise2d(1, 1);
    }

    //interpolate between those values according to the x and y fractions
    float v1 = (x0y0 + (Xfrac * (x1y0 - x0y0)));
    float v2 = (x0y1 + (Xfrac * (x1y1 - x0y1)));
    float fin = (v1 + (Yfrac * (v2 - v1)));

    return fin;
}

__global__ void noiseFilter(uchar* input, uchar* output, int width, int height, int colorWidthStep, int grayWidthStep)
{
    const int x = blockIdx.x * blockDim.x + threadIdx.x;
    const int y = blockIdx.y * blockDim.y + threadIdx.y;

    //Only valid threads perform memory I/O
    if ((x < width) && (y < height))
    {
        const int position = y * colorWidthStep + (4 * x);
        float4 n = make_float4(Noise2d(x, y));
        float4 src_color = GetColorSrc(x, y, input, colorWidthStep, width, height) + n * 255.0f;
        float4 minimal = make_float4(0.0f);
        float4 maximal = make_float4(255.0f);
        src_color = clamp(src_color, minimal, maximal);
        rgbaFloat4ToUchar4(output, position, src_color, 1.0f);
    }
}


//---------------------------------------------------------------------
//Swirl Filter
//---------------------------------------------------------------------

inline  __host__ __device__ float EuclideanDist(float tcX, float tcY, float centerX, float centerY)
{
    float diffX = tcX - centerX;
    float diffY = tcY - centerY;
    return sqrt(diffX * diffX + diffY * diffY);
}

inline  __host__ __device__ float DotProduct(float tcX, float tcY, float qX, float qY)
{
    return tcX * qX + tcY * qY;
}

inline  __host__ __device__ float4 PostFX(uchar* input, int x, int y, float radius, float angleDegree, int colorWidthStep, int widthIn, int heightIn)
{
    float xOut = x;
    float yOut = y;

    //Calcul du centre
    float centerX = (float)widthIn / 2.0f;
    float centerY = (float)heightIn / 2.0f;

    float tcX = (float)x - centerX;
    float tcY = (float)y - centerY;

    float angle = angleDegree * 0.0174532925;

    float dist = EuclideanDist(x, y, centerX, centerY);

    if (dist < radius)
    {
        float percent = (radius - dist) / radius;
        float theta = percent * percent * angle * 8.0;
        float s = sin(theta);
        float c = cos(theta);
        xOut = DotProduct(tcX, tcY, c, -s);
        yOut = DotProduct(tcX, tcY, s, c);

        tcX = xOut;
        tcY = yOut;
    }

    tcX += centerX;
    tcY += centerY;

    return GetColorSrc((int)tcX, (int)tcY, input, colorWidthStep, widthIn, heightIn);
}

__global__ void swirlFilter(uchar* input, uchar* output, int width, int height, float radius, float angleDegree, int colorWidthStep, int grayWidthStep)
{
    const int x = blockIdx.x * blockDim.x + threadIdx.x;
    const int y = blockIdx.y * blockDim.y + threadIdx.y;

    //Only valid threads perform memory I/O
    if ((x < width) && (y < height))
    {
        const int position = y * colorWidthStep + (4 * x);
        float4 color = PostFX(input, x, y, radius, angleDegree, colorWidthStep, width, height);
        rgbaFloat4ToUchar4(output, position, color, 1.0f);
    }
}

//---------------------------------------------------------------------
//Application du filtre Soften
//	kernel = {  1, 1, 1, 1, 8, 1, 1, 1, 1 };
//	factor = 16;
//---------------------------------------------------------------------
__global__ void softenFilter(uchar* input, uchar* output, int width, int height, int colorWidthStep, int grayWidthStep)
{
    const int x = blockIdx.x * blockDim.x + threadIdx.x;
    const int y = blockIdx.y * blockDim.y + threadIdx.y;

    //Only valid threads perform memory I/O
    if ((x < width) && (y < height))
    {
        const int position = y * colorWidthStep + (4 * x);
        float4 sum = GetColorSrc(x - 1, y - 1, input, colorWidthStep, width, height);
        sum += GetColorSrc(x, y - 1, input, colorWidthStep, width, height);
        sum += GetColorSrc(x + 1, y - 1, input, colorWidthStep, width, height);
        sum += GetColorSrc(x - 1, y, input, colorWidthStep, width, height);
        sum += GetColorSrc(x, y, input, colorWidthStep, width, height) * make_float4(8.0f);
        sum += GetColorSrc(x + 1, y, input, colorWidthStep, width, height);
        sum += GetColorSrc(x - 1, y + 1, input, colorWidthStep, width, height);
        sum += GetColorSrc(x, y + 1, input, colorWidthStep, width, height);
        sum += GetColorSrc(x + 1, y + 1, input, colorWidthStep, width, height);
        sum = sum / make_float4(16.0f);
        rgbaFloat4ToUchar4(output, position, sum, 1.0f);
    }
}


//----------------------------------------------------
//Filtre Posterization
//----------------------------------------------------
__global__ void distorsionFilter(uchar* input, uchar* output, int width, int height, float correctionRadius, int colorWidthStep, int grayWidthStep)
{
    const int x = blockIdx.x * blockDim.x + threadIdx.x;
    const int y = blockIdx.y * blockDim.y + threadIdx.y;

    //Only valid threads perform memory I/O
    if ((x < width) && (y < height))
    {
        const int color_tid = y * colorWidthStep + (4 * x);

        float theta = 1;
        int halfWidth = (width / 2);
        int halfHeight = (height / 2);

        float newX = x - halfWidth;
        float newY = y - halfHeight;
        float value = pow((float)newX, (float)2.0) + pow((float)newY, (float)2.0);
        float distance = sqrt(value);
        float r = distance / correctionRadius;
        if (r != 0.0)
            theta = atan(r) / r;

        int sourceX = round(halfWidth + theta * newX);
        int sourceY = round(halfHeight + theta * newY);

        int positionSrc = sourceY * colorWidthStep + (4 * sourceX);

        ColorRef colorIn = GetfColor(input, positionSrc);

        SetColor(output, color_tid, colorIn);

    }
}
//----------------------------------------------------
//solarization Filter
//----------------------------------------------------
// Run Sharpening Filter on GPU
__global__ void posterisationFilter(uchar* input, uchar* output, unsigned int width, unsigned int height, int level, int colorWidthStep, int grayWidthStep)
{
    const int x = blockIdx.x * blockDim.x + threadIdx.x;
    const int y = blockIdx.y * blockDim.y + threadIdx.y;

    //Only valid threads perform memory I/O
    if ((x < width) && (y < height))
    {
        const int color_tid = y * colorWidthStep + (4 * x);

        //Location of gray pixel in output
        const int gray_tid = y * grayWidthStep + (4 * x);

        ColorRef colorIn = GetfColor(input, color_tid);
        ColorRef colorOut;

        int _levels = max(2, min(16, level));
        float _offset = (float)256 / (float)_levels;

        int red = colorIn.red / _offset;
        int green = colorIn.green / _offset;
        int blue = colorIn.blue / _offset;

        colorOut.red = (red * _offset);
        colorOut.green = (green * _offset);
        colorOut.blue = (blue * _offset);
        colorOut.alpha = colorIn.alpha;

        SetColor(output, gray_tid, colorOut);

    }
}


//----------------------------------------------------
//solarization Filter
//----------------------------------------------------
__global__ void solarizationFilter(uchar* input, uchar* output, unsigned int width, unsigned int height, float threshold, int colorWidthStep, int grayWidthStep)
{
    const int x = blockIdx.x * blockDim.x + threadIdx.x;
    const int y = blockIdx.y * blockDim.y + threadIdx.y;

    //Only valid threads perform memory I/O
    if ((x < width) && (y < height))
    {
        const int color_tid = y * colorWidthStep + (4 * x);
        float4 colorInput = GetColorSrc(x, y, input, colorWidthStep, width, height);
        float4 colorOutput = colorInput;

        float red = colorInput.x;
        float green = colorInput.y;
        float blue = colorInput.z;
        float fthreshold = threshold;

        if (red > fthreshold)
            colorOutput.x = 255.0f - red;
        else
            colorOutput.x = red;

        if (green > fthreshold)
            colorOutput.y = 255.0f - green;
        else
            colorOutput.y = green;

        if (blue > fthreshold)
            colorOutput.z = 255.0f - blue;
        else
            colorOutput.z = blue;

        rgbaFloat4ToUchar4(output, color_tid, colorOutput, 1.0f);
    }
}

//----------------------------------------------------
//dilate Filter
//----------------------------------------------------

__global__ void erodeFilter(uchar* input, uchar* output, int width, int height, int colorWidthStep, int grayWidthStep)
{
    const int x = blockIdx.x * blockDim.x + threadIdx.x;
    const int y = blockIdx.y * blockDim.y + threadIdx.y;

    //Only valid threads perform memory I/O
    if ((x < width) && (y < height))
    {
        const int color_tid = y * colorWidthStep + (4 * x);
        float4 minValue = make_float4(255.0f);

        for (int n = -1; n < 2; n++)
        {
            int posY = y + n;
            for (int m = -1; m < 2; m++)
            {
                int posX = x + m;
                if ((posX < width) && (posY < height))
                    minValue = fminf(GetColorSrc(posX, posY, input, colorWidthStep, width, height), minValue);
            }
        }

        rgbaFloat4ToUchar4(output, color_tid, minValue, 1.0f);
    }
}

__global__ void dilateFilter(uchar* input, uchar* output, int width, int height, int colorWidthStep, int grayWidthStep)
{
    const int x = blockIdx.x * blockDim.x + threadIdx.x;
    const int y = blockIdx.y * blockDim.y + threadIdx.y;

    //Only valid threads perform memory I/O
    if ((x < width) && (y < height))
    {
        const int color_tid = y * colorWidthStep + (4 * x);
        float4 maxValue = make_float4(0.0f);

        for (int n = -1; n < 2; n++)
        {
            int posY = y + n;
            for (int m = -1; m < 2; m++)
            {
                int posX = x + m;
                if ((posX < width) && (posY < height))
                    maxValue = fmaxf(GetColorSrc(posX, posY, input, colorWidthStep, width, height), maxValue);
            }
        }

        rgbaFloat4ToUchar4(output, color_tid, maxValue, 1.0f);
    }
}

//----------------------------------------------------
//sharpenMasking Filter
//----------------------------------------------------

__global__ void sharpenMasking(uchar* input, uchar* output, uchar* gaussian, int width, int height, float sharpness, int colorWidthStep, int grayWidthStep)
{
    const int xIndex = blockIdx.x * blockDim.x + threadIdx.x;
    const int yIndex = blockIdx.y * blockDim.y + threadIdx.y;

    //Only valid threads perform memory I/O
    if ((xIndex < width) && (yIndex < height))
    {
        //Location of colored pixel in input
        const int color_tid = yIndex * colorWidthStep + (4 * xIndex);

        //Location of gray pixel in output
        const int gray_tid = yIndex * grayWidthStep + (4 * xIndex);

        ColorRef colorIn = GetfColor(input, color_tid);
        ColorRef colorGaussian = GetfColor(gaussian, color_tid);

        ColorRef color = colorIn - colorGaussian;

        ColorRef value = colorIn + color * sharpness;

        value = clamp(value, 0.0f, 255.0f);

        SetColor(output, gray_tid, value);
    }
}

//----------------------------------------------------
//Filtre Sepia
//----------------------------------------------------
__global__ void sepiaFilter(uchar* input, uchar* output, int width, int height, int colorWidthStep, int grayWidthStep)
{
    const int xIndex = blockIdx.x * blockDim.x + threadIdx.x;
    const int yIndex = blockIdx.y * blockDim.y + threadIdx.y;

    //Only valid threads perform memory I/O
    if ((xIndex < width) && (yIndex < height))
    {
        //Location of colored pixel in input
        const int color_tid = yIndex * colorWidthStep + (4 * xIndex);

        //Location of gray pixel in output
        const int gray_tid = yIndex * grayWidthStep + (4 * xIndex);

        ColorRef color = GetfColor(input, color_tid);
        ColorRef colorOut;

        colorOut.red = (0.393f * color.blue) + (0.769f * color.green) + (0.189f * color.red);
        colorOut.green = (0.349f * color.blue) + (0.686f * color.green) + (0.168f * color.red);
        colorOut.blue = (0.272f * color.blue) + (0.534f * color.green) + (0.131f * color.red);

        color = clamp(colorOut, 0.0, 255.0);
        
        SetColor(output, gray_tid, color);

    }
}

//----------------------------------------------------
//Filtre Mosaic
//----------------------------------------------------
__global__ void mosaicFilter(uchar* input, uchar* output, int width, int height, int fTileSize, int colorWidthStep, int grayWidthStep)
{
    const int xIndex = blockIdx.x * blockDim.x + threadIdx.x;
    const int yIndex = blockIdx.y * blockDim.y + threadIdx.y;

    //Only valid threads perform memory I/O
    if ((xIndex < width) && (yIndex < height))
    {
        //Location of colored pixel in input
        const int color_tid = yIndex * colorWidthStep + (4 * xIndex);

        float w = fTileSize;
        float h = fTileSize;

        float s = floor(xIndex / w);
        float t = floor(yIndex / h);

        int sourcex = __float2int_rd(s * w);
        int sourcey = __float2int_rd(t * h);

        int positionSrc = sourcey * colorWidthStep + (4 * sourcex);

        ColorRef colorIn = GetfColor(input, positionSrc);

        SetColor(output, color_tid, colorIn);

    }
}

//----------------------------------------------------
//Filtre Median
//----------------------------------------------------


inline  __host__ __device__ void s2(float4* a, float4* b)
{
    float4 temp = *a;
    *a = fminf(*a, *b);
    *b = fmaxf(temp, *b);
}

inline  __host__ __device__ void mn3(float4* a, float4* b, float4* c)
{
    s2(a, b);
    s2(a, c);
}

inline  __host__ __device__ void mx3(float4* a, float4* b, float4* c)
{
    s2(b, c);
    s2(a, c);
}

inline  __host__ __device__ void mnmx3(float4* a, float4* b, float4* c)
{
    mx3(a, b, c);
    s2(a, b);
}

inline  __host__ __device__ void mnmx4(float4* a, float4* b, float4* c, float4* d)
{
    s2(a, b); s2(c, d); s2(a, c); s2(b, d);
}

inline  __host__ __device__ void mnmx5(float4* a, float4* b, float4* c, float4* d, float4* e)
{
    s2(a, b); s2(c, d); mn3(a, c, e); mx3(b, d, e);
}

inline  __host__ __device__ void mnmx6(float4* a, float4* b, float4* c, float4* d, float4* e, float4* f)
{
    s2(a, d); s2(b, e); s2(c, f); mn3(a, b, c); mx3(d, e, f);
}

__global__ void medianFilter(uchar* input, uchar* output, int width, int height, int colorWidthStep, int grayWidthStep)
{
    const int x = blockIdx.x * blockDim.x + threadIdx.x;
    const int y = blockIdx.y * blockDim.y + threadIdx.y;

    //Only valid threads perform memory I/O
    if ((x < width) && (y < height))
    {
        //Location of colored pixel in input
        const int color_tid = y * colorWidthStep + (4 * x);
        float4 k0 = GetColorSrc(x - 1, y - 1, input, colorWidthStep, width, height);
        float4 k1 = GetColorSrc(x, y - 1, input, colorWidthStep, width, height);
        float4 k2 = GetColorSrc(x + 1, y - 1, input, colorWidthStep, width, height);
        float4 k3 = GetColorSrc(x - 1, y, input, colorWidthStep, width, height);
        float4 k4 = GetColorSrc(x, y, input, colorWidthStep, width, height);
        float4 k5 = GetColorSrc(x + 1, y, input, colorWidthStep, width, height);
        float4 k6 = GetColorSrc(x - 1, y + 1, input, colorWidthStep, width, height);
        float4 k7 = GetColorSrc(x, y + 1, input, colorWidthStep, width, height);
        float4 k8 = GetColorSrc(x + 1, y + 1, input, colorWidthStep, width, height);

        mnmx6(&k0, &k1, &k2, &k3, &k4, &k5);
        mnmx5(&k1, &k2, &k3, &k4, &k6);
        mnmx4(&k2, &k3, &k4, &k7);
        mnmx3(&k3, &k4, &k8);

        rgbaFloat4ToUchar4(output, color_tid, k4, 1.0f);
    }

}

//----------------------------------------------------
//Filtre Photo Filtre
//----------------------------------------------------
__global__  void photoFilter(uchar* input, uchar* output, int width, int height, float intensity, uint4 color, int colorWidthStep, int grayWidthStep)
{
    const int xIndex = blockIdx.x * blockDim.x + threadIdx.x;
    const int yIndex = blockIdx.y * blockDim.y + threadIdx.y;

    //Only valid threads perform memory I/O
    if ((xIndex < width) && (yIndex < height))
    {
        //Location of colored pixel in input
        const int color_tid = yIndex * colorWidthStep + (4 * xIndex);

        //Location of gray pixel in output
        const int gray_tid = yIndex * grayWidthStep + (4 * xIndex);

        ColorRef colorIn = GetfColor(input, color_tid);
        ColorRef colorOut;

        float coeff = intensity / 100.0f;
        float diff = 1.0f - coeff;

        colorOut.red = (float)(color.x) * coeff + colorIn.red * diff;
        colorOut.green = (float)(color.y) * coeff + colorIn.green * diff;
        colorOut.blue = (float)(color.z) * coeff + colorIn.blue * diff;

        colorIn = clamp(colorOut, 0.0, 255.0);

        SetColor(output, gray_tid, colorIn);
    }

}

void CCudaComputeFilter::ApplyEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output)
{
    const int colorBytes = input.step * input.rows;
    const int grayBytes = output.step * output.rows;

    d_input = (uchar*)input.ptr();
    d_output = (uchar*)output.ptr();

    ExecuteEffect(input, output);

    // Synchronize to check for any kernel launch errors
    cudaSafeCall(cudaDeviceSynchronize(), "Kernel Launch Failed");
}

void CSepiaFilter::ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output)
{
    // Specify a reasonable block size
    const dim3 block(16, 16);

    // Calculate grid size to cover the whole image
    const dim3 grid((input.cols + block.x - 1) / block.x, (input.rows + block.y - 1) / block.y);

    // Launch the color conversion kernel
    sepiaFilter << <grid, block >> > (d_input, d_output, input.cols, input.rows, input.step, output.step);

}

void CPhotoFilter::ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output)
{
    // Specify a reasonable block size
    const dim3 block(16, 16);

    // Calculate grid size to cover the whole image
    const dim3 grid((input.cols + block.x - 1) / block.x, (input.rows + block.y - 1) / block.y);

    uint4 color;
    color.x = clValue.GetRed();
    color.y = clValue.GetGreen();
    color.z = clValue.GetBlue();

    // Launch the color conversion kernel
    photoFilter << <grid, block >> > (d_input, d_output, output.cols, output.rows, intensity, color, input.step, output.step);
}

void CMosaicFilter::ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output)
{
    // Specify a reasonable block size
    const dim3 block(16, 16);

    // Calculate grid size to cover the whole image
    const dim3 grid((input.cols + block.x - 1) / block.x, (input.rows + block.y - 1) / block.y);

    // Launch the color conversion kernel
    mosaicFilter << <grid, block >> > (d_input, d_output, output.cols, output.rows, fTileSize, input.step, output.step);
}

// The wrapper is used to call sharpening filter 
void CCudaSharpenMaskingFilter::ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output)
{
    // Specify a reasonable block size
    const dim3 block(16, 16);

    // Calculate grid size to cover the whole image
    const dim3 grid((input.cols + block.x - 1) / block.x, (input.rows + block.y - 1) / block.y);

    // Run BoxFilter kernel on CUDA 
    sharpenMasking << <grid, block >> > (d_input, d_output, gaussian_input, output.cols, output.rows, sharpness, input.step, output.step);

}

void CCudaMedianFilter::ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output)
{
    // Specify a reasonable block size
    const dim3 block(16, 16);

    // Calculate grid size to cover the whole image
    const dim3 grid((input.cols + block.x - 1) / block.x, (input.rows + block.y - 1) / block.y);

    // Run BoxFilter kernel on CUDA 
    medianFilter << <grid, block >> > (d_input, d_output, output.cols, output.rows, input.step, output.step);
}

void CDilateFilter::ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output)
{
    // Specify a reasonable block size
    const dim3 block(16, 16);

    // Calculate grid size to cover the whole image
    const dim3 grid((input.cols + block.x - 1) / block.x, (input.rows + block.y - 1) / block.y);

    // Run BoxFilter kernel on CUDA 
    dilateFilter << <grid, block >> > (d_input, d_output, output.cols, output.rows, input.step, output.step);
}

void CErodeFilter::ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output)
{
    // Specify a reasonable block size
    const dim3 block(16, 16);

    // Calculate grid size to cover the whole image
    const dim3 grid((input.cols + block.x - 1) / block.x, (input.rows + block.y - 1) / block.y);

    // Run BoxFilter kernel on CUDA 
    erodeFilter << <grid, block >> > (d_input, d_output, output.cols, output.rows, input.step, output.step);
}


// The wrapper is used to call sharpening filter 
void CSolarizationFilter::ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output)
{
    // Specify a reasonable block size
    const dim3 block(16, 16);

    // Calculate grid size to cover the whole image
    const dim3 grid((input.cols + block.x - 1) / block.x, (input.rows + block.y - 1) / block.y);

    // Run BoxFilter kernel on CUDA 
    solarizationFilter << <grid, block >> > (d_input, d_output, output.cols, output.rows, level, input.step, output.step);
}

// The wrapper is used to call sharpening filter 
void CPosterizationFilter::ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output)
{
    // Specify a reasonable block size
    const dim3 block(16, 16);

    // Calculate grid size to cover the whole image
    const dim3 grid((input.cols + block.x - 1) / block.x, (input.rows + block.y - 1) / block.y);

    // Run BoxFilter kernel on CUDA 
    posterisationFilter << <grid, block >> > (d_input, d_output, output.cols, output.rows, level, input.step, output.step);
}

// The wrapper is used to call sharpening filter 
void CDistorsionFilter::ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output)
{
    // Specify a reasonable block size
    const dim3 block(16, 16);

    // Calculate grid size to cover the whole image
    const dim3 grid((input.cols + block.x - 1) / block.x, (input.rows + block.y - 1) / block.y);

    // Run BoxFilter kernel on CUDA 
    distorsionFilter << <grid, block >> > (d_input, d_output, output.cols, output.rows, correctionRadius, input.step, output.step);
}

void CSoftenFilter::ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output)
{
    // Specify a reasonable block size
    const dim3 block(16, 16);

    // Calculate grid size to cover the whole image
    const dim3 grid((input.cols + block.x - 1) / block.x, (input.rows + block.y - 1) / block.y);

    // Run BoxFilter kernel on CUDA 
    softenFilter << <grid, block >> > (d_input, d_output, output.cols, output.rows, input.step, output.step);
}


void CSwirlFilter::ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output)
{
    // Specify a reasonable block size
    const dim3 block(16, 16);

    // Calculate grid size to cover the whole image
    const dim3 grid((input.cols + block.x - 1) / block.x, (input.rows + block.y - 1) / block.y);

    // Run BoxFilter kernel on CUDA 
    swirlFilter << <grid, block >> > (d_input, d_output, output.cols, output.rows, radius, angleDegree, input.step, output.step);
}

void CNoiseFilter::ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output)
{
    // Specify a reasonable block size
    const dim3 block(16, 16);

    // Calculate grid size to cover the whole image
    const dim3 grid((input.cols + block.x - 1) / block.x, (input.rows + block.y - 1) / block.y);

    // Run BoxFilter kernel on CUDA 
    noiseFilter << <grid, block >> > (d_input, d_output, output.cols, output.rows, input.step, output.step);
}