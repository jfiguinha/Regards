//
// CUDA implementation of Image Sharpening Filter
//

#include "pictureFilter.h"
#include <cuda.h>
#include "cuda_runtime.h"
#include "helper_math.h"
#include <opencv2/core/utility.hpp>
#include <opencv2/core/cuda/common.hpp>
#define BLOCK_SIZE      16
#define FILTER_WIDTH    3       
#define FILTER_HEIGHT   3       

using namespace std;

 __host__ __device__ struct ColorRef
{
     uchar red;
     uchar green;
     uchar blue;
     uchar alpha;
};

inline  __host__ __device__  uchar4 GetColorSrc(unsigned char* input, int position)
{
    uchar4 value;
    value.x = input[position];
    value.y = input[position + 1];
    value.z = input[position + 2];
    value.w = input[position + 3];
    return value;
}

inline  __host__ __device__  float4 GetfColorSrc(unsigned char* input, int position)
{
    float4 value;
    value.x = input[position];
    value.y = input[position + 1];
    value.z = input[position + 2];
    value.w = input[position + 3];
    return value;
}

//----------------------------------------------------
// Conversion du NV12 vers du 32 bits
//----------------------------------------------------
inline  __host__ __device__ void rgbaFloat4ToUchar4(uchar * output, int position, float4 rgba, float fScale)
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


inline  __host__ __device__  float4 GetColorSrc(int x, int y, uchar * input, int width, int height)
{
    if (x < width && y < height && y >= 0 && x >= 0)
    {
        int position = (x + y * width) * 4;
        float4 value;
        value.x = (float)input[position];
        value.y = (float)input[position + 1];
        value.z = (float)input[position + 2];
        value.w = (float)input[position + 3];
        return value;
    }
    return make_float4(0.0f, 0.0f, 0.0f,0.0f);
}


//----------------------------------------------------
//Filtre Niveau de gris
//----------------------------------------------------



//---------------------------------------------------------------------
//Limite les valeurs entre 0 et 1.0f
//---------------------------------------------------------------------
inline  __host__ __device__ float4 NormalizeValue(float4 sum)
{
    float4 value;
    value.x = max(min(sum.x, 255.0f), 0.0f);
    value.y = max(min(sum.y, 255.0f), 0.0f);
    value.z = max(min(sum.z, 255.0f), 0.0f);
    value.w = max(min(sum.w, 255.0f), 0.0f);
    return value;
}

//---------------------------------------------------------------------
//Application du filtre Motion Blur
//---------------------------------------------------------------------
__global__ void motionBlur(uchar* input, uchar * output,  int width, int height, const float* kernelMotion, const int2 * offsets, int kernelSize)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    int position = (x + y * width) * 4;
    if (x < width && y < height && y >= 0 && x >= 0)
    {
        float4 sum = make_float4(0);
        for (int i = 0; i < kernelSize; i++)
        {
            int u = x + offsets[i].x;
            int v = y + offsets[i].y;
            if ((u < 0) || (u >= width) || (v < 0) || (v >= height))
                continue;

            float4 color = kernelMotion[i] * GetColorSrc(u, v, input, width, height);
            sum = sum + color;
        }

        rgbaFloat4ToUchar4(output, position, NormalizeValue(sum), 1.0f);

    }
}





inline __host__ __device__ float4 make_float4(uchar4 value)
{
    float4 color = make_float4(0.0);
    color.x = value.x;
    color.y = value.y;
    color.z = value.z;
    color.w = value.w;
    return color;
}

inline __host__ __device__ uchar4 make_uchar4(float4 value)
{
    uchar4 color;;
    color.x = clamp((uchar)value.x,0,255);
    color.y = clamp((uchar)value.y, 0, 255);
    color.z = clamp((uchar)value.z, 0, 255);
    color.w = clamp((uchar)value.w, 0, 255);
    return color;
}

__global__ void cuda_filter2d(uchar* input, uchar* output, float* kernelMotion, int width, int height, int kernelSize)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    int position = (x + y * width) * 4;
    if (x < width && y < height && y >= 0 && x >= 0)
    {
        float4 data = GetColorSrc(x, y, input, width, height);
        float4 sum = make_float4(0);
        for (int i = 0; i < kernelSize; i++)
        {
            float4 color = kernelMotion[i] * data;
            sum = sum + color;
        }
        rgbaFloat4ToUchar4(output, position, NormalizeValue(sum), 1.0f);
    }
}



inline  __host__ __device__ ColorRef GetColor(uchar* input, const int color_tid)
{
    ColorRef color;
    color.blue = input[color_tid];
    color.green = input[color_tid + 1];
    color.red = input[color_tid + 2];
    color.alpha = input[color_tid + 3];
    return color;
}

/**
 * @brief      BGR to Gray Kernel
 *
 *             This is a simple image processing kernel that converts color
 *             images to black and white by iterating over the individual
 *             pixels.
 *
 * @param      input           The input
 * @param      output          The output
 * @param[in]  width           The width
 * @param[in]  height          The height
 * @param[in]  colorWidthStep  The color width step
 * @param[in]  grayWidthStep   The gray width step
 */
__global__ void bgr_to_gray_kernel(unsigned char* input, unsigned char* output, int width, int height, int colorWidthStep, int grayWidthStep) {
    //2D Index of current thread
    const int xIndex = blockIdx.x * blockDim.x + threadIdx.x;
    const int yIndex = blockIdx.y * blockDim.y + threadIdx.y;

    //Only valid threads perform memory I/O
    if ((xIndex < width) && (yIndex < height))
    {
        //Location of colored pixel in input
        const int color_tid = yIndex * colorWidthStep + (4 * xIndex);

        //Location of gray pixel in output
        const int gray_tid = yIndex * grayWidthStep + (4 * xIndex);

        const unsigned char blue = input[color_tid];
        const unsigned char green = input[color_tid + 1];
        const unsigned char red = input[color_tid + 2];

        const float gray = red * 0.3f + green * 0.59f + blue * 0.11f;

        output[gray_tid] = static_cast<unsigned char>(gray);
        output[gray_tid + 1] = static_cast<unsigned char>(gray);
        output[gray_tid + 2] = static_cast<unsigned char>(gray);
        output[gray_tid + 3] = static_cast<unsigned char>(gray);
    }
}


void convert_to_gray(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output) {
    // Calculate total number of bytes of input and output image
    const int colorBytes = input.step * input.rows;
    const int grayBytes = output.step * output.rows;

    unsigned char* d_input, * d_output;

    d_input = (uchar*)input.ptr();
    d_output = (uchar*)output.ptr();

    // Specify a reasonable block size
    const dim3 block(16, 16);

    // Calculate grid size to cover the whole image
    const dim3 grid((input.cols + block.x - 1) / block.x, (input.rows + block.y - 1) / block.y);

    // Launch the color conversion kernel
    bgr_to_gray_kernel << <grid, block >> > (d_input, d_output, input.cols, input.rows, input.step, output.step);

    // Synchronize to check for any kernel launch errors
    cudaSafeCall(cudaDeviceSynchronize(), "Kernel Launch Failed");
}


void cuda_filter2d(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, const vector<float>& kernelMotion, int kernelSize)
{
    uchar* d_input;
    uchar* d_output;
    float* f_kernel;

    float* kernel = new float[kernelMotion.size()];
    for (auto i = 0; i < kernelMotion.size(); i++)
        kernel[i] = kernelMotion[i];

    cudaMalloc<float>(&f_kernel, kernelMotion.size());

    d_input = (uchar*)input.ptr();
    d_output = (uchar*)output.ptr();

    // Specify block size
    const dim3 block(BLOCK_SIZE, BLOCK_SIZE);

    // Calculate grid size to cover the whole image
    const dim3 grid((output.cols + block.x - 1) / block.x, (output.rows + block.y - 1) / block.y);

    // Run BoxFilter kernel on CUDA 
    cuda_filter2d << <grid, block >> > (d_input, d_output, f_kernel, output.cols, output.rows, kernelSize);

    cudaFree(f_kernel);
    delete[] kernel;
}




void motionBlur(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, const vector<double>& kernelMotion, const vector<wxPoint>& offsets, int kernelSize)
{
    uchar* d_input;
    uchar* d_output;
    float* f_kernel;
    int2 * i_offsetsMotion;

    float * kernel = new float[kernelMotion.size()];
    for (auto i = 0; i < kernelMotion.size(); i++)
        kernel[i] = kernelMotion[i];

    int2* offsetsMotion = new int2[offsets.size()];
    for (auto i = 0; i < offsets.size(); i++)
    {
        offsetsMotion[i].x = offsets[i].x;
        offsetsMotion[i].y = offsets[i].y;
    }

    // Allocate device memory
    cudaMalloc<float>(&f_kernel, kernelMotion.size());
    cudaMalloc<int2>(&i_offsetsMotion, offsets.size());

    d_input = (uchar*)input.ptr();
    d_output = (uchar*)output.ptr();

    cudaMemcpy(f_kernel, kernel, kernelMotion.size() * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(i_offsetsMotion, offsetsMotion, offsets.size() * sizeof(int2), cudaMemcpyHostToDevice);

    // Specify block size
    const dim3 block(BLOCK_SIZE, BLOCK_SIZE);

    // Calculate grid size to cover the whole image
    const dim3 grid((output.cols + block.x - 1) / block.x, (output.rows + block.y - 1) / block.y);

    // Run BoxFilter kernel on CUDA 
    motionBlur << <grid, block >> > (d_input, d_output, output.cols, output.rows, f_kernel, i_offsetsMotion, kernelSize);

    cudaFree(f_kernel);
    cudaFree(offsetsMotion);

    delete[] i_offsetsMotion;
    delete[] f_kernel;
}











