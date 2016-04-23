#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <string>
#include <vector>
using namespace std;

class CConvertUtility
{
public:
	CConvertUtility(void);
	~CConvertUtility(void);

	static wxString GetTimeLibelle(const int &timePosition);
	static std::vector<wxString> split(const wxString & s, char seperator);

};

