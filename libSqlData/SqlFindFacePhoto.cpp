#include <header.h>
#include "SqlFindFacePhoto.h"
#include "SqlResult.h"
#include <SqlParameter.h>
using namespace Regards::Sqlite;

CSqlFindFacePhoto::CSqlFindFacePhoto()
	: CSqlExecuteRequest(L"RegardsDB"), type(0)
{
}


std::vector<int> CSqlFindFacePhoto::GetListFaceNum(const wxString& photopath)
{
	type = 0;
	listNumFace.clear();
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(photopath));
	ExecuteSqlWithStatement("SELECT NumFace FROM FACEPHOTO WHERE fullpath = ?", parameter);
	return listNumFace;
}

std::vector<CFaceName> CSqlFindFacePhoto::GetListFaceName()
{
	type = 1;
	listFaceName.clear();
	ExecuteRequest("SELECT NumFace, FaceName, isSelectable FROM FACE_NAME Order By FaceName");
	return listFaceName;
}

std::vector<CFaceName> CSqlFindFacePhoto::GetListFaceNameSelectable()
{
	type = 1;
	listFaceName.clear();
	ExecuteRequest("SELECT NumFace, FaceName, isSelectable FROM FACE_NAME WHERE isSelectable = 1 and NumFace in (select DISTINCT NumFaceCompatible from FACE_RECOGNITION)");
	return listFaceName;
}

std::vector<CFaceName> CSqlFindFacePhoto::GetListFaceName(const wxString& photoPath)
{
	type = 1;
	listFaceName.clear();
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(photoPath));
	ExecuteSqlWithStatement("SELECT NumFace, FaceName, isSelectable FROM FACE_NAME WHERE NumFace in (select NumFaceCompatible from FACE_RECOGNITION where NumFace in (select NumFace from FACEPHOTO where FullPath = ?))", parameter);
	return listFaceName;
}

int CSqlFindFacePhoto::GetNbListFaceToRecognize()
{
	type = 2;
	nbFaceList = 0;
	ExecuteRequest("SELECT count(*) as NbElement FROM FACEPHOTO WHERE FACEPHOTO.NumFace not in (SELECT DISTINCT NumFace From FACE_RECOGNITION)");
	return nbFaceList;
}

std::vector<int> CSqlFindFacePhoto::GetListFaceToRecognize()
{
	type = 0;
	listNumFace.clear();
	ExecuteRequest("SELECT FACEPHOTO.NumFace FROM FACEPHOTO WHERE FACEPHOTO.NumFace not in (SELECT DISTINCT NumFace From FACE_RECOGNITION)");
	return listNumFace;
}

std::vector<CFaceFilePath> CSqlFindFacePhoto::GetListPhotoFace(const int& numFace, const double& pertinence)
{
	type = 3;
	listFace.clear();
	wxString value = wxString::Format(wxT("%f"), pertinence / 100.0f);
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(value));
	parameter.push_back(std::make_unique<CSqlInt>(numFace));
	ExecuteSqlWithStatement("SELECT FACE_RECOGNITION.NumFace, FACEPHOTO.FullPath, NumPhoto FROM FACE_RECOGNITION INNER JOIN FACEPHOTO ON FACEPHOTO.NumFace = FACE_RECOGNITION.NumFace INNER JOIN PHOTOS ON FACEPHOTO.FullPath = PHOTOS.FullPath where Pertinence > ? and NumFaceCompatible = ?", parameter);
	return listFace;
}

std::vector<CFaceFilePath> CSqlFindFacePhoto::GetListAllPhotoFace(const double& pertinence)
{
	type = 3;
	listFace.clear();
	wxString value = wxString::Format(wxT("%f"), pertinence / 100.0f);
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(value));
	ExecuteSqlWithStatement("SELECT FACE_RECOGNITION.NumFace, FACEPHOTO.FullPath, NumPhoto  FROM FACE_RECOGNITION INNER JOIN FACEPHOTO ON FACEPHOTO.NumFace = FACE_RECOGNITION.NumFace INNER JOIN PHOTOS ON FACEPHOTO.FullPath = PHOTOS.FullPath where Pertinence > ?", parameter);
	return listFace;
}

std::vector<wxString> CSqlFindFacePhoto::GetPhotoListNotProcess()
{
	type = 4;
	listPhoto.clear();
	ExecuteRequest("SELECT FullPath FROM PHOTOS WHERE FullPath not in (select distinct FullPath FROM FACEPHOTO");
	return listPhoto;
}


int CSqlFindFacePhoto::TraitementResult(CSqlResult* sqlResult)
{
	
	
	int nbResult = 0;
	while (sqlResult->Next())
	{
		switch (type)
		{
		case 0:
			listNumFace.push_back(sqlResult->ColumnDataInt(0));
			break;
		case 1:
		{
			CFaceName faceName;
			faceName.numFace = sqlResult->ColumnDataInt(0);
			faceName.faceName = sqlResult->ColumnDataText(1);
			faceName.isSelectable = sqlResult->ColumnDataInt(2);
			listFaceName.push_back(faceName);
			break;
		}
		case 2:
			nbFaceList = sqlResult->ColumnDataInt(0);
			break;
		case 3:
		{
			CFaceFilePath faceFilePath;
			faceFilePath.numFace = sqlResult->ColumnDataInt(0);
			faceFilePath.faceFilePath = sqlResult->ColumnDataText(1);
			faceFilePath.numPhoto = sqlResult->ColumnDataInt(2);
			listFace.push_back(faceFilePath);
			break;
		}
		case 4:
			listPhoto.push_back(sqlResult->ColumnDataText(0));
			break;
		}

		nbResult++;
	}
	return nbResult;
};
