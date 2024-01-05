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
    type = 0;
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
    type = 0;
	return (ExecuteRequestWithNoResult("DELETE FROM PHOTOSGPS WHERE NumPhoto = " + to_string(numPhoto)) != -1)
		       ? true
		       : false;
}


bool CSqlPhotoGPS::DeleteListOfPhoto(const vector<wxString>& listPhoto)
{

	for (int i = 0; i < listPhoto.size(); i++)
	{
		DeletePhoto(i);
	}

	return false;
}

 int CSqlPhotoGPS::GetListPhoto(GpsPhotosVector * photoGpsVec)
 {
    type = 1;
    m_photoGpsVec = photoGpsVec;
	ExecuteRequest("SELECT NumPhoto, FullPath, NumFolderId FROM PHOTOSGPS Limit 1");
	return nbResult;   
 }

int CSqlPhotoGPS::GetFirstPhoto(int& numPhoto, wxString& filepath, int& numFolderId)
{
    type = 0;
	ExecuteRequest("SELECT NumPhoto, FullPath, NumFolderId FROM PHOTOSGPS Limit 1");
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
        switch (type)
		{
            case 0:
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
            case 1:
            {
                GpsPhoto gpsPhoto;
                for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
                {
                    switch (i)
                    {
                    case 0:
                        gpsPhoto.numPhoto = sqlResult->ColumnDataInt(i);
                        break;
                    case 1:
                        gpsPhoto.filepath = sqlResult->ColumnDataText(i);
                        break;
                    case 2:
                        gpsPhoto.numFolderId = sqlResult->ColumnDataInt(i);
                        break;
                    default: ;
                    }
                }
                m_photoGpsVec->push_back(gpsPhoto);
                nbResult++;
            }
            break;
        }
        
        if(type == 0)
            break;
	}
	return nbResult;
}
