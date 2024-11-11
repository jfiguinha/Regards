//
// CUDA implementation of Image Sharpening Filter
//
#include "videoFilter.h"
#include <cuda.h>
#include "cuda_runtime.h"
#include "helper_math.h"
#define BLOCK_SIZE      16
#define FILTER_WIDTH    3       
#define FILTER_HEIGHT   3       

using namespace std;

//----------------------------------------------------
// Conversion du NV12 vers du 32 bits
//----------------------------------------------------
inline  __host__ __device__ uint rgbaFloat4ToUint(float4 rgba, float fScale)
{
    unsigned int uiPackedPix = 0U;
    uiPackedPix |= 0x000000FF & (unsigned int)(rgba.x * fScale);
    uiPackedPix |= 0x0000FF00 & (((unsigned int)(rgba.y * fScale)) << 8);
    uiPackedPix |= 0x00FF0000 & (((unsigned int)(rgba.z * fScale)) << 16);
    uiPackedPix |= 0xFF000000 & (((unsigned int)(rgba.w * fScale)) << 24);
    return uiPackedPix;
}



//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
__global__ void convertYUVtoRGB32(uint *output, const uchar *inputY, const uchar *inputU, const uchar *inputV, int widthIn, int heightIn, int widthOut, int heightOut, int pitch, int colorRange, int colorSpace) 
{ 
   int x = blockIdx.x*blockDim.x + threadIdx.x;
   int y = blockIdx.y*blockDim.y + threadIdx.y;
   int position = x + y * widthOut;
	if(x < widthOut && y < heightOut && y >= 0 && x >= 0)	
	{
		float4 color;
		int positionSrc = x + y * pitch;
		int positionUV = 0;
		if (x & 1)
		{		
			if (y & 1)
				positionUV = ((x - 1) / 2) + ((y - 1) / 2) * (pitch / 2);
			else
				positionUV = ((x - 1) / 2) + (y / 2) * (pitch / 2);
		}
		else
		{
			if (y & 1)
				positionUV = (x / 2) + ((y - 1) / 2) * (pitch / 2);
			else
				positionUV = (x / 2) + (y / 2) * (pitch / 2);
		}
		float uComp = inputU[positionUV];
		float vComp = inputV[positionUV];
		float yComp = inputY[positionSrc];
		    // RGB conversion
		
		float3 matrix[3];
		if(colorSpace == 0)
		{
			//default
			matrix[0].x = 1.164;
			matrix[0].y = 0;
			matrix[0].z = 1.596;
			
			matrix[1].x = 1.164;
			matrix[1].y = -0.391;
			matrix[1].z = -0.813;

			matrix[2].x = 1.164;
			matrix[2].y = 2.018;
			matrix[2].z = 0;			
		}
		else if(colorSpace == 1)
		{
			//bt601
			matrix[0].x = 1;
			matrix[0].y = 0;
			matrix[0].z = 1;
			
			matrix[1].x = 1;
			matrix[1].y = -0.344;
			matrix[1].z = -0.714;

			matrix[2].x = 1;
			matrix[2].y = 1.772;
			matrix[2].z = 0;
		}
		else if(colorSpace == 2)
		{
			//BT.709
			matrix[0].x = 1;
			matrix[0].y = 0;
			matrix[0].z = 1.5748;
			
			matrix[1].x = 1;
			matrix[1].y = -0.187324;
			matrix[1].z = -0.468124;

			matrix[2].x = 1;
			matrix[2].y = 1.8556;
			matrix[2].z = 0;
		}		
		else if(colorSpace == 3)
		{
			//BT.2020
			matrix[0].x = 1;
			matrix[0].y = 0;
			matrix[0].z = 1.402;
			
			matrix[1].x = 1;
			matrix[1].y = -0.344136286;
			matrix[1].z = -0.7141362862;

			matrix[2].x = 1;
			matrix[2].y = 1.772;
			matrix[2].z = 0;
		}	
		
		color.z = (matrix[0].x * (yComp - 16) + matrix[0].y * (uComp-128) + matrix[0].z * (vComp-128));
		color.y = (matrix[1].x * (yComp - 16) + matrix[1].y * (uComp-128) + matrix[1].z * (vComp-128));
		color.x = (matrix[2].x * (yComp - 16) + matrix[2].y * (uComp-128) + matrix[2].z * (vComp-128));
		color.w = 255.0f;

		float4 minimal = make_float4(0.0);
		float4 maximal = make_float4(255.0);
		
		if(colorRange == 1)
		{
			minimal = make_float4(16.0);
			maximal = make_float4(235.0);
		}
        
		

		color = clamp(color,minimal,maximal);
		output[position] = rgbaFloat4ToUint(color,1.0f);
	}
} 


//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
__global__ void convertNV12toRGB32(uint * output,  unsigned char * inputY, unsigned char * inputUV, int widthIn, int heightIn, int widthOut, int heightOut, int pitch, int colorRange, int colorSpace) 
{ 
   int x = blockIdx.x*blockDim.x + threadIdx.x;
   int y = blockIdx.y*blockDim.y + threadIdx.y;
   int position = x + y * widthOut;
	if(x < widthOut && y < heightOut && y >= 0 && x >= 0)	
	{
		//float4 color = getColorFromNV12(inputY, inputUV, x,  y, widthIn, heightIn, pitch, colorRange, colorSpace); 
        
		float4 color;
		int positionSrc = x + y * pitch;
		int positionUV = 0;

		int yModulo = y % 2;
		int xModulo = x % 2;
		if (xModulo == 1)
		{		
			if (yModulo == 1)
				positionUV = (x - 1) + ((y - 1) / 2) * pitch;
			else
				positionUV = (x - 1) + (y / 2) * pitch;
		}
		else
		{
			if (yModulo == 1)
				positionUV = x + ((y - 1) / 2) * pitch;
			else
				positionUV = x + (y / 2) * pitch;
		}
		
		uchar vComp = inputUV[positionUV];
		uchar uComp = inputUV[positionUV + 1];
		uchar yComp = inputY[positionSrc];
		
		float3 matrix[3];
		if(colorSpace == 0)
		{
			//default
			matrix[0].x = 1.164;
			matrix[0].y = 0;
			matrix[0].z = 1.596;
			
			matrix[1].x = 1.164;
			matrix[1].y = -0.391;
			matrix[1].z = -0.813;

			matrix[2].x = 1.164;
			matrix[2].y = 2.018;
			matrix[2].z = 0;			
		}
		else if(colorSpace == 1)
		{
			//bt601
			matrix[0].x = 1;
			matrix[0].y = 0;
			matrix[0].z = 1;
			
			matrix[1].x = 1;
			matrix[1].y = -0.344;
			matrix[1].z = -0.714;

			matrix[2].x = 1;
			matrix[2].y = 1.772;
			matrix[2].z = 0;
		}
		else if(colorSpace == 2)
		{
			//BT.709
			matrix[0].x = 1;
			matrix[0].y = 0;
			matrix[0].z = 1.5748;
			
			matrix[1].x = 1;
			matrix[1].y = -0.187324;
			matrix[1].z = -0.468124;

			matrix[2].x = 1;
			matrix[2].y = 1.8556;
			matrix[2].z = 0;
		}		
		else if(colorSpace == 3)
		{
			//BT.2020
			matrix[0].x = 1;
			matrix[0].y = 0;
			matrix[0].z = 1.402;

			matrix[1].x = 1;
			matrix[1].y = -0.344136286;
			matrix[1].z = -0.7141362862;

			matrix[2].x = 1;
			matrix[2].y = 1.772;
			matrix[2].z = 0;
		}	
		
		color.x = (matrix[0].x * (yComp - 16) + matrix[0].y * (uComp-128) + matrix[0].z * (vComp-128));
		color.y = (matrix[1].x * (yComp - 16) + matrix[1].y * (uComp-128) + matrix[1].z * (vComp-128));
		color.z = (matrix[2].x * (yComp - 16) + matrix[2].y * (uComp-128) + matrix[2].z * (vComp-128));
		color.w = 255.0f;

		float4 minimal = make_float4(0.0);
		float4 maximal = make_float4(255.0);
		
		if(colorRange == 1)
		{
			minimal = make_float4(16.0);
			maximal = make_float4(235.0);
		}
        
        color = clamp(color,minimal,maximal);
        
		output[position] = rgbaFloat4ToUint(color,1.0f);
	}
} 

// The wrapper is used to call sharpening filter 
void convertNV12toRGB32(cv::cuda::GpuMat& output, const cv::Mat& inputY, const cv::Mat& inputUV, int pitch, int colorRange, int colorSpace)
{
        // Use cuda event to catch time
        cudaEvent_t start, stop;
        cudaEventCreate(&start);
        cudaEventCreate(&stop);

        // Calculate number of image channels
        int channel = 4;//inputY.step/inputY.cols; 

        // Calculate number of input & output bytes in each block
        const int inputYSize = inputY.cols * inputY.rows;
        const int inputUVSize = inputUV.cols * inputUV.rows;
        const int outputSize = output.cols * output.rows * channel;
        unsigned char *d_inputY, * d_inputUV;
        uint * d_output;
        
        // Allocate device memory
        cudaMalloc<unsigned char>(&d_inputY,inputYSize);
        cudaMalloc<unsigned char>(&d_inputUV,inputUVSize);
        cudaMalloc<uint>(&d_output,outputSize);

        // Copy data from OpenCV input image to device memory
        cudaMemcpy(d_inputY,inputY.ptr(),inputYSize,cudaMemcpyHostToDevice);
        cudaMemcpy(d_inputUV,inputUV.ptr(),inputUVSize,cudaMemcpyHostToDevice);

        // Specify block size
        const dim3 block(BLOCK_SIZE,BLOCK_SIZE);

        // Calculate grid size to cover the whole image
        const dim3 grid((output.cols + block.x - 1)/block.x, (output.rows + block.y - 1)/block.y);

        // Start time
        cudaEventRecord(start);

        // Run BoxFilter kernel on CUDA 
        convertNV12toRGB32<<<grid,block>>>(d_output,d_inputY,d_inputUV, inputY.cols, inputY.rows, output.cols, output.rows, pitch, colorRange, colorSpace);

        // Stop time
        cudaEventRecord(stop);

        //Copy data from device memory to output image
        cudaMemcpy(output.ptr(),d_output,outputSize,cudaMemcpyDeviceToDevice);

        //Free the device memory
        cudaFree(d_inputY);
        cudaFree(d_inputUV);
        cudaFree(d_output);

        cudaEventSynchronize(stop);
        float milliseconds = 0;
        
        // Calculate elapsed time in milisecond  
        cudaEventElapsedTime(&milliseconds, start, stop);
        cout<< "\nProcessing time on GPU (ms): " << milliseconds << "\n";
}


// The wrapper is used to call sharpening filter 
void convertYUVtoRGB32(cv::cuda::GpuMat& output, const cv::Mat& inputY, const cv::Mat& inputU, const cv::Mat& inputV, int pitch, int colorRange, int colorSpace)
{
        // Use cuda event to catch time
        cudaEvent_t start, stop;
        cudaEventCreate(&start);
        cudaEventCreate(&stop);

        // Calculate number of image channels
        int channel = 4;//inputY.step/inputY.cols; 

        // Calculate number of input & output bytes in each block
        const int inputYSize = inputY.cols * inputY.rows;
        const int inputUSize = inputU.cols * inputU.rows;
        const int inputVSize = inputV.cols * inputV.rows;
        const int outputSize = output.cols * output.rows * channel;
        unsigned char *d_inputY, * d_inputU, * d_inputV;
        uint * d_output;
        
        // Allocate device memory
        cudaMalloc<unsigned char>(&d_inputY,inputYSize);
        cudaMalloc<unsigned char>(&d_inputU,inputUSize);
        cudaMalloc<unsigned char>(&d_inputV,inputVSize);
        cudaMalloc<uint>(&d_output,outputSize);

        // Copy data from OpenCV input image to device memory
        cudaMemcpy(d_inputY,inputY.ptr(),inputYSize,cudaMemcpyHostToDevice);
        cudaMemcpy(d_inputU,inputU.ptr(),inputUSize,cudaMemcpyHostToDevice);
        cudaMemcpy(d_inputV,inputV.ptr(),inputVSize,cudaMemcpyHostToDevice);
        // Specify block size
        const dim3 block(BLOCK_SIZE,BLOCK_SIZE);

        // Calculate grid size to cover the whole image
        const dim3 grid((output.cols + block.x - 1)/block.x, (output.rows + block.y - 1)/block.y);

        // Start time
        cudaEventRecord(start);

        // Run BoxFilter kernel on CUDA 
        convertYUVtoRGB32<<<grid,block>>>(d_output,d_inputY,d_inputU,d_inputV, inputY.cols, inputY.rows, output.cols, output.rows, pitch, colorRange, colorSpace);

        // Stop time
        cudaEventRecord(stop);

        //Copy data from device memory to output image
        cudaMemcpy(output.ptr(),d_output,outputSize,cudaMemcpyDeviceToDevice);

        //Free the device memory
        cudaFree(d_inputY);
        cudaFree(d_inputU);
        cudaFree(d_inputV);
        cudaFree(d_output);

        cudaEventSynchronize(stop);
        float milliseconds = 0;
        
        // Calculate elapsed time in milisecond  
        cudaEventElapsedTime(&milliseconds, start, stop);
        cout<< "\nProcessing time on GPU (ms): " << milliseconds << "\n";
}