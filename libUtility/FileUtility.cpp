#include "header.h"
#include "FileUtility.h"
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/dir.h>
#include "ConvertUtility.h"

CFileUtility::CFileUtility(void)
{
}


CFileUtility::~CFileUtility(void)
{
}

wxString CFileUtility::GetTempFile(wxString filename, wxString folder, const bool& removeFile)
{
	wxString file;

#ifdef WIN32
	file = folder + "\\" + filename;
#else
	file = folder + "/" + filename;
#endif

	try
	{
		if (removeFile)
		{
			if (wxFileExists(file))
				wxRemoveFile(file);
		}
	}
	catch (...)
	{
	}

	return file;
}

wxString CFileUtility::GetTempFile(wxString filename, const bool& removeFile)
{
	wxString file;
	wxString documentPath = GetDocumentFolderPath();
#ifdef WIN32
	wxString tempFolder = documentPath + "\\temp";
#else
	wxString tempFolder = documentPath + "/temp";
#endif
	if (!wxDir::Exists(tempFolder))
	{
		wxDir::Make(tempFolder);
	}

#ifdef WIN32
	file = tempFolder + "\\" + filename;
#else
    file = tempFolder + "/" + filename;
#endif

	try
	{
		if (removeFile)
		{
			if (wxFileExists(file))
				wxRemoveFile(file);
		}
	}
	catch (...)
	{
	}


	return file;
}


wxString CFileUtility::GetFileExtension(const wxString& szFilePath)
{
	auto filename = wxFileName(szFilePath);
	return filename.GetExt();
}

wxString CFileUtility::GetFileName(const wxString& szFilePath)
{
	auto filename = wxFileName(szFilePath);
	return filename.GetFullName();
}

wxString CFileUtility::GetFolder(const wxString& szFilePath)
{
	auto filename = wxFileName(szFilePath);
	return filename.GetPath();
}

wxString CFileUtility::GetResourcesFolderPath()
{
#if defined(__APPLE__) && not defined(__LLVM__)

     //printf("toto 1 GetResourcesFolderPath \n");

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

wxString CFileUtility::GetFaceZScorePath(const int& numFace)
{
	wxString documentPath = GetDocumentFolderPath();
#ifdef WIN32
	documentPath.append("\\Face");
#else
	documentPath.append("/Face");
#endif

	if (!wxDir::Exists(documentPath))
	{
		wxDir::Make(documentPath);
	}

#ifdef WIN32
	documentPath.append("\\" + to_string(numFace) + ".bin");
#else
	documentPath.append("/" + to_string(numFace) + ".bin");
#endif

	return documentPath;
}

wxString CFileUtility::GetFaceThumbnailPath(const int& numFace)
{
	wxString documentPath = GetDocumentFolderPath();
#ifdef WIN32
	documentPath.append("\\Face");
#else
	documentPath.append("/Face");
#endif

	if (!wxDir::Exists(documentPath))
	{
		wxDir::Make(documentPath);
	}

#ifdef WIN32
	documentPath.append("\\" + to_string(numFace) + ".jpg");
#else
	documentPath.append("/" + to_string(numFace) + ".jpg");
#endif

	return documentPath;
}

wxString CFileUtility::GetVideoThumbnailPath(const wxString& path, const int& numFrame)
{
	wxString ext;
	wxString dirpath;
	wxString name;
	wxFileName::SplitPath(path, &dirpath, &name, &ext);

	wxString documentPath = GetDocumentFolderPath();
#ifdef WIN32
	documentPath.append("\\ThumbnailVideo");
#else
	documentPath.append("/ThumbnailVideo");
#endif

	if (!wxDir::Exists(documentPath))
	{
		wxDir::Make(documentPath);
	}

#ifdef WIN32
	documentPath.append("\\" + name + "_" + to_string(numFrame) + ".jpg");
#else
	documentPath.append("/" + name + "_" + to_string(numFrame) + ".jpg");
#endif

	return documentPath;
}

wxString CFileUtility::GetThumbnailPath(const wxString& path)
{
	wxString ext;
	wxString dirpath;
	wxString name;
	wxFileName::SplitPath(path, &dirpath, &name, &ext);

	wxString documentPath = GetDocumentFolderPath();
#ifdef WIN32
	documentPath.append("\\Thumbnail");
#else
	documentPath.append("/Thumbnail");
#endif

	if (!wxDir::Exists(documentPath))
	{
		wxDir::Make(documentPath);
	}

#ifdef WIN32
	documentPath.append("\\" + name + ".jpg");
#else
	documentPath.append("/" + name + ".jpg");
#endif

	return documentPath;
}

wxString CFileUtility::GetDocumentFolderPath()
{
	wxStandardPathsBase& stdp = wxStandardPaths::Get();
	wxString documentPath = stdp.GetDocumentsDir();


#ifdef WIN32
	//documentPath = "d:\\Regards";

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

wxString CFileUtility::GetFileTime(const wxString& szFileName)
{
	wxStructStat strucStat;
	wxStat(szFileName, &strucStat);
	//wxFileOffset filelen = strucStat.st_size;
	wxDateTime last_modified_time(strucStat.st_mtime);
	return last_modified_time.Format(wxT("%Y.%m.%d"));
}
