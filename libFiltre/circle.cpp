#include <header.h>
#include "circle.h"
#include <RegardsBitmap.h>
#include <RGBAQuad.h>
#include <LibResource.h>
#include "Color.h"

using namespace Regards::FiltreEffet;
const double pi = 3.14159265358979323846264338327950288419716939937510;
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
wxImage CCircle::GenerateCircle(const CRgbaquad &m_color, const int &iTaille, const float &fAlpha)
{
	wxImage image = CreateFromSVG(iTaille * 2, iTaille * 2, CLibResource::GetVector("IDB_CIRCLE"));

	uint8_t * alpha = nullptr;
	if (image.HasAlpha())
		alpha = image.GetAlpha();

	uint8_t * data = image.GetData();

	int width = image.GetWidth();
	int height = image.GetHeight();

#pragma omp parallel for
    for (auto y = 0; y < height; y++)
    {
#pragma omp parallel for
        for (auto x = 0; x < width; x++)
		{
			int i = y * width + x;
			if (data[i * 3] == 0 && data[i * 3 + 1] == 0 && data[i * 3 + 2] == 0)
			{
				if (alpha != nullptr)
					alpha[i] = alpha[i] * (1.0f - fAlpha);
			}
		}
	}

	image.Replace(0, 0, 0,
		m_color.GetRed(), m_color.GetGreen(), m_color.GetBlue());

	return image;
}


///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
wxImage CCircle::GradientTransparent(const CRgbaquad &m_color, const int &iTaille, const float &fAlpha)
{
	wxImage image = CreateFromSVG(iTaille, iTaille, CLibResource::GetVector("IDB_CIRCLE"));

	int rayon = iTaille / 2;

	uint8_t * alpha = nullptr;
	if (image.HasAlpha())
		alpha = image.GetAlpha();

	uint8_t * data = image.GetData();

	int width = image.GetWidth();
	int height = image.GetHeight();

#pragma omp parallel for
    for (auto y = 0; y < height; y++)
    {
#pragma omp parallel for
        for (auto x = 0; x < width; x++)
		{
			int i = y * width + x;
			if (data[i * 3] == 0 && data[i * 3 + 1] == 0 && data[i * 3 + 2] == 0)
			{
				float fValue = sqrt((double)((x - rayon) * (x - rayon) + (y - rayon) *  (y - rayon)));
				alpha[i] = 0;
				if (fValue <= rayon)
				{
					int valeurAlpha = 255 - (int)((fValue / (float)rayon) * 255.0f);

					if (alpha != nullptr)
						alpha[i] = valeurAlpha;
				}
				i++;
			}
		}
	}

	image.Replace(0, 0, 0,
		m_color.GetRed(), m_color.GetGreen(), m_color.GetBlue());

	return image;
}

wxImage CCircle::Burst(const int &iTaille, const int &iColor, const int &iIntensity, const int &iColorIntensity)
{
	float y1 = 0.6f;
	float x1 = 0.5f;

	float a = (y1 - 1.0f) / (x1 * (x1 - 1.0f));
	float b = 1.0f - a;
	vector<CRgbaquad> listColor;

	int rayon = iTaille/2;

	int i = 0;

	for (i = 0; i <= iTaille; i++)
	{
		float fAlpha;
		//double m_iIntensity;

		//int j = iTaille - i;

		//Test nouvelle version de l'intensité

		float k = (float)i / (float)iTaille;

		fAlpha = a * (k * k) + b * k;

		if (fAlpha < 0.0f)
			fAlpha = 0.0f;

		HSB m_value = { iColor, static_cast<long>(iColorIntensity * fAlpha), 100 };
		CRgbaquad m_rgbValue;
		CColor::HSBToRGB(m_value, m_rgbValue);
		m_rgbValue.SetAlpha(fAlpha * 255.0f);
		listColor.push_back(m_rgbValue);

	}


	wxImage image = CreateFromSVG(iTaille, iTaille, CLibResource::GetVector("IDB_CIRCLE"));


	

	uint8_t * alpha = nullptr;
	if (image.HasAlpha())
		alpha = image.GetAlpha();

	uint8_t * data = image.GetData();

	int width = image.GetWidth();
	int height = image.GetHeight();

#pragma omp parallel for
	for (auto y = 0; y < height; y++)
    {
#pragma omp parallel for
        for (auto x = 0; x < width; x++)
		{
			int i = y * width + x;
			if (data[i * 3] == 0 && data[i * 3 + 1] == 0 && data[i * 3 + 2] == 0)
			{
				float fValue = sqrt((double)((x - rayon) * (x - rayon) + (y - rayon) *  (y - rayon)));
				int distance = (int)fValue;
				alpha[i] = 0;
				if (fValue <= rayon)
				{
					int valeurAlpha = 255 - (int)((fValue / (float)rayon) * 255.0f);

					if (alpha != nullptr)
						alpha[i] = valeurAlpha;
				}

				if (distance <= iTaille)
				{
					data[i * 3] = listColor[distance].GetRed();
					data[i * 3 + 1] = listColor[distance].GetGreen();
					data[i * 3 + 2] = listColor[distance].GetBlue();
				}
				i++;
			}
		}
	}

	return image;

}

wxImage CCircle::HaloGradient(const int &iTaille, const int &iWidth, const float &fAlpha2)
{
	vector<CRgbaquad> listColor;
	int rayon = iTaille / 2;
	int iNb = (360 / iWidth);
	int i = 0;

	for (i = iTaille - iWidth; i <= iTaille; i++)
	{
		int j = iTaille - i;
		if (!(j < 0))
		{
			double m_iIntensity = ((double)((double)(i) / (double)(100)));

			m_iIntensity = asin(m_iIntensity);

			double m = (m_iIntensity * 90) / pi;

			m_iIntensity = exp(-m*m*0.006)*0.50 + exp(-m*0.03)*(1 - 0.50);


		}

		HSB m_value = { iNb * j, 50, 100 };
		CRgbaquad m_rgbValue;
		CColor::HSBToRGB(m_value, m_rgbValue);

		listColor.push_back(m_rgbValue);

	}


	wxImage image = CreateFromSVG(iTaille, iTaille, CLibResource::GetVector("IDB_CIRCLE"));

	uint8_t * alpha = nullptr;
	if (image.HasAlpha())
		alpha = image.GetAlpha();

	uint8_t * data = image.GetData();

	//int size = image.GetHeight() * image.GetWidth();

	int width = image.GetWidth();
	int height = image.GetHeight();

#pragma omp parallel for
    for (auto y = 0; y < height; y++)
    {
#pragma omp parallel for
        for (auto x = 0; x < width; x++)
		{
			int i = y * width + x;
			if (data[i * 3] == 0 && data[i * 3 + 1] == 0 && data[i * 3 + 2] == 0)
			{
				float fValue = sqrt((double)((x - rayon) * (x - rayon) + (y - rayon) *  (y - rayon)));
				int distance = (int)fValue;
				alpha[i] = 0;
			
				if (distance <= rayon && distance >= (rayon - iWidth))
				{
					int position = iWidth - (rayon - distance);
					data[i * 3] = listColor[position].GetRed();
					data[i * 3 + 1] = listColor[position].GetGreen();
					data[i * 3 + 2] = listColor[position].GetBlue();
					alpha[i] = 255 - (fAlpha2 * 255.0f);
				}	
				else
				{
					int position = iWidth - (rayon - distance);
					if (position > 0)
					{
						data[i * 3] = listColor[iWidth].GetRed();
						data[i * 3 + 1] = listColor[iWidth].GetGreen();
						data[i * 3 + 2] = listColor[iWidth].GetBlue();
					}
					else
					{
						data[i * 3] = listColor[0].GetRed();
						data[i * 3 + 1] = listColor[0].GetGreen();
						data[i * 3 + 2] = listColor[0].GetBlue();
					}
				}
			}
			i++;
		}
	}

	return image;
}


wxImage CCircle::Halo(const int &iColor, const int &iColorIntensity, const int &iTaille, const int &iWidth, const float &fAlpha2, const int &iCentre)
{
	int rayon = iTaille / 2;
	vector<CRgbaquad> listColorCenter;
	vector<CRgbaquad> listColorOut;

	wxImage image = CreateFromSVG(iTaille, iTaille, CLibResource::GetVector("IDB_CIRCLE"));

	float y1 = 0.3f;
	float x1 = 0.5f;
	float a = (y1 - 1.0f) / (x1 * (x1 - 1.0f));
	float b = 1.0f - a;
	int i = 0;

	if (iCentre)
	{
		for (i = 0; i < iTaille - iWidth; i++)
		{
			float fAlpha = ((float)i / (float)(iTaille));
			fAlpha = (a * (fAlpha * fAlpha) + b * fAlpha);

			if (fAlpha < 0.0f)
				fAlpha = 0.0f;

			HSB m_value = { iColor, static_cast<long>((fAlpha * iColorIntensity)), 100 };
			CRgbaquad m_rgbValue;
			CColor::HSBToRGB(m_value, m_rgbValue);
			listColorCenter.push_back(m_rgbValue);
			//MidpointCircle(x, y, i, m_rgbValue, fAlpha2);
		}
	}

	const double m_dCent = 100;
	for (i = iTaille - iWidth; i <= iTaille; i++)
	{
		float fAlpha;
		int j = 100 - i;
		if (j < 0)
			fAlpha = 1.0f;
		else
		{
			double m_iIntensity = ((double)((double)(i) / m_dCent));

			m_iIntensity = asin(m_iIntensity);

			double m = (m_iIntensity * 90) / pi;

			m_iIntensity = exp(-m*m*0.006)*0.50 + exp(-m*0.03)*(1 - 0.50);

			fAlpha = 1.0f - m_iIntensity;

			if (fAlpha > 1.0f)
				fAlpha = 1.0f;
		}

		HSB m_value = { iColor, static_cast<long>((fAlpha * iColorIntensity)), 100 };
		CRgbaquad m_rgbValue;
		CColor::HSBToRGB(m_value, m_rgbValue);
		listColorOut.push_back(m_rgbValue);
		/*
		if (i == iTaille - 1)
			MidpointCircle(x, y, i, m_rgbValue, fAlpha2, true);
		else
			MidpointCircle(x, y, i, m_rgbValue, fAlpha2);
		*/
	}

	uint8_t * alpha = nullptr;
	if (image.HasAlpha())
		alpha = image.GetAlpha();

	uint8_t * data = image.GetData();

	int width = image.GetWidth();
	int height = image.GetHeight();
#pragma omp parallel for
    for (auto y = 0; y < height; y++)
	{
#pragma omp parallel for
		for (auto x = 0; x < width; x++)
		{
			int i = y * width + x;
			if (data[i * 3] == 0 && data[i * 3 + 1] == 0 && data[i * 3 + 2] == 0)
			{
				float fValue = sqrt((double)((x - rayon) * (x - rayon) + (y - rayon) *  (y - rayon)));
				int distance = (int)fValue;
				alpha[i] = 0;

				if (distance < (rayon - iWidth) && iCentre)
				{
					data[i * 3] = listColorCenter[distance].GetRed();
					data[i * 3 + 1] = listColorCenter[distance].GetGreen();
					data[i * 3 + 2] = listColorCenter[distance].GetBlue();
					alpha[i] = 255 - (fAlpha2 * 255.0f);					
				}				
				else if (distance <= rayon && distance >= (rayon - iWidth))
				{
					int position = iWidth - (rayon - distance);
					data[i * 3] = listColorOut[position].GetRed();
					data[i * 3 + 1] = listColorOut[position].GetGreen();
					data[i * 3 + 2] = listColorOut[position].GetBlue();
					alpha[i] = 255 - (fAlpha2 * 255.0f);
				}
				else
				{
					data[i * 3] = 255;
					data[i * 3 + 1] = 255;
					data[i * 3 + 2] = 255;
				}
			}
			i++;
		}
	}

	return image;
}