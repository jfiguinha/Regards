
//----------------------------------------------------
//Conversion en yuv => Récupération de la valeur Y
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
