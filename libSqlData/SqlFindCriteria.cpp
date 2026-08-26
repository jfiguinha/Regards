#include <header.h>
#include "SqlFindCriteria.h"
#include "SqlResult.h"
#include <SqlParameter.h>
using namespace Regards::Sqlite;

CSqlFindCriteria::CSqlFindCriteria()
	: CSqlExecuteRequest(L"RegardsDB"), m_criteriaVector(nullptr)
{
}


bool CSqlFindCriteria::SearchCriteriaAlone(CriteriaVector* criteriaVector)
{
	m_criteriaVector = criteriaVector;
	if (m_criteriaVector == nullptr)
		return false;

	m_criteriaVector->clear();
	return (ExecuteRequest("SELECT NumCriteria, NumCategorie, Libelle from CRITERIA where NumCriteria not in (select NumCriteria From PhotosCRITERIA)")
		       != -1)
		       ? true
		       : false;
}

bool CSqlFindCriteria::SearchCriteria(CriteriaVector* criteriaVector, const int64_t& numCategorie,
                                      const int64_t& numCatalog)
{
	m_criteriaVector = criteriaVector;
	if (m_criteriaVector == nullptr)
		return false;

	m_criteriaVector->clear();
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCatalog));
	parameter.push_back(std::make_unique<CSqlInt>(numCategorie));
	return ExecuteSqlWithStatement("SELECT NumCriteria, NumCategorie, Libelle FROM CRITERIA WHERE numCatalog = ? and NumCategorie = ? order by libelle", parameter);
}

bool CSqlFindCriteria::SearchCriteria(CriteriaVector* criteriaVector, const int64_t& numCategorie,
                                      const int64_t& numCatalog, const int64_t& numFolder)
{
	m_criteriaVector = criteriaVector;
	if (m_criteriaVector == nullptr)
		return false;

	m_criteriaVector->clear();
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCatalog));
	parameter.push_back(std::make_unique<CSqlInt>(numCategorie));
	parameter.push_back(std::make_unique<CSqlInt>(numFolder));
	return ExecuteSqlWithStatement("SELECT NumCriteria, NumCategorie, Libelle FROM CRITERIA WHERE numCatalog = ? and NumCategorie = ? and NumCriteria in (select distinct NumCriteria From PHOTOSCRITERIA inner join PHOTOS on PHOTOSCRITERIA.NumPhoto = PHOTOS.NumPhoto where PHOTOS.NumFolderCatalog = ?) order by libelle", parameter);
}

bool CSqlFindCriteria::SearchCriteria(CriteriaVector* criteriaVector, const wxString& key, const int64_t& numCategorie,
                                      const int64_t& numCatalog)
{
	m_criteriaVector = criteriaVector;
	if (m_criteriaVector == nullptr)
		return false;

	m_criteriaVector->clear();
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCatalog));
	parameter.push_back(std::make_unique<CSqlInt>(numCategorie));
	parameter.push_back(std::make_unique<CSqlString>(key + "%"));
	return ExecuteSqlWithStatement("SELECT NumCriteria, NumCategorie, Libelle FROM CRITERIA WHERE numCatalog = ? and NumCategorie = ? and libelle like ?", parameter);

}

bool CSqlFindCriteria::SearchCriteria(CriteriaVector* criteriaVector, const int64_t& numPhoto)
{
	m_criteriaVector = criteriaVector;
	if (m_criteriaVector == nullptr)
		return false;

	m_criteriaVector->clear();
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numPhoto));
	return ExecuteSqlWithStatement("SELECT CR.NumCriteria, CR.NumCategorie, CR.Libelle FROM CRITERIA as CR INNER JOIN PHOTOSCRITERIA as PHCR ON CR.NumCriteria = PHCR.NumCriteria WHERE NumPhoto = ?", parameter);
}

int CSqlFindCriteria::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		CCriteria _cCriteria;
		_cCriteria.SetId(sqlResult->ColumnDataInt(0));
		_cCriteria.SetCategorieId(sqlResult->ColumnDataInt(1));
		_cCriteria.SetLibelle(sqlResult->ColumnDataText(2));
		m_criteriaVector->push_back(_cCriteria);
		nbResult++;
	}
	return nbResult;
};
