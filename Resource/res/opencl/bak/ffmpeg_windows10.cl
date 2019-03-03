//----------------------------------------------------
// Conversion du NV12 vers du 32 bits
//----------------------------------------------------
uchar4 GetColorFromYUV(const __global uchar *inputY, const __global uchar *inputU, const __global uchar *inputV, int x, int y, int width, int height, int pitch)
{
	if(x < width && y < height && y >= 0 && x >= 0)	
	{
		uchar4 color;
		int positionSrc = x + y * pitch;
		int positionUV = 0;
		int yModulo = y % 2;
		int xModulo = x % 2;
		if (xModulo == 1)
		{		
			if (yModulo == 1)
				positionUV = ((x - 1) / 2) + ((y - 1) / 2) * (pitch / 2);
			else
				positionUV = ((x - 1) / 2) + (y / 2) * (pitch / 2);
		}
		else
		{
			if (yModulo == 1)
				positionUV = (x / 2) + ((y - 1) / 2) * (pitch / 2);
			else
				positionUV = (x / 2) + (y / 2) * (pitch / 2);
		}
		
		int vComp = inputU[positionUV];
		int uComp = inputV[positionUV];
		int yComp = inputY[positionSrc];
		    // RGB conversion
		int r = yComp + 1.402*(vComp-128);
		int g = yComp - 0.344*(uComp-128) - 0.714*(vComp-128);
		int b = yComp + 1.772*(uComp-128);
		
		if(r < 0)
			r = 0;
		if(r > 255)
			r = 255;

		if(g < 0)
			g = 0;
		if(g > 255)
			g = 255;
			
		if(b < 0)
			b = 0;
		if(b > 255)
			b = 255;			
		
		color.x = r;
		color.y = g;
		color.z = b;
		color.w = 255;
		
		return color;
	}
	return (uchar4)0;
}

float4 BiCubicYUV(float x, float y, const __global uchar *inputY, const __global uchar *inputU, const __global uchar *inputV, int width, int height, int pitch)
{
	float4 nDenom = 0.0f;
	int valueA = (int)x;
	int valueB = (int)y;
	float realA = x - valueA;
	float realB = y - valueB;
	
	float4 fy1 = Cubic(-(-1.0f - realB));
	float4 fy2 = Cubic(realB);
	float4 fy3 = Cubic(-(1.0f - realB));
	
	float4 fx1 = Cubic(-1.0f - realA);
	float4 fx2 = Cubic(- realA);
	float4 fx3 = Cubic(1.0f - realA);
	
	nDenom += fy1 * (fx1 + fx2 + fx3) + fy2 * (fx1 + fx2 + fx3) + fy3 * (fx1 + fx2 + fx3);

	
	float4 sum = convert_float4(GetColorFromYUV(inputY, inputU, inputV, x - 1, y - 1, width, height, pitch)) * (fy1 * fx1);
	sum += convert_float4(GetColorFromYUV(inputY, inputU, inputV, x , y - 1, width, height, pitch)) * (fy1 * fx2);
	sum += convert_float4(GetColorFromYUV(inputY, inputU, inputV, x + 1, y - 1, width, height, pitch)) * (fy1 * fx3);
	
	sum += convert_float4(GetColorFromYUV(inputY, inputU, inputV, x - 1, y, width, height, pitch)) * (fy2 * fx1);
	sum += convert_float4(GetColorFromYUV(inputY, inputU, inputV, x , y, width, height, pitch)) * (fy2 * fx2);
	sum += convert_float4(GetColorFromYUV(inputY, inputU, inputV, x + 1, y, width, height, pitch)) * (fy2 * fx3);

	sum += convert_float4(GetColorFromYUV(inputY, inputU, inputV, x - 1, y + 1, width, height, pitch)) * (fy3 * fx1);
	sum += convert_float4(GetColorFromYUV(inputY, inputU, inputV, x , y + 1, width, height, pitch)) * (fy3 * fx2);
	sum += convert_float4(GetColorFromYUV(inputY, inputU, inputV, x + 1, y + 1, width, height, pitch)) * (fy3 * fx3);
	
    return (sum / nDenom);
}

//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
uchar4 ExecuteBicubicYUV(int x, int y, const __global uchar *inputY, const __global uchar *inputU, const __global uchar *inputV, int widthIn, int heightIn, int width, int height, int flipH, int flipV, int angle, int bicubic, int pitch) 
{ 
	float ratioX = (float)widthIn / (float)width;
	float ratioY = (float)heightIn / (float)height;
	if (angle == 90 || angle == 270)
	{
		ratioX = (float)widthIn / (float)height;
		ratioY = (float)heightIn / (float)width;
	}

	float posY = (float)y * ratioY;
	float posX = (float)x * ratioX;

	if (angle == 90)
	{
		int srcx = posY;
		int srcy = posX;

		posX = srcx;
		posY = srcy;

		posX = widthIn - posX - 1;
	}
	else if (angle == 180)
	{
		posX = widthIn - posX - 1;
		posY = heightIn - posY - 1;
	}
	else if (angle == 270)
	{
		int srcx = posY;
		int srcy = posX;

		posX = srcx;
		posY = srcy;

		posY = heightIn - posY - 1;
	}

	if (flipH == 1)
	{
		posX = widthIn - posX - 1;
	}

	if (flipV == 1)
	{
		posY = heightIn - posY - 1;
	}
	
	uchar4 color;
	if(bicubic)
		color = convert_uchar4(BiCubicYUV(posX, posY, inputY, inputU, inputV, widthIn, heightIn, pitch));
	else
		color = GetColorFromYUV(inputY, inputU, inputV, posX,  posY, widthIn, heightIn, pitch);
	
	return color;
} 



//----------------------------------------------------
// Conversion Special Effect Video du YUV vers le WXWIDGET
//----------------------------------------------------
__kernel void BicubicYUVtowxWidget(__global uchar *output, const __global uchar *inputY, const __global uchar *inputU, const __global uchar *inputV, int widthIn, int heightIn, int widthOut, int heightOut, int flipH, int flipV, int angle, int bicubic, int pitch) 
{ 
	int position = get_global_id(0);
	int y = position / widthOut;
	int x = position - (y * widthOut);


	uchar4 color = ExecuteBicubicYUV(x, y, inputY, inputU, inputV, widthIn, heightIn, widthOut, heightOut, flipH, flipV, angle, bicubic, pitch);
	
	output[position*3+2] = color.x;
	output[position*3+1] = color.y;
	output[position*3] = color.z;
} 

//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
__kernel void BicubicYUVtoRegardsBitmap(__global uchar4 *output, const __global uchar *inputY, const __global uchar *inputU, const __global uchar *inputV, int widthIn, int heightIn, int widthOut, int heightOut, int flipH, int flipV, int angle, int bicubic, int pitch) 
{ 
    int x = get_global_id(0);
	int y = get_global_id(1);
	int position = x + y * widthOut;

	output[position]  = ExecuteBicubicYUV(x, y, inputY, inputU, inputV, widthIn, heightIn, widthOut, heightOut, flipH, flipV, angle, bicubic, pitch);
} 


__constant sampler_t sampler =
      CLK_NORMALIZED_COORDS_FALSE
    | CLK_ADDRESS_CLAMP_TO_EDGE
    | CLK_FILTER_NEAREST;

//----------------------------------------------------
// Conversion du 
//----------------------------------------------------
uchar4 GetColorFromOpenGLTexture( __read_only image2d_t input, int x, int y, int width, int height)
{
	const int2 pos = {x,y};
	if(x < width && y < height && y >= 0 && x >= 0)	
	{
		float4 data = read_imagef(input, sampler, pos);
		
		int position = x + y * width;

		uchar4 color;
		color.x = data.x * (float)255;
		color.y = data.y * (float)255;
		color.z = data.z * (float)255;
		return color;
	}
	return (uchar4)0;
}

float4 BiCubicFromOpenGLTexture(float x, float y, __read_only image2d_t input, int width, int height)
{
	float4 nDenom = 0.0f;
	int valueA = (int)x;
	int valueB = (int)y;
	float realA = x - valueA;
	float realB = y - valueB;
	
	float4 fy1 = Cubic(-(-1.0f - realB));
	float4 fy2 = Cubic(realB);
	float4 fy3 = Cubic(-(1.0f - realB));
	
	float4 fx1 = Cubic(-1.0f - realA);
	float4 fx2 = Cubic(- realA);
	float4 fx3 = Cubic(1.0f - realA);
	
	nDenom += fy1 * (fx1 + fx2 + fx3) + fy2 * (fx1 + fx2 + fx3) + fy3 * (fx1 + fx2 + fx3);

	
	float4 sum = convert_float4(GetColorFromOpenGLTexture(input, x - 1, y - 1, width, height)) * (fy1 * fx1);
	sum += convert_float4(GetColorFromOpenGLTexture(input, x , y - 1, width, height)) * (fy1 * fx2);
	sum += convert_float4(GetColorFromOpenGLTexture(input, x + 1, y - 1, width, height)) * (fy1 * fx3);
	
	sum += convert_float4(GetColorFromOpenGLTexture(input, x - 1, y, width, height)) * (fy2 * fx1);
	sum += convert_float4(GetColorFromOpenGLTexture(input, x , y, width, height)) * (fy2 * fx2);
	sum += convert_float4(GetColorFromOpenGLTexture(input, x + 1, y, width, height)) * (fy2 * fx3);

	sum += convert_float4(GetColorFromOpenGLTexture(input, x - 1, y + 1, width, height)) * (fy3 * fx1);
	sum += convert_float4(GetColorFromOpenGLTexture(input, x , y + 1, width, height)) * (fy3 * fx2);
	sum += convert_float4(GetColorFromOpenGLTexture(input, x + 1, y + 1, width, height)) * (fy3 * fx3);
	
    return (sum / nDenom);
}

//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
uchar4 ExecuteBicubicFromOpenGLTexture(int x, int y, __read_only image2d_t input, int widthIn, int heightIn, int width, int height, int flipH, int flipV, int angle, int bicubic) 
{ 
	float ratioX = (float)widthIn / (float)width;
	float ratioY = (float)heightIn / (float)height;
	if (angle == 90 || angle == 270)
	{
		ratioX = (float)widthIn / (float)height;
		ratioY = (float)heightIn / (float)width;
	}

	float posY = (float)y * ratioY;
	float posX = (float)x * ratioX;

	if (angle == 90)
	{
		int srcx = posY;
		int srcy = posX;

		posX = srcx;
		posY = srcy;

		posX = widthIn - posX - 1;
	}
	else if (angle == 180)
	{
		posX = widthIn - posX - 1;
		posY = heightIn - posY - 1;
	}
	else if (angle == 270)
	{
		int srcx = posY;
		int srcy = posX;

		posX = srcx;
		posY = srcy;

		posY = heightIn - posY - 1;
	}

	if (flipH == 1)
	{
		posX = widthIn - posX - 1;
	}

	if (flipV == 1)
	{
		posY = heightIn - posY - 1;
	}
	
	uchar4 color;
	if(bicubic)
		color = convert_uchar4(BiCubicFromOpenGLTexture(posX, posY, input, widthIn, heightIn));
	else
		color = GetColorFromOpenGLTexture(input, posX,  posY, widthIn, heightIn);
	
	return color;
} 

//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
__kernel void BicubicFromOpenGLTexture(__global uchar4 *output, __read_only image2d_t input, int width, int height, int widthOut, int heightOut, int flipH, int flipV, int angle, int bicubic) 
{ 
    int x = get_global_id(0);
	int y = get_global_id(1);
	int position = x + y * widthOut;

	output[position]  = ExecuteBicubicFromOpenGLTexture(x, y, input, width, height, widthOut, heightOut, flipH, flipV, angle, bicubic);

} 

//----------------------------------------------------
//Change la valeur de la couche alpha
//----------------------------------------------------
__kernel void SetAlphaValueFromOpenGLTexture(__global uchar4 *output, __read_only image2d_t input, int width, int height, float alphaValue) 
{
    int x = get_global_id(0);
	int y = get_global_id(1);

	int position = x + (height - y - 1) * width;
	
	
	const int2 pos = {x,y};

	float4 data = read_imagef(input, sampler, pos);

	uchar4 color;
	color.x = data.z * (float)255;
	color.y = data.y * (float)255;
	color.z = data.x * (float)255;
	color.w = (alphaValue / 100.0) * 255.0;

	output[position] = color;
}


//----------------------------------------------------
//Change la valeur de la couche alpha
//----------------------------------------------------
__kernel void SetAlphaValue(__global uchar4 *output, const __global uchar4 * input, int width, int height, float alphaValue) 
{
    int x = get_global_id(0);
	int y = get_global_id(1);

	int position = x + y * width;


	uchar4 data = input[position];

	uchar4 color;
	color.x = data.x;
	color.y = data.y;
	color.z = data.z;
	color.w = (alphaValue / 100.0) * 255.0;

	output[position] = color;
}
