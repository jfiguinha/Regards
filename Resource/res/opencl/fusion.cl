__kernel void Fusion(__global uchar4 * output, const __global uchar4 *input, const __global uchar4 *inputSecond, float pourcentage)
{
	int position = get_global_id(0);
	float4 prcent = (float4)pourcentage;
	float4 diff = (float4)1.0f - (float4)pourcentage;
	output[position] = convert_uchar4(convert_float4(input[position]) * diff + convert_float4(inputSecond[position]) * prcent);
}
