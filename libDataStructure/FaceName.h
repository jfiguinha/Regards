#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class CFaceName
{
public:
	CFaceName()
	{
	};
	~CFaceName()
	{
	};

	int numFace;
	wxString faceName;
	int isSelectable;
};