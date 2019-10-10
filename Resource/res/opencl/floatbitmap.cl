//----------------------------------------------------
//Conversion d'un regards bitmap en floatant
//----------------------------------------------------
__kernel void LoadFloatBitmap(__global float4 * output,const __global float4 * input, int width, int height)
{
	int position = get_global_id(0);
	output[position] = input[position];
}

//----------------------------------------------------
//Conversion d'un regards bitmap en floatant
//----------------------------------------------------
__kernel void GetFloatBitmap(__global float4 * output,const __global float4 * input, int width, int height)
{
	int position = get_global_id(0);
	output[position] = input[position];
}