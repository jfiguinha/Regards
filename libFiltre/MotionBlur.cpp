#include <header.h>
#include "MotionBlur.h"
#include <math.h>
#include <stdlib.h>
#include <RegardsBitmap.h>
const double pi = 3.14159265358979323846264338327950288419716939937510;
#define MagickSQ2PI 2.50662827463100024161235523934010416269302368164062
#define DegreesToRadians(x) (pi*(x)/180.0)
using namespace Regards::FiltreEffet;

CMotionBlur::CMotionBlur(void)
{
}


CMotionBlur::~CMotionBlur(void)
{
}

vector<wxPoint> CMotionBlur::GetOffsetKernel(int width, const double &angle)
{
	vector<wxPoint> offsets;
	//blur_image->storage_class=DirectClass;
	int x = static_cast<long>(width * sin(DegreesToRadians(angle)));
	int y = static_cast<long>(width * cos(DegreesToRadians(angle)));

	for (auto i = 0; i < width; i++)
	{
		wxPoint pt;
		pt.x = (int)((double)(i*x) / sqrt((double)x*x + y*y));
		pt.y = (int)((double)(i*y) / sqrt((double)x*x + y*y));
		offsets.push_back(pt);
	}

	return offsets;
}

int CMotionBlur::GenerateMotionBlurKernel(int width, const double sigma, vector<double> & kernel)
{
	const int KernelRank = 3;
	double alpha, normalize;
	int bias;
    long i;

	/*
	Generate a 1-D convolution matrix.  Calculate the kernel at higher
	resolution than needed and average the results as a form of numerical
	integration to get the best accuracy.
	*/
	if (width <= 0)
		width=3;

	for (i = 0; i < width; i++)
		kernel.push_back(0);

	bias=KernelRank*width;

	double m_dValue = (2.0*KernelRank*KernelRank*sigma*sigma);
	double m_dValue2 = MagickSQ2PI*sigma;

	for (i=0; i < bias; i++)
	{
		alpha=exp(-((double) i*i)/m_dValue);
		kernel[i/KernelRank]+=alpha/m_dValue2;
	}

	normalize=0;
	for (i=0; i < width; i++)
		normalize+=kernel[i];

	for (i=0; i < width; i++)
		kernel[i]/=normalize;
	return(width);
}

#define MaxCRgbaquad  255UL

vector<double> CMotionBlur::GetMotionBlurKernel(const double &radius,const double sigma)
{
	vector<double> kernel;
	vector<double> last_kernel;
	if (radius > 0)
		GenerateMotionBlurKernel((int)(2.0*ceil(radius) + 1.0), sigma, kernel);
	else
	{
		int width = GenerateMotionBlurKernel(3, sigma, kernel);
		while ((MaxCRgbaquad*kernel[width - 1]) > 0.0)
		{
			if (last_kernel.size() != 0)
				last_kernel.clear();

			copy(kernel.begin(), kernel.end(), back_inserter(last_kernel));
			kernel.clear();
			width = GenerateMotionBlurKernel(width + 2, sigma, kernel);
		}
		if (last_kernel.size() != 0)
		{
			kernel.clear();
			copy(last_kernel.begin(), last_kernel.end(), back_inserter(kernel));
		}
	}

	return kernel;
}

bool CMotionBlur::MotionBlur(CRegardsBitmap * bitmap, const double &radius, const double &sigma, const double &angle)
{
	if (bitmap != nullptr)
	{
		vector<double> kernel;
		vector<wxPoint> offsets;

		if (sigma == 0.0)
			return 0;

		kernel = GetMotionBlurKernel(radius, sigma);

		if (kernel.size() < 3)
			return false;

		offsets = GetOffsetKernel(kernel.size(), angle);

		Execute(bitmap, kernel, offsets);
	}
	return true;
}

void CMotionBlur::Execute(CRegardsBitmap * bitmap, const vector<double> & kernel, const vector<wxPoint> & offsets)
{
	uint8_t * pBitsSrc = bitmap->GetPtBitmap();
	//long i = 0;
	long bmHeight = bitmap->GetBitmapHeight();
	long bmWidth = bitmap->GetBitmapWidth();
	long m_lSize = bmHeight * bmWidth * 4;
	long lWidthSize = bitmap->GetWidthSize();
	uint8_t * lData = new uint8_t[m_lSize * sizeof(uint8_t)];

#pragma omp parallel for
	for (auto y = 0; y < bmHeight; y++)
	{
#pragma omp parallel for
		for (auto x = 0; x < bmWidth; x++)
		{
			int position = y * bmWidth * 4 + x * 4;
			int r = 0,g = 0,b = 0;//, opacity = 0;

			for (auto i = 0; i < kernel.size(); i++)
			{
				int u = x + (long)offsets[i].x;
				int v = y + (long)offsets[i].y;
				if ((u < 0) || (u >= bmWidth) || (v < 0) || (v >= bmHeight))
					continue;

				long ptImage = (v * lWidthSize) + (u * 4);

				r += kernel[i] * *(pBitsSrc + ptImage);
				g += kernel[i] * *(pBitsSrc + ptImage + 1);
				b += kernel[i] * *(pBitsSrc + ptImage + 2);
			}
			
			if (r < 0)
				r = 0;
			else if (r > 255)
				r = 255;

			*(lData + position) = (uint8_t)r;

			if (g < 0)
				g = 0;
			else if (g > 255)
				g = 255;

			*(lData + position + 1) = (uint8_t)g;


			if (b < 0)
				b = 0;
			else if (b > 255)
				b = 255;

			*(lData + position + 2) = (uint8_t)b;

			*(lData + position + 3) = 0;
		}
	}

	bitmap->SetBitmap(lData, (int)bmWidth, bmHeight);

	delete[] lData;
}