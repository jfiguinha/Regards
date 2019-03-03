//----------------------------------------------------
// Conversion du NV12 vers du 32 bits
//----------------------------------------------------
float4 GetColorFromNV12(const __global uchar * input, int x, int y, int width, int height, int rectWidth, int rectHeight)
{
	if(x < rectWidth && y < rectHeight && y >= 0 && x >= 0)	
	{
		float4 color;
		int crv = 104597;
		int	cbu = 132201;
		int cgu = 25675;  
		int cgv = 53279;
		int positionSrc = x + y * width;
		int positionUV = width * height;

		int yModulo = y % 2;
		int xModulo = x % 2;
		if (xModulo == 1)
		{		
			if (yModulo == 1)
				positionUV = (x - 1) + ((y - 1) / 2) * width + positionUV;
			else
				positionUV = (x - 1) + (y / 2) * width + positionUV;
		}
		else
		{
			if (yModulo == 1)
				positionUV = x + ((y - 1) / 2) * width + positionUV;
			else
				positionUV = x + (y / 2) * width + positionUV;
		}
		
		int v = input[positionUV];
		int u = input[positionUV + 1];

		int c1 = (v-128) * crv;
		int c2 = (u-128) * cgu;
		int c3 = (v-128) * cgv;
		int c4 = (u-128) * cbu;
		
		int y1 = 76309*(input[positionSrc]-16);
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
	return (float)0.0f;
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

float4 BiCubicNV12(float x, float y, const __global uchar *input, int width, int height, int rectWidth, int rectHeight)
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

	
	float4 sum = GetColorFromNV12(input, x - 1, y - 1, width, height, rectWidth, rectHeight) * (fy1 * fx1);
	sum += GetColorFromNV12(input, x , y - 1, width, height, rectWidth, rectHeight) * (fy1 * fx2);
	sum += GetColorFromNV12(input, x + 1, y - 1, width, height, rectWidth, rectHeight) * (fy1 * fx3);
	
	sum += GetColorFromNV12(input, x - 1, y, width, height, rectWidth, rectHeight) * (fy2 * fx1);
	sum += GetColorFromNV12(input, x , y, width, height, rectWidth, rectHeight) * (fy2 * fx2);
	sum += GetColorFromNV12(input, x + 1, y, width, height, rectWidth, rectHeight) * (fy2 * fx3);

	sum += GetColorFromNV12(input, x - 1, y + 1, width, height, rectWidth, rectHeight) * (fy3 * fx1);
	sum += GetColorFromNV12(input, x , y + 1, width, height, rectWidth, rectHeight) * (fy3 * fx2);
	sum += GetColorFromNV12(input, x + 1, y + 1, width, height, rectWidth, rectHeight) * (fy3 * fx3);
	
    return (sum / nDenom);
}


__kernel void BicubicNV12toRGB32(__global float4 *output, const __global uchar *input, int width, int height, int rectWidth, int rectHeight, int widthOut, int heightOut, int flipH, int flipV, int angle) 
{ 
    int x = get_global_id(0);
	int y = get_global_id(1);

	float ratioX = (float)width / (float)widthOut;
	float ratioY = (float)height / (float)heightOut;	

	if(angle == 90 || angle == 270)
	{
		ratioX = (float)height / (float)widthOut;
		ratioY = (float)width / (float)heightOut;
	}
	
	float posX = (float)x * ratioX;
	float posY = (float)y * ratioY;
	
	if(angle == 90)
	{
		int srcx = posY;
		int srcy = height - posX - 1;
		
		posX = srcx;
		posY = srcy;		
	}
	else if(angle == 180)
	{
		posX = width - posX - 1;
		posY = height - posY - 1;
	}
	else if(angle == 270)
	{
		int srcx = width - posY - 1;
		int srcy = posX;

		posX = srcx;
		posY = srcy;		
	}	
		
	if(flipH == 1)
	{
		posX = width - posX - 1;
	}
	
	if(flipV == 0)
	{
		posY = height - posY - 1;
	}
	
	int position = x + y * widthOut;

	output[position] = BiCubicNV12(posX, posY, input, width, height, rectWidth, rectHeight);
} 

//----------------------------------------------------
// Conversion Special Effect Video RGB32
//----------------------------------------------------

float4 GetColorSrc(int x, int y, const __global float4 *input, int widthIn, int heightIn)
{
	if(x < widthIn && y < heightIn && y >= 0 && x >= 0)	
	{
		int position = x + y * widthIn;
		return input[position];
	}
	return 0.0f;
}

float4 BiCubic(float x, float y, const __global float4 *input, int widthIn, int heightIn)
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

	
	float4 sum = GetColorSrc(x - 1, y - 1, input, widthIn, heightIn) * (fy1 * fx1);
	sum += GetColorSrc(x , y - 1, input, widthIn, heightIn) * (fy1 * fx2);
	sum += GetColorSrc(x + 1, y - 1, input, widthIn, heightIn) * (fy1 * fx3);
	
	sum += GetColorSrc(x - 1, y, input, widthIn, heightIn) * (fy2 * fx1);
	sum += GetColorSrc(x , y, input, widthIn, heightIn) * (fy2 * fx2);
	sum += GetColorSrc(x + 1, y, input, widthIn, heightIn) * (fy2 * fx3);

	sum += GetColorSrc(x - 1, y + 1, input, widthIn, heightIn) * (fy3 * fx1);
	sum += GetColorSrc(x , y + 1, input, widthIn, heightIn) * (fy3 * fx2);
	sum += GetColorSrc(x + 1, y + 1, input, widthIn, heightIn) * (fy3 * fx3);
	
    return (sum / nDenom);
}

__kernel void BicubicRGB32(__global float4 * output, const __global float4 * input, int width, int height, int widthOut, int heightOut, int flipH, int flipV, int angle) 
{ 
    int x = get_global_id(0);
	int y = get_global_id(1);
	

	float ratioX = (float)width / (float)widthOut;
	float ratioY = (float)height / (float)heightOut;	

	if(angle == 90 || angle == 270)
	{
		ratioX = (float)height / (float)widthOut;
		ratioY = (float)width / (float)heightOut;
	}
	
	float posX = (float)x * ratioX;
	float posY = (float)y * ratioY;
	
	if(angle == 90)
	{
		int srcx = posY;
		int srcy = posX;

		posX = srcx;
		posY = srcy;

		posX = width - posX - 1;	
	}
	else if(angle == 180)
	{
		posX = width - posX - 1;
		posY = height - posY - 1;
	}
	else if(angle == 270)
	{
		int srcx = posY;
		int srcy = posX;

		posX = srcx;
		posY = srcy;

		posY = height - posY - 1;		
	}	
		
	if(flipH == 1)
	{
		posX = width - posX - 1;
	}
	
	if(flipV == 0)
	{
		posY = height - posY - 1;
	}
	
	int position = x + y * widthOut;

	output[position] = BiCubic(posX, posY, input, width, height);
} 

