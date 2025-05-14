#pragma once

struct s_rgb
{
	unsigned int b : 8;
	unsigned int g : 8;
	unsigned int r : 8;
	unsigned int alpha : 8;
};

class CInterpolationBicubic
{
public:

    struct weightX
	{
	public:
		float tabF[4];
	};

	CInterpolationBicubic(const double & dWidth = 0.0f);
	~CInterpolationBicubic();
	void Execute(const cv::Mat& in, cv::Mat& Out);

protected:

	virtual inline double Filter(const double &x);
			
	virtual inline void Bicubic(const cv::Mat& in, uchar*& data, const int& x, const int& y, float* tabF1, float* tabF);

	void CalculWeight(const int32_t &width, const int32_t &height, const float &ratioY, const float &ratioX, const float &posTop, const float &posLeft);

	weightX * wX;
	weightX * wY;
	double m_dWidth;
};

