//Implementation de l'interpolation bicubic en OpenCL
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
		return 0.0;
	}
	return exp(-x * x / 2.0) / sqrt (FILTER_2PI); 
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
	return 0.42 + 0.5 * cos(FILTER_2PI * x / ( dN - 1.0 )) + 0.08 * cos (FILTER_4PI * x / ( dN - 1.0 )); 
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

	return(0.54f+0.46f*cos(FILTER_PI*x));
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

float4 GetColorSrc(int x, int y, const __global float4 *input, int widthIn, int heightIn)
{
	if(x < widthIn && y < heightIn && y >= 0 && x >= 0)	
	{
		int position = x + y * widthIn;
		return input[position];
	}
	return 0.0f;
}

float4 KernelExecution(float x, float y, const __global float4 *input, int widthIn, int heightIn, int type)
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

	
	float4 sum = GetColorSrc(x - 1, y - 1, input, widthIn, heightIn) * (fy1 * fx1);
	sum += GetColorSrc(x , y - 1, input, widthIn, heightIn) * (fy1 * fx2);
	sum += GetColorSrc(x + 1, y - 1, input, widthIn, heightIn) * (fy1 * fx3);
	
	sum += GetColorSrc(x - 1, y, input, widthIn, heightIn) * (fy2 * fx1);
	sum += GetColorSrc(x , y, input, widthIn, heightIn) * (fy2 * fx2);
	sum += GetColorSrc(x + 1, y, input, widthIn, heightIn) * (fy2 * fx3);

	sum += GetColorSrc(x - 1, y + 1, input, widthIn, heightIn) * (fy3 * fx1);
	sum += GetColorSrc(x , y + 1, input, widthIn, heightIn) * (fy3 * fx2);
	sum += GetColorSrc(x + 1, y + 1, input, widthIn, heightIn) * (fy3 * fx3);
	
    return (sum / nDenom);
}

//----------------------------------------------------------------------------
//Bicubic Interpolation
//----------------------------------------------------------------------------
__kernel void Interpolation(__global float4 *output, const __global float4 *input, int widthIn, int heightIn, int widthOut, int heightOut, int flipH, int flipV, int angle, int type)
{
	int width = widthOut;
	int height = heightOut;

    int x = get_global_id(0);
	int y = get_global_id(1);

	float ratioX = (float)widthIn / (float)width;
	float ratioY = (float)heightIn / (float)height;

	if (angle == 90)
	{
		ratioX = (float)widthIn / (float)height;
		ratioY = (float)heightIn / (float)width;
	}
	else if(angle == 270)
	{
		ratioX = (float)widthIn / (float)height;
		ratioY = (float)heightIn / (float)width;
	}		

	float posY = (float)y * ratioY;
	float posX = (float)x * ratioX;

	if (angle == 270)
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
	else if (angle == 90)
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

	int position = x + y * widthOut;

	output[position] = KernelExecution(posX, posY, input, widthIn, heightIn, type);

}

__kernel void InterpolationZone(__global float4 *output, const __global float4 *input, int widthIn, int heightIn, int widthOut, int heightOut, float left, float top, float bitmapWidth, float bitmapHeight, int flipH, int flipV, int angle, int type)
{
    int x = get_global_id(0);
	int y = get_global_id(1);

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

	if (angle == 270)
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
	else if (angle == 90)
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
		
	int position = x + y * widthOut;

	output[position] = KernelExecution(posX, posY, input, widthIn, heightIn, type);
}

