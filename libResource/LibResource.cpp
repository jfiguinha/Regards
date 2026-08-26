// DllResource.cpp : définit les fonctions exportées pour l'application DLL.
//
#include "header.h"
#include "LibResource.h"
#include <SqlResource.h>
#include <SqlLibResource.h>
#include <SqlEngine.h>
#include <ConvertUtility.h>
#include <FileUtility.h>
#include <ParamInit.h>
#include <libPicture.h>
#include <RegardsConfigParam.h>
#include <wx/filename.h>
#include <wx/textfile.h>
using namespace Regards::Sqlite;

wxString CLibResource::GetPhotoCancel()
{
	wxFileName file(CFileUtility::GetResourcesFolderPath(), "photo_cancel.png");
	return file.GetFullPath();
}

bool CLibResource::InitializeSQLServerDatabase(const wxString& folder)
{
	auto libResource = new CSqlLibResource(true, true);
	wxFileName file(folder, "resource.db");
	printf("ResourceDB %s \n", CConvertUtility::ConvertToStdString(file.GetFullPath()).c_str());
	return CSqlEngine::Initialize(file.GetFullPath(), L"ResourceDB", libResource);
}

void CLibResource::KillSqlEngine()
{
	CSqlEngine::kill(L"ResourceDB");
}
wxString CLibResource::ReadFile(const wxString& fileName)
{
	wxString text;

	//Read data from filepath
	// open the file
	wxTextFile tfile;
	tfile.Open(fileName);

	// read the first line
	text.append(tfile.GetFirstLine());
	text.append("\n");
	// read all lines one by one
	// until the end of the file
	while (!tfile.Eof())
	{
		text.append(tfile.GetNextLine());
		text.append("\n");
	}

	return text;
}


vector<wxString> CLibResource::GetSavePictureFormat()
{
	CSqlResource sqlResource;
	return sqlResource.GetSavePictureFormat();
}

vector<wxString> CLibResource::GetSavePictureExtension()
{
	CSqlResource sqlResource;
	return sqlResource.GetSavePictureExtension();
}

wxImage CLibResource::CreatePictureFromSVG(const wxString& idName, const int& buttonWidth, const int& buttonHeight)
{
	CSqlResource sqlResource;
	wxFileName resourcePath = wxFileName(CFileUtility::GetResourcesFolderPath());
	resourcePath.AppendDir("vector");
	resourcePath.SetFullName(sqlResource.GetVectorFromFile(idName));
	return Regards::Picture::CLibPicture::CreatePictureFromSVGFilename(resourcePath.GetFullPath(), buttonWidth, buttonHeight);
}

wxString CLibResource::LoadBitmapFromResource(const wxString& idName)
{
	wxFileName resourcePath = wxFileName(CFileUtility::GetResourcesFolderPath());
	resourcePath.AppendDir("bitmap");
	resourcePath.SetFullName(idName + ".png");
	return resourcePath.GetFullPath();
}

wxString CLibResource::LoadStringFromResource(const wxString& idName, const int& idLang)
{
	CSqlResource sqlResource;
	int numLanguage = idLang;

	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		numLanguage = config->GetNumLanguage();

	return sqlResource.GetLibelle(idName, numLanguage);
}

wxString CLibResource::GetVector(const wxString& idName)
{
	CSqlResource sqlResource;
	wxFileName resourcePath = wxFileName(CFileUtility::GetResourcesFolderPath());
	resourcePath.AppendDir("vector");
	resourcePath.SetFullName(sqlResource.GetVectorFromFile(idName));
	return ReadFile(resourcePath.GetFullPath());
}

int CLibResource::GetExtensionId(const wxString& extension)
{
	CSqlResource sqlResource;
	int id = sqlResource.GetExtensionId(extension);
	return id;
}

wxString CLibResource::GetOpenGLShaderProgram(const wxString& idName)
{
	CSqlResource sqlResource;
	wxFileName resourcePath = wxFileName(CFileUtility::GetResourcesFolderPath());
	resourcePath.AppendDir("shader");
	resourcePath.AppendDir("opengl");
	resourcePath.SetFullName(sqlResource.GetOpenGLFromFile(idName));
	return ReadFile(resourcePath.GetFullPath());
}

wxString CLibResource::GetOpenCLUcharProgram(const wxString& idName)
{
	CSqlResource sqlResource;
	wxFileName resourcePath = wxFileName(CFileUtility::GetResourcesFolderPath());
	resourcePath.AppendDir("shader");
	resourcePath.AppendDir("opencl_uchar");
	resourcePath.SetFullName(sqlResource.GetOpenCLUcharFromFile(idName));
	return ReadFile(resourcePath.GetFullPath());
}
