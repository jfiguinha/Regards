#include "SqlCriteria.h"
using namespace Regards::Sqlite;

CSqlCriteria::CSqlCriteria(CSqlLib * _sqlLibTransaction, const bool &useTransaction)
	: CSqlExecuteRequest(L"RegardsDB")
{
	criteriaId = -1;
	this->_sqlLibTransaction = _sqlLibTransaction;
	this->useTransaction = useTransaction;
}


CSqlCriteria::~CSqlCriteria()
{
}

bool CSqlCriteria::InsertCriteria(const int64_t &numCatalog, const int64_t &numCategorie, const wxString & libelle)
{
	return (ExecuteRequestWithNoResult("INSERT INTO CRITERIA (NumCatalog,NumCategorie,Libelle) VALUES (" + to_string(numCatalog) + "," + to_string(numCategorie) + ",'" + libelle + "')") != -1) ? true : false;;
}

int64_t CSqlCriteria::GetCriteriaId(const int64_t &numCatalog, const int64_t &numCategorie, const wxString & libelle)
{
	ExecuteRequest("SELECT NumCriteria FROM CRITERIA WHERE NumCatalog = " + to_string(numCatalog) + " and NumCategorie = " + to_string(numCategorie) + " and Libelle = '" + libelle + "'");
	return criteriaId;
}

int64_t CSqlCriteria::GetOrInsertCriteriaId(const int64_t &numCatalog, const int64_t &numCategorie, const wxString & libelle, bool &isNew)
{
	isNew = false;
	wxString _value = libelle;
	int64_t id = GetCriteriaId(numCatalog, numCategorie, _value);
	if (id == -1)
	{
		isNew = true;
		InsertCriteria(numCatalog, numCategorie, _value);
		id = GetLastId();
	}
	return id;
}

bool CSqlCriteria::DeleteCriteriaAlone()
{
	return (ExecuteRequestWithNoResult("Delete from CRITERIA where NumCriteria not in (select NumCriteria From PhotosCRITERIA)") != -1) ? true : false;
}

bool CSqlCriteria::DeletePhotoCriteria(const int64_t &numCatalog, const int64_t &numCriteria)
{
	return (ExecuteRequestWithNoResult("DELETE FROM PHOTOS WHERE NumCatalog = " + to_string(numCatalog) + " and NumCriteria = " + to_string(numCriteria)) != -1) ? true : false;
}

int64_t CSqlCriteria::GetNumCategorieId(const int64_t &numCatalog, const wxString & libelle)
{
	ExecuteRequest("SELECT NumCategorie FROM CRITERIA WHERE NumCatalog = " + to_string(numCatalog) + "  and Libelle = '" + libelle + "'");
	return criteriaId;
}

bool CSqlCriteria::DeleteCriteriaFolder(const int64_t &numFolder)
{
	wxString sql;
	sql = "DELETE FROM CRITERIA WHERE NumCriteria in (";
	sql.append("SELECT DISTINCT NumCriteria FROM PHOTOS as PH INNER JOIN PHOTOSCRITERIA as CR ON CR.NumPHOTO = PH.NumPHOTO WHERE PH.NumFolderCatalog = " + to_string(numFolder) + " AND NumCriteria NOT IN(");
	sql.append("SELECT DISTINCT NumCriteria FROM PHOTOS as PH INNER JOIN PHOTOSCRITERIA as CR ON CR.NumPHOTO = PH.NumPHOTO WHERE PH.NumFolderCatalog != " + to_string(numFolder) + "))");
	return (ExecuteRequestWithNoResult(sql) != -1) ? true : false;
}

bool CSqlCriteria::DeleteCriteriaCatalog(const int64_t &numCatalog)
{
	return (ExecuteRequestWithNoResult("DELETE FROM CRITERIA WHERE NumCatalog = " + to_string(numCatalog)) != -1) ? true : false;
}

int CSqlCriteria::TraitementResult(CSqlResult * sqlResult)
{
	int nbResult = 0;
	criteriaId = -1;

	while (sqlResult->Next())
	{
		for (int i = 0; i < sqlResult->GetColumnCount(); i++)
		{
			switch (i)
			{
			case 0:
				criteriaId = sqlResult->ColumnDataInt(i);
				break;
			}
		}
		nbResult++;
	}
	return nbResult;
}