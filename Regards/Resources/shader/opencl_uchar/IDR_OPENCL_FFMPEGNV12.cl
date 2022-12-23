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

float4 GetColorFromNV12(const __global uchar * inputY, const __global uchar *inputUV, int x, int y, int width, int height, int pitch, int colorRange, int colorSpace)
{
		float4 color;
		int positionSrc = x + y * pitch;
		int positionUV = 0;

		int yModulo = y % 2;
		int xModulo = x % 2;
		if (xModulo == 1)
		{		
			if (yModulo == 1)
				positionUV = (x - 1) + ((y - 1) / 2) * pitch;
			else
				positionUV = (x - 1) + (y / 2) * pitch;
		}
		else
		{
			if (yModulo == 1)
				positionUV = x + ((y - 1) / 2) * pitch;
			else
				positionUV = x + (y / 2) * pitch;
		}
		
		uchar vComp = inputUV[positionUV];
		uchar uComp = inputUV[positionUV + 1];
		uchar yComp = inputY[positionSrc];
		
		float3 matrix[3];
		if(colorSpace == 0)
		{
			//default
			matrix[0].x = 1.164;
			matrix[0].y = 0;
			matrix[0].z = 1.596;
			
			matrix[1].x = 1.164;
			matrix[1].y = -0.391;
			matrix[1].z = -0.813;

			matrix[2].x = 1.164;
			matrix[2].y = 2.018;
			matrix[2].z = 0;			
		}
		else if(colorSpace == 1)
		{
			//bt601
			matrix[0].x = 1;
			matrix[0].y = 0;
			matrix[0].z = 1;
			
			matrix[1].x = 1;
			matrix[1].y = -0.344;
			matrix[1].z = -0.714;

			matrix[2].x = 1;
			matrix[2].y = 1.772;
			matrix[2].z = 0;
		}
		else if(colorSpace == 2)
		{
			//BT.709
			matrix[0].x = 1;
			matrix[0].y = 0;
			matrix[0].z = 1.5748;
			
			matrix[1].x = 1;
			matrix[1].y = -0.187324;
			matrix[1].z = -0.468124;

			matrix[2].x = 1;
			matrix[2].y = 1.8556;
			matrix[2].z = 0;
		}		
		else if(colorSpace == 3)
		{
			//BT.2020
			matrix[0].x = 1;
			matrix[0].y = 0;
			matrix[0].z = 1.402;
			
			matrix[1].x = 1;
			matrix[1].y = -0.344136286;
			matrix[1].z = -0.7141362862;

			matrix[2].x = 1;
			matrix[2].y = 1.772;
			matrix[2].z = 0;
		}	
		
		color.x = (matrix[0].x * (yComp - 16) + matrix[0].y * (uComp-128) + matrix[0].z * (vComp-128));
		color.y = (matrix[1].x * (yComp - 16) + matrix[1].y * (uComp-128) + matrix[1].z * (vComp-128));
		color.z = (matrix[2].x * (yComp - 16) + matrix[2].y * (uComp-128) + matrix[2].z * (vComp-128));
		color.w = 255.0f;

		float4 minimal = 0.0;
		float4 maximal = 255.0;
		
		if(colorRange == 1)
		{
			minimal = 16.0;
			maximal = 235.0;
		}

		return color = min(max(color,minimal),maximal);

}

//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
__kernel void Convert(__global uint *output, const __global uchar *inputY, const __global uchar *inputUV, int widthIn, int heightIn, int widthOut, int heightOut, int pitch, int colorRange, int colorSpace) 
{ 
    int x = get_global_id(0);
	int y = get_global_id(1);
	int position = x +  (heightOut - y - 1) * widthOut;
	if(x < widthOut && y < heightOut && y >= 0 && x >= 0)	
	{
		float4 color = GetColorFromNV12(inputY, inputUV, x,  y, widthIn, heightIn, pitch, colorRange, colorSpace); 
		output[position] = rgbaFloat4ToUint(color,1.0f);
	}
} 
