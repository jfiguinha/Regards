#pragma once

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
