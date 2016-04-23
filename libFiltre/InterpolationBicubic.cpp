#include "InterpolationBicubic.h"
#include "RegardsBitmap.h"


CInterpolationBicubic::CInterpolationBicubic()
{
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
	for (int y = 0; y < height; y++)
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
	for (int x = 0; x < width; x++)
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

void CInterpolationBicubic::Execute(CRegardsBitmap * In, CRegardsBitmap * & Out, const wxRect &rectToShow)
{
	int width = Out->GetBitmapWidth();
	int height = Out->GetBitmapHeight();

	int widthIn = In->GetBitmapWidth();
	int heightIn = In->GetBitmapHeight();

	float ratioX = float(widthIn) / float(rectToShow.width);
	float ratioY = float(heightIn) / float(rectToShow.height);

	float posTop = float(rectToShow.y) * ratioY;
	float posLeft = float(rectToShow.x) * ratioY;

	CalculWeight(width, height, ratioY, ratioX, posTop, posLeft);

#pragma omp parallel for
	for (int y = 0; y < height; y++)
	{
		float posY = float(y) * ratioY + posTop;
		//int tailleYOut = y * width;

		for (int x = 0; x < width; x++)
		{
			float posX = float(x) * ratioX + posLeft;
			CRgbaquad * color = Out->GetPtColorValue(x, y);
			Bicubic(color, In, posX, posY, wY[y].tabF, wX[x].tabF);
		}
	}

	delete[] wX;
	delete[] wY;
}

void CInterpolationBicubic::Execute(CRegardsBitmap * In, CRegardsBitmap * & Out, const int &flipH, const int &flipV, const int &angle)
{
	int width = Out->GetBitmapWidth();
	int height = Out->GetBitmapHeight();

	int widthIn = In->GetBitmapWidth();
	int heightIn = In->GetBitmapHeight();

	float ratioX = (float)widthIn / (float)width;
	float ratioY = (float)heightIn / (float)height;

	if (angle == 90 || angle == 270)
	{
		ratioX = (float)heightIn / (float)width;
		ratioY = (float)widthIn / (float)height;
	}

	CalculWeight(width, height, ratioY, ratioX, 0.0f, 0.0f);
	
#pragma omp parallel for
	for (int y = 0; y < height; y++)
	{
		float posY = (float)y * ratioY;
		//int tailleYOut = y * width;

		for (int x = 0; x < width; x++)
		{
			float posX = (float)x * ratioX;

			if (angle == 90)
			{
				int srcx = posY;
				int srcy = height - posX - 1;

				posX = srcx;
				posY = srcy;
			}
			else if (angle == 180)
			{
				posX = width - posX - 1;
				posY = height - posY - 1;
			}
			else if (angle == 270)
			{
				int srcx = width - posY - 1;
				int srcy = posX;

				posX = srcx;
				posY = srcy;
			}

			if (flipH == 1)
			{
				posX = width - posX - 1;
			}

			if (flipV == 0)
			{
				posY = height - posY - 1;
			}

			CRgbaquad * color = Out->GetPtColorValue(x, y);
			Bicubic(color, In, posX, posY, wY[y].tabF, wX[x].tabF);
		}
	}
	delete[] wX;
	delete[] wY;
}

void CInterpolationBicubic::Execute(CRegardsBitmap * In, CRegardsBitmap * & Out)
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
		
		for (int x = 0; x < width; x++)
		{
			float posX = (float)x * ratioX;
			CRgbaquad * color = Out->GetPtColorValue(x, y);
			Bicubic(color, In, posX, posY, wY[y].tabF, wX[x].tabF);
		}
	}

}


void CInterpolationBicubic::Bicubic(CRgbaquad * & data, CRegardsBitmap * In, const float &x, const float &y, float * tabF1, float * tabF)
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

	for (int n = debutN; n <= finN; n++)
	{
		for (int m = debutM; m <= finM; m++)
		{
			CRgbaquad * color = In->GetPtColorValue(posX + m, posY + n);
			float f = tabF1[n + 1] * tabF[m + 1];
			nDenom += f;
			r += color->GetFRed() * f;
			g += color->GetFGreen() * f;
			b += color->GetFBlue() * f;
			a += color->GetFAlpha() * f;
		}
	}


	data->SetColor(uint8_t(r / nDenom), uint8_t(g / nDenom), uint8_t(b / nDenom), uint8_t(a / nDenom));
}

float CInterpolationBicubic::Filter(const float &f)
{
	
	return (f < -2.0 || f > 2.0) ? 0.0 : (
		(f < -1.0) ? (2.0 + f)*(2.0 + f)*(2.0 + f) / 6.0 : (
		(f < 0.0) ? (4.0 + f*f*(-6.0 - 3.0*f)) / 6.0 : (
		(f < 1.0) ? (4.0 + f*f*(-6.0 + 3.0*f)) / 6.0 : (2.0 - f)*(2.0 - f)*(2.0 - f) / 6.0)));
	
}