#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class CFaceFilePath
{
public:
	CFaceFilePath()
	{
	};
	~CFaceFilePath()
	{
	};

	int numFace;
	int numPhoto;
	wxString faceFilePath;
};