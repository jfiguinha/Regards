//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
__constant sampler_t sampler =
      CLK_NORMALIZED_COORDS_FALSE
    | CLK_ADDRESS_CLAMP_TO_EDGE
    | CLK_FILTER_NEAREST;
	
__kernel void CopyFromOpenGLTexture(__global float4 * output, __read_only image2d_t input, int width, int height) 
{ 
    int x = get_global_id(0);
	int y = get_global_id(1);
	int position = x + (height - y -1) * width;
	const int2 pos = {x,y};
	output[position] = read_imagef(input, sampler, pos);
} 