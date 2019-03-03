#pragma once
#include "Painting.h"
class CRegardsBitmap;
class CRgbaquad;

class CCircle : public CPainting
{
public:
	CCircle(){};
	virtual ~CCircle(){};
	wxImage GenerateCircle(const CRgbaquad &m_color, const int &iTaille, const float &fAlpha);
	wxImage GradientTransparent(const CRgbaquad &m_color, const int &iTaille, const float &fAlpha);
	wxImage Burst(const int &iTaille, const int &iColor, const int &iIntensity, const int &iColorIntensity);
	wxImage HaloGradient(const int &iTaille, const int &iWidth, const float &fAlpha2);
	wxImage Halo(const int &iColor, const int &iColorIntensity, const int &iTaille, const int &iWidth, const float &fAlpha2 = 0.9f, const int &iCentre = 1);

};