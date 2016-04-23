#include "Catalog.h"


CCatalog::CCatalog()
{
}


CCatalog::~CCatalog()
{
}


void CCatalog::SetNumCatalog(const int &numCatalog)
{
	this->numCatalog = numCatalog;
}

int CCatalog::GetNumCatalog()
{
	return numCatalog;
}

void CCatalog::SetLibelle(const wxString &libelle)
{
	this->libelle = libelle;
}

wxString CCatalog::GetLibelle()
{
	return libelle;
}
