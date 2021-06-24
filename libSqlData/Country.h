#pragma once

class CCountry
{
public:
	CCountry();
	~CCountry();

	void SetId(const int& numId);
	int GetId();

	void SetCode(const wxString& code);
	wxString GetCode();

	void SetContinent(const wxString& continent);
	wxString GetContinent();

	void SetLibelle(const wxString& libelle);
	wxString GetLibelle();

	// this works for the last name

	bool operator==(const CCountry& right)
	{
		return (this->code == right.code);
	}

private:
	int numId;
	wxString code;
	wxString continent;
	wxString libelle;
};


using CountryVector = std::vector<CCountry>;
