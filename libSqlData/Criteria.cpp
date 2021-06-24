#include <header.h>
#include "Criteria.h"


CCriteria::CCriteria(): numId(0), numCategorieId(0)
{
}


CCriteria::~CCriteria()
{
}

void CCriteria::SetId(const int& numId)
{
	this->numId = numId;
}

int CCriteria::GetId()
{
	return numId;
}

void CCriteria::SetCategorieId(const int& numId)
{
	numCategorieId = numId;
}

int CCriteria::GetCategorieId()
{
	return numCategorieId;
}

void CCriteria::SetLibelle(const wxString& libelle)
{
	this->libelle = libelle;
}

wxString CCriteria::GetLibelle()
{
	return libelle;
}
