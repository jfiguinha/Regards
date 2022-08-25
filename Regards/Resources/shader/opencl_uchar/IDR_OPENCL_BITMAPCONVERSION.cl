//----------------------------------------------------
//Conversion d'un bitmap en wxImage
//----------------------------------------------------
__kernel void BitmapToOpenGLTexture(__write_only image2d_t output, const __global uchar *input, int width, int height)
{
	//int stride = 3 * ((24 * width + 31)/32);
	const int2 pos = {get_global_id(0), get_global_id(1)};
    int x = get_global_id(0);
	int y = get_global_id(1);
	int position = x * 3 + y * width * 3;

	float4 value;

	value.x = input[position+2]/ 255.0f;
	value.y = input[position+1] / 255.0f;
	value.z = input[position] / 255.0f;
	value.w = 0;

	write_imagef(output, (int2)(pos.x, pos.y), value);
}

__kernel void BitmapToOpenGLTextureApple(__write_only image2d_t output, const __global uchar *input, int width, int height)
{
	//int stride = 3 * ((24 * width + 23)/24);
	const int2 pos = {get_global_id(0), get_global_id(1)};
    int x = get_global_id(0);
	int y = get_global_id(1);

    if(x < width && y < height && y >= 0 && x >= 0)	
    {
	    int position = x * 3 + y * width * 3;

	    float4 value;

	    value.x = input[position]/ 255.0f;
	    value.y = input[position+1] / 255.0f;
	    value.z = input[position+2] / 255.0f;
	    value.w = 0;

	    write_imagef(output, (int2)(pos.x, pos.y), value);
    }
}
