#include <header.h>
#include "InterpolationFloatBicubic.h"
#include "RegardsFloatBitmap.h"

CInterpolationFloat::CInterpolationFloat(const double& dWidth)
{
	wX = nullptr;
	wY = nullptr;
	m_dWidth = dWidth;
}


CInterpolationFloat::~CInterpolationFloat()
{
	if (wX != nullptr)
		delete[] wX;

	if (wY != nullptr)
		delete[] wY;
}

void CInterpolationFloat::CalculWeight(const int32_t& width, const int32_t& height, const float& ratioY,
                                       const float& ratioX, const float& posTop, const float& posLeft)
{
	wX = new weightX[width];
	wY = new weightX[height];

	tbb::parallel_for(0, height, 1, [=](int y)
		{
				float posY = static_cast<float>(y) * ratioY + posTop;
				int valueB = static_cast<int>(posY);
				float realB = posY - valueB;
				wY[y].tabF[0] = Filter(-(-1.0f - realB));
				wY[y].tabF[1] = Filter(-(0.0f - realB));
				wY[y].tabF[2] = Filter(-(1.0f - realB));
				wY[y].tabF[3] = Filter(-(2.0f - realB));
		});
	
	tbb::parallel_for(0, width, 1, [=](int x)
		{
				float posX = static_cast<float>(x) * ratioX + posLeft;
				int valueA = static_cast<int>(posX);
				float realA = posX - valueA;
				wX[x].tabF[0] = Filter((-1.0f - realA));
				wX[x].tabF[1] = Filter((0.0f - realA));
				wX[x].tabF[2] = Filter((1.0f - realA));
				wX[x].tabF[3] = Filter((2.0f - realA));
		});
}

void CInterpolationFloat::Execute(CRegardsFloatBitmap* In, CRegardsFloatBitmap* & Out)
{
	int width = Out->GetWidth();
	int height = Out->GetHeight();

	int widthIn = In->GetWidth();
	int heightIn = In->GetHeight();

	if (widthIn > 0 && heightIn > 0 && width > 0 && height > 0)
	{
		float ratioX = static_cast<float>(widthIn) / static_cast<float>(width);
		float ratioY = static_cast<float>(heightIn) / static_cast<float>(height);

		float* data = Out->GetData();

		CalculWeight(width, height, ratioY, ratioX, 0.0f, 0.0f);

		tbb::parallel_for(0, height, 1, [=](int y)
			{
				for (auto x = 0; x < width; x++)
				{
					float posY = static_cast<float>(y) * ratioY;
					float posX = static_cast<float>(x) * ratioX;
					float color[4];
					Bicubic(color, In, posX, posY, wY[y].tabF, wX[x].tabF);
					int i = (x << 2) + (y * (width << 2)); // int i = Out->GetPosition(x, y);
					memcpy(&data[i], &color, sizeof(weightX));
					//Out->SetColorValue(x, y, color);
				}
			});
	}
}

void CInterpolationFloat::Bicubic(float* data, CRegardsFloatBitmap* In, const float& x, const float& y, float* tabF1,
                                  float* tabF)
{
	float nDenom = 0.0;
	int valueA = static_cast<int>(x);
	int valueB = static_cast<int>(y);

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

	for (auto n = debutN; n <= finN; n++)
	{
		for (auto m = debutM; m <= finM; m++)
		{
			float* color = In->GetColorValue(posX + m, posY + n);
			float f = tabF1[n + 1] * tabF[m + 1];
			nDenom += f;
			r += color[0] * f;
			g += color[1] * f;
			b += color[2] * f;
			a += color[3] * f;
		}
	}

	data[0] = r / nDenom;
	data[1] = g / nDenom;
	data[2] = b / nDenom;
	data[3] = a / nDenom;
}

double CInterpolationFloatBicubic::Filter(const double& f)
{
	return (f < -2.0 || f > 2.0)
		       ? 0.0
		       : (
			       (f < -1.0)
				       ? (2.0 + f) * (2.0 + f) * (2.0 + f) / 6.0
				       : (
					       (f < 0.0)
						       ? (4.0 + f * f * (-6.0 - 3.0 * f)) / 6.0
						       : (
							       (f < 1.0)
								       ? (4.0 + f * f * (-6.0 + 3.0 * f)) / 6.0
								       : (2.0 - f) * (2.0 - f) * (2.0 - f) / 6.0)));
}
