#pragma once
#include "RGBAQuad.h"
class CCircle;

namespace Regards::FiltreEffet
{
	class CLensFlare
	{
	public:
		CLensFlare(void);
		~CLensFlare(void);

		void LensFlare(cv::Mat* pBitmap, int iPosX, int iPosY, int iPuissance,
			int iType, int iIntensity, int iColor, int iColorIntensity);

	private:
		void Halo(int x, int y, int iColor, int iTaille, int iWidth,
			float fAlpha2 = 0.9f, int iCentre = 1);
		void HaloGradient(int x, int y, int iTaille, int iWidth,
			float fAlpha2 = 0.9f);
		void Circle(int x, int y, const CRgbaquad& m_color, int iTaille, float fAlpha);
		void CircleGradient(int x, int y, const CRgbaquad& m_color, int iTaille,
			float fAlpha = 0.8f);
		void Burst(int x, int y, int iTaille, int iColor, int iIntensity = 25,
			int iColorIntensity = 30);
		int InsertwxImage(const wxImage& bitmap, int xPos, int yPos);

		// NOTE: iColorIntensity et pBitmap sont des membres mutables réinitialisés
		// à chaque appel de LensFlare(). Si une même instance de CLensFlare est
		// partagée entre plusieurs threads, ces membres doivent être protégés
		// (mutex) ou passés en paramètres plutôt qu'en état interne.
		int iColorIntensity;
		cv::Mat* pBitmap = nullptr;
	};
}