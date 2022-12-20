#include <header.h>
#include "SqlFaceLabel.h"
#include "SqlResult.h"
using namespace Regards::Sqlite;

CSqlFaceLabel::CSqlFaceLabel()
	: CSqlExecuteRequest(L"RegardsDB")
{
}


CSqlFaceLabel::~CSqlFaceLabel()
{
}

//--------------------------------------------------------
//Chargement de toutes les données d'un album
//--------------------------------------------------------
bool CSqlFaceLabel::InsertFaceLabel(const int &numFace, const wxString &faceName, const int &isSelectable)
{
	return (ExecuteRequestWithNoResult("INSERT INTO FACE_NAME (numFace, faceName, isSelectable) VALUES (" + to_string(numFace) + ",'" + faceName + "'," + to_string(isSelectable) + ")") != -1) ? true : false;
}

int CSqlFaceLabel::GetNumFace(const wxString &faceName)
{
	numFace = -1;
	type = 2;
	ExecuteRequest("SELECT NumFace FROM FACE_NAME WHERE faceName = '" + faceName + "'");
	return numFace;
}

int CSqlFaceLabel::GetFaceNumLabel(int idFace)
{
	numFace = -1;
	type = 2;
	ExecuteRequest("SELECT NumFaceCompatible FROM FACE_RECOGNITION WHERE NumFace = " + to_string(idFace));
	return numFace;
}

bool CSqlFaceLabel::UpdateNumFaceLabel(const int &numFace, const int &NewNumName)
{
	return (ExecuteRequestWithNoResult("UPDATE FACE_NAME SET numFace = " + to_string(NewNumName) + " where numFace = " + to_string(numFace)) != -1) ? true : false;
}

bool CSqlFaceLabel::UpdateFaceLabel(const int &numFace, const wxString &faceName)
{
	return (ExecuteRequestWithNoResult("UPDATE FACE_NAME SET faceName = '" + faceName + "' where numFace = " + to_string(numFace)) != -1) ? true : false;
}

bool CSqlFaceLabel::UpdateFaceLabel(const int &numFace, const int &isSelectable)
{
	return (ExecuteRequestWithNoResult("UPDATE FACE_NAME SET isSelectable = "  + to_string(isSelectable) + " where numFace = " + to_string(numFace)) != -1) ? true : false;
}

vector<int> CSqlFaceLabel::GetFaceLabelAlone()
{
	listOfFace.clear();
	type = 1;
	ExecuteRequest("SELECT NumFace FROM FACE_NAME WHERE NumFace not in (select NumFace From FacePhoto)");
	return listOfFace;
}

wxString CSqlFaceLabel::GetFaceName(int numFace)
{
	type = 0;
	ExecuteRequest("SELECT faceName FROM FACE_NAME WHERE NumFace = " + to_string(numFace));
	return faceName;
}

bool CSqlFaceLabel::DeleteFaceLabelDatabase(int numFace)
{
	return (ExecuteRequestWithNoResult("DELETE FROM FACE_NAME where numFace = " + to_string(numFace)) != -1) ? true : false;
}

bool CSqlFaceLabel::DeleteFaceLabelDatabase()
{
	return (ExecuteRequestWithNoResult("DELETE FROM FACE_NAME") != -1) ? true : false;
}

int CSqlFaceLabel::TraitementResult(CSqlResult * sqlResult)
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
					faceName =  sqlResult->ColumnDataText(i);
					break;
				}
			}
			else if(type == 1)
			{
				switch (i)
				{
				case 0:
					listOfFace.push_back(sqlResult->ColumnDataInt(i));
					break;
				}
			}
			else if(type == 2)
			{
				switch (i)
				{
				case 0:
					numFace = sqlResult->ColumnDataInt(i);
					break;
				}
			}
		}
		nbResult++;
	}
	return nbResult;
}