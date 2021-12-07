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

//----------------------------------------------------
//Conversion d'un regards bitmap en floatant
//----------------------------------------------------
__kernel void LoadFloatBitmap(__global uint * output,const __global float4 * input, int width, int height)
{
	int position = get_global_id(0);
	output[position] = rgbaFloat4ToUint(input[position],255.0f);
}

//----------------------------------------------------
//Conversion d'un regards bitmap en floatant
//----------------------------------------------------
__kernel void GetFloatBitmap(__global float4 * output,const __global uint * input, int width, int height)
{
	int position = get_global_id(0);
	output[position] = input[position];
}

//----------------------------------------------------
//Conversion d'un regards bitmap en floatant
//----------------------------------------------------
__kernel void LoadRegardsBitmap(__global uint * output,const __global uchar4 * input, int width, int height)
{
	int position = get_global_id(0);
	output[position] = rgbaUChar4ToUint(input[position]);
}

//----------------------------------------------------
//Conversion d'un regards bitmap en floatant
//----------------------------------------------------
__kernel void GetRegardsBitmap(__global uchar4 * output,const __global uint * input, int width, int height)
{
	int position = get_global_id(0);
	output[position] = rgbaUintToUChar4(input[position]);
}

//----------------------------------------------------
//Conversion d'un bitmap en wxImage
//----------------------------------------------------
__kernel void BitmapToWxImage(__global uchar * output, const __global uint * input, int width, int height)
{
	int position = get_global_id(0);

	int y = position / width;
	int x = position - (y * width);
	int positionInput = (height - y - 1) * width + x;
	
	uchar4 color = rgbaUintToUChar4(input[positionInput]);
	
	output[position * 3] = color.z;
	output[position * 3+1] = color.y;
	output[position * 3+2] = color.x;
}

//----------------------------------------------------
//Conversion d'un bitmap en wxImage
//----------------------------------------------------
__kernel void BitmapToWxImageDXVA2(__global uchar *output,const __global uint *input, int width, int height)
{
	int position = get_global_id(0);

	int y = position / width;
	int x = position - (y * width);
	
	uchar4 color = rgbaUintToUChar4(input[position]);
	
	output[position * 3] = color.z;
	output[position * 3+1] = color.y;
	output[position * 3+2] = color.x;
}

//----------------------------------------------------
//Conversion d'un bitmap en wxImage
//----------------------------------------------------
__kernel void BitmapToOpenGLTexture(__write_only image2d_t output, const __global uint *input, int width, int height, int rgba)
{
	const int2 pos = {get_global_id(0), get_global_id(1)};
    int x = get_global_id(0);
	int y = get_global_id(1);
	y = height - y - 1;
	int position = x + y * width;

	float4 value;
	float4 divisor = 255.0f;
	float4 color = rgbaUintToFloat4(input[position]);
		
	if(rgba == 1)
	{
		value = color / divisor;
	}
	else
	{
		value.x = color.z / 255.0f;
		value.y = color.y / 255.0f;
		value.z = color.x / 255.0f;
		value.w = color.w / 255.0f;
	}

	write_imagef(output, (int2)(pos.x, pos.y), value);
}


__constant sampler_t sampler =
      CLK_NORMALIZED_COORDS_FALSE
    | CLK_ADDRESS_CLAMP_TO_EDGE
    | CLK_FILTER_NEAREST;
	
//----------------------------------------------------
//Conversion d'un bitmap en wxImage
//----------------------------------------------------
__kernel void ImportFromOpencv(__global uchar4 * output,const __global uchar4 * input, int width, int height)
{
	int position = get_global_id(0);
	output[position] = input[position];

}

//----------------------------------------------------
//Conversion d'un bitmap en wxImage
//----------------------------------------------------
__kernel void CopyToOpencv(__global uchar4 * output,const __global uchar4 * input, int width, int height)
{
	int position = get_global_id(0);
	output[position] = input[position];
}

	
//----------------------------------------------------
//Conversion d'un bitmap en wxImage
//----------------------------------------------------
__kernel void OpenGLTextureToBitmap( __global uint * output, __read_only image2d_t input, int width, int height)
{
	const int2 pos = {get_global_id(0), get_global_id(1)};
    int x = get_global_id(0);
	int y = get_global_id(1);
	y = height - y - 1;
	float4 data = read_imagef(input, sampler, pos);
	
	int position = x + y * width;
	output[position] = rgbaFloat4ToUint(data, 255.0f);

}

//----------------------------------------------------
//Conversion d'un cximage en regards bitmap
//----------------------------------------------------
__kernel void LoadCxImageAlpha(__global uint * output,const __global uchar * input, const __global uchar * alpha, int width, int height, int effwidth)
{
	int position = get_global_id(0);
	
	int y = position / width;
	int x = position - (y * width);	
	
	int positionSource = effwidth * y + x * 3;
	
	uchar4 color;
	color.x  = (float)input[positionSource];
	color.y = input[positionSource + 1];
	color.z  = input[positionSource + 2];
	color.w  = alpha[position];
	
	output[position] = rgbaUChar4ToUint(color);
}

__kernel void LoadCxImageAlphaRGB(__global uint * output,const __global uchar * input, const __global uchar * alpha, int width, int height, int effwidth)
{
	int position = get_global_id(0);
	
	int y = position / width;
	int x = position - (y * width);	
	
	int positionSource = effwidth * y + x * 3;	
	
	uchar4 color;
	color.x  = (float)input[positionSource + 2];
	color.y = (float)input[positionSource + 1];
	color.z = (float)input[positionSource];
	color.w = (float)alpha[position];
	
	output[position] = rgbaUChar4ToUint(color);
}

//----------------------------------------------------
//Conversion d'un cximage en regards bitmap
//----------------------------------------------------
__kernel void LoadCxImage(__global uint *output,const __global uchar *input, int width, int height, int effwidth)
{
	int position = get_global_id(0);
	
	int y = position / width;
	int x = position - (y * width);	
	
	int positionSource = effwidth * y + x * 3;		
	
	uchar4 color;
	color.x = input[positionSource];
	color.y = input[positionSource + 1];
	color.z = input[positionSource + 2];
	color.w = 0;
	output[position] = rgbaUChar4ToUint(color);
}


//----------------------------------------------------
//Conversion d'un cximage en regards bitmap
//----------------------------------------------------
__kernel void LoadCxImageRGB(__global uint *output,const __global uchar *input, int width, int height, int effwidth)
{
	int position = get_global_id(0);
	
	int y = position / width;
	int x = position - (y * width);	
	
	int positionSource = effwidth * y + x * 3;		
	
	uchar4 color;
	color.x = input[positionSource + 2];
	color.y = input[positionSource + 1];
	color.z = input[positionSource];
	color.w = 0;
	output[position] = rgbaUChar4ToUint(color);	
}

//----------------------------------------------------
//Conversion d'un wximage en regards bitmap
//----------------------------------------------------
__kernel void LoadWxImageAlpha(__global uint * output,const __global uchar * input, const __global uchar * alpha, int width, int height, int effwidth)
{
	int position = get_global_id(0);
	int y = position / width;
	int x = position - (y * width);		
	
	int positionSource = (height - y - 1) * effwidth + x * 3;
	
	uchar4 color;
	color.x  = input[positionSource + 2];
	color.y = input[positionSource + 1];
	color.z = input[positionSource];
	color.w = alpha[position];
	output[position] = rgbaUChar4ToUint(color);
}

//----------------------------------------------------
//Conversion d'un cximage en regards bitmap
//----------------------------------------------------
__kernel void LoadWxImage(__global uint *output,const __global uchar *input, int width, int height, int effwidth)
{
	int position = get_global_id(0);
	
	int y = position / width;
	int x = position - (y * width);		
	
	int positionSource = (height - y - 1) * effwidth + x * 3;
	
	uchar4 color;
	color.x = input[positionSource + 2];
	color.y = input[positionSource + 1];
	color.z = input[positionSource];
	output[position] = rgbaUChar4ToUint(color);	
}

