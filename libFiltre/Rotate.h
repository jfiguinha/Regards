#pragma once
#include <stdint.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
class CRotate
{
public:
	CRotate();
	~CRotate();

	static void CalculBoundSize(const int32_t &width, const int32_t &height, const double &angle, wxRect &rc);
	

private:

	static wxPoint CalculPosition(int x, int y, const int32_t &width, const int32_t &height, const double &angle);
	static int CalculXPos(int x, int y, const double &angle);
	static int CalculYPos(int x, int y, const double &angle);
	
};

