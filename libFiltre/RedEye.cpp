#include "RedEye.h"
#include <algorithm>
#include "InterpolationBicubic.h"
#include <libResource.h>
#include "Filtre.h"
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

wxRect CRedEye::FindIris(wxPoint leftEye, int distanceMax, CRegardsBitmap * bitmap)
{
	//---------------------------------------------------------
	//Left Eye
	//---------------------------------------------------------
	vector<wxPoint> ptLeftEye;
	CRgbaquad colorRed = { 31, 43, 61, 0 };

	for (int x = leftEye.x; x < leftEye.x + distanceMax; x++)
	{
		int lasty = leftEye.y;
		for (int y = leftEye.y; y < leftEye.y + distanceMax; y++)
		{

			CRgbaquad color = bitmap->GetColorValue(x, y);
			if (color.GetBlue() != 0)
			{
				ptLeftEye.push_back({ x, y });
			}
			else
				break;

			lasty = y;
		}

		if (lasty == leftEye.y)
			break;
	}

	for (int x = leftEye.x; x > (leftEye.x - distanceMax); x--)
	{
		int lasty = leftEye.y;
		for (int y = leftEye.y; y < leftEye.y + distanceMax; y++)
		{
			CRgbaquad color = bitmap->GetColorValue(x, y);
			if (color.GetBlue() != 0)
			{
				ptLeftEye.push_back({ x, y });
			}
			else
				break;

			lasty = y;
		}
		if (lasty == leftEye.y)
			break;
	}

	for (int x = leftEye.x; x < leftEye.x + distanceMax; x++)
	{
		int lasty = leftEye.y;
		for (int y = leftEye.y; y > (leftEye.y - distanceMax); y--)
		{
			CRgbaquad color = bitmap->GetColorValue(x, y);
			if (color.GetBlue() != 0)
				ptLeftEye.push_back({ x, y });
			else
				break;

			lasty = y;
		}

		if (lasty == leftEye.y)
			break;
	}

	for (int x = leftEye.x; x > (leftEye.x - distanceMax); x--)
	{
		int lasty = leftEye.y;
		for (int y = leftEye.y; y > (leftEye.y - distanceMax); y--)
		{
			CRgbaquad color = bitmap->GetColorValue(x, y);
			if (color.GetBlue() != 0)
			{
				ptLeftEye.push_back({ x, y });
			}
			else
				break;

			lasty = y;
		}
		if (lasty == leftEye.y)
			break;
	}

	wxRect rcLeftEye;
	std::sort(ptLeftEye.begin(), ptLeftEye.end(), [](const wxPoint & a, const wxPoint & b)
	{
		return a.x > b.x;
	});

	rcLeftEye.width = ptLeftEye[0].x;

	std::sort(ptLeftEye.begin(), ptLeftEye.end(), [](const wxPoint & a, const wxPoint & b)
	{
		return a.x < b.x;
	});

	rcLeftEye.x = ptLeftEye[0].x;

	std::sort(ptLeftEye.begin(), ptLeftEye.end(), [](const wxPoint & a, const wxPoint & b)
	{
		return a.y < b.y;
	});

	rcLeftEye.y = ptLeftEye[0].y;

	std::sort(ptLeftEye.begin(), ptLeftEye.end(), [](const wxPoint & a, const wxPoint & b)
	{
		return a.y > b.y;
	});

	rcLeftEye.height = ptLeftEye[0].y;

	return rcLeftEye;
}


bool CRedEye::RemoveRedEye(CRegardsBitmap * pBitmap,const CRgbaquad & backcolor, const wxRect& rSelectionBox)
{
	int xmin, xmax, ymin, ymax;
	xmin = rSelectionBox.x;
	xmax = rSelectionBox.x + rSelectionBox.width;
	ymax = rSelectionBox.y + rSelectionBox.height;
	ymin = rSelectionBox.y;
    
    if(xmin > xmax)
    {
        int old = xmax;
        xmax = xmin;
        xmin = old;
    }
    
    if(ymin > xmax)
    {
        int old = ymax;
        ymax = ymin;
        ymin = old;
    }
    
    
    for(int32_t y=ymin; y<ymax; y++){
        for(int32_t x=xmin; x<xmax; x++)
        {
            float a = 1.0f-5.0f*((float)((x-0.5f*(xmax+xmin))*(x-0.5f*(xmax+xmin))+(y-0.5f*(ymax+ymin))*(y-0.5f*(ymax+ymin))))/((float)((xmax-xmin)*(ymax-ymin)));
            if (a<0)
                a=0;
            
            CRgbaquad * color = pBitmap->GetPtColorValue(x,y);
            color->SetRed((uint8_t)(a*min(color->GetGreen(),color->GetBlue())+(1.0f-a)*color->GetRed()));
            
        }
    }
    
    return true;
    
#ifdef OLD_CODE

    
	//Recherche des points lumineux
	CRegardsBitmap * bitmap = new CRegardsBitmap(rSelectionBox.width, rSelectionBox.height);
	//uint8_t * data = bitmap->GetPtBitmap();
	for (int y = ymin; y < ymax; y++)
	{
		for (int x = xmin; x < xmax; x++)
		{
			CRgbaquad color = pBitmap->GetColorValue(x, y);
            CRgbaquad * colorNew = bitmap->GetPtColorValue(x - xmin, y - ymin);
			colorNew->SetColor(color.GetGreyValue());
		}
	}


	vector<level> vectorListLevelColor;

	//Recherche par ligne le niveau Max de lumière
	for (int x = 0; x < bitmap->GetBitmapWidth(); x++)
	{
		int maxLux = 0;
		wxPoint pt;
		for (int y = 0; y < bitmap->GetBitmapHeight(); y++)
		{
			CRgbaquad color = bitmap->GetColorValue(x, y);
			if (color.GetBlue() > maxLux)
			{
				maxLux = color.GetBlue();
				pt.x = x;
				pt.y = y;

			}
		}

		level niveau;
		niveau.maxLux = maxLux;
		niveau.pt = pt;
		niveau.distanceLeft = x;
		niveau.distanceRight = bitmap->GetBitmapWidth() - x;
		vectorListLevelColor.push_back(niveau);
	}

	//Ordonnancement par ligne
	std::sort(vectorListLevelColor.begin(), vectorListLevelColor.end(), [](const level & a, const level & b)
	{
		return a.pt.y > b.pt.y;
	});

	//Elimination des doublons par ligne
	vector<level> vectorLevelByRow;
	vector<level> vectorLevelFinalLeft;
	vector<level> vectorLevelFinalRight;
	int y = vectorListLevelColor.at(0).pt.y;
	vectorLevelByRow.push_back(vectorListLevelColor.at(0));

	int minPosX = bitmap->GetBitmapWidth() / 4;
	int maxPosX = bitmap->GetBitmapWidth() - minPosX;

	int minPosY = bitmap->GetBitmapHeight() / 4;
	int maxPosY = bitmap->GetBitmapHeight() - minPosY;

	for (level niveau : vectorListLevelColor)
	{
		if (y == -1)
		{
			vectorLevelByRow.push_back(niveau);
			y = niveau.pt.y;
		}
		else if (y == niveau.pt.y)
		{
			vectorLevelByRow.push_back(niveau);
		}
		else
		{
			level maxLux = vectorLevelByRow.at(0);
			for (level niveau : vectorLevelByRow)
			{
				if (maxLux.maxLux < niveau.maxLux)
				{
					maxLux = niveau;
				}
			}
			if (maxLux.maxLux > 200 && maxLux.pt.x > minPosX &&  maxLux.pt.x < maxPosX && maxLux.pt.y > minPosY &&  maxLux.pt.y < maxPosY)
			{
				vectorLevelFinalLeft.push_back(maxLux);
			}
			vectorLevelByRow.clear();
			y = -1;
		}
	}

	if (vectorLevelFinalLeft.size() == 0 || vectorLevelFinalLeft.size() == 0)
		return false;

	//Fusion des points pour trouver des convergences
	std::sort(vectorLevelFinalLeft.begin(), vectorLevelFinalLeft.end(), [](const level & a, const level & b)
	{
		return a.distanceLeft < b.distanceLeft;
	});

	int distanceLeft = vectorLevelFinalLeft[0].distanceLeft + (vectorLevelFinalLeft[0].distanceLeft * 0.2);

	for (level niveau : vectorLevelFinalLeft)
	{
		if (niveau.distanceLeft  > distanceLeft)
			vectorLevelFinalRight.push_back(niveau);
	}

	std::sort(vectorLevelFinalRight.begin(), vectorLevelFinalRight.end(), [](const level & a, const level & b)
	{
		return a.maxLux > b.maxLux;
	});

	wxPoint leftEye = vectorLevelFinalLeft[0].pt;
	wxPoint rightEye = vectorLevelFinalRight[0].pt;

	CPosterize * filtre = new CPosterize(3.0, 0.6);
	filtre->SetParameter(bitmap, backcolor);
	filtre->Compute();
	delete filtre;

	CRgbaquad color = { 31, 43, 61, 0 };

	int distanceMax = abs(leftEye.x - rightEye.x) / 8;

	//---------------------------------------------------------
	//Left Eye
	//---------------------------------------------------------
	wxRect rcLeftEye = FindIris(leftEye, distanceMax, bitmap);
    
    int xStart = xmin;
    int yStart = ymin;
    
	//---------------------------------------------------------
	//Right Eye
	//---------------------------------------------------------
	wxRect rcRightEye = FindIris(rightEye, distanceMax, bitmap);

	//------------------------------------------------------------------------------

	int width = rcRightEye.width - rcRightEye.x;
	int height = rcRightEye.height - rcRightEye.y;
	CInterpolationBicubic interpolation;
	CPictureData * eyeFile = CLibResource::LoadBitmapFromResource("IDB_EYEIRIS");
	CRegardsBitmap * eyeBitmap = new CRegardsBitmap();
	eyeBitmap->SetBitmap(eyeFile->data, eyeFile->width, eyeFile->height);
	delete eyeFile;

	CRegardsBitmap * regardsbitmapOut = new CRegardsBitmap(width, height);
	interpolation.Execute(eyeBitmap, regardsbitmapOut);
	wxPoint rightEyeMaxLux;
	rightEyeMaxLux.x = xmin + (((rcRightEye.width + rcRightEye.x) / 2) - (regardsbitmapOut->GetBitmapWidth() / 2));
	rightEyeMaxLux.y = ymin + (((rcRightEye.y + rcRightEye.height) / 2) - (regardsbitmapOut->GetBitmapHeight() / 2));
	pBitmap->InsertBitmap(regardsbitmapOut, rightEyeMaxLux.x, rightEyeMaxLux.y);
	delete regardsbitmapOut;

	width = rcLeftEye.width - rcLeftEye.x;
	height = rcLeftEye.height - rcLeftEye.y;
	regardsbitmapOut = new CRegardsBitmap(width, height );
	interpolation.Execute(eyeBitmap, regardsbitmapOut);

	wxPoint leftEyeMaxLux;
	leftEyeMaxLux.x = xmin + (((rcLeftEye.width + rcLeftEye.x) / 2) - (regardsbitmapOut->GetBitmapWidth() / 2));
	leftEyeMaxLux.y = ymin + (((rcLeftEye.y + rcLeftEye.height) / 2) - (regardsbitmapOut->GetBitmapHeight() / 2));
	pBitmap->InsertBitmap(regardsbitmapOut, leftEyeMaxLux.x, leftEyeMaxLux.y);
	delete regardsbitmapOut;

	delete eyeBitmap;
    
    delete bitmap;
    
#endif
	
	return true;
}