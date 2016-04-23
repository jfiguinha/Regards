#pragma once
#include <stdint.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class CRgbaquad;
class CRegardsBitmap;

class CInterpolationBilinear
{
public:

    struct weightX
	{
	public:
		float tabF[4];
	};

	CInterpolationBilinear();
	~CInterpolationBilinear();
	void Execute(CRegardsBitmap * In, CRegardsBitmap * & Out);
	void Execute(CRegardsBitmap * In, CRegardsBitmap * & Out, const wxRect &rectToShow);

protected:

	virtual float Filter(const float &f);

private:

	void Bilinear(CRgbaquad * & data, CRegardsBitmap * In, const float &x, const float &y, float * tabF1, float * tabF);
	void CalculWeight(const int32_t &width, const int32_t &height, const float &ratioY, const float &ratioX, const float &posTop, const float &posLeft);
	weightX * wX = nullptr;
	weightX * wY = nullptr;
};

