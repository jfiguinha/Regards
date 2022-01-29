float4 rgbaUintToFloat4(uint c)
{
    float4 rgba;
    rgba.x = c & 0xff;
    rgba.y = (c >> 8) & 0xff;
    rgba.z = (c >> 16) & 0xff;
    rgba.w = (c >> 24) & 0xff;
    return rgba;
}

uint rgbaFloat4ToUint(float4 rgba, float fScale)
{
    unsigned int uiPackedPix = 0U;
    uiPackedPix |= 0x000000FF & (unsigned int)(rgba.x * fScale);
    uiPackedPix |= 0x0000FF00 & (((unsigned int)(rgba.y * fScale)) << 8);
    uiPackedPix |= 0x00FF0000 & (((unsigned int)(rgba.z * fScale)) << 16);
    uiPackedPix |= 0xFF000000 & (((unsigned int)(rgba.w * fScale)) << 24);
    return uiPackedPix;
}

uint NormalizeValue(float4 sum)
{
	float4 value = sum;
	value.x = max(min(sum.x, 255.0f), 0.0f);
	value.y = max(min(sum.y, 255.0f), 0.0f);
	value.z = max(min(sum.z, 255.0f), 0.0f);  
	value.w = max(min(sum.w, 255.0f), 0.0f); 
	return rgbaFloat4ToUint(value,1.0f);
}

float4 GetColorSrc(int x, int y, const __global uint *input, int width, int height)
{
	if(x < width && y < height && y >= 0 && x >= 0)	
	{
		int position = x + y * width;
		return rgbaUintToFloat4(input[position]);
	}
	return (float4)0.0f;
}

__kernel void SharpenMasking( __global uint *output, const __global uint *input, int width, int height, float sharpness)
{
    int x = get_global_id(0);
	int y = get_global_id(1);

	float4 origin = GetColorSrc(x, y, input, width, height);

	float4 color = 0.0f;

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			color = color + GetColorSrc(x + j, y + i, input, width, height);
		}
	}

	color = color / (float4)9.0f;
	color = origin - color;
	
	float4 value = origin + color * sharpness;

	int position = x + y * width;
	output[position] = NormalizeValue(value);
}
