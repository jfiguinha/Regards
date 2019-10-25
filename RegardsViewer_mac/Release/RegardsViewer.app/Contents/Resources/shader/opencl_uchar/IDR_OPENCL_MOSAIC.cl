__kernel void Mosaic(__global uint * output, const __global uint *input, int width, float fTileSize)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	
	float w = fTileSize;
	float h = fTileSize;		

	float s = floor(x / w);
	float t = floor(y / h);	
	
	int sourcex = convert_int(s * w);
	int sourcey = convert_int(t * h);
	
	int positionSrc = sourcex + sourcey * width;
	int positionDest = x + y * width;

	output[positionDest] = input[positionSrc];
}

