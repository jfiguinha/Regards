#include <header.h>
#include "SqlFindFacePhoto.h"
#include "SqlResult.h"
using namespace Regards::Sqlite;

CSqlFindFacePhoto::CSqlFindFacePhoto()
	: CSqlExecuteRequest(L"RegardsDB"), type(0)
{
}


CSqlFindFacePhoto::~CSqlFindFacePhoto()
{
}

std::vector<CFaceName> CSqlFindFacePhoto::GetListFaceNum(const wxString& photopath)
{
	wxString fullpath = photopath;
	fullpath.Replace("'", "''");
	type = 2;
	listFaceName.clear();
	ExecuteRequest("SELECT NumFace FROM FACEPHOTO WHERE fullpath = '" + fullpath + "'");
	return listFaceName;
}

std::vector<CFaceName> CSqlFindFacePhoto::GetListFaceName()
{
	type = 2;
	listFaceName.clear();
	ExecuteRequest("SELECT NumFace, FaceName, isSelectable FROM FACE_NAME Order By FaceName");
	return listFaceName;
}

std::vector<CFaceName> CSqlFindFacePhoto::GetListFaceNameSelectable()
{
	type = 2;
	listFaceName.clear();
	ExecuteRequest("SELECT NumFace, FaceName, isSelectable FROM FACE_NAME WHERE isSelectable = 1 and NumFace in (select DISTINCT NumFaceCompatible from FACE_RECOGNITION)");
	return listFaceName;
}

std::vector<CFaceName> CSqlFindFacePhoto::GetListFaceName(const wxString& photoPath)
{
	type = 2;
	listFaceName.clear();
	wxString fullpath = photoPath;
	fullpath.Replace("'", "''");
	ExecuteRequest(
		"SELECT NumFace, FaceName, isSelectable FROM FACE_NAME WHERE NumFace  in (select NumFaceCompatible from FACE_RECOGNITION where NumFace in (select NumFace from FACEPHOTO where FullPath = '"
		+ fullpath + "'))");
	return listFaceName;
}

std::vector<int> CSqlFindFacePhoto::GetListFaceToRecognize()
{
	type = 5;
	listFace.clear();
	ExecuteRequest(
		"SELECT FACEPHOTO.NumFace FROM FACEPHOTO WHERE FACEPHOTO.NumFace not in(SELECT DISTINCT NumFace From FACE_RECOGNITION)");
	return listNumFace;
}

std::vector<CFaceFilePath> CSqlFindFacePhoto::GetListPhotoFace(const int& numFace, const double& pertinence)
{
	type = 3;
	listFace.clear();
	wxString value = wxString::Format(wxT("%f"), pertinence / 100.0f);
	ExecuteRequest(
		"SELECT FACE_RECOGNITION.NumFace, FACEPHOTO.FullPath, NumPhoto  FROM FACE_RECOGNITION INNER JOIN FACEPHOTO ON FACEPHOTO.NumFace = FACE_RECOGNITION.NumFace INNER JOIN PHOTOS ON FACEPHOTO.FullPath = PHOTOS.FullPath where Pertinence > "
		+ value + " and NumFaceCompatible = " + to_string(numFace));
	//ExecuteRequest("SELECT FACE_RECOGNITION.NumFace, FACEPHOTO.FullPath, NumPhoto  FROM FACE_RECOGNITION INNER JOIN FACEPHOTO ON FACEPHOTO.NumFace = FACE_RECOGNITION.NumFace INNER JOIN PHOTOS ON FACEPHOTO.FullPath = PHOTOS.FullPath where NumFaceCompatible = " + to_string(numFace));
	return listFace;
}

std::vector<wxString> CSqlFindFacePhoto::GetPhotoListNotProcess()
{
	type = 0;
	listPhoto.clear();
	ExecuteRequest("SELECT FullPath FROM PHOTOS WHERE FullPath not in (select distinct FullPath FROM FACEPHOTO");
	return listPhoto;
}


int CSqlFindFacePhoto::TraitementResult(CSqlResult* sqlResult)
{
	CFaceName faceName;
	CFaceFilePath faceFilePath;
	int nbResult = 0;
	while (sqlResult->Next())
	{
		for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
		{
			if (type == 0)
			{
				switch (i)
				{
				case 0:
					listPhoto.push_back(sqlResult->ColumnDataText(i));
					break;
				default: ;
				}
			}
			else if (type == 2)
			{
				switch (i)
				{
				case 0:
					faceName.numFace = sqlResult->ColumnDataInt(i);
					break;
				case 1:
					faceName.faceName = sqlResult->ColumnDataText(i);
					break;
				case 2:
					faceName.isSelectable = sqlResult->ColumnDataInt(i);
					break;
				default: ;
				}
			}
			else if (type == 3)
			{
				switch (i)
				{
				case 0:
					faceFilePath.numFace = sqlResult->ColumnDataInt(i);
					break;
				case 1:
					faceFilePath.faceFilePath = sqlResult->ColumnDataText(i);
					break;
				case 2:
					faceFilePath.numPhoto = sqlResult->ColumnDataInt(i);
					break;
				default: ;
				}
			}
			else if (type == 5)
			{
				switch (i)
				{
				case 0:
					listNumFace.push_back(sqlResult->ColumnDataInt(i));
					break;
				default: ;
				}
			}
		}

		if (type == 2)
		{
			listFaceName.push_back(faceName);
		}
		else if (type == 3)
		{
			listFace.push_back(faceFilePath);
		}

		nbResult++;
	}
	return nbResult;
};
