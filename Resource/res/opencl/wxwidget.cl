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

