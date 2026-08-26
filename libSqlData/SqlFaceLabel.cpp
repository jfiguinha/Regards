#include <header.h>
#include "SqlFaceLabel.h"
#include "SqlResult.h"
#include <SqlParameter.h>
using namespace Regards::Sqlite;

CSqlFaceLabel::CSqlFaceLabel()
	: CSqlExecuteRequest(L"RegardsDB"), type(0), numFace(0)
{
}

//--------------------------------------------------------
//Chargement de toutes les données d'un album
//--------------------------------------------------------
bool CSqlFaceLabel::InsertFaceLabel(const int& numFace, const wxString& faceName, const int& isSelectable)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFace));
	parameter.push_back(std::make_unique<CSqlString>(faceName));
	parameter.push_back(std::make_unique<CSqlInt>(isSelectable));
	return ExecuteSqlWithStatementNoResult("INSERT INTO FACE_NAME (numFace, faceName, isSelectable) VALUES (?,?,?)", parameter);
}


int CSqlFaceLabel::GetLastFaceNum()
{
	numFace = -1;
	type = 2;
	ExecuteRequest("SELECT NumFace FROM FACEPHOTO ORDER BY NumFace desc LIMIT 1");
	return numFace;
}


int CSqlFaceLabel::GetNumFace(const wxString& faceName)
{
	numFace = -1;
	type = 2;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(faceName));
	ExecuteSqlWithStatement("SELECT NumFace FROM FACE_NAME WHERE faceName = ?", parameter);
	return numFace;
}

int CSqlFaceLabel::GetFaceNumLabel(int idFace)
{
	numFace = -1;
	type = 2;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFace));
	ExecuteSqlWithStatement("SELECT NumFaceCompatible FROM FACE_RECOGNITION WHERE NumFace = ?", parameter);
	return numFace;
}

bool CSqlFaceLabel::UpdateNumFaceLabel(const int& numFace, const int& NewNumName)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(NewNumName));
	parameter.push_back(std::make_unique<CSqlInt>(numFace));
	return ExecuteSqlWithStatementNoResult("UPDATE FACE_NAME SET numFace = ? where numFace = ? ", parameter);
}

bool CSqlFaceLabel::UpdateFaceLabel(const int& numFace, const wxString& faceName)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFace));
	parameter.push_back(std::make_unique<CSqlString>(faceName));
	return ExecuteSqlWithStatementNoResult("UPDATE FACE_NAME SET faceName = ? where numFace = ?", parameter);
}

bool CSqlFaceLabel::UpdateFaceLabel(const int& numFace, const int& isSelectable)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(isSelectable));
	parameter.push_back(std::make_unique<CSqlInt>(numFace));
	return ExecuteSqlWithStatementNoResult("UPDATE FACE_NAME SET isSelectable = ? where numFace = ?", parameter);
}

vector<int> CSqlFaceLabel::GetFaceLabelAlone()
{
	listOfFace.clear();
	type = 1;
	ExecuteRequest("SELECT NumFace FROM FACE_NAME WHERE NumFace not in (select NumFace From FacePhoto)");
	return listOfFace;
}

vector<int> CSqlFaceLabel::GetAllFace()
{
	listOfFace.clear();
	type = 1;
	ExecuteRequest("SELECT NumFace FROM FACE_NAME");
	return listOfFace;
}

wxString CSqlFaceLabel::GetFaceName(int numFace)
{
	type = 0;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFace));
	ExecuteSqlWithStatement("SELECT faceName FROM FACE_NAME WHERE NumFace = ?", parameter);
	return faceName;
}

bool CSqlFaceLabel::DeleteFaceLabelDatabase(int numFace)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFace));
	return ExecuteSqlWithStatementNoResult("DELETE FROM FACE_NAME WHERE NumFace = ?", parameter);
}

bool CSqlFaceLabel::DeleteFaceLabelDatabase()
{
	return (ExecuteRequestWithNoResult("DELETE FROM FACE_NAME") != -1) ? true : false;
}

int CSqlFaceLabel::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		switch(type)
		{
			case 0:
				faceName = sqlResult->ColumnDataText(0);
				break;
			case 1:
				listOfFace.push_back(sqlResult->ColumnDataInt(0));
				break;
			case 2:
				numFace = sqlResult->ColumnDataInt(0);
				break;
		}
		nbResult++;
	}
	return nbResult;
}
