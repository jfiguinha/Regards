#pragma once


class CFileUtility
{
public:
	CFileUtility(void);
	~CFileUtility(void);
    
	static wxString GetFileName(const wxString& szFilePath);
	static wxString GetFileExtension(const wxString& szFilePath);
	static wxString GetFolder(const wxString& szFilePath);
	static wxString GetProgramFolderPath();
    static wxString GetResourcesFolderFontPathWithExt(const wxString& szFile);
	static wxString GetResourcesFolderPath();
    static wxString GetResourcesFolderPathWithExt(const wxString & folder);
	static wxString GetFileTime(const wxString& szFileName);
	static wxString GetDocumentFolderPath();
    static wxString GetDocumentFolderPathWithFilename(const wxString & folder);
	static wxString GetThumbnailPath(const wxString& path);
	static wxString GetVideoThumbnailPath(const wxString& path, const int& numFrame);
	static wxString GetFaceThumbnailPath(const int& numFace);
	static wxString GetTempFile(wxString filename, const bool& removeFile = true);

	static wxString GetFaceZScorePath(const int& numFace);
	static wxString GetOrCreateSubFolder(const wxString& folderName);
	static std::string GetFullpathModel(const wxString& filename);
private:
	static wxString GetTempFile(wxString filename, wxString folder, const bool& removeFile);
#if defined(__APPLE__)
	static wxString GetAppleExecFolderPath(const wxString& folder);
#endif
};
