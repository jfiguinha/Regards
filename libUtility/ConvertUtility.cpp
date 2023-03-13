#include "header.h"
#include "ConvertUtility.h"
using namespace std;

CConvertUtility::CConvertUtility(void)
{
}


CConvertUtility::~CConvertUtility(void)
{
}

wxString CConvertUtility::GeneratePath(const wxString& firstPart, const wxString& secondPart)
{
#if defined(WIN32)
	return firstPart + "\\" + secondPart;

#else
	return firstPart + "/" + secondPart;
#endif
}

const std::wstring CConvertUtility::ConvertToStdWstring(const wxString& s)
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

const std::string CConvertUtility::ConvertToStdString(const wxString& fileName)
{
#ifdef __APPLE__
    return fileName.ToStdString();
#elif defined(WIN32)
	const std::wstring ws = fileName.ToStdWstring();
	const std::string s(ws.begin(), ws.end());
	return s;
#else
    return std::string(ConvertToUTF8(fileName)); 
#endif
}

const char* CConvertUtility::ConvertToUTF8(const wxString& s)
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


wxString CConvertUtility::GetTimeLibelle(const int& secs)
{
	char timestring[9];
	printf("GetTimeLibelle %d secs \n", secs);
	if(secs > 0)
	{
		uint32_t hh = secs / 3600;
		uint32_t mm = (secs % 3600) / 60;
		uint32_t ss = (secs % 3600) % 60;
		if(hh > 100)
			hh = 99;
		if(mm > 100)
			mm = 99;
		if(ss > 100)
			ss = 99;
		sprintf(timestring, "%02d:%02d:%02d", hh, mm, ss);
	}
	else
	{
		strcpy(timestring, "00:00:00");
	}

	return wxString(timestring);
}

std::vector<wxString> CConvertUtility::split(const wxString& s, char seperator)
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
