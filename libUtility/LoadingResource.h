#pragma once
class CRegardsBitmap;

class CLoadingResource
{
public:

	
	static CRegardsBitmap * LoadRegardsBmpResource(const wxString & resourceName);
	static wxImage LoadImageResource(const wxString & resourceName);
	static wxImage ConvertTowxImageRGB(CRegardsBitmap * bitmap);
    
};

