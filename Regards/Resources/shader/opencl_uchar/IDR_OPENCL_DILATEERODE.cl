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

float4 GetColorSrc(int x, int y, const __global uint *input, int widthIn, int heightIn)
{
	if(x < widthIn && y < heightIn && y >= 0 && x >= 0)	
	{
		int position = x + y * widthIn;
		return rgbaUintToFloat4(input[position]);
	}
	return 0.0f;
}

__kernel void Erode(__global uint *output, const __global uint *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	float4 maxValue = 0;

	for (int n = -1; n < 2; n++)
	{
		int posY = y + n;
		for (int m = -1; m < 2; m++)
		{
			int posX = x + m;				
			maxValue = max(GetColorSrc(posX, posY, input, width, height) , maxValue);
		}
	}
	
	int position = x + y * width;

	output[position] = rgbaFloat4ToUint(maxValue,1.0f);
}

__kernel void Dilate(__global uint *output, const __global uint *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	float4 minValue = 255.0f;

	for (int n = -1; n < 2; n++)
	{
		int posY = y + n;
		for (int m = -1; m < 2; m++)
		{
			int posX = x + m;				
			minValue = min(GetColorSrc(posX, posY, input, width, height) , minValue);
		}
	}
	
	int position = x + y * width;

	output[position] = rgbaFloat4ToUint(minValue,1.0f);
}
