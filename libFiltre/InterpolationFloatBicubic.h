#pragma once
class CRegardsFloatBitmap;

class CInterpolationFloatBicubic
{
public:

    struct weightX
	{
	public:
		float tabF[4];
	};

	CInterpolationFloatBicubic(const double & dWidth = 0.0f);
	~CInterpolationFloatBicubic();
	void Execute(CRegardsFloatBitmap * In, CRegardsFloatBitmap * & Out);

protected:

	virtual inline double Filter(const double &x);
	virtual inline void Bicubic(float * data, CRegardsFloatBitmap * In, const float &x, const float &y, float * tabF1, float * tabF);
	void CalculWeight(const int32_t &width, const int32_t &height, const float &ratioY, const float &ratioX, const float &posTop, const float &posLeft);

	weightX * wX;
	weightX * wY;
	double m_dWidth;
};

