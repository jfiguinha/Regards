#define INV_SQRT_OF_2PI 0.39894228040143267793994605993439
#define INV_PI 0.31830988618379067153776752674503

float4 GetColorSrc(int x, int y, const __global float4 *input, int widthIn, int heightIn)
{
	if(x < widthIn && y < heightIn && y >= 0 && x >= 0)	
	{
		int position = x + y * widthIn;
		return input[position];
	}
	return 0.0f;
}

float4 DenoiseFilter(const __global float4 *input, int x, int y, int widthIn, int heightIn, float sigma, float threshold, float kSigma)
{
    float radius = round(kSigma*sigma);
    float radQ = radius * radius;

    float invSigmaQx2 = .5 / (sigma * sigma);      // 1.0 / (sigma^2 * 2.0)
    float invSigmaQx2PI = INV_PI * invSigmaQx2;    // // 1/(2 * PI * sigma^2)

    float invThresholdSqx2 = .5 / (threshold * threshold);     // 1.0 / (sigma^2 * 2.0)
    float invThresholdSqrt2PI = INV_SQRT_OF_2PI / threshold;   // 1.0 / (sqrt(2*PI) * sigma)

	float4 centrPx = GetColorSrc(x,y,input,widthIn,heightIn);

    float zBuff = 0.0;
    float4 aBuff = 0.0;

    int2 d;
    for (d.x=-radius; d.x <= radius; d.x++) {
        float pt = sqrt(radQ-d.x*d.x);       // pt = yRadius: have circular trend
        for (d.y=-pt; d.y <= pt; d.y++) {
            
			float2 fd;
			
			float blurFactor = exp( -dot(fd , fd) * invSigmaQx2 ) * invSigmaQx2PI;

			float4 walkPx = GetColorSrc(x + d.x, y + d.y,input,widthIn,heightIn);
            float4 dC = walkPx-centrPx;
            float deltaFactor = exp( -dot(dC, dC) * invThresholdSqx2) * invThresholdSqrt2PI * blurFactor;

            zBuff += deltaFactor;
            aBuff += deltaFactor*walkPx;
        }
    }
    return aBuff/zBuff;
}

__kernel void Denoise(__global float4 *output,const __global float4 *input, int width, int height, float sigma, float threshold, float kSigma)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	int position = x + y * width;
	output[position] = DenoiseFilter(input, x,y, width, height, sigma, threshold, kSigma);
}
