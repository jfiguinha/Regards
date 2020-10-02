#include <header.h>
#include "TreeData.h"
using namespace Regards::Window;

CTreeData::CTreeData(void)
{
    type = 1;
	valeur = L"";
	exifKey = L"";
	key = L"";
	actif = false;
	type = 0;
}


CTreeData::~CTreeData(void)
{
}

void CTreeData::SetActif(const bool & value)
{
	actif = value;
}

bool CTreeData::GetActif()
{
	return actif;
}


void CTreeData::SetKey(const wxString & value)
{
	key = value;
}

wxString CTreeData::GetKey()
{
	return key;
}


void CTreeData::SetValue(const wxString & value)
{
	valeur = value;
}

wxString CTreeData::GetValue()
{
	return valeur;
}


void CTreeData::SetExifKey(const wxString & value)
{
	exifKey = value;
}

wxString CTreeData::GetExifKey()
{
	return exifKey;
}


void CTreeData::SetType(const int & type)
{
	this->type = type;
}

int CTreeData::GetType()
{
	return type;
}