#include <header.h>
// Dessin.cpp: implementation of the CDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "Draw.h"
using namespace Regards::FiltreEffet;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDraw::CDraw()
{
	m_fRatioValue = 1.0;
}

CDraw::~CDraw()
{
}

bool CDraw::VerifierValiditerPoint(const wxPoint& pt)
{
	if (pt.x < m_rcAffichage.x || (pt.x > (m_rcAffichage.x + m_rcAffichage.width)) || (pt.y > (m_rcAffichage.y + m_rcAffichage.height)) || (pt.y < m_rcAffichage.y))
		return false;

	return true;
}

float CDraw::XDrawingPosition(const float& m_lx, const long& m_lHScroll, const float& ratio)
{
	int x = (m_lx * ratio) - m_lHScroll;
	return x;
}

float CDraw::YDrawingPosition(const float& m_ly, const long& m_lVScroll, const float& ratio)
{
	int y = (m_ly * ratio) - m_lVScroll;
	return y;
}


float CDraw::XRealPosition(const float& m_lx, const long& m_lHScroll, const float& ratio)
{
	int x = (m_lx + m_lHScroll) / ratio;
	if (x < 0)
	{
		printf("Error");
	}
	return x;
}

float CDraw::YRealPosition(const float& m_ly, const long& m_lVScroll, const float& ratio)
{
	int y = (m_ly + m_lVScroll) / ratio;
	return y;
}


void CDraw::DessinerDashRectangle(wxDC* deviceContext, const int32_t& iTaille, const wxRect& rc,
                                  const wxColour& rgbFirst, const wxColour& rgbSecond)
{
	wxPen dashDotPen(rgbFirst, 1, wxPENSTYLE_LONG_DASH);
	deviceContext->SetPen(dashDotPen);
	deviceContext->SetBrush(*wxTRANSPARENT_BRUSH);
	deviceContext->DrawRectangle(rc);
	deviceContext->SetBrush(wxNullBrush);
	deviceContext->SetPen(wxNullPen);
}


void CDraw::DessinerDotDashRectangle(wxDC* deviceContext, const int32_t& iTaille, const wxRect& rc,
                                     const wxColour& rgbFirst, const wxColour& rgbSecond)
{
	wxPen dashDotPen(rgbFirst, iTaille, wxPENSTYLE_DOT_DASH);
	deviceContext->SetPen(dashDotPen);
	deviceContext->SetBrush(*wxTRANSPARENT_BRUSH);
	deviceContext->DrawRectangle(rc);
	deviceContext->SetBrush(wxNullBrush);
	deviceContext->SetPen(wxNullPen);
}

void CDraw::DessinerRectangleVide(wxDC* deviceContext, const int32_t& iTaille, const wxRect& rc, const wxColour& rgb)
{
	//wxBrush brush = wxBrush(*wxTRANSPARENT_BRUSH);
	wxPen pen(rgb, iTaille, wxPENSTYLE_SOLID);
	deviceContext->SetPen(pen);
	deviceContext->SetBrush(*wxTRANSPARENT_BRUSH);
	deviceContext->DrawRectangle(rc);
	deviceContext->SetBrush(wxNullBrush);
	deviceContext->SetPen(wxNullPen);
}

void CDraw::DrawARectangle(wxDC* deviceContext, const wxRect& rc, const wxColour& rgb)
{
	wxPen pen(rgb, 2, wxPENSTYLE_SOLID);
	deviceContext->SetPen(pen);
	deviceContext->DrawRectangle(rc);
	deviceContext->SetPen(wxNullPen);
}

void CDraw::DessinerCarre(wxDC* deviceContext, const int32_t& iLargeur, const int32_t& iHauteur, const int32_t& iMarge,
                          const int32_t& iPosX, const int32_t& iPosY, const wxColour& rgb)
{
	wxRect rc;

	rc.x = iPosX;
	rc.y = iPosY;
	rc.width = iMarge;
	rc.height = iHauteur;

	DrawARectangle(deviceContext, rc, rgb);

	//MoveToEx(destDC,iPosX,iPosY,nullptr);
	rc.x = iPosX;
	rc.y = iPosY;
	rc.width = iLargeur;
	rc.height = iMarge;
	DrawARectangle(deviceContext, rc, rgb);

	//MoveToEx(destDC,iPosX,iPosY,nullptr);
	rc.x = iPosX;
	rc.y = iPosY + (iHauteur - iMarge);
	rc.width = iLargeur;
	rc.height = iHauteur;
	DrawARectangle(deviceContext, rc, rgb);

	//MoveToEx(destDC,iPosX,iPosY,nullptr);
	rc.x = iPosX + (iLargeur - iMarge);
	rc.y = iPosY;
	rc.width = iPosX + iLargeur;
	rc.height = iPosY + iHauteur;
	DrawARectangle(deviceContext, rc, rgb);
}

void CDraw::SetMaxPosition(const wxRect& m_rcPicture)
{
	m_rcAffichage = m_rcPicture;
}

void CDraw::SetRatio(const float& m_fValue)
{
	m_fRatioValue = m_fValue;
}
