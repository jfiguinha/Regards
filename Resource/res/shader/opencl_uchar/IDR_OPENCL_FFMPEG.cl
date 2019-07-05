//----------------------------------------------------
// Conversion du 
//----------------------------------------------------

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

float Cubic( float f )
{
	if (f < -2.0f)
		return(0.0f);
	if (f < -1.0f)
		return((2.0f+f)*(2.0f+f)*(2.0f+f)/6.0f);
	if (f < 0.0f)
		return((4.0f+f*f*(-6.0f-3.0f*f))/6.0f);
	if (f < 1.0f)
		return((4.0f+f*f*(-6.0f+3.0f*f))/6.0f);
	if (f < 2.0f)
		return((2.0f-f)*(2.0f-f)*(2.0f-f)/6.0f);
	return(0.0f);
}

__constant sampler_t sampler =
      CLK_NORMALIZED_COORDS_FALSE
    | CLK_ADDRESS_CLAMP_TO_EDGE
    | CLK_FILTER_NEAREST;

float4 GetColorFromOpenGLTexture( __read_only image2d_t input, int x, int y, int width, int height)
{
	const int2 pos = {x,y};
	if(x < width && y < height && y >= 0 && x >= 0)	
	{
		float4 data = read_imagef(input, sampler, pos);
		return data;
	}
	return 0.0f;
}

float4 BiCubicFromOpenGLTexture(float x, float y, __read_only image2d_t input, int width, int height)
{
	float4 nDenom = 0.0f;
	int valueA = (int)x;
	int valueB = (int)y;
	float realA = x - valueA;
	float realB = y - valueB;
	
	float4 fy1 = Cubic(-(-1.0f - realB));
	float4 fy2 = Cubic(realB);
	float4 fy3 = Cubic(-(1.0f - realB));
	
	float4 fx1 = Cubic(-1.0f - realA);
	float4 fx2 = Cubic(- realA);
	float4 fx3 = Cubic(1.0f - realA);
	
	nDenom += fy1 * (fx1 + fx2 + fx3) + fy2 * (fx1 + fx2 + fx3) + fy3 * (fx1 + fx2 + fx3);

	float4 sum = GetColorFromOpenGLTexture(input, x - 1, y - 1, width, height) * (fy1 * fx1);
	sum += GetColorFromOpenGLTexture(input, x , y - 1, width, height) * (fy1 * fx2);
	sum += GetColorFromOpenGLTexture(input, x + 1, y - 1, width, height) * (fy1 * fx3);
	
	sum += GetColorFromOpenGLTexture(input, x - 1, y, width, height) * (fy2 * fx1);
	sum += GetColorFromOpenGLTexture(input, x , y, width, height) * (fy2 * fx2);
	sum += GetColorFromOpenGLTexture(input, x + 1, y, width, height) * (fy2 * fx3);

	sum += GetColorFromOpenGLTexture(input, x - 1, y + 1, width, height) * (fy3 * fx1);
	sum += GetColorFromOpenGLTexture(input, x , y + 1, width, height) * (fy3 * fx2);
	sum += GetColorFromOpenGLTexture(input, x + 1, y + 1, width, height) * (fy3 * fx3);
	
    return (sum / nDenom);
}

//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
float4 ExecuteBicubicFromOpenGLTexture(int x, int y, __read_only image2d_t input, int widthIn, int heightIn, int width, int height, int angle, int bicubic) 
{ 
	float ratioX = (float)widthIn / (float)width;
	float ratioY = (float)heightIn / (float)height;
	if (angle == 90 || angle == 270)
	{
		ratioX = (float)widthIn / (float)height;
		ratioY = (float)heightIn / (float)width;
	}

	float posY = (float)y * ratioY;
	float posX = (float)x * ratioX;

	if (angle == 90)
	{
		int srcx = posY;
		int srcy = posX;

		posX = srcx;
		posY = srcy;

		posX = widthIn - posX - 1;
	}
	else if (angle == 180)
	{
		posX = widthIn - posX - 1;
		posY = heightIn - posY - 1;
	}
	else if (angle == 270)
	{
		int srcx = posY;
		int srcy = posX;

		posX = srcx;
		posY = srcy;

		posY = heightIn - posY - 1;
	}
	
	float4 color;
	if(bicubic)
		color = BiCubicFromOpenGLTexture(posX, posY, input, widthIn, heightIn);
	else
		color = GetColorFromOpenGLTexture(input, posX,  posY, widthIn, heightIn);
	
	return color;
} 

//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
__kernel void InterpolationFromOpenGLTexture(__global uint *output, __read_only image2d_t input, int width, int height, int widthOut, int heightOut, int angle, int bicubic) 
{ 
    int x = get_global_id(0);
	int y = get_global_id(1);
	int position = x + y * widthOut;
	output[position] = rgbaFloat4ToUint(ExecuteBicubicFromOpenGLTexture(x, y, input, width, height, widthOut, heightOut, angle, bicubic),1.0f);
} 

//----------------------------------------------------
//Change la valeur de la couche alpha
//----------------------------------------------------
__kernel void SetAlphaValueFromOpenGLTexture(__global uint *output, __read_only image2d_t input, int width, int height, float alphaValue) 
{
    int x = get_global_id(0);
	int y = get_global_id(1);

	int position = x + (height - y - 1) * width;
	const int2 pos = {x,y};

	float4 data = read_imagef(input, sampler, pos);
	data.w = (alphaValue / 100.0f);

	output[position] = rgbaFloat4ToUint(data,1.0f);
}