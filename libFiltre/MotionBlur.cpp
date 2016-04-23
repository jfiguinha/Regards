#include "MotionBlur.h"
#include <math.h>
#include <stdlib.h>
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


int CMotionBlur::GetMotionBlurKernel(int width,const double sigma,double **kernel)
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

	*kernel=(double *)malloc(width*sizeof(double));

	if (*kernel == (double *) nullptr)
		return(0);

	for (i=0; i < width; i++)
		(*kernel)[i]=0.0;

	bias=KernelRank*width;

	double m_dValue = (2.0*KernelRank*KernelRank*sigma*sigma);
	double m_dValue2 = MagickSQ2PI*sigma;

	for (i=0; i < bias; i++)
	{
		alpha=exp(-((double) i*i)/m_dValue);
		(*kernel)[i/KernelRank]+=alpha/m_dValue2;
	}

	normalize=0;
	for (i=0; i < width; i++)
		normalize+=(*kernel)[i];

	for (i=0; i < width; i++)
		(*kernel)[i]/=normalize;
	return(width);
}

#define MaxCRgbaquad  255UL

bool CMotionBlur::MotionBlur(CRegardsBitmap * m_CRegardsBitmap, const double &radius,const double &sigma,const double &angle)
{
	double *kernel;

	typedef struct _PointInfo
	{
	  double
		x,
		y;
	} PointInfo;

	int width;

	long y;

	PointInfo *offsets;

	long x, u, v;

	if (sigma == 0.0)
		return 0;

	kernel=(double *) nullptr;
	if (radius > 0)
	{
		width=GetMotionBlurKernel((int) (2.0*ceil(radius)+1.0),sigma,&kernel);
	}
	else
	{
		double * last_kernel = nullptr;

		width=GetMotionBlurKernel(3,sigma,&kernel);
		while ((MaxCRgbaquad*kernel[width-1]) > 0.0)
		{
			if (last_kernel != nullptr)
				free(last_kernel);
			last_kernel=kernel;
			kernel = nullptr;
			width = GetMotionBlurKernel(width+2,sigma,&kernel);
		}
		if (last_kernel != nullptr)
		{
			free(kernel);
			width-=2;
			kernel=last_kernel;
		}
	}
	if (width < 3)
		return false;

	offsets=(PointInfo *)malloc(width*sizeof(PointInfo));
	if (offsets == nullptr)
		return false;

  
	if (m_CRegardsBitmap->GetPtBitmap() != nullptr)
	{
		uint8_t * pBitsSrc = m_CRegardsBitmap->GetPtBitmap();
		long i = 0;
		long bmHeight = m_CRegardsBitmap->GetBitmapHeight();
		long bmWidth = m_CRegardsBitmap->GetBitmapWidth();
		long m_lSize = bmHeight * bmWidth * 4;
		long lWidthSize = m_CRegardsBitmap->GetWidthSize();

		uint8_t * lData = new uint8_t[m_lSize * sizeof(uint8_t)];

		memset(lData,0,m_lSize * sizeof(uint8_t));

		long r,g,b,opacity;

		long ptImage;

		//blur_image->storage_class=DirectClass;
		x=(long) (width*sin(DegreesToRadians(angle)));
		y=(long) (width*cos(DegreesToRadians(angle)));

		for (i=0; i < width; i++)
		{
			offsets[i].x=i*x/sqrt((double)x*x+y*y);
			offsets[i].y=i*y/sqrt((double)x*x+y*y);
		}

		ptImage = 0;

		for (y=0; y < (long)bmHeight; y++)
		{
			for (x=0; x < (long)bmWidth; x++)
			{
				r=g=b=opacity=0;
				
				for (i=0; i < width; i++)
				{
					u=x+(long)offsets[i].x;
					v=y+(long)offsets[i].y;
					if ((u < 0) || (u >= (long)bmWidth) || (v < 0) || (v >= (long)bmHeight))
						continue;

					//pixel=AcquireOnePixel(image,u,v,exception);
					
					ptImage = (v * lWidthSize) + (u * 4);

					r += kernel[i] * *(pBitsSrc + ptImage);
					g += kernel[i] * *(pBitsSrc + ptImage + 1);
					b += kernel[i] * *(pBitsSrc + ptImage + 2);
					
					//opacity+=kernel[i]*pixel.opacity;
				}

				//ptImage = (y * lWidthSize) + (x * 4);
				
				if(r < 0)
					r = 0;
				else if(r > 255)
					r = 255;

				*(lData + ptImage) = (uint8_t)r;
			
				if(g < 0)
					g = 0;
				else if(g > 255)
					g = 255;

				*(lData + ptImage + 1) = (uint8_t)g;

			
				if(b < 0)
					b = 0;
				else if(b > 255)
					b = 255;

				*(lData + ptImage + 2)  = (uint8_t)b;

				*(lData + ptImage + 3)  = 0;

				ptImage += 4;
			}

			
		}
	
		m_CRegardsBitmap->SetBitmap(lData, (int)bmWidth, bmHeight);

		delete[] lData;
		
	}

	free(kernel);
	free(offsets);

	return true;
}