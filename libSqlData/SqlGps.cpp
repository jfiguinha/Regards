#include <header.h>
//
//  SqlGps.cpp
//  Regards.libSqlData
//
//  Created by figuinha jacques on 29/09/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#include "SqlGps.h"
#include "SqlResult.h"
#include <SqlParameter.h>
using namespace Regards::Sqlite;

CSqlGps::CSqlGps(CSqlLib* _sqlLibTransaction, const bool& useTransaction)
	: CSqlExecuteRequest(L"RegardsDB")
{
	this->m_transaction = _sqlLibTransaction;
	this->m_useTransaction = useTransaction;
	typeResult = 0;
	photogpsVector = nullptr;
}

bool CSqlGps::InsertGps(const wxString& filepath, const wxString& latitude, const wxString& longitude)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(filepath));
	parameter.push_back(std::make_unique<CSqlString>(latitude));
	parameter.push_back(std::make_unique<CSqlString>(longitude));
	return ExecuteSqlWithStatementNoResult("INSERT INTO PHOTOGPS (FullPath, latitude, longitude) VALUES (?, ?, ?)", parameter);
}

bool CSqlGps::UpdateGps(const wxString& filepath, const wxString& latitude, const wxString& longitude)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(latitude));
	parameter.push_back(std::make_unique<CSqlString>(longitude));
	parameter.push_back(std::make_unique<CSqlString>(filepath));
	return ExecuteSqlWithStatementNoResult("UPDATE PHOTOGPS SET latitude = ?, longitude = ? WHERE FullPath = ?", parameter);
}

bool CSqlGps::DeleteGps(const wxString& filepath)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(filepath));
	return ExecuteSqlWithStatementNoResult("DELETE FROM PHOTOGPS WHERE FullPath = ?", parameter);
}

void CSqlGps::GetGps(PhotoGpsVector* photogpsVector, const wxString& filepath)
{
	typeResult = 0;
	if (photogpsVector == nullptr)
		return;

	this->photogpsVector = photogpsVector;
	photogpsVector->clear();
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(filepath));
	ExecuteSqlWithStatement("SELECT id, FullPath, latitude, longitude FROM PHOTOGPS where FullPath = ?", parameter);
}


int CSqlGps::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		switch(typeResult)
		{
			case 0:
			{
				CPhotoGps photoGps;
				photoGps.SetId(sqlResult->ColumnDataInt(0));
				photoGps.SetPath(sqlResult->ColumnDataText(1));
				photoGps.SetLatitude(sqlResult->ColumnDataText(2));
				photoGps.SetLongitude(sqlResult->ColumnDataText(3));
				photogpsVector->push_back(photoGps);
				break;
			}
		}
		nbResult++;
	}
	return nbResult;
}
