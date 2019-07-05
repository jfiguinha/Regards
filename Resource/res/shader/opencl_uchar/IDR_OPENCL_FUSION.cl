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

__kernel void Fusion(__global uint * output, const __global uint *input, const __global uint *inputSecond, float pourcentage)
{
	int position = get_global_id(0);
	float4 prcent = pourcentage;
	float4 diff = 1.0f - pourcentage;
	float4 image1 = rgbaUintToFloat4(input[position]) * diff;
	float4 image2 = rgbaUintToFloat4(inputSecond[position]) * prcent;
	output[position] = rgbaFloat4ToUint(image1 + image2, 1.0f);
}

