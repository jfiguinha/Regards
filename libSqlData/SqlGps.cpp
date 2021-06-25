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
using namespace Regards::Sqlite;

CSqlGps::CSqlGps(CSqlLib * _sqlLibTransaction, const bool &useTransaction)
: CSqlExecuteRequest(L"RegardsDB")
{
    this->_sqlLibTransaction = _sqlLibTransaction;
    this->useTransaction = useTransaction;
	typeResult = 0;
	photogpsVector = nullptr;
}


CSqlGps::~CSqlGps()
{
}

bool CSqlGps::InsertGps(const wxString & filepath, const wxString &latitude, const wxString &longitude)
{
    wxString fullpath = filepath;
    fullpath.Replace("'", "''");
    return (ExecuteRequestWithNoResult("INSERT INTO PHOTOGPS (FullPath, latitude, longitude) VALUES ('" + filepath + "', '" + latitude + "', '" + longitude + "')") != -1) ? true : false;
}

bool CSqlGps::UpdateGps(const wxString & filepath, const wxString &latitude, const wxString &longitude)
{
    wxString fullpath = filepath;
    fullpath.Replace("'", "''");
    return (ExecuteRequestWithNoResult("UPDATE PHOTOGPS SET latitude = '" + latitude + "', longitude = '" + longitude + "' WHERE FullPath = '" + fullpath + "'") != -1) ? true : false;
}

bool CSqlGps::DeleteGps(const wxString & filepath)
{
    wxString fullpath = filepath;
    fullpath.Replace("'", "''");
    return (ExecuteRequestWithNoResult("DELETE FROM PHOTOGPS WHERE FullPath = '" + fullpath + "'") != -1) ? true : false;
}

void CSqlGps::GetGps(PhotoGpsVector * photogpsVector, const wxString & filepath)
{
    wxString fullpath = filepath;
    fullpath.Replace("'", "''");
    typeResult = 0;
    this->photogpsVector = photogpsVector;
    ExecuteRequest("SELECT id, FullPath, latitude, longitude FROM PHOTOGPS where FullPath = '" + fullpath + "'");
}


int CSqlGps::TraitementResult(CSqlResult * sqlResult)
{
    int nbResult = 0;
    while (sqlResult->Next())
    {
        if (typeResult == 0)
        {
            CPhotoGps photoGps;
            
            for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
            {
                switch (i)
                {
                    case 0:
                        photoGps.SetId(sqlResult->ColumnDataInt(i));
                        break;
                    case 1:
                        photoGps.SetPath(sqlResult->ColumnDataText(i));
                        break;
                    case 2:
                        photoGps.SetLatitude(sqlResult->ColumnDataText(i));
                        break;
                    case 3:
                        photoGps.SetLongitude(sqlResult->ColumnDataText(i));
                        break;
                    default: ;
                }
            }
            photogpsVector->push_back(photoGps);
        }
        nbResult++;
    }
    return nbResult;
}