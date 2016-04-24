// Copyright (c) 2009-2011 Intel Corporation
// All rights reserved.
//
// WARRANTY DISCLAIMER
//
// THESE MATERIALS ARE PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL OR ITS
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THESE
// MATERIALS, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Intel Corporation is the author of the Materials, and requests that all
// problem reports or change requests be submitted to it directly

float4 GetColorSrc(int x, int y, const __global uchar4 *input, int width, int height)
{
	if(x < width && y < height && y >= 0 && x >= 0)	
	{
		int position = x + y * width;
		return convert_float4(input[position]);
	}
	return (float4)0.0f;
}

__kernel void SharpenMasking( __global uchar4 *output, const __global uchar4 *input, int width, int height, int sharpness)
{
    int x = get_global_id(0);
	int y = get_global_id(1);

	float4 data = GetColorSrc(x, y, input, width, height);

	float4 sum = GetColorSrc(x - 1, y - 1, input, width, height);
	sum += GetColorSrc(x, y - 1, input, width, height);
	sum += GetColorSrc(x + 1, y - 1, input, width, height);
	sum += GetColorSrc(x - 1, y, input, width, height);
	sum += data;
	sum += GetColorSrc(x + 1, y, input, width, height);
	sum += GetColorSrc(x - 1, y + 1, input, width, height);
	sum += GetColorSrc(x, y + 1, input, width, height);
	sum += GetColorSrc(x + 1, y + 1, input, width, height);
	
	float4 blur = sum / (float4)9.0;
	data = data + (data - blur) * convert_float4(sharpness);	

	int position = x + y * width;
	output[position] = convert_uchar4(data);	
}