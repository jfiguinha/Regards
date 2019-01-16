#include "GaussianBlur.h"
#include <LoadingResource.h>
#include <RegardsBitmap.h>
#ifdef CUDA
#include <Host.h>
#endif
using namespace Regards::FiltreEffet;

int CGaussianBlur::GaussianBlur(CRegardsBitmap * pBitmap, const int &r, const int &boxSize)
{
#ifdef CUDA
	width = pBitmap->GetBitmapWidth();
	height = pBitmap->GetBitmapHeight();
	bitmapSize = width * height * 4;
	uint8_t * data = pBitmap->GetPtBitmap();
	CHost::BoxBlur(data, bitmapSize, width, height, r, boxSize);
#else
	width = pBitmap->GetBitmapWidth();
	height = pBitmap->GetBitmapHeight();
	bitmapSize = width * height * 4;
	gaussCoeff = BoxesForGauss(r, boxSize);
	uint8_t * data = pBitmap->GetPtBitmap();
	temp = new uint8_t[bitmapSize];
	uint8_t * scl = new uint8_t[bitmapSize];
	//memcpy(scl, data, bitmapSize);
	//memcpy(temp, data, bitmapSize);
	for (auto i = 0; i < boxSize; i++)
	{
		memcpy(scl, data, bitmapSize);
		BoxBlur(scl, data, (gaussCoeff[i] - 1) / 2);
	}

	//BoxBlur(data, scl, (gaussCoeff[1] - 1) / 2);
	//BoxBlur(scl, data, (gaussCoeff[2] - 1) / 2);
	delete[] scl;
	delete[] temp;
#endif
	return 0;
}

wxImage CGaussianBlur::GaussianBlurwxImage(CRegardsBitmap * pBitmap, const int &r, const int &boxSize)
{
	GaussianBlur(pBitmap, r, boxSize);
	return CLoadingResource::ConvertTowxImageRGB(pBitmap);
}

void CGaussianBlur::BoxBlur(uint8_t * & pBitsSrc, uint8_t * & pBitsDest, const int &coeff)
{
	//memcpy(temp, pBitsSrc, bitmapSize);
	BoxBlurH_3(pBitsSrc, temp, coeff);
	BoxBlurV_3(temp, pBitsDest, coeff);
}

void CGaussianBlur::BoxBlurH_3(uint8_t * & scl, uint8_t * & tcl, const int &coeff)
{
	int w = width << 2;
#pragma omp parallel for
	for (auto i = 0;i < height; i++)
	{
#pragma omp parallel for
		for (auto j = 0; j < width; j++)
		{
			int valr = 0;
			int valg = 0;
			int valb = 0;
			for (auto ix = j - coeff; ix < j + coeff + 1; ix++)
			{
				int x = min(width - 1, max(0, ix));
				x = x << 2;
				valr += scl[i*w + x];
				valg += scl[i*w + 1 + x];
				valb += scl[i*w + 2 + x];
			}

			int x = j << 2;
			tcl[i*w + x] = (int)((double)valr / (double)(coeff + coeff + 1));
			tcl[i*w + x + 1] = (int)((double)valg / (double)(coeff + coeff + 1));
			tcl[i*w + x + 2] = (int)((double)valb / (double)(coeff + coeff + 1));
		}
	}
	
}

void CGaussianBlur::BoxBlurV_3(uint8_t * & scl, uint8_t * & tcl, const int &coeff)
{
	int w = width * 4;
#pragma omp parallel for
    for (auto i = 0;i < height; i++)
    {
#pragma omp parallel for
			for (auto j = 0; j < width; j++)
		{
			int x = j * 4;
			int valr = 0;
			int valg = 0;
			int valb = 0;
			for (auto iy = i - coeff; iy<i + coeff + 1; iy++)
			{
				int y = min(height - 1, max(0, iy));
				
				valr += scl[y*w + x];
				valg += scl[y*w + 1 + x];
				valb += scl[y*w + 2 + x];
			}

			tcl[i*w + x] = (int)((double)valr / (double)(coeff + coeff + 1));
			tcl[i*w + x + 1] = (int)((double)valg / (double)(coeff + coeff + 1));
			tcl[i*w + x + 2] = (int)((double)valb / (double)(coeff + coeff + 1));
		}
	}

}


//Génération des coefficients pour le filtre gaussian
vector<int> CGaussianBlur::BoxesForGauss(const int & r, const int & boxSize)
{
	double wIdeal = sqrt((double)((12 * r*r / boxSize) + 1));
	int wl = (int)floor(wIdeal);
	if (wl % 2 == 0)
		wl--;
	int wu = wl + 2;
	double mIdeal = (12 * r*r - boxSize*wl*wl - 4 * boxSize*wl - 3 * boxSize) / (-4 * wl - 4);
	int m = round(mIdeal);
	vector<int> sizes;
	for (auto i = 0; i<boxSize; i++)
		sizes.push_back(i<m ? wl : wu);
	return sizes;
}
