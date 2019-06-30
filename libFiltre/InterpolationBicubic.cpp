#include <header.h>
#include "InterpolationBicubic.h"
#include "RegardsBitmap.h"
#include "Interpolation.h"
#include <omp.h>

CInterpolationBicubic::CInterpolationBicubic(const double & dWidth)
{
	wX = nullptr;
	wY = nullptr;
	m_dWidth = dWidth;
}


CInterpolationBicubic::~CInterpolationBicubic()
{
	if (wX != nullptr)
		delete[] wX;

	if (wY != nullptr)
		delete[] wY;
}

void CInterpolationBicubic::CalculWeight(const int32_t &width, const int32_t &height, const float &ratioY, const float &ratioX, const float &posTop, const float &posLeft)
{
	wX = new weightX[width];
	wY = new weightX[height];

#pragma omp parallel for
	for (auto y = 0; y < height; y++)
	{
		float posY = (float)y * ratioY + posTop;
		int valueB = (int)posY;
		float realB = posY - valueB;
		wY[y].tabF[0] = Filter(-(-1.0f - realB));
		wY[y].tabF[1] = Filter(-(0.0f - realB));
		wY[y].tabF[2] = Filter(-(1.0f - realB));
		wY[y].tabF[3] = Filter(-(2.0f - realB));
	}
#pragma omp parallel for
	for (auto x = 0; x < width; x++)
	{
		float posX = (float)x * ratioX + posLeft;
		int valueA = (int)posX;
		float realA = posX - valueA;
		wX[x].tabF[0] = Filter((-1.0f - realA));
		wX[x].tabF[1] = Filter((0.0f - realA));
		wX[x].tabF[2] = Filter((1.0f - realA));
		wX[x].tabF[3] = Filter((2.0f - realA));
	}
}
void CInterpolationBicubic::Execute(CRegardsBitmap * In, CRegardsBitmap * & Out, const int &flipH, const int &flipV, const int &angle)
{
	int width = Out->GetBitmapWidth();
	int height = Out->GetBitmapHeight();

	int widthIn = In->GetBitmapWidth();
	int heightIn = In->GetBitmapHeight();

	if(widthIn > 0 && heightIn > 0 && width > 0 && height > 0)
	{

		float ratioX = (float)widthIn / (float)width;
		float ratioY = (float)heightIn / (float)height;

		if (angle == 90 || angle == 270)
		{
			ratioX = (float)heightIn / (float)width;
			ratioY = (float)widthIn / (float)height;
		}

		uint8_t * data = Out->GetPtBitmap();

		CalculWeight(width, height, ratioY, ratioX, 0.0f, 0.0f);
	
	#pragma omp parallel for
		for (auto y = 0; y < height; y++)
		{
			for (auto x = 0; x < width; x++)
			{
				float posY = 0;
				float posX = 0;
				CInterpolation::CalculPosition(x, y, widthIn, heightIn, width, height, flipH, flipV, angle, posX, posY);
				CRgbaquad color;
				Bicubic(color, In, posX, posY, wY[y].tabF, wX[x].tabF);
				Out->SetColorValue(x, y, color);
			}
		}
		delete[] wX;
		delete[] wY;
		wX = nullptr;
		wY = nullptr;
	}
}

void CInterpolationBicubic::Execute(wxImage * In, CRegardsBitmap * & Out)
{
	int width = Out->GetBitmapWidth();
	int height = Out->GetBitmapHeight();

	int widthIn = In->GetWidth();
	int heightIn = In->GetHeight();

	if(widthIn > 0 && heightIn > 0 && width > 0 && height > 0)
	{

		float ratioX = (float)widthIn / (float)width;
		float ratioY = (float)heightIn / (float)height;

		uint8_t * data = Out->GetPtBitmap();

		CalculWeight(width, height, ratioY, ratioX, 0.0f, 0.0f);

	#pragma omp parallel for
		for (auto y = 0; y < height; y++)
		{
	#pragma omp parallel for
			for (auto x = 0; x < width; x++)
			{
				float posY = (float)y * ratioY;
				float posX = (float)x * ratioX;
				CRgbaquad color;
				Bicubic(color, In, posX, posY, wY[y].tabF, wX[x].tabF);
				int i =  (x << 2) + (y * (width << 2));// int i = Out->GetPosition(x, y);
				memcpy(data + i, &color, sizeof(CRgbaquad));
				//Out->SetColorValue(x, y, color);
			}
		}
	}
}

void CInterpolationBicubic::Execute(CRegardsBitmap * In, CRegardsBitmap * & Out)
{
	int width = Out->GetBitmapWidth();
	int height = Out->GetBitmapHeight();

	int widthIn = In->GetBitmapWidth();
	int heightIn = In->GetBitmapHeight();

	if(widthIn > 0 && heightIn > 0 && width > 0 && height > 0)
	{

		float ratioX = (float)widthIn / (float)width;
		float ratioY = (float)heightIn / (float)height;

		uint8_t * data = Out->GetPtBitmap();

		CalculWeight(width, height, ratioY, ratioX, 0.0f, 0.0f);

	#pragma omp parallel for
		for (auto y = 0; y < height; y++)
		{
	#pragma omp parallel for
			for (auto x = 0; x < width; x++)
			{
				float posY = (float)y * ratioY;
				float posX = (float)x * ratioX;
				CRgbaquad color;
				Bicubic(color, In, posX, posY, wY[y].tabF, wX[x].tabF);
				int i =  (x << 2) + (y * (width << 2));// int i = Out->GetPosition(x, y);
				memcpy(data + i, &color, sizeof(CRgbaquad));
				//Out->SetColorValue(x, y, color);
			}
		}
	}

}

void CInterpolationBicubic::Bicubic(CRgbaquad & data, wxImage * In, const float &x, const float &y, float * tabF1, float * tabF)
{

	float nDenom = 0.0;
	int valueA = (int)x;
	int valueB = (int)y;

	float r = 0.0, g = 0.0, b = 0.0, a = 0.0;

	int debutN = -1;
	int finN = 2;
	//Calcul démarrage du y;
	if (valueB == 0)
	{
		debutN = 0;
	}

	if (valueB == In->GetHeight() - 2)
	{
		finN = 1;
	}

	if (valueB == In->GetHeight() - 1)
	{
		finN = 0;
	}

	int debutM = -1;
	int finM = 2;
	//Calcul démarrage du y;
	if (valueA == 0)
	{
		debutM = 0;
	}

	if (valueA == In->GetWidth() - 2)
	{
		finM = 1;
	}

	if (valueA == In->GetWidth() - 1)
	{
		finM = 0;
	}

	int posX = valueA + debutM;
	if (valueA == 1)
		posX = valueA;

	int posY = valueB + debutN;
	if (valueB == 1)
		posY = valueB;

	uint8_t * datawxImage = In->GetData();

	for (auto n = debutN; n <= finN; n++)
	{
		for (auto m = debutM; m <= finM; m++)
		{
			int position = (posX + m) * 3 + (posY + n) * In->GetWidth() * 3;
			float f = tabF1[n + 1] * tabF[m + 1];
			nDenom += f;
			r += datawxImage[position+2] * f;
			g += datawxImage[position+1] * f;
			b += datawxImage[position] * f;
			a += 0;
		}
	}


	data.SetColor(uint8_t(r / nDenom), uint8_t(g / nDenom), uint8_t(b / nDenom), uint8_t(a / nDenom));
}

void CInterpolationBicubic::Bicubic(CRgbaquad & data, CRegardsBitmap * In, const float &x, const float &y, float * tabF1, float * tabF)
{

	float nDenom = 0.0;
	int valueA = (int)x;
	int valueB = (int)y;

	float r = 0.0, g = 0.0, b = 0.0, a = 0.0;

	int debutN = -1;
	int finN = 2;
	//Calcul démarrage du y;
	if (valueB == 0)
	{
		debutN = 0;
	}

	if (valueB == In->GetBitmapHeight() - 2)
	{
		finN = 1;
	}

	if (valueB == In->GetBitmapHeight() - 1)
	{
		finN = 0;
	}

	int debutM = -1;
	int finM = 2;
	//Calcul démarrage du y;
	if (valueA == 0)
	{
		debutM = 0;
	}

	if (valueA == In->GetBitmapWidth() - 2)
	{
		finM = 1;
	}

	if (valueA == In->GetBitmapWidth() - 1)
	{
		finM = 0;
	}

	int posX = valueA + debutM;
	if (valueA == 1)
		posX = valueA;

	int posY = valueB + debutN;
	if (valueB == 1)
		posY = valueB;

	for (auto n = debutN; n <= finN; n++)
	{
		for (auto m = debutM; m <= finM; m++)
		{
			CRgbaquad color = In->GetColorValue(posX + m, posY + n);
			float f = tabF1[n + 1] * tabF[m + 1];
			nDenom += f;
			r += color.GetFRed() * f;
			g += color.GetFGreen() * f;
			b += color.GetFBlue() * f;
			a += color.GetFAlpha() * f;
		}
	}


	data.SetColor(uint8_t(r / nDenom), uint8_t(g / nDenom), uint8_t(b / nDenom), uint8_t(a / nDenom));
}

double CInterpolationBicubic::Filter(const double &f)
{
	
	return (f < -2.0 || f > 2.0) ? 0.0 : (
		(f < -1.0) ? (2.0 + f)*(2.0 + f)*(2.0 + f) / 6.0 : (
		(f < 0.0) ? (4.0 + f*f*(-6.0 - 3.0*f)) / 6.0 : (
		(f < 1.0) ? (4.0 + f*f*(-6.0 + 3.0*f)) / 6.0 : (2.0 - f)*(2.0 - f)*(2.0 - f) / 6.0)));
	
}