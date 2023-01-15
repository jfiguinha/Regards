#pragma once
class CRegardsBitmap;
class CRgbaquad;

class CCircle
{
public:
	CCircle()
	{
	};

	virtual ~CCircle()
	{
	};
	static wxImage GenerateCircle(const CRgbaquad& m_color, const int& iTaille, const float& fAlpha);
	static wxImage GradientTransparent(const CRgbaquad& m_color, const int& iTaille, const float& fAlpha);
	static wxImage Burst(const int& iTaille, const int& iColor, const int& iIntensity, const int& iColorIntensity);
	static wxImage HaloGradient(const int& iTaille, const int& iWidth, const float& fAlpha2);
	static wxImage Halo(const int& iColor, const int& iColorIntensity, const int& iTaille, const int& iWidth,
	                    const float& fAlpha2 = 0.9f, const int& iCentre = 1);
	static void CleanCircle();

private:
	static wxImage GetCircle(const int& rayon);
	static std::map<int, wxImage> listOfCircle;
};
