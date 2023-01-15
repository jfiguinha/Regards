#include <header.h>
#include "SqlPhotosWithoutThumbnail.h"
#include "SqlFindPhotos.h"
#include "SqlThumbnailVideo.h"
#include "libPicture.h"
#include "SqlResult.h"
using namespace Regards::Sqlite;
using namespace Regards::Picture;

CSqlPhotosWithoutThumbnail::CSqlPhotosWithoutThumbnail()
	: CSqlExecuteRequest(L"RegardsDB")
{
	typeResult = 0;
	priority = 0;
	photoList = nullptr;
}

CSqlPhotosWithoutThumbnail::~CSqlPhotosWithoutThumbnail()
{
}

void CSqlPhotosWithoutThumbnail::GeneratePhotoList()
{
	ExecuteRequestWithNoResult("DELETE FROM PHOTOSWIHOUTTHUMBNAIL");
	ExecuteRequestWithNoResult(
		"INSERT INTO PHOTOSWIHOUTTHUMBNAIL (FullPath, Priority, ProcessStart) SELECT FullPath, 1, 0 From PHOTOSSEARCHCRITERIA WHERE FullPath not in ( SELECT FullPath From PHOTOSTHUMBNAIL) ORDER BY CreateDate desc, GeoGps");
	ExecuteRequestWithNoResult(
		"INSERT INTO PHOTOSWIHOUTTHUMBNAIL (FullPath, Priority, ProcessStart) SELECT FullPath, 1, 0 FROM VIDEOTHUMBNAIL WHERE FullPath in (select FullPath from PHOTOS where MultiFiles = 1)  GROUP BY fullpath HAVING COUNT(*) < 20");

	//UpdateVideoList();
}

void CSqlPhotosWithoutThumbnail::UpdateVideoList()
{
	PhotosVector photosVector;
	CSqlFindPhotos findPhotos;
	findPhotos.GetAllVideo(&photosVector);


	// for(CPhotos photo : photosVector)

	tbb::parallel_for(0, static_cast<int>(photosVector.size()), 1, [=](int i)
	{
		CSqlThumbnailVideo sqlThumbnailVideo;
		CLibPicture libPicture;
		CPhotos photo = photosVector[i];
		if (libPicture.TestIsVideo(photo.GetPath()) || libPicture.TestIsPDF(photo.GetPath()) || libPicture.
			TestIsAnimation(photo.GetPath()))
		{
			if (sqlThumbnailVideo.GetNbThumbnail(photo.GetPath()) == 0)
			{
				wxString fullpath = photo.GetPath();
				fullpath.Replace("'", "''");
				if (!IsPathFind(photo.GetPath()))
					ExecuteRequestWithNoResult(
						"INSERT INTO PHOTOSWIHOUTTHUMBNAIL (FullPath, Priority, ProcessStart) VALUES ('" + fullpath +
						"', 1, 0)");
			}
		}
	});
}

void CSqlPhotosWithoutThumbnail::UpdatePhotoList()
{
	ExecuteRequestWithNoResult(
		"DELETE FROM PHOTOSWIHOUTTHUMBNAIL WHERE FullPath in (SELECT FullPath From PHOTOSTHUMBNAIL)");
	ExecuteRequestWithNoResult(
		"DELETE FROM PHOTOSWIHOUTTHUMBNAIL WHERE FullPath in (SELECT distinct FullPath From VIDEOTHUMBNAIL)");
	//ExecuteRequestWithNoResult("INSERT INTO PHOTOSWIHOUTTHUMBNAIL (FullPath, Priority, ProcessStart) SELECT FullPath, 1, 0 From PHOTOS where (ExtensionId >= 100 or ExtensionId in (5,) and FullPath not in (SELECT distinct FullPath From VIDEOTHUMBNAIL)");
	//UpdateVideoList();
}

void CSqlPhotosWithoutThumbnail::GetPhotoList(vector<wxString>* photoList)
{
	this->photoList = photoList;
	typeResult = 0;
	ExecuteRequest("SELECT DISTINCT FullPath from PHOTOSWIHOUTTHUMBNAIL WHERE ProcessStart = 0");
}

bool CSqlPhotosWithoutThumbnail::IsPathFind(const wxString& photo)
{
	typeResult = 3;
	fullpath = "";
	wxString path = photo;
	path.Replace("'", "''");
	ExecuteRequest("SELECT FullPath from PHOTOSWIHOUTTHUMBNAIL WHERE FullPath = '" + path + "'");
	if (fullpath == photo)
		return true;
	return false;
}

void CSqlPhotosWithoutThumbnail::InsertPhotoPriority(const wxString& photoPath)
{
	wxString filename = photoPath;
	filename.Replace("'", "''");
	typeResult = 1;
	//ExecuteRequest("SELECT distinct Priority from PHOTOSWIHOUTTHUMBNAIL ORDER BY Priority desc LIMIT 1");
	//priority++;
	ExecuteRequestWithNoResult(
		"UPDATE PHOTOSWIHOUTTHUMBNAIL SET Priority = " + to_string(priority) + " WHERE FullPath = '" + filename + "'");
}

void CSqlPhotosWithoutThumbnail::InsertProcessStart(const wxString& photoPath)
{
	wxString filename = photoPath;
	filename.Replace("'", "''");

	ExecuteRequestWithNoResult("UPDATE PHOTOSWIHOUTTHUMBNAIL SET ProcessStart = 1 WHERE FullPath = '" + filename + "'");
}

int CSqlPhotosWithoutThumbnail::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		if (typeResult == 0)
		{
			for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
			{
				switch (i)
				{
				case 0:
					photoList->push_back(sqlResult->ColumnDataText(i));
					break;
				default: ;
				}
			}

			nbResult++;
		}
		else if (typeResult == 1)
		{
			for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
			{
				switch (i)
				{
				case 0:
					priority = sqlResult->ColumnDataInt(i);
					break;
				default: ;
				}
			}

			nbResult++;
		}
		else if (typeResult == 3)
		{
			for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
			{
				switch (i)
				{
				case 0:
					fullpath = sqlResult->ColumnDataText(i);
					break;
				default: ;
				}
			}

			nbResult++;
		}
	}
	return nbResult;
}
