#pragma once
class CRegardsBitmap;

class CModificationManager
{
public:
	CModificationManager(const wxString& folder);
	~CModificationManager();

	void SetNumModification(const unsigned int& numModification);
	unsigned int GetNbModification();
	unsigned int GetNumModification();
	CRegardsBitmap* GetModification(const unsigned int& numModification);
	void AddModification(CRegardsBitmap* bitmap, const wxString& libelle);
	void Init(CRegardsBitmap* bitmap);
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
