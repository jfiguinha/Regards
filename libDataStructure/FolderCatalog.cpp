#include <header.h>
#include "FolderCatalog.h"


CFolderCatalog::CFolderCatalog()
{

}


CFolderCatalog::~CFolderCatalog()
{

}

void CFolderCatalog::SetNumFolder(const int &numFolder)
{
	this->numFolder = numFolder;
}

int CFolderCatalog::GetNumFolder()
{
	return numFolder;
}

void CFolderCatalog::SetNumCatalog(const int &numCatalog)
{
	this->numCatalog = numCatalog;
}

int CFolderCatalog::GetNumCatalog()
{
	return numCatalog;
}

void CFolderCatalog::SetFolderPath(const wxString &folderPath)
{
	this->folderPath = folderPath;
}

wxString CFolderCatalog::GetFolderPath()
{
	return folderPath;
}

