#pragma once

class CConvertUtility
{
public:
	CConvertUtility(void);
	~CConvertUtility(void);

	static wxString GetTimeLibelle(const int& timePosition);
	static std::vector<wxString> split(const wxString& s, char seperator);
	static const char* ConvertToUTF8(const wxString& s);
    static wxString ConvertToBase64(const wxString& s);
    static wxString ConvertFromBase64(const wxString& s);
	static wxString GenerateValue(const int& value, const size_t & n);
	static const std::string ConvertToStdString(const wxString& s);
	static const std::wstring ConvertToStdWstring(const wxString& s);
	static wxString GeneratePath(const wxString& firstPart, const wxString& secondPart);
};
