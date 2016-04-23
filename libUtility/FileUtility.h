#pragma once
#include <string>
#include <vector>
using namespace std;

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


class CFileUtility
{
public:
	CFileUtility(void);
	~CFileUtility(void);
	static wxString GetFileName(const wxString &szFilePath);
	static wxString GetFileExtension(const wxString &szFilePath);
	static wxString GetFolder(const wxString &szFilePath);
	static wxString GetProgramFolderPath();
    static wxString GetResourcesFolderPath();
	static wxString GetFileTime(const wxString &szFileName);
	static wxString GetDocumentFolderPath();
};