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


inline  __host__ __device__  float4 GetColorSrc(int x, int y, const unsigned char* input, int width, int height)
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
__global__ void distorsionFilter(uchar* input, uchar* output,  int width, int height, float correctionRadius)
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

//---------------------------------------------------------------------
//Application du filtre Soften
//	kernel = {  1, 1, 1, 1, 8, 1, 1, 1, 1 };
//	factor = 16;
//---------------------------------------------------------------------
__global__ void softenFilter(uchar* input, uchar * output,  int width, int height)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    int position = (x + y * width) * 4;
    if (x < width && y < height && y >= 0 && x >= 0)
    {
        float4 sum = GetColorSrc(x - 1, y - 1, input, width, height);
        sum += GetColorSrc(x, y - 1, input, width, height);
        sum += GetColorSrc(x + 1, y - 1, input, width, height);
        sum += GetColorSrc(x - 1, y, input, width, height);
        sum += GetColorSrc(x, y, input, width, height) * make_float4(8.0f);
        sum += GetColorSrc(x + 1, y, input, width, height);
        sum += GetColorSrc(x - 1, y + 1, input, width, height);
        sum += GetColorSrc(x, y + 1, input, width, height);
        sum += GetColorSrc(x + 1, y + 1, input, width, height);
        sum = sum / make_float4(16.0f);
        rgbaFloat4ToUchar4(output, position, sum, 1.0f);
    }
}

__global__ void sepiaFilter(uchar* input, uchar* output,  int width, int height)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    int position = (x + y * width) * 4;
    if (x < width && y < height && y >= 0 && x >= 0)
    {
        float4 color = GetColorSrc(x, y, input, width, height);
        float4 color_out = make_float4(0.0f);
        color_out.x = 0.272 * color.x +0.534 * color.y + 0.131 * color.z;
        color_out.y = 0.349 * color.x + 0.686 * color.y + 0.168 * color.z; //0.349, 0.686, 0.168 
        color_out.z = 0.393 * color.x + 0.769 * color.y + 0.189 * color.z; //0.393, 0.769, 0.189
        color_out.w = color.w;
        rgbaFloat4ToUchar4(output, position, color_out, 1.0f);
    }
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

__global__ void sharpenMasking(uchar* input, uchar* output, uchar * gaussian, int width, int height, float sharpness)
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

__global__ void noiseFilter(uchar* input, uchar * output, int width, int height)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    int position = (x + y * width) * 4;
    if (x < width && y < height && y >= 0 && x >= 0)
    {
        float4 n = make_float4(Noise2d(x, y));
        float4 src_color = GetColorSrc(x, y, input, width, height) + n * 255.0f;
        float4 minimal = make_float4(0.0f);
        float4 maximal = make_float4(255.0f);
        src_color = clamp(src_color, minimal, maximal);
        rgbaFloat4ToUchar4(output, position, src_color, 1.0f);
    }
}


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

inline  __host__ __device__ float4 PostFX(uchar * input, int x, int y, float radius, float angleDegree, int widthIn, int heightIn)
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

    return GetColorSrc((int)tcX, (int)tcY, input, widthIn, heightIn);
}

__global__ void swirlFilter(uchar* input, uchar * output,  int width, int height, float radius, float angleDegree)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    int position = (x + y * width) * 4;
    if (x < width && y < height && y >= 0 && x >= 0)
    {
        float4 color = PostFX(input, x, y, radius, angleDegree, width, height);
        rgbaFloat4ToUchar4(output, position, color, 1.0f);
    }
}

__global__ void erodeFilter(uchar* input, uchar* output, int width, int height)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    int position = (x + y * width) * 4;
    if (x < width && y < height && y >= 0 && x >= 0)
    {
        float4 maxValue = make_float4(0.0f);

        for (int n = -1; n < 2; n++)
        {
            int posY = y + n;
            for (int m = -1; m < 2; m++)
            {
                int posX = x + m;
                maxValue = fmaxf(GetColorSrc(posX, posY, input, width, height), maxValue);
            }
        }

        rgbaFloat4ToUchar4(output, position, maxValue, 1.0f);
    }
}

__global__ void dilateFilter(uchar* input, uchar* output, int width, int height)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    int position = (x + y * width) * 4;
    if (x < width && y < height && y >= 0 && x >= 0)
    {
        float4 minValue = make_float4(1.0f);

        for (int n = -1; n < 2; n++)
        {
            int posY = y + n;
            for (int m = -1; m < 2; m++)
            {
                int posX = x + m;
                minValue = fminf(GetColorSrc(posX, posY, input, width, height), minValue);
            }
        }

        rgbaFloat4ToUchar4(output, position, minValue, 1.0f);
    }
}


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

__global__ void medianFilter(uchar* input, uchar* output, int width, int height)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    int position = (x + y * width) * 4;
    if (x < width && y < height && y >= 0 && x >= 0)
    {
        float4 k0 = GetColorSrc(x - 1, y - 1, input, width, height);
        float4 k1 = GetColorSrc(x, y - 1, input, width, height);
        float4 k2 = GetColorSrc(x + 1, y - 1, input, width, height);
        float4 k3 = GetColorSrc(x - 1, y, input, width, height);
        float4 k4 = GetColorSrc(x, y, input, width, height);
        float4 k5 = GetColorSrc(x + 1, y, input, width, height);
        float4 k6 = GetColorSrc(x - 1, y + 1, input, width, height);
        float4 k7 = GetColorSrc(x, y + 1, input, width, height);
        float4 k8 = GetColorSrc(x + 1, y + 1, input, width, height);

        mnmx6(&k0, &k1, &k2, &k3, &k4, &k5);
        mnmx5(&k1, &k2, &k3, &k4, &k6);
        mnmx4(&k2, &k3, &k4, &k7);
        mnmx3(&k3, &k4, &k8);

        rgbaFloat4ToUchar4(output, position, k4, 1.0f);
    }

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

// The wrapper is used to call sharpening filter 
void medianFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output)
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
    medianFilter << <grid, block >> > (d_input, d_output, output.cols, output.rows);
}


// The wrapper is used to call sharpening filter 
void dilateFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output)
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
    dilateFilter << <grid, block >> > (d_input, d_output, output.cols, output.rows);
}

// The wrapper is used to call sharpening filter 
void erodeFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output)
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
    erodeFilter << <grid, block >> > (d_input, d_output, output.cols, output.rows);
}

// The wrapper is used to call sharpening filter 
void noiseFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output)
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
    noiseFilter << <grid, block >> > (d_input, d_output, output.cols, output.rows);
}


// The wrapper is used to call sharpening filter 
void swirlFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, float radius, float angleDegree)
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
    swirlFilter << <grid, block >> > (d_input, d_output, output.cols, output.rows, radius, angleDegree);
}


// The wrapper is used to call sharpening filter 
void sepiaFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output)
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
    sepiaFilter << <grid, block >> > (d_input, d_output, output.cols, output.rows);
}

// The wrapper is used to call sharpening filter 
void softenFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output)
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
    softenFilter << <grid, block >> > (d_input, d_output, output.cols, output.rows);
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










