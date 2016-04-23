#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <string>
#include <vector>
using namespace std;

class CCriteria
{
public:
	CCriteria();
	~CCriteria();

	void SetId(const int &numId);
	int GetId();

	void SetCategorieId(const int &numId);
	int GetCategorieId();

	void SetLibelle(const wxString &libelle);
	wxString GetLibelle();

private:
	int numId;
	int numCategorieId;
	wxString libelle;
};

typedef std::vector<CCriteria> CriteriaVector;



