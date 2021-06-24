#include <header.h>
#include "Country.h"


CCountry::CCountry(): numId(0)
{
}


CCountry::~CCountry()
{
}


void CCountry::SetId(const int& numId)
{
	this->numId = numId;
}

int CCountry::GetId()
{
	return numId;
}

void CCountry::SetContinent(const wxString& continent)
{
	this->continent = continent;
}

wxString CCountry::GetContinent()
{
	return continent;
}

void CCountry::SetCode(const wxString& code)
{
	this->code = code;
}

wxString CCountry::GetCode()
{
	return code;
}

void CCountry::SetLibelle(const wxString& libelle)
{
	this->libelle = libelle;
}

wxString CCountry::GetLibelle()
{
	return libelle;
}
