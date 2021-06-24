#include <header.h>
#include "PhotoCatalog.h"


CPhotoCatalog::CPhotoCatalog(): numCatalog(0)
{
}


CPhotoCatalog::~CPhotoCatalog()
{
}


void CPhotoCatalog::SetNumCatalog(const int& numCatalog)
{
	this->numCatalog = numCatalog;
}

int CPhotoCatalog::GetNumCatalog()
{
	return numCatalog;
}

void CPhotoCatalog::SetLibelle(const wxString& libelle)
{
	this->libelle = libelle;
}

wxString CPhotoCatalog::GetLibelle()
{
	return libelle;
}
