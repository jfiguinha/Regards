//----------------------------------------------------
// Inline device function to convert 32-bit unsigned integer to floating point rgba color 
//----------------------------------------------------
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
//Conversion d'un bitmap en wxImage
//----------------------------------------------------
__kernel void BitmapToOpenGLTexture(__write_only image2d_t output, const __global uint *input, int width, int height)
{
	const int2 pos = {get_global_id(0), get_global_id(1)};
    int x = get_global_id(0);
	int y = get_global_id(1);
	int position = x + y * width;

	float4 value;
	float4 color = rgbaUintToFloat4(input[position]);

	value.x = color.z / 255.0f;
	value.y = color.y / 255.0f;
	value.z = color.x / 255.0f;
	value.w = color.w / 255.0f;

	write_imagef(output, (int2)(pos.x, pos.y), value);
}

