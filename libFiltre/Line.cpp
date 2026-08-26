#include <header.h>
#include <RGBAQuad.h>
#include <algorithm>
#include <cmath>
#include "Line.h"

CLine::CLine(const int& heightMax, const int& widthMax)
    : heightMax(heightMax), widthMax(widthMax) {}

CLine::~CLine() = default;

void CLine::MidpointLine(cv::Mat* bitmap, const int& xFrom, const int& yFrom,
    const int& xTo, const int& yTo, const CRgbaquad& color,
    const float& alpha, const bool& antialiasing) {
    if (bitmap == nullptr || bitmap->empty()) {
        return;
    }

    if (widthMax <= 0 || heightMax <= 0) {
        return;
    }

    const float clampedAlpha = std::clamp(alpha, 0.0f, 1.0f);

    // Cas particulier : point unique.
    if (xFrom == xTo && yFrom == yTo) {
        SetAlphaColorValue(xFrom, yFrom, xFrom, yFrom, clampedAlpha, color, bitmap);

        return;
    }

    // Bresenham.
    int x0 = xFrom;
    int y0 = yFrom;

    const int x1 = xTo;
    const int y1 = yTo;

    const int dx = std::abs(x1 - x0);

    const int dy = std::abs(y1 - y0);

    const int sx = (x0 < x1) ? 1 : -1;

    const int sy = (y0 < y1) ? 1 : -1;

    int error = dx - dy;

    while (true) {
        SetAlphaColorValue(x0, y0, x0, y0, clampedAlpha, color, bitmap);

        if (x0 == x1 && y0 == y1) {
            break;
        }

        const int error2 = error * 2;

        if (error2 > -dy) {
            error -= dy;
            x0 += sx;
        }

        if (error2 < dx) {
            error += dx;
            y0 += sy;
        }
    }
}

void CLine::SetAlphaColorValue(const int& xFrom, const int& yFrom, const int& x,
    const int& y, const float& alpha,
    const CRgbaquad& color, cv::Mat* bitmap) {
    if (bitmap == nullptr || bitmap->empty()) {
        return;
    }

    const float clampedAlpha = std::clamp(alpha, 0.0f, 1.0f);

    if (clampedAlpha <= 0.0f) return;

    /*
     * La zone utilisable est :
     *
     * [widthMax - bitmapWidth, widthMax[
     * [heightMax - bitmapHeight, heightMax[
     *
     * On conserve ici la logique de l'ancien code.
     */

    const int bitmapWidth = bitmap->cols;

    const int bitmapHeight = bitmap->rows;

    const int minX = bitmapWidth - widthMax;

    const int minY = bitmapHeight - heightMax;

    if (xFrom < minX || xFrom >= widthMax || x < minX || x >= widthMax) {
        return;
    }

    if (yFrom < minY || yFrom >= heightMax || y < minY || y >= heightMax) {
        return;
    }

    // Protection supplémentaire contre les coordonnées
    // situées hors de la matrice OpenCV.
    if (xFrom < 0 || xFrom >= bitmapWidth || yFrom < 0 || yFrom >= bitmapHeight ||
        x < 0 || x >= bitmapWidth || y < 0 || y >= bitmapHeight) {
        return;
    }

    CRgbaquad* currentColor = CRgbaquad::GetPtColorValue(bitmap, xFrom, yFrom);

    if (currentColor == nullptr) return;

    const float inverseAlpha = 1.0f - clampedAlpha;

    currentColor->SetRed(color.GetFRed() * inverseAlpha +
        currentColor->GetFRed() * clampedAlpha);

    currentColor->SetGreen(color.GetFGreen() * inverseAlpha +
        currentColor->GetFGreen() * clampedAlpha);

    currentColor->SetBlue(color.GetFBlue() * inverseAlpha +
        currentColor->GetFBlue() * clampedAlpha);
}