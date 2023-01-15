#include <header.h>
#include "SqlPhotoGPS.h"
#include "SqlResult.h"
using namespace Regards::Sqlite;

CSqlPhotoGPS::CSqlPhotoGPS(CSqlLib* _sqlLibTransaction, const bool& useTransaction)
	: CSqlExecuteRequest(L"RegardsDB"), numPhoto(0), numFolderId(0), nbResult(0)
{
	this->_sqlLibTransaction = _sqlLibTransaction;
	this->useTransaction = useTransaction;
}


CSqlPhotoGPS::~CSqlPhotoGPS()
{
}

bool CSqlPhotoGPS::InsertPhoto(const int& numPhoto, const wxString& filepath, const int& numFolderId)
{
	wxString fullpath = filepath;
	fullpath.Replace("'", "''");
	return (ExecuteRequestWithNoResult(
		       "INSERT INTO PHOTOSGPS (NumPhoto, FullPath, NumFolderId) VALUES (" + to_string(numPhoto) + ", '" +
		       fullpath + "'," + to_string(numFolderId) + ")") != -1)
		       ? true
		       : false;
}

bool CSqlPhotoGPS::DeletePhoto(const int64_t& numPhoto)
{
	return (ExecuteRequestWithNoResult("DELETE FROM PHOTOSGPS WHERE NumPhoto = " + to_string(numPhoto)) != -1)
		       ? true
		       : false;
}


int CSqlPhotoGPS::GetFirstPhoto(int& numPhoto, wxString& filepath, int& numFolderId)
{
	ExecuteRequest("SELECT NumPhoto, FullPath, NumFolderId FROM PHOTOSGPS");
	numPhoto = this->numPhoto;
	filepath = this->filepath;
	numFolderId = this->numFolderId;
	return nbResult;
}

int CSqlPhotoGPS::TraitementResult(CSqlResult* sqlResult)
{
	nbResult = 0;
	while (sqlResult->Next())
	{
		for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
		{
			switch (i)
			{
			case 0:
				numPhoto = sqlResult->ColumnDataInt(i);
				break;
			case 1:
				filepath = sqlResult->ColumnDataText(i);
				break;
			case 2:
				numFolderId = sqlResult->ColumnDataInt(i);
				break;
			default: ;
			}
		}

		nbResult++;
		break;
	}
	return nbResult;
}
