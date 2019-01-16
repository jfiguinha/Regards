#include "SqlFindCriteria.h"
using namespace Regards::Sqlite;

CSqlFindCriteria::CSqlFindCriteria()
	: CSqlExecuteRequest(L"RegardsDB")
{
}


CSqlFindCriteria::~CSqlFindCriteria()
{
}

bool CSqlFindCriteria::SearchUniqueCriteria(CriteriaVector * criteriaVector, const int64_t & numFolder, const int64_t &numCatalog)
{
	wxString sql;
	m_criteriaVector = criteriaVector;
	sql = "SELECT NumCriteria, NumCategorie, Libelle FROM CRITERIA WHERE NumCriteria in (";
	sql.append("SELECT DISTINCT NumCriteria FROM PHOTOS as PH INNER JOIN PHOTOSCRITERIA as CR ON CR.NumPHOTO = PH.NumPHOTO WHERE PH.NumFolderCatalog = " + to_string(numFolder) + " AND NumCriteria NOT IN(");
	sql.append("SELECT DISTINCT NumCriteria FROM PHOTOS as PH INNER JOIN PHOTOSCRITERIA as CR ON CR.NumPHOTO = PH.NumPHOTO WHERE PH.NumFolderCatalog != " + to_string(numFolder) + "))");
	return (ExecuteRequest(sql) != -1) ? true : false;
}

bool CSqlFindCriteria::SearchCriteriaAlone(CriteriaVector * criteriaVector)
{
	m_criteriaVector = criteriaVector;
	return (ExecuteRequest("SELECT * from CRITERIA where NumCriteria not in (select NumCriteria From PhotosCRITERIA)") != -1) ? true : false;
}

bool CSqlFindCriteria::SearchCriteria(CriteriaVector * criteriaVector, const int64_t & numCategorie, const int64_t &numCatalog)
{

	m_criteriaVector = criteriaVector;
	return (ExecuteRequest("SELECT NumCriteria, NumCategorie, Libelle FROM CRITERIA WHERE numCatalog = " + to_string(numCatalog) + " and NumCategorie = " + to_string(numCategorie) + " order by libelle") != -1) ? true : false;
}

bool CSqlFindCriteria::SearchCriteria(CriteriaVector * criteriaVector, const int64_t & numCategorie, const int64_t &numCatalog, const int64_t &numFolder)
{

	m_criteriaVector = criteriaVector;
	return (ExecuteRequest("SELECT NumCriteria, NumCategorie, Libelle FROM CRITERIA WHERE numCatalog = " + to_string(numCatalog) + " and NumCategorie = " + to_string(numCategorie) + " and NumCriteria in (select distinct NumCriteria From PHOTOSCRITERIA inner join PHOTOS on PHOTOSCRITERIA.NumPhoto = PHOTOS.NumPhoto  where PHOTOS.NumFolderCatalog = " + to_string(numFolder) + ") order by libelle") != -1) ? true : false;
}

bool CSqlFindCriteria::SearchCriteria(CriteriaVector * criteriaVector, const wxString &key, const int64_t &numCategorie, const int64_t &numCatalog)
{
	m_criteriaVector = criteriaVector;
	return (ExecuteRequest("SELECT NumCriteria, NumCategorie, Libelle FROM CRITERIA WHERE numCatalog = " + to_string(numCatalog) + " and NumCategorie = " + to_string(numCategorie) + " and libelle like '" + key + "%'") != -1) ? true : false;
}

bool CSqlFindCriteria::SearchCriteria(CriteriaVector * criteriaVector, const int64_t & numPhoto)
{
	m_criteriaVector = criteriaVector;
	return (ExecuteRequest("SELECT CR.NumCriteria, CR.NumCategorie, CR.Libelle FROM CRITERIA as CR INNER JOIN PHOTOSCRITERIA as PHCR ON CR.NumCriteria = PHCR.NumCriteria WHERE NumPhoto = " + to_string(numPhoto)) != -1) ? true : false;

}

int CSqlFindCriteria::TraitementResult(CSqlResult * sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		CCriteria _cCriteria;
		for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
		{

			switch (i)
			{
			case 0:
				_cCriteria.SetId(sqlResult->ColumnDataInt(i));
				break;
			case 1:
				_cCriteria.SetCategorieId(sqlResult->ColumnDataInt(i));
				break;
			case 2:
				_cCriteria.SetLibelle(sqlResult->ColumnDataText(i));
				break;
			}
		}
		m_criteriaVector->push_back(_cCriteria);
		nbResult++;
	}
	return nbResult;
};