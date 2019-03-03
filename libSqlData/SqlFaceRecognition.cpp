#include <header.h>
#include "SqlFaceRecognition.h"
using namespace Regards::Sqlite;

CSqlFaceRecognition::CSqlFaceRecognition()
	: CSqlExecuteRequest(L"RegardsDB")
{

}


CSqlFaceRecognition::~CSqlFaceRecognition()
{
}

int CSqlFaceRecognition::GetCompatibleFace(const int & numFace)
{
	type = 1;
	faceCompatible = 0;
	ExecuteRequest("SELECT NumFaceCompatible FROM FACE_RECOGNITION WHERE NumFace = " + to_string(numFace));
	return faceCompatible;
}

bool CSqlFaceRecognition::MoveFaceRecognition(int numFace, int NewnumFaceCompatible)
{
	return (ExecuteRequestWithNoResult("Update FACE_RECOGNITION Set NumFaceCompatible = " + to_string(NewnumFaceCompatible) + " WHERE NumFace = " + to_string(numFace)) != -1) ? true : false;
}

bool CSqlFaceRecognition::UpdateFaceRecognition(int OldnumFaceCompatible, int NewnumFaceCompatible)
{
	return (ExecuteRequestWithNoResult("Update FACE_RECOGNITION Set NumFaceCompatible = " + to_string(NewnumFaceCompatible) + " WHERE NumFaceCompatible = " + to_string(OldnumFaceCompatible)) != -1) ? true : false;
}

bool CSqlFaceRecognition::InsertFaceRecognition(int numFace, int numFaceCompatible)
{
	return (ExecuteRequestWithNoResult("INSERT INTO FACE_RECOGNITION (NumFace, NumFaceCompatible) VALUES (" + to_string(numFace) + "," + to_string(numFaceCompatible) + ")") != -1) ? true : false;
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
	return (ExecuteRequest("DELETE FACE_RECOGNITION") != -1) ? true : false;
}

bool CSqlFaceRecognition::DeleteFaceRecognitionDatabase(int numFace)
{
	return (ExecuteRequest("DELETE FACE_RECOGNITION WHERE NumFace = " + to_string(numFace)) != -1) ? true : false;
}

int CSqlFaceRecognition::TraitementResult(CSqlResult * sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
		{
			if(type == 0)
			{
				switch (i)
				{
				case 0:
					listFace.push_back(sqlResult->ColumnDataInt(i));
					break;
				}
			}
			else if(type == 1)
			{
				switch (i)
				{
				case 0:
					faceCompatible = sqlResult->ColumnDataInt(i);
					break;
				}
			}
		}
		nbResult++;
	}
	return nbResult;
};