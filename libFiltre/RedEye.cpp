#include <header.h>
#include "RedEye.h"
#include "InterpolationBicubic.h"
#include <LibResource.h>
#include "Filtre.h"
#include <RegardsBitmap.h>
#include <DeepLearning.h>
#include <libPicture.h>
#include <PictureData.h>
using namespace std;
using namespace Regards::FiltreEffet;

CRgbaquad CRedEye::CalculColorOut(CRgbaquad colorIn)
{
	int intensity = colorIn.GetGreyValue();
	int inverseintensity = 255 - intensity;
	float pourcentage = intensity / inverseintensity;
	colorIn.Mul(pourcentage);
	return colorIn;
}

void CRedEye::RemoveRedEye(CRegardsBitmap * pBitmap, const wxRect & rSelectionBox)
{
	int xmin, xmax, ymin, ymax;
	xmin = rSelectionBox.x;
	xmax = rSelectionBox.x + rSelectionBox.width;
	ymax = pBitmap->GetBitmapHeight() - (rSelectionBox.y + rSelectionBox.height);
	ymin = pBitmap->GetBitmapHeight() - rSelectionBox.y;

	if (xmin > xmax)
	{
		int old = xmax;
		xmax = xmin;
		xmin = old;
	}

	if (ymin > xmax)
	{
		int old = ymax;
		ymax = ymin;
		ymin = old;
	}


	for (int32_t y = ymin; y < ymax; y++) {
		for (int32_t x = xmin; x < xmax; x++)
		{
			float a = 1.0f - 5.0f*((float)((x - 0.5f*(xmax + xmin))*(x - 0.5f*(xmax + xmin)) + (y - 0.5f*(ymax + ymin))*(y - 0.5f*(ymax + ymin)))) / ((float)((xmax - xmin)*(ymax - ymin)));
			if (a < 0)
				a = 0;

			//CRgbaquad * color = pBitmap->GetPtColorValue(x,y);
			//color->SetRed((uint8_t)(a*min(color->GetGreen(),color->GetBlue())+(1.0f-a)*color->GetRed()));
			CRgbaquad color = pBitmap->GetColorValue(x, y);
			color.SetRed((uint8_t)(a*min(color.GetGreen(), color.GetBlue()) + (1.0f - a)*color.GetRed()));
			//color.SetColor(0, 255, 0, 0);
			pBitmap->SetColorValue(x, y, color);
		}
	}
}

bool CRedEye::RemoveRedEye(CRegardsBitmap * pBitmap)
{
	wxString filename = pBitmap->GetFilename();
	CLibPicture libPicture;
	bool pictureOk;
	CPictureData * pictureData = libPicture.LoadPictureData(filename, pictureOk);
	if (pictureData != nullptr)
	{
		if (pictureOk)
		{
			vector<wxRect> listOfEye = DeepLearning::CDeepLearning::DetectEyes(pictureData);
			for (wxRect rect : listOfEye)
			{
				RemoveRedEye(pBitmap, rect);
			}
		}
		delete pictureData;
	}

	return true;
}