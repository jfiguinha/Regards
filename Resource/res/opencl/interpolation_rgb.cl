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



float3 GetColorSrc(int x, int y, const __global uchar3 *input, int widthIn, int heightIn)
{
	if(x < widthIn && y < heightIn && y >= 0 && x >= 0)	
	{
		int position = x + y * widthIn;
		return convert_float3(input[position]);
	}
	return 0.0f;
}

float3 Bilinear(float x, float y, const __global uchar3 *input, int widthIn, int heightIn)
{
	int valueA = (int)x;
	int valueB = (int)y;
	
	float3 p0q0  = GetColorSrc(valueA, valueB, input,widthIn,heightIn) ;
	float3 p1q0 = GetColorSrc(valueA + 1, valueB, input,widthIn,heightIn);
	float3 p0q1  = GetColorSrc(valueA, valueB + 1, input,widthIn,heightIn) ;
	float3 p1q1 = GetColorSrc(valueA + 1, valueB + 1, input,widthIn,heightIn);
	
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
	return (p0q0 * (float3)w1 + p1q0 * (float3)w2 + p0q1 * (float3)w3 + p1q1 * (float3)w4);
}



float3 BiCubic(float x, float y, const __global uchar3 *input, int widthIn, int heightIn)
{
	float3 nDenom = 0.0f;
	int valueA = (int)x;
	int valueB = (int)y;
	float realA = x - valueA;
	float realB = y - valueB;
	
	float3 fy1 = Cubic(-(-1.0f - realB));
	float3 fy2 = Cubic(realB);
	float3 fy3 = Cubic(-(1.0f - realB));
	
	float3 fx1 = Cubic(-1.0f - realA);
	float3 fx2 = Cubic(- realA);
	float3 fx3 = Cubic(1.0f - realA);
	
	nDenom += fy1 * (fx1 + fx2 + fx3) + fy2 * (fx1 + fx2 + fx3) + fy3 * (fx1 + fx2 + fx3);

	
	float3 sum = GetColorSrc(x - 1, y - 1, input, widthIn, heightIn) * (fy1 * fx1);
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


__kernel void BicubicInterpolation(__global uchar3 *output, const __global uchar3 *input, int widthIn, int heightIn, int widthOut, int heightOut)
{
    int x = get_global_id(0);
	int y = get_global_id(1);

	float ratioX = (float)widthIn / (float)widthOut;
	float ratioY = (float)heightIn / (float)heightOut;	
		
	float posX = (float)x * ratioX;
	float posY = (float)y * ratioY;

	int position = x + y * widthOut;

	output[position] = convert_uchar3(BiCubic(posX, posY, input, widthIn, heightIn));

}
