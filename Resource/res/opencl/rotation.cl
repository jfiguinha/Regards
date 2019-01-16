int CalculXPos(float x, float y, float angle)
{
	float radians = angle * 0.017453292519943;
	float cosine = cos(radians);
	float sine = sin(radians);
	return convert_int(x * cosine - y * sine);
}

int CalculYPos(float x, float y, float angle)
{
	float radians = angle * 0.017453292519943;
	float cosine = cos(radians);
	float sine = sin(radians);
	return convert_int(x * sine + y * cosine);
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

__kernel void Rotation270(__global float4 * output, const __global float4 *input, int widthIn, int heightIn, int widthOut, int heightOut, float angle)
{
    int x = get_global_id(0);
	int y = get_global_id(1);

	int srcx = widthIn - y - 1;
	int srcy = x;
	
	int positionSrc = srcx + srcy * widthIn;
	int positionDest = x + y * widthOut;

	output[positionDest] = input[positionSrc];
}

__kernel void Rotation90(__global float4 * output, const __global float4 *input, int widthIn, int heightIn, int widthOut, int heightOut, float angle)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	
	int srcx = y;
	int srcy = heightIn - x - 1;
	
	int positionSrc = srcx + srcy * widthIn;
	int positionDest = x + y * widthOut;

	output[positionDest] = input[positionSrc];
}

__kernel void RotateFree(__global float4 * output, const __global float4 *input, int widthIn, int heightIn, int widthOut, int heightOut, float angle)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	
	int depx = (widthIn - widthOut) / 2;
	int depy = (heightIn - heightOut) / 2;
	
	int origineX = widthIn / 2;
	int origineY = heightIn / 2;
	
	int newX = x - origineX + depx;
	int newY = y - origineY + depy;
	
	int sourcex = CalculXPos(newX, newY, angle) + origineX;
	int sourcey = CalculYPos(newX, newY, angle) + origineY;
	
	int positionDest = x + y * widthOut;
	output[positionDest] = GetColorSrc(sourcex, sourcey, input, widthIn, heightIn);
}
