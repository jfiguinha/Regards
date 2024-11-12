//
// CUDA implementation of Image Sharpening Filter
//
#include "pictureFilter.h"
#include <cuda.h>
#include "cuda_runtime.h"
#include "helper_math.h"
#define BLOCK_SIZE      16
#define FILTER_WIDTH    3       
#define FILTER_HEIGHT   3       

using namespace std;

inline  __host__ __device__  uchar4 GetColorSrc(unsigned char* input, int position)
{
    uchar4 value;
    value.x = input[position];
    value.y = input[position + 1];
    value.z = input[position + 2];
    value.w = input[position + 3];
    return value;
}

//----------------------------------------------------
// Conversion du NV12 vers du 32 bits
//----------------------------------------------------
inline  __host__ __device__ void rgbaFloat4ToUchar4(unsigned char*& output, int position, float4 rgba, float fScale)
{
    output[position] = (uchar)(rgba.x * fScale);
    output[position + 1] = (uchar)(rgba.y * fScale);
    output[position + 2] = (uchar)(rgba.z * fScale);
    output[position + 3] = (uchar)(rgba.w * fScale);

}


inline  __host__ __device__  float4 GetColorSrc(int x, int y, unsigned char* input, int width, int height)
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


// Run Sharpening Filter on GPU
__global__ void mosaicFilter(uchar*srcImage, uchar*dstImage, unsigned int width, unsigned int height, int fTileSize)
{
   int x = blockIdx.x*blockDim.x + threadIdx.x;
   int y = blockIdx.y*blockDim.y + threadIdx.y;

	if(x < width && y < height && y >= 0 && x >= 0)	
    {
	    float w = fTileSize;
	    float h = fTileSize;		

	    float s = floor(x / w);
	    float t = floor(y / h);	
	    
	    int sourcex = __float2int_rd(s * w);
	    int sourcey = __float2int_rd(t * h);
	    
	    int positionSrc = (sourcex + sourcey * width) * 4;
	    int positionDest = (x + y * width) * 4;

        dstImage[positionDest] = srcImage[positionSrc];
        dstImage[positionDest + 1] = srcImage[positionSrc + 1];
        dstImage[positionDest + 2] = srcImage[positionSrc + 2];
        dstImage[positionDest + 3] = srcImage[positionSrc + 3];
    }
}


// Run Sharpening Filter on GPU
__global__ void solarizationFilter(uchar*srcImage, uchar*dstImage, unsigned int width, unsigned int height, int threshold)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    int position = (x + y * width) * 4;

	if(x < width && y < height && y >= 0 && x >= 0)	
    {
	    float4 colorInput = GetColorSrc(x,y,srcImage,width,height);
	    float4 colorOutput = colorInput;
	    
	    float red = colorInput.x;
	    float green = colorInput.y;
	    float blue = colorInput.z;
	    float fthreshold = (float)threshold;
	    
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

        dstImage[position] = colorOutput.x;
        dstImage[position + 1] = colorOutput.y;
        dstImage[position + 2] = colorOutput.z;
        dstImage[position + 3] = colorOutput.w;
    }
}


// Run Sharpening Filter on GPU
__global__ void posterisationFilter(uchar* srcImage, uchar* dstImage, unsigned int width, unsigned int height, int level)
{
   int x = blockIdx.x*blockDim.x + threadIdx.x;
   int y = blockIdx.y*blockDim.y + threadIdx.y;
   int position = (x + y * width) * 4;

	if(x < width && y < height && y >= 0 && x >= 0)	
    {
	    uchar4 colorInput = GetColorSrc(srcImage,position);
	    uchar4 colorOutput = colorInput;
	    int _levels = max(2, min(16, level));
	    float _offset = (float)256 / (float)_levels;
	    
	    int red = colorInput.x / _offset;
	    int green = colorInput.y / _offset;
	    int blue = colorInput.z / _offset;
	    
	    colorOutput.x = (red * _offset);
	    colorOutput.y = (green * _offset);
	    colorOutput.z = (blue * _offset);

        dstImage[position] = colorOutput.x;
        dstImage[position+1] = colorOutput.y;
        dstImage[position+2] = colorOutput.z;
        dstImage[position+3] = colorOutput.w;
    }
}



//----------------------------------------------------
//Filtre Posterization
//----------------------------------------------------
__global__ void distorsionFilter(uchar* output, uchar* input, int width, int height, float correctionRadius)
{
   int x = blockIdx.x*blockDim.x + threadIdx.x;
   int y = blockIdx.y*blockDim.y + threadIdx.y;
   int position = (x + y * width) * 4;

    if(x < width && y < height && y >= 0 && x >= 0)	
    {
	    float theta = 1;
	    int halfWidth = (width / 2); 
	    int halfHeight = (height / 2);
	    
	    float newX = x - halfWidth; 
	    float newY = y - halfHeight;
	    float value = pow((float)newX,(float)2.0) + pow((float)newY, (float)2.0);
	    float distance = sqrt(value);
	    float r = distance / correctionRadius;
	    if (r != 0.0)
		    theta = atan(r) / r;
		    
	    int sourceX = round(halfWidth + theta*newX);
	    int sourceY = round(halfHeight + theta * newY);
	    
	    int positionSrc = (sourceX + sourceY * width) * 4;

	    output[position] = input[positionSrc];
        output[position+1] = input[positionSrc + 1];
        output[position + 2] = input[positionSrc + 2];
        output[position + 3] = input[positionSrc + 3];
    }
}

__global__ void sharpenMasking(uchar* output, uchar * input, uchar * gaussian, int width, int height, float sharpness)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    int position = (x + y * width) * 4;
    if (x < width && y < height && y >= 0 && x >= 0)
    {

        float4 origin = GetColorSrc(x, y, input, width, height);
        float4 color = GetColorSrc(x, y, gaussian, width, height);

        color = origin - color;

        float4 value = origin + color * sharpness;


        value = clamp(value, 255.0f, 0.0f);

        rgbaFloat4ToUchar4(output, position, value, 1.0f);
    }
}


// The wrapper is used to call sharpening filter 
void distorsionFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, float correctionRadius)
{
    uchar * d_input;
    uchar * d_output;

    d_input = (uchar * )input.ptr();
    d_output = (uchar*)output.ptr();

    // Specify block size
    const dim3 block(BLOCK_SIZE,BLOCK_SIZE);

    // Calculate grid size to cover the whole image
    const dim3 grid((output.cols + block.x - 1)/block.x, (output.rows + block.y - 1)/block.y);

    // Run BoxFilter kernel on CUDA 
    distorsionFilter<<<grid,block>>>(d_input, d_output, output.cols, output.rows, correctionRadius);
}



// The wrapper is used to call sharpening filter 
void posterisationFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, int threshold)
{
    uchar* d_input;
    uchar* d_output;

    d_input = (uchar*)input.ptr();
    d_output = (uchar*)output.ptr();

    // Specify block size
    const dim3 block(BLOCK_SIZE, BLOCK_SIZE);

    // Calculate grid size to cover the whole image
    const dim3 grid((output.cols + block.x - 1) / block.x, (output.rows + block.y - 1) / block.y);

    // Run BoxFilter kernel on CUDA 
    posterisationFilter<<<grid,block>>>(d_input, d_output, output.cols, output.rows, threshold);
}



// The wrapper is used to call sharpening filter 
void solarizationFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, int level)
{
    uchar* d_input;
    uchar* d_output;

    d_input = (uchar*)input.ptr();
    d_output = (uchar*)output.ptr();

    // Specify block size
    const dim3 block(BLOCK_SIZE, BLOCK_SIZE);

    // Calculate grid size to cover the whole image
    const dim3 grid((output.cols + block.x - 1) / block.x, (output.rows + block.y - 1) / block.y);

    // Run BoxFilter kernel on CUDA 
    solarizationFilter<<<grid,block>>>(d_input, d_output, output.cols, output.rows, level);
}


// The wrapper is used to call sharpening filter 
void mosaicFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, int fTileSize)
{
    uchar* d_input;
    uchar* d_output;

    d_input = (uchar*)input.ptr();
    d_output = (uchar*)output.ptr();

    // Specify block size
    const dim3 block(BLOCK_SIZE, BLOCK_SIZE);

    // Calculate grid size to cover the whole image
    const dim3 grid((output.cols + block.x - 1) / block.x, (output.rows + block.y - 1) / block.y);

    // Run BoxFilter kernel on CUDA 
    mosaicFilter<<<grid,block>>>(d_input, d_output, output.cols, output.rows, fTileSize);

}

// The wrapper is used to call sharpening filter 
void sharpenMasking(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, const cv::cuda::GpuMat& gaussian, float sharpness)
{
    uchar* d_input;
    uchar* d_output;
    uchar* gaussian_input;

    d_input = (uchar*)input.ptr();
    d_output = (uchar*)output.ptr();
    gaussian_input = (uchar*)gaussian.ptr();

    // Specify block size
    const dim3 block(BLOCK_SIZE, BLOCK_SIZE);

    // Calculate grid size to cover the whole image
    const dim3 grid((output.cols + block.x - 1) / block.x, (output.rows + block.y - 1) / block.y);

    // Run BoxFilter kernel on CUDA 
    sharpenMasking << <grid, block >> > (d_input, d_output, gaussian_input, output.cols, output.rows, sharpness);

}










