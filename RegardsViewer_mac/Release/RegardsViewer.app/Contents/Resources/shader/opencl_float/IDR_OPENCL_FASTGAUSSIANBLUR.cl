//---------------------------------------------------------------------
//Recherche le pixel
//---------------------------------------------------------------------
float4 GetColorSrc(int x, int y, const __global float4 *input, int width, int height)
{
	if(x < width && y < height && y >= 0 && x >= 0)	
	{
		int position = x + y * width;
		return input[position];
	}
	return (float4)0.0f;
}

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

void FinalizeData(__global float4 * output, const __global float4 *input, int position, float4 sum)
{
	float4 color = input[position];
	float alpha = color.w;
	color = NormalizeValue(sum);
	color.w = alpha;
	output[position] = color; 
}

//---------------------------------------------------------------------
//Application du filtre Gaussian Blur
//	int kernel[9] = { 5, 15, 31, 49, 56, 49, 31, 15, 5 };
//	int size = 3;
//	int factor = 256;
//---------------------------------------------------------------------
__kernel void FastGaussianBlur(__global float4 * output, const __global float4 *input, int width, int height, int r)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	float4 div = ((r+r+1)*(r+r+1));
	float4 val = 0.0f;
	for(int iy=y-r; iy<y+r+1; iy++)
	{
		for(int ix=x-r; ix<x+r+1; ix++)
		{
			val += GetColorSrc(ix,iy, input, width, height);
		}
	}

	float4 sum = val/div;
	int position = x + y * width;
	FinalizeData(output, input, position, sum);
}
