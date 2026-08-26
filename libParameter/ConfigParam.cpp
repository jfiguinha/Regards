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
    xmlBuffer.clear();

    try
    {
        std::ifstream file(CConvertUtility::ConvertToStdString(filename), std::ios::binary);

        if (!file.is_open())
            return false;

        xmlBuffer.assign(std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>());

        if (xmlBuffer.empty())
            return false;

        xmlBuffer.push_back('\0');

        doc.parse<0>(xmlBuffer.data());

        LoadParameter();

        return true;
    }
    catch (const rapidxml::parse_error& e)
    {
        wxMessageBox("XML Parse Error : " + wxString(e.what()));

        return false;
    }
    catch (...)
    {
        wxMessageBox("Unable to open config file : " + filename);

        return false;
    }
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
