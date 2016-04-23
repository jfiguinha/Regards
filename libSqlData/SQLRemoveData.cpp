#include "SQLRemoveData.h"
#include "SqlCatalog.h"
#include "SqlFolderCatalog.h"
#include "SqlPhotos.h"
#include "SqlCriteria.h"
#include "SqlPhotoCriteria.h"
#include "SqlFindCriteria.h"
using namespace Regards::Sqlite;

CSQLRemoveData::CSQLRemoveData()
{
}


CSQLRemoveData::~CSQLRemoveData()
{
}


////////////////////////////////////////////////////////////////////
//Suppression de toutes les données d'un catalog
////////////////////////////////////////////////////////////////////
bool CSQLRemoveData::DeleteCatalog(const int &numCatalog)
{

	//Suppression des critères des photos
	CSqlCriteria sqlCriteria;
	sqlCriteria.DeleteCriteriaCatalog(numCatalog);

	//Suppression des critères des photos
	CSqlPhotoCriteria sqlPhotoCriteria;
	sqlPhotoCriteria.DeleteCatalogCriteria(numCatalog);

	//Suppression des photos du catalog
	CSqlPhotos sqlPhoto;
	sqlPhoto.DeletePhotoCatalog(numCatalog);

	//Suppression des répertoires du catalog
	CSqlFolderCatalog sqlFolderCatalog;
	sqlFolderCatalog.DeleteCatalog(numCatalog);

	//Suppression du catalog
	CSqlCatalog sqlCatalog;
	sqlCatalog.DeleteCatalog(numCatalog);

	return 0;
}

////////////////////////////////////////////////////////////////////
//Suppression de toutes les données d'un répertoire
////////////////////////////////////////////////////////////////////
bool CSQLRemoveData::DeleteFolder(const int &numFolder)
{

	//Suppression des critères des photos
	CSqlCriteria sqlCriteria;
	sqlCriteria.DeleteCriteriaFolder(numFolder);

	CSqlPhotoCriteria sqlPhotoCriteria;
	sqlPhotoCriteria.DeleteFolderCriteria(numFolder);

	//Suppression des photos du catalog
	CSqlPhotos sqlPhoto;
	sqlPhoto.DeletePhotoFolder(numFolder);

	//Suppression des répertoires du catalog
	CSqlFolderCatalog sqlFolderCatalog;
	sqlFolderCatalog.DeleteFolder(numFolder);

	return 0;
}

////////////////////////////////////////////////////////////////////
//Suppression de toutes les données d'un répertoire
////////////////////////////////////////////////////////////////////
bool CSQLRemoveData::DeleteListPhoto(const vector<int> &listPhoto, CriteriaVector * criteriaVector)
{
	CSqlPhotoCriteria sqlPhotoCriteria;
	CSqlPhotos sqlPhoto;
	CSqlFindCriteria sqlFindCriteria;
	CSqlCriteria sqlCriteria;
	for (int photo : listPhoto)
	{
		//Suppression des critères des photos		
		sqlPhotoCriteria.DeletePhoto(photo);

		//Suppression des photos du catalog
		sqlPhoto.DeletePhoto(photo);
		
	}

	sqlFindCriteria.SearchCriteriaAlone(criteriaVector);
	//Suppression des critères orphelins
	sqlCriteria.DeleteCriteriaAlone();
	return 0;
}