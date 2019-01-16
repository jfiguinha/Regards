#pragma once
class CRegardsBitmap;

class CPainting
{
public:
	CPainting();
	~CPainting();
	wxImage CreateFromSVG(const int & buttonWidth, const int & buttonHeight, const wxString &vector);
	CRegardsBitmap * ConvertWXImageToRegardsBitmap(wxImage * image);
};

