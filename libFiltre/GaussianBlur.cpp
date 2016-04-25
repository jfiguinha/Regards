#include "GaussianBlur.h"
#include <algorithm>
using namespace Regards::FiltreEffet;

int CGaussianBlur::GaussianBlur(CRegardsBitmap * pBitmap, const int &r)
{
	width = pBitmap->GetBitmapWidth();
	height = pBitmap->GetBitmapHeight();
	bitmapSize = width * height * 4;
	gaussCoeff = BoxesForGauss(r, 3);
	
	uint8_t * scl = new uint8_t[bitmapSize];
	uint8_t * tcl = new uint8_t[bitmapSize];
	memcpy(scl, pBitmap->GetPtBitmap(), bitmapSize);
	memcpy(tcl, pBitmap->GetPtBitmap(), bitmapSize);

	BoxBlur(scl, tcl, (gaussCoeff[0] - 1) / 2);
	BoxBlur(tcl, scl, (gaussCoeff[1] - 1) / 2);
	BoxBlur(scl, tcl, (gaussCoeff[2] - 1) / 2);

	memcpy(pBitmap->GetPtBitmap(), tcl, bitmapSize);

	//delete[] scl;
	delete[] tcl;

	return 0;
}

void CGaussianBlur::BoxBlur(uint8_t * & pBitsSrc, uint8_t * & pBitsDest, const int &coeff)
{
	uint8_t * scl = new uint8_t[bitmapSize];
	uint8_t * tcl = new uint8_t[bitmapSize];
	memcpy(scl, pBitsSrc, bitmapSize);
	memcpy(tcl, pBitsSrc, bitmapSize);
	BoxBlurH_3(tcl, scl, coeff);
	BoxBlurV_3(scl, tcl, coeff);
	memcpy(pBitsDest, tcl, bitmapSize);
	delete[] scl;
	delete[] tcl;
}

void CGaussianBlur::BoxBlurH_3(uint8_t * & scl, uint8_t * & tcl, const int &coeff)
{
	int w = width * 4;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int valr = 0;
			int valg = 0;
			int valb = 0;
			for (int ix = j - coeff; ix < j + coeff + 1; ix++)
			{
				int x = min(width - 1, max(0, ix));
				x = x * 4;
				valr += scl[i*w + x];
				valg += scl[i*w + 1 + x];
				valb += scl[i*w + 2 + x];
			}

			int x = j * 4;
			tcl[i*w + x] = (int)((double)valr / (double)(coeff + coeff + 1));
			tcl[i*w + x + 1] = (int)((double)valg / (double)(coeff + coeff + 1));
			tcl[i*w + x + 2] = (int)((double)valb / (double)(coeff + coeff + 1));
		}
	}
	
}

void CGaussianBlur::BoxBlurV_3(uint8_t * & scl, uint8_t * & tcl, const int &coeff)
{
	int w = width * 4;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int x = j * 4;
			int valr = 0;
			int valg = 0;
			int valb = 0;
			for (int iy = i - coeff; iy<i + coeff + 1; iy++)
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

void CGaussianBlur::BoxBlurH_4(uint8_t * & scl, uint8_t * & tcl, const int &coeff)
{
	double iarr = (double)1.0 / (double)(coeff + coeff + 1);
	for (int i = 0; i<height; i++) 
	{
		int ti = i*(width * 4), li = ti, ri = ti + coeff;
		int fvr = scl[ti], lvr = scl[ti + (width - 1)*4], valr = (coeff + 1)*fvr;
		int fvg = scl[ti+1], lvg = scl[ti + 1 + (width - 1)*4], valg = (coeff + 1)*fvg;
		int fvb = scl[ti+2], lvb = scl[ti + 2 + (width - 1)*4], valb = (coeff + 1)*fvb;
		
		for (int j = 0; j < coeff; j++)
		{
			valr += scl[ti + j];
			valg += scl[ti + j + 1];
			valb += scl[ti + j + 2];
		}
			
		for (int j = 0; j <= coeff; j++)
		{ 
			valr += scl[ri] - fvr;   
			valg += scl[ri+1] - fvg;
			valb += scl[ri+2] - fvb;
			tcl[ti] = round(valr*iarr);
			tcl[ti+1] = round(valg*iarr);
			tcl[ti+2] = round(valb*iarr);

			ri+=4;
			ti+=4;
		}
		
		for (int j = coeff + 1; j<width - coeff; j++)
		{ 
			valr += scl[ri] - scl[li];  
			valg += scl[ri+1] - scl[li+1];
			valb += scl[ri+2] - scl[li+2];
			tcl[ti] = round(valr*iarr);
			tcl[ti+1] = round(valg*iarr);
			tcl[ti+2] = round(valb*iarr);

			ri+=4;
			li+=4;
			ti+=4;
		}
		
		for (int j = width - coeff; j< width; j++)
		{ 
			valr += lvr - scl[li];
			valg += lvg - scl[li+1];
			valb += lvb - scl[li+2];
			tcl[ti] = round(valr*iarr); 
			tcl[ti+1] = round(valg*iarr);
			tcl[ti+2] = round(valb*iarr);

			li += 4;
			ti += 4;
		}
	}
}

void CGaussianBlur::BoxBlurV_4(uint8_t * & scl, uint8_t * & tcl, const int &coeff)
{
	int w = width * 4;
	double iarr = (double)1.0 / (double)(coeff + coeff + 1);
	for (int i = 0; i < width; i++)
	{
		int ti = i, li = ti, ri = ti + coeff*w;
		int fvr = scl[ti], lvr = scl[ti + w *(height - 1)], valr = (coeff + 1)*fvr;
		int fvg = scl[ti + 1], lvg = scl[ti + 1 + w *(height - 1)], valg = (coeff + 1)*fvg;
		int fvb = scl[ti + 2], lvb = scl[ti + 2 + w * (height - 1)], valb = (coeff + 1)*fvb;

		for (int j = 0; j < coeff; j++)
		{
			valr += scl[ti + j*w];
			valg += scl[ti + 1 + j*w];
			valb += scl[ti + 2 + j*w];
		}
		for (int j = 0; j <= coeff; j++)
		{
			valr += scl[ri] - fvr;
			valg += scl[ri + 1] - fvg;
			valb += scl[ri + 2] - fvb;
			tcl[ti] = round(valr*iarr);
			tcl[ti + 1] = round(valg*iarr);
			tcl[ti + 2] = round(valb*iarr);
			ri += w;
			ti += w;
		}

		for (int j = coeff + 1; j < height - coeff; j++)
		{
			valr += scl[ri] - scl[li];
			valg += scl[ri + 1] - scl[li + 1];
			valb += scl[ri + 2] - scl[li + 2];
			tcl[ti] = round(valr*iarr);
			tcl[ti + 1] = round(valg*iarr);
			tcl[ti + 2] = round(valb*iarr);
			li += w;
			ri += w;
			ti += w;
		}

		for (int j = height - coeff; j < height; j++)
		{
			valr += lvr - scl[li];
			valg += lvg - scl[li + 1];
			valb += lvb - scl[li + 3];
			tcl[ti] = round(valr*iarr);
			tcl[ti + 1] = round(valg*iarr);
			tcl[ti + 2] = round(valb*iarr);
			li += w;
			ti += w;
		}
	}
}

//Génération des coefficients pour le filtre gaussian
vector<int> CGaussianBlur::BoxesForGauss(const int & r, const int & boxSize)
{
	double wIdeal = sqrt((12 * r*r / boxSize) + 1);
	int wl = (int)floor(wIdeal);
	if (wl % 2 == 0)
		wl--;
	int wu = wl + 2;
	double mIdeal = (12 * r*r - boxSize*wl*wl - 4 * boxSize*wl - 3 * boxSize) / (-4 * wl - 4);
	int m = round(mIdeal);
	vector<int> sizes;
	for (int i = 0; i<boxSize; i++)
		sizes.push_back(i<m ? wl : wu);
	return sizes;
}
