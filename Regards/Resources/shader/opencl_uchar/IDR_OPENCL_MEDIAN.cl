uint GetColorSrc(int x, int y, const __global uint *input, int width, int height)
{
	if(x < width && y < height && y >= 0 && x >= 0)	
	{
		int position = x + y * width;
		return input[position];
	}
	return 0;
}

void s2(uint * a,uint * b)
{
	uint temp = *a; 
	*a = min(*a, *b); 
	*b = max(temp, *b);
}
void mn3(uint * a,uint * b,uint * c)
{
	s2(a, b); 
	s2(a, c); 
}
void mx3(uint * a,uint * b,uint * c)
{
	s2(b, c); 
	s2(a, c); 
}
void mnmx3(uint * a,uint * b,uint * c)
{
	mx3(a, b, c); 
	s2(a, b); 
}
void mnmx4(uint * a,uint * b,uint * c, uint * d)
{
	s2(a, b); s2(c, d); s2(a, c); s2(b, d); 
}
void mnmx5(uint * a,uint * b,uint * c, uint * d, uint * e)
{
	s2(a, b); s2(c, d); mn3(a, c, e); mx3(b, d, e);  
}
void mnmx6(uint * a,uint * b,uint * c, uint * d, uint * e, uint * f)
{
	s2(a, d); s2(b, e); s2(c, f); mn3(a, b, c); mx3(d, e, f);  
}

__kernel void Median(__global uint *output, const __global uint *input, int width, int height)
{																  	
    int x = get_global_id(0);
	int y = get_global_id(1);
	int pos = 0;

	uint k0 = GetColorSrc(x - 1, y - 1, input, width, height);
	uint k1 = GetColorSrc(x , y - 1, input, width, height);
	uint k2 = GetColorSrc(x + 1, y - 1, input, width, height);
	uint k3 = GetColorSrc(x - 1, y, input, width, height);
	uint k4 = GetColorSrc(x , y, input, width, height);
	uint k5 = GetColorSrc(x + 1, y, input, width, height);
	uint k6 = GetColorSrc(x - 1, y + 1, input, width, height);
	uint k7 = GetColorSrc(x , y + 1, input, width, height);
	uint k8 = GetColorSrc(x + 1, y + 1, input, width, height);
	
	mnmx6(&k0, &k1, &k2, &k3, &k4, &k5);
	mnmx5(&k1, &k2, &k3, &k4, &k6);
	mnmx4(&k2, &k3, &k4, &k7);
	mnmx3(&k3, &k4, &k8);
	
	int position = x + y * width;
	output[position] = k4;

}

