#pragma once
#include <stdint.h>
#include "RegardsBitmap.h"

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

			long CalculPos(long x, long y, long width);
			void AlphaBlending(uint8_t * pBitsSrc,const int &iPosSrc, uint8_t * pBitSrc2, const int &iPosSrc2, uint8_t * pBitDest, const int &iPosDest, const int &alpha);
			void AdditiveBlending(uint8_t * pBitsSrc,const int &iPosSrc, uint8_t * pBitSrc2, const int &iPosSrc2, uint8_t * pBitDest, const int &iPosDest, const int &alpha);

			void Halo(const int &x, const int &y,const int &iColor, const int &iTaille, const int &iWidth, const float &fAlpha2 = 0.9f, const int &iCentre = 1);
			void HaloGradient(const int &x, const int &y, const int &iTaille, const int &iWidth, const float &fAlpha2 = 0.9f);
			void DefineZoneBilinear(const int &x, const int &y, const int &iTaille);
			void Circle(const int &x, const int &y,const CRgbaquad &m_color, const int &iTaille, const float &fAlpha);
			void CircleGradient(const int &x, const int &y,const CRgbaquad &m_color, const int &iTaille,const float &fAlpha = 0.8f);
			void RadialBlur(const int &xFrom, const int &yFrom, const int &iRayon,const int &iSize = 2);
			void RadialBlurCercleVide(const int &xFrom, const int &yFrom, const int &iRayon,const int &iCercleTaille);
			void Burst(const int &x, const int &y,const int &iTaille,const int &iColor, const int &iIntensity = 25, const int &iColorIntensity = 30);
			void SetAlphaColorValue(const int &xFrom, const int &yFrom, const int &x, const int &y,const float &fAlpha,const CRgbaquad &m_color);
			void MidpointCircle(const int &xFrom, const int &yFrom,const int &Rayon, const CRgbaquad &m_color, const float &fAlpha, const bool &bAntialiasing = false, const bool &bFullCircle = false);
			void WriteCirclePoint(const int &xFrom, const int &yFrom, const int &i, const int &x, const float &fAlpha, const CRgbaquad &m_color);
			int BilinearFiltering(const int &iSrcX,const int &iSrcY, CRgbaquad &m_colorValue);
			void MidpointLine(const int &xFrom, const int &yFrom,const int &xTo, const int &yTo, const CRgbaquad &m_color, const float &fAlpha, const bool &m_bAntialiasing);


			typedef struct KERNELSTORAGE
			{
				int m_dData[256];
			}DATAKERNELSTORAGE, * LPDATAKERNELSTORAGE;

			unsigned long m_lSize;
			int iTypeTransfert;
			int iBilinearFiltering;
			int iWidthMax, iHeightMax;
			int iColorIntensity;
			CRegardsBitmap * pBitmap;

		};
	}
}

