#include "header.h"
#include "SqlResource.h"
#include <lz4.h>
#include <libPicture.h>
#include <ImageLoadingFormat.h>
#include "PictureData.h"
#include <wx/textfile.h>
#include <FileUtility.h>
#include "SqlResult.h"
#include <ConvertUtility.h>
#include <wx/filename.h>
#include <SqlParameter.h>
using namespace std;
using namespace Regards::Sqlite;
using namespace Regards::Picture;

CSqlResource::CSqlResource(CSqlLib* _sqlLibTransaction, const bool& useTransaction)
	: CSqlExecuteRequest(L"ResourceDB"), typeResult(0), id(0)
{
	this->m_transaction = _sqlLibTransaction;
	this->m_useTransaction = useTransaction;
}

vector<wxString> CSqlResource::GetSavePictureFormat()
{
	list.clear();
	typeResult = 1;
	ExecuteRequest("SELECT Label FROM SavePictureFormat");
	return list;
}

vector<wxString> CSqlResource::GetSavePictureExtension()
{
	list.clear();
	typeResult = 1;
	ExecuteRequest("SELECT Extension FROM SavePictureFormat");
	return list;
}

wxString CSqlResource::GetOpenCLUcharFromFile(const wxString& idName)
{
	text = "";
	typeResult = 0;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(idName));
	ExecuteSqlWithStatement("SELECT FilePath FROM OpenclUcharResource WHERE idName = ?", parameter);
	return text;
}

wxString CSqlResource::GetOpenGLFromFile(const wxString& idName)
{
	text = "";
	typeResult = 0;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(idName));
	ExecuteSqlWithStatement("SELECT FilePath FROM OpenglResource WHERE idName = ?", parameter);
	return text;
}

wxString CSqlResource::GetVectorFromFile(const wxString& idName)
{
	text = "";
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(idName));
	ExecuteSqlWithStatement("SELECT FilePath FROM VectorResource WHERE idName = ?", parameter);
	return text;
}


int CSqlResource::GetExtensionId(const wxString& extension)
{
	id = 0;
	typeResult = 2;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(extension));
	ExecuteSqlWithStatement("SELECT pictureId FROM FileExtension WHERE extension = ?", parameter);
	return id;
}

wxString CSqlResource::GetLibelle(const wxString& idName, const int& idLang)
{
	typeResult = 0;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(idName));
	parameter.push_back(std::make_unique<CSqlInt>(idLang));
	ExecuteSqlWithStatement("SELECT libelle FROM StringResource WHERE idName = ? and language = ?", parameter);
	return text;
}


int CSqlResource::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		switch (typeResult)
		{
		case 0:
			text = sqlResult->ColumnDataText(0);
			break;
		case 1:
			list.push_back(sqlResult->ColumnDataText(0));
			break;
		case 2:
			id = sqlResult->ColumnDataInt(0);
			break;
		default:;
		}
	}

	return nbResult;
}
