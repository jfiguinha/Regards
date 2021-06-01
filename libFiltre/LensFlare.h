#pragma once
#include "RGBAQuad.h"
class CCircle;
class CRegardsBitmap;
namespace Regards
{
	namespace FiltreEffet
	{
		class CLensFlare
		{
		public:
			CLensFlare(void);
			~CLensFlare(void);

			void LensFlare(CRegardsBitmap * pBitmap, const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity);

		private:

			void Halo(const int &x, const int &y,const int &iColor, const int &iTaille, const int &iWidth, const float &fAlpha2 = 0.9f, const int &iCentre = 1);
			void HaloGradient(const int &x, const int &y, const int &iTaille, const int &iWidth, const float &fAlpha2 = 0.9f);
			void Circle(const int &x, const int &y,const CRgbaquad &m_color, const int &iTaille, const float &fAlpha);
			void CircleGradient(const int &x, const int &y,const CRgbaquad &m_color, const int &iTaille,const float &fAlpha = 0.8f);
			void Burst(const int &x, const int &y,const int &iTaille,const int &iColor, const int &iIntensity = 25, const int &iColorIntensity = 30);

			int iColorIntensity;
			CRegardsBitmap * pBitmap;


		};
	}
}

