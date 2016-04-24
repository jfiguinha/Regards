typedef struct
{
	int red;
	int green;
	int blue;
	int alpha;
}COLORData;

//----------------------------------------------------
//Filtre Niveau de gris
//----------------------------------------------------
__kernel void GrayLevel(__global uchar4 *output,const __global uchar4 *input)
{
    int position = get_global_id(0);
	uchar average = (input[position].x * 0.299) + (input[position].y * 0.587) + (input[position].z * 0.114);
	output[position].x = average;
	output[position].y = average;
	output[position].z = average;
	output[position].w = input[position].w;
}

//----------------------------------------------------
//Filtre Niveau de gris
//----------------------------------------------------
__kernel void Negatif(__global uchar4 *output,const __global uchar4 *input)
{
    int position = get_global_id(0);
	uchar4 diffValue = (uchar4)255;
	output[position] = diffValue - input[position];
}

//----------------------------------------------------
//Filtre Niveau de gris
//----------------------------------------------------
__kernel void NoirEtBlanc(__global uchar4 *output,const __global uchar4 *input)
{
    int position = get_global_id(0);
	int sum = (input[position].x * 0.299) + (input[position].y * 0.587) + (input[position].z * 0.114);
	if(sum > 128)
		sum = 255;
	else
		sum = 0.0;
	output[position].x = sum;
	output[position].y = sum;
	output[position].z = sum;
	output[position].w = input[position].w;
}

//----------------------------------------------------
//Filtre Niveau de gris
//----------------------------------------------------
__kernel void Sepia(__global uchar4 *output,const __global uchar4 *input)
{
	int position = get_global_id(0);
	float m_iDiff = 255.0f - 172.0f;
	float sum = ((input[position].x * 0.299) + (input[position].y * 0.587) + (input[position].z * 0.114)) / 255.0f;
	int r = convert_int(sum * (172.0f + m_iDiff));
	int g = convert_int(sum * (122.0f + m_iDiff));
	int b = convert_int(sum * (51.0f + m_iDiff));
	output[position].x = min(b, 255);
	output[position].y = min(g, 255);
	output[position].z = min(r, 255);	
	output[position].w = input[position].w;
}

//----------------------------------------------------
//Filtre Photo Filtre
//----------------------------------------------------
__kernel void PhotoFiltre(__global uchar4 *output,const __global uchar4 *input, int intensity, const __global COLORData * color)
{
    int position = get_global_id(0);
	
	float coeff = (float)intensity / 100.0f;
	float diff = 1.0f - coeff;

	float4 src_color = convert_float4(input[position]);
	uchar red = color->blue * coeff + input[position].x * diff;
	uchar green = color->green * coeff + input[position].y * diff;
	uchar blue = color->red * coeff + input[position].z * diff;
	
	output[position].x = red;
	output[position].y = green;
	output[position].z = blue;
	output[position].w = input[position].w;

}

//----------------------------------------------------
//Filtre RGB Filtre
//----------------------------------------------------
__kernel void RGBFiltre(__global uchar4 *output,const __global uchar4 *input, const __global COLORData * color)
{
    int position = get_global_id(0);
	
	int red = color->blue + input[position].x;
	red = min(max(red ,0), 255);
	
	int green = color->green + input[position].y;
	green = min(max(green ,0), 255);
	
	int blue = color->red + input[position].z;
	blue = min(max(blue ,0), 255);
	
	output[position].x = red;
	output[position].y = green;
	output[position].z = blue;
	output[position].w = input[position].w;
}

//----------------------------------------------------
//Filtre Posterization
//----------------------------------------------------
__kernel void Posterisation(__global uchar4 *output,const __global uchar4 *input, int level)
{
    int position = get_global_id(0);
	
	int _levels = max(2, min(16, level));
	int _offset = 256 / _levels;
	
	int red = input[position].x / _offset;
	int green = input[position].y / _offset;
	int blue = input[position].z / _offset;
	
	output[position].x = red * _offset;
	output[position].y = green * _offset;
	output[position].z = blue * _offset;
	output[position].w = input[position].w;
	
}

//----------------------------------------------------
//Filtre Solarization
//----------------------------------------------------
__kernel void Solarization(__global uchar4 *output,const __global uchar4 *input, int threshold)
{
    int position = get_global_id(0);
	
	int red = input[position].x;
	int green = input[position].y;
	int blue = input[position].z;
	
	if (red > threshold)
		output[position].x = 255 - red;
	else
		output[position].x = red;
		
	if (green > threshold)
		output[position].y = 255 - green;
	else
		output[position].y = green;

	if (blue > threshold)
		output[position].z = 255 - blue;
	else
		output[position].z = blue;

	output[position].w = input[position].w;
	
}


//----------------------------------------------------
//Filtre Lumière et Contraste
//----------------------------------------------------
int ConstrastFilter(int color, float contrast, float offset)
{
	float csupp = contrast * (offset - 256.0f) + 128.0f;
	float value = (float)color * contrast + csupp;
	
	int returnValue = max(min((int)value,255),0);
	
	return returnValue;
}

int LightFilter(int color, float factor)
{
	float nChange = 2.55f * factor;

	if (nChange > 255.0f)
		nChange = 255.0f;
	else if (nChange < -255.0f)
		nChange = -255.0f;
		
	int returnValue = color + (int)nChange;
	
	return max(min(returnValue,255),0);
}

__kernel void LightAndContrast(__global uchar4 *output,const __global uchar4 *input, float contrast, float light)
{
    int position = get_global_id(0);
	
	uchar4 color;
	uchar4 src_color = input[position];
	
	int constrastValue = (int)contrast;
	
	if (constrastValue == 0)
	{
		color.x = ConstrastFilter(src_color.x,1.0f, 128.0f);
		color.y = ConstrastFilter(src_color.y,1.0f, 128.0f);
		color.z = ConstrastFilter(src_color.z,1.0f, 128.0f);
	}
	else if (constrastValue > 0)
	{
		float offset = contrast;
		offset /= 100.0f;
		offset = 1.0f + offset;
		color.x = ConstrastFilter(src_color.x,offset, 128.0f);
		color.y = ConstrastFilter(src_color.y,offset, 128.0f);
		color.z = ConstrastFilter(src_color.z,offset, 128.0f);
	}
	else if (constrastValue < 0)
	{
		float offset = -contrast;
		offset /= 100.0f;
		offset = 1.0f - offset;
		color.x = ConstrastFilter(src_color.x,offset, 128.0f);
		color.y = ConstrastFilter(src_color.y,offset, 128.0f);
		color.z = ConstrastFilter(src_color.z,offset, 128.0f);
	}

	color.x = LightFilter(color.x, light);
	color.y = LightFilter(color.y, light);
	color.z = LightFilter(color.z, light);	
	color.w = src_color.w;
	
	output[position] = color;
	
	/*
	output[position].x = src_color[0];
	output[position].y = src_color[1];
	output[position].z = src_color[2];
	output[position].w = input[position].w;
	*/
}
