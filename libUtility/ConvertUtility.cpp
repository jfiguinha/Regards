#include "header.h"
#include "ConvertUtility.h"
#include <wx/base64.h>
#include <wx/filename.h>
using namespace std;

CConvertUtility::CConvertUtility(void)
{
}


CConvertUtility::~CConvertUtility(void)
{
}



wxString CConvertUtility::ConvertToBase64(const wxString& s)
{
	wxCharBuffer utf8 = s.ToUTF8();

	return wxBase64Encode(
		utf8.data(),
		strlen(utf8.data())
	);
}

wxString CConvertUtility::ConvertFromBase64(const wxString& s)
{
	wxMemoryBuffer buffer = wxBase64Decode(s);

	wxString decoded(
		static_cast<const char*>(buffer.GetData()),
		wxConvUTF8,
		buffer.GetDataLen()
	);

	return decoded;
}

wxString CConvertUtility::GeneratePath(
	const wxString& firstPart,
	const wxString& secondPart)
{
	wxFileName file(firstPart, secondPart);
	return file.GetFullPath();
}

std::wstring CConvertUtility::ConvertToStdWstring(const wxString& s)
{
	return s.ToStdWstring();
}

int CConvertUtility::StringToInt(const wxString& s)
{
	long value;

	if (!s.ToLong(&value))
		throw std::invalid_argument("Invalid integer");

	return static_cast<int>(value);
}

wxString CConvertUtility::GenerateValue(const int& value, const size_t & n)
{
	string str = to_string(value);
	int precision = n - std::min(n, str.size());
	std::string s = std::string(precision, '0').append(str);
	return s;
}

std::string CConvertUtility::ConvertToStdString(const wxString& s)
{
	return s.utf8_string();
}

wxString CConvertUtility::GetTimeLibelle(const int& secs)
{
	if (secs <= 0)
		return "00:00:00";

	uint32_t hh = std::min<uint32_t>(secs / 3600, 99);
	uint32_t mm = std::min<uint32_t>((secs % 3600) / 60, 99);
	uint32_t ss = std::min<uint32_t>((secs % 60), 99);

	return wxString::Format("%02u:%02u:%02u", hh, mm, ss);
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
