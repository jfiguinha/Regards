#pragma once

class CCriteria
{
public:
	CCriteria();
	~CCriteria();

	void SetId(const int& numId);
	int GetId();

	void SetCategorieId(const int& numId);
	int GetCategorieId();

	void SetLibelle(const wxString& libelle);
	wxString GetLibelle();

private:
	int numId;
	int numCategorieId;
	wxString libelle;
};

using CriteriaVector = std::vector<CCriteria>;
