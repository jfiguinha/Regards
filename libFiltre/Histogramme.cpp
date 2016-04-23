#include "Histogramme.h"
#include "Color.h"
#include <RegardsBitmap.h>
#include <stdint.h>
#include <math.h>
using namespace Regards::FiltreEffet;

#define RGB2GRAY(r,g,b) (((b)*117 + (g)*601 + (r)*306) >> 10)

CHistogramme::CHistogramme(void)
{
	m_lMaxMoyenneValue = 0;
	m_lMaxRedValue = 0;
	m_lMaxGreenValue = 0;
	m_lMaxBlueValue = 0;
}

CHistogramme::~CHistogramme(void)
{
}

//----------------------------------------------------------------
//----------------------------------------------------------------
void CHistogramme::ParcoursBitmap(CRegardsBitmap * bitmap)
{
	memset(&m_lMoyenneValue,0,256*sizeof(long));
	memset(&m_lRedValue,0,256*sizeof(long));
	memset(&m_lGreenValue,0,256*sizeof(long));
	memset(&m_lBlueValue,0,256*sizeof(long));
	if(bitmap != nullptr)
	{
		for(int y = 0;y < bitmap->GetBitmapHeight();y++)
		{
			for(int x = 0;x < bitmap->GetBitmapWidth();x++)
			{
				CRgbaquad m_color = bitmap->GetColorValue(x, y);
				
				int Moyenne = m_color.GetGreyValue();
				m_lMoyenneValue[Moyenne] = m_lMoyenneValue[Moyenne] + 1;
				if(m_lMoyenneValue[Moyenne] > m_lMaxMoyenneValue)
					m_lMaxMoyenneValue = m_lMoyenneValue[Moyenne];

				m_lRedValue[m_color.GetRed()]++;
				if(m_lRedValue[m_color.GetRed()] > m_lMaxRedValue)
					m_lMaxRedValue = m_lRedValue[m_color.GetRed()];

				m_lGreenValue[m_color.GetGreen()]++;
				if(m_lGreenValue[m_color.GetGreen()] > m_lMaxGreenValue)
					m_lMaxGreenValue = m_lGreenValue[m_color.GetGreen()];

				m_lBlueValue[m_color.GetBlue()]++;
				if (m_lBlueValue[m_color.GetBlue()] > m_lMaxBlueValue)
					m_lMaxBlueValue = m_lBlueValue[m_color.GetBlue()];
			}
		}
	}
}

//----------------------------------------------------------------
//----------------------------------------------------------------
int CHistogramme::GetValue(int iColor, int iYMax, int iPos)
{
	int m_iYSize = 0;
	switch(iColor)
	{
		case 0:
			{
				if(m_lMoyenneValue[iPos] != 0)
					m_iYSize = (m_lMoyenneValue[iPos] * iYMax) / m_lMaxMoyenneValue;
			}
			break;

		case 1:
			{
				if(m_lRedValue[iPos] != 0)
					m_iYSize = (m_lRedValue[iPos] * iYMax) / m_lMaxRedValue;
			}
			break;

		case 2:
			{
				if(m_lGreenValue[iPos] != 0)
					m_iYSize = (m_lGreenValue[iPos] * iYMax) / m_lMaxGreenValue;
			}
			break;

		case 3:
			{
				if(m_lBlueValue[iPos] != 0)
					m_iYSize = (m_lBlueValue[iPos] * iYMax) / m_lMaxBlueValue;
			}
			break;
	}

	return (iYMax - m_iYSize) + 5;
}


////////////////////////////////////////////////////////////////////////////////
// HistogramEqualize function by <dave> : dave(at)posortho(dot)com
bool CHistogramme::HistogramEqualize(CRegardsBitmap * bitmap)
{
	int32_t histogram[256];
	int32_t map[256];
	int32_t equalize_map[256];
	int32_t x, y, i, j;
	CRgbaquad color;
	CRgbaquad yuvClr;
	uint32_t YVal, high, low;

	memset(&histogram, 0, sizeof(int32_t) * 256);
	memset(&map, 0, sizeof(int32_t) * 256);
	memset(&equalize_map, 0, sizeof(int32_t) * 256);

	// form histogram
	for (y = 0; y < bitmap->GetBitmapHeight(); y++)
	{
		for (x = 0; x < bitmap->GetBitmapWidth(); x++){
			color = bitmap->GetColorValue(x, y);
			YVal = (uint32_t)color.GetGreyValue();
			histogram[YVal]++;
		}
	}

	// integrate the histogram to get the equalization map.
	j = 0;
	for (i = 0; i <= 255; i++){
		j += histogram[i];
		map[i] = j;
	}

	// equalize
	low = map[0];
	high = map[255];
	if (low == high)
		return false;

	for (i = 0; i <= 255; i++)
		equalize_map[i] = (uint32_t)((((double)(map[i] - low)) * 255) / (high - low));

	// stretch the histogram

#pragma omp parallel for
	for (int y = 0; y < bitmap->GetBitmapHeight(); y++)
	{
#pragma omp parallel for
		for (int x = 0; x < bitmap->GetBitmapWidth(); x++)
		{
			CRgbaquad color = bitmap->GetColorValue(x, y);
			CRgbaquad yuvClr = CColor::RGBtoYUV(color);
			yuvClr.SetRed((uint8_t)equalize_map[yuvClr.GetRed()]);
			color = CColor::YUVtoRGB(yuvClr);
			bitmap->SetColorValue(x, y, color);
		}
	}

	return true;
}


////////////////////////////////////////////////////////////////////////////////
// HistogramLog function by <dave> : dave(at)posortho(dot)com
bool CHistogramme::HistogramLog(CRegardsBitmap * bitmap)
{
	//q(i,j) = 255/log(1 + |high|) * log(1 + |p(i,j)|);
	int32_t x, y, i;
	CRgbaquad  color;
	CRgbaquad	yuvClr;
	int32_t map[256];
	int32_t equalize_map[256];
	uint32_t YVal, high = 1, low;
	int32_t histogram[256];

	memset(&histogram, 0, sizeof(int32_t) * 256);
	memset(&map, 0, sizeof(int32_t) * 256);
	memset(&equalize_map, 0, sizeof(int32_t) * 256);

	// Find Highest Luminance Value in the Image

	for (y = 0; y < bitmap->GetBitmapHeight(); y++)
	{
		for (x = 0; x < bitmap->GetBitmapWidth(); x++)
		{
			color = bitmap->GetColorValue(x, y);
			YVal = (uint32_t)color.GetGreyValue();
			if (YVal > high) 
				high = YVal;

			histogram[YVal]++;
		}
	}

	// integrate the histogram to get the equalization map.
	int j = 0;
	for (i = 0; i <= 255; i++){
		j += histogram[i];
		map[i] = j;
	}

	// equalize
	low = map[0];
	high = map[255];
	if (low == high) 
		return false;

#pragma omp parallel for
	for (i = 0; i <= 255; i++)
	{
		equalize_map[i] = (uint32_t)((((double)(map[i] - low)) * 255) / (high - low));
	}


	// Logarithm Operator
	//double k = 255.0 / ::log(1.0 + (double)high);

#pragma omp parallel for
	for (int y = 0; y < bitmap->GetBitmapHeight(); y++)
	{
#pragma omp parallel for
		for (int x = 0; x < bitmap->GetBitmapWidth(); x++)
		{
			CRgbaquad color = bitmap->GetColorValue(x, y);
			CRgbaquad yuvClr = CColor::RGBtoYUV(color);
			yuvClr.SetRed((uint8_t)equalize_map[yuvClr.GetRed()]);
			color = CColor::YUVtoRGB(yuvClr);
			bitmap->SetColorValue(x, y, color);
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
// HistogramEqualize function by <dave> : dave(at)posortho(dot)com
bool CHistogramme::HistogramNormalize(CRegardsBitmap * bitmap)
{
	int32_t histogram[256];
	int32_t threshold_intensity, intense;
	int32_t x, y, i;
	uint32_t normalize_map[256];
	uint32_t high, low, YVal;

	CRgbaquad   color;
	CRgbaquad	yuvClr;

	memset(&histogram, 0, sizeof(int32_t) * 256);
	memset(&normalize_map, 0, sizeof(uint32_t) * 256);

	// form histogram
	for (y = 0; y < bitmap->GetBitmapHeight(); y++)
	{
		for (x = 0; x < bitmap->GetBitmapWidth(); x++)
		{
			color = bitmap->GetColorValue(x, y);
			YVal = (uint32_t)color.GetGreyValue();;
			histogram[YVal]++;
		}
	}

	// find histogram boundaries by locating the 1 percent levels
	threshold_intensity = (bitmap->GetBitmapWidth() * bitmap->GetBitmapHeight()) / 100;

	intense = 0;

	for (low = 0; low < 255; low++){
		intense += histogram[low];
		if (intense > threshold_intensity)	break;
	}

	intense = 0;
	for (high = 255; high != 0; high--){
		intense += histogram[high];
		if (intense > threshold_intensity) break;
	}

	if (low == high){
		// Unreasonable contrast;  use zero threshold to determine boundaries.
		threshold_intensity = 0;
		intense = 0;
		for (low = 0; low < 255; low++){
			intense += histogram[low];
			if (intense > threshold_intensity)	break;
		}
		intense = 0;
		for (high = 255; high != 0; high--){
			intense += histogram[high];
			if (intense > threshold_intensity)	break;
		}
	}
	if (low == high) return false;  // zero span bound

	// Stretch the histogram to create the normalized image mapping.
	for (i = 0; i <= 255; i++){
		if (i < (int32_t)low){
			normalize_map[i] = 0;
		}
		else {
			if (i >(int32_t) high)
				normalize_map[i] = 255;
			else
				normalize_map[i] = (255 - 1) * (i - low) / (high - low);
		}
	}

	// Normalize

#pragma omp parallel for
	for (int y = 0; y < bitmap->GetBitmapHeight(); y++)
	{
#pragma omp parallel for
		for (int x = 0; x < bitmap->GetBitmapWidth(); x++)
		{
			CRgbaquad color = bitmap->GetColorValue(x, y);
			CRgbaquad yuvClr = CColor::RGBtoYUV(color);
			yuvClr.SetRed((uint8_t)normalize_map[yuvClr.GetRed()]);
			color = CColor::YUVtoRGB(yuvClr);
			bitmap->SetColorValue(x, y, color);
		}
	}


	return true;
}
