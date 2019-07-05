
inline uchar4 rgbaUintToUChar4(uint c)
{
    uchar4 rgba;
    rgba.x = c & 0xff;
    rgba.y = (c >> 8) & 0xff;
    rgba.z = (c >> 16) & 0xff;
    rgba.w = (c >> 24) & 0xff;
    return rgba;
}

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
//Conversion en yuv => Rcupration de la valeur Y
//----------------------------------------------------
__kernel void ConvertToY(__global float * output, const __global uint * input)
{
	int position = get_global_id(0);
	float4 color = rgbaUintToFloat4(input[position]);
	output[position] = (0.257f * (float)color.z + 0.504f * color.y + 0.098f * color.x + 16.0f);
}

//----------------------------------------------------
//Incorporation de la valeur Y
//----------------------------------------------------
__kernel void InsertYValue(__global uint * output, const __global float * Yinput, const __global uint * source)
{
	int position = get_global_id(0);
	float4 color = rgbaUintToFloat4(source[position]);
	float B = color.x;
	float G = color.y;
	float R = color.z;

	float half_parted = 128.0f;
	float seize = 16.0f;
	
	float Y = Yinput[position];
	float V = (0.439 * R) - (0.368 * G) - (0.071 * B) + half_parted;
	float U = -(0.148 * R) - (0.291 * G) + (0.439 * B) + half_parted;

	float4 colorSource = rgbaUintToFloat4(source[position]);
	color.x = (1.164 * (Y - seize) + 2.018 * (U - half_parted));
	color.y = (1.164 * (Y - seize) - 0.813 * (V - half_parted) - 0.391 * (U - half_parted));
	color.z = (1.164 * (Y - seize) + 1.596 * (V - half_parted));	
	color.w = colorSource.w;
	
	output[position] = rgbaFloat4ToUint(color, 1.0f);
}

//----------------------------------------------------
//Extraction d'un bloc size de la valeur Y
//----------------------------------------------------
__kernel void ExtractBlocSize(__global float * output, const __global float * input, int size, int marge, int width, int height, int x, int y)
{
    int posX = get_global_id(0);
	int posY = get_global_id(1);

	int posLocal = (posY * (size + marge * 2) + posX);
	
	int inputX = posX + (x * size) - marge;
	int inputY = posY + (y * size) - marge;
	
	if(inputX < width && inputX >= 0 && inputY >= 0 && inputY < height)
		output[posLocal] = input[inputX + inputY * width];
	else
		output[posLocal] = 0.0f;
}

//----------------------------------------------------
//Extraction d'un bloc size de la valeur Y
//----------------------------------------------------
__kernel void InsertBlockSize(__global float * yPicture, const __global float * wiener, int size, int marge, int width, int height, int x, int y)
{
    int posX = get_global_id(0);
	int posY = get_global_id(1);

	int position = (posY * (size + marge * 2) + posX);
	
	int inputX = posX + (x * size) - marge;
	int inputY = posY + (y * size) - marge;
	
	int posMax = size + marge;
	
	if(inputX <  width && inputX >= 0 && inputY >= 0 && inputY < height && posX >= marge && posX < posMax && posY >= marge && posY < posMax)
	{
		int posLocal = inputX + inputY * width;
		yPicture[posLocal] = wiener[position];
	}
}
