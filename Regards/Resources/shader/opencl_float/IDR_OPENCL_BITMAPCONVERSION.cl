inline float4 rgbaUintToFloat4(uint c)
{
    float4 rgba;
    rgba.x = c & 0xff;
    rgba.y = (c >> 8) & 0xff;
    rgba.z = (c >> 16) & 0xff;
    rgba.w = (c >> 24) & 0xff;
    return rgba;
}


//----------------------------------------------------
//Conversion d'un regards bitmap en floatant
//----------------------------------------------------
__kernel void LoadFloatBitmap(__global float4 * output,const __global float4 * input, int width, int height)
{
	int position = get_global_id(0);
	output[position] = input[position];
}

//----------------------------------------------------
//Conversion d'un regards bitmap en floatant
//----------------------------------------------------
__kernel void GetFloatBitmap(__global float4 * output,const __global float4 * input, int width, int height)
{
	int position = get_global_id(0);
	output[position] = input[position];
}

//----------------------------------------------------
//Conversion d'un regards bitmap en floatant
//----------------------------------------------------
__kernel void LoadRegardsBitmap(__global float4 * output,const __global uchar4 * input, int width, int height)
{
	int position = get_global_id(0);
	float4 divisor = 255.0f;
	output[position] = convert_float4(input[position]) / divisor;
}

//----------------------------------------------------
//Conversion d'un regards bitmap en floatant
//----------------------------------------------------
__kernel void GetRegardsBitmap(__global uchar4 * output,const __global float4 * input, int width, int height)
{
	int position = get_global_id(0);
	float4 multiplicator = 255.0f;
	float4 value = input[position] * multiplicator;
	output[position] = convert_uchar4(value);
}

//----------------------------------------------------
//Conversion d'un bitmap en wxImage
//----------------------------------------------------
__kernel void BitmapToWxImage(__global uchar *output,const __global float * input, int width, int height)
{
	int position = get_global_id(0);

	int y = position / width;
	int x = position - (y * width);
	int positionInput = (height - y - 1) * width + x;
	
	output[position * 3] = input[positionInput * 4 + 2] * 255;
	output[position * 3+1] = input[positionInput * 4 + 1] * 255;
	output[position * 3+2] = input[positionInput * 4] * 255;
}

//----------------------------------------------------
//Conversion d'un bitmap en wxImage
//----------------------------------------------------
__kernel void BitmapToWxImageDXVA2(__global uchar *output,const __global float *input, int width, int height)
{
	int position = get_global_id(0);

	int y = position / width;
	int x = position - (y * width);
	
	output[position * 3] = input[position * 4 + 2] * 255;
	output[position * 3+1] = input[position * 4 + 1] * 255;
	output[position * 3+2] = input[position * 4] * 255;
}

//----------------------------------------------------
//Conversion d'un bitmap en wxImage
//----------------------------------------------------
__kernel void BitmapToOpenGLTexture(__write_only image2d_t output, const __global float4 *input, int width, int height, int rgba)
{
	const int2 pos = {get_global_id(0), get_global_id(1)};
    int x = get_global_id(0);
	int y = get_global_id(1);
	y = height - y - 1;
	int position = x + y * width;
	
	float4 value = input[position];
	if(rgba == 0)
	{
		value.x = input[position].z;
		value.z = input[position].x;
	}
	
	/*
	float4 value;
		
	if(rgba == 1)
	{
		value.x = (float)input[position].x / (float)255;
		value.y = (float)input[position].y / (float)255;
		value.z = (float)input[position].z / (float)255;
		value.w = (float)input[position].w / (float)255;	
	}
	else
	{
		value.x = (float)input[position].z / (float)255;
		value.y = (float)input[position].y / (float)255;
		value.z = (float)input[position].x / (float)255;
		value.w = (float)input[position].w / (float)255;
	}
	*/
	write_imagef(output, (int2)(pos.x, pos.y), value);
}
__constant sampler_t sampler =
      CLK_NORMALIZED_COORDS_FALSE
    | CLK_ADDRESS_CLAMP_TO_EDGE
    | CLK_FILTER_NEAREST;

//----------------------------------------------------
//Conversion d'un bitmap en wxImage
//----------------------------------------------------
__kernel void ImportFromOpencv(__global float4 * output,const __global uchar4 * input, int width, int height)
{
	int position = get_global_id(0);
	float4 divisor = 255.0f;
	output[position] = convert_float4(input[position]) / divisor;
}

//----------------------------------------------------
//Conversion d'un bitmap en wxImage
//----------------------------------------------------
__kernel void ImportFromOpencvFloat(__global float4 * output,const __global float4 * input, int width, int height)
{
	int position = get_global_id(0);
	output[position] = input[position];
}

//----------------------------------------------------
//Conversion d'un bitmap en wxImage
//----------------------------------------------------
__kernel void CopyToOpencv(__global uchar4 * output,const __global float4 * input, int width, int height)
{
	int position = get_global_id(0);
	float divisor = 255.0f;
	output[position].x = (uchar)(input[position].x * divisor);
	output[position].y = (uchar)(input[position].y * divisor);
	output[position].z = (uchar)(input[position].z * divisor);
	output[position].w = (uchar)(input[position].w * divisor);
}

__kernel void CopyToOpencvFloat(__global float4 * output,const __global float4 * input, int width, int height)
{
	int position = get_global_id(0);
	output[position] = input[position];
}
	
//----------------------------------------------------
//Conversion d'un bitmap en wxImage
//----------------------------------------------------
__kernel void OpenGLTextureToBitmap( __global float4 * output, __read_only image2d_t input, int width, int height)
{
	const int2 pos = {get_global_id(0), get_global_id(1)};
    int x = get_global_id(0);
	int y = get_global_id(1);
	y = height - y - 1;
	//float4 data = read_imagef(input, sampler, pos);
	
	int position = x + y * width;
	output[position] = read_imagef(input, sampler, pos);
	/*
	output[position].x = data.x * (float)255;
	output[position].y = data.y * (float)255;
	output[position].z = data.z * (float)255;
	output[position].w = data.w * (float)255;
	*/
}

//----------------------------------------------------
//Conversion d'un cximage en regards bitmap
//----------------------------------------------------
__kernel void LoadCxImageAlpha(__global float * output,const __global uchar * input, const __global uchar * alpha, int width, int height, int effwidth)
{
	int position = get_global_id(0);
	
	int y = position / width;
	int x = position - (y * width);	
	
	int positionSource = effwidth * y + x * 3;
	
	output[position * 4]   = (float)input[positionSource] / 255.0f;
	output[position * 4+1] = (float)input[positionSource + 1] / 255.0f;
	output[position * 4+2] = (float)input[positionSource + 2] / 255.0f;
	output[position * 4+3] = (float)alpha[position] / 255.0f;
}

__kernel void LoadCxImageAlphaRGB(__global float * output,const __global uchar * input, const __global uchar * alpha, int width, int height, int effwidth)
{
	int position = get_global_id(0);
	
	int y = position / width;
	int x = position - (y * width);	
	
	int positionSource = effwidth * y + x * 3;	
	
	output[position * 4]   = (float)input[positionSource + 2] / 255.0f;
	output[position * 4+1] = (float)input[positionSource + 1] / 255.0f;
	output[position * 4+2] = (float)input[positionSource] / 255.0f;
	output[position * 4+3] = (float)alpha[position] / 255.0f;
}

//----------------------------------------------------
//Conversion d'un cximage en regards bitmap
//----------------------------------------------------
__kernel void LoadCxImage(__global float *output,const __global uchar *input, int width, int height, int effwidth)
{
	int position = get_global_id(0);
	
	int y = position / width;
	int x = position - (y * width);	
	
	int positionSource = effwidth * y + x * 3;		
	
	output[position * 4]   = (float)input[positionSource] / 255.0f;
	output[position * 4+1] = (float)input[positionSource + 1] / 255.0f;
	output[position * 4+2] = (float)input[positionSource + 2] / 255.0f;
}


//----------------------------------------------------
//Conversion d'un cximage en regards bitmap
//----------------------------------------------------
__kernel void LoadCxImageRGB(__global float *output,const __global uchar *input, int width, int height, int effwidth)
{
	int position = get_global_id(0);
	
	int y = position / width;
	int x = position - (y * width);	
	
	int positionSource = effwidth * y + x * 3;		
	
	output[position * 4]   = (float)input[positionSource + 2] / 255.0f;
	output[position * 4+1] = (float)input[positionSource + 1] / 255.0f;
	output[position * 4+2] = (float)input[positionSource] / 255.0f;
}

//----------------------------------------------------
//Conversion d'un wximage en regards bitmap
//----------------------------------------------------
__kernel void LoadWxImageAlpha(__global float * output,const __global uchar * input, const __global uchar * alpha, int width, int height, int effwidth)
{
	int position = get_global_id(0);
	int y = position / width;
	int x = position - (y * width);		
	
	int positionSource = (height - y - 1) * effwidth + x * 3;
	
	output[position * 4]   = (float)input[positionSource + 2] / 255.0f;
	output[position * 4+1] = (float)input[positionSource + 1]/ 255.0f;
	output[position * 4+2] = (float)input[positionSource]/ 255.0f;
	output[position * 4+3] = (float)alpha[position]/ 255.0f;
}

//----------------------------------------------------
//Conversion d'un cximage en regards bitmap
//----------------------------------------------------
__kernel void LoadWxImage(__global float *output,const __global uchar *input, int width, int height, int effwidth)
{
	int position = get_global_id(0);
	
	int y = position / width;
	int x = position - (y * width);		
	
	int positionSource = (height - y - 1) * effwidth + x * 3;
	
	output[position * 4]   = (float)input[positionSource + 2]/ 255.0f;
	output[position * 4+1] = (float)input[positionSource + 1]/ 255.0f;
	output[position * 4+2] = (float)input[positionSource]/ 255.0f;
}

