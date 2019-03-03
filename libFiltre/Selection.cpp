#include <header.h>
// Crop.cpp: implementation of the CCrop class.
//
//////////////////////////////////////////////////////////////////////
#include "Selection.h"
using namespace Regards::FiltreEffet;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSelection::CSelection()
{
	ptSelection = new CPenDrawInfo[1];
	ptSelection[0].x = 0;
	ptSelection[0].y = 0;
	iSelect = 0;
	marge = 4;
	ptScreen = wxPoint(0, 0);
}

CSelection::~CSelection()
{
	if (ptSelection != nullptr)
		delete[] ptSelection;

	ptSelection = nullptr;
}


void CSelection::Dessiner(wxDC * deviceContext, const long &m_lHScroll, const long &m_lVScroll, const float &ratio, const wxColour &rgb, const wxColour &rgbFirst, const wxColour &rgbSecond, const int32_t &style)
{
	wxRect rcTemp;
	rcTemp.x = XDrawingPosition(ptSelection[0].x, m_lHScroll, ratio) - marge + m_rcAffichage.x;
	rcTemp.width = marge * 2;
	rcTemp.y = YDrawingPosition(ptSelection[0].y, m_lVScroll, ratio) - marge + m_rcAffichage.y;
	rcTemp.height = marge * 2;
	DessinerRectangleVide(deviceContext, 1, rcTemp, rgb);
	
}

void CSelection::Selection(const int32_t &xNewSize, const int32_t &yNewSize, const long &m_lHScroll, const long &m_lVScroll, const float &ratio)
{
	//Changement de souris
	//Test par rapport au x et y
	//Différent cas
	//float x = 0, y = 0;

	pt.x = xNewSize;
	pt.y = yNewSize;

	iSelect = 0;

	if (VerifierValiditerPoint(pt))
	{
		//int x = 0, y = 0;
		//x = XRealPosition(xNewSize - m_rcAffichage.x, m_lVScroll, ratio);
		//y = YRealPosition(yNewSize - m_rcAffichage.y, m_lHScroll, ratio);

		ptScreen = pt;

		switch (iSelect)
		{
		default:
			wxSetCursor(wxCursor(wxCURSOR_CROSS));
			break;
		}
	}
}

void CSelection::MouseMove(const long &xNewSize, const long &yNewSize, const long &m_lHScroll, const long &m_lVScroll, const float &ratio)
{
	//Dessiner le rectangle
	pt.x = (int)xNewSize;
	pt.y = (int)yNewSize;

	if (VerifierValiditerPoint(pt))
	{
		ptScreen = pt;
		//float x = 0, y = 0;
		ptSelection[0].x = XRealPosition(xNewSize - m_rcAffichage.x, m_lHScroll, ratio);
		ptSelection[0].y = YRealPosition(yNewSize - m_rcAffichage.y, m_lVScroll, ratio);
		wxSetCursor(wxCursor(wxCURSOR_CROSS));
	}
}

void CSelection::InitPoint(const long &m_lx, const long &m_ly, const long &m_lHScroll, const long &m_lVScroll, const float &ratio)
{
	float x, y;
	if(iSelect == 0)
	{	
		//pt.x = m_lx;
		//pt.y = m_ly;
		if (VerifierValiditerPoint(pt))
		{
			x = XRealPosition(m_lx - m_rcAffichage.x, m_lHScroll , ratio);
			y = YRealPosition(m_ly - m_rcAffichage.y, m_lVScroll, ratio);
			ptSelection[0].x = x;
			ptSelection[0].y = y;
			ptScreen = pt;
		}
	}
}

void CSelection::GetPoint(wxPoint &pt)
{
	pt.x = ptSelection[0].x;
	pt.y = ptSelection[0].y;
}


void CSelection::GetScreenPoint(wxPoint &pt)
{
	pt = ptScreen;
}
