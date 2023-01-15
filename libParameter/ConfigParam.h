#pragma once
#include <rapidxml/rapidxml.hpp>

using namespace rapidxml;


class CConfigParam
{
public:
	CConfigParam();
	virtual ~CConfigParam();

	bool OpenFile(const wxString& configFile);
	bool SaveFile();

protected:
	virtual void LoadParameter()
	{
	};

	virtual void SaveParameter()
	{
	};

	const char* stralloc(const wxString& str);
	xml_node<>* node(const wxString& name, const wxString& content = "");

	xml_document<> doc;
	wxString filename;
};
