#pragma once

/////////////////////////////////////////////////////////////////////////////
//Filtre supportée :
// Box, Bilinear, Gaussian, Hamming, Cubic, Blackman
/////////////////////////////////////////////////////////////////////////////


#define FILTER_PI  double (3.1415926535897932384626433832795)
#define FILTER_2PI double (2.0 * 3.1415926535897932384626433832795)
#define FILTER_4PI double (4.0 * 3.1415926535897932384626433832795)

#include "Interpolation.h"
#include "InterpolationBicubic.h"

class CBoxFilter : public CInterpolationBicubic
{
public:
	CBoxFilter(double dWidth = 0.5) : CInterpolationBicubic(dWidth)
	{
	}

	~CBoxFilter() override
	{
	}

	double Filter(const double& dVal) override { return (fabs(dVal) <= m_dWidth ? 1.0 : 0.0); }
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class CBilinearFilter : public CInterpolationBicubic
{
public:
	CBilinearFilter(double dWidth = 1.0) : CInterpolationBicubic(dWidth)
	{
	}

	~CBilinearFilter() override
	{
	}

	double Filter(const double& dVal) override
	{
		double m_dVal = fabs(dVal);
		return (m_dVal < m_dWidth ? m_dWidth - m_dVal : 0.0);
	}
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class CGaussianFilter : public CInterpolationBicubic
{
public:
	CGaussianFilter(double dWidth = 1.25) : CInterpolationBicubic(dWidth)
	{
	}

	~CGaussianFilter() override
	{
	}

	double Filter(const double& dVal) override
	{
		if (fabs(dVal) > m_dWidth)
		{
			return 0.0;
		}
		return exp(-dVal * dVal / 2.0) / sqrt(FILTER_2PI);
	}
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class CHammingFilter : public CInterpolationBicubic
{
public:
	CHammingFilter(double dWidth = 1.0) : CInterpolationBicubic(dWidth)
	{
	}

	~CHammingFilter() override
	{
	}

	double Filter(const double& dVal) override
	{
		if (fabs(dVal) > m_dWidth)
		{
			return 0.0;
		}
		double dWindow = 0.54 + 0.46 * cos(FILTER_2PI * dVal);
		double dSinc = (dVal == 0) ? 1.0 : sin(FILTER_PI * dVal) / (FILTER_PI * dVal);
		return dWindow * dSinc;
	}
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class CCubicFilter : public CInterpolationBicubic
{
public:
	CCubicFilter(double dWidth = 2.0) : CInterpolationBicubic(dWidth)
	{
	}

	~CCubicFilter() override
	{
	}

	double Filter(const double& x) override
	{
		if (fabs(x) > m_dWidth)
		{
			return 0.0;
		}

		if (x < -2.0)
			return (0.0);
		if (x < -1.0)
			return ((2.0 + x) * (2.0 + x) * (2.0 + x) / 6.0);
		if (x < 0.0)
			return ((4.0 + x * x * (-6.0 - 3.0 * x)) / 6.0);
		if (x < 1.0)
			return ((4.0 + x * x * (-6.0 + 3.0 * x)) / 6.0);
		if (x < 2.0)
			return ((2.0 - x) * (2.0 - x) * (2.0 - x) / 6.0);
		return (0.0);
	}
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class CBlackmanFilter : public CInterpolationBicubic
{
public:
	CBlackmanFilter(double dWidth = 1.0) : CInterpolationBicubic(dWidth)
	{
	}

	~CBlackmanFilter() override
	{
	}

	double Filter(const double& dVal) override
	{
		if (fabs(dVal) > m_dWidth)
		{
			return 0.0;
		}
		double dN = 2.0 * m_dWidth + 1.0;
		return 0.42 + 0.5 * cos(FILTER_2PI * dVal / (dN - 1.0)) +
			0.08 * cos(FILTER_4PI * dVal / (dN - 1.0));
	}
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class CQuadraticFilter : public CInterpolationBicubic
{
public:
	CQuadraticFilter(double dWidth = 1.5) : CInterpolationBicubic(dWidth)
	{
	}

	~CQuadraticFilter() override
	{
	}

	double Filter(const double& x) override
	{
		if (fabs(x) > m_dWidth)
		{
			return 0.0;
		}

		if (x < -1.5)
			return (0.0);
		if (x < -0.5)
			return (0.5 * (x + 1.5) * (x + 1.5));
		if (x < 0.5)
			return (0.75 - x * x);
		if (x < 1.5)
			return (0.5 * (x - 1.5) * (x - 1.5));
		return (0.0);
	}
};


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class CMitchellFilter : public CInterpolationBicubic
{
public:
	CMitchellFilter(double dWidth = 2.0) : CInterpolationBicubic(dWidth)
	{
	}

	~CMitchellFilter() override
	{
	}

	double Filter(const double& x) override
	{
		if (fabs(x) > m_dWidth)
		{
			return 0.0;
		}

#define B   (1.0/3.0)
#define C   (1.0/3.0)
#define P0  ((  6.0- 2.0*B       )/6.0)
#define P2  ((-18.0+12.0*B+ 6.0*C)/6.0)
#define P3  (( 12.0- 9.0*B- 6.0*C)/6.0)
#define Q0  ((       8.0*B+24.0*C)/6.0)
#define Q1  ((     -12.0*B-48.0*C)/6.0)
#define Q2  ((       6.0*B+30.0*C)/6.0)
#define Q3  ((     - 1.0*B- 6.0*C)/6.0)

		if (x < -2.0)
			return (0.0);
		if (x < -1.0)
			return (Q0 - x * (Q1 - x * (Q2 - x * Q3)));
		if (x < 0.0)
			return (P0 + x * x * (P2 - x * P3));
		if (x < 1.0)
			return (P0 + x * x * (P2 + x * P3));
		if (x < 2.0)
			return (Q0 + x * (Q1 + x * (Q2 + x * Q3)));
		return (0.0);
	}
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class CTriangleFilter : public CInterpolationBicubic
{
public:
	CTriangleFilter(double dWidth = 1.0) : CInterpolationBicubic(dWidth)
	{
	}

	~CTriangleFilter() override
	{
	}

	double Filter(const double& x) override
	{
		if (fabs(x) > m_dWidth)
		{
			return 0.0;
		}

		if (x < -1.0)
			return (0.0);
		if (x < 0.0)
			return (1.0 + x);
		if (x < 1.0)
			return (1.0 - x);
		return (0.0);
	}
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class CSincFilter : public CInterpolationBicubic
{
public:
	CSincFilter(double dWidth = 1.5) : CInterpolationBicubic(dWidth)
	{
	}

	~CSincFilter() override
	{
	}

	double Filter(const double& x) override
	{
		if (fabs(x) > m_dWidth)
		{
			return 0.0;
		}

		if (x == 0.0)
			return (1.0);

		return (sin(FILTER_PI * x) / (FILTER_PI * x));
	}
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

const double Pone[] = {
	0.3511751914303552822533318e+3,
	0.7210391804904475039280863e+3,
	0.4259873011654442389886993e+3,
	0.831898957673850827325226e+2,
	0.45681716295512267064405e+1,
	0.3532840052740123642735e-1
};

const double Qone[] =
{
	0.74917374171809127714519505e+4,
	0.154141773392650970499848051e+5,
	0.91522317015169922705904727e+4,
	0.18111867005523513506724158e+4,
	0.1038187585462133728776636e+3,
	0.1e+1
};


class CBesselFilter : public CInterpolationBicubic
{
public:
	CBesselFilter(double dWidth = 1.5) : CInterpolationBicubic(dWidth)
	{
	}

	~CBesselFilter() override
	{
	}

	double Filter(const double& x) override
	{
		if (fabs(x) > m_dWidth)
		{
			return 0.0;
		}

		if (x == 0.0)
			return (FILTER_PI / 4.0);
		return (BesselOrderOne(FILTER_PI * x) / (2.0 * x));
	}

private:
	double BesselOrderOne(double x)
	{
		double p, q;

		if (x == 0.0)
			return (0.0);

		p = x;
		if (x < 0.0)
			x = (-x);

		if (x < 8.0)
			return (p * J1(x));

		q = sqrt(2.0 / (FILTER_PI * x)) * (P1(x) * (1.0 / sqrt(2.0) * (sin(x) - cos(x))) - 8.0 / x * QUU1(x) * (-1.0 /
			sqrt(2.0) * (sin(x) + cos(x))));
		if (p < 0.0)
			q = (-q);
		return (q);
	}

	double P1(double x)
	{
		double p, q;
		long i;

		p = Pone[5];
		q = Qone[5];
		for (i = 4; i >= 0; i--)
		{
			p = p * (8.0 / x) * (8.0 / x) + Pone[i];
			q = q * (8.0 / x) * (8.0 / x) + Qone[i];
		}
		return (p / q);
	};

	double QUU1(double x)
	{
		double p, q;

		long i;

		p = Pone[5];
		q = Qone[5];
		for (i = 4; i >= 0; i--)
		{
			p = p * (8.0 / x) * (8.0 / x) + Pone[i];
			q = q * (8.0 / x) * (8.0 / x) + Qone[i];
		}
		return (p / q);
	};

	double J1(double x)
	{
		double p, q;
		long i;
		const double
			Pone[] =
			{
				0.581199354001606143928050809e+21,
				-0.6672106568924916298020941484e+20,
				0.2316433580634002297931815435e+19,
				-0.3588817569910106050743641413e+17,
				0.2908795263834775409737601689e+15,
				-0.1322983480332126453125473247e+13,
				0.3413234182301700539091292655e+10,
				-0.4695753530642995859767162166e+7,
				0.270112271089232341485679099e+4
			},
			Qone[] =
			{
				0.11623987080032122878585294e+22,
				0.1185770712190320999837113348e+20,
				0.6092061398917521746105196863e+17,
				0.2081661221307607351240184229e+15,
				0.5243710262167649715406728642e+12,
				0.1013863514358673989967045588e+10,
				0.1501793594998585505921097578e+7,
				0.1606931573481487801970916749e+4,
				0.1e+1
			};

		p = Pone[8];
		q = Qone[8];
		for (i = 7; i >= 0; i--)
		{
			p = p * x * x + Pone[i];
			q = q * x * x + Qone[i];
		}
		return (p / q);
	}
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class CBlackmanBesselFilter : public CInterpolationBicubic
{
public:
	CBlackmanBesselFilter(double dWidth = 3.2383) : CInterpolationBicubic(dWidth)
	{
	}

	~CBlackmanBesselFilter() override
	{
	}

	double Filter(const double& x) override
	{
		CBesselFilter m_BesselFilter(m_dWidth);
		CBlackmanFilter m_BlackmanFilter(m_dWidth);

		if (fabs(x) > m_dWidth)
		{
			return 0.0;
		}

		return (m_BlackmanFilter.Filter(x / m_dWidth) * m_BesselFilter.Filter(x));
	}

private:
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class CLanczosFilter : public CInterpolationBicubic
{
public:
	CLanczosFilter(double dWidth = 3.0) : CInterpolationBicubic(dWidth)
	{
	}

	~CLanczosFilter() override
	{
	}

	double Filter(const double& x) override
	{
		CSincFilter m_SincFilter(m_dWidth);

		if (fabs(x) > m_dWidth)
		{
			return 0.0;
		}

		if (x < -3.0)
			return (0.0);
		if (x < 0.0)
			return (m_SincFilter.Filter(-x) * m_SincFilter.Filter(-x / 3.0));
		if (x < 3.0)
			return (m_SincFilter.Filter(x) * m_SincFilter.Filter(x / 3.0));
		return (0.0);
	}
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class CBlackmanSincFilter : public CInterpolationBicubic
{
public:
	CBlackmanSincFilter(double dWidth = 4.0) : CInterpolationBicubic(dWidth)
	{
	}

	~CBlackmanSincFilter() override
	{
	}

	double Filter(const double& x) override
	{
		CSincFilter m_SincFilter(m_dWidth);
		CBlackmanFilter m_BlackmanFilter(m_dWidth);

		if (fabs(x) > m_dWidth)
		{
			return 0.0;
		}

		return (m_BlackmanFilter.Filter(x / m_dWidth) * m_SincFilter.Filter(x));
	}
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class CCatromFilter : public CInterpolationBicubic
{
public:
	CCatromFilter(double dWidth = 2.0) : CInterpolationBicubic(dWidth)
	{
	}

	~CCatromFilter() override
	{
	}

	double Filter(const double& x) override
	{
		if (fabs(x) > m_dWidth)
		{
			return 0.0;
		}

		if (x < -2.0)
			return (0.0);
		if (x < -1.0)
			return (0.5 * (4.0 + x * (8.0 + x * (5.0 + x))));
		if (x < 0.0)
			return (0.5 * (2.0 + x * x * (-5.0 - 3.0 * x)));
		if (x < 1.0)
			return (0.5 * (2.0 + x * x * (-5.0 + 3.0 * x)));
		if (x < 2.0)
			return (0.5 * (4.0 + x * (-8.0 + x * (5.0 - x))));
		return (0.0);
	}
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class CHanningFilter : public CInterpolationBicubic
{
public:
	CHanningFilter(double dWidth = 1.0) : CInterpolationBicubic(dWidth)
	{
	}

	~CHanningFilter() override
	{
	}

	double Filter(const double& x) override
	{
		if (fabs(x) > m_dWidth)
		{
			return 0.0;
		}

		return (0.54 + 0.46 * cos(FILTER_PI * x));
	}
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class CHermiteFilter : public CInterpolationBicubic
{
public:
	CHermiteFilter(double dWidth = 1.5) : CInterpolationBicubic(dWidth)
	{
	}

	~CHermiteFilter() override
	{
	}

	double Filter(const double& x) override
	{
		if (fabs(x) > m_dWidth)
		{
			return 0.0;
		}

		if (x < -1.0)
			return (0.0);
		if (x < 0.0)
			return ((2.0 * (-x) - 3.0) * (-x) * (-x) + 1.0);
		if (x < 1.0)
			return ((2.0 * x - 3.0) * x * x + 1.0);
		return (0.0);
	}
};

static void ApplyInterpolationFilters(CRegardsBitmap* bitmap, CRegardsBitmap* & bitmapOut, const wxRect& rc,
                                      const int& flipH, const int& flipV, const int& angle,
                                      const int& filterInterpolation)
{
	switch (filterInterpolation)
	{
	case 0:
		{
			CCubicFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, rc, flipH, flipV, angle);
		}
		break;
	case 1:
		{
			CBoxFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, rc, flipH, flipV, angle);
		}
		break;
	case 2:
		{
			CHermiteFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, rc, flipH, flipV, angle);
		}
		break;
	case 3:
		{
			CHanningFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, rc, flipH, flipV, angle);
		}
		break;
	case 4:
		{
			CCatromFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, rc, flipH, flipV, angle);
		}
		break;
	case 5:
		{
			CMitchellFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, rc, flipH, flipV, angle);
		}
	case 6:
		{
			CTriangleFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, rc, flipH, flipV, angle);
		}
		break;
	case 7:
		{
			CQuadraticFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, rc, flipH, flipV, angle);
		}
		break;
	case 8:
		{
			CBlackmanFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, rc, flipH, flipV, angle);
		}
		break;
	case 9:
		{
			CHammingFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, rc, flipH, flipV, angle);
		}
		break;
	case 10:
		{
			CGaussianFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, rc, flipH, flipV, angle);
		}
		break;
	case 11:
		{
			CBilinearFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, rc, flipH, flipV, angle);
		}
		break;
	default:
		{
			CInterpolation interpolation;
			interpolation.Execute(bitmap, bitmapOut, rc, flipH, flipV, angle);
		}
		break;
	}
}

static void ApplyInterpolationFilters(CRegardsBitmap* bitmap, CRegardsBitmap* & bitmapOut, const int& flipH,
                                      const int& flipV, const int& angle, const int& filterInterpolation)
{
	switch (filterInterpolation)
	{
	case 0:
		{
			CCubicFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, flipH, flipV, angle);
		}
		break;
	case 1:
		{
			CBoxFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, flipH, flipV, angle);
		}
		break;
	case 2:
		{
			CHermiteFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, flipH, flipV, angle);
		}
		break;
	case 3:
		{
			CHanningFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, flipH, flipV, angle);
		}
		break;
	case 4:
		{
			CCatromFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, flipH, flipV, angle);
		}
		break;
	case 5:
		{
			CMitchellFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, flipH, flipV, angle);
		}
	case 6:
		{
			CTriangleFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, flipH, flipV, angle);
		}
		break;
	case 7:
		{
			CQuadraticFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, flipH, flipV, angle);
		}
		break;
	case 8:
		{
			CBlackmanFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, flipH, flipV, angle);
		}
		break;
	case 9:
		{
			CHammingFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, flipH, flipV, angle);
		}
		break;
	case 10:
		{
			CGaussianFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, flipH, flipV, angle);
		}
		break;
	case 11:
		{
			CBilinearFilter cubicFilter;
			cubicFilter.Execute(bitmap, bitmapOut, flipH, flipV, angle);
		}
		break;
	default:
		{
			CInterpolation interpolation;
			interpolation.Execute(bitmap, bitmapOut, flipH, flipV, angle);
		}
		break;
	}
}
