//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
__kernel void InterpolationFromOpenGLTexture(__global float4 *output, __read_only image2d_t input, int width, int height, int widthOut, int heightOut, int flipH, int flipV, int angle, int bicubic) 
{ 
    int x = get_global_id(0);
	int y = get_global_id(1);
	int position = x + (heightOut - y - 1) * widthOut;

	output[position] = ExecuteBicubicFromOpenGLTexture(x, y, input, width, height, widthOut, heightOut, flipH, flipV, angle, bicubic);

} 