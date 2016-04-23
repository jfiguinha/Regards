#pragma once
#include <stdint.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class CRegardsBitmap;

class CInterpolation
{
public:

	CInterpolation();
	~CInterpolation();
	void Execute(CRegardsBitmap * In, CRegardsBitmap * & Out);
	void Execute(CRegardsBitmap * In, CRegardsBitmap * & Out, const wxRect &rectToShow);

};


