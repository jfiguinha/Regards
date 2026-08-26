#include <header.h>
#include "SqlPhotosWithoutThumbnail.h"
#include "SqlResult.h"
using namespace Regards::Sqlite;


CSqlPhotosWithoutThumbnail::CSqlPhotosWithoutThumbnail()
	: CSqlExecuteRequest(L"RegardsDB")
{
	typeResult = 0;
	priority = 0;
	photoList = nullptr;
}


int CSqlPhotosWithoutThumbnail::GetPhotoElement()
{
	nbElement = 0;
	typeResult = 0;
	ExecuteRequest("SELECT count(*) as nbResult from PHOTOSWIHOUTTHUMBNAIL_VIEW");
	return nbElement;
}

void CSqlPhotosWithoutThumbnail::GetPhotoList(std::deque<wxString> * photoList, int nbElement)
{
	photoList->clear();
	this->photoList = photoList;
	if (photoList != nullptr)
	{
		typeResult = 1;
		if (nbElement > 0)
			ExecuteRequest("SELECT DISTINCT FullPath from PHOTOSWIHOUTTHUMBNAIL_VIEW LIMIT " + to_string(nbElement));
		else
			ExecuteRequest("SELECT DISTINCT FullPath from PHOTOSWIHOUTTHUMBNAIL_VIEW");
	}
}

int CSqlPhotosWithoutThumbnail::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		switch (typeResult)
		{
		case 0:
			nbElement = sqlResult->ColumnDataInt(0);
			break;

		case 1:
			photoList->push_back(sqlResult->ColumnDataText(0));
			break;
		default: ;
		}
		nbResult++;
	}
	return nbResult;
}
