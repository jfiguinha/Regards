#pragma once

class CImageLoadingFormat;

class CSavePicture
{
public:
	CSavePicture();
	~CSavePicture();
	static void SavePicture(wxWindow * window, CImageLoadingFormat * bitmap, const wxString &filename);
};

