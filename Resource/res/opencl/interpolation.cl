//Implementation de l'interpolation bicubic en OpenCL

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



float4 GetColorSrc(int x, int y, const __global float4 *input, int widthIn, int heightIn)
{
	if(x < widthIn && y < heightIn && y >= 0 && x >= 0)	
	{
		int position = x + y * widthIn;
		return input[position];
	}
	return 0.0f;
}

float4 Bilinear(float x, float y, const __global float4 *input, int widthIn, int heightIn)
{
	int valueA = (int)x;
	int valueB = (int)y;
	
	float4 p0q0  = GetColorSrc(valueA, valueB, input,widthIn,heightIn) ;
	float4 p1q0 = GetColorSrc(valueA + 1, valueB, input,widthIn,heightIn);
	float4 p0q1  = GetColorSrc(valueA, valueB + 1, input,widthIn,heightIn) ;
	float4 p1q1 = GetColorSrc(valueA + 1, valueB + 1, input,widthIn,heightIn);
	
	// Calculate the weights for each pixel  
	float fx = x - valueA;
	float fy = y - valueB;
	float fx1 = 1.0f - fx;  
	float fy1 = 1.0f - fy;

	float w1 = fx1 * fy1;
	float w2 = fx  * fy1;
	float w3 = fx1 * fy;
	float w4 = fx  * fy; 	

	// Calculate the weighted sum of pixels (for each color channel) 
	return (p0q0 * (float4)w1 + p1q0 * (float4)w2 + p0q1 * (float4)w3 + p1q1 * (float4)w4);
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

float4 BiCubicWithWeight(int posX, int posY, float x, float y, const __global float4 *input, int widthIn, int heightIn, const __global float * wWidth, const __global float * wHeight)
{
	int poswWidth = posX * 3;
	int poswHeight = posY * 3;
	float4 fy1 = wHeight[poswHeight];
	float4 fy2 = wHeight[poswHeight + 1];
	float4 fy3 = wHeight[poswHeight + 2];
	
	float4 fx1 = wWidth[poswWidth];
	float4 fx2 = wWidth[poswWidth + 1];
	float4 fx3 = wWidth[poswWidth + 2];
	
	float4 nDenom = fy1 * (fx1 + fx2 + fx3) + fy2 * (fx1 + fx2 + fx3) + fy3 * (fx1 + fx2 + fx3);

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

//----------------------------------------------------------------------------
//Bicubic Interpolation
//----------------------------------------------------------------------------
__kernel void BicubicInterpolation(__global float4 *output, const __global float4 *input, int widthIn, int heightIn, int widthOut, int heightOut, int flipH, int flipV, int angle)
{
	int width = widthOut;
	int height = heightOut;

    int x = get_global_id(0);
	int y = get_global_id(1);

	float ratioX = (float)widthIn / (float)width;
	float ratioY = (float)heightIn / (float)height;
	if (angle == 90 || angle == 270)
	{
		ratioX = (float)widthIn / (float)height;
		ratioY = (float)heightIn / (float)width;
	}

	float posY = (float)y * ratioY;
	float posX = (float)x * ratioX;

	if (angle == 270)
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
	else if (angle == 90)
	{
		int srcx = posY;
		int srcy = posX;

		posX = srcx;
		posY = srcy;

		posY = heightIn - posY - 1;
	}
	
	if(angle == 90 || angle == 270)
	{
		if (flipV == 1)
		{
			posX = widthIn - posX - 1;
		}

		if (flipH == 1)
		{
			posY = heightIn - posY - 1;
		}
	
	}
	else
	{
		if (flipH == 1)
		{
			posX = widthIn - posX - 1;
		}

		if (flipV == 1)
		{
			posY = heightIn - posY - 1;
		}
	}

	int position = x + y * widthOut;

	output[position] = BiCubic(posX, posY, input, widthIn, heightIn);

}

__kernel void BicubicInterpolationZone(__global float4 *output, const __global float4 *input, int widthIn, int heightIn, int widthOut, int heightOut, float left, float top, float bitmapWidth, float bitmapHeight, int flipH, int flipV, int angle)
{
    int x = get_global_id(0);
	int y = get_global_id(1);

	float ratioX = (float)widthIn / bitmapWidth;
	float ratioY = (float)heightIn / bitmapHeight;
	if (angle == 90 || angle == 270)
	{
		ratioX = (float)widthIn / (float)bitmapHeight;
		ratioY = (float)heightIn / (float)bitmapWidth;
	}

	float posX = (float)x * ratioX + left * ratioX;
	float posY = (float)y * ratioY + top * ratioY;

	if (angle == 270)
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
	else if (angle == 90)
	{
		int srcx = posY;
		int srcy = posX;

		posX = srcx;
		posY = srcy;

		posY = heightIn - posY - 1;
	}

	if(angle == 90 || angle == 270)
	{
		if (flipV == 1)
		{
			posX = widthIn - posX - 1;
		}

		if (flipH == 1)
		{
			posY = heightIn - posY - 1;
		}
	
	}
	else
	{
		if (flipH == 1)
		{
			posX = widthIn - posX - 1;
		}

		if (flipV == 1)
		{
			posY = heightIn - posY - 1;
		}
	}
		
	int position = x + y * widthOut;

	output[position] = BiCubic(posX, posY, input, widthIn, heightIn);
}

//----------------------------------------------------------------------------
//Bilinear Interpolation
//----------------------------------------------------------------------------
__kernel void BilinearInterpolation(__global float4 *output, const __global float4 *input, int widthIn, int heightIn, int widthOut, int heightOut)
{
    int x = get_global_id(0);
	int y = get_global_id(1);

	float ratioX = (float)widthIn / (float)widthOut;
	float ratioY = (float)heightIn / (float)heightOut;	
		
	float posX = (float)x * ratioX;
	float posY = (float)y * ratioY;

	int position = x + y * widthOut;
	
	output[position] = Bilinear(posX, posY, input, widthIn, heightIn);

}

__kernel void BilinearInterpolationZone(__global float4 *output, const __global float4 *input, int widthIn, int heightIn, int widthOut, int heightOut, float left, float top, float bitmapWidth, float bitmapHeight)
{
    int x =  get_global_id(0);
	int y =  get_global_id(1);
		
	float ratioX = (float)widthIn / (float)bitmapWidth;
	float ratioY = (float)heightIn / (float)bitmapHeight;	
		
	float posX = (float)x * ratioX + (float)left * ratioX;
	float posY = (float)y * ratioY + (float)top * ratioY;

	int position = x + y *widthOut;
	
	output[position] = Bilinear(posX, posY, input, widthIn, heightIn);
}

//----------------------------------------------------------------------------
//Fast Interpolation
//----------------------------------------------------------------------------
__kernel void FastInterpolation(__global float4 *output, const __global float4 *input, int widthIn, int heightIn, int widthOut, int heightOut)
{
    int x = get_global_id(0);
	int y = get_global_id(1);

	float ratioX = (float)widthIn / (float)widthOut;
	float ratioY = (float)heightIn / (float)heightOut;	
		
	float posX = (float)x * ratioX;
	float posY = (float)y * ratioY;

	int position = x + y * widthOut;
	int positionIn = (int)posX + (int)posY *widthIn;

	output[position] = input[positionIn];

}

__kernel void FastInterpolationZone(__global float4 *output, const __global float4 *input, int widthIn, int heightIn, int widthOut, int heightOut, float left, float top, float bitmapWidth, float bitmapHeight)
{
    int x =  get_global_id(0);
	int y =  get_global_id(1);
		
	float ratioX = (float)widthIn / (float)bitmapWidth;
	float ratioY = (float)heightIn / (float)bitmapHeight;	
		
	float posX = (float)x * ratioX + (float)left * ratioX;
	float posY = (float)y * ratioY + (float)top * ratioY;

	int position = x + y * widthOut;
	int positionIn = (int)posX + (int)posY * widthIn;

	output[position] = input[positionIn];
}