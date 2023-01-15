#include <header.h>
// Crop.cpp: implementation of the CCrop class.
//
//////////////////////////////////////////////////////////////////////
#include "Crop.h"
using namespace Regards::FiltreEffet;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrop::CCrop()
{
	ptSelection = new CPenDrawInfo[4];
	for (auto i = 0; i < 4; i++)
	{
		ptSelection[i].x = 0;
		ptSelection[i].y = 0;
		ptSelection[i].size = 0;
	}
	iSelect = 0;
	marge = 4;
}

CCrop::~CCrop()
{
	if (ptSelection != nullptr)
		delete[] ptSelection;

	ptSelection = nullptr;
}


void CCrop::Dessiner(wxDC* deviceContext, const long& m_lHScroll, const long& m_lVScroll, const float& ratio,
                     const wxColour& rgb, const wxColour& rgbFirst, const wxColour& rgbSecond, const int32_t& style)
{
	//Ajout d'un wxRectangle pour tester le crop
	//Trouver le wxPoint central

	//wxPoint 0 : NO
	//wxPoint 1 : NE
	//wxPoint 2 : SE
	//wxPoint 3 : SO


	wxRect rcTemp;
	rcTemp.x = XDrawingPosition(ptSelection[0].x, m_lHScroll, ratio) + m_rcAffichage.x;
	rcTemp.width = (XDrawingPosition(ptSelection[1].x, m_lHScroll, ratio) + m_rcAffichage.x) - rcTemp.x;
	rcTemp.y = YDrawingPosition(ptSelection[0].y, m_lVScroll, ratio) + m_rcAffichage.y;
	rcTemp.height = (YDrawingPosition(ptSelection[3].y, m_lVScroll, ratio) + m_rcAffichage.y) - rcTemp.y;

	switch (style)
	{
	case 0:
		DessinerRectangleVide(deviceContext, 1, rcTemp, rgbFirst);
		break;

	case 1:
		DessinerDashRectangle(deviceContext, 1, rcTemp, rgbFirst, rgbSecond);
		break;

	case 2:
		DessinerDotDashRectangle(deviceContext, 1, rcTemp, rgbFirst, rgbSecond);
		break;
	default: ;
	}


	wxRect rcTemp2;

	//Dessin de tous les petits wxRectangles
	marge = 6;

	int middlePoint = marge / 2;

	rcTemp2.x = rcTemp.x - middlePoint;
	rcTemp2.width = marge;
	rcTemp2.y = rcTemp.y - middlePoint;
	rcTemp2.height = marge;

	DessinerRectangleVide(deviceContext, 1, rcTemp2, rgb);

	//Dessin de tous les petits wxRectangles

	rcTemp2.x = (rcTemp.x + rcTemp.width) - middlePoint;
	rcTemp2.width = marge;
	rcTemp2.y = rcTemp.y - middlePoint;
	rcTemp2.height = marge;

	DessinerRectangleVide(deviceContext, 1, rcTemp2, rgb);

	//Dessin de tous les petits wxRectangles

	rcTemp2.x = (rcTemp.x + rcTemp.width) - middlePoint;
	rcTemp2.width = marge;
	rcTemp2.y = (rcTemp.y + rcTemp.height) - middlePoint;
	rcTemp2.height = marge;

	DessinerRectangleVide(deviceContext, 1, rcTemp2, rgb);

	//Dessin de tous les petits wxRectangles

	rcTemp2.x = rcTemp.x - middlePoint;
	rcTemp2.width = marge;
	rcTemp2.y = (rcTemp.y + rcTemp.height) - middlePoint;
	rcTemp2.height = marge;

	DessinerRectangleVide(deviceContext, 1, rcTemp2, rgb);

	//Dessin de tous les petits wxRectangles

	rcTemp2.x = (rcTemp.x + (rcTemp.width / 2)) - middlePoint;
	rcTemp2.width = marge;
	rcTemp2.y = (rcTemp.y + rcTemp.height) - middlePoint;
	rcTemp2.height = marge;

	DessinerRectangleVide(deviceContext, 1, rcTemp2, rgb);

	//Dessin de tous les petits wxRectangles

	rcTemp2.x = (rcTemp.x + (rcTemp.width / 2)) - middlePoint;
	rcTemp2.width = marge;
	rcTemp2.y = rcTemp.y - middlePoint;
	rcTemp2.height = marge;

	DessinerRectangleVide(deviceContext, 1, rcTemp2, rgb);

	//Dessin de tous les petits wxRectangles

	rcTemp2.x = rcTemp.x - middlePoint;
	rcTemp2.width = marge;
	rcTemp2.y = (rcTemp.y + (rcTemp.height / 2)) - middlePoint;
	rcTemp2.height = marge;

	DessinerRectangleVide(deviceContext, 1, rcTemp2, rgb);

	//Dessin de tous les petits wxRectangles

	rcTemp2.x = (rcTemp.x + rcTemp.width) - middlePoint;
	rcTemp2.width = marge;
	rcTemp2.y = (rcTemp.y + (rcTemp.height / 2)) - middlePoint;
	rcTemp2.height = marge;

	DessinerRectangleVide(deviceContext, 1, rcTemp2, rgb);
}

void CCrop::Selection(const int32_t& xNewSize, const int32_t& yNewSize, const long& m_lHScroll, const long& m_lVScroll,
                      const float& ratio)
{
	//Changement de souris
	//Test par rapport au x et y
	//Différent cas
	//float x = 0, y = 0;

	wxPoint pt;
	pt.x = xNewSize;
	pt.y = yNewSize;

	iSelect = 0;

	if (VerifierValiditerPoint(pt))
	{
		const int x = XRealPosition(xNewSize - m_rcAffichage.x, m_lVScroll, ratio);
		const int y = YRealPosition(yNewSize - m_rcAffichage.y, m_lHScroll, ratio);


#if defined(WIN32) && defined(_DEBUG)
		wchar_t message[255];
		wsprintf(message, L"Pos : x : %d et y : %d \n", x, y);
		OutputDebugString(message);
#endif

		if ((x > (ptSelection[0].x - marge) && x < (ptSelection[0].x + marge)))
			iSelect = 1;

		if ((x > (ptSelection[1].x - marge) && x < (ptSelection[1].x + marge)))
			iSelect = 2;

		if ((y > (ptSelection[0].y - marge) && y < (ptSelection[0].y + marge)))
		{
			switch (iSelect)
			{
			case 0:
				iSelect = 3;
				break;
			case 1:
				iSelect = 4;
				break;
			case 2:
				iSelect = 5;
				break;
			default: ;
			}
		}


		if ((y > (ptSelection[2].y - marge) && y < (ptSelection[2].y + marge)))
		{
			switch (iSelect)
			{
			case 0:
				iSelect = 6;
				break;
			case 1:
				iSelect = 7;
				break;
			case 2:
				iSelect = 8;
				break;
			default: ;
			}
		}


		switch (iSelect)
		{
		case 2:
		case 1:
			wxSetCursor(wxCursor(wxCURSOR_SIZEWE));
			break;
		case 3:
		case 6:
			wxSetCursor(wxCursor(wxCURSOR_SIZENS));
			break;
		case 8:
		case 4:
			wxSetCursor(wxCursor(wxCURSOR_SIZENWSE));
			break;
		case 7:
		case 5:
			wxSetCursor(wxCursor(wxCURSOR_SIZENESW));
			break;
		default:
			wxSetCursor(wxCursor(wxCURSOR_CROSS));
			break;
		}
	}
}

void CCrop::MouseMove(const long& xNewSize, const long& yNewSize, const long& m_lHScroll, const long& m_lVScroll,
                      const float& ratio)
{
	//Dessiner le wxRectangle
	wxPoint pt;
	pt.x = static_cast<int>(xNewSize);
	pt.y = static_cast<int>(yNewSize);

	if (VerifierValiditerPoint(pt))
	{
		const float x = XRealPosition(xNewSize - m_rcAffichage.x, m_lHScroll, ratio);
		const float y = YRealPosition(yNewSize - m_rcAffichage.y, m_lVScroll, ratio);

		switch (iSelect)
		{
		case 0:
			{
				ptSelection[1].x = x;
				ptSelection[1].y = ptSelection[0].y;

				ptSelection[2].x = x;
				ptSelection[2].y = y;

				ptSelection[3].x = ptSelection[0].x;
				ptSelection[3].y = y;

				wxSetCursor(wxCursor(wxCURSOR_CROSS));
			}
			break;

		case 1:
			{
				ptSelection[0].x = x;
				ptSelection[3].x = x;
				wxSetCursor(wxCursor(wxCURSOR_SIZEWE));
			}
			break;

		case 2:
			{
				ptSelection[1].x = x;
				ptSelection[2].x = x;
				wxSetCursor(wxCursor(wxCURSOR_SIZEWE));
			}
			break;

		case 3:
			{
				ptSelection[0].y = y;
				ptSelection[1].y = y;
				wxSetCursor(wxCursor(wxCURSOR_SIZENS));
			}
			break;

		case 6:
			{
				ptSelection[2].y = y;
				ptSelection[3].y = y;
				wxSetCursor(wxCursor(wxCURSOR_SIZENS));
			}
			break;

		case 4:
			{
				//NO - SE
				//wxPoint bougean 0 NO
				//wxPoint 1 bouge en x  NE
				//wxPoint 2 ne bouge pas SE
				//wxPoint 3 bouge en y SO

				ptSelection[0].x = x;
				ptSelection[0].y = y;
				ptSelection[1].y = y;
				ptSelection[3].x = x;

				wxSetCursor(wxCursor(wxCURSOR_SIZENWSE));
			}
			break;

		case 5:
			{
				//NE - SO
				//wxPoint bougean 3 
				//wxPoint 0 bouge en y et en x
				//wxPoint 1 ne bouge pas
				//wxPoint 2 bouge en x et en y

				ptSelection[0].y = y;
				ptSelection[1].x = x;
				ptSelection[1].y = y;
				ptSelection[2].x = x;

				wxSetCursor(wxCursor(wxCURSOR_SIZENESW));
			}
			break;

		case 7:
			{
				//SO - NE
				ptSelection[0].x = x;
				ptSelection[3].x = x;
				ptSelection[3].y = y;
				ptSelection[2].y = y;

				wxSetCursor(wxCursor(wxCURSOR_SIZENESW));
			}
			break;

		case 8:
			{
				//SE - NO
				ptSelection[1].x = x;
				ptSelection[2].x = x;
				ptSelection[2].y = y;
				ptSelection[3].y = y;
				wxSetCursor(wxCursor(wxCURSOR_SIZENWSE));
			}
			break;
		default: ;
		}
	}
	else
	{
		VerifierValiditerPoint(pt);
	}
}

void CCrop::InitPoint(const long& m_lx, const long& m_ly, const long& m_lHScroll, const long& m_lVScroll,
                      const float& ratio)
{
	float x, y;
	if (iSelect == 0)
	{
		wxPoint pt;
		pt.x = static_cast<int>(m_lx);
		pt.y = static_cast<int>(m_ly);
		if (VerifierValiditerPoint(pt))
		{
			x = XRealPosition(m_lx - m_rcAffichage.x, m_lHScroll, ratio);
			y = YRealPosition(m_ly - m_rcAffichage.y, m_lVScroll, ratio);

			for (auto i = 0; i < 4; i++)
			{
				ptSelection[i].x = x;
				ptSelection[i].y = y;
			}
		}
	}
}

void CCrop::GetPos(wxRect& rc)
{
	rc.x = ptSelection[0].x;
	rc.width = ptSelection[1].x - rc.x;
	rc.y = ptSelection[0].y;
	rc.height = ptSelection[3].y - rc.y;

	if (rc.width < 0)
	{
		rc.x = rc.x + rc.width;
		rc.width = abs(rc.width);
	}

	if (rc.height < 0)
	{
		rc.y = rc.y + rc.height;
		rc.height = abs(rc.height);
	}

	/*
	if ((rc.height + rc.y) > rc.y)
	{
		rc.y = rc.height + rc.y;
		rc.height = 0;
	}

	if (rc.x > (rc.width + rc.x))
	{
		rc.x = rc.x + rc.width;
		rc.width = 0;
	}
	 */
}
