#include <header.h>

#include <algorithm>
#include <array>
#include <cmath>

#include "Crop.h"

using namespace Regards::FiltreEffet;

namespace {
    constexpr int POINT_NORTH_WEST = 0;
    constexpr int POINT_NORTH_EAST = 1;
    constexpr int POINT_SOUTH_EAST = 2;
    constexpr int POINT_SOUTH_WEST = 3;
}  // namespace

CCrop::CCrop() {
    for (auto& point : ptSelection) {
        point.x = 0;
        point.y = 0;
        point.size = 0;
    }
}

wxRect CCrop::GetDrawingRect(const long& hScroll, const long& vScroll,
    const float& ratio) {
    wxRect rect;

    rect.x = XDrawingPosition(ptSelection[POINT_NORTH_WEST].x, hScroll, ratio);

    const int right =
        XDrawingPosition(ptSelection[POINT_NORTH_EAST].x, hScroll, ratio);

    rect.width = right - rect.x;

    rect.y = YDrawingPosition(ptSelection[POINT_NORTH_WEST].y, vScroll, ratio);

    const int bottom =
        YDrawingPosition(ptSelection[POINT_SOUTH_WEST].y, vScroll, ratio);

    rect.height = bottom - rect.y;

    return rect;
}

void CCrop::DrawHandle(wxDC* deviceContext, const wxRect& rect, int x, int y,
    const wxColour& colour) const {
    wxRect handle(x - HANDLE_HALF_SIZE, y - HANDLE_HALF_SIZE, HANDLE_SIZE,
        HANDLE_SIZE);

    DessinerRectangleVide(deviceContext, 1, handle, colour);
}

void CCrop::Dessiner(wxDC* deviceContext, const long& m_lHScroll,
    const long& m_lVScroll, const float& ratio,
    const wxColour& rgb, const wxColour& rgbFirst,
    const wxColour& rgbSecond, const int32_t& style) {
    if (deviceContext == nullptr) return;

    const wxRect rect = GetDrawingRect(m_lHScroll, m_lVScroll, ratio);

    switch (style) {
    case 0:
        DessinerRectangleVide(deviceContext, 1, rect, rgbFirst);
        break;

    case 1:
        DessinerDashRectangle(deviceContext, 1, rect, rgbFirst, rgbSecond);
        break;

    case 2:
        DessinerDotDashRectangle(deviceContext, 1, rect, rgbFirst, rgbSecond);
        break;

    default:
        break;
    }

    const int centerX = rect.x + rect.width / 2;

    const int centerY = rect.y + rect.height / 2;

    DrawHandle(deviceContext, rect, rect.x, rect.y, rgb);

    DrawHandle(deviceContext, rect, rect.x + rect.width, rect.y, rgb);

    DrawHandle(deviceContext, rect, rect.x + rect.width, rect.y + rect.height,
        rgb);

    DrawHandle(deviceContext, rect, rect.x, rect.y + rect.height, rgb);

    DrawHandle(deviceContext, rect, centerX, rect.y, rgb);

    DrawHandle(deviceContext, rect, centerX, rect.y + rect.height, rgb);

    DrawHandle(deviceContext, rect, rect.x, centerY, rgb);

    DrawHandle(deviceContext, rect, rect.x + rect.width, centerY, rgb);
}

void CCrop::Selection(const int32_t& xNewSize, const int32_t& yNewSize,
    const long& m_lHScroll, const long& m_lVScroll,
    const float& ratio) {
    const wxPoint point(xNewSize, yNewSize);

    iSelect = SelectionType::Aucun;

    if (!VerifierValiditerPoint(point)) {
        UpdateCursor();
        return;
    }

    const int x = XRealPosition(xNewSize, m_lHScroll, ratio);

    const int y = YRealPosition(yNewSize, m_lVScroll, ratio);

    const auto& topLeft = ptSelection[POINT_NORTH_WEST];

    const auto& topRight = ptSelection[POINT_NORTH_EAST];

    const auto& bottomRight = ptSelection[POINT_SOUTH_EAST];

    const int margin = HANDLE_SIZE;

    const bool left = x > topLeft.x - margin && x < topLeft.x + margin;

    const bool right = x > topRight.x - margin && x < topRight.x + margin;

    const bool top = y > topLeft.y - margin && y < topLeft.y + margin;

    const bool bottom = y > bottomRight.y - margin && y < bottomRight.y + margin;

    if (left && top)
        iSelect = SelectionType::TopLeft;
    else if (right && top)
        iSelect = SelectionType::TopRight;
    else if (left && bottom)
        iSelect = SelectionType::BottomLeft;
    else if (right && bottom)
        iSelect = SelectionType::BottomRight;
    else if (left)
        iSelect = SelectionType::Left;
    else if (right)
        iSelect = SelectionType::Right;
    else if (top)
        iSelect = SelectionType::Top;
    else if (bottom)
        iSelect = SelectionType::Bottom;

    UpdateCursor();
}

void CCrop::UpdateCursor() const {
    switch (iSelect) {
    case SelectionType::Left:
    case SelectionType::Right:
        wxSetCursor(wxCursor(wxCURSOR_SIZEWE));
        break;

    case SelectionType::Top:
    case SelectionType::Bottom:
        wxSetCursor(wxCursor(wxCURSOR_SIZENS));
        break;

    case SelectionType::TopLeft:
    case SelectionType::BottomRight:
        wxSetCursor(wxCursor(wxCURSOR_SIZENWSE));
        break;

    case SelectionType::TopRight:
    case SelectionType::BottomLeft:
        wxSetCursor(wxCursor(wxCURSOR_SIZENESW));
        break;

    default:
        wxSetCursor(wxCursor(wxCURSOR_CROSS));
        break;
    }
}

void CCrop::MouseMove(const long& xNewSize, const long& yNewSize,
    const long& m_lHScroll, const long& m_lVScroll,
    const float& ratio) {
    const wxPoint point(static_cast<int>(xNewSize), static_cast<int>(yNewSize));

    if (!VerifierValiditerPoint(point)) return;

    const float x = XRealPosition(xNewSize, m_lHScroll, ratio);

    const float y = YRealPosition(yNewSize, m_lVScroll, ratio);

    switch (iSelect) {
    case SelectionType::Aucun:
        ptSelection[POINT_NORTH_EAST].x = x;
        ptSelection[POINT_NORTH_EAST].y = ptSelection[POINT_NORTH_WEST].y;

        ptSelection[POINT_SOUTH_EAST].x = x;
        ptSelection[POINT_SOUTH_EAST].y = y;

        ptSelection[POINT_SOUTH_WEST].x = ptSelection[POINT_NORTH_WEST].x;

        ptSelection[POINT_SOUTH_WEST].y = y;
        break;

    case SelectionType::Left:
        ptSelection[POINT_NORTH_WEST].x = x;
        ptSelection[POINT_SOUTH_WEST].x = x;
        break;

    case SelectionType::Right:
        ptSelection[POINT_NORTH_EAST].x = x;
        ptSelection[POINT_SOUTH_EAST].x = x;
        break;

    case SelectionType::Top:
        ptSelection[POINT_NORTH_WEST].y = y;
        ptSelection[POINT_NORTH_EAST].y = y;
        break;

    case SelectionType::Bottom:
        ptSelection[POINT_SOUTH_EAST].y = y;
        ptSelection[POINT_SOUTH_WEST].y = y;
        break;

    case SelectionType::TopLeft:
        ptSelection[POINT_NORTH_WEST].x = x;
        ptSelection[POINT_NORTH_WEST].y = y;

        ptSelection[POINT_NORTH_EAST].y = y;
        ptSelection[POINT_SOUTH_WEST].x = x;
        break;

    case SelectionType::TopRight:
        ptSelection[POINT_NORTH_EAST].x = x;
        ptSelection[POINT_NORTH_EAST].y = y;

        ptSelection[POINT_NORTH_WEST].y = y;
        ptSelection[POINT_SOUTH_EAST].x = x;
        break;

    case SelectionType::BottomLeft:
        ptSelection[POINT_SOUTH_WEST].x = x;
        ptSelection[POINT_SOUTH_WEST].y = y;

        ptSelection[POINT_NORTH_WEST].x = x;
        ptSelection[POINT_SOUTH_EAST].y = y;
        break;

    case SelectionType::BottomRight:
        ptSelection[POINT_SOUTH_EAST].x = x;
        ptSelection[POINT_SOUTH_EAST].y = y;

        ptSelection[POINT_NORTH_EAST].x = x;
        ptSelection[POINT_SOUTH_WEST].y = y;
        break;
    }

    UpdateCursor();
}

void CCrop::InitPoint(const long& m_lx, const long& m_ly,
    const long& m_lHScroll, const long& m_lVScroll,
    const float& ratio) {
    if (iSelect != SelectionType::Aucun) return;

    const wxPoint point(static_cast<int>(m_lx), static_cast<int>(m_ly));

    if (!VerifierValiditerPoint(point)) return;

    const float x = XRealPosition(m_lx, m_lHScroll, ratio);

    const float y = YRealPosition(m_ly, m_lVScroll, ratio);

    for (auto& selectionPoint : ptSelection) {
        selectionPoint.x = x;
        selectionPoint.y = y;
    }
}

void CCrop::GetPos(wxRect& rc) {
    const int x1 = static_cast<int>(ptSelection[POINT_NORTH_WEST].x);

    const int y1 = static_cast<int>(ptSelection[POINT_NORTH_WEST].y);

    const int x2 = static_cast<int>(ptSelection[POINT_NORTH_EAST].x);

    const int y2 = static_cast<int>(ptSelection[POINT_SOUTH_WEST].y);

    rc.x = std::min(x1, x2);
    rc.y = std::min(y1, y2);

    rc.width = std::abs(x2 - x1);
    rc.height = std::abs(y2 - y1);
}