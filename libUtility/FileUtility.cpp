#include "header.h"
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


wxString CFileUtility::GetTempFile(wxString filename, const bool &removeFile)
{
	wxString file;
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
#ifdef WIN32
	wxString tempFolder = documentPath + "\\temp";
    if (!wxMkDir(tempFolder)) {
#else
	wxString tempFolder = documentPath + "/temp";
    if (!wxMkDir(tempFolder,  wxS_DIR_DEFAULT)) {
#endif
	
		// handle the error here
	}
	else
	{
#ifdef WIN32
		file = tempFolder + "\\" + filename;
#else
		file = tempFolder + "/" + filename;
#endif

		if (removeFile)
		{
			if (wxFileExists(file))
				wxRemoveFile(file);
		}
	}
	return file;
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
#if defined(__APPLE__) && not defined(__LLVM__)

     printf("toto 1 GetResourcesFolderPath \n");

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

#ifdef WIN32
	documentPath.append("\\Regards");
#else
	documentPath.append("/Regards");
#endif

	if (!wxDir::Exists(documentPath))
	{
		wxDir::Make(documentPath);
	}
	return documentPath;
}

wxString CFileUtility::GetProgramFolderPath()
{
#ifdef WIN32
    wxString path = wxStandardPaths::Get().GetExecutablePath();
    return GetFolder(path);
#elif defined(__APPLE__) && not defined(__LLVM__)
    
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
