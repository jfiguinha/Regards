
//----------------------------------------------------
//Conversion en yuv => Rcupration de la valeur Y
//----------------------------------------------------
__kernel void ConvertToY(__global float * output, const __global float4 * input)
{
	int position = get_global_id(0);

	float seize = 16.0f;
		
	float B = input[position].x * 255.0f;
	float G = input[position].y * 255.0f;
	float R = input[position].z * 255.0f;
	
	output[position] = 0.257f * R + 0.504f * G + 0.098f * B + seize;
}

//----------------------------------------------------
//Conversion en yuv => Rcupration de la valeur Y
//----------------------------------------------------
__kernel void ConvertToYUchar(__global uchar * output, const __global float4 * input)
{
	int position = get_global_id(0);

	float seize = 16.0f;
		
	float B = input[position].x * 255.0f;
	float G = input[position].y * 255.0f;
	float R = input[position].z * 255.0f;
	
	output[position] = 0.257f * R + 0.504f * G + 0.098f * B + seize;
}

//----------------------------------------------------
//Incorporation de la valeur Y
//----------------------------------------------------
__kernel void InsertYValueFromUchar(__global float4 * output, const __global uchar * Yinput, const __global float4 * source)
{
	int position = get_global_id(0);
	float B = source[position].x * 255.0f;
	float G = source[position].y * 255.0f;
	float R = source[position].z * 255.0f;

	float half_parted = 128.0f;
	float seize = 16.0f;
	
	float Y = Yinput[position];
	float V = (0.439 * R) - (0.368 * G) - (0.071 * B) + half_parted;
	float U = -(0.148 * R) - (0.291 * G) + (0.439 * B) + half_parted;

	output[position].x = (1.164 * (Y - seize) + 2.018 * (U - half_parted)) / 255.0f;
	output[position].y = (1.164 * (Y - seize) - 0.813 * (V - half_parted) - 0.391 * (U - half_parted)) / 255.0f;
	output[position].z = (1.164 * (Y - seize) + 1.596 * (V - half_parted)) / 255.0f;	
	
	
	output[position].w = source[position].w;
}

//----------------------------------------------------
//Incorporation de la valeur Y
//----------------------------------------------------
__kernel void InsertYValue(__global float4 * output, const __global float * Yinput, const __global float4 * source)
{
	int position = get_global_id(0);
	float B = source[position].x * 255.0f;
	float G = source[position].y * 255.0f;
	float R = source[position].z * 255.0f;

	float half_parted = 128.0f;
	float seize = 16.0f;
	
	float Y = Yinput[position];
	float V = (0.439 * R) - (0.368 * G) - (0.071 * B) + half_parted;
	float U = -(0.148 * R) - (0.291 * G) + (0.439 * B) + half_parted;

	output[position].x = (1.164 * (Y - seize) + 2.018 * (U - half_parted)) / 255.0f;
	output[position].y = (1.164 * (Y - seize) - 0.813 * (V - half_parted) - 0.391 * (U - half_parted)) / 255.0f;
	output[position].z = (1.164 * (Y - seize) + 1.596 * (V - half_parted)) / 255.0f;	
	
	
	output[position].w = source[position].w;
}

//----------------------------------------------------
//Extraction d'un bloc size de la valeur Y
//----------------------------------------------------
__kernel void ExtractBlocSize(__global float * output, const __global float * input, int size, int marge, int width, int height, int x, int y)
{
    int posX = get_global_id(0);
	int posY = get_global_id(1);

	int posLocal = (posY * (size + marge * 2) + posX);
	
	int inputX = posX + (x * size) - marge;
	int inputY = posY + (y * size) - marge;
	
	if(inputX < width && inputX >= 0 && inputY >= 0 && inputY < height)
		output[posLocal] = input[inputX + inputY * width];
	else
		output[posLocal] = 0.0f;
}

//----------------------------------------------------
//Extraction d'un bloc size de la valeur Y
//----------------------------------------------------
__kernel void InsertBlockSize(__global float * yPicture, const __global float * wiener, int size, int marge, int width, int height, int x, int y)
{
    int posX = get_global_id(0);
	int posY = get_global_id(1);

	int position = (posY * (size + marge * 2) + posX);
	
	int inputX = posX + (x * size) - marge;
	int inputY = posY + (y * size) - marge;
	
	int posMax = size + marge;
	
	if(inputX <  width && inputX >= 0 && inputY >= 0 && inputY < height && posX >= marge && posX < posMax && posY >= marge && posY < posMax)
	{
		int posLocal = inputX + inputY * width;
		yPicture[posLocal] = wiener[position];
	}
}
