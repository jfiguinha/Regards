#pragma once

class CConvertUtility
{
public:
	CConvertUtility(void);
	~CConvertUtility(void);
	static int ToInt(const wxString& s)
	{
		long value{};
		return s.ToLong(&value) ? value : 0;
	};
	static wxString GetTimeLibelle(const int& timePosition);
	static std::vector<wxString> split(const wxString& s, char seperator);
    static wxString ConvertToBase64(const wxString& s);
    static wxString ConvertFromBase64(const wxString& s);
	static wxString GenerateValue(const int& value, const size_t & n);
	static std::string ConvertToStdString(const wxString& s);
	static std::wstring ConvertToStdWstring(const wxString& s);
	static wxString GeneratePath(const wxString& firstPart, const wxString& secondPart);
	static int StringToInt(const wxString& s);
};
