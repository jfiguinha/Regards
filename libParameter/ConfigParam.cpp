#include "header.h"
#include "ConfigParam.h"
#include <fstream>
#include <rapidxml/rapidxml.hpp>
#include <ConvertUtility.h>
using namespace rapidxml;

CConfigParam::CConfigParam()
{
}


CConfigParam::~CConfigParam()
{
	doc.clear();
}

bool CConfigParam::OpenFile(const wxString& configFile)
{
	filename = configFile;
	doc.clear();
	// Read the xml file into a vector
	ifstream theFile(CConvertUtility::ConvertToStdString(filename));
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	if (buffer.size() > 0)
	{
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc.parse<0>(&buffer[0]);
		LoadParameter();
	}
	return true;
}

bool CConfigParam::SaveFile()
{
	SaveParameter();
	return true;
}

// allocate the given string from xml document memory
// to ensure that it says alive for the duration of xml
// operations
const char* CConfigParam::stralloc(const wxString& str)
{
	return doc.allocate_string(str.c_str());
}

// create a node
xml_node<>* CConfigParam::node(const wxString& name, const wxString& content)
{
	return doc.allocate_node(node_element, stralloc(name), stralloc(content));
}
