typedef struct
{
	int red;
	int green;
	int blue;
	int alpha;
}COLORData;

float4 GetColorSrc(int x, int y, const __global uchar4 *input, int widthIn, int heightIn)
{
	if(x < widthIn && y < heightIn && y >= 0 && x >= 0)	
	{
		int position = x + y * widthIn;
		return convert_float4(input[position]);
	}
	return (float4)0.0f;
}

float Noise2d(int x, int y)
{
	int n = ((x + (y << 6)) << 13) ^ (x + (y << 6));
	return 0.2f * (255.0f * (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0));
}

float CalculPosValue(int Xint, int Yint)
{
	int m = Xint + ((Yint) << 6);
	int n = (m << 13) ^ (m);
	return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0); 
}

float GetValue(float x, float y) 
{
	int Xint = (int)x;
	int Yint = (int)y;
	
	float Xfrac = x - (float)Xint;
	float Yfrac = y - (float)Yint;

	float x0y0,x1y0,x0y1,x1y1;

	if(Xint != 0 || Yint != 0)
	{		
		x0y0 = CalculPosValue(Xint,Yint);
		x1y0 = CalculPosValue(Xint+1,Yint);
		x0y1 = CalculPosValue(Xint,Yint+1);
		x1y1 = CalculPosValue(Xint+1,Yint+1);
	}
	else
	{	
		x0y0 = Noise2d(0, 0);
		x1y0 = Noise2d(1, 0);
		x0y1 = Noise2d(0, 1);
		x1y1 = Noise2d(1, 1);		
	}

	//interpolate between those values according to the x and y fractions
	float v1 = (x0y0 + (Xfrac * (x1y0 - x0y0)));
	float v2 = (x0y1 + (Xfrac * (x1y1 - x0y1)));
	float fin = (v1 + (Yfrac * (v2 - v1)));

	return fin;
}

/*
__kernel void PerlinNoise(__global uchar4 *output, const __global uchar4 *input, const __global COLORData * color1, const __global COLORData *  color2, const __global float * frequence, const __global float * amplitude, int octaves, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	float total = 0.0f;
	
	for(uint k=0; k < octaves; k++) 
	{
		total += GetValue(x * frequence[k], y * frequence[k]) * amplitude[k];					
	}

	total = total * 0.5f + 0.5f;

	if(total<0) 
		total = 0.0f;
	if(total>1) 
		total = 1.0f;
		
	float4 value = color1 * (float4)total + color2 * (float4)(1 - total);

	int position = x + y * width;
	output[position] = convert_uchar4(value);
}
*/

__kernel void Noise(__global uchar4 *output, const __global uchar4 *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	float4 n = (float4)Noise2d(x, y);
	float4 src_color = GetColorSrc(x, y, input, width, height) + n;
	int position = x + y * width;

	output[position] = convert_uchar4(src_color);
}