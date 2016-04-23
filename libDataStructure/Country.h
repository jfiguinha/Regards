#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <string>
#include <vector>
using namespace std;

class CCountry
{
public:
	CCountry();
	~CCountry();

	void SetId(const int &numId);
	int GetId();

	void SetCode(const wxString &code);
	wxString GetCode();

	void SetContinent(const wxString &continent);
	wxString GetContinent();

	void SetLibelle(const wxString &libelle);
	wxString GetLibelle();

	// this works for the last name

	inline bool operator==(const CCountry& right)
	{
		return (this->code == right.code);
	}

private:

	int numId;
	wxString code;
	wxString continent;
	wxString libelle;
};


typedef std::vector<CCountry> CountryVector;


