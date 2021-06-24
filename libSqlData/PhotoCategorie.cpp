#include <header.h>
#include "PhotoCategorie.h"


CPhotoCategorie::CPhotoCategorie(): numId(0)
{
}


CPhotoCategorie::~CPhotoCategorie()
{
}


void CPhotoCategorie::SetId(const int& numId)
{
	this->numId = numId;
}

int CPhotoCategorie::GetId()
{
	return this->numId;
}

void CPhotoCategorie::SetLibelle(const wxString& libelle)
{
	this->libelle = libelle;
}

wxString CPhotoCategorie::GetLibelle()
{
	return libelle;
}
