#include <header.h>
//
//  SqlVersion.cpp
//  Regards.libSqlData
//
//  Created by figuinha jacques on 29/09/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#include "SqlVersion.h"
#include "SqlResult.h"
#include <ConvertUtility.h>
#include <SqlParameter.h>
using namespace Regards::Sqlite;

CSqlVersion::CSqlVersion(CSqlLib* _sqlLibTransaction, const bool& useTransaction)
	: CSqlExecuteRequest(L"RegardsDB")
{
	this->m_transaction = _sqlLibTransaction;
	this->m_useTransaction = useTransaction;
	typeResult = 0;
	result = "";
}

bool CSqlVersion::InsertVersion(const wxString& version)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(version));
	return ExecuteSqlWithStatementNoResult("INSERT INTO VERSION (libelle) VALUES (?)", parameter);
}

bool CSqlVersion::UpdateVersion(const wxString& version, const wxString& oldValue)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(version));
	parameter.push_back(std::make_unique<CSqlString>(oldValue));
	return ExecuteSqlWithStatementNoResult("UPDATE VERSION SET libelle = ? WHERE libelle = ?", parameter);
}

bool CSqlVersion::DeleteVersion()
{
	return (ExecuteRequestWithNoResult("DELETE FROM VERSION") != -1) ? true : false;
}

wxString CSqlVersion::GetVersion()
{
	typeResult = 0;
	ExecuteRequest("SELECT libelle FROM VERSION");
	cout << "Version : " << CConvertUtility::ConvertToStdString(result) << endl;
	return result;
}


int CSqlVersion::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		switch (typeResult)
		{
		case 0:
			result = sqlResult->ColumnDataText(0);
			break;
		default: ;
		}

		nbResult++;
	}
	return nbResult;
}
