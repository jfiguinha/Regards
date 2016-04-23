#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <string>
#include <vector>
using namespace std;

class CCatalog
{
public:
	CCatalog();
	~CCatalog();

	void SetNumCatalog(const int &numCatalog);
	int GetNumCatalog();

	void SetLibelle(const wxString &libelle);
	wxString GetLibelle();


private:
	int numCatalog;
	wxString libelle;
};

typedef std::vector<CCatalog> CatalogVector;