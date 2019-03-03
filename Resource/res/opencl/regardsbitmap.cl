//----------------------------------------------------
//Conversion d'un regards bitmap en floatant
//----------------------------------------------------
__kernel void LoadRegardsBitmap(__global float4 * output,const __global uchar4 * input, int width, int height)
{
	int position = get_global_id(0);
	float4 divisor = 255.0f;
	output[position] = convert_float4(input[position]) / divisor;
}

//----------------------------------------------------
//Conversion d'un regards bitmap en floatant
//----------------------------------------------------
__kernel void GetRegardsBitmap(__global uchar4 * output,const __global float4 * input, int width, int height)
{
	int position = get_global_id(0);
	float4 multiplicator = 255.0f;
	float4 value = input[position] * multiplicator;
	output[position] = convert_uchar4(value);
}