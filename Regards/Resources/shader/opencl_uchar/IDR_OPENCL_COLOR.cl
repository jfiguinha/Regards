typedef struct
{
	int red;
	int green;
	int blue;
	int alpha;
}COLORData;

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
//Filtre Niveau de gris
//----------------------------------------------------

//---------------------------------------------------------------------
//Limite les valeurs entre 0 et 1.0f
//---------------------------------------------------------------------
float4 NormalizeValue(float4 sum)
{
	float4 value;
	value.x = max(min(sum.x, 255.0f), 0.0f);
	value.y = max(min(sum.y, 255.0f), 0.0f);
	value.z = max(min(sum.z, 255.0f), 0.0f);  
	value.w = max(min(sum.w, 255.0f), 0.0f); 
	return value;
}


//----------------------------------------------------
//Filtre Posterization
//----------------------------------------------------
__kernel void Posterisation(__global uint *output,const __global uint *input, int level)
{
    int position = get_global_id(0);
	uchar4 colorInput = rgbaUintToUChar4(input[position]);
	uchar4 colorOutput = colorInput;
	int _levels = max(2, min(16, level));
	float _offset = (float)256 / (float)_levels;
	
	int red = colorInput.x / _offset;
	int green = colorInput.y / _offset;
	int blue = colorInput.z / _offset;
	
	colorOutput.x = (red * _offset);
	colorOutput.y = (green * _offset);
	colorOutput.z = (blue * _offset);

	output[position] = rgbaUChar4ToUint(colorOutput);
	
}

//----------------------------------------------------
//Filtre Solarization
//----------------------------------------------------
__kernel void Solarization(__global uint *output,const __global uint *input, int threshold)
{
    int position = get_global_id(0);
	float4 colorInput = rgbaUintToFloat4(input[position]);
	float4 colorOutput = colorInput;
	
	float red = colorInput.x;
	float green = colorInput.y;
	float blue = colorInput.z;
	float fthreshold = (float)threshold;
	
	if (red > fthreshold)
		colorOutput.x = 255.0f - red;
	else
		colorOutput.x = red;
		
	if (green > fthreshold)
		colorOutput.y = 255.0f - green;
	else
		colorOutput.y = green;

	if (blue > fthreshold)
		colorOutput.z = 255.0f - blue;
	else
		colorOutput.z = blue;

	output[position] = rgbaFloat4ToUint(colorOutput, 1.0f);
	
}


