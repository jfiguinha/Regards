__kernel void PhotoFiltre(__global uchar4 * output, const __global uchar4 *input, int width, int intensity, uchar4 color)
{
    int x = get_global_id(0);
	int y = get_global_id(1);

	int positionDest = x + y * width;

	float coeff = (float)intensity / 100.0f;
	float diff = 1.0f - coeff;

	float4 colorOut = convert_flot4(input[positionSrc]) * (float4)diff + color * coeff;


	output[positionDest] = convert_uchar4(colorOut);
}
