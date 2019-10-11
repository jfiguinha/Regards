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
//Application du filtre Box Blur
//---------------------------------------------------------------------
__kernel void BoxBlurH(__global float4 * output, const __global float4 *input, int width, int height, int coeff)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	float4 sum = 0;
	
	for (int ix = x - coeff; ix < x + coeff + 1; ix++)
	{
		int posX = min(width - 1, max(0, ix));
		sum = sum + GetColorSrc(posX, y, input, width, height);
	}

	sum = sum / (coeff + coeff + 1);
	int position = x + y * width;
	FinalizeData(output, input, position, sum); 
}

__kernel void BoxBlurV(__global float4 * output, const __global float4 *input, int width, int height, int coeff)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	float4 sum = 0;
	
	for (int iy = y - coeff; iy<y + coeff + 1; iy++)
	{
		int posY = min(height - 1, max(0, iy));
		sum = sum + GetColorSrc(x, posY, input, width, height);
	}

	sum = sum / (float4)(coeff + coeff + 1);
	int position = x + y * width;
	FinalizeData(output, input, position, sum);  
}
