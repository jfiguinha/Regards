#include <header.h>
#include "SqlPhotoCriteria.h"
#include "SqlCriteria.h"
#include "SqlPhotos.h"
#include <ConvertUtility.h>
#include "SqlTransaction.h"
#include <SqlParameter.h>
using namespace Regards::Sqlite;

CSqlPhotoCriteria::CSqlPhotoCriteria()
	: CSqlExecuteRequest(L"RegardsDB")
{
}

bool CSqlPhotoCriteria::InsertPhotoListCriteria(const CListCriteriaPhoto& listPhotoCriteria, bool& isNew,
	bool criteriaUpdate, const int& numFolder)
{
	CSqlTransaction sqlTransaction;
	CSqlPhotos sqlPhoto(this->m_transaction, this->m_useTransaction);
	CSqlCriteria sqlCriteria(this->m_transaction, this->m_useTransaction);

	for (auto it = listPhotoCriteria.listCriteria.begin(); it != listPhotoCriteria.listCriteria.end(); ++it)
	{
		CInsertCriteria insertCriteria = *it;

		//Old Criteria
		int oldCriteriaId = sqlCriteria.GetCriteriaIdByCategorie(listPhotoCriteria.numPhotoId, insertCriteria.type);

		//printf("insertCriteria value : %s \n", CConvertUtility::ConvertToStdString(insertCriteria.value));

		insertCriteria.id = sqlCriteria.GetOrInsertCriteriaId(listPhotoCriteria.numCatalog, insertCriteria.type,
			insertCriteria.value, insertCriteria.isNew);

		if (oldCriteriaId == insertCriteria.id)
			continue;

		if (insertCriteria.isNew && isNew == false)
			isNew = true;

		if (!isNew && numFolder != 0)
		{
			int numCriteria = sqlCriteria.GetCriteriaId(insertCriteria.id, numFolder);
			if (numCriteria != insertCriteria.id)
				isNew = true;
		}

		if (oldCriteriaId != -1)
			DeletePhotoCriteria(listPhotoCriteria.numPhotoId, oldCriteriaId);


		InsertPhotoCriteria(listPhotoCriteria.numPhotoId, insertCriteria.id);
	}

	if (criteriaUpdate)
		sqlPhoto.UpdatePhotoCriteria(listPhotoCriteria.numPhotoId);

	sqlTransaction.commit();

	return true;
}

bool CSqlPhotoCriteria::InsertPhotoListCriteria(const CListCriteriaPhoto& listPhotoCriteria, bool& isNew,
                                                bool criteriaUpdate)
{
	return InsertPhotoListCriteria(listPhotoCriteria, isNew, criteriaUpdate, 0);
}

bool CSqlPhotoCriteria::InsertPhotoCriteria(const int64_t& numPhoto, const int64_t& numCriteria)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCriteria));
	parameter.push_back(std::make_unique<CSqlInt>(numPhoto));
	return ExecuteSqlWithStatementNoResult("INSERT INTO PHOTOSCRITERIA (NumCriteria, NumPhoto) VALUES (?,?)", parameter);

}

/////////////////////////////////////////////////////////////////////////////
//Suppression de tous les attributs pour les albums
/////////////////////////////////////////////////////////////////////////////
bool CSqlPhotoCriteria::DeleteCriteria(const int64_t& numCriteria)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCriteria));
	return ExecuteSqlWithStatementNoResult("DELETE FROM PHOTOSCRITERIA WHERE NumCriteria = ?", parameter);

}

bool CSqlPhotoCriteria::DeletePhoto(const int64_t& numPhoto)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numPhoto));
	return ExecuteSqlWithStatementNoResult("DELETE FROM PHOTOSCRITERIA WHERE NumPhoto = ?", parameter);
}

bool CSqlPhotoCriteria::DeletePhotoCriteria(const int64_t& numPhoto, const int64_t& numCriteria)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCriteria));
	parameter.push_back(std::make_unique<CSqlInt>(numPhoto));
	return ExecuteSqlWithStatementNoResult("DELETE FROM PHOTOSCRITERIA WHERE NumCriteria = ? and NumPhoto = ?", parameter);

}

bool CSqlPhotoCriteria::DeleteFolderCriteria(const int64_t& numFolder)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFolder));
	return ExecuteSqlWithStatementNoResult("DELETE FROM PHOTOSCRITERIA WHERE NumPhoto in (select NumPhoto from PHOTOS where NumFolderCatalog = ?)", parameter);

}

bool CSqlPhotoCriteria::DeleteCatalogCriteria(const int64_t& numCatalog)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCatalog));
	return ExecuteSqlWithStatementNoResult("DELETE FROM PHOTOSCRITERIA as PC INNER JOIN PHOTOS as PH ON PC.NumPhoto = PH.NumPhoto INNER JOIN FOLDERCATALOG FC on FC.NumFolderCatalog = PH.NumFolderCatalog WHERE FC.NumCatalog = ?", parameter);

}


bool CSqlPhotoCriteria::DeletePhotoCriteria()
{
	return (ExecuteRequestWithNoResult("DELETE FROM PHOTOSCRITERIA") != -1) ? true : false;
}
