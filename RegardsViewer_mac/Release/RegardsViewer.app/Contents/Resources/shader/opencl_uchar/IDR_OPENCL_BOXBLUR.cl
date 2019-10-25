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
