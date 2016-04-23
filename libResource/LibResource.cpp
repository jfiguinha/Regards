// DllResource.cpp : définit les fonctions exportées pour l'application DLL.
//

#include "LibResource.h"
#include "SqlResource.h"
#include "SqlLibResource.h"
#include <SqlEngine.h>
using namespace Regards::Sqlite;

void CLibResource::InitializeSQLServerDatabase(const wxString & folder)
{
	CSqlLibResource * libResource = new CSqlLibResource();
	wxString filename = folder;
#ifdef WIN32
	filename.append(L"\\resource.db");
#else
    filename.append(L"/resource.db");
#endif
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
	wxString libelle = sqlResource.GetLibelle(idName, idLang);
	return libelle;
}

wxString CLibResource::GetVector(const wxString &idName)
{
    CSqlResource sqlResource;
    wxString libelle = sqlResource.GetVector(idName);
    return libelle;
}

wxString CLibResource::GetShaderProgram(const wxString &idName)
{
	CSqlResource sqlResource;
	wxString program = sqlResource.GetText(idName);
	//printf("Program : %s \n",program.ToStdString().c_str());
	return program;
}