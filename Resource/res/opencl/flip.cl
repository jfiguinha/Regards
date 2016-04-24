
__kernel void FlipVertical(__global uchar4 * output, const __global uchar4 *input, int widthIn, int heightIn, int widthOut, int heightOut)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	int srcy = heightIn - y - 1;
	int positionSrc = x + srcy * widthIn;
	int positionDest = x + y * widthOut;
	output[positionDest] = input[positionSrc];
}

__kernel void FlipHorizontal(__global uchar4 * output, const __global uchar4 *input, int widthIn, int heightIn, int widthOut, int heightOut)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	int srcx = widthIn - x - 1;
	int positionSrc = srcx + y * widthIn;
	int positionDest = x + y * widthOut;
	output[positionDest] = input[positionSrc];
}

