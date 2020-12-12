//----------------------------------------------------
// Conversion du NV12 vers du 32 bits
//----------------------------------------------------
inline uint rgbaFloat4ToUint(float4 rgba, float fScale)
{
    unsigned int uiPackedPix = 0U;
    uiPackedPix |= 0x000000FF & (unsigned int)(rgba.x * fScale);
    uiPackedPix |= 0x0000FF00 & (((unsigned int)(rgba.y * fScale)) << 8);
    uiPackedPix |= 0x00FF0000 & (((unsigned int)(rgba.z * fScale)) << 16);
    uiPackedPix |= 0xFF000000 & (((unsigned int)(rgba.w * fScale)) << 24);
    return uiPackedPix;
}

float4 GetColorFromNV12(const __global uchar * inputY, const __global uchar *inputUV, int x, int y, int width, int height, int pitch)
{
	if(x < width && y < height && y >= 0 && x >= 0)	
	{
		float4 color;
		int positionSrc = x + y * pitch;
		int positionUV = 0;

		int yModulo = y % 2;
		int xModulo = x % 2;
		if (xModulo == 1)
		{		
			if (yModulo == 1)
				positionUV = (x - 1) + ((y - 1) / 2) * width;
			else
				positionUV = (x - 1) + (y / 2) * width;
		}
		else
		{
			if (yModulo == 1)
				positionUV = x + ((y - 1) / 2) * width + positionUV;
			else
				positionUV = x + (y / 2) * width + positionUV;
		}
		
		float vComp = inputUV[positionUV];
		float uComp = inputUV[positionUV + 1];
		float yComp = inputY[positionSrc];
		
		color.x = (1.164 * (yComp - 16) + 1.596*(vComp-128));
		color.y = (1.164 * (yComp - 16) - 0.391*(uComp-128) - 0.813*(vComp-128));
		color.z = (1.164 * (yComp - 16) + 2.018*(uComp-128));
		color.w = 255.0f;
		
		color = color / (float4)255.0f;

		float4 minimal = 0.0;
		float4 maximal = 1.0;

		return color = min(max(color,minimal),maximal);
		
	}
	return 0.0f;
}

//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
__kernel void Convert(__global uint *output, const __global uchar *inputY, const __global uchar *inputUV, int widthIn, int heightIn, int widthOut, int heightOut, int pitch) 
{ 
    int x = get_global_id(0);
	int y = get_global_id(1);
	int position = x + (heightIn - y - 1) * widthOut;

	float4 color = GetColorFromNV12(inputY, inputUV, x,  y, widthIn, heightIn, pitch); 
	output[position] = rgbaFloat4ToUint(color,1.0f);
} 

