#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <string>
#include <vector>
using namespace std;

class CPhotoCategorie
{
public:
	CPhotoCategorie();
	~CPhotoCategorie();

	void SetId(const int &numId);
	int GetId();

	void SetLibelle(const wxString &libelle);
	wxString GetLibelle();

private:
	int numId;
	wxString libelle;
};

typedef std::vector<CPhotoCategorie> PhotoCategorieVector;
