__kernel void Fusion(__global float4 * output, const __global float4 *input, const __global float4 *inputSecond, float pourcentage)
{
	int position = get_global_id(0);
	float4 prcent = pourcentage;
	float4 diff = 1.0f - pourcentage;
	output[position] = input[position] * diff + inputSecond[position] * prcent;
}
