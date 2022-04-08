// ReSharper disable All
#include <header.h>
#include "circle.h"
#include <RGBAQuad.h>
#include <LibResource.h>
#include "Color.h"

using namespace Regards::FiltreEffet;
const double pi = 3.14159265358979323846264338327950288419716939937510;

std::map<int, wxImage> CCircle::listOfCircle;

void CCircle::CleanCircle()
{
	listOfCircle.clear();
}

wxImage CCircle::GetCircle(const int &rayon)
{
	if (listOfCircle.find(rayon) != listOfCircle.end())
	{
		return listOfCircle.at(rayon).Copy();
	}
	wxImage image = CLibResource::CreatePictureFromSVG("IDB_CIRCLE", rayon, rayon);
	listOfCircle[rayon] = image;
	return image.Copy();
}

///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
wxImage CCircle::GenerateCircle(const CRgbaquad &m_color, const int &iTaille, const float &fAlpha)
{
	wxImage image = GetCircle(iTaille * 2);// CLibResource::CreatePictureFromSVG("IDB_CIRCLE", iTaille * 2, iTaille * 2);

	uint8_t * alpha = nullptr;
	if (image.HasAlpha())
		alpha = image.GetAlpha();

	uint8_t * data = image.GetData();

	const int width = image.GetWidth();
	const int height = image.GetHeight();

	tbb::parallel_for(0, height, 1, [=](int y) {
			for (auto x = 0; x < width; x++)
			{
				int i = y * width + x;
				if (data[i * 3] == 0 && data[i * 3 + 1] == 0 && data[i * 3 + 2] == 0)
				{
					if (alpha != nullptr)
						alpha[i] *= (1.0f - fAlpha);
				}
			}
		});

	image.Replace(0, 0, 0,
		m_color.GetRed(), m_color.GetGreen(), m_color.GetBlue());

	return image;
}


///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
wxImage CCircle::GradientTransparent(const CRgbaquad &m_color, const int &iTaille, const float &fAlpha)
{
	wxImage image = GetCircle(iTaille); //CLibResource::CreatePictureFromSVG("IDB_CIRCLE", iTaille, iTaille);

	int rayon = iTaille / 2;

	uint8_t * alpha = nullptr;
	if (image.HasAlpha())
		alpha = image.GetAlpha();

	uint8_t * data = image.GetData();

	int width = image.GetWidth();
	int height = image.GetHeight();


	tbb::parallel_for(0, height, 1, [=](int y) {

		for (auto x = 0; x < width; x++)
		{
			int i = y * width + x;
			if (data[i * 3] == 0 && data[i * 3 + 1] == 0 && data[i * 3 + 2] == 0)
			{
				float fValue = sqrt((double)((x - rayon) * (x - rayon) + (y - rayon) * (y - rayon)));
				if (image.HasAlpha())
					alpha[i] = 0;
				if (fValue <= rayon)
				{
					const int valeurAlpha = 255 - (int)((fValue / (float)rayon) * 255.0f);
					if (image.HasAlpha())
						alpha[i] = valeurAlpha;
				}
			}
		}
		});

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

	const int rayon = iTaille/2;

	for (int i = 0; i <= iTaille; i++)
	{
		float k = (float)i / (float)iTaille;

		float f_alpha = a * (k * k) + b * k;

		if (f_alpha < 0.0f)
			f_alpha = 0.0f;

		HSB m_value = { iColor, static_cast<long>(iColorIntensity * f_alpha), 100 };
		CRgbaquad m_rgbValue;
		CColor::HSBToRGB(m_value, m_rgbValue);
		m_rgbValue.SetAlpha(f_alpha * 255.0f);
		listColor.push_back(m_rgbValue);

	}


	wxImage image = GetCircle(iTaille); //CLibResource::CreatePictureFromSVG("IDB_CIRCLE", iTaille, iTaille);

	uint8_t * alpha = nullptr;
	if (image.HasAlpha())
		alpha = image.GetAlpha();

	uint8_t * data = image.GetData();

	int width = image.GetWidth();
	int height = image.GetHeight();


	tbb::parallel_for(0, height, 1, [=](int y) {
		for (auto x = 0; x < width; x++)
		{
			int i = y * width + x;
			if (data[i * 3] == 0 && data[i * 3 + 1] == 0 && data[i * 3 + 2] == 0)
			{
				const float fValue = sqrt((double)((x - rayon) * (x - rayon) + (y - rayon) * (y - rayon)));
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
		});

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
		HSB m_value = { iNb * j, 50, 100 };
		CRgbaquad m_rgbValue;
		CColor::HSBToRGB(m_value, m_rgbValue);

		listColor.push_back(m_rgbValue);

	}


	wxImage image = GetCircle(iTaille); //CLibResource::CreatePictureFromSVG("IDB_CIRCLE", iTaille, iTaille);

	uint8_t * alpha = nullptr;
	if (image.HasAlpha())
		alpha = image.GetAlpha();

	uint8_t * data = image.GetData();

	//int size = image.GetHeight() * image.GetWidth();

	int width = image.GetWidth();
	int height = image.GetHeight();


	tbb::parallel_for(0, height, 1, [=](int y) {
		for (auto x = 0; x < width; x++)
		{
			int i1 = y * width + x;
			if (data[i1 * 3] == 0 && data[i1 * 3 + 1] == 0 && data[i1 * 3 + 2] == 0)
			{
				const float fValue = sqrt((double)((x - rayon) * (x - rayon) + (y - rayon) * (y - rayon)));
				int distance = (int)fValue;
				alpha[i1] = 0;

				if (distance <= rayon && distance >= (rayon - iWidth))
				{
					int position = iWidth - (rayon - distance);
					data[i1 * 3] = listColor[position].GetRed();
					data[i1 * 3 + 1] = listColor[position].GetGreen();
					data[i1 * 3 + 2] = listColor[position].GetBlue();
					alpha[i1] = 255 - (fAlpha2 * 255.0f);
				}
				else
				{
					int position = iWidth - (rayon - distance);
					if (position > 0)
					{
						data[i1 * 3] = listColor[iWidth].GetRed();
						data[i1 * 3 + 1] = listColor[iWidth].GetGreen();
						data[i1 * 3 + 2] = listColor[iWidth].GetBlue();
					}
					else
					{
						data[i1 * 3] = listColor[0].GetRed();
						data[i1 * 3 + 1] = listColor[0].GetGreen();
						data[i1 * 3 + 2] = listColor[0].GetBlue();
					}
				}
			}
		}
		});

	return image;
}


wxImage CCircle::Halo(const int &iColor, const int &iColorIntensity, const int &iTaille, const int &iWidth, const float &fAlpha2, const int &iCentre)
{
	int rayon = iTaille / 2;
	vector<CRgbaquad> listColorCenter;
	vector<CRgbaquad> listColorOut;
	/*
	cv::Mat out = cv::Mat(iTaille, iTaille, CV_8UC3);
	cv::circle(out, cv::Point(rayon, rayon), rayon, cv::Scalar(0, 0, 0), cv::FILLED);

	long imsize = out.rows * out.cols * out.channels();
	wxImage image(out.cols, out.rows, (unsigned char*)malloc(imsize), false);
	unsigned char* s = out.data;
	unsigned char* d = image.GetData();
	memcpy(d, s, imsize);

	uint8_t* charAlpha = new uint8_t[iTaille* iTaille];
	image.SetAlpha(charAlpha);
	*/
	wxImage image = GetCircle(iTaille); //CLibResource::CreatePictureFromSVG("IDB_CIRCLE", iTaille, iTaille);

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
			double m_iIntensity = (double)i / m_dCent;
			double m = (asin(m_iIntensity) * 90) / pi;
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

    //for (auto y = 0; y < height; y++)
	//{
	tbb::parallel_for(0, height, 1, [=](int y) {
		for (auto x = 0; x < width; x++)
		{
			int i = y * width + x;
			if (data[i * 3] == 0 && data[i * 3 + 1] == 0 && data[i * 3 + 2] == 0)
			{
				float fValue = sqrt((double)((x - rayon) * (x - rayon) + (y - rayon) * (y - rayon)));
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
		});

	return image;
}