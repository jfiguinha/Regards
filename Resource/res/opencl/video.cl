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

float4 GetColorSrc(int x, int y, const __global uchar4 *input, int widthIn, int heightIn, int rectWidth, int rectHeight)
{
	if(x < rectWidth && y < rectHeight && y >= 0 && x >= 0)	
	{
		int position = x + y * widthIn;
		return convert_float4(input[position]);
	}
	return 0.0f;
}

float4 BiCubic(float x, float y, const __global uchar4 *input, int widthIn, int heightIn, int rectWidth, int rectHeight)
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

	
	float4 sum = GetColorSrc(x - 1, y - 1, input, widthIn, heightIn, rectWidth, rectHeight) * (fy1 * fx1);
	sum += GetColorSrc(x , y - 1, input, widthIn, heightIn, rectWidth, rectHeight) * (fy1 * fx2);
	sum += GetColorSrc(x + 1, y - 1, input, widthIn, heightIn, rectWidth, rectHeight) * (fy1 * fx3);
	
	sum += GetColorSrc(x - 1, y, input, widthIn, heightIn, rectWidth, rectHeight) * (fy2 * fx1);
	sum += GetColorSrc(x , y, input, widthIn, heightIn, rectWidth, rectHeight) * (fy2 * fx2);
	sum += GetColorSrc(x + 1, y, input, widthIn, heightIn, rectWidth, rectHeight) * (fy2 * fx3);

	sum += GetColorSrc(x - 1, y + 1, input, widthIn, heightIn, rectWidth, rectHeight) * (fy3 * fx1);
	sum += GetColorSrc(x , y + 1, input, widthIn, heightIn, rectWidth, rectHeight) * (fy3 * fx2);
	sum += GetColorSrc(x + 1, y + 1, input, widthIn, heightIn, rectWidth, rectHeight) * (fy3 * fx3);
	
	return (sum / nDenom);
}

//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
__kernel void BicubicRGB32Video(__global uchar4 *output, const __global uchar *input, int width, int height, int widthOut, int heightOut, int flipH, int flipV, int angle) 
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

	output[position] = convert_uchar4(BiCubic(posX, posY, input, width, height, rectWidth, rectHeight));
}

//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
__kernel void BicubicVideo(__global uchar4 *output, const __global uchar *input, int width, int height, int rectWidth, int rectHeight, int widthOut, int heightOut, int flipH, int flipV, int angle) 
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

	output[position] = convert_uchar4(BiCubic(posX, posY, input, width, height, rectWidth, rectHeight));
} 