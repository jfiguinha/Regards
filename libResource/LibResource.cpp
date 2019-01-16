// DllResource.cpp : définit les fonctions exportées pour l'application DLL.
//

#include "LibResource.h"
#include "SqlResource.h"
#include "SqlLibResource.h"
#include <SqlEngine.h>
#include <ConvertUtility.h>
#include <RegardsConfigParam.h>
#include "ParamInit.h"
using namespace Regards::Sqlite;

void CLibResource::InitializeSQLServerDatabase(const wxString & folder)
{
	CSqlLibResource * libResource = new CSqlLibResource(true, true);
	wxString filename = folder;
#ifdef WIN32
	filename.append(L"\\resource.db");
#else
    filename.append(L"/resource.db");
#endif
    printf("ResourceDB %s \n", CConvertUtility::ConvertToUTF8(filename));
	CSqlEngine::Initialize(filename, L"ResourceDB", libResource);
}

void CLibResource::KillSqlEngine()
{
	CSqlEngine::kill(L"ResourceDB");
}

wxString CLibResource::LoadExifNameFromResource(const wxString &id)
{
	CSqlResource sqlResource;
	wxString libelle = sqlResource.GetExifLibelle(id);
	return libelle;
}

CPictureData * CLibResource::LoadBitmapFromResource(const wxString &idName)
{
	CSqlResource sqlResource;
	return sqlResource.GetBitmap(idName);
}

wxString CLibResource::LoadStringFromResource(const wxString &idName, const int &idLang)
{
	CSqlResource sqlResource;
    CRegardsConfigParam * config = (CRegardsConfigParam*)CParamInit::getInstance();
    int numLanguage = idLang;
    if(config != nullptr)
        numLanguage = config->GetNumLanguage();
	wxString libelle = sqlResource.GetLibelle(idName, numLanguage);
	return libelle;
}

wxString CLibResource::GetVector(const wxString &idName)
{
    CSqlResource sqlResource;
    wxString libelle = sqlResource.GetVector(idName);
    return libelle;
}

int CLibResource::GetExtensionId(const wxString &extension)
{
    CSqlResource sqlResource;
    int id = sqlResource.GetExtensionId(extension);
    return id;
}

wxString CLibResource::GetShaderProgram(const wxString &idName)
{
	CSqlResource sqlResource;
	wxString program = sqlResource.GetText(idName);
	//printf("Program : %s \n",CConvertUtility::ConvertToUTF8(program));
	return program;
}