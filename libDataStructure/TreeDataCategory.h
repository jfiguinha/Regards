#pragma once
#include "TreeData.h"

class CTreeDataCategory : public CTreeData
{
public:

	CTreeDataCategory();
	~CTreeDataCategory();

	bool operator==(const CTreeDataCategory &value) const;

	void SetTypeElement(const int &typeElement);
	int GetTypeElement();

	void SetIdElement(const int &id);
	int GetIdElement();
	wxString GetStringIdElement();

	void SetNumElement(const int &iNumElement);
	int GetNumElement();

	void SetNumParent(const int &iNumElement);
	int GetNumParent();

	void SetNumCatalog(const int & value);
	int GetNumCatalog();

	void SetNumCategorie(const int & value);
	int GetNumCategorie();

	void SetNumLevel(const int &iNumLevel);
	int GetNumLevel();

private:
	int typeElement;
	int iNumElement;
	int id;
	int iNumParent;
	int numCatalog;
	int numCategorie;
	int iNumLevel;
};
