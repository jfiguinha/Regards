#pragma once

class CRegardsBitmap;

class CSavePicture
{
public:
	CSavePicture();
	~CSavePicture();
	static void SavePicture(wxWindow * window, CRegardsBitmap * bitmap, const wxString &filename);
};

