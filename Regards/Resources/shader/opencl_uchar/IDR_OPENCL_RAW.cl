//----------------------------------------------------
// Execution du wavelet denoise
//----------------------------------------------------

__kernel void WaveletDenoiseByColSetValue(__global float4 *output, const __global float4 * input, int iwidth, int iheight, int lpass, int hpass, int sc) 
{ 
	int col = get_global_id(0);
	int row = get_global_id(1);

	if(col < sc)
	{
		output[lpass + row*iwidth + col] = ((float4)2*input[hpass+row*iwidth + col] + input[hpass+row*iwidth + (sc-col)] + input[hpass+row*iwidth + (col+sc)]) * (float4)0.25;
	}
	else if(col+sc < iwidth)
	{
		output[lpass + row*iwidth + col] = ((float4)2*input[hpass+row*iwidth + col] + input[hpass+row*iwidth + (col - sc)] + input[hpass+row*iwidth + (col+sc)]) * (float4)0.25;
	}
	else if(col < iwidth)
	{
		output[lpass + row*iwidth + col] = ((float4)2*input[hpass+row*iwidth + col] + input[hpass+row*iwidth + (col-sc)] + input[hpass+row*iwidth + (2*iwidth-2-(col+sc))]) * (float4)0.25;	
	}
	
}


ushort GetValue(float calcul)
{
	ushort value;
	if(calcul < 0)
		value = 0;
	else if(calcul > 65535)
		value = 65535;
	else
		value = (ushort)calcul;
	
	return value;
}

//----------------------------------------------------
//----------------------------------------------------
__kernel void OutputData(__global ushort4 *output, const __global float4 * input, int lpass) 
{ 
	int i = get_global_id(0);
	float4 value = ((input[i]+input[lpass+i]) * (input[i]+input[lpass+i]))/(float4)(0x10000);
	output[i].x = GetValue(value.x);
	output[i].y = GetValue(value.y);
	output[i].z = GetValue(value.z);
	output[i].w = GetValue(value.w);
}

//----------------------------------------------------
//----------------------------------------------------
__kernel void GetAlphaChannel(__global float * output, const __global float4 * input, int lpass) 
{ 
	int i = get_global_id(0);
	output[i] = input[i].w;
}


//----------------------------------------------------
//----------------------------------------------------
__kernel void InitData(__global float4 *output, const __global ushort4 * input, int scale) 
{ 
	int i = get_global_id(0);
	output[i].x = (float)(256 * sqrt((float)(input[i].x << scale)));
	output[i].y = (float)(256 * sqrt((float)(input[i].y << scale)));
	output[i].z = (float)(256 * sqrt((float)(input[i].z << scale)));
	output[i].w = (float)(256 * sqrt((float)(input[i].w << scale)));
}

//----------------------------------------------------
//----------------------------------------------------
__kernel void WaveletDenoiseByRowSetValue(__global float4 *output, const __global float4 * input, int iwidth, int iheight, int lpass, int sc) 
{ 
	int col = get_global_id(0);
	int row = get_global_id(1);

	if(row < sc)
	{
		output[lpass + row*iwidth + col] = ((float4)2*input[lpass+col + iwidth*row] + input[lpass+col +iwidth*(sc-row)] + input[lpass+col +iwidth*(row+sc)]) * (float4)0.25;
	}
	else if(row+sc < iheight)
	{
		output[lpass + row*iwidth + col] = ((float4)2*input[lpass+col +iwidth*row] + input[lpass+col +iwidth*(row-sc)] + input[lpass+col +iwidth*(row+sc)]) * (float4)0.25;
	}
	else if(row < iheight)
	{
		output[lpass + row*iwidth + col] = ((float4)2*input[lpass+col +iwidth*row] + input[lpass+col +iwidth*(row-sc)] + input[lpass+col +iwidth*(2*iheight-2-(row+sc))]) * (float4)0.25;	
	}
}

float NormalizeThold(float value, float thold)
{
	float output = value;
	if	(output < -thold) 
		output += thold;
	else if (output >  thold) 
		output -= thold;
	else	 
		output = 0;
		
	return output;
}

//----------------------------------------------------
//----------------------------------------------------
__kernel void WaveletDenoiseNormalize(__global float4 *output, int hpass, int lpass, float thold) 
{ 
    int i = get_global_id(0);
	output[hpass+i] -= output[lpass+i];
	output[hpass+i].x = NormalizeThold(output[hpass+i].x,thold);
	output[hpass+i].y = NormalizeThold(output[hpass+i].y,thold);
	output[hpass+i].z = NormalizeThold(output[hpass+i].z,thold);
	output[hpass+i].w = NormalizeThold(output[hpass+i].w,thold);

	if (hpass) 
		output[i] += output[hpass+i];
		
}

