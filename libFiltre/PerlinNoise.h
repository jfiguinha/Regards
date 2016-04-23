#pragma once
#include "RegardsBitmap.h"
#include "RGBAQuad.h"

namespace Regards
{
	namespace FiltreEffet
	{
		class CPerlinNoise
		{

		public:

			CPerlinNoise();

			~CPerlinNoise();

			void Clouds(CRegardsBitmap * pBitmap,const CRgbaquad & color1, const CRgbaquad & color2,const float &freq,const float &pers,const int &octaves);

		private:

			inline const float Noise2d(const int &x, const int &y);

			inline const double Interpolate(const double &a,const double &b,const double &x);

			const float GetValue(float x, float y);

			float FinalNoise(float x, float y);

			float Smooth_Noise(int x, int y);

			float m_fPrivx0y0,m_fPrivx1y0,m_fPrivx0y1,m_fPrivx1y1;

			inline float CalculPosValue(const int Xint,const int Yint);
		};
	}
}
