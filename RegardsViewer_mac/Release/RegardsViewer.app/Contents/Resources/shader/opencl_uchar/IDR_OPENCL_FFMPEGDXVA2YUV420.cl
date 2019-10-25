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


//----------------------------------------------------
// Conversion du NV12 vers du 32 bits
//----------------------------------------------------
float4 GetColorFromNV12(const __global uchar *inputY, int x, int y, int width, int height, int pitch, int surfaceHeight)
{
	if(x < width && y < height && y >= 0 && x >= 0)	
	{
		float4 color;
		int positionSrc = x + y * pitch;
		int positionUV = pitch * surfaceHeight;
		if (x & 1)
		{		
			if (y & 1)
				positionUV = (x - 1) + ((y - 1) / 2) * pitch + positionUV;
			else
				positionUV = (x - 1) + (y / 2) * pitch + positionUV;
		}
		else
		{
			if (y & 1)
				positionUV = x + ((y - 1) / 2) * pitch + positionUV;
			else
				positionUV = x + (y / 2) * pitch + positionUV;
		}
		
		float uComp= inputY[positionUV];
		float vComp = inputY[positionUV + 1];
		float yComp = inputY[positionSrc];		
				
		color.z = (1.164 * (yComp - 16) + 1.596*(vComp-128));
		color.y = (1.164 * (yComp - 16) - 0.391*(uComp-128) - 0.813*(vComp-128));
		color.x = (1.164 * (yComp - 16) + 2.018*(uComp-128));
		color.w = 255.0f;
		
		color = color;

		float4 minimal = 0.0;
		float4 maximal = 255.0;

		color = min(max(color,minimal),maximal);

	}
	return 0.0f;
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

float4 BiCubicNV12(float x, float y, const __global uchar *inputY, int width, int height, int pitch, int surfaceHeight)
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

	
	float4 sum = GetColorFromNV12(inputY, x - 1, y - 1, width, height, pitch,surfaceHeight) * (fy1 * fx1);
	sum += GetColorFromNV12(inputY, x , y - 1, width, height, pitch,surfaceHeight) * (fy1 * fx2);
	sum += GetColorFromNV12(inputY, x + 1, y - 1, width, height, pitch,surfaceHeight) * (fy1 * fx3);
	
	sum += GetColorFromNV12(inputY, x - 1, y, width, height, pitch,surfaceHeight) * (fy2 * fx1);
	sum += GetColorFromNV12(inputY, x , y, width, height, pitch,surfaceHeight) * (fy2 * fx2);
	sum += GetColorFromNV12(inputY, x + 1, y, width, height, pitch,surfaceHeight) * (fy2 * fx3);

	sum += GetColorFromNV12(inputY, x - 1, y + 1, width, height, pitch,surfaceHeight) * (fy3 * fx1);
	sum += GetColorFromNV12(inputY, x , y + 1, width, height, pitch,surfaceHeight) * (fy3 * fx2);
	sum += GetColorFromNV12(inputY, x + 1, y + 1, width, height, pitch,surfaceHeight) * (fy3 * fx3);
	
    return (sum / nDenom);
}

//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
float4 BicubicNV12(int x, int y, const __global uchar *input, int widthIn, int heightIn, int width, int height, int angle, int bicubic, int pitch, int surfaceHeight) 
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
		color = BiCubicNV12(posX, posY, input, widthIn, heightIn,pitch,surfaceHeight);
	else
		color = GetColorFromNV12(input, posX,  posY, widthIn, heightIn, pitch,surfaceHeight);
	
	return color;
} 


//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
__kernel void BicubicNV12toRegardsBitmap(__global uint *output, const __global uchar *input, int widthIn, int heightIn, int widthOut, int heightOut, int angle, int bicubic, int pitch, int surfaceHeight) 
{ 
    int x = get_global_id(0);
	int y = get_global_id(1);
	int position = x + y * widthOut;

	float4 color = BicubicNV12(x, y, input, widthIn, heightIn, widthOut, heightOut, angle, bicubic, pitch, surfaceHeight);
	output[position]  = rgbaFloat4ToUint(color,1.0f);
} 
