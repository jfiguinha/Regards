#include <header.h>
#include "InterpolationBilinear.h"
#include "RegardsBitmap.h"
extern float clamp(float val, float minval, float maxval);

CInterpolationBilinear::CInterpolationBilinear()
{
	wX = nullptr;
	wY = nullptr;
}


CInterpolationBilinear::~CInterpolationBilinear()
{
	if (wX != nullptr)
		delete[] wX;

	if (wY != nullptr)
		delete[] wY;
}


void CInterpolationBilinear::CalculWeight(const int32_t& width, const int32_t& height, const float& ratioY,
                                          const float& ratioX, const float& posTop, const float& posLeft)
{
	wX = new weightX[width];
	wY = new weightX[height];

	tbb::parallel_for(0, height, 1, [=](int y)
		{
				float posY = static_cast<float>(y) * ratioY + posTop;
				int32_t valueB = static_cast<int32_t>(posY);
				float realB = posY - valueB;
				wY[y].tabF[0] = Filter(-(-1.0f - realB));
				wY[y].tabF[1] = Filter(-(0.0f - realB));
		});

	tbb::parallel_for(0, width, 1, [=](int x)
		{
				float posX = static_cast<float>(x) * ratioX + posLeft;
				int32_t valueA = static_cast<int32_t>(posX);
				float realA = posX - valueA;
				wX[x].tabF[0] = Filter((-1.0f - realA));
				wX[x].tabF[1] = Filter((0.0f - realA));
		});
}

void CInterpolationBilinear::Execute(CRegardsBitmap* In, CRegardsBitmap* & Out, const wxRect& rectToShow)
{
	int32_t width = Out->GetBitmapWidth();
	int32_t height = Out->GetBitmapHeight();

	int32_t widthIn = In->GetBitmapWidth();
	int32_t heightIn = In->GetBitmapHeight();

	float ratioX = static_cast<float>(widthIn) / static_cast<float>(rectToShow.width);
	float ratioY = static_cast<float>(heightIn) / static_cast<float>(rectToShow.height);

	auto wX = new weightX[width];
	auto wY = new weightX[height];

	float posTop = static_cast<float>(rectToShow.y) * ratioY;
	float posLeft = static_cast<float>(rectToShow.x) * ratioY;

	CalculWeight(width, height, ratioY, ratioX, posTop, posLeft);

	tbb::parallel_for(0, height, 1, [=](int y)
		{
			float posY = static_cast<float>(y) * ratioY + posTop;
			//int64_t tailleYOut = y * width;

			for (auto x = 0; x < width; x++)
			{
				float posX = static_cast<float>(x) * ratioX + posLeft;
				CRgbaquad color;
				Bilinear(color, Out, posX, posY, wY[y].tabF, wX[x].tabF);
				Out->SetColorValue(x, y, color);
			}
		});
}


void CInterpolationBilinear::Execute(CRegardsBitmap* In, CRegardsBitmap* & Out)
{
	int width = Out->GetBitmapWidth();
	int height = Out->GetBitmapHeight();

	int widthIn = In->GetBitmapWidth();
	int heightIn = In->GetBitmapHeight();

	float ratioX = static_cast<float>(widthIn) / static_cast<float>(width);
	float ratioY = static_cast<float>(heightIn) / static_cast<float>(height);

	CalculWeight(width, height, ratioY, ratioX, 0.0f, 0.0f);


	tbb::parallel_for(0, height, 1, [=](int y)
		{
			float posY = static_cast<float>(y) * ratioY;
			//int tailleYOut = y * width;

			for (auto x = 0; x < width; x++)
			{
				float posX = static_cast<float>(x) * ratioX;
				CRgbaquad color;
				Bilinear(color, In, posX, posY, wY[y].tabF, wX[x].tabF);
				Out->SetColorValue(x, y, color);
			}
		});
}


void CInterpolationBilinear::Bilinear(CRgbaquad& data, CRegardsBitmap* In, const float& x, const float& y, float* tabF1,
                                      float* tabF)
{
	float nDenom = 0.0;
	int valueA = static_cast<int>(x);
	int valueB = static_cast<int>(y);

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

	for (auto n = debutN; n <= finN; n++)
	{
		for (auto m = debutM; m <= finM; m++)
		{
			CRgbaquad colorIn = In->GetColorValue(posX + m, posY + n);
			float f = tabF1[n + 1] * tabF[m + 1];
			nDenom += f;
			r += colorIn.GetFRed() * f;
			g += colorIn.GetFGreen() * f;
			b += colorIn.GetFBlue() * f;
			a += colorIn.GetFAlpha() * f;
		}
	}

	data.SetColor(clamp(static_cast<uint8_t>(r / nDenom), 0, 255), clamp(static_cast<uint8_t>(g / nDenom), 0, 255), clamp(static_cast<uint8_t>(b / nDenom), 0, 255),
		clamp(static_cast<uint8_t>(a / nDenom), 0, 255));
}


float CInterpolationBilinear::Filter(const float& f)
{
	double m_dVal = fabs(f);
	return (m_dVal < 1.0f ? 1.0f - m_dVal : 0.0);
}
