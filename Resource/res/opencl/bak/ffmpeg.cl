//----------------------------------------------------
// Conversion du NV12 vers du 32 bits
//----------------------------------------------------
float4 GetColorFromYUV(const __global uchar *inputY, const __global uchar *inputU, const __global uchar *inputV, int x, int y, int width, int height, int pitch)
{
	if(x < width && y < height && y >= 0 && x >= 0)	
	{
		float4 color;
		int crv = 104597;
		int	cbu = 132201;
		int cgu = 25675;  
		int cgv = 53279;
		int positionSrc = x + y * pitch;
		int positionUV = 0;
		int yModulo = y % 2;
		int xModulo = x % 2;
		if (xModulo == 1)
		{		
			if (yModulo == 1)
				positionUV = ((x - 1) / 2) + ((y - 1) / 2) * (pitch / 2);
			else
				positionUV = ((x - 1) / 2) + (y / 2) * (pitch / 2);
		}
		else
		{
			if (yModulo == 1)
				positionUV = (x / 2) + ((y - 1) / 2) * (pitch / 2);
			else
				positionUV = (x / 2) + (y / 2) * (pitch / 2);
		}
		
		int v = inputU[positionUV];
		int u = inputV[positionUV];

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
		
		color.x = (float)r / 255.0f;
		color.y = (float)g / 255.0f;
		color.z = (float)b / 255.0f;
		color.w = 1.0f;
		
		return color;
	}
	return 0.0f;
}

float4 BiCubicYUV(float x, float y, const __global uchar *inputY, const __global uchar *inputU, const __global uchar *inputV, int width, int height, int pitch)
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

	
	float4 sum = GetColorFromYUV(inputY, inputU, inputV, x - 1, y - 1, width, height, pitch) * (fy1 * fx1);
	sum += GetColorFromYUV(inputY, inputU, inputV, x , y - 1, width, height, pitch) * (fy1 * fx2);
	sum += GetColorFromYUV(inputY, inputU, inputV, x + 1, y - 1, width, height, pitch) * (fy1 * fx3);
	
	sum += GetColorFromYUV(inputY, inputU, inputV, x - 1, y, width, height, pitch) * (fy2 * fx1);
	sum += GetColorFromYUV(inputY, inputU, inputV, x , y, width, height, pitch) * (fy2 * fx2);
	sum += GetColorFromYUV(inputY, inputU, inputV, x + 1, y, width, height, pitch) * (fy2 * fx3);

	sum += GetColorFromYUV(inputY, inputU, inputV, x - 1, y + 1, width, height, pitch) * (fy3 * fx1);
	sum += GetColorFromYUV(inputY, inputU, inputV, x , y + 1, width, height, pitch) * (fy3 * fx2);
	sum += GetColorFromYUV(inputY, inputU, inputV, x + 1, y + 1, width, height, pitch) * (fy3 * fx3);
	
    return (sum / nDenom);
}

//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
float4 ExecuteBicubicYUV(int x, int y, const __global uchar *inputY, const __global uchar *inputU, const __global uchar *inputV, int widthIn, int heightIn, int width, int height, int flipH, int flipV, int angle, int bicubic, int pitch) 
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
	
	float4 color;
	if(bicubic)
		color = BiCubicYUV(posX, posY, inputY, inputU, inputV, widthIn, heightIn, pitch);
	else
		color = GetColorFromYUV(inputY, inputU, inputV, posX,  posY, widthIn, heightIn, pitch);
	
	return color;
} 

//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
__kernel void BicubicYUVtoRegardsBitmap(__global float4 *output, const __global uchar *inputY, const __global uchar *inputU, const __global uchar *inputV, int widthIn, int heightIn, int widthOut, int heightOut, int flipH, int flipV, int angle, int bicubic, int pitch) 
{ 
    int x = get_global_id(0);
	int y = get_global_id(1);
	int position = x + y * widthOut;

	output[position]  = ExecuteBicubicYUV(x, y, inputY, inputU, inputV, widthIn, heightIn, widthOut, heightOut, flipH, flipV, angle, bicubic, pitch);
} 
