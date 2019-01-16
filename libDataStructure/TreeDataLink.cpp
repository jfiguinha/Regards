#include "TreeDataLink.h"

CTreeDataLink::CTreeDataLink(void)
{
    type = 2;
	linktype = 0;
    id = 0;
}


CTreeDataLink::~CTreeDataLink(void)
{
}

void CTreeDataLink::SetLinkType(const int & type)
{
    linktype = type;
}

int CTreeDataLink::GetLinkType()
{
    return linktype;
}

void CTreeDataLink::SetLinkPath(const wxString & value)
{
    linkPath = value;
}

wxString CTreeDataLink::GetLinkPath()
{
    return linkPath;
}

void CTreeDataLink::SetId(const int & id)
{
    this->id = id;
}

int CTreeDataLink::GetId()
{
    return id;
}