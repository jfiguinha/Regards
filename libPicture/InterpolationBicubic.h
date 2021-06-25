#pragma once

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

	CInterpolationBicubic(const double& dWidth = 0.0f);
	virtual ~CInterpolationBicubic();
	void Execute(wxImage* In, CRegardsBitmap* & Out);
	void Execute(CRegardsBitmap* In, CRegardsBitmap* & Out);
	void Execute(CRegardsBitmap* In, CRegardsBitmap* & Out, const int& flipH, const int& flipV, const int& angle);
	void Execute(CRegardsBitmap* In, CRegardsBitmap* & Out, const wxRect& rectToShow, const int& flipH,
	             const int& flipV, const int& angle);

	virtual void Bicubic(CRgbaquad& data, CRegardsBitmap* In, const float& x, const float& y, float* tabF1,
	                     float* tabF);
	virtual void Bicubic(CRgbaquad& data, wxImage* In, const float& x, const float& y, float* tabF1, float* tabF);


	weightX* wX;
	weightX* wY;

protected:
	virtual inline double Filter(const double& x);

	void CalculWeight(const int32_t& width, const int32_t& height, const float& ratioY, const float& ratioX,
	                  const float& posTop, const float& posLeft);

	double m_dWidth;
};
