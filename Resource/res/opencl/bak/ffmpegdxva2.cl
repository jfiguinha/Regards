//----------------------------------------------------
// Conversion du NV12 vers du 32 bits
//----------------------------------------------------
uchar4 GetColorFromNV12(const __global uchar *inputY, int x, int y, int width, int height, int pitch, int surfaceHeight)
{
	if(x < width && y < height && y >= 0 && x >= 0)	
	{
		uchar4 color;
		int crv = 104597;
		int	cbu = 132201;
		int cgu = 25675;  
		int cgv = 53279;
		int positionSrc = x + y * pitch;
		int positionUV = pitch * surfaceHeight;

		int yModulo = y % 2;
		int xModulo = x % 2;
		if (xModulo == 1)
		{		
			if (yModulo == 1)
				positionUV = (x - 1) + ((y - 1) / 2) * pitch + positionUV;
			else
				positionUV = (x - 1) + (y / 2) * pitch + positionUV;
		}
		else
		{
			if (yModulo == 1)
				positionUV = x + ((y - 1) / 2) * pitch + positionUV;
			else
				positionUV = x + (y / 2) * pitch + positionUV;
		}
		
		int v = inputY[positionUV];
		int u = inputY[positionUV + 1];

		int c1 = (v-128) * crv;
		int c2 = (u-128) * cgu;
		int c3 = (v-128) * cgv;
		int c4 = (u-128) * cbu;
		
		int y1 = 76309*(inputY[positionSrc]-16);
		int r = (y1 + c1) >> 16;
		int g = (y1 - c2 - c3) >> 16;
		int b = (y1 + c4) >> 16;
		
		if(r < 0)
			r = 0;
		if(r > 255)
			r = 255;

		if(g < 0)
			g = 0;
		if(g > 255)
			g = 255;
			
		if(b < 0)
			b = 0;
		if(b > 255)
			b = 255;		
		
		color.x = r;
		color.y = g;
		color.z = b;
		color.w = 255;
		
		return color;
	}
	return (uchar4)0;
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

	
	float4 sum = convert_float4(GetColorFromNV12(inputY, x - 1, y - 1, width, height, pitch,surfaceHeight)) * (fy1 * fx1);
	sum += convert_float4(GetColorFromNV12(inputY, x , y - 1, width, height, pitch,surfaceHeight)) * (fy1 * fx2);
	sum += convert_float4(GetColorFromNV12(inputY, x + 1, y - 1, width, height, pitch,surfaceHeight)) * (fy1 * fx3);
	
	sum += convert_float4(GetColorFromNV12(inputY, x - 1, y, width, height, pitch,surfaceHeight)) * (fy2 * fx1);
	sum += convert_float4(GetColorFromNV12(inputY, x , y, width, height, pitch,surfaceHeight)) * (fy2 * fx2);
	sum += convert_float4(GetColorFromNV12(inputY, x + 1, y, width, height, pitch,surfaceHeight)) * (fy2 * fx3);

	sum += convert_float4(GetColorFromNV12(inputY, x - 1, y + 1, width, height, pitch,surfaceHeight)) * (fy3 * fx1);
	sum += convert_float4(GetColorFromNV12(inputY, x , y + 1, width, height, pitch,surfaceHeight)) * (fy3 * fx2);
	sum += convert_float4(GetColorFromNV12(inputY, x + 1, y + 1, width, height, pitch,surfaceHeight)) * (fy3 * fx3);
	
    return (sum / nDenom);
}

//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
uchar4 BicubicNV12(int x, int y, const __global uchar *input, int widthIn, int heightIn, int width, int height, int flipH, int flipV, int angle, int bicubic, int pitch, int surfaceHeight) 
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

	if (flipH == 1)
	{
		posX = widthIn - posX - 1;
	}

	if (flipV == 1)
	{
		posY = heightIn - posY - 1;
	}
	
	uchar4 color;
	if(bicubic)
		color = convert_uchar4(BiCubicNV12(posX, posY, input, widthIn, heightIn,pitch,surfaceHeight));
	else
		color = GetColorFromNV12(input, posX,  posY, widthIn, heightIn, pitch,surfaceHeight);
	
	return color;
} 

//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
__kernel void BicubicNV12toRegardsBitmap(__global uchar4 *output, const __global uchar *input, int widthIn, int heightIn, int widthOut, int heightOut, int flipH, int flipV, int angle, int bicubic, int pitch, int surfaceHeight) 
{ 
    int x = get_global_id(0);
	int y = get_global_id(1);
	int position = x + y * widthOut;

	output[position]  = BicubicNV12(x, y, input, widthIn, heightIn, widthOut, heightOut, flipH, flipV, angle, bicubic, pitch, surfaceHeight);
} 