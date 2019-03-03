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
__kernel void GrayLevel(__global float4 *output,const __global float4 *input)
{
    int position = get_global_id(0);
	float average = input[position].x * 0.299f + input[position].y * 0.587f + input[position].z * 0.114f;
	output[position].x = average;
	output[position].y = average;
	output[position].z = average;
	output[position].w = input[position].w;
}

//----------------------------------------------------
//Filtre Niveau de gris
//----------------------------------------------------
__kernel void Negatif(__global float4 *output,const __global float4 *input)
{
    int position = get_global_id(0);
	float4 diffValue = 1.0f;
	output[position] = diffValue - input[position];
}

//----------------------------------------------------
//Filtre Niveau de gris
//----------------------------------------------------
__kernel void NoirEtBlanc(__global float4 *output,const __global float4 *input)
{
    int position = get_global_id(0);
	float sum = (input[position].x * 0.299) + (input[position].y * 0.587) + (input[position].z * 0.114);
	if(sum > 0.5)
		sum = 1.0;
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

//---------------------------------------------------------------------
//Limite les valeurs entre 0 et 1.0f
//---------------------------------------------------------------------
float4 NormalizeValue(float4 sum)
{
	float4 value;
	value.x = max(min(sum.x, 1.0f), 0.0f);
	value.y = max(min(sum.y, 1.0f), 0.0f);
	value.z = max(min(sum.z, 1.0f), 0.0f);  
	value.w = max(min(sum.w, 1.0f), 0.0f); 
	return value;
}

__kernel void Sepia(__global float4 *output,const __global float4 *input)
{

	int position = get_global_id(0);
	float m_iDiff = 255.0f - 172.0f;
	float4 value = input[position];	
	float average = input[position].x * 0.299f + input[position].y * 0.587f + input[position].z * 0.114f;

	float r = average * ((172.0f + m_iDiff) / 255.0f);
	float g = average * ((122.0f + m_iDiff) / 255.0f);
	float b = average * ((51.0f + m_iDiff) / 255.0f);
	
	output[position].x = b;
	output[position].y = g;
	output[position].z = r;	
	output[position].w = input[position].w;
	
	output[position] = NormalizeValue(output[position]);

}

//----------------------------------------------------
//Filtre Photo Filtre
//----------------------------------------------------
__kernel void PhotoFiltre(__global float4 *output,const __global float4 *input, int intensity, const __global COLORData * color)
{
    int position = get_global_id(0);
	
	float coeff = (float)intensity / 100.0f;
	float diff = 1.0f - coeff;

	float red = (float)(color->blue / 255.0f) * coeff + input[position].x * diff;
	float green = (float)(color->green / 255.0f) * coeff + input[position].y * diff;
	float blue = (float)(color->red / 255.0f) * coeff + input[position].z * diff;
	
	output[position].x = red;
	output[position].y = green;
	output[position].z = blue;
	output[position].w = input[position].w;

}

//----------------------------------------------------
//Filtre RGB Filtre
//----------------------------------------------------
__kernel void RGBFiltre(__global float4 *output,const __global float4 *input, const __global COLORData * color)
{
    int position = get_global_id(0);
	
	float red = (color->blue / 255.0f) + input[position].x;
	red = min(max(red ,0.0f), 1.0f);
	
	float green = (color->green/ 255.0f) + input[position].y;
	green = min(max(green ,0.0f), 1.0f);
	
	float blue = (color->red / 255.0f) + input[position].z;
	blue = min(max(blue ,0.0f), 1.0f);
	
	output[position].x = red;
	output[position].y = green;
	output[position].z = blue;
	output[position].w = input[position].w;
}

//----------------------------------------------------
//Filtre Posterization
//----------------------------------------------------
__kernel void Posterisation(__global float4 *output,const __global float4 *input, int level)
{
    int position = get_global_id(0);
	
	int _levels = max(2, min(16, level));
	float _offset = (float)256 / (float)_levels;
	
	int red = (int)(input[position].x * 255.0f) / _offset;
	int green = (int)(input[position].y * 255.0f) / _offset;
	int blue =(int)(input[position].z * 255.0f) / _offset;
	
	output[position].x = (float)(red * _offset) / 255.0f;
	output[position].y = (float)(green * _offset)/ 255.0f;
	output[position].z = (float)(blue * _offset)/ 255.0f;
	output[position].w = input[position].w;
	
}

//----------------------------------------------------
//Filtre Solarization
//----------------------------------------------------
__kernel void Solarization(__global float4 *output,const __global float4 *input, int threshold)
{
    int position = get_global_id(0);
	
	float red = input[position].x;
	float green = input[position].y;
	float blue = input[position].z;
	float fthreshold = (float)threshold / 255.0f;
	
	if (red > fthreshold)
		output[position].x = 1.0f - red;
	else
		output[position].x = red;
		
	if (green > fthreshold)
		output[position].y = 1.0f - green;
	else
		output[position].y = green;

	if (blue > fthreshold)
		output[position].z = 1.0f - blue;
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

__kernel void LightAndContrast(__global float4 *output,const __global float4 *input, float contrast, float light)
{
    int position = get_global_id(0);
	
	float4 color;
	float4 src_color = input[position];
	src_color = (float4)255.0f * src_color;
	
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
	
	output[position] = color / (float4)255.0f;
}
