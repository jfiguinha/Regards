#pragma once

class CPhotoCatalog
{
public:
	CPhotoCatalog();
	~CPhotoCatalog();

	void SetNumCatalog(const int& numCatalog);
	int GetNumCatalog();

	void SetLibelle(const wxString& libelle);
	wxString GetLibelle();


private:
	int numCatalog;
	wxString libelle;
};

using CatalogVector = std::vector<CPhotoCatalog>;
