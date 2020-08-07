#include "header.h"
#include "ConvertUtility.h"
using namespace std;

CConvertUtility::CConvertUtility(void)
{
}


CConvertUtility::~CConvertUtility(void)
{
}

const std::wstring CConvertUtility::ConvertToStdWstring(const wxString & s)
{
#ifdef __WXGTK__
    std::string chaine = ConvertToStdString(s);
    std::wstring ws;
    ws.assign(chaine.begin(), chaine.end());
    return ws;
#else
    return s.ToStdWstring();
#endif
}

const std::string CConvertUtility::ConvertToStdString(const wxString & s)
{
#ifdef __WXGTK__
    return std::string(ConvertToUTF8(s)); 
#else
    return s.ToStdString();
#endif    
}

const char * CConvertUtility::ConvertToUTF8(const wxString & s)
{
    /*
#ifdef __WXGTK__    
    const char* str = (const char*)s.mb_str(wxConvUTF8);
    return s.ToUTF8();
#else
    return s.mb_str(wxConvUTF8);
#endif
 * */
    return s.ToUTF8();
}

wxString CConvertUtility::GetTimeLibelle(const int &timePosition)
{
	wxString libelle = L"";
	//wchar_t buf[255];
	int hour = timePosition / 3600;
	int minute = (timePosition % 3600) / 60;
	int seconds = timePosition % 60;

	wxString shour;
	shour << hour;
	wxString sminute;
	sminute << minute;
	wxString sseconds;
	sseconds << seconds;

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

