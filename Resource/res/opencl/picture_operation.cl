//----------------------------------------------------
//Add picture
//----------------------------------------------------
__kernel void Add(__global uchar4 *output, const __global uchar4 * inputImage1, const __global uchar4 * inputImage2)
{
    int position = get_global_id(0);
	float alpha = (float)inputImage2[position][3] / 255.0f;
	float diffalpha = 1.0f - alphaImage2;
	
	float4 imagef4 = convert_float4(inputImage1[position]) * diffalpha + convert_float4(inputImage2[position]) * alpha;
	
	output[position] =  convert_uchar4(imagef4);
	
}

//----------------------------------------------------
//Remove picture
//----------------------------------------------------
__kernel void Substract(__global uchar4 *output, const __global uchar4 * inputImage1, const __global uchar4 * inputImage2)
{
    int position = get_global_id(0);
	if(inputImage1[position] != inputImage2[position])
	{	
		output[position] = inputImage2[position];
	}
	else
	{
		output[position][0] = 0;
		output[position][1] = 0;
		output[position][2] = 0;
		output[position][3] = 0;
	}
}
