#define FILTER_RADIUS 5

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


//---------------------------------------------------------------------------
//BilateralFilter
//---------------------------------------------------------------------------
__kernel void kernelBilateralFilter(__global uint * output, const __global uint *input, int width, int height)
{
	float sum = 0.0f;
	int iResult = 255;
	float domainDist, colorDist, factor, mod;
	//float4 iTemp;
	float4 rgba, dummy, t, fResult, fTemp, fTemp1;

	int x = get_global_id(0);
	int y = get_global_id(1);
	int2 coords = (int2)(get_global_id(0), get_global_id(1));

	int tid = y * width + x;

	if ((x < FILTER_RADIUS + 1) || (y < FILTER_RADIUS + 1) ||
		(x > width - FILTER_RADIUS - 1) || (y > height - FILTER_RADIUS - 1))
	{
		output[tid] = 0;
		return;
	}

	float gaussian[11];
	gaussian[0] = gaussian[10]	= 0.45783335f;
	gaussian[1] = gaussian[9] 	= 0.60653067f;
	gaussian[2] = gaussian[8]	= 0.7548396f;
	gaussian[3] = gaussian[7]	= 0.8824969f;
	gaussian[4] = gaussian[6]	= 0.9692332f;
	gaussian[5] = 1.0f;

	// Read from Global memory.
	//float4 multiplicator = 255.0f;
	float4 value = rgbaUintToFloat4(input[tid]);
	rgba.x = value.x * 0.003921568627f;
	rgba.y = value.y * 0.003921568627f;
	rgba.z = value.z * 0.003921568627f;

	t = 0.f;

	for (int i = -FILTER_RADIUS; i <= FILTER_RADIUS; i++)
	{
		int neighborY = y + i;
		for (int j = -FILTER_RADIUS; j <= FILTER_RADIUS; j++)
		{
			int neighborX = x + j;

			domainDist = gaussian[FILTER_RADIUS + i] * gaussian[FILTER_RADIUS + j];

			// Read from Global memory.
			float4 iTemp = rgbaUintToFloat4(input[neighborY * width + neighborX]);
			fTemp.x = iTemp.x * 0.003921568627f;
			fTemp.y = iTemp.y * 0.003921568627f;
			fTemp.z = iTemp.z * 0.003921568627f;

			mod = (rgba.x - fTemp.x) * (rgba.x - fTemp.x) + 
					(rgba.y - fTemp.y) * (rgba.y - fTemp.y) + 
					(rgba.z - fTemp.z) * (rgba.z - fTemp.z);
			colorDist = (float)exp2(-mod / (2.0f * 0.25f * 0.25f));

			factor = domainDist * colorDist;
			sum += factor;
			t = t + factor * fTemp;
		}
	}

	iResult = 0;
	//int iMulFactor = 255;

	fResult = (255.0f / sum) * t;
	float4 result = rgbaUintToFloat4(input[tid]);
	result.x = (fabs(fResult.x));
	result.y = (fabs(fResult.y));
	result.z = (fabs(fResult.z));
	
	output[tid] = rgbaFloat4ToUint(result,1.0f);
	
}

//---------------------------------------------------------------------------
//bilat2_
//---------------------------------------------------------------------------
__kernel void bilat2(__global uint *output, const __global uint *input, int width,  int height,  int fSize,  float sigmaX, float sigmaP)
{

  uint i = get_global_id(0);
  uint j = get_global_id(1);
  
  int local_position = i + j * width;

  float weight;
  float res_x = 0;
  float sum_x = 0;
  
  float res_y = 0;
  float sum_y = 0;

  float res_z = 0;
  float sum_z = 0;  
  
  float4 pix0 = rgbaUintToFloat4(input[local_position]);
  float SX = 1.f/sigmaX/sigmaX;
  float SP = 1.f/sigmaP/sigmaP;

  for(int k = -fSize;k<=fSize;k++)
  {
    for(int m = -fSize;m<=fSize;m++)
	{
		int position = (i+k) + (j+m) * width;
		if(position >= 0 && position < (width * height))
		{
			float4 pix1 = rgbaUintToFloat4(input[position]);  
			
			weight = exp(-SX*(k*k+m*m))* exp(-SP*((1.f*pix0.x-pix1.x)*(1.f*pix0.x-pix1.x)));
			res_x += pix1.x*weight;
			sum_x += weight;
			
			weight = exp(-SX*(k*k+m*m))* exp(-SP*((1.f*pix0.y-pix1.y)*(1.f*pix0.y-pix1.y)));
			res_y += pix1.y*weight;
			sum_y += weight;

			weight = exp(-SX*(k*k+m*m))* exp(-SP*((1.f*pix0.z-pix1.z)*(1.f*pix0.z-pix1.z)));
			res_z += pix1.z*weight;
			sum_z += weight;
		}
    }
  }
  
  float4 result;

  result.x = (res_x/sum_x);
  result.y = (res_y/sum_y);
  result.z = (res_z/sum_z);
  
  output[local_position] = rgbaFloat4ToUint(result,1.0f);
}


//---------------------------------------------------------------------------
//bilateral_filter
//---------------------------------------------------------------------------

#define POW2(a) ((a) * (a))


//preserve => 8.0 default 0 - 100
//radius => 
__kernel void bilateral_filter(__global uint *out, const __global uint *in, int width,  int height, const float radius, const  float preserve)
{
    int gidx       = get_global_id(0);
    int gidy       = get_global_id(1);
    int n_radius   = ceil(radius);
    //int dst_width  = width;
    //int src_width  = dst_width + n_radius * 2;

    int u, v, i, j;
    float4 center_pix = rgbaUintToFloat4(in[(gidy + n_radius) * width + gidx + n_radius]);
    float4 accumulated = 0.0f;
    float4 temp       = 0.0;
    float  count       = 0.0f;
    float  diff_map, gaussian_weight, weight;

    for (v = -n_radius;v <= n_radius; ++v)
    {
        for (u = -n_radius;u <= n_radius; ++u)
        {
            i = gidx + n_radius + u;
            j = gidy + n_radius + v;

            int gid1d = i + j * width;
            temp = rgbaUintToFloat4(in[gid1d]);

            diff_map = exp (
                - (   POW2(center_pix.x - temp.x)
                    + POW2(center_pix.y - temp.y)
                    + POW2(center_pix.z - temp.z))
                * preserve);

            gaussian_weight =
                exp( - 0.5f * (POW2(u) + POW2(v)) / radius);

            weight = diff_map * gaussian_weight;

            accumulated += convert_float4(temp) * weight;
            count += weight;
        }
    }
	
	float4 value; 
    value.x = (accumulated.x / count);
	value.y = (accumulated.y / count);
	value.z = (accumulated.z / count);	
	
    out[gidx + gidy * width] = rgbaFloat4ToUint(value,1.0f);

}


//---------------------------------------------------------------------------
//nlmeans.cl
//---------------------------------------------------------------------------
__kernel void run2d(__global uint *output, const __global uint *input, int width,  int height, const int FSIZE, const int BSIZE,const float SIGMA)
{
	uint i = get_global_id(0);
	uint j = get_global_id(1);
	uint positionPicture = i + j * width;

	float4 res = 0;
	float4 sum = 0;
	float4 pix1;
	float4 p0, p1;
	
	float4 weight = 0;
	float4 dist = 0 ;
	
	float4 value1f = (float4)1.0f;
	float4 sigmaValue = (float4)SIGMA;
	float4 fSizeValue = (float4)(2*FSIZE+1);
	
	int maxPosition = width * height;
				
	
	int filterSize = BSIZE + FSIZE;
	
	if ((i < filterSize + 1) || (j < filterSize + 1) ||
		(i > width - filterSize - 1) || (j > height - filterSize - 1))
	{
		output[positionPicture] = 0;
		return;
	}
	
	int position = 0;

	for(int i1 = -BSIZE;i1<=BSIZE;i1++)
	{
		for(int j1 = -BSIZE;j1<=BSIZE;j1++)
		{
			weight = 0;
			dist = 0 ;		
			
			position = (i+i1) + (j+j1) * width;
			if(position < 0 || position >= maxPosition)
				continue;

			pix1 = rgbaUintToFloat4(input[position]);

			for(int i2 = -FSIZE;i2<=FSIZE;i2++)
			{
				for(int j2 = -FSIZE;j2<=FSIZE;j2++)
				{
					position = (i+i2) + (j+j2) * width;
					if(position < 0 || position >= maxPosition)
						continue;				
					
					position = (i+i1+i2) + (j+j1+j2) * width;
					if(position < 0 || position >= maxPosition)
						continue;						
					
					p0 = rgbaUintToFloat4(input[(i+i2) + (j+j2) * width]);
					p1 = rgbaUintToFloat4(input[(i+i1+i2) + (j+j1+j2) * width]);
					
					dist += (value1f*p1-value1f*p0)*(value1f*p1-value1f*p0);				  
				}
			}

			dist *= value1f/fSizeValue/fSizeValue;		
			weight = exp(-value1f/sigmaValue/sigmaValue*dist);		
			res += value1f*pix1*weight;		
			sum += weight;
		}
	}	
	
	float4 result = res/sum;
	float4 value = rgbaUintToFloat4(input[positionPicture]);
	result.w = value.w;
	
	output[i+j*width] = rgbaFloat4ToUint(result,1.0f);

  
}
