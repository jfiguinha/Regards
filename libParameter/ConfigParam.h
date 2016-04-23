#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <string.h>
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>

#include <iostream>
#include <fstream>
#include <vector>

using namespace rapidxml;
using namespace std;


class CConfigParam
{
public:
	CConfigParam();
	~CConfigParam();

	bool OpenFile(const wxString &configFile);
	bool SaveFile();


protected:

	virtual void LoadParameter(){};
	virtual void SaveParameter(){};

	const char* stralloc(const wxString & str);
	xml_node<>*  node(const wxString & name, const wxString & content = "");

	xml_document<> doc;
	wxString filename;
};

