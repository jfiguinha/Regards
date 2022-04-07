//----------------------------------------------------
//Conversion d'un bitmap en wxImage
//----------------------------------------------------
__kernel void BitmapToOpenGLTexture(__write_only image2d_t output, const __global uchar *input, int width, int height)
{
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