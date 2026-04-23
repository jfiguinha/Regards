//Implementation de l'interpolation bicubic en OpenCL
#define FILTER_PI 3.1415926535f
#define FILTER_2PI 2.0f * 3.1415926535f
#define FILTER_4PI 4.0f * 3.1415926535f

// Inline device function to convert 32-bit unsigned integer to floating point rgba color 
//*****************************************************************
inline float4 rgbaUintToFloat4(uint c)
{
    float4 rgba;
    rgba.x = c & 0xff;
    rgba.y = (c >> 8) & 0xff;
    rgba.z = (c >> 16) & 0xff;
    rgba.w = (c >> 24) & 0xff;
    return rgba;
}

// Inline device function to convert floating point rgba color to 32-bit unsigned integer
//*****************************************************************
inline uint rgbaFloat4ToUint(float4 rgba, float fScale)
{
    unsigned int uiPackedPix = 0U;
    uiPackedPix |= 0x000000FF & (unsigned int)(rgba.x * fScale);
    uiPackedPix |= 0x0000FF00 & (((unsigned int)(rgba.y * fScale)) << 8);
    uiPackedPix |= 0x00FF0000 & (((unsigned int)(rgba.z * fScale)) << 16);
    uiPackedPix |= 0xFF000000 & (((unsigned int)(rgba.w * fScale)) << 24);
    return uiPackedPix;
}

// Pre-computed filter functions inline - avoid function call overhead
inline float KernelFilter_selection(float x, int type)
{
    switch(type)
    {
        case 1: // BboxFilter
            return (x <= 0.5f ? 1.0f : 0.0f);
            
        case 2: // HermiteFilter
            if (x > 1.5f) return 0.0f;
            if (x < -1.0f) return 0.0f;
            if (x < 0.0f) return (2.0f*(-x)-3.0f)*(-x)*(-x)+1.0f;
            if (x < 1.0f) return (2.0f*x-3.0f)*x*x+1.0f;
            return 0.0f;
            
        case 3: // HanningFilter
            if (x > 1.0f) return 0.0f;
            return 0.54f + 0.46f*cos(FILTER_PI*x);
            
        case 4: // CatromFilter
            if (x > 2.0f) return 0.0f;
            if (x < -2.0f) return 0.0f;
            if (x < -1.0f) return 0.5f*(4.0f+x*(8.0f+x*(5.0f+x)));
            if (x < 0.0f) return 0.5f*(2.0f+x*x*(-5.0f-3.0f*x));
            if (x < 1.0f) return 0.5f*(2.0f+x*x*(-5.0f+3.0f*x));
            if (x < 2.0f) return 0.5f*(4.0f+x*(-8.0f+x*(5.0f-x)));
            return 0.0f;
            
        case 5: // MitchellFilter
            if (x > 2.0f) return 0.0f;
            #define B   (1.0f/3.0f)
            #define C   (1.0f/3.0f)
            #define P0  ((  6.0f- 2.0f*B       )/6.0f)
            #define P2  ((-18.0f+12.0f*B+ 6.0f*C)/6.0f)
            #define P3  (( 12.0f- 9.0f*B- 6.0f*C)/6.0f)
            #define Q0  ((       8.0f*B+24.0f*C)/6.0f)
            #define Q1  ((     -12.0f*B-48.0f*C)/6.0f)
            #define Q2  ((       6.0f*B+30.0f*C)/6.0f)
            #define Q3  ((     - 1.0f*B- 6.0f*C)/6.0f)
            if (x < -2.0f) return 0.0f;
            if (x < -1.0f) return Q0-x*(Q1-x*(Q2-x*Q3));
            if (x < 0.0f) return P0+x*x*(P2-x*P3);
            if (x < 1.0f) return P0+x*x*(P2+x*P3);
            if (x < 2.0f) return Q0+x*(Q1+x*(Q2+x*Q3));
            return 0.0f;
            
        case 6: // TriangleFilter
            if (x > 1.0f) return 0.0f;
            if (x < -1.0f) return 0.0f;
            if (x < 0.0f) return 1.0f+x;
            return 1.0f-x;
            
        case 7: // QuadraticFilter
            if (x > 1.5f) return 0.0f;
            if (x < -1.5f) return 0.0f;
            if (x < -0.5f) return 0.5f*(x+1.5f)*(x+1.5f);
            if (x < 0.5f) return 0.75f-x*x;
            return 0.5f*(x-1.5f)*(x-1.5f);
            
        case 8: // BlackmanFilter
            if (x > 1.0f) return 0.0f;
            float dN = 3.0f;
            return 0.42f + 0.5f * cos(FILTER_2PI * x / ( dN - 1.0f )) + 0.08f * cos (FILTER_4PI * x / ( dN - 1.0f ));
            
        case 9: // HammingFilter
            if (x > 1.0f) return 0.0f;
            float dWindow = 0.54f + 0.46f * cos (FILTER_2PI * x);
            float dSinc = (x == 0) ? 1.0f : sin (FILTER_PI * x) / (FILTER_PI * x);
            return dWindow * dSinc;
            
        case 10: // GaussianFilter
            if (fabs(x) > 1.25f) return 0.0f;
            return exp(-x * x / 2.0f) / sqrt (FILTER_2PI);
            
        case 11: // BilinearFilter
            return (x < 1.0f ? 1.0f - x : 0.0f);
            
        default: // CubicFilter
            if (x > 2.0f) return 0.0f;
            if (x < -2.0f) return 0.0f;
            if (x < -1.0f) return (2.0f+x)*(2.0f+x)*(2.0f+x)/6.0f;
            if (x < 0.0f) return (4.0f+x*x*(-6.0f-3.0f*x))/6.0f;
            if (x < 1.0f) return (4.0f+x*x*(-6.0f+3.0f*x))/6.0f;
            if (x < 2.0f) return (2.0f-x)*(2.0f-x)*(2.0f-x)/6.0f;
            return 0.0f;
    }
}


inline float4 GetColorSrc(int x, int y, const __global uint *input, int widthIn, int heightIn)
{
	if(x < widthIn && y < heightIn && y >= 0 && x >= 0)	
	{
		int position = x + y * widthIn;
		return rgbaUintToFloat4(input[position]);
	}
	return (float4)0.0f;
}

inline uint GetColorSrc_short(int x, int y, const __global uint *input, int widthIn, int heightIn)
{
	if(x < widthIn && y < heightIn && y >= 0 && x >= 0)	
	{
		int position = x + y * widthIn;
		return input[position];
	}
	return 0;
}


//***********************************************************************************************************
//Two Pass Filter
//***********************************************************************************************************
// Helper: Horizontal pass - interpolate across 3 pixels in X direction
inline float4 FilterHorizontal(float x, int y, const __global uint *input, int widthIn, int heightIn, int type)
{
	int valueA = (int)x;
	float realA = x - valueA;

	float fx1 = KernelFilter_selection(-1.0f - realA, type);
	float fx2 = KernelFilter_selection(-realA, type);
	float fx3 = KernelFilter_selection(1.0f - realA, type);

	float fxSum = fx1 + fx2 + fx3;
	float fxInv = 1.0f / fxSum;

	float4 left = GetColorSrc(valueA - 1, y, input, widthIn, heightIn) * (fx1 * fxInv);
	float4 center = GetColorSrc(valueA, y, input, widthIn, heightIn) * (fx2 * fxInv);
	float4 right = GetColorSrc(valueA + 1, y, input, widthIn, heightIn) * (fx3 * fxInv);

	return left + center + right;
}

// Helper: Vertical pass - interpolate across 3 pixels in Y direction using mix
inline float4 FilterVertical(float4 row1, float4 row2, float4 row3, float y, int type)
{
	int valueB = (int)y;
	float realB = y - valueB;

	float fy1 = KernelFilter_selection(-(-1.0f - realB), type);
	float fy2 = KernelFilter_selection(realB, type);
	float fy3 = KernelFilter_selection(-(1.0f - realB), type);

	float fySum = fy1 + fy2 + fy3;
	float fyInv = 1.0f / fySum;

	float wny1 = fy1 * fyInv;
	float wny2 = fy2 * fyInv;
	float wny3 = fy3 * fyInv;

	return mix(mix(row1, row2, wny2), row3, wny3);
}

// Two-pass separable filter interpolation
inline uint KernelExecution(float x, float y, const __global uint *input, int widthIn, int heightIn, int type)
{
	int valueB = (int)y;

	// Pass 1: Horizontal filtering on 3 rows
	float4 row1 = FilterHorizontal(x, valueB - 1, input, widthIn, heightIn, type);
	float4 row2 = FilterHorizontal(x, valueB, input, widthIn, heightIn, type);
	float4 row3 = FilterHorizontal(x, valueB + 1, input, widthIn, heightIn, type);

	// Pass 2: Vertical filtering using mix
	float4 result = FilterVertical(row1, row2, row3, y, type);

	return rgbaFloat4ToUint(result, 1.0f);
}


/*
//***********************************************************************************************************
//Old Pass Filter
//***********************************************************************************************************
inline uint KernelExecution(float x, float y, const __global uint *input, int widthIn, int heightIn, int type)
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
	
    return rgbaFloat4ToUint((sum / nDenom),1.0f);
}
*/

/*
//***********************************************************************************************************
//Optimize Pass Filter
//***********************************************************************************************************
inline uint KernelExecution(float x, float y, const __global uint *input, int widthIn, int heightIn, int type)
{
	int valueA = (int)x;
	int valueB = (int)y;
	float realA = x - valueA;
	float realB = y - valueB;

	float fy1 = KernelFilter_selection(-(-1.0f - realB), type);
	float fy2 = KernelFilter_selection(realB, type);
	float fy3 = KernelFilter_selection(-(1.0f - realB), type);

	float fx1 = KernelFilter_selection(-1.0f - realA, type);
	float fx2 = KernelFilter_selection(- realA, type);
	float fx3 = KernelFilter_selection(1.0f - realA, type);

	float fxSum = fx1 + fx2 + fx3;
	float fDenom = fy1 * fxSum + fy2 * fxSum + fy3 * fxSum;
	float fInvDenom = 1.0f / fDenom;

	float wnx1 = fx1 * fInvDenom;
	float wnx2 = fx2 * fInvDenom;
	float wnx3 = fx3 * fInvDenom;

	float4 row1 = GetColorSrc(valueA - 1, valueB - 1, input, widthIn, heightIn) * wnx1
				+ GetColorSrc(valueA, valueB - 1, input, widthIn, heightIn) * wnx2
				+ GetColorSrc(valueA + 1, valueB - 1, input, widthIn, heightIn) * wnx3;

	float4 row2 = GetColorSrc(valueA - 1, valueB, input, widthIn, heightIn) * wnx1
				+ GetColorSrc(valueA, valueB, input, widthIn, heightIn) * wnx2
				+ GetColorSrc(valueA + 1, valueB, input, widthIn, heightIn) * wnx3;

	float4 row3 = GetColorSrc(valueA - 1, valueB + 1, input, widthIn, heightIn) * wnx1
				+ GetColorSrc(valueA, valueB + 1, input, widthIn, heightIn) * wnx2
				+ GetColorSrc(valueA + 1, valueB + 1, input, widthIn, heightIn) * wnx3;

	float wny1 = fy1 * fInvDenom / fxSum;
	float wny2 = fy2 * fInvDenom / fxSum;
	float wny3 = fy3 * fInvDenom / fxSum;

	float4 sum = mix(mix(row1, row2, wny2), row3, wny3);

	return rgbaFloat4ToUint(sum, 1.0f);
}
*/

inline uint CalculInterpolation(const __global uint *input, int widthIn, int heightIn, int widthOut, int heightOut, int flipH, int flipV, int angle, int type, float ratioX, float ratioY, int x, int y, float left, float top)
{
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

	
	if(type == 12)
		return GetColorSrc_short(posX, posY, input, widthIn, heightIn);
	return KernelExecution(posX, posY, input, widthIn, heightIn, type);
}

//----------------------------------------------------------------------------
//Interpolation
//----------------------------------------------------------------------------
__kernel void Interpolation(__global uint *output, const __global uint *input, int widthIn, int heightIn, int widthOut, int heightOut, int flipH, int flipV, int angle, int type)
{
	int width = widthOut;
	int height = heightOut;

    int x = get_global_id(0);
	int y = get_global_id(1);

    if(x < width && y < height && y >= 0 && x >= 0)	
    {
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

		int position = x + y * widthOut;
		output[position] = CalculInterpolation(input, widthIn, heightIn, widthOut, heightOut, flipH, flipV, angle, type, ratioX, ratioY, x, y, 0, 0);
	}
	barrier(CLK_GLOBAL_MEM_FENCE);
}

__kernel void InterpolationZone(__global uint *output, const __global uint *input, int widthIn, int heightIn, int widthOut, int heightOut, float left, float top, float bitmapWidth, float bitmapHeight, int flipH, int flipV, int angle, int type)
{
    int x = get_global_id(0);
	int y = get_global_id(1);

    if(x < widthOut && y < heightOut && y >= 0 && x >= 0)	
    {
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

		int position = x + y * widthOut;
		output[position] = CalculInterpolation(input, widthIn, heightIn, widthOut, heightOut, flipH, flipV, angle, type, ratioX, ratioY, x, y, left, top);
	}
	barrier(CLK_GLOBAL_MEM_FENCE);
}


__kernel void InterpolationDirect(__global uint *output, const __global uint *input, int widthIn, int heightIn, int widthOut, int heightOut, int type)
{
    int x = get_global_id(0);
	int y = get_global_id(1);

    if(x < widthOut && y < heightOut && y >= 0 && x >= 0)	
    {
		float ratioX = (float)widthIn / widthOut;
		float ratioY = (float)heightIn / heightOut;
		int position = x + y * widthOut;
		float posX = (float)x * ratioX;
		float posY = (float)y * ratioY;
		
		if(type == 12)
			output[position] = GetColorSrc_short(posX, posY, input, widthIn, heightIn);
		else
			output[position] = KernelExecution(posX, posY, input, widthIn, heightIn, type);
	}
	barrier(CLK_GLOBAL_MEM_FENCE);
}

