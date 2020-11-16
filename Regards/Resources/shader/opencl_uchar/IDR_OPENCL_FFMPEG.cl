
#define FILTER_PI 3.1415926535f
#define FILTER_2PI 2.0f * 3.1415926535f
#define FILTER_4PI 4.0f * 3.1415926535f

float BilinearFilter( float x)
{
	return (x < 1.0f ? 1.0f - x : 0.0); 
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
float GaussianFilter( float x)
{
	if (fabs (x) > 1.25) 
	{
		return (float)0.0;
	}
	return (float)exp((float)(-x * x) / (float)2.0) / sqrt(FILTER_2PI); 
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
float HammingFilter( float x)
{
	if (x > 1.0f) 
	{
		return 0.0; 
	}
	float dWindow = 0.54 + 0.46 * cos (FILTER_2PI * x); 
	float dSinc = (x == 0) ? 1.0 : sin (FILTER_PI * x) / (FILTER_PI * x); 
	return dWindow * dSinc;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

float CubicFilter( float x) 
{
	if (x > 2.0) 
	{
		return 0.0; 
	}

	if (x < -2.0)
		return(0.0);
	if (x < -1.0)
		return((2.0+x)*(2.0+x)*(2.0+x)/6.0);
	if (x < 0.0)
		return((4.0+x*x*(-6.0-3.0*x))/6.0);
	if (x < 1.0)
		return((4.0+x*x*(-6.0+3.0*x))/6.0);
	if (x < 2.0)
		return((2.0-x)*(2.0-x)*(2.0-x)/6.0);
	return(0.0);

}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

float BlackmanFilter( float x) 
{
	if (x > 1.0f) 
	{
		return 0.0; 
	}
	float dN = 2.0 * 1.0f + 1.0; 
	return 0.42 + 0.5 * (float)cos((float)(FILTER_2PI * x) / (float)( dN - 1.0 )) + (float)0.08 * (float)cos((float)(FILTER_4PI * x) / (float)( dN - 1.0 )); 
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
 
float QuadraticFilter( float x) 
{
	if (x > 1.5f) 
	{
		return 0.0f; 
	}

	if (x < -1.5)
		return(0.0);
	if (x < -0.5)
		return(0.5*(x+1.5)*(x+1.5));
	if (x < 0.5)
		return(0.75-x*x);
	if (x < 1.5)
		return(0.5*(x-1.5)*(x-1.5));
	return(0.0);

}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
float MitchellFilter( float x) 
{
	if (x > 2.0f) 
		return 0.0; 

	#define B   (1.0/3.0)
	#define C   (1.0/3.0)
	#define P0  ((  6.0- 2.0*B       )/6.0)
	#define P2  ((-18.0+12.0*B+ 6.0*C)/6.0)
	#define P3  (( 12.0- 9.0*B- 6.0*C)/6.0)
	#define Q0  ((       8.0*B+24.0*C)/6.0)
	#define Q1  ((     -12.0*B-48.0*C)/6.0)
	#define Q2  ((       6.0*B+30.0*C)/6.0)
	#define Q3  ((     - 1.0*B- 6.0*C)/6.0)

	if (x < -2.0)
		return(0.0);
	if (x < -1.0)
		return(Q0-x*(Q1-x*(Q2-x*Q3)));
	if (x < 0.0)
		return(P0+x*x*(P2-x*P3));
	if (x < 1.0)
		return(P0+x*x*(P2+x*P3));
	if (x < 2.0)
		return(Q0+x*(Q1+x*(Q2+x*Q3)));
	return(0.0);

}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

float TriangleFilter( float x) 
{
	if (x > 1.0f) 
		return 0.0f; 
	if (x < -1.0f)
		return(0.0f);
	if (x < 0.0f)
		return(1.0f+x);
	if (x < 1.0f)
		return(1.0f-x);
	return(0.0f);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

float CatromFilter( float x )
{
	float m_dWidth = 2.0f;
	if (x > m_dWidth) 
	{
		return 0.0f; 
	}

	if (x < -2.0f)
		return(0.0f);
	if (x < -1.0f)
		return(0.5f*(4.0f+x*(8.0f+x*(5.0f+x))));
	if (x < 0.0f)
		return(0.5f*(2.0f+x*x*(-5.0f-3.0f*x)));
	if (x < 1.0f)
		return(0.5f*(2.0f+x*x*(-5.0f+3.0f*x)));
	if (x < 2.0f)
		return(0.5f*(4.0f+x*(-8.0f+x*(5.0f-x))));
	return(0.0f);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

float HanningFilter( float x )
{
	if (x > 1.0f) 
	{
		return 0.0f; 
	}

	return(0.54f+0.46f*(float)cos((float)FILTER_PI*x));
}
		
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

float HermiteFilter( float x )
{
	float m_dWidth = 1.5f;
	if (x > m_dWidth) 
		return 0.0; 

	if (x < -1.0)
		return(0.0);
	if (x < 0.0)
		return((2.0*(-x)-3.0)*(-x)*(-x)+1.0);
	if (x < 1.0)
		return((2.0*x-3.0)*x*x+1.0);
	return(0.0);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

float BboxFilter( float f )
{
	float width = 0.5f;
	return (f <= width ? 1.0f : 0.0f);
}


float KernelFilter_selection( float f, int type)
{
	if(type == 1)
	{
		return BboxFilter(f);
	}
	else if(type == 2)
	{
		return HermiteFilter(f);
	}
	else if(type == 3)
	{
		return HanningFilter(f);
	}	
	else if(type == 4)
	{
		return CatromFilter(f);
	}		
	else if(type == 5)
	{
		return MitchellFilter(f);
	}	
	else if(type == 6)
	{
		return TriangleFilter(f);
	}	
	else if(type == 7)
	{
		return QuadraticFilter(f);
	}	
	else if(type == 8)
	{
		return BlackmanFilter(f);
	}	
	else if(type == 9)
	{
		return HammingFilter(f);
	}	
	else if(type == 10)
	{
		return GaussianFilter(f);
	}
	else if(type == 11)
	{
		return BilinearFilter(f);
	}	
	else
	{
		return CubicFilter(f);
	}
}

__constant sampler_t sampler =
      CLK_NORMALIZED_COORDS_FALSE
    | CLK_ADDRESS_CLAMP_TO_EDGE
    | CLK_FILTER_NEAREST;

float4 GetColorFromOpenGLTexture( __read_only image2d_t input, int x, int y, int width, int height)
{
	const int2 pos = {x,y};
	if(x < width && y < height && y >= 0 && x >= 0)	
	{
		float4 data = read_imagef(input, sampler, pos);
		
		int position = x + y * width;

		float4 color;
		color.x = data.x;
		color.y = data.y;
		color.z = data.z;
		return color;
	}
	return 0.0f;
}

float4 BiCubicFromOpenGLTexture(float x, float y, __read_only image2d_t input, int width, int height, int type)
{
	float4 nDenom = 0.0f;
	int valueA = (int)x;
	int valueB = (int)y;
	float realA = x - valueA;
	float realB = y - valueB;
	
	float4 fy1 = KernelFilter_selection(-(-1.0f - realB), type);
	float4 fy2 = KernelFilter_selection(realB, type);
	float4 fy3 = KernelFilter_selection(-(1.0f - realB), type);
	
	float4 fx1 = KernelFilter_selection(-1.0f - realA, type);
	float4 fx2 = KernelFilter_selection(- realA, type);
	float4 fx3 = KernelFilter_selection(1.0f - realA, type);
	
	nDenom += fy1 * (fx1 + fx2 + fx3) + fy2 * (fx1 + fx2 + fx3) + fy3 * (fx1 + fx2 + fx3);

	
	float4 sum = GetColorFromOpenGLTexture(input, x - 1, y - 1, width, height) * (fy1 * fx1);
	sum += GetColorFromOpenGLTexture(input, x , y - 1, width, height) * (fy1 * fx2);
	sum += GetColorFromOpenGLTexture(input, x + 1, y - 1, width, height) * (fy1 * fx3);
	
	sum += GetColorFromOpenGLTexture(input, x - 1, y, width, height) * (fy2 * fx1);
	sum += GetColorFromOpenGLTexture(input, x , y, width, height) * (fy2 * fx2);
	sum += GetColorFromOpenGLTexture(input, x + 1, y, width, height) * (fy2 * fx3);

	sum += GetColorFromOpenGLTexture(input, x - 1, y + 1, width, height) * (fy3 * fx1);
	sum += GetColorFromOpenGLTexture(input, x , y + 1, width, height) * (fy3 * fx2);
	sum += GetColorFromOpenGLTexture(input, x + 1, y + 1, width, height) * (fy3 * fx3);
	
    return (sum / nDenom);
}

//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
float4 ExecuteBicubicFromOpenGLTexture(int x, int y, __read_only image2d_t input, int widthIn, int heightIn, int width, int height, int flipH, int flipV, int angle, int bicubic) 
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
	
		
	if(angle == 90 || angle == 270)
	{
		if (flipV == 1)
		{
			posX = widthIn - posX - 1;
		}

		if (flipH == 1)
		{
			posY = heightIn - posY - 1;
		}
	
	}
	else
	{
		if (flipH == 1)
		{
			posX = widthIn - posX - 1;
		}

		if (flipV == 1)
		{
			posY = heightIn - posY - 1;
		}
	}
	
	float4 color;
	if(bicubic != 0)
		color = BiCubicFromOpenGLTexture(posX, posY, input, widthIn, heightIn, bicubic);
	else
		color = GetColorFromOpenGLTexture(input, posX,  posY, widthIn, heightIn);
	
	return color;
} 

//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32
//----------------------------------------------------
__kernel void InterpolationFromOpenGLTexture(__global uint *output, __read_only image2d_t input, int width, int height, int widthOut, int heightOut, int angle, int bicubic) 
{ 
    int x = get_global_id(0);
	int y = get_global_id(1);
	int position = x + (heightOut - y - 1) * widthOut;
	output[position] = rgbaFloat4ToUint(ExecuteBicubicFromOpenGLTexture(x, y, input, width, height, widthOut, heightOut, angle, bicubic),1.0f);
} 

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

//----------------------------------------------------
// Conversion Special Effect Video du NV12 vers le RGB32 Par zone
//---------------------------------------------------- 
__kernel void InterpolationZone(__global float4 *output, __read_only image2d_t input, int widthIn, int heightIn, int widthOut, int heightOut, float left, float top, float bitmapWidth, float bitmapHeight, int angle, int bicubic)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	int position = x + (heightOut - y - 1) * widthOut;
	
	float ratioX = (float)widthIn / bitmapWidth;
	float ratioY = (float)heightIn / bitmapHeight;
	if (angle == 90)
	{
		ratioX = (float)widthIn / (float)bitmapHeight;
		ratioY = (float)heightIn / (float)bitmapWidth;
	}
	else if(angle == 270)
	{
		ratioX = (float)widthIn / (float)bitmapHeight;
		ratioY = (float)heightIn / (float)bitmapWidth;	
	}

	float posX = (float)x * ratioX + left * ratioX;
	float posY = (float)y * ratioY + top * ratioY;

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
	
		
	if(angle == 90 || angle == 270)
	{
		if (flipV == 1)
		{
			posX = widthIn - posX - 1;
		}

		if (flipH == 1)
		{
			posY = heightIn - posY - 1;
		}
	
	}
	else
	{
		if (flipH == 1)
		{
			posX = widthIn - posX - 1;
		}

		if (flipV == 1)
		{
			posY = heightIn - posY - 1;
		}
	}
	
	float4 color;
	if(bicubic != 0)
		color = BiCubicFromOpenGLTexture(posX, posY, input, widthIn, heightIn, bicubic);
	else
		color = GetColorFromOpenGLTexture(input, posX,  posY, widthIn, heightIn);
	
	output[position] = rgbaFloat4ToUint(color,1.0f);
}
//----------------------------------------------------
//Change la valeur de la couche alpha
//----------------------------------------------------
__kernel void SetAlphaValueFromOpenGLTexture(__global uint *output, __read_only image2d_t input, int width, int height, float alphaValue) 
{
    int x = get_global_id(0);
	int y = get_global_id(1);

	int position = x + (height - y - 1) * width;
	const int2 pos = {x,y};

	float4 data = read_imagef(input, sampler, pos);
	data.w = (alphaValue / 100.0f);

	output[position] = rgbaFloat4ToUint(data,1.0f);
}