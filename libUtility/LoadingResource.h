#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class CRegardsBitmap;

class CLoadingResource
{
public:
	CLoadingResource(){};
	~CLoadingResource(){};

	wxBitmap LoadBmpResource(const wxString & resourceName, const bool &loadAlpha = true);

	wxImage LoadImageResource(const wxString & resourceName, const bool &loadAlpha = true);

	CRegardsBitmap * LoadRegardsBmpResource(const wxString & resourceName);

	wxImage ConvertTowxImage(CRegardsBitmap * bitmap, const bool &loadAlpha = true, const bool &invertY = true);

	wxImage ConvertTowxImageRGB(CRegardsBitmap * bitmap, const bool &loadAlpha = true);
    
};

