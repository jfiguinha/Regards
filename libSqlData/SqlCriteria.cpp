#include <header.h>
#include "SqlResult.h"
#include "SqlCriteria.h"
#include <SqlParameter.h>
#include <appcontext.h>
extern AppContext application_context;
using namespace Regards::Sqlite;

CSqlCriteria::CSqlCriteria(CSqlLib* _sqlLibTransaction, const bool& useTransaction)
	: CSqlExecuteRequest(L"RegardsDB")
{
	criteriaId = -1;

	this->m_transaction = _sqlLibTransaction;
	this->m_useTransaction = useTransaction;
}

wxString CSqlCriteria::GetCriteriaLibelle(const int& criteriaId)
{
	type = 2;
	libelle = "";
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(criteriaId));
	ExecuteSqlWithStatement("select libelle from criteria where NumCriteria = ?", parameter);
	return libelle;
}

deque<int> CSqlCriteria::GetListCriteriaToGeolocalize()
{
	type = 1;
	listCriteriaToGeolocalize.clear();
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(application_context.special_key + "%"));
	ExecuteSqlWithStatement("select NumCriteria from criteria where libelle like ?", parameter);
	return listCriteriaToGeolocalize;
}

bool CSqlCriteria::InsertCriteria(const int64_t& numCatalog, const int64_t& numCategorie, const wxString& libelle)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCategorie));
	parameter.push_back(std::make_unique<CSqlInt>(numCatalog));
	parameter.push_back(std::make_unique<CSqlString>(libelle));
	return ExecuteSqlWithStatementNoResult("INSERT INTO CRITERIA (NumCategorie,NumCatalog,Libelle) VALUES (?,?,?)", parameter);
}

void CSqlCriteria::RemoveUnusedCriteria()
{
	ExecuteRequestWithNoResult("DELETE FROM CRITERIA WHERE NumCriteria not in (select distinct NumCriteria from PHOTOSCRITERIA) and NumCategory != 5");
}

int64_t CSqlCriteria::GetCriteriaId(const int& numCriteria, const int& numFolder)
{
	criteriaId = 0;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFolder));
	parameter.push_back(std::make_unique<CSqlInt>(numCriteria));
	ExecuteSqlWithStatement("select distinct NumCriteria From PHOTOSCRITERIA inner join PHOTOS on PHOTOSCRITERIA.NumPhoto = PHOTOS.NumPhoto where PHOTOS.NumFolderCatalog = ? and NumCriteria = ?", parameter);
	return criteriaId;
}

int64_t CSqlCriteria::GetCriteriaIdByCategorie(const int& numPhoto, const int& numCategorie)
{
	criteriaId = 0;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numPhoto));
	parameter.push_back(std::make_unique<CSqlInt>(numCategorie));
	ExecuteSqlWithStatement("select PHOTOSCRITERIA.NumCriteria From PHOTOSCRITERIA inner join CRITERIA on PHOTOSCRITERIA.NumCriteria = CRITERIA.NumCriteria  where NumPhoto = ? and NumCategorie = ?", parameter);
	return criteriaId;
}

bool CSqlCriteria::UpdateCriteria(const int64_t& numCatalog, const int64_t& numCategorie, const wxString& libelle)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCatalog));
	parameter.push_back(std::make_unique<CSqlInt>(numCategorie));
	parameter.push_back(std::make_unique<CSqlString>(libelle));
	return ExecuteSqlWithStatementNoResult("UPDATE CRITERIA SET Libelle = ? WHERE NumCatalog = ? and NumCriteria = ? ", parameter);
}

int64_t CSqlCriteria::GetCriteriaId(const int64_t& numCatalog, const int64_t& numCategorie, const wxString& libelle)
{
	criteriaId = 0;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCatalog));
	parameter.push_back(std::make_unique<CSqlInt>(numCategorie));
	parameter.push_back(std::make_unique<CSqlString>(libelle));
	ExecuteSqlWithStatement("SELECT NumCriteria FROM CRITERIA WHERE NumCatalog = ? and NumCategorie = ? and Libelle = ?", parameter);
	return criteriaId;
}

int64_t CSqlCriteria::GetOrInsertCriteriaId(const int64_t& numCatalog, const int64_t& numCategorie,
                                            const wxString& libelle, bool& isNew)
{
	isNew = false;
	wxString _value = libelle;
	int64_t id = GetCriteriaId(numCatalog, numCategorie, _value);
	if (id == -1)
	{
		isNew = true;
		InsertCriteria(numCatalog, numCategorie, _value);
		//id = GetLastId();
		id = GetCriteriaId(numCatalog, numCategorie, _value);
	}
	return id;
}

bool CSqlCriteria::DeleteCriteria(const int& numCriteria, const int& numCategory)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCriteria));
	parameter.push_back(std::make_unique<CSqlInt>(numCategory));
	return ExecuteSqlWithStatementNoResult("Delete from CRITERIA where NumCriteria = ? and NumCategorie =  ? ", parameter);
}

bool CSqlCriteria::DeleteCriteriaAlone()
{
	return (ExecuteRequestWithNoResult(
			       "Delete from CRITERIA where NumCriteria not in (select NumCriteria From PhotosCRITERIA) and NumCategorie not in (5,6,7)")
		       != -1)
		       ? true
		       : false;
}

bool CSqlCriteria::DeletePhotoCriteria(const int64_t& numCatalog, const int64_t& numCriteria)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCatalog));
	parameter.push_back(std::make_unique<CSqlInt>(numCriteria));
	return ExecuteSqlWithStatementNoResult("DELETE FROM PHOTOS WHERE NumCatalog = ? and NumCriteria = ? ", parameter);
}

int64_t CSqlCriteria::GetNumCategorieId(const int64_t& numCatalog, const wxString& libelle)
{
	criteriaId = 0;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCatalog));
	parameter.push_back(std::make_unique<CSqlString>(libelle));
	ExecuteSqlWithStatement("SELECT NumCategorie FROM CRITERIA WHERE NumCatalog = ?  and Libelle = ? ", parameter);
	return criteriaId;
}

bool CSqlCriteria::DeleteCriteriaFolder(const int64_t& numFolder)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFolder));
	parameter.push_back(std::make_unique<CSqlInt>(numFolder));

	wxString sql;
	sql = "DELETE FROM CRITERIA WHERE NumCategory < 5 and NumCriteria in (";
	sql.append("SELECT DISTINCT NumCriteria FROM PHOTOS as PH INNER JOIN PHOTOSCRITERIA as CR ON CR.NumPHOTO = PH.NumPHOTO WHERE PH.NumFolderCatalog = ? AND NumCriteria NOT IN (");
	sql.append("SELECT DISTINCT NumCriteria FROM PHOTOS as PH INNER JOIN PHOTOSCRITERIA as CR ON CR.NumPHOTO = PH.NumPHOTO WHERE PH.NumFolderCatalog != ? ))");

	return ExecuteSqlWithStatementNoResult(sql, parameter);
}

bool CSqlCriteria::DeleteCriteriaCatalog(const int64_t& numCatalog)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCatalog));
	return ExecuteSqlWithStatementNoResult("DELETE FROM CRITERIA WHERE NumCatalog = ?", parameter);
}

int CSqlCriteria::GetCriteriaId()
{
	criteriaId = 0;
	type = 0;
	ExecuteRequest("select numcriteria from criteria order by numcriteria desc limit 1");
	return criteriaId;
}

int CSqlCriteria::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	criteriaId = -1;

	while (sqlResult->Next())
	{
		switch (type)
		{
		case 0:
			criteriaId = sqlResult->ColumnDataInt(0);
			break;
		case 1:
			listCriteriaToGeolocalize.push_back(sqlResult->ColumnDataInt(0));
			break;
		case 2:
			libelle = sqlResult->ColumnDataText(0);
			break;
		default: ;
		}
		
		nbResult++;
	}
	return nbResult;
}
