#pragma once

class CImageLoadingFormat;

class CSavePicture
{
public:
	CSavePicture();
	~CSavePicture();
	static void SavePicture(wxWindow* window, CImageLoadingFormat* bitmap, const wxString& filename);
	static void ExportPicture(wxWindow* window, const wxString& filename);
	static wxString LoadPicture();
	static wxArrayString LoadMultiplePicture();

private:
	static wxString SelectExternalFormat(wxWindow* window, const wxString& filename);
	static vector<int> SelectPage(wxWindow* window, const wxString& filename);
	//static wxString ProcessPDFExtractFile(const vector<int> & listPage, const wxString &filename);
};
