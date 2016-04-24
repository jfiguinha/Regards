uchar4 GetColorSrc(int x, int y, const __global uchar4 *input, int widthIn, int heightIn)
{
	if(x < widthIn && y < heightIn && y >= 0 && x >= 0)	
	{
		int position = x + y * widthIn;
		return input[position];
	}
	return (uchar4)0;
}

__kernel void Erode(__global uchar4 *output, const __global uchar4 *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	uchar4 maxValue = (uchar4)0;

	for (int n = -1; n < 2; n++)
	{
		int posY = y + n;
		for (int m = -1; m < 2; m++)
		{
			int posX = x + m;				
			maxValue = max(GetColorSrc(posX, posY, input, width, height) , maxValue);
		}
	}
	
	int position = x + y * width;

	output[position] = maxValue;
}

__kernel void Dilate(__global uchar4 *output, const __global uchar4 *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	uchar4 minValue = (uchar4)255;

	for (int n = -1; n < 2; n++)
	{
		int posY = y + n;
		for (int m = -1; m < 2; m++)
		{
			int posX = x + m;				
			minValue = min(GetColorSrc(posX, posY, input, width, height) , minValue);
		}
	}
	
	int position = x + y * width;

	output[position] = minValue;
}