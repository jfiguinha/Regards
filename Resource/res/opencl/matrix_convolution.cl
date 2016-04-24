//---------------------------------------------------------------------
//Recherche le pixel
//---------------------------------------------------------------------
int4 GetColorSrc(int x, int y, const __global uchar4 *input, int width, int height)
{
	if(x < width && y < height && y >= 0 && x >= 0)	
	{
		int position = x + y * width;
		return convert_int4(input[position]);
	}
	return (int4)0;
}

//---------------------------------------------------------------------
//Limite les valeurs entre 0 et 255
//---------------------------------------------------------------------
uchar4 NormalizeValue(int4 sum)
{
	uchar4 value;
	value.x = (uchar)max(min(sum.x, 255), 0);
	value.y = (uchar)max(min(sum.y, 255), 0);
	value.z = (uchar)max(min(sum.z, 255), 0);  
	value.w = (uchar)max(min(sum.w, 255), 0); 
	return value;
}

//---------------------------------------------------------------------
//Application du filtre Emboss
//---------------------------------------------------------------------
__kernel void Emboss(__global uchar4 * output, const __global uchar4 *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	
	int4 sum = GetColorSrc(x + 1, y + 1, input, width, height) - GetColorSrc(x - 1, y - 1, input, width, height) + (int4)127;
 
	int position = x + y * width;
	output[position] = NormalizeValue(sum); 
}

//---------------------------------------------------------------------
//Application du filtre Edge
//---------------------------------------------------------------------

uint4 GradientX(const __global uchar4 *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	int4 sum = (int4)0;
	sum = GetColorSrc(x - 1, y - 1, input, width, height);
	sum += GetColorSrc(x - 1, y, input, width, height) * (int4)2;
	sum += GetColorSrc(x - 1, y + 1, input, width, height);
	sum -= GetColorSrc(x + 1, y - 1, input, width, height);
	sum -= GetColorSrc(x + 1, y, input, width, height) * (int4)2;
	sum -= GetColorSrc(x + 1, y + 1, input, width, height);
	return abs(sum);
}

uint4 GradientY(const __global uchar4 *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	int4 sum = (int4)0;
	sum = GetColorSrc(x - 1, y - 1, input, width, height);
	sum += GetColorSrc(x, y - 1, input, width, height) * (int4)2;
	sum += GetColorSrc(x + 1, y  - 1, input, width, height);
	sum -= GetColorSrc(x - 1, y + 1, input, width, height);
	sum -= GetColorSrc(x, y + 1, input, width, height) * (int4)2;
	sum -= GetColorSrc(x + 1, y + 1, input, width, height);
	return abs(sum);
}

__kernel void Edge(__global uchar4 * output, const __global uchar4 *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);

	int4 sum = (int4)0;
	int4 gx = convert_int4(GradientX(input, width, height));
	int4 gy = convert_int4(GradientY(input, width, height));
	sum = gx + gy;

	int position = x + y * width;
	output[position] = NormalizeValue(sum); 
}

//---------------------------------------------------------------------
//Application du filtre Sharpen
//	kernel = { -1, -1, -1, -1, 16,- 1, -1, -1, -1 };
//	factor = 8;
//---------------------------------------------------------------------
__kernel void Sharpen(__global uchar4 * output, const __global uchar4 *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	
	int4 sum = -GetColorSrc(x - 1, y - 1, input, width, height);
	sum -= GetColorSrc(x , y - 1, input, width, height);
	sum -= GetColorSrc(x + 1, y - 1, input, width, height);
	sum -= GetColorSrc(x - 1, y, input, width, height);
	sum += GetColorSrc(x , y, input, width, height) * (int4)16;
	sum -= GetColorSrc(x + 1, y, input, width, height);
	sum -= GetColorSrc(x - 1, y + 1, input, width, height);
	sum -= GetColorSrc(x , y + 1, input, width, height);
	sum -= GetColorSrc(x + 1, y + 1, input, width, height);
	
	sum = sum / (int4)8;

	int position = x + y * width;
	output[position] = NormalizeValue(sum);  
}

//---------------------------------------------------------------------
//Application du filtre SharpenStrong
//	kernel = { -1, -1, -1, -1, 9,- 1, -1, -1, -1 };
//	factor = 1;
//---------------------------------------------------------------------
__kernel void SharpenStrong(__global uchar4 * output, const __global uchar4 *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	
	int4 sum = -GetColorSrc(x - 1, y - 1, input, width, height);
	sum -= GetColorSrc(x , y - 1, input, width, height);
	sum -= GetColorSrc(x + 1, y - 1, input, width, height);
	sum -= GetColorSrc(x - 1, y, input, width, height);
	sum += GetColorSrc(x , y, input, width, height) * (int4)9;
	sum -= GetColorSrc(x + 1, y, input, width, height);
	sum -= GetColorSrc(x - 1, y + 1, input, width, height);
	sum -= GetColorSrc(x , y + 1, input, width, height);
	sum -= GetColorSrc(x + 1, y + 1, input, width, height);

	int position = x + y * width;
	output[position] = NormalizeValue(sum);  
}

//---------------------------------------------------------------------
//Application du filtre Blur
//	kernel = {  1, 1, 1, 1, 1, 1, 1, 1, 1 };
//	factor = 9;
//---------------------------------------------------------------------
__kernel void Blur(__global uchar4 * output, const __global uchar4 *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	
	int4 sum = GetColorSrc(x - 1, y - 1, input, width, height);
	sum += GetColorSrc(x , y - 1, input, width, height);
	sum += GetColorSrc(x + 1, y - 1, input, width, height);
	sum += GetColorSrc(x - 1, y, input, width, height);
	sum += GetColorSrc(x , y, input, width, height);
	sum += GetColorSrc(x + 1, y, input, width, height);
	sum += GetColorSrc(x - 1, y + 1, input, width, height);
	sum += GetColorSrc(x , y + 1, input, width, height);
	sum += GetColorSrc(x + 1, y + 1, input, width, height);
	sum = sum / (int4)9;
	int position = x + y * width;
	output[position] = NormalizeValue(sum);  
}

//---------------------------------------------------------------------
//Application du filtre Soften
//	kernel = {  1, 1, 1, 1, 8, 1, 1, 1, 1 };
//	factor = 16;
//---------------------------------------------------------------------
__kernel void Soften(__global uchar4 * output, const __global uchar4 *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	
	int4 sum = GetColorSrc(x - 1, y - 1, input, width, height);
	sum += GetColorSrc(x , y - 1, input, width, height);
	sum += GetColorSrc(x + 1, y - 1, input, width, height);
	sum += GetColorSrc(x - 1, y, input, width, height);
	sum += GetColorSrc(x , y, input, width, height) * (int4)8;
	sum += GetColorSrc(x + 1, y, input, width, height);
	sum += GetColorSrc(x - 1, y + 1, input, width, height);
	sum += GetColorSrc(x , y + 1, input, width, height);
	sum += GetColorSrc(x + 1, y + 1, input, width, height);
	sum = sum / (int4)16;
	int position = x + y * width;
	output[position] = NormalizeValue(sum);  
}

//---------------------------------------------------------------------
//Application du filtre Gaussian Blur
//	int kernel[9] = { 5, 15, 31, 49, 56, 49, 31, 15, 5 };
//	int size = 3;
//	int factor = 256;
//---------------------------------------------------------------------
__kernel void GaussianBlur(__global uchar4 * output, const __global uchar4 *input, int width, int height)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	
	int4 sum = (GetColorSrc(x - 1, y - 1, input, width, height) + GetColorSrc(x + 1, y + 1, input, width, height))* (int4)5;
	sum += (GetColorSrc(x , y - 1, input, width, height) + GetColorSrc(x , y + 1, input, width, height)) * (int4)15;
	sum += (GetColorSrc(x + 1, y - 1, input, width, height) + GetColorSrc(x - 1, y + 1, input, width, height)) * (int4)31;
	sum += (GetColorSrc(x - 1, y, input, width, height) + GetColorSrc(x + 1, y, input, width, height)) * (int4)49;
	sum += GetColorSrc(x , y, input, width, height) * (int4)56;

	sum = sum / (int4)256;
	int position = x + y * width;
	output[position] = NormalizeValue(sum);  
}