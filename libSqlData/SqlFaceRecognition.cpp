#include <header.h>
#include "SqlFaceRecognition.h"
#include "SqlResult.h"
#include <SqlParameter.h>
using namespace Regards::Sqlite;

CSqlFaceRecognition::CSqlFaceRecognition()
	: CSqlExecuteRequest(L"RegardsDB")
{
}


int CSqlFaceRecognition::GetCompatibleFace(const int& numFace)
{
	type = 1;
	faceCompatible = 0;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFace));
	ExecuteSqlWithStatement("SELECT NumFaceCompatible FROM FACE_RECOGNITION WHERE NumFace = ?", parameter);
	return faceCompatible;
}

bool CSqlFaceRecognition::MoveFaceRecognition(int numFace, int NewnumFaceCompatible)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFace));
	parameter.push_back(std::make_unique<CSqlInt>(NewnumFaceCompatible));
	return ExecuteSqlWithStatementNoResult("Update FACE_RECOGNITION Set NumFaceCompatible = ? WHERE NumFace = ?", parameter);
}

bool CSqlFaceRecognition::UpdateFaceRecognition(int OldnumFaceCompatible, int NewnumFaceCompatible)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(NewnumFaceCompatible));
	parameter.push_back(std::make_unique<CSqlInt>(OldnumFaceCompatible));
	return ExecuteSqlWithStatementNoResult("Update FACE_RECOGNITION Set NumFaceCompatible = ? WHERE NumFaceCompatible = ?", parameter);
}

bool CSqlFaceRecognition::InsertFaceRecognition(int numFace, int numFaceCompatible)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFace));
	parameter.push_back(std::make_unique<CSqlInt>(numFaceCompatible));

	return ExecuteSqlWithStatementNoResult("INSERT INTO FACE_RECOGNITION (NumFace, NumFaceCompatible) VALUES (?,?)", parameter);
}

std::vector<int> CSqlFaceRecognition::GetUniqueFace()
{
	type = 0;
	listFace.clear();
	ExecuteRequest("SELECT distinct NumFace FROM FACE_RECOGNITION");
	return listFace;
}

bool CSqlFaceRecognition::DeleteFaceRecognitionDatabase()
{
	return (ExecuteRequest("DELETE FROM FACE_RECOGNITION") != -1) ? true : false;
}

bool CSqlFaceRecognition::DeleteFaceRecognitionDatabase(int numFace)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFace));
	return ExecuteSqlWithStatementNoResult("DELETE FROM FACE_RECOGNITION WHERE NumFace = ?", parameter);
}

int CSqlFaceRecognition::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		switch (type)
		{
		case 0:
			listFace.push_back(sqlResult->ColumnDataInt(0));
			break;
		case 1:
			faceCompatible = sqlResult->ColumnDataInt(0);
			break;
		}
		nbResult++;
	}
	return nbResult;
};
