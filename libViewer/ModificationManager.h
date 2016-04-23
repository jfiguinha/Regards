#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <RegardsBitmap.h>
#include <string>
using namespace std;

class CModificationManager
{
public:
	CModificationManager(const wxString &folder);
	~CModificationManager();

	void SetNumModification(const unsigned int &numModification);
	unsigned int GetNbModification();
	unsigned int GetNumModification();
	CRegardsBitmap * GetModification(const unsigned int &numModification);
	void AddModification(CRegardsBitmap * bitmap, const wxString &libelle);
	void Init(CRegardsBitmap * bitmap);
    wxString GetModificationLibelle(const unsigned int &numModification);
private:

    wxString GetFilenameWithModification(const unsigned int &numModification);
	void EraseData();
	int nbModification = 0;
	int numModification = 0;
	wxString folder;
    vector<wxString> listLibelle;
};

