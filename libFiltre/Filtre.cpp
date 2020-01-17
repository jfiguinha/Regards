#include <header.h>
#include "Filtre.h"
#include "RegardsBitmap.h"
using namespace Regards::FiltreEffet;

#include <tbb/parallel_for.h>
#include <tbb/task_scheduler_init.h>

struct mytask {
	mytask(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest, CFiltre * pt)
	{
		this->x = x;
		this->y = y;
		this->pBitsSrc= pBitsSrc;
		this->pBitsDest = pBitsDest;
		this->filtre = pt;
	}

	void operator()() {
		filtre->PixelCompute(x, y, pBitsSrc, pBitsDest);
	}
	int x;
	int y;
	uint8_t * pBitsSrc;
	uint8_t * pBitsDest;
	CFiltre * filtre;
};

CFiltre::CFiltre()
{

}


CFiltre::~CFiltre()
{
}

void CFiltre::SetParameter(CRegardsBitmap * pBitmap, CRgbaquad color)
{
	this->pBitmap = pBitmap;
	this->color = color;
	bitmapWidthSize = (int)pBitmap->GetWidthSize();
	bmWidth = pBitmap->GetBitmapWidth();
	bmHeight = pBitmap->GetBitmapHeight();
	pictureSize = bmHeight * (bmWidth << 2);
}

int CFiltre::GetPosition(const int &x, const int &y)
{
	if (x >= 0 && x < bmWidth && y < bmHeight && y >= 0)
		return (y * bitmapWidthSize) + (x << 2);
	else
		return -1;
}

//Effet GrayScale
void CFiltre::Compute()
{
	if (pBitmap->GetPtBitmap() != nullptr)
	{
		uint8_t * pBitsSrc;
		uint8_t * pBitsDest;
		pBitsDest = new uint8_t[pictureSize];
		pBitsSrc = pBitmap->GetPtBitmap();

		
		//tbb::task_scheduler_init init;  // Automatic number of threads
		tbb::task_scheduler_init init(tbb::task_scheduler_init::default_num_threads());  // Explicit number of threads

		std::vector<mytask> tasks;
		//#pragma omp parallel for
		for (auto y = 0; y < bmHeight; y++)
		{
			//#pragma omp parallel for
			for (auto x = 0; x < bmWidth; x++)
			{
				tasks.push_back(mytask(x, y, pBitsSrc, pBitsDest, this));
			}
		}

		tbb::parallel_for(
			tbb::blocked_range<size_t>(0, tasks.size()),
			[&tasks](const tbb::blocked_range<size_t>& r) 
			{
				for (size_t i = r.begin(); i < r.end(); ++i) 
					tasks[i]();
			}
		);


		/*
//#pragma omp parallel for
		for (auto y = 0; y < bmHeight; y++)
		{
//#pragma omp parallel for
			for (auto x = 0; x < bmWidth; x++)
			{
				PixelCompute(x, y, pBitsSrc, pBitsDest);
			}
		}
		*/

		pBitmap->SetBitmap(pBitsDest, pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());
		delete[] pBitsDest;
		pBitsDest = nullptr;
	}
}


void CSharpenMasking::PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest)
{
	int position = GetPosition(x, y);
	int origin_r = pBitsSrc[position];
	int origin_g = pBitsSrc[position + 1];
	int origin_b = pBitsSrc[position + 2];

	int _r = 0;
	int _g = 0;
	int _b = 0;

	for (auto i = -1; i <= 1; i++)
	{
		for (auto j = -1; j <= 1; j++)
		{
			int pos = GetPosition(x + j, y + i);
			if (pos != -1)
			{
				_r  += *(pBitsSrc + pos);
				_g  += *(pBitsSrc + pos + 1);
				_b  += *(pBitsSrc + pos + 2);
			}
		}
	}

	_r /= 9;
	_g /= 9;
	_b /= 9;

	_r = origin_r + (origin_r - _r) * sharpness;
	_g = origin_g + (origin_g - _g) * sharpness;
	_b = origin_b + (origin_b - _b) * sharpness;

	_r = _r < 0 ? 0 : _r;
	_r = _r > 255 ? 255 : _r;

	_g = _g < 0 ? 0 : _g;
	_g = _g > 255 ? 255 : _g;

	_b = _b < 0 ? 0 : _b;
	_b = _b > 255 ? 255 : _b;

	pBitsDest[position] = _r;
	pBitsDest[position + 1] = _g;
	pBitsDest[position + 2] = _b;
}

void CGrayScale::PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest)
{
	int position = GetPosition(x, y);
	uint8_t Moyenne = ((pBitsSrc[position] * 0.299) + (pBitsSrc[position + 1] * 0.587) + (pBitsSrc[position + 2] * 0.114));
	uint8_t alpha = pBitsSrc[position + 3];
	uint8_t data[4] = { Moyenne, Moyenne, Moyenne, alpha };
	memcpy(pBitsDest + position, data, 4);
}

void CBlackAndWhite::PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest)
{
	uint8_t iValue = 0;
	int position = GetPosition(x, y);
	double moyenne = ((pBitsSrc[position] * 0.144f) + (pBitsSrc[position + 1] * 0.587f) + (pBitsSrc[position + 2] * 0.299f));
	moyenne < 128.0 ? iValue = 0 : iValue = 255;
	uint8_t alpha = pBitsSrc[position + 3];
	uint8_t data[4] = { iValue, iValue, iValue, alpha };
	memcpy(pBitsDest + position, data, 4);



}

void CBilateral::PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest)
{
	//uint8_t iValue = 0;
	int position = GetPosition(x, y);

	  float weight;
	  float res_x = 0;
	  float sum_x = 0;
  
	  float res_y = 0;
	  float sum_y = 0;

	  float res_z = 0;
	  float sum_z = 0;  
  
	  uint8_t r = pBitsSrc[position];
	  uint8_t g = pBitsSrc[position+1];
	  uint8_t b = pBitsSrc[position+2];
	  float SX = 1.f/sigmaX/sigmaX;
	  float SP = 1.f/sigmaP/sigmaP;

	  int maxPosition = pBitmap->GetBitmapSize();

	  for(int k = -fSize;k<=fSize;k++)
	  {
		for(int m = -fSize;m<=fSize;m++)
		{
			int position_local = (x+k) * 4 + (y+m) * pBitmap->GetBitmapWidth() * 4;
			if(position_local >= 0 && position_local < maxPosition)
			{
				uint8_t r_pix1 = pBitsSrc[position_local];  
				uint8_t g_pix1 = pBitsSrc[position_local+1];  
				uint8_t b_pix1 = pBitsSrc[position_local+2];  
			
                float value = exp(-SX*(k*k+m*m));
            
				weight = value * exp(-SP*((1.f*r-r_pix1)*(1.f*r-r_pix1)));
				res_x += r_pix1*weight;
				sum_x += weight;
			
				weight = value * exp(-SP*((1.f*g-g_pix1)*(1.f*g-g_pix1)));
				res_y += g_pix1*weight;
				sum_y += weight;

				weight = value * exp(-SP*((1.f*b-b_pix1)*(1.f*b-b_pix1)));
				res_z += b_pix1*weight;
				sum_z += weight;
			}
		}
	  }

	  pBitsDest[position] = (res_x/sum_x);
	  pBitsDest[position+1] = (res_y/sum_y);
	  pBitsDest[position+2] = (res_z/sum_z);
}

void CNlmeans::PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest)
{
	//uint8_t iValue = 0;
	int position = GetPosition(x, y);

	float res_x = 0;
	float res_y = 0;
	float res_z = 0;
	
	float sum_x = 0;
	float sum_y = 0;
	float sum_z = 0;

	float weight_x = 0;
	float weight_y = 0;
	float weight_z = 0;
	
	int maxPosition = pBitmap->GetBitmapSize();
				
	for(int i1 = -bSize;i1<=bSize;i1++)
	{
		for(int j1 = -bSize;j1<=bSize;j1++)
		{
			float dist_x = 0;
			float dist_y = 0;		
			float dist_z = 0;	
			uint8_t r;
			uint8_t g;
			uint8_t b;

			uint8_t r_p0;
			uint8_t g_p0;
			uint8_t b_p0;

			uint8_t r_p1;
			uint8_t g_p1;
			uint8_t b_p1;
			
			int position_local = (x+i1) * 4 + (y+j1) * pBitmap->GetBitmapWidth() * 4;
			if(position_local < 0 || position_local >= maxPosition)
				continue;

			r = pBitsSrc[position];
			g = pBitsSrc[position+1];
			b = pBitsSrc[position+2];

			for(int i2 = -fSize;i2<=fSize;i2++)
			{
				for(int j2 = -fSize;j2<=fSize;j2++)
				{
					int pos1 = (x+i2) * 4 + (y+j2) *  pBitmap->GetBitmapWidth();
					if(pos1 < 0 || pos1 >= maxPosition)
						continue;				
					
					int pos2 = (x+i1+i2) * 4 + (y+j1+j2) *  pBitmap->GetBitmapWidth();
					if(pos2 < 0 || pos2 >= maxPosition)
						continue;						
					
					r_p0 = pBitsSrc[pos1];
					g_p0 = pBitsSrc[pos1+1];
					b_p0 = pBitsSrc[pos1+2];

					r_p1 = pBitsSrc[pos2];
					g_p1 = pBitsSrc[pos2 + 1];
					b_p1 = pBitsSrc[pos2 + 2];
					
					dist_x += (1.f*r_p1-1.f*r_p0)*(1.f*r_p1-1.f*r_p0);
					dist_y += (1.f*g_p1-1.f*g_p0)*(1.f*g_p1-1.f*g_p0);
					dist_z += (1.f*b_p1-1.f*b_p0)*(1.f*b_p1-1.f*b_p0);
				  
				}
			}

			dist_x *= 1.f/fSize/fSize;
			dist_y *= 1.f/(2*fSize+1)/(2*fSize+1);
			dist_z *= 1.f/(2*fSize+1)/(2*fSize+1);
			
			weight_x = exp(-1.f/sigma/sigma*dist_x);
			weight_y = exp(-1.f/sigma/sigma*dist_y);
			weight_z = exp(-1.f/sigma/sigma*dist_z);
			
			res_x += 1.f*r*weight_x;
			res_y += 1.f*g*weight_y;
			res_z += 1.f*b*weight_z;
			
			sum_x += weight_x;
			sum_y += weight_y;
			sum_z += weight_z;
		}
	}	
	
	pBitsDest[position] = res_x / sum_x;
	pBitsDest[position+1] = res_y / sum_y;
	pBitsDest[position+2] = res_z / sum_z;

}

#define ELEM_SWAP(a,b) {  short t=(a);(a)=(b);(b)=t; }

short CMedian::quick_select(short * arr, int n)
{
	int low, high;
	int median;
	int middle, ll, hh;

	low = 0; high = n - 1; median = (low + high) / 2;
	for (;;) {
		if (high <= low) /* One element only */
			return arr[median];

		if (high == low + 1) {  /* Two elements only */
			if (arr[low] > arr[high])
				ELEM_SWAP(arr[low], arr[high]);
			return arr[median];
		}

		/* Find median of low, middle and high items; swap into position low */
		middle = (low + high) / 2;
		if (arr[middle] > arr[high])    ELEM_SWAP(arr[middle], arr[high]);
		if (arr[low] > arr[high])       ELEM_SWAP(arr[low], arr[high]);
		if (arr[middle] > arr[low])     ELEM_SWAP(arr[middle], arr[low]);

		/* Swap low item (now in position middle) into position (low+1) */
		ELEM_SWAP(arr[middle], arr[low + 1]);

		/* Nibble from each end towards middle, swapping items when stuck */
		ll = low + 1;
		hh = high;
		for (;;) {
			do ll++; while (arr[low] > arr[ll]);
			do hh--; while (arr[hh]  > arr[low]);

			if (hh < ll)
				break;

			ELEM_SWAP(arr[ll], arr[hh]);
		}

		/* Swap middle item (in position low) back into correct position */
		ELEM_SWAP(arr[low], arr[hh]);

		/* Re-set active partition */
		if (hh <= median)
			low = ll;
		if (hh >= median)
			high = hh - 1;
	}
}

#undef ELEM_SWAP

void CMedian::PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest)
{
	int position = GetPosition(x, y);
	int iSize = 9 * sizeof(short);
	short m_TabRValue[9];
	short m_TabGValue[9];
	short m_TabBValue[9];
	
	memset(m_TabRValue, 0, iSize);
	memset(m_TabGValue, 0, iSize);
	memset(m_TabBValue, 0, iSize);

	int k = 0;

	for (auto i = -1; i <= 1; i++)
	{
		for (auto j = -1; j <= 1; j++)
		{
			int pos = GetPosition(x + j, y + i);
			if (pos != -1)
			{
				m_TabRValue[k] = *(pBitsSrc + pos);
				m_TabGValue[k] = *(pBitsSrc + pos + 1);
				m_TabBValue[k] = *(pBitsSrc + pos + 2);
			}
			k++;
		}
	}

	uint8_t alpha = pBitsSrc[position + 3];
	uint8_t data[4] = { static_cast<uint8_t>(quick_select(m_TabRValue, 9)), static_cast<uint8_t>(quick_select(m_TabGValue, 9)), static_cast<uint8_t>(quick_select(m_TabBValue, 9)), alpha };
	memcpy(pBitsDest + position, data, 4);

}


void CNegatif::PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest)
{
	//uint8_t iValue = 0;
	int position = GetPosition(x, y);
	uint8_t data[4] = { static_cast<uint8_t>(255 - pBitsSrc[position]), static_cast<uint8_t>(255 - pBitsSrc[position + 1]), static_cast<uint8_t>(255 - pBitsSrc[position + 2]), pBitsSrc[position + 3] };
	memcpy(pBitsDest + position, data, 4);
}

void CPhotoFiltre::PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest)
{
	//CRgbaquad color;
	int position = GetPosition(x, y);
	uint8_t alpha = pBitsSrc[position + 3];
	//memcpy(&color, pBitsSrc + position, 4);

	int _r = r + pBitsSrc[position] * diff;
	int _g = g + pBitsSrc[position + 1] * diff;
	int _b = b + pBitsSrc[position + 2] * diff;

	_r = _r < 0 ? 0 : _r;
	_r = _r > 255 ? 255 : _r;

	_g = _g < 0 ? 0 : _g;
	_g = _g > 255 ? 255 : _g;

	_b = _b < 0 ? 0 : _b;
	_b = _b > 255 ? 255 : _b;

	uint8_t data[4] = { static_cast<uint8_t>(_r), static_cast<uint8_t>(_g), static_cast<uint8_t>(_b), alpha };

	memcpy(pBitsDest + position, data, 4);
}

void CRgbFiltre::PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest)
{
	int position = GetPosition(x, y);
	uint8_t alpha = pBitsSrc[position + 3];
	memcpy(&color, pBitsSrc + position, 4);

	int red = color.GetRed() + m_lRValue;
	int green = color.GetGreen() + m_lGValue;
	int blue = color.GetBlue() + m_lBValue;

	red = red < 0 ? 0 : red;
	uint8_t r = red > 255 ? 255 : red;

	green = green < 0 ? 0 : green;
	uint8_t g = green > 255 ? 255 : green;

	blue = blue < 0 ? 0 : blue;
	uint8_t b = blue > 255 ? 255 : blue;

	uint8_t data[4] = { r, g, b, alpha };

	memcpy(pBitsDest + position, data, 4);
}


void CSepiaFiltre::PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest)
{
	CRgbaquad color;
	int position = GetPosition(x, y);
	uint8_t alpha = pBitsSrc[position + 3];
	memcpy(&color, pBitsSrc + position, 4);

	float temp;
	temp = (0.393f * color.GetFRed()) + (0.769f * color.GetFGreen()) + (0.189f * color.GetFBlue());
	uint8_t b = temp>255 ? 255 : temp;
	temp = (0.349f * color.GetFRed()) + (0.686f * color.GetFGreen()) + (0.168f * color.GetFBlue());
	uint8_t g = temp>255 ? 255 : temp;
	temp = (0.272f * color.GetFRed()) + (0.534f * color.GetFGreen()) + (0.131f * color.GetFBlue());
	uint8_t r = temp>255 ? 255 : temp;

	uint8_t data[4] = { r, g, b, alpha };

	memcpy(pBitsDest + position, data, 4);
}

void CMatrixConvolution::PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest)
{
	int position = GetPosition(x, y);
	uint8_t alpha = pBitsSrc[position + 3];
	float red = 0.0;
	float green = 0.0;
	float blue = 0.0;
	int k = 0;
	int start = -Ksize / 2;
	int end = Ksize / 2;
	int Kfactor = 0;

	for (auto i = start; i <= end; i++)
	{
		for (auto j = start; j <= end; j++)
		{
			Kfactor += kernel[k];
			int pos = GetPosition(x + j, y + i);
			if (pos != -1)
			{
				red += *(pBitsSrc + pos) * kernel[k];
				green += *(pBitsSrc + pos + 1) * kernel[k];
				blue += *(pBitsSrc + pos + 2) * kernel[k];
			}
			k++;
			
		}
	}

	red = (red / Kfactor) + Koffset;
	green = (green / Kfactor) + Koffset;
	blue = (blue / Kfactor) + Koffset;

	red = red < 0.0 ? 0.0 : red;
	uint8_t r = red > 255.0 ? 255 : red;

	green = green < 0 ? 0 : green;
	uint8_t g = green > 255 ? 255 : green;

	blue = blue < 0 ? 0 : blue;
	uint8_t b = blue > 255 ? 255 : blue;
	
	uint8_t data[4] = { r, g, b, alpha };
	memcpy(pBitsDest + position, data, 4);

}

void CDilate::PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest)
{
	int position = GetPosition(x, y);
	uint8_t alpha = pBitsSrc[position + 3];
	uint8_t r = 255;
	uint8_t g = 255;
	uint8_t b = 255;

	for (auto i = -1; i <= 1; i++)
	{
		for (auto j = -1; j <= 1; j++)
		{
			int pos = GetPosition(x + j, y + i);
			if (pos != -1)
			{
				r = min(r, *(pBitsSrc + pos));
				g = min(g, *(pBitsSrc + pos + 1));
				b = min(b, *(pBitsSrc + pos + 2));
			}
		}
	}

	uint8_t data[4] = { r, g, b, alpha };
	memcpy(pBitsDest + position, data, 4);
}

void CErode::PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest)
{
	int position = GetPosition(x, y);
	uint8_t alpha = pBitsSrc[position + 3];
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;

	for (auto i = -1; i <= 1; i++)
	{
		for (auto j = -1; j <= 1; j++)
		{
			int pos = GetPosition(x + j, y + i);
			if (pos != -1)
			{
				r = max(r, *(pBitsSrc + pos));
				g = max(g, *(pBitsSrc + pos + 1));
				b = max(b, *(pBitsSrc + pos + 2));
			}
		}
	}

	uint8_t data[4] = { r, g, b, alpha };
	memcpy(pBitsDest + position, data, 4);
}

float CNoise::Noise2d(int x, int y)
{
	int n = ((x + (y << 6)) << 13) ^ (x + (y << 6));
	return 255 * (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

void CNoise::PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest)
{
	int pos = GetPosition(x, y);
	uint8_t alpha = pBitsSrc[pos + 3];
	float n = Noise2d(int(x), int(y));
	int r = *(pBitsSrc + pos) + n;
	int g = *(pBitsSrc + pos + 1) + n;
	int b = *(pBitsSrc + pos + 2) + n;

	
	r = max(0, min(255, r));
	g = max(0, min(255, g));
	b = max(0, min(255, b));

	uint8_t data[4] = { static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), alpha };
	memcpy(pBitsDest + pos, data, 4);
}

void CMosaic::PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest)
{
	//uint8_t r = 0;
	//uint8_t g = 0;
	//uint8_t b = 0;

	float w = size;
	float h = size;

	float s = floor(x / w);
	float t = floor(y / h);

	int posOut = GetPosition(x, y);
	int pos = GetPosition(s*w, t*h);

	uint8_t data[4] = { *(pBitsSrc + pos), *(pBitsSrc + pos + 1), *(pBitsSrc + pos + 2), *(pBitsSrc + pos + 3) };
	memcpy(pBitsDest + posOut, data, 4);
}

float CSwirl::EuclideanDist(FLOATPOINT p)
{
	return abs(p.x - p.y);
}

float CSwirl::EuclideanDist(FLOATPOINT p, FLOATPOINT q)
{
	FLOATPOINT diff;
	diff.x = p.x - q.x;
	diff.y = p.y - q.y;
	return sqrt(diff.x*diff.x + diff.y*diff.y);
}

float CSwirl::DotProduct(FLOATPOINT p, FLOATPOINT q)
{
	float dot;
	dot = p.x * q.x + p.y * q.y;
	return dot;
}

wxPoint CSwirl::PostFX(int x, int y, int width, int height, float radius, float angleDegree)
{
	//Calcul du centre
	FLOATPOINT pt;
	FLOATPOINT pttest;
	wxPoint ptOut;
	FLOATPOINT ptCentre;

	ptCentre.x = float(width) / 2.0;
	ptCentre.y = float(height) / 2.0;

	pttest.x = x;
	pttest.y = y;

	pt.x = x - ptCentre.x;
	pt.y = y - ptCentre.y;

	float angle = angleDegree * 0.0174532925;

	float dist = EuclideanDist(pttest, ptCentre);
	if (dist < radius)
	{
		float percent = (radius - dist) / radius;
		float theta = percent * percent * angle * 8.0;
		float s = sin(theta);
		float c = cos(theta);
		FLOATPOINT pt1;
		pt1.x = c;
		pt1.y = -s;

		FLOATPOINT pt2;
		pt2.x = s;
		pt2.y = c;

		FLOATPOINT ptOut;
		ptOut.x = DotProduct(pt, pt1);
		ptOut.y = DotProduct(pt, pt2);

		pt = ptOut;

		//tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s, c)));
	}
	pt.x += ptCentre.x;
	pt.y += ptCentre.y;


	ptOut.x = pt.x;
	ptOut.y = pt.y;
	return ptOut;

}

void CSwirl::PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest)
{
	//uint8_t data[4];
	wxPoint pt = PostFX(x, y, bmWidth, bmHeight, radius, angle);
	if (pt.x >= 0 && pt.x < bmWidth && pt.y >= 0 && pt.y < bmHeight)
	{
		int positionSrc = GetPosition(pt.x, pt.y); 
		int positionDest = GetPosition(x, y);
		memcpy(pBitsDest + positionDest, pBitsSrc + positionSrc, 4);
	}
	else
	{
		int positionDest = GetPosition(x, y);
		memcpy(pBitsDest + positionDest, &color, 4);
	}
}

void CContrast::PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest)
{
	int position = GetPosition(x, y);
	uint8_t alpha = pBitsSrc[position + 3];

	int r = m_dTemp[pBitsSrc[position]];
	int g = m_dTemp[pBitsSrc[position + 1]];
	int b = m_dTemp[pBitsSrc[position + 2]];

	uint8_t data[4] = { static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), alpha };

	memcpy(pBitsDest + position, data, 4);
}

void CLightness::PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest)
{
	int position = GetPosition(x, y);
	uint8_t alpha = pBitsSrc[position + 3];

	int r = m_dTemp[pBitsSrc[position]];
	int g = m_dTemp[pBitsSrc[position + 1]];
	int b = m_dTemp[pBitsSrc[position + 2]];

	uint8_t data[4] = { static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), alpha };

	memcpy(pBitsDest + position, data, 4);
}

void CPosterize::PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest)
{
	CRgbaquad color;
	int position = GetPosition(x, y);
	//uint8_t alpha = pBitsSrc[position + 3];
	memcpy(&color, pBitsSrc + position, 4);

	int r = color.GetFRed() / _offset;
	int g = color.GetFGreen() / _offset;
	int b = color.GetFBlue() / _offset;

	//double dr = pow(posterize[r], gammaFactor);
	//double dg = pow(posterize[g], gammaFactor);
	//double db = pow(posterize[b], gammaFactor);

	color = CRgbaquad(posterize[r], posterize[g], posterize[b]);

	memcpy(pBitsDest + position, &color, 3);

}

void CSolarize::PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest)
{
	CRgbaquad color;
	int position = GetPosition(x, y);
	//uint8_t alpha = pBitsSrc[position + 3];
	memcpy(&color, pBitsSrc + position, 4);

	color = CRgbaquad(solarize[color.GetRed()], solarize[color.GetGreen()], solarize[color.GetBlue()]);

	memcpy(pBitsDest + position, &color, 3);
}
