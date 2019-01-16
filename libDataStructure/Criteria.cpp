#include "Criteria.h"


CCriteria::CCriteria()
{
}


CCriteria::~CCriteria()
{
}

void CCriteria::SetId(const int &numId)
{
	this->numId = numId;
}

int CCriteria::GetId()
{
	return numId;
}

void CCriteria::SetCategorieId(const int &numId)
{
	numCategorieId = numId;
}

int CCriteria::GetCategorieId()
{
	return numCategorieId;
}

void CCriteria::SetLibelle(const wxString &libelle)
{
	this->libelle = libelle;
}

wxString CCriteria::GetLibelle()
{
	return libelle;
}