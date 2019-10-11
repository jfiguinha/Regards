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
//Application du filtre Edge
//---------------------------------------------------------------------

float4 GradientX(const __global float4 *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	float4 sum = (float4)0.0f;
	sum = GetColorSrc(x - 1, y - 1, input, width, height);
	sum += GetColorSrc(x - 1, y, input, width, height) * (float4)2.0f;
	sum += GetColorSrc(x - 1, y + 1, input, width, height);
	sum -= GetColorSrc(x + 1, y - 1, input, width, height);
	sum -= GetColorSrc(x + 1, y, input, width, height) * (float4)2.0f;
	sum -= GetColorSrc(x + 1, y + 1, input, width, height);
	return fabs(sum);
}

float4 GradientY(const __global float4 *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	float4 sum = (float4)0;
	sum = GetColorSrc(x - 1, y - 1, input, width, height);
	sum += GetColorSrc(x, y - 1, input, width, height) * (float4)2.0f;
	sum += GetColorSrc(x + 1, y  - 1, input, width, height);
	sum -= GetColorSrc(x - 1, y + 1, input, width, height);
	sum -= GetColorSrc(x, y + 1, input, width, height) * (float4)2.0f;
	sum -= GetColorSrc(x + 1, y + 1, input, width, height);
	return fabs(sum);
}

__kernel void Edge(__global float4 * output, const __global float4 *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);

	float4 sum = (float4)0.0f;
	float4 gx = GradientX(input, width, height);
	float4 gy = GradientY(input, width, height);
	sum = gx + gy;

	int position = x + y * width;
	FinalizeData(output, input, position, sum);
}
