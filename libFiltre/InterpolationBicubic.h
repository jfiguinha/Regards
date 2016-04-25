#pragma once
#include <stdint.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class CRegardsBitmap;
class CRgbaquad;

class CInterpolationBicubic
{
public:

    struct weightX
	{
	public:
		float tabF[4];
	};

	CInterpolationBicubic();
	~CInterpolationBicubic();
	void Execute(CRegardsBitmap * In, CRegardsBitmap * & Out);
	void Execute(CRegardsBitmap * In, CRegardsBitmap * & Out, const wxRect &rectToShow);
	void Execute(CRegardsBitmap * In, CRegardsBitmap * & Out, const int &flipH, const int &flipV, const int &angle);

protected:

	virtual float Filter(const float &f);
			
	virtual void Bicubic(CRgbaquad & data, CRegardsBitmap * In, const float &x, const float &y, float * tabF1, float * tabF);

	void CalculWeight(const int32_t &width, const int32_t &height, const float &ratioY, const float &ratioX, const float &posTop, const float &posLeft);

	weightX * wX = nullptr;
	weightX * wY = nullptr;
};

