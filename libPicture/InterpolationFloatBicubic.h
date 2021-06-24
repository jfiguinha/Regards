#pragma once
class CRegardsFloatBitmap;

class CInterpolationFloat
{
public:
	struct weightX
	{
	public:
		float tabF[4];
	};

	CInterpolationFloat(const double& dWidth = 0.0f);
	~CInterpolationFloat();
	void Execute(CRegardsFloatBitmap* In, CRegardsFloatBitmap* & Out);

protected:
	virtual double Filter(const double& x) = 0;
	virtual void Bicubic(float* data, CRegardsFloatBitmap* In, const float& x, const float& y, float* tabF1,
	                     float* tabF);
	void CalculWeight(const int32_t& width, const int32_t& height, const float& ratioY, const float& ratioX,
	                  const float& posTop, const float& posLeft);

	weightX* wX;
	weightX* wY;
	double m_dWidth;
};


class CInterpolationFloatBicubic : public CInterpolationFloat
{
public:
	CInterpolationFloatBicubic(const double& dWidth = 2.0f) : CInterpolationFloat(dWidth)
	{
	};

	~CInterpolationFloatBicubic()
	{
	};
protected:
	double Filter(const double& x) override;
};
