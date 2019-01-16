__kernel void wavelet_denoise_firstpass(__global float *output, const __global float * input, int width, int height, int lpass, int hpass, int sc)
{
    int col = get_global_id(0);
	int row = get_global_id(1);
	int inPosition = hpass+row*width;
	int outPosition = lpass + row*width + col;

	if(col < sc)
		output[outPosition] = 0.25 * (2.0f * input[inPosition + col] + input[inPosition + (sc-col)] + input[inPosition + (col+sc)]);
	else if((col + sc) < width)
		output[outPosition] = 0.25 * (2.0f * input[inPosition +col] + input[inPosition + (col-sc)] + input[inPosition + (col+sc)]);
	else
		output[outPosition] = 0.25 * (2.0f * input[inPosition +col] + input[inPosition + (col-sc)] + input[inPosition + (2*iwidth-2-(col+sc))]);
		
}

__kernel void wavelet_denoise_secondpass(__global float *output, const __global float * input, int width, int height, int lpass, int hpass, int sc)
{
    int col = get_global_id(0);
	int row = get_global_id(1);
	int inPosition = lpass+col;
	int outPosition = lpass + row*width + col;

	if(row < sc)
		fimg[outPosition] = 0.25 * (2.0f*input[inPosition + width*row] + input[inPosition + width*(sc-row)] + input[inPosition + width*(row+sc)]);
	else if((row+sc) < height)
		fimg[outPosition] = 0.25 * (2.0f*input[inPosition + width*row] + input[inPosition + width*(row-sc)] + input[inPosition + width*(row+sc)]);
	else
		fimg[outPosition] = 0.25 * (2.0f*input[inPosition + width*row] + input[inPosition + width*(row-sc)] + input[inPosition + iwidth*(2*iheight-2-(row+sc))]);
		
}