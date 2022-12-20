#pragma once


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
	static wxString GetThumbnailPath(const wxString & path);
	static wxString GetVideoThumbnailPath(const wxString & path, const int &numFrame);
	static wxString GetFaceThumbnailPath(const int &numFace);
    static wxString GetTempFile(wxString filename, const bool &removeFile = true);
	static wxString GetTempFile(wxString filename, wxString folder, const bool& removeFile = true);
	static wxString GetFaceZScorePath(const int& numFace);
};