#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class CResourceCursor
{
public:
	CResourceCursor(){};
	~CResourceCursor(){};
	static wxCursor GetClosedHand();

};