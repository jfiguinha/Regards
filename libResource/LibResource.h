#pragma once

class CPictureData;

class CLibResource
{
public:
	CLibResource()
	{
	};

	~CLibResource()
	{
	};
	static bool InitializeSQLServerDatabase(const wxString& folder);
	static void KillSqlEngine();
	static wxString LoadBitmapFromResource(const wxString& idName);
	static wxString LoadStringFromResource(const wxString& idName, const int& idLang);
	static wxString GetOpenGLShaderProgram(const wxString& idName);
	static wxString GetOpenCLUcharProgram(const wxString& idName);
	static wxString GetVector(const wxString& idName);

	static wxImage CreatePictureFromSVG(const wxString& idName, const int& buttonWidth, const int& buttonHeight);
	static int GetExtensionId(const wxString& extension);
	static vector<wxString> GetSavePictureFormat();
	static vector<wxString> GetSavePictureExtension();
	static wxString GetPhotoCancel();
	static wxString ReadFile(const wxString& fileName);
};
