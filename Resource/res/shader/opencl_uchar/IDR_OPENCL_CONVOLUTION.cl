// Inline device function to convert 32-bit unsigned integer to floating point rgba color 
//*****************************************************************
inline float4 rgbaUintToFloat4(uint c)
{
    float4 rgba;
    rgba.x = c & 0xff;
    rgba.y = (c >> 8) & 0xff;
    rgba.z = (c >> 16) & 0xff;
    rgba.w = (c >> 24) & 0xff;
    return rgba;
}

inline uchar4 rgbaUintToUChar4(uint c)
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
inline uint rgbaFloat4ToUint(float4 rgba, float fScale)
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
inline  uint rgbaUChar4ToUint(uchar4 rgba)
{
    unsigned int uiPackedPix = 0U;
    uiPackedPix |= 0x000000FF & (unsigned int)(rgba.x);
    uiPackedPix |= 0x0000FF00 & (((unsigned int)(rgba.y)) << 8);
    uiPackedPix |= 0x00FF0000 & (((unsigned int)(rgba.z)) << 16);
    uiPackedPix |= 0xFF000000 & (((unsigned int)(rgba.w)) << 24);
    return uiPackedPix;
}

//---------------------------------------------------------------------
//Recherche le pixel
//---------------------------------------------------------------------
uint GetColorSrc(int x, int y, const __global uint *input, int width, int height)
{
	if(x < width && y < height && y >= 0 && x >= 0)	
	{
		int position = x + y * width;
		return input[position];
	}
	return 0;
}

float4 GetfColorSrc(int x, int y, const __global uint *input, int width, int height)
{
	if(x < width && y < height && y >= 0 && x >= 0)	
	{
		int position = x + y * width;
		return rgbaUintToFloat4(input[position]);
	}
	return (float4)0.0f;
}

//---------------------------------------------------------------------
//Limite les valeurs entre 0 et 1.0f
//---------------------------------------------------------------------
float4 NormalizeValue(float4 sum)
{
	float4 value;
	value.x = max(min(sum.x, 255.0f), 0.0f);
	value.y = max(min(sum.y, 255.0f), 0.0f);
	value.z = max(min(sum.z, 255.0f), 0.0f);  
	value.w = max(min(sum.w, 255.0f), 0.0f); 
	return value;
}



//---------------------------------------------------------------------
//Application du filtre Emboss
//---------------------------------------------------------------------
__kernel void Emboss(__global uint * output, const __global uint *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	
	float4 sum = GetfColorSrc(x + 1, y + 1, input, width, height) - GetfColorSrc(x - 1, y - 1, input, width, height) + (float4)(127.0f);
 
	int position = x + y * width;
	output[position] = rgbaFloat4ToUint(NormalizeValue(sum),1.0f); 
}

//---------------------------------------------------------------------
//Application du filtre Edge
//---------------------------------------------------------------------

float4 GradientX(const __global uint *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	float4 sum = 0;
	sum = GetfColorSrc(x - 1, y - 1, input, width, height);
	sum += GetfColorSrc(x - 1, y, input, width, height) * 2.0f;
	sum += GetfColorSrc(x - 1, y + 1, input, width, height);
	sum -= GetfColorSrc(x + 1, y - 1, input, width, height);
	sum -= GetfColorSrc(x + 1, y, input, width, height) * 2.0f;
	sum -= GetfColorSrc(x + 1, y + 1, input, width, height);
	return fabs(sum);
}

float4 GradientY(const __global uint *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	float4 sum = 0;
	sum = GetfColorSrc(x - 1, y - 1, input, width, height);
	sum += GetfColorSrc(x, y - 1, input, width, height) * 2.0f;
	sum += GetfColorSrc(x + 1, y  - 1, input, width, height);
	sum -= GetfColorSrc(x - 1, y + 1, input, width, height);
	sum -= GetfColorSrc(x, y + 1, input, width, height) * 2.0f;
	sum -= GetfColorSrc(x + 1, y + 1, input, width, height);
	return fabs(sum);
}

__kernel void Edge(__global uint * output, const __global uint *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);

	float4 sum = 0;
	float4 gx = GradientX(input, width, height);
	float4 gy = GradientY(input, width, height);
	sum = gx + gy;

	int position = x + y * width;
	output[position] = rgbaFloat4ToUint(NormalizeValue(sum),1.0f); 
}

//---------------------------------------------------------------------
//Application du filtre Sharpen
//	kernel = { -1, -1, -1, -1, 16,- 1, -1, -1, -1 };
//	factor = 8;
//---------------------------------------------------------------------
__kernel void Sharpen(__global uint * output, const __global uint *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	
	float4 sum = -GetfColorSrc(x - 1, y - 1, input, width, height);
	sum -= GetfColorSrc(x , y - 1, input, width, height);
	sum -= GetfColorSrc(x + 1, y - 1, input, width, height);
	sum -= GetfColorSrc(x - 1, y, input, width, height);
	sum += GetfColorSrc(x , y, input, width, height) * (float4)16.0f;
	sum -= GetfColorSrc(x + 1, y, input, width, height);
	sum -= GetfColorSrc(x - 1, y + 1, input, width, height);
	sum -= GetfColorSrc(x , y + 1, input, width, height);
	sum -= GetfColorSrc(x + 1, y + 1, input, width, height);
	
	sum = sum / (float4)8.0f;

	int position = x + y * width;
	output[position] = rgbaFloat4ToUint(NormalizeValue(sum),1.0f); 
}

//---------------------------------------------------------------------
//Application du filtre SharpenStrong
//	kernel = { -1, -1, -1, -1, 9,- 1, -1, -1, -1 };
//	factor = 1;
//---------------------------------------------------------------------
__kernel void SharpenStrong(__global uint * output, const __global uint *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	
	float4 sum = -GetfColorSrc(x - 1, y - 1, input, width, height);
	sum -= GetfColorSrc(x , y - 1, input, width, height);
	sum -= GetfColorSrc(x + 1, y - 1, input, width, height);
	sum -= GetfColorSrc(x - 1, y, input, width, height);
	sum += GetfColorSrc(x , y, input, width, height) * (float4)9.0f;
	sum -= GetfColorSrc(x + 1, y, input, width, height);
	sum -= GetfColorSrc(x - 1, y + 1, input, width, height);
	sum -= GetfColorSrc(x , y + 1, input, width, height);
	sum -= GetfColorSrc(x + 1, y + 1, input, width, height);

	int position = x + y * width;
	output[position] = rgbaFloat4ToUint(NormalizeValue(sum),1.0f);   
}

//---------------------------------------------------------------------
//Application du filtre Blur
//	kernel = {  1, 1, 1, 1, 1, 1, 1, 1, 1 };
//	factor = 9;
//---------------------------------------------------------------------
__kernel void Blur(__global uint * output, const __global uint *input, int width, int height, int size)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	float4 sum = 0;
	int count = 0;
	int start = -size / 2;
	int end = size / 2;
	
	for(int i = start;i <= size;i++)
	{
		for(int j = end;j <= size;j++)
		{
			count++;
			sum += GetfColorSrc(x + i, y + j, input, width, height);
		}
	}
	sum = sum / (float4)count;
	
	int position = x + y * width;
	output[position] = rgbaFloat4ToUint(NormalizeValue(sum),1.0f);  
}

//---------------------------------------------------------------------
//Application du filtre Soften
//	kernel = {  1, 1, 1, 1, 8, 1, 1, 1, 1 };
//	factor = 16;
//---------------------------------------------------------------------
__kernel void Soften(__global uint * output, const __global uint *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	
	float4 sum = GetfColorSrc(x - 1, y - 1, input, width, height);
	sum += GetfColorSrc(x , y - 1, input, width, height);
	sum += GetfColorSrc(x + 1, y - 1, input, width, height);
	sum += GetfColorSrc(x - 1, y, input, width, height);
	sum += GetfColorSrc(x , y, input, width, height) * (float4)8.0f;
	sum += GetfColorSrc(x + 1, y, input, width, height);
	sum += GetfColorSrc(x - 1, y + 1, input, width, height);
	sum += GetfColorSrc(x , y + 1, input, width, height);
	sum += GetfColorSrc(x + 1, y + 1, input, width, height);
	sum = sum / (float4)16.0f;
	int position = x + y * width;
	output[position] = rgbaFloat4ToUint(NormalizeValue(sum),1.0f);    
}

//---------------------------------------------------------------------
//Application du filtre Gaussian Blur
//	int kernel[9] = { 5, 15, 31, 49, 56, 49, 31, 15, 5 };
//	int size = 3;
//	int factor = 256;
//---------------------------------------------------------------------
__kernel void FastGaussianBlur(__global uint * output, const __global uint *input, int width, int height, int r)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	float4 div = ((r+r+1)*(r+r+1));
	float4 val = 0.0f;
	for(int iy=y-r; iy<y+r+1; iy++)
	{
		for(int ix=x-r; ix<x+r+1; ix++)
		{
			val += GetfColorSrc(ix,iy, input, width, height);
		}
	}

	float4 sum = val/div;
	int position = x + y * width;
	output[position] = rgbaFloat4ToUint(NormalizeValue(sum),1.0f);    
}

//---------------------------------------------------------------------
//Application du filtre Gaussian Blur
//	int kernel[9] = { 5, 15, 31, 49, 56, 49, 31, 15, 5 };
//	int size = 3;
//	int factor = 256;
//---------------------------------------------------------------------
__kernel void GaussianBlur(__global uint * output, const __global uint *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	
	float4 sum = (GetfColorSrc(x - 1, y - 1, input, width, height) + GetfColorSrc(x + 1, y + 1, input, width, height))* (float4)5.0f;
	sum += (GetfColorSrc(x , y - 1, input, width, height) + GetfColorSrc(x , y + 1, input, width, height)) * (float4)15.0f;
	sum += (GetfColorSrc(x + 1, y - 1, input, width, height) + GetfColorSrc(x - 1, y + 1, input, width, height)) * (float4)31.0f;
	sum += (GetfColorSrc(x - 1, y, input, width, height) + GetfColorSrc(x + 1, y, input, width, height)) * (float4)49.0f;
	sum += GetfColorSrc(x , y, input, width, height) * (float4)56.0f;

	sum = sum / (float4)256.0f;
	int position = x + y * width;
	output[position] = rgbaFloat4ToUint(NormalizeValue(sum),1.0f);  
}

//---------------------------------------------------------------------
//Application du filtre Box Blur
//---------------------------------------------------------------------
__kernel void BoxBlurH(__global uint * output, const __global uint *input, int width, int height, int coeff)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	float4 sum = 0;
	
	for (int ix = x - coeff; ix < x + coeff + 1; ix++)
	{
		int posX = min(width - 1, max(0, ix));
		sum = sum + GetfColorSrc(posX, y, input, width, height);
	}

	sum = sum / (coeff + coeff + 1);
	int position = x + y * width;
	output[position] = rgbaFloat4ToUint(NormalizeValue(sum),1.0f);  
}

__kernel void BoxBlurV(__global uint * output, const __global uint *input, int width, int height, int coeff)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	float4 sum = 0;
	
	for (int iy = y - coeff; iy<y + coeff + 1; iy++)
	{
		int posY = min(height - 1, max(0, iy));
		sum = sum + GetfColorSrc(x, posY, input, width, height);
	}

	sum = sum / (float4)(coeff + coeff + 1);
	int position = x + y * width;
	output[position] = rgbaFloat4ToUint(NormalizeValue(sum),1.0f);  
}

//---------------------------------------------------------------------
//Application du filtre Motion Blur
//---------------------------------------------------------------------
__kernel void MotionBlur(__global uint * output, const __global uint *input, int width, int height, const __global float * kernelMotion, const __global int2 * offsets, int kernelSize)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	float4 sum = 0;
	for (int i = 0; i < kernelSize; i++)
	{
		int u = x + offsets[i].x;
		int v = y + offsets[i].y;
		if ((u < 0) || (u >= width) || (v < 0) || (v >= height))
			continue;

		float4 color = kernelMotion[i] * GetfColorSrc(u, v, input, width, height);
		sum = sum + color;
	}
	int position = x + y * width;
	output[position] = rgbaFloat4ToUint(NormalizeValue(sum),1.0f);    
}
