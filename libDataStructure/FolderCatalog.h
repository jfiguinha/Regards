#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <string>
#include <vector>
using namespace std;

class CFolderCatalog
{
public:
	CFolderCatalog();
	~CFolderCatalog();

	void SetNumFolder(const int &numFolder);
	int GetNumFolder();

	void SetNumCatalog(const int &numCatalog);
	int GetNumCatalog();

	void SetFolderPath(const wxString &folderPath);
	wxString GetFolderPath();

    
    
private:
	int numFolder;
	int numCatalog;
	wxString folderPath;

};

typedef vector<CFolderCatalog> FolderCatalogVector;
