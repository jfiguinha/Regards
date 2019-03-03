#include <header.h>
#include "Rotate.h"
#include <math.h>

CRotate::CRotate()
{
}


CRotate::~CRotate()
{
}


int CRotate::CalculXPos(int x, int y, const double &angle)
{
	float radians = angle * 0.017453292519943;
	float cosine = cos(radians);
	float sine = sin(radians);
	return int(float(x) * cosine - float(y) * sine);
}

int CRotate::CalculYPos(int x, int y, const double &angle)
{
	float radians = angle * 0.017453292519943;
	float cosine = cos(radians);
	float sine = sin(radians);
	return int(float(x) * sine + float(y) * cosine);
}

wxPoint CRotate::CalculPosition(int x, int y, const int32_t &width, const int32_t &height, const double &angle)
{
	wxPoint pt;
	int origineX = int(width / 2);
	int origineY = int(height / 2);

	int newX = x - origineX;
	int newY = y - origineY;

	pt.x = CalculXPos(newX, newY, angle) + origineX;
	pt.y = CalculYPos(newX, newY, angle) + origineY;

	return pt;
}

void CRotate::CalculBoundSize(const int32_t &width, const int32_t &height, const double &angle, wxRect &rc)
{
	wxPoint ptNO = CalculPosition(0, 0, angle, width, height);
	wxPoint ptNE = CalculPosition(width, 0, angle, width, height);
	wxPoint ptSO = CalculPosition(0, height, angle, width, height);
	wxPoint ptSE = CalculPosition(width, height, angle, width, height);

	int minx = min(ptNO.x, min(ptNE.x, min(ptSO.x, ptSE.x)));
	int maxx = max(ptNO.x, max(ptNE.x, max(ptSO.x, ptSE.x)));
	int miny = min(ptNO.y, min(ptNE.y, min(ptSO.y, ptSE.y)));
	int maxy = max(ptNO.y, max(ptNE.y, max(ptSO.y, ptSE.y)));

	rc.x = minx;
	rc.width = maxx;
	rc.y = miny;
	rc.height = maxy;
}
