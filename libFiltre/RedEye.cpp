#include <header.h>
#include "RedEye.h"
#include "InterpolationBicubic.h"
#include <LibResource.h>
#include "Filtre.h"
#include <RegardsBitmap.h>
#include <DeepLearning.h>
#include <PictureData.h>
#include <FileUtility.h>
using namespace Regards::DeepLearning;
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


wxPoint rotate_point(double cx, double cy, double angle, wxPoint p)
{
	double s = sin(angle);
	double c = cos(angle);
	// translate point back to origin:
	p.x -= cx;
	p.y -= cy;
	// rotate point
	//double Xnew = p.x * c - p.y * s;
	//double Ynew = p.x * s + p.y * c;
	float Xnew = p.x * c + p.y * s;
	float Ynew = -p.x * s + p.y * c;
	// translate point back:
	p.x = Xnew + cx;
	p.y = Ynew + cy;
	return p;
}

wxRect CRedEye::ConvertPositionWithAngle(CRegardsBitmap * pBitmap, const wxRect & rSelectionBox, int angle)
{
	wxRect outputRect;
	wxPoint in;
	in.x = rSelectionBox.x;
	in.y = rSelectionBox.y;
	wxPoint out = rotate_point(pBitmap->GetBitmapWidth() / 2, pBitmap->GetBitmapHeight() / 2, angle, in);
	outputRect.x = out.x;
	outputRect.y = out.y;
	if (angle == 90 || angle == 270)
	{
		outputRect.width = rSelectionBox.height;
		outputRect.height = rSelectionBox.width;
	}
	else
	{
		outputRect.width = rSelectionBox.width;
		outputRect.height = rSelectionBox.height;
	}
	return outputRect;
}

void CRedEye::RemoveRedEye(CRegardsBitmap * pBitmap, const wxRect & rSelectionBox, int angle)
{
	int xmin, xmax, ymin, ymax;
	//int local_angle = (360 - angle);
	wxRect out = ConvertPositionWithAngle(pBitmap, rSelectionBox, angle);

	xmin = out.x;
	xmax = out.x + out.width;
	ymax = out.y + out.height;
	ymin = out.y;

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
			//color.SetRed((uint8_t)(a*min(color.GetGreen(), color.GetBlue()) + (1.0f - a)*color.GetRed()));
			color.SetColor(0, 255, 0, 0);
			pBitmap->SetColorValue(x, y, color);
		}
	}
}

bool CRedEye::RemoveRedEye(CRegardsBitmap * pBitmap)
{

	DeepLearning::CDeepLearning::DetectEyes(pBitmap);

	/*
	wxString filename = pBitmap->GetFilename();
	CLibPicture libPicture;
	bool pictureOk;
	CPictureData * pictureData = libPicture.LoadPictureData(filename, pictureOk);
	if (pictureData != nullptr)
	{
		if (pictureOk)
		{
			DeepLearning::CDeepLearning::DetectEyes(pictureData, angle);

			switch (angle)
			{
			case 90:
				pBitmap->RotateRawExif(6);
				break;
			case 180:
				pBitmap->RotateRawExif(3);
				break;
			case 270:
				pBitmap->RotateRawExif(8);
				break;
			}

			for (wxRect rect : listOfEye)
			{
				RemoveRedEye(pBitmap, rect, angle);
			}

			switch (angle)
			{
			case 90:
				pBitmap->RotateRawExif(8);
				break;
			case 180:
				pBitmap->RotateRawExif(3);
				break;
			case 270:
				pBitmap->RotateRawExif(6);
				break;
			}
		}
		delete pictureData;
	}
	*/
	return true;
}
