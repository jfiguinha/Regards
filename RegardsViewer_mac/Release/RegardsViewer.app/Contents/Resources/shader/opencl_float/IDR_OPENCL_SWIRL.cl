float4 GetColorSrc(int x, int y, const __global float4 *input, int widthIn, int heightIn)
{
	if(x < widthIn && y < heightIn && y >= 0 && x >= 0)	
	{
		int position = x + y * widthIn;
		return input[position];
	}
	return 0.0f;
}

float EuclideanDist(float tcX, float tcY, float centerX, float centerY)
{
	float diffX = tcX - centerX;
	float diffY = tcY - centerY;
	return sqrt(diffX * diffX + diffY * diffY);
}

float DotProduct(float tcX, float tcY, float qX, float qY)
{
	return tcX * qX + tcY * qY;
}

float4 PostFX(const __global float4 *input, int x, int y, float radius, float angleDegree, int widthIn, int heightIn)
{
  float xOut = x;
  float yOut = y;
  
   //Calcul du centre
  float centerX = (float)widthIn / 2.0f;
  float centerY = (float)heightIn / 2.0f;
  
  float tcX = (float)x - centerX;
  float tcY = (float)y - centerY;
  
  float angle = angleDegree * 0.0174532925;
   
  float dist = EuclideanDist(x, y, centerX, centerY);
 
  if (dist < radius) 
  {
    float percent = (radius - dist) / radius;
    float theta = percent * percent * angle * 8.0;
    float s = sin(theta);
    float c = cos(theta);
	xOut = DotProduct(tcX, tcY, c, -s);
    yOut = DotProduct(tcX, tcY, s, c);
	
	tcX = xOut;
	tcY = yOut;
  }

  tcX += centerX;
  tcY += centerY;
  
  return GetColorSrc((int)tcX, (int)tcY, input, widthIn, heightIn);
}

__kernel void Swirl(__global float4 *output,const __global float4 *input, int width, int height, float radius, float angleDegree)
{
    int x = get_global_id(0);
	int y = get_global_id(1);
	int position = x + y * width;
	output[position] = PostFX(input, x,y, radius, angleDegree, width, height);
}
