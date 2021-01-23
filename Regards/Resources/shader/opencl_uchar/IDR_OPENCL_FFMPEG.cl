//----------------------------------------------------
// Inline device function to convert floating point rgba color to 32-bit unsigned integer
//*****************************************************************
__constant sampler_t sampler =
      CLK_NORMALIZED_COORDS_FALSE
    | CLK_ADDRESS_CLAMP_TO_EDGE
    | CLK_FILTER_NEAREST;
	
inline uint rgbaFloat4ToUint(float4 rgba, float fScale)
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
__kernel void CopyFromOpenGLTexture(__global uint *output, __read_only image2d_t input, int width, int height) 
{ 
    int x = get_global_id(0);
	int y = get_global_id(1);
	int position = x + (height - y -1) * width;
	const int2 pos = {x,y};
	output[position] = rgbaFloat4ToUint(read_imagef(input, sampler, pos),255.0f);
} 