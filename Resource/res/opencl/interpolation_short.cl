//Implementation de l'interpolation bicubic en OpenCL
uchar4 GetColorSrc(int x, int y, const __global uchar4 *input, int widthIn, int heightIn)
{
	if(x < widthIn && y < heightIn && y >= 0 && x >= 0)	
	{
		int position = x + y * widthIn;
		return input[position];
	}
	return 0;
}


//----------------------------------------------------------------------------
//Fast Interpolation
//----------------------------------------------------------------------------
__kernel void FastInterpolation(__global uchar4 *output, const __global uchar4 *input, int widthIn, int heightIn, int widthOut, int heightOut, int flipH, int flipV, int angle)
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

	output[position] = GetColorSrc(posX, posY, input, widthIn, heightIn);

}

__kernel void FastInterpolationZone(__global uchar4 *output, const __global uchar4 *input, int widthIn, int heightIn, int widthOut, int heightOut, float left, float top, float bitmapWidth, float bitmapHeight, int flipH, int flipV, int angle)
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
	output[position] = GetColorSrc(posX, posY, input, widthIn, heightIn);
}