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
inline  __host__ __device__ void rgbaFloat4ToUchar4(unsigned char* & output, int position, float4 rgba, float fScale)
{
    output[position] = (uchar)(rgba.x * fScale);
    output[position+1] = (uchar)(rgba.y * fScale);
    output[position+2] = (uchar)(rgba.z * fScale);
    output[position+3] = (uchar)(rgba.w * fScale);

}

inline  __host__ __device__ float4 yuvToRgb32(float yComp, float uComp, float vComp, int colorRange, int colorSpace)
{
    float4 color = make_float4(0.0);
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

    return color;
}

//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
__global__ void convertYUVtoRGB32(unsigned char * output, const uchar *inputY, const uchar *inputU, const uchar *inputV, int widthIn, int heightIn, int widthOut, int heightOut, int pitch, int colorRange, int colorSpace)
{ 
   int x = blockIdx.x*blockDim.x + threadIdx.x;
   int y = blockIdx.y*blockDim.y + threadIdx.y;
   int position = (x + y * widthOut) * 4;
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
		color = yuvToRgb32(yComp, uComp, vComp, colorRange, colorSpace);

        rgbaFloat4ToUchar4(output, position, color, 1.0f);
	}
} 


//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
__global__ void convertNV12toRGB32(unsigned char * output,  unsigned char * inputY, unsigned char * inputUV, int widthIn, int heightIn, int widthOut, int heightOut, int pitch, int colorRange, int colorSpace)
{ 
   int x = blockIdx.x*blockDim.x + threadIdx.x;
   int y = blockIdx.y*blockDim.y + threadIdx.y;
   int position = (x + y * widthOut) * 4;
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
        
		color = yuvToRgb32(yComp, uComp, vComp, colorRange, colorSpace);
        
        rgbaFloat4ToUchar4(output, position, color, 1.0f);

	}
} 

// The wrapper is used to call sharpening filter 
void convertNV12toRGB32(cv::cuda::GpuMat& output, uint8_t* bufferY, int sizeY, uint8_t* bufferUV, int sizeUV, const int& width,
    const int& height, const int& lineSize, const int& widthOut, const int& heightOut,
    const int& colorRange, const int& colorSpace)
{
        unsigned char *d_inputY, * d_inputUV;
        unsigned char * d_output = output.ptr();
        
        // Allocate device memory
        cudaMalloc<unsigned char>(&d_inputY, sizeY);
        cudaMalloc<unsigned char>(&d_inputUV, sizeUV);
        //cudaMalloc<unsigned char>(&d_output,outputSize);

        // Copy data from OpenCV input image to device memory
        cudaMemcpy(d_inputY, bufferY, sizeY,cudaMemcpyHostToDevice);
        cudaMemcpy(d_inputUV, bufferUV, sizeUV,cudaMemcpyHostToDevice);

        // Specify block size
        const dim3 block(BLOCK_SIZE,BLOCK_SIZE);

        // Calculate grid size to cover the whole image
        const dim3 grid((output.cols + block.x - 1)/block.x, (output.rows + block.y - 1)/block.y);

        // Run BoxFilter kernel on CUDA 
        convertNV12toRGB32<<<grid,block>>>(d_output,d_inputY,d_inputUV, width, height, widthOut, heightOut, lineSize, colorRange, colorSpace);

        //Free the device memory
        cudaFree(d_inputY);
        cudaFree(d_inputUV);
}


// The wrapper is used to call sharpening filter 
void convertYUVtoRGB32(cv::cuda::GpuMat& output, uint8_t* bufferY, int sizeY, uint8_t* bufferU, int sizeU, uint8_t* bufferV, int sizeV,
    const int& width, const int& height, const int& lineSize, const int& widthOut,
    const int& heightOut, const int& colorRange, const int& colorSpace)
{
        unsigned char *d_inputY, * d_inputU, * d_inputV;
        unsigned char* d_output = output.ptr();
        
        // Allocate device memory
        cudaMalloc<unsigned char>(&d_inputY, sizeY);
        cudaMalloc<unsigned char>(&d_inputU, sizeU);
        cudaMalloc<unsigned char>(&d_inputV, sizeV);
       // cudaMalloc<unsigned char>(&d_output,outputSize);

        // Copy data from OpenCV input image to device memory
        cudaMemcpy(d_inputY, bufferY, sizeY,cudaMemcpyHostToDevice);
        cudaMemcpy(d_inputU, bufferU, sizeU,cudaMemcpyHostToDevice);
        cudaMemcpy(d_inputV, bufferV, sizeV,cudaMemcpyHostToDevice);
        // Specify block size
        const dim3 block(BLOCK_SIZE,BLOCK_SIZE);

        // Calculate grid size to cover the whole image
        const dim3 grid((output.cols + block.x - 1)/block.x, (output.rows + block.y - 1)/block.y);

        // Run BoxFilter kernel on CUDA 
        convertYUVtoRGB32<<<grid,block>>>(d_output,d_inputY,d_inputU,d_inputV, width, height, widthOut, heightOut, lineSize, colorRange, colorSpace);

        //Free the device memory
        cudaFree(d_inputY);
        cudaFree(d_inputU);
        cudaFree(d_inputV);
}