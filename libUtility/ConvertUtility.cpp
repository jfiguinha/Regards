#include "ConvertUtility.h"
#include <sstream>
#include <string>
using namespace std;

CConvertUtility::CConvertUtility(void)
{
}


CConvertUtility::~CConvertUtility(void)
{
}

wxString CConvertUtility::GetTimeLibelle(const int &timePosition)
{
	wxString libelle = L"";
	//wchar_t buf[255];
	int hour = timePosition / 3600;
	int minute = (timePosition % 3600) / 60;
	int seconds = timePosition % 60;

	wxString shour = std::to_string(hour);
	wxString sminute = std::to_string(minute);
	wxString sseconds = std::to_string(seconds);

	if (shour.size() < 2)
		libelle.append(L"0");
	libelle.append(shour);
	libelle.append(L":");
	if (sminute.size() < 2)
		libelle.append(L"0");
	libelle.append(sminute);
	libelle.append(L":");
	if (sseconds.size() < 2)
		libelle.append(L"0");
	libelle.append(sseconds);

	return libelle;
}

std::vector<wxString> CConvertUtility::split(const wxString & s, char seperator)
{
	std::vector<wxString> output;

	wxString::size_type prev_pos = 0, pos = 0;

	while ((pos = s.find(seperator, pos)) != std::string::npos)
	{
		wxString substring(s.substr(prev_pos, pos - prev_pos));

		output.push_back(substring);

		prev_pos = ++pos;
	}

	output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

	return output;
}

