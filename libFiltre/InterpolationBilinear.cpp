#include "InterpolationBilinear.h"
#include "RegardsBitmap.h"


CInterpolationBilinear::CInterpolationBilinear()
{
}


CInterpolationBilinear::~CInterpolationBilinear()
{
	if (wX != nullptr)
		delete[] wX;

	if (wY != nullptr)
		delete[] wY;
}



void CInterpolationBilinear::CalculWeight(const int32_t &width, const int32_t &height, const float &ratioY, const float &ratioX, const float &posTop, const float &posLeft)
{
	wX = new weightX[width];
	wY = new weightX[height];

#pragma omp parallel for
	for (int y = 0; y < height; y++)
	{
		float posY = (float)y * ratioY + posTop;
		int32_t valueB = (int32_t)posY;
		float realB = posY - valueB;
		wY[y].tabF[0] = Filter(-(-1.0f - realB));
		wY[y].tabF[1] = Filter(-(0.0f - realB));
	}
#pragma omp parallel for
	for (int x = 0; x < width; x++)
	{
		float posX = (float)x * ratioX + posLeft;
		int32_t valueA = (int32_t)posX;
		float realA = posX - valueA;
		wX[x].tabF[0] = Filter((-1.0f - realA));
		wX[x].tabF[1] = Filter((0.0f - realA));
	}
}

void CInterpolationBilinear::Execute(CRegardsBitmap * In, CRegardsBitmap * & Out, const wxRect &rectToShow)
{
	int32_t width = Out->GetBitmapWidth();
	int32_t height = Out->GetBitmapHeight();

	int32_t widthIn = In->GetBitmapWidth();
	int32_t heightIn = In->GetBitmapHeight();

	float ratioX = float(widthIn) / float(rectToShow.width);
	float ratioY = float(heightIn) / float(rectToShow.height);

	weightX * wX = new weightX[width];
	weightX * wY = new weightX[height];

	float posTop = float(rectToShow.y) * ratioY;
	float posLeft = float(rectToShow.x) * ratioY;

	CalculWeight(width, height, ratioY, ratioX, posTop, posLeft);


#pragma omp parallel for
	for (int32_t y = 0; y < height; y++)
	{
		//float posY = (float)y * ratioY;
		float posY = float(y) * ratioY + posTop;
		//int64_t tailleYOut = y * width;

		for (int x = 0; x < width; x++)
		{
			float posX = float(x) * ratioX + posLeft;
			CRgbaquad * color = Out->GetPtColorValue(x, y);
			Bilinear(color, Out, posX, posY, wY[y].tabF, wX[x].tabF);
		}
	}
}


void CInterpolationBilinear::Execute(CRegardsBitmap * In, CRegardsBitmap * & Out)
{
	int width = Out->GetBitmapWidth();
	int height = Out->GetBitmapHeight();

	int widthIn = In->GetBitmapWidth();
	int heightIn = In->GetBitmapHeight();

	float ratioX = (float)widthIn / (float)width;
	float ratioY = (float)heightIn / (float)height;

	CalculWeight(width, height, ratioY, ratioX, 0.0f, 0.0f);

#pragma omp parallel for
	for (int y = 0; y < height; y++)
	{
		float posY = (float)y * ratioY;
		//int tailleYOut = y * width;

		for (int x = 0; x < width; x++)
		{
			float posX = (float)x * ratioX;
			CRgbaquad * color = Out->GetPtColorValue(x, y);
			Bilinear(color, In, posX, posY, wY[y].tabF, wX[x].tabF);

		}
	}
}



void CInterpolationBilinear::Bilinear(CRgbaquad * & data, CRegardsBitmap * In, const float &x, const float &y, float * tabF1, float * tabF)
{
	float nDenom = 0.0;
	int valueA = (int)x;
	int valueB = (int)y;

	float r = 0.0, g = 0.0, b = 0.0, a = 0.0;

	int debutN = -1;
	int finN = 0;
	//Calcul démarrage du y;
	if (valueB == 0)
	{
		debutN = 0;
	}

	int debutM = -1;
	int finM = 0;
	//Calcul démarrage du y;
	if (valueA == 0)
	{
		debutM = 0;
	}

	int posX = valueA + debutM;
	if (valueA == 1)
		posX = valueA;

	int posY = valueB + debutN;
	if (valueB == 1)
		posY = valueB;

	for (int n = debutN; n <= finN; n++)
	{
		for (int m = debutM; m <= finM; m++)
		{
			CRgbaquad * colorIn = In->GetPtColorValue(posX + m, posY + n);
			float f = tabF1[n + 1] * tabF[m + 1];
			nDenom += f;
			r += colorIn->GetFRed() * f;
			g += colorIn->GetFGreen() * f;
			b += colorIn->GetFBlue() * f;
			a += colorIn->GetFAlpha() * f;
		}
	}

	data->SetColor(uint8_t(r / nDenom), uint8_t(g / nDenom), uint8_t(b / nDenom), uint8_t(a / nDenom));
}


float CInterpolationBilinear::Filter(const float &f)
{
	double m_dVal = fabs(f);
	return (m_dVal < 1.0f ? 1.0f - m_dVal : 0.0);
}
