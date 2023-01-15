#pragma once

class CFolderCatalog
{
public:
	CFolderCatalog();
	~CFolderCatalog();

	void SetNumFolder(const int& numFolder);
	int GetNumFolder();

	void SetNumCatalog(const int& numCatalog);
	int GetNumCatalog();

	void SetFolderPath(const wxString& folderPath);
	wxString GetFolderPath();

private:
	int numFolder;
	int numCatalog;
	wxString folderPath;
};

using FolderCatalogVector = vector<CFolderCatalog>;
