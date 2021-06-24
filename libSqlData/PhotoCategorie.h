#pragma once

class CPhotoCategorie
{
public:
	CPhotoCategorie();
	~CPhotoCategorie();

	void SetId(const int& numId);
	int GetId();

	void SetLibelle(const wxString& libelle);
	wxString GetLibelle();

private:
	int numId;
	wxString libelle;
};

using PhotoCategorieVector = std::vector<CPhotoCategorie>;
