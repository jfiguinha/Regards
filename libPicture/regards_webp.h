#pragma once
class CRegardsBitmap;

class CRegardsWebp
{
public:


	
	static CRegardsBitmap* GetPicture(const wxString& filename);
	static int GetNbFrame(const wxString& filename);
	static void GetPictureDimension(const wxString& filename, int& width, int& height);
	static void SavePicture(const wxString& filenameOut, CRegardsBitmap* source, const int& compression = 50);
	static vector<CRegardsBitmap*> GetAllPicture(const wxString& filename, int& delay);
private:

};