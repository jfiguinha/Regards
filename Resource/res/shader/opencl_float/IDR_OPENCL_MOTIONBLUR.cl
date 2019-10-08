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
//Application du filtre Motion Blur
//---------------------------------------------------------------------
__kernel void MotionBlur(__global float4 * output, const __global float4 *input, int width, int height, const __global float * kernelMotion, const __global int2 * offsets, int kernelSize)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	float4 sum = 0;
	
	for (int i = 0; i < kernelSize; i++)
	{
		int u = x + offsets[i].x;
		int v = y + offsets[i].y;
		if ((u < 0) || (u >= width) || (v < 0) || (v >= height))
			continue;

		float4 color = kernelMotion[i] * GetColorSrc(u, v, input, width, height);
		sum = sum + color;
	}
	
	int position = x + y * width;
	FinalizeData(output, input, position, sum);  
}