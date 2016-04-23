#include "FileUtility.h"
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/dir.h>

CFileUtility::CFileUtility(void)
{
}


CFileUtility::~CFileUtility(void)
{
}

wxString CFileUtility::GetFileExtension(const wxString &szFilePath)
{
	wxFileName filename = wxFileName(szFilePath.c_str());
	return filename.GetExt();
}

wxString CFileUtility::GetFileName(const wxString &szFilePath)
{
	wxFileName filename = wxFileName(szFilePath.c_str());
	return filename.GetFullName();
}

wxString CFileUtility::GetFolder(const wxString &szFilePath)
{
	wxFileName filename = wxFileName(szFilePath.c_str());
	return filename.GetPath();
}

wxString CFileUtility::GetResourcesFolderPath()
{
#ifdef __APPLE__

    wxString exeFolderMacOs = ".app/Contents/";
    wxString path = wxStandardPaths::Get().GetExecutablePath();
    int index = path.Find(exeFolderMacOs);
    wxString realPath;
    realPath.append(path.begin(),path.begin() + index + exeFolderMacOs.size());
    realPath.append("Resources");
    
    return realPath;

#else

  	wxString path = GetFolder(wxStandardPaths::Get().GetExecutablePath());
	#ifdef WIN32
		path.append("\\Resources");
	#else
		path.append("/Resources");
	#endif
	return path;  

#endif
}

wxString CFileUtility::GetDocumentFolderPath()
{
	wxStandardPathsBase& stdp = wxStandardPaths::Get();
	wxString documentPath = stdp.GetDocumentsDir();
#ifndef __APPLE__

#ifdef WIN32
	documentPath.append("\\Regards");
#else
	documentPath.append("/Regards");
#endif

	if (!wxDir::Exists(documentPath))
	{
		wxDir::Make(documentPath);
	}

#endif
	return documentPath;
}

wxString CFileUtility::GetProgramFolderPath()
{
#ifdef WIN32
    wxString path = wxStandardPaths::Get().GetExecutablePath();
    return GetFolder(path);
#elif __APPLE
    
    wxString exeFolderMacOs = ".app/Contents/";
    wxString path = wxStandardPaths::Get().GetExecutablePath();
    int index = path.Find(exeFolderMacOs);
    wxString realPath;
    realPath.append(path.begin(),path.begin() + index + exeFolderMacOs.size());
    realPath.append("MacOS");
    
    return realPath;
#else 
    wxString path = wxStandardPaths::Get().GetExecutablePath();
    return GetFolder(path);
#endif
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

wxString CFileUtility::GetFileTime(const wxString &szFileName)
{
	wxStructStat strucStat;
	wxStat(szFileName.c_str(), &strucStat);
	//wxFileOffset filelen = strucStat.st_size;
	wxDateTime last_modified_time(strucStat.st_mtime);
	return last_modified_time.Format(wxT("%Y.%m.%d"));
}
