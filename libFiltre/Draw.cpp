#include <header.h>

#include <algorithm>
#include <cmath>

#include "Draw.h"

using namespace Regards::FiltreEffet;

CDraw::CDraw() : m_fRatioValue(1.0f) {}

CDraw::~CDraw() = default;

bool CDraw::VerifierValiditerPoint(const wxPoint& pt) {
    const int right = m_rcAffichage.x + m_rcAffichage.width;

    const int bottom = m_rcAffichage.y + m_rcAffichage.height;

    return pt.x >= m_rcAffichage.x && pt.x <= right && pt.y >= m_rcAffichage.y &&
        pt.y <= bottom;
}

float CDraw::XDrawingPosition(const float& x, const long& hScroll,
    const float& ratio) {
    return x * ratio - static_cast<float>(hScroll);
}

float CDraw::YDrawingPosition(const float& y, const long& vScroll,
    const float& ratio) {
    return y * ratio - static_cast<float>(vScroll);
}

float CDraw::XRealPosition(const float& x, const long& hScroll,
    const float& ratio) {
    if (std::abs(ratio) < std::numeric_limits<float>::epsilon()) return 0.0f;

    return (x - static_cast<float>(m_rcAffichage.x) +
        static_cast<float>(hScroll)) /
        ratio;
}

float CDraw::YRealPosition(const float& y, const long& vScroll,
    const float& ratio) {
    if (std::abs(ratio) < std::numeric_limits<float>::epsilon()) return 0.0f;

    return (y - static_cast<float>(m_rcAffichage.y) +
        static_cast<float>(vScroll)) /
        ratio;
}

void CDraw::DessinerDashRectangle(wxDC* deviceContext, const int32_t& iTaille,
    const wxRect& rc, const wxColour& rgbFirst,
    const wxColour& rgbSecond) {
    if (deviceContext == nullptr) return;

    wxPen pen(rgbFirst, std::max(1, static_cast<int>(iTaille)),
        wxPENSTYLE_LONG_DASH);

    deviceContext->SetPen(pen);
    deviceContext->SetBrush(*wxTRANSPARENT_BRUSH);

    deviceContext->DrawRectangle(rc);

    deviceContext->SetBrush(wxNullBrush);
    deviceContext->SetPen(wxNullPen);
}

void CDraw::DessinerDotDashRectangle(wxDC* deviceContext,
    const int32_t& iTaille, const wxRect& rc,
    const wxColour& rgbFirst,
    const wxColour& rgbSecond) {
    if (deviceContext == nullptr) return;

    wxPen pen(rgbFirst, std::max(1, static_cast<int>(iTaille)),
        wxPENSTYLE_DOT_DASH);

    deviceContext->SetPen(pen);
    deviceContext->SetBrush(*wxTRANSPARENT_BRUSH);

    deviceContext->DrawRectangle(rc);

    deviceContext->SetBrush(wxNullBrush);
    deviceContext->SetPen(wxNullPen);
}

void CDraw::DessinerRectangleVide(wxDC* deviceContext, const int32_t& iTaille,
    const wxRect& rc, const wxColour& rgb) {
    if (deviceContext == nullptr) return;

    wxPen pen(rgb, std::max(1, static_cast<int>(iTaille)), wxPENSTYLE_SOLID);

    deviceContext->SetPen(pen);
    deviceContext->SetBrush(*wxTRANSPARENT_BRUSH);

    deviceContext->DrawRectangle(rc);

    deviceContext->SetBrush(wxNullBrush);
    deviceContext->SetPen(wxNullPen);
}

void CDraw::DrawARectangle(wxDC* deviceContext, const wxRect& rc,
    const wxColour& rgb) {
    if (deviceContext == nullptr) return;

    wxPen pen(rgb, 2, wxPENSTYLE_SOLID);

    deviceContext->SetPen(pen);
    deviceContext->SetBrush(*wxTRANSPARENT_BRUSH);

    deviceContext->DrawRectangle(rc);

    deviceContext->SetBrush(wxNullBrush);
    deviceContext->SetPen(wxNullPen);
}

void CDraw::DessinerCarre(wxDC* deviceContext, const int32_t& iLargeur,
    const int32_t& iHauteur, const int32_t& iMarge,
    const int32_t& iPosX, const int32_t& iPosY,
    const wxColour& rgb) {
    if (deviceContext == nullptr) return;

    if (iLargeur <= 0 || iHauteur <= 0 || iMarge <= 0) {
        return;
    }

    wxRect rect;

    // Gauche
    rect.x = iPosX;
    rect.y = iPosY;
    rect.width = iMarge;
    rect.height = iHauteur;

    DrawARectangle(deviceContext, rect, rgb);

    // Haut
    rect.x = iPosX;
    rect.y = iPosY;
    rect.width = iLargeur;
    rect.height = iMarge;

    DrawARectangle(deviceContext, rect, rgb);

    // Bas
    rect.x = iPosX;
    rect.y = iPosY + iHauteur - iMarge;
    rect.width = iLargeur;
    rect.height = iMarge;

    DrawARectangle(deviceContext, rect, rgb);

    // Droite
    rect.x = iPosX + iLargeur - iMarge;
    rect.y = iPosY;
    rect.width = iMarge;
    rect.height = iHauteur;

    DrawARectangle(deviceContext, rect, rgb);
}

void CDraw::SetMaxPosition(const wxRect& rcPicture) {
    m_rcAffichage = rcPicture;
}

void CDraw::SetRatio(const float& ratio) {
    if (std::isfinite(ratio) && ratio > 0.0f) {
        m_fRatioValue = ratio;
    }
    else {
        m_fRatioValue = 1.0f;
    }
}