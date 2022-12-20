#pragma once

class CPictureData;

class CLibResource
{
public:
	CLibResource(){};
	~CLibResource(){};
    static void InitializeSQLServerDatabase(const wxString & folder);
    static void KillSqlEngine();
    static wxString LoadBitmapFromResource(const wxString &idName);
    static wxString LoadStringFromResource(const wxString &idName, const int &idLang);
	static wxString LoadExifNameFromResource(const wxString &id);
    static wxString GetOpenGLShaderProgram(const wxString &idName);
	static wxString GetOpenCLFloatProgram(const wxString& idName);
	static wxString GetOpenCLUcharProgram(const wxString& idName);
    static wxString GetVector(const wxString &idName);
    static wxImage CreatePictureFromSVGFilename(const wxString& filename, const int& buttonWidth, const int& buttonHeight);
	static wxImage CreatePictureFromSVG(const wxString& idName, const int& buttonWidth, const int& buttonHeight);
    static int GetExtensionId(const wxString &extension);
    static vector<wxString> GetSavePictureFormat();
    static vector<wxString> GetSavePictureExtension();
	static wxString GetPhotoCancel();
};
