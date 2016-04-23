#include "Interpolation.h"
#include "RegardsBitmap.h"

CInterpolation::CInterpolation()
{
}


CInterpolation::~CInterpolation()
{
}


void CInterpolation::Execute(CRegardsBitmap * In, CRegardsBitmap * & Out, const wxRect &rectToShow)
{
	int32_t width = Out->GetBitmapWidth();
	int32_t height = Out->GetBitmapHeight();

	int32_t widthIn = In->GetBitmapWidth();
	int32_t heightIn = In->GetBitmapHeight();

	float ratioX = float(widthIn) / float(rectToShow.width);
	float ratioY = float(heightIn) / float(rectToShow.height);

	float posTop = float(rectToShow.y) * ratioY;
	float posLeft = float(rectToShow.x) * ratioY;


#pragma omp parallel for
	for (int y = 0; y < height; y++)
	{
#pragma omp parallel for
		for (int x = 0; x < width; x++)
		{
			float posY = float(y) * ratioY + posTop;
			float posX = float(x) * ratioX + posLeft;
			CRgbaquad * color = In->GetPtColorValue(posX, posY);
			Out->SetColorValue(x, y, *color);
		}
	}

}


void CInterpolation::Execute(CRegardsBitmap * In, CRegardsBitmap * & Out)
{
	int width = Out->GetBitmapWidth();
	int height = Out->GetBitmapHeight();

	int widthIn = In->GetBitmapWidth();
	int heightIn = In->GetBitmapHeight();

	float ratioX = (float)widthIn / (float)width;
	float ratioY = (float)heightIn / (float)height;

#pragma omp parallel for
	for (int y = 0; y < height; y++)
	{
#pragma omp parallel for
		for (int x = 0; x < width; x++)
		{
			float posY = float(y) * ratioY;
			float posX = float(x) * ratioX;
			CRgbaquad * color = In->GetPtColorValue(posX, posY);
			Out->SetColorValue(x, y, *color);
		}
	}

}


