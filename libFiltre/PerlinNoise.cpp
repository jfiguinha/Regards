#include "PerlinNoise.h"
#include "RegardsBitmap.h"

using namespace Regards::FiltreEffet;

CPerlinNoise::CPerlinNoise()
{
	int Xint = 0;
	int Yint = 0;
	//Calcul Préemptif
	m_fPrivx0y0 = Noise2d(Xint, Yint);
	m_fPrivx1y0 = Noise2d(Xint+1, Yint);
	m_fPrivx0y1 = Noise2d(Xint, Yint+1);
	m_fPrivx1y1 = Noise2d(Xint+1, Yint+1);
}

CPerlinNoise::~CPerlinNoise()
{
}

inline const float CPerlinNoise::Noise2d(const int &x, const int &y)
{
	int n = ((x + (y << 6)) << 13) ^ (x + (y << 6));
	return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

inline const double CPerlinNoise::Interpolate(const double &a,const double &b,const double &x)
{
	return (a + (x * (b - a)));
}

inline float CPerlinNoise::CalculPosValue(const int Xint,const int Yint)
{
	int m = Xint + ((Yint) << 6);
	int n = (m << 13) ^ (m);
	return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f); 

}

const float CPerlinNoise::GetValue(float x, float y) 
{
		int Xint = int(x);
		int Yint = int(y);

		float Xfrac = x - Xint;
		float Yfrac = y - Yint;

		float x0y0,x1y0,x0y1,x1y1;

		if(Xint != 0 || Yint != 0)
		{		
			#pragma omp parallel
			{
				x0y0 = CalculPosValue(Xint,Yint);
				x1y0 = CalculPosValue(Xint+1,Yint);
				x0y1 = CalculPosValue(Xint,Yint+1);
				x1y1 = CalculPosValue(Xint+1,Yint+1);
			}
		}
		else
		{
			x0y0 = m_fPrivx0y0;  //find the noise values of the four corners
			x1y0 = m_fPrivx1y0;
			x0y1 = m_fPrivx0y1;
			x1y1 = m_fPrivx1y1;
		}

		//interpolate between those values according to the x and y fractions
		float v1 = (x0y0 + (Xfrac * (x1y0 - x0y0)));//Interpolate(x0y0, x1y0, Xfrac); //interpolate in x direction (y)
		float v2 = (x0y1 + (Xfrac * (x1y1 - x0y1)));//Interpolate(x0y1, x1y1, Xfrac); //interpolate in x direction (y+1)
		float fin = (v1 + (Yfrac * (v2 - v1)));//Interpolate(v1, v2, Yfrac);  //interpolate in y direction

		return fin;
}

float CPerlinNoise::FinalNoise(float x, float y) {
		//properties of the perlin noise
		float P = 0.5f;
		int octaves = 4;

		//properties of one octave (changing each loop)
		float total = 0.0f;
		int freq = 1;
		float pers = 1;

		for(int i=0; i<octaves; i++) 
		{
			total += GetValue(x*freq,y*freq) * pers;
			freq *= 2;
			pers *= P;
		}
		return total;
}

float CPerlinNoise::Smooth_Noise(int x, int y) 
{
		float corners = ( Noise2d(x-1, y-1) + Noise2d(x+1, y-1) + Noise2d(x-1, y+1) + Noise2d(x+1, y+1) ) / 16.0f;
		float sides = ( Noise2d(x-1, y) +Noise2d(x+1, y) + Noise2d(x, y-1) + Noise2d(x, y+1) ) / 8.0f;
		float center = Noise2d(x, y) / 4.0f;
		return corners + sides + center;
}



void CPerlinNoise::Clouds(CRegardsBitmap * pBitmap,const CRgbaquad & color1, const CRgbaquad & color2,const float &freq,const float &pers,const int &octaves)
{

	if (pBitmap->GetPtBitmap() != nullptr)
	{
		uint8_t * pBitsSrc = nullptr;
		int bmWidth, bmHeight;
		//unsigned long m_lSize;
		//pBitmap->GetBitmap(&pBitsSrc, bmWidth, bmHeight, m_lSize);

	
		bmWidth = pBitmap->GetBitmapWidth();
		bmHeight = pBitmap->GetBitmapHeight();
		pBitsSrc = new uint8_t[bmWidth*bmHeight * 4];

		//long k = 0;
		//long m;
	
		//int nbtour = octaves >> 2;

		//int i, j,k;

		float * tabFrequence = new float[octaves];
		float * tabAmplitude = new float[octaves];
	
		tabFrequence[0] = freq;
		tabAmplitude[0] = 1;

		for(int k=1; k<octaves; k++)
		{
			tabFrequence[k] = tabFrequence[k - 1] * 2;
			tabAmplitude[k] = tabAmplitude[k - 1] * pers;
		}

#pragma omp parallel for
		for(int i=0; i< bmHeight; i++) 
		{
	#pragma omp parallel for
			for (int  j = 0; j< bmWidth; j++)
			{

				int l = ((i * bmWidth) << 2) + (j << 2);

				float total = 0.0f;
				float total1 = 0.0f;
				float total2 = 0.0f;
				float total3 = 0.0f;
				float total4 = 0.0f;		
				
				for(int k=0; k< octaves; k+=4) 
				{
					total1 = GetValue(j*(*(tabFrequence + k)), i*(*(tabFrequence + k))) * (*(tabAmplitude + k));					
					total2 = GetValue(j*(*(tabFrequence + k + 1)), i*(*(tabFrequence + k + 1))) * (*(tabAmplitude + k + 1));
					total3 = GetValue(j*(*(tabFrequence + k + 2)), i*(*(tabFrequence + k + 2))) * (*(tabAmplitude + k + 2));
					total4 = GetValue(j*(*(tabFrequence + k + 3)), i*(*(tabFrequence + k + 3))) * (*(tabAmplitude + k + 3));
					total += total1 + total2 + total3 + total4;
				}

				total = total*0.5f + 0.5f;

				if(total<0) 
					total = 0.0f;
				if(total>1) 
					total = 1.0f;

				*(pBitsSrc + l) = color1.GetFBlue() * total + color2.GetFBlue()*(1 - total);
				*(pBitsSrc + l + 1) = color1.GetFGreen() * total + color2.GetFGreen()*(1 - total);
				*(pBitsSrc + l + 2) = color1.GetFRed() * total + color2.GetFRed()*(1 - total);
				*(pBitsSrc + l + 3) = 0;

			}
		}

		pBitmap->SetBitmap(pBitsSrc, bmWidth, bmHeight);

		delete[] pBitsSrc;
		delete[] tabFrequence;
		delete[] tabAmplitude;
	}
}