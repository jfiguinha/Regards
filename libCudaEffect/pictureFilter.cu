//
// CUDA implementation of Image Sharpening Filter
//
#include "pictureFilter.h"
#include <cuda.h>
#include "cuda_runtime.h"

#define BLOCK_SIZE      16
#define FILTER_WIDTH    3       
#define FILTER_HEIGHT   3       

using namespace std;

// Inline device function to convert 32-bit unsigned integer to floating point rgba color 
//*****************************************************************
inline  __host__ __device__ float4 rgbaUintToFloat4(uint c)
{
    float4 rgba;
    rgba.x = c & 0xff;
    rgba.y = (c >> 8) & 0xff;
    rgba.z = (c >> 16) & 0xff;
    rgba.w = (c >> 24) & 0xff;
    return rgba;
}

inline  __host__ __device__ uchar4 rgbaUintToUChar4(uint c)
{
    uchar4 rgba;
    rgba.x = c & 0xff;
    rgba.y = (c >> 8) & 0xff;
    rgba.z = (c >> 16) & 0xff;
    rgba.w = (c >> 24) & 0xff;
    return rgba;
}

// Inline device function to convert floating point rgba color to 32-bit unsigned integer
//*****************************************************************
inline  __host__ __device__ uint rgbaFloat4ToUint(float4 rgba, float fScale)
{
    unsigned int uiPackedPix = 0U;
    uiPackedPix |= 0x000000FF & (unsigned int)(rgba.x * fScale);
    uiPackedPix |= 0x0000FF00 & (((unsigned int)(rgba.y * fScale)) << 8);
    uiPackedPix |= 0x00FF0000 & (((unsigned int)(rgba.z * fScale)) << 16);
    uiPackedPix |= 0xFF000000 & (((unsigned int)(rgba.w * fScale)) << 24);
    return uiPackedPix;
}


// Inline device function to convert floating point rgba color to 32-bit unsigned integer
//*****************************************************************
inline  __host__ __device__  uint rgbaUChar4ToUint(uchar4 rgba)
{
    unsigned int uiPackedPix = 0U;
    uiPackedPix |= 0x000000FF & (unsigned int)(rgba.x);
    uiPackedPix |= 0x0000FF00 & (((unsigned int)(rgba.y)) << 8);
    uiPackedPix |= 0x00FF0000 & (((unsigned int)(rgba.z)) << 16);
    uiPackedPix |= 0xFF000000 & (((unsigned int)(rgba.w)) << 24);
    return uiPackedPix;
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


// Run Sharpening Filter on GPU
__global__ void mosaicFilter(uint *srcImage, uint *dstImage, unsigned int width, unsigned int height, int fTileSize)
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
	    
	    int positionSrc = sourcex + sourcey * width;
	    int positionDest = x + y * width;

	    dstImage[positionDest] = srcImage[positionSrc];
    }
}


// Run Sharpening Filter on GPU
__global__ void solarizationFilter(uint *srcImage, uint *dstImage, unsigned int width, unsigned int height, int threshold)
{
   int x = blockIdx.x*blockDim.x + threadIdx.x;
   int y = blockIdx.y*blockDim.y + threadIdx.y;
   
   int position = x + y * width;

	if(x < width && y < height && y >= 0 && x >= 0)	
    {
	    float4 colorInput = rgbaUintToFloat4(srcImage[position]);
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

	    dstImage[position] = rgbaFloat4ToUint(colorOutput, 1.0f);
    }
}


// Run Sharpening Filter on GPU
__global__ void posterisationFilter(uint * srcImage, uint * dstImage, unsigned int width, unsigned int height, int level)
{
   int x = blockIdx.x*blockDim.x + threadIdx.x;
   int y = blockIdx.y*blockDim.y + threadIdx.y;
   int position = x + y * width;

	if(x < width && y < height && y >= 0 && x >= 0)	
    {
	    uchar4 colorInput = rgbaUintToUChar4(srcImage[position]);
	    uchar4 colorOutput = colorInput;
	    int _levels = max(2, min(16, level));
	    float _offset = (float)256 / (float)_levels;
	    
	    int red = colorInput.x / _offset;
	    int green = colorInput.y / _offset;
	    int blue = colorInput.z / _offset;
	    
	    colorOutput.x = (red * _offset);
	    colorOutput.y = (green * _offset);
	    colorOutput.z = (blue * _offset);

        dstImage[position] = rgbaUChar4ToUint(colorOutput);
    }
}

//----------------------------------------------------
//Filtre Posterization
//----------------------------------------------------
__global__ void distorsionFilter(uint *output, uint * input, int width, int height, float correctionRadius)
{
   int x = blockIdx.x*blockDim.x + threadIdx.x;
   int y = blockIdx.y*blockDim.y + threadIdx.y;
   int position = x + y * width;

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
	    
	    int positionSrc = sourceX + sourceY * width;
	    int position = x + y * width;

	    output[position] = input[positionSrc];
    }
}


// The wrapper is used to call sharpening filter 
void distorsionFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, float correctionRadius)
{
        // Use cuda event to catch time
        cudaEvent_t start, stop;
        cudaEventCreate(&start);
        cudaEventCreate(&stop);

        // Calculate number of image channels
        int channel = 4;//input.step/input.cols; 

        // Calculate number of input & output bytes in each block
        const int inputSize = input.cols * input.rows * channel;
        const int outputSize = output.cols * output.rows * channel;
        uint *d_input, *d_output;
        
        // Allocate device memory
        cudaMalloc<uint>(&d_input,inputSize);
        cudaMalloc<uint>(&d_output,outputSize);

        // Copy data from OpenCV input image to device memory
        cudaMemcpy(d_input,input.ptr(),inputSize,cudaMemcpyDeviceToDevice);

        // Specify block size
        const dim3 block(BLOCK_SIZE,BLOCK_SIZE);

        // Calculate grid size to cover the whole image
        const dim3 grid((output.cols + block.x - 1)/block.x, (output.rows + block.y - 1)/block.y);

        // Start time
        cudaEventRecord(start);

        // Run BoxFilter kernel on CUDA 
        distorsionFilter<<<grid,block>>>(d_input, d_output, output.cols, output.rows, correctionRadius);

        // Stop time
        cudaEventRecord(stop);

        //Copy data from device memory to output image
        cudaMemcpy(output.ptr(),d_output,outputSize,cudaMemcpyDeviceToDevice);

        //Free the device memory
        cudaFree(d_input);
        cudaFree(d_output);

        cudaEventSynchronize(stop);
        float milliseconds = 0;
        
        // Calculate elapsed time in milisecond  
        cudaEventElapsedTime(&milliseconds, start, stop);
        cout<< "\nProcessing time on GPU (ms): " << milliseconds << "\n";
}



// The wrapper is used to call sharpening filter 
void posterisationFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, int threshold)
{
        // Use cuda event to catch time
        cudaEvent_t start, stop;
        cudaEventCreate(&start);
        cudaEventCreate(&stop);

        // Calculate number of image channels
        int channel = 4;//input.step/input.cols; 

        // Calculate number of input & output bytes in each block
        const int inputSize = input.cols * input.rows * channel;
        const int outputSize = output.cols * output.rows * channel;
        uint *d_input, *d_output;
        
        // Allocate device memory
        cudaMalloc<uint>(&d_input,inputSize);
        cudaMalloc<uint>(&d_output,outputSize);

        // Copy data from OpenCV input image to device memory
        cudaMemcpy(d_input,input.ptr(),inputSize,cudaMemcpyDeviceToDevice);

        // Specify block size
        const dim3 block(BLOCK_SIZE,BLOCK_SIZE);

        // Calculate grid size to cover the whole image
        const dim3 grid((output.cols + block.x - 1)/block.x, (output.rows + block.y - 1)/block.y);

        // Start time
        cudaEventRecord(start);

        // Run BoxFilter kernel on CUDA 
        posterisationFilter<<<grid,block>>>(d_input, d_output, output.cols, output.rows, threshold);

        // Stop time
        cudaEventRecord(stop);

        //Copy data from device memory to output image
        cudaMemcpy(output.ptr(),d_output,outputSize,cudaMemcpyDeviceToDevice);

        //Free the device memory
        cudaFree(d_input);
        cudaFree(d_output);

        cudaEventSynchronize(stop);
        float milliseconds = 0;
        
        // Calculate elapsed time in milisecond  
        cudaEventElapsedTime(&milliseconds, start, stop);
        cout<< "\nProcessing time on GPU (ms): " << milliseconds << "\n";
}



// The wrapper is used to call sharpening filter 
void solarizationFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, int level)
{
        // Use cuda event to catch time
        cudaEvent_t start, stop;
        cudaEventCreate(&start);
        cudaEventCreate(&stop);

        // Calculate number of image channels
        int channel = 4;//input.step/input.cols; 

        // Calculate number of input & output bytes in each block
        const int inputSize = input.cols * input.rows * channel;
        const int outputSize = output.cols * output.rows * channel;
        uint *d_input, *d_output;
        
        // Allocate device memory
        cudaMalloc<uint>(&d_input,inputSize);
        cudaMalloc<uint>(&d_output,outputSize);

        // Copy data from OpenCV input image to device memory
        cudaMemcpy(d_input,input.ptr(),inputSize,cudaMemcpyDeviceToDevice);

        // Specify block size
        const dim3 block(BLOCK_SIZE,BLOCK_SIZE);

        // Calculate grid size to cover the whole image
        const dim3 grid((output.cols + block.x - 1)/block.x, (output.rows + block.y - 1)/block.y);

        // Start time
        cudaEventRecord(start);

        // Run BoxFilter kernel on CUDA 
        solarizationFilter<<<grid,block>>>(d_input, d_output, output.cols, output.rows, level);

        // Stop time
        cudaEventRecord(stop);

        //Copy data from device memory to output image
        cudaMemcpy(output.ptr(),d_output,outputSize,cudaMemcpyDeviceToDevice);

        //Free the device memory
        cudaFree(d_input);
        cudaFree(d_output);

        cudaEventSynchronize(stop);
        float milliseconds = 0;
        
        // Calculate elapsed time in milisecond  
        cudaEventElapsedTime(&milliseconds, start, stop);
        cout<< "\nProcessing time on GPU (ms): " << milliseconds << "\n";
}


// The wrapper is used to call sharpening filter 
void mosaicFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, int fTileSize)
{
        // Use cuda event to catch time
        cudaEvent_t start, stop;
        cudaEventCreate(&start);
        cudaEventCreate(&stop);

        // Calculate number of image channels
        int channel = 4;//input.step/input.cols; 

        // Calculate number of input & output bytes in each block
        const int inputSize = input.cols * input.rows * channel;
        const int outputSize = output.cols * output.rows * channel;
        uint *d_input, *d_output;
        
        // Allocate device memory
        cudaMalloc<uint>(&d_input,inputSize);
        cudaMalloc<uint>(&d_output,outputSize);

        // Copy data from OpenCV input image to device memory
        cudaMemcpy(d_input,input.ptr(),inputSize,cudaMemcpyDeviceToDevice);

        // Specify block size
        const dim3 block(BLOCK_SIZE,BLOCK_SIZE);

        // Calculate grid size to cover the whole image
        const dim3 grid((output.cols + block.x - 1)/block.x, (output.rows + block.y - 1)/block.y);

        // Start time
        cudaEventRecord(start);

        // Run BoxFilter kernel on CUDA 
        mosaicFilter<<<grid,block>>>(d_input, d_output, output.cols, output.rows, fTileSize);

        // Stop time
        cudaEventRecord(stop);

        //Copy data from device memory to output image
        cudaMemcpy(output.ptr(),d_output,outputSize,cudaMemcpyDeviceToDevice);

        //Free the device memory
        cudaFree(d_input);
        cudaFree(d_output);

        cudaEventSynchronize(stop);
        float milliseconds = 0;
        
        // Calculate elapsed time in milisecond  
        cudaEventElapsedTime(&milliseconds, start, stop);
        cout<< "\nProcessing time on GPU (ms): " << milliseconds << "\n";
}










