//----------------------------------------------------
//Conversion d'un cximage en regards bitmap
//----------------------------------------------------
__kernel void LoadCxImageAlpha(__global float * output,const __global uchar * input, const __global uchar * alpha, int width, int height, int effwidth)
{
	int position = get_global_id(0);
	
	int y = position / width;
	int x = position - (y * width);	
	
	int positionSource = effwidth * y + x * 3;
	
	output[position * 4]   = (float)input[positionSource] / 255.0f;
	output[position * 4+1] = (float)input[positionSource + 1] / 255.0f;
	output[position * 4+2] = (float)input[positionSource + 2] / 255.0f;
	output[position * 4+3] = (float)alpha[position] / 255.0f;
}

__kernel void LoadCxImageAlphaRGB(__global float * output,const __global uchar * input, const __global uchar * alpha, int width, int height, int effwidth)
{
	int position = get_global_id(0);
	
	int y = position / width;
	int x = position - (y * width);	
	
	int positionSource = effwidth * y + x * 3;	
	
	output[position * 4]   = (float)input[positionSource + 2] / 255.0f;
	output[position * 4+1] = (float)input[positionSource + 1] / 255.0f;
	output[position * 4+2] = (float)input[positionSource] / 255.0f;
	output[position * 4+3] = (float)alpha[position] / 255.0f;
}

//----------------------------------------------------
//Conversion d'un cximage en regards bitmap
//----------------------------------------------------
__kernel void LoadCxImage(__global float *output,const __global uchar *input, int width, int height, int effwidth)
{
	int position = get_global_id(0);
	
	int y = position / width;
	int x = position - (y * width);	
	
	int positionSource = effwidth * y + x * 3;		
	
	output[position * 4]   = (float)input[positionSource] / 255.0f;
	output[position * 4+1] = (float)input[positionSource + 1] / 255.0f;
	output[position * 4+2] = (float)input[positionSource + 2] / 255.0f;
}


//----------------------------------------------------
//Conversion d'un cximage en regards bitmap
//----------------------------------------------------
__kernel void LoadCxImageRGB(__global float *output,const __global uchar *input, int width, int height, int effwidth)
{
	int position = get_global_id(0);
	
	int y = position / width;
	int x = position - (y * width);	
	
	int positionSource = effwidth * y + x * 3;		
	
	output[position * 4]   = (float)input[positionSource + 2] / 255.0f;
	output[position * 4+1] = (float)input[positionSource + 1] / 255.0f;
	output[position * 4+2] = (float)input[positionSource] / 255.0f;
}

//----------------------------------------------------
//Conversion d'un wximage en regards bitmap
//----------------------------------------------------
__kernel void LoadWxImageAlpha(__global float * output,const __global uchar * input, const __global uchar * alpha, int width, int height, int effwidth)
{
	int position = get_global_id(0);
	int y = position / width;
	int x = position - (y * width);		
	
	int positionSource = (height - y - 1) * effwidth + x * 3;
	
	output[position * 4]   = (float)input[positionSource + 2] / 255.0f;
	output[position * 4+1] = (float)input[positionSource + 1]/ 255.0f;
	output[position * 4+2] = (float)input[positionSource]/ 255.0f;
	output[position * 4+3] = (float)alpha[position]/ 255.0f;
}

//----------------------------------------------------
//Conversion d'un cximage en regards bitmap
//----------------------------------------------------
__kernel void LoadWxImage(__global float *output,const __global uchar *input, int width, int height, int effwidth)
{
	int position = get_global_id(0);
	
	int y = position / width;
	int x = position - (y * width);		
	
	int positionSource = (height - y - 1) * effwidth + x * 3;
	
	output[position * 4]   = (float)input[positionSource + 2]/ 255.0f;
	output[position * 4+1] = (float)input[positionSource + 1]/ 255.0f;
	output[position * 4+2] = (float)input[positionSource]/ 255.0f;
}
