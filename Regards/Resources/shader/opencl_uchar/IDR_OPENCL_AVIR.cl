//----------------------------------------------------
//Conversion d'un bitmap en wxImage
//----------------------------------------------------
__constant float tbl[256] = {
		0.0f, 0.000303527f, 0.000607054f, 0.000910581f, 0.001214108f,
		0.001517635f, 0.001821162f, 0.002124689f, 0.002428216f, 0.002731743f,
		0.00303527f, 0.003348383f, 0.003678029f, 0.004025973f, 0.004392482f,
		0.004777817f, 0.005182236f, 0.005605992f, 0.006049334f, 0.006512507f,
		0.006995751f, 0.007499306f, 0.008023403f, 0.008568275f, 0.009134147f,
		0.009721244f, 0.01032979f, 0.01095999f, 0.01161207f, 0.01228625f,
		0.01298271f, 0.01370169f, 0.01444337f, 0.01520795f, 0.01599565f,
		0.01680664f, 0.01764113f, 0.01849931f, 0.01938136f, 0.02028748f,
		0.02121784f, 0.02217263f, 0.02315203f, 0.02415622f, 0.02518537f,
		0.02623966f, 0.02731927f, 0.02842436f, 0.0295551f, 0.03071166f,
		0.0318942f, 0.0331029f, 0.03433791f, 0.03559939f, 0.03688751f,
		0.03820243f, 0.03954429f, 0.04091326f, 0.04230949f, 0.04373313f,
		0.04518433f, 0.04666325f, 0.04817003f, 0.04970482f, 0.05126777f,
		0.05285903f, 0.05447873f, 0.05612702f, 0.05780404f, 0.05950994f,
		0.06124485f, 0.06300892f, 0.06480227f, 0.06662506f, 0.0684774f,
		0.07035945f, 0.07227132f, 0.07421317f, 0.07618511f, 0.07818728f,
		0.08021981f, 0.08228283f, 0.08437647f, 0.08650086f, 0.08865612f,
		0.09084239f, 0.09305977f, 0.09530841f, 0.09758843f, 0.09989995f,
		0.1022431f, 0.104618f, 0.1070247f, 0.1094634f, 0.1119343f,
		0.1144373f, 0.1169728f, 0.1195406f, 0.1221411f, 0.1247742f,
		0.1274402f, 0.1301391f, 0.132871f, 0.1356361f, 0.1384344f,
		0.1412662f, 0.1441314f, 0.1470303f, 0.1499628f, 0.1529292f,
		0.1559296f, 0.158964f, 0.1620326f, 0.1651354f, 0.1682726f,
		0.1714443f, 0.1746506f, 0.1778916f, 0.1811674f, 0.1844781f,
		0.1878239f, 0.1912047f, 0.1946208f, 0.1980722f, 0.2015591f,
		0.2050815f, 0.2086396f, 0.2122334f, 0.215863f, 0.2195286f,
		0.2232303f, 0.2269681f, 0.2307422f, 0.2345526f, 0.2383994f,
		0.2422828f, 0.2462029f, 0.2501597f, 0.2541534f, 0.258184f,
		0.2622517f, 0.2663564f, 0.2704985f, 0.2746778f, 0.2788946f,
		0.2831489f, 0.2874409f, 0.2917705f, 0.296138f, 0.3005433f,
		0.3049867f, 0.3094681f, 0.3139877f, 0.3185456f, 0.3231419f,
		0.3277766f, 0.3324498f, 0.3371618f, 0.3419124f, 0.3467019f,
		0.3515303f, 0.3563977f, 0.3613041f, 0.3662498f, 0.3712348f,
		0.3762591f, 0.3813228f, 0.3864261f, 0.391569f, 0.3967517f,
		0.4019741f, 0.4072364f, 0.4125387f, 0.4178811f, 0.4232636f,
		0.4286864f, 0.4341495f, 0.4396529f, 0.4451969f, 0.4507815f,
		0.4564067f, 0.4620726f, 0.4677794f, 0.4735271f, 0.4793158f,
		0.4851456f, 0.4910165f, 0.4969287f, 0.5028822f, 0.5088771f,
		0.5149135f, 0.5209915f, 0.5271111f, 0.5332725f, 0.5394757f,
		0.5457208f, 0.5520078f, 0.558337f, 0.5647082f, 0.5711217f,
		0.5775775f, 0.5840756f, 0.5906162f, 0.5971993f, 0.6038251f,
		0.6104935f, 0.6172047f, 0.6239587f, 0.6307557f, 0.6375956f,
		0.6444787f, 0.6514048f, 0.6583742f, 0.665387f, 0.6724431f,
		0.6795426f, 0.6866857f, 0.6938724f, 0.7011027f, 0.7083769f,
		0.7156948f, 0.7230567f, 0.7304625f, 0.7379124f, 0.7454064f,
		0.7529446f, 0.7605271f, 0.7681539f, 0.7758252f, 0.7835409f,
		0.7913012f, 0.7991061f, 0.8069558f, 0.8148502f, 0.8227894f,
		0.8307736f, 0.8388028f, 0.846877f, 0.8549964f, 0.8631609f,
		0.8713707f, 0.8796259f, 0.8879265f, 0.8962726f, 0.9046642f,
		0.9131014f, 0.9215843f, 0.930113f, 0.9386874f, 0.9473078f,
		0.9559742f, 0.9646865f, 0.973445f, 0.9822496f, 0.9911004f,
		0.9999975f };

inline float convertSRGB2Lin(uint s0)
{
    return tbl[s0];
}

float4 ucharTofloat(uchar4 input)
{
	float4 output = (float4)0.0;
    output.x = tbl[input.x];
    output.y = tbl[input.y];
    output.z = tbl[input.z];
    output.w = tbl[input.w];	
	return output;
}
	
__kernel void ConvertToFloat(__global float4 * output, const __global uchar4 *input, int width, int height)
{
    int x = get_global_id(0);
    int y = get_global_id(1);

    if (x < width && y < height)
    {
        int position = x + y * width;
        output[position] = ucharTofloat(input[position]);
    }
}

__kernel void UpSample2DUchar(__global float4 * output, const __global uchar4 *input, int width, int height, int widthSrc, int start, int outLen, int ResampleFactor)
{
	int k = get_global_id(0);
	int i = get_global_id(1);

	if (k < width && i < height) 
	{
		int pos = k + i * width;
		output[pos] = (float4)(0.0f);

		int posSrc = i * widthSrc;
		if (k < start) 
		{
			output[pos] = ucharTofloat(input[posSrc]);
		} 
		else if (k < (outLen + start)) 
		{
			int kInput = (k - start) / ResampleFactor + posSrc;
			output[pos] = ucharTofloat(input[kInput]);
		} 
		else 
		{
			output[pos] = ucharTofloat(input[widthSrc - 1 + posSrc]);
		}
	}
}

__kernel void UpSample2D(__global float4 * output, const __global float4 *input, int width, int height, int widthSrc, int start, int outLen, int ResampleFactor)
{
	int k = get_global_id(0);
	int i = get_global_id(1);

	if (k < width && i < height) 
	{
		int pos = k + i * width;
		output[pos] = (float4)(0.0f);

		int posSrc = i * widthSrc;
		if (k < start) 
		{
			output[pos] = input[posSrc];
		} 
		else if (k < (outLen + start)) 
		{
			int kInput = (k - start) / ResampleFactor + posSrc;
			output[pos] = input[kInput];
		} 
		else 
		{
			output[pos] = input[widthSrc - 1 + posSrc];
		}
	}
}

__kernel void doResize22D(__global float4 * output, const __global float4 *input, int width, int height, __global const int * PositionTab, __global const float* ftp,
    const int IntFltLen0, int inputWidth)
{
	int k = get_global_id(0);
	int j = get_global_id(1);

	if (k < width && j < height) 
	{
		float4 sum = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
		int positionSrc = PositionTab[k] / 4;

		for (int i = 0; i < IntFltLen0 / 2; i++) 
		{
			const float xx = ftp[i + k * IntFltLen0 / 2];
			if (positionSrc < inputWidth && positionSrc >= 0)  
			{
				int localPos = positionSrc + j * inputWidth;
				float4 pixel = input[localPos];
				sum += xx * pixel;
			}
			positionSrc += 2;
		}

		output[k + j * width] = sum;
	}
}

__kernel void doFilter2D(__global float4 * output, const __global float4 *input, int widthSrc, int heightSrc, int width, int height, __global const float* f, const int flen, const int step)
{
	int k = get_global_id(0);
	int j = get_global_id(1);

	if (k < width && j < height) 
	{
		float4 sum = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
		int position = min(k * step + j * widthSrc, heightSrc * widthSrc - 1);

		// Pré-calculer la première valeur pour éviter de la répéter dans la boucle
		float4 inputVal = input[position];
		sum = f[0] * inputVal;

		for (int i = 1; i < flen; i++) 
		{
			int pos1 = position + i;
			int pos2 = max(position - i, 0); // Utilisation de max() pour éviter les indices négatifs

			float4 ip1 = input[pos1];
			float4 ip2 = input[pos2];

			// Accumuler les contributions des deux positions
			sum += f[i] * (ip1 + ip2);
		}

		// Écriture dans la mémoire globale
		output[k + j * width] = sum;
	}
}

__kernel void doFilter2DUchar(__global float4 * output, const __global uchar4 *input, int widthSrc, int heightSrc, int width, int height, __global const float* f, const int flen, const int step)
{
	int k = get_global_id(0);
	int j = get_global_id(1);
	
	if (k < width && j < height) 
	{
		float4 sum = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
		int position = min(k * step + j * widthSrc, heightSrc * widthSrc - 1);

		// Pré-calculer la première valeur pour éviter de la répéter dans la boucle
		float4 inputVal = ucharTofloat(input[position]);
		sum = f[0] * inputVal;

		for (int i = 1; i < flen; i++) 
		{
			int pos1 = position + i;
			int pos2 = max(position - i, 0); // Utilisation de max() pour éviter les indices négatifs

			float4 ip1 = ucharTofloat(input[pos1]);
			float4 ip2 = ucharTofloat(input[pos2]);

			// Accumuler les contributions des deux positions
			sum += f[i] * (ip1 + ip2);
		}

		// Écriture dans la mémoire globale
		output[k + j * width] = sum;
	}
}


inline float pow24i_sRGB(float x0)
{
	const double x = (double)x0;
	const double sx = sqrt(x);
	const double ssx = sqrt(sx);
	const double sssx = sqrt(ssx);

	return((float)(0.000213364515060263 + 0.0149409239419218 * x +
		0.433973412731747 * sx + ssx * (0.659628181609715 * sssx -
			0.0380957908841466 - 0.0706476137208521 * sx)));
}

float convertLin2SRGB(float s)
{
	float a = (float)0.055;

	if (s <= (float)0.0031308)
	{
		return((float)12.92 * s);
	}

	return(((float)1 + a) * pow24i_sRGB(s) - a);
}


__kernel void doResize2D(__global float4 * output, const __global float4 *input, int width, int height, __global const int * PositionTab, int tabSize, __global const float* ftp,
    const int IntFltLen0, int inputWidth)
{
	int k = get_global_id(0);
	int j = get_global_id(1);

	if (k < width && j < height) 
	{
		float4 sum = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
		if(k < tabSize)
		{
			int positionSrc = PositionTab[k] / 4;
			int baseIndex = j * inputWidth;

			for (int i = 0; i < IntFltLen0; i++) 
			{
				if (positionSrc < inputWidth && positionSrc >= 0) 
				{
					float xx = ftp[i + k * IntFltLen0];
					int localPos = positionSrc + baseIndex;
					float4 pixel = input[localPos];
					sum += xx * pixel;
				}
				positionSrc++;
			}
		}

		output[k + j * width] = sum;
	}
}


__kernel void doResize2DUchar(__global float4 * output, const __global uchar4 *input, int width, int height, __global const int * PositionTab, int tabSize, __global const float* ftp,
    const int IntFltLen0, int inputWidth)
{
	int k = get_global_id(0);
	int j = get_global_id(1);

	if (k < width && j < height) 
	{
		float4 sum = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
		if(k < tabSize)
		{
			int positionSrc = PositionTab[k] / 4;
			int baseIndex = j * inputWidth;

			for (int i = 0; i < IntFltLen0; i++) 
			{
				if (positionSrc < inputWidth && positionSrc >= 0) 
				{
					float xx = ftp[i + k * IntFltLen0];
					int localPos = positionSrc + baseIndex;
					float4 pixel = ucharTofloat(input[localPos]);
					sum += xx * pixel;
				}
				positionSrc++;
			}
		}


		output[k + j * width] = sum;
	}
}

__kernel void GetDataHtoV2D(__global float4 * output, const __global float4 *input, int width, int height)
{
    int k = get_global_id(0);
	int j = get_global_id(1);
	if(k < width && j < height && j >= 0 && k >= 0)	
	{
	
		int widthDest = height;
		int heightDest = width;

		int position = k + j * width;
		int outpos = j + k * widthDest;
		output[outpos] = input[position];

	}
}


__kernel void GetDataHtoVDither2D(__global uchar4 * output, const __global float4 *input, int width, int height, float gm, float PkOut, float TrMul0)
{
	int k = get_global_id(0); // Index global
	int xPos = get_global_id(1);

	if (k >= height) 
		return;

	int position = xPos + k * width;
	int outpos = k + xPos * height;

	float4 inValue = input[position];
	float4 outValue = {
		convertLin2SRGB(inValue.x) * gm,
		convertLin2SRGB(inValue.y) * gm,
		convertLin2SRGB(inValue.z) * gm,
		convertLin2SRGB(inValue.w) * gm
	};

	const float c0 = 0;

	if (TrMul0 == 1.0f) {
		output[outpos].x = (uchar)(clamp(round(outValue.x), c0, PkOut));
		output[outpos].y = (uchar)(clamp(round(outValue.y), c0, PkOut));
		output[outpos].z = (uchar)(clamp(round(outValue.z), c0, PkOut));
		output[outpos].w = (uchar)(clamp(round(outValue.w), c0, PkOut));
	} else {
		const float TrMul = (float)TrMul0;
		const float TrMulI = 1.0f / TrMul;

		output[outpos].x = (uchar)(clamp(round(outValue.x * TrMulI) * TrMul, c0, PkOut));
		output[outpos].y = (uchar)(clamp(round(outValue.y * TrMulI) * TrMul, c0, PkOut));
		output[outpos].z = (uchar)(clamp(round(outValue.z * TrMulI) * TrMul, c0, PkOut));
		output[outpos].w = (uchar)(clamp(round(outValue.w * TrMulI) * TrMul, c0, PkOut));
	}

}



__kernel void doFilter2DLastStep(__global uchar4 * output, const __global float4 *input, int widthSrc, int heightSrc, int width, int height, __global const float* f, const int flen, const int step, float gm, float PkOut, float TrMul0)
{
	int k = get_global_id(0);
	int j = get_global_id(1);

	if (k < widthSrc && j < heightSrc) 
	{
		float4 sum = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
		int position = min(k * step + j * widthSrc, heightSrc * widthSrc - 1);

		// Pré-calculer la première valeur pour éviter de la répéter dans la boucle
		float4 inputVal = input[position];
		sum = f[0] * inputVal;

		for (int i = 1; i < flen; i++) 
		{
			int pos1 = position + i;
			int pos2 = max(position - i, 0); // Utilisation de max() pour éviter les indices négatifs

			float4 ip1 = input[pos1];
			float4 ip2 = input[pos2];

			// Accumuler les contributions des deux positions
			sum += f[i] * (ip1 + ip2);
		}

		// Écriture dans la mémoire globale
		//output[k + j * width] = sum;
		float4 inValue = sum;
		float4 outValue = {
			convertLin2SRGB(inValue.x) * gm,
			convertLin2SRGB(inValue.y) * gm,
			convertLin2SRGB(inValue.z) * gm,
			convertLin2SRGB(inValue.w) * gm
		};

		const float c0 = 0;
		uchar4 outputValue = (uchar4)0;

		if (TrMul0 == 1.0f) {
			outputValue.x = (uchar)(clamp(round(outValue.x), c0, PkOut));
			outputValue.y = (uchar)(clamp(round(outValue.y), c0, PkOut));
			outputValue.z = (uchar)(clamp(round(outValue.z), c0, PkOut));
			outputValue.w = (uchar)(clamp(round(outValue.w), c0, PkOut));
		} else {
			const float TrMul = (float)TrMul0;
			const float TrMulI = 1.0f / TrMul;

			outputValue.x = (uchar)(clamp(round(outValue.x * TrMulI) * TrMul, c0, PkOut));
			outputValue.y = (uchar)(clamp(round(outValue.y * TrMulI) * TrMul, c0, PkOut));
			outputValue.z = (uchar)(clamp(round(outValue.z * TrMulI) * TrMul, c0, PkOut));
			outputValue.w = (uchar)(clamp(round(outValue.w * TrMulI) * TrMul, c0, PkOut));
		}
		
		output[j + k * width] = outputValue;
	}
}


__kernel void doFilter2DV(__global float4 * output, const __global float4 *input, int widthSrc, int heightSrc, int width, int height, __global const float* f, const int flen, const int step)
{
	int k = get_global_id(0);
	int j = get_global_id(1);

	if (k < widthSrc && j < heightSrc) 
	{
		float4 sum = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
		int position = min(k * step + j * widthSrc, heightSrc * widthSrc - 1);

		// Pré-calculer la première valeur pour éviter de la répéter dans la boucle
		float4 inputVal = input[position];
		sum = f[0] * inputVal;

		for (int i = 1; i < flen; i++) 
		{
			int pos1 = position + i;
			int pos2 = max(position - i, 0); // Utilisation de max() pour éviter les indices négatifs

			float4 ip1 = input[pos1];
			float4 ip2 = input[pos2];

			// Accumuler les contributions des deux positions
			sum += f[i] * (ip1 + ip2);
		}

		// Écriture dans la mémoire globale
		output[j + k * width] = sum;
	}
}