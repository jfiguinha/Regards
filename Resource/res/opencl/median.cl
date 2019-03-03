float4 GetColorSrc(int x, int y, const __global float4 *input, int width, int height)
{
	if(x < width && y < height && y >= 0 && x >= 0)	
	{
		int position = x + y * width;
		return input[position];
	}
	return 0.0f;
}

void s2(float4 * a,float4 * b)
{
	float4 temp = *a; 
	*a = min(*a, *b); 
	*b = max(temp, *b);
}
void mn3(float4 * a,float4 * b,float4 * c)
{
	s2(a, b); 
	s2(a, c); 
}
void mx3(float4 * a,float4 * b,float4 * c)
{
	s2(b, c); 
	s2(a, c); 
}
void mnmx3(float4 * a,float4 * b,float4 * c)
{
	mx3(a, b, c); 
	s2(a, b); 
}
void mnmx4(float4 * a,float4 * b,float4 * c, float4 * d)
{
	s2(a, b); s2(c, d); s2(a, c); s2(b, d); 
}
void mnmx5(float4 * a,float4 * b,float4 * c, float4 * d, float4 * e)
{
	s2(a, b); s2(c, d); mn3(a, c, e); mx3(b, d, e);  
}
void mnmx6(float4 * a,float4 * b,float4 * c, float4 * d, float4 * e, float4 * f)
{
	s2(a, d); s2(b, e); s2(c, f); mn3(a, b, c); mx3(d, e, f);  
}

__kernel void Median(__global float4 *output, const __global float4 *input, int width, int height)
{																  	
    int x = get_global_id(0);
	int y = get_global_id(1);
	int pos = 0;

	float4 k0 = GetColorSrc(x - 1, y - 1, input, width, height);
	float4 k1 = GetColorSrc(x , y - 1, input, width, height);
	float4 k2 = GetColorSrc(x + 1, y - 1, input, width, height);
	float4 k3 = GetColorSrc(x - 1, y, input, width, height);
	float4 k4 = GetColorSrc(x , y, input, width, height);
	float4 k5 = GetColorSrc(x + 1, y, input, width, height);
	float4 k6 = GetColorSrc(x - 1, y + 1, input, width, height);
	float4 k7 = GetColorSrc(x , y + 1, input, width, height);
	float4 k8 = GetColorSrc(x + 1, y + 1, input, width, height);
	
	mnmx6(&k0, &k1, &k2, &k3, &k4, &k5);
	mnmx5(&k1, &k2, &k3, &k4, &k6);
	mnmx4(&k2, &k3, &k4, &k7);
	mnmx3(&k3, &k4, &k8);
	
	int position = x + y * width;
	output[position] = k4;

}
