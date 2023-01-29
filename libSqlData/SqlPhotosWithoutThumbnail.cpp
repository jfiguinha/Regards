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

int CSqlPhotosWithoutThumbnail::GetPhotoElement()
{
	nbElement = 0;
	typeResult = 1;
	ExecuteRequest("SELECT count(*) from PHOTOSWIHOUTTHUMBNAIL WHERE FullPath not in (SELECT FullPath From PHOTOSTHUMBNAIL)");
	return nbElement;
}

void CSqlPhotosWithoutThumbnail::GetPhotoList(vector<wxString>* photoList, int nbElement)
{
	this->nbElement = nbElement;
	this->photoList = photoList;
	typeResult = 0;
	ExecuteRequest("SELECT DISTINCT FullPath from PHOTOSWIHOUTTHUMBNAIL LIMIT " + to_string(nbElement));
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
					nbElement = priority = sqlResult->ColumnDataInt(i);
					break;
				default: ;
				}
			}

			nbResult++;
		}
	}
	return nbResult;
}
