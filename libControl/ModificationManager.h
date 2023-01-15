#pragma once
class CImageLoadingFormat;

class CModificationManager
{
public:
	CModificationManager(const wxString& folder);
	~CModificationManager();

	void SetNumModification(const unsigned int& numModification);
	unsigned int GetNbModification();
	unsigned int GetNumModification();
	CImageLoadingFormat* GetModification(const unsigned int& numModification);
	void AddModification(CImageLoadingFormat* bitmap, const wxString& libelle);
	void Init(CImageLoadingFormat* bitmap);
	wxString GetModificationLibelle(const unsigned int& numModification);

private:
	wxString GetFilenameWithModification(const unsigned int& numModification);
	void EraseData();
	int nbModification;
	int numModification;
	int orientation;
	wxString folder;
	wxString filenameBitmap;
	vector<wxString> listLibelle;
};
