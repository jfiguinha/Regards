//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
__kernel void CopyFromOpenGLTexture(__global uint *output, __read_only image2d_t input, int width, int height) 
{ 
    int x = get_global_id(0);
	int y = get_global_id(1);
	int pos = x + y * width;
	int position = x + y * width;
	const int2 pos = {x,y};
	output[position] = rgbaFloat4ToUint(read_imagef(input, sampler, pos),1.0f);
} 