#include "TreeDataCategory.h"


CTreeDataCategory::CTreeDataCategory()
{
	iNumElement = 0;
}


CTreeDataCategory::~CTreeDataCategory()
{
}

bool CTreeDataCategory::operator == (const CTreeDataCategory &value) const
{
	if (typeElement == value.typeElement &&
		iNumElement == value.iNumElement &&
		iNumParent == value.iNumParent &&
		numCatalog == value.numCatalog &&
		numCategorie == value.numCategorie &&
		valeur == value.valeur &&
		exifKey == value.exifKey &&
		key == value.key &&
		parent == value.parent && id == value.id && iNumLevel == value.iNumLevel)
		return true;

	return false;
}

void CTreeDataCategory::SetNumLevel(const int &iNumLevel)
{
	this->iNumLevel = iNumLevel;
}
int CTreeDataCategory::GetNumLevel()
{
	return iNumLevel;
}

void CTreeDataCategory::SetTypeElement(const int &typeElement)
{
	this->typeElement = typeElement;
}

int CTreeDataCategory::GetTypeElement()
{
	return typeElement;
}

void CTreeDataCategory::SetIdElement(const int & id)
{
	this->id = id;
}

int CTreeDataCategory::GetIdElement()
{
	return id;
}

wxString CTreeDataCategory::GetStringIdElement()
{
	return std::to_string(id);
}

void CTreeDataCategory::SetNumCatalog(const int & value)
{
	numCatalog = value;
}

int CTreeDataCategory::GetNumCatalog()
{
	return numCatalog;
}

void CTreeDataCategory::SetNumCategorie(const int & value)
{
	numCategorie = value;
}

int CTreeDataCategory::GetNumCategorie()
{
	return numCategorie;
}


void CTreeDataCategory::SetNumElement(const int &iNumElement)
{
	this->iNumElement = iNumElement;
}

int CTreeDataCategory::GetNumElement()
{
	return iNumElement;
}

void CTreeDataCategory::SetNumParent(const int &iNumElement)
{
	this->iNumParent = iNumElement;
}

int CTreeDataCategory::GetNumParent()
{
	return iNumParent;
}
