//
//  SqlVersion.cpp
//  Regards.libSqlData
//
//  Created by figuinha jacques on 29/09/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#include "SqlVersion.h"
#include <ConvertUtility.h>
using namespace Regards::Sqlite;

CSqlVersion::CSqlVersion(CSqlLib * _sqlLibTransaction, const bool &useTransaction)
: CSqlExecuteRequest(L"RegardsDB")
{
    this->_sqlLibTransaction = _sqlLibTransaction;
    this->useTransaction = useTransaction;
	typeResult = 0;
    result = "";
}


CSqlVersion::~CSqlVersion()
{
}

bool CSqlVersion::InsertVersion(const wxString & version)
{
    return (ExecuteRequestWithNoResult("INSERT INTO VERSION (libelle) VALUES ('" + version + "')") != -1) ? true : false;
}

bool CSqlVersion::UpdateVersion(const wxString & version, const wxString &oldValue)
{
    return (ExecuteRequestWithNoResult("UPDATE VERSION SET libelle = '" + version + "' WHERE libelle = '" + oldValue + "'") != -1) ? true : false;
}

bool CSqlVersion::DeleteVersion()
{
    return (ExecuteRequestWithNoResult("DELETE FROM VERSION") != -1) ? true : false;
}

wxString CSqlVersion::GetVersion()
{
    typeResult = 0;
    ExecuteRequest("SELECT libelle FROM VERSION");
    printf("Version : %s \n", CConvertUtility::ConvertToUTF8(result));
    return result;
}


int CSqlVersion::TraitementResult(CSqlResult * sqlResult)
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
                        result = sqlResult->ColumnDataText(i);
                        break;
                }
            }
        }
        nbResult++;
    }
    return nbResult;
}