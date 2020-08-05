#include <header.h>
#include "SQLRemoveData.h"
#include "SqlCatalog.h"
#include "SqlFolderCatalog.h"
#include "SqlPhotos.h"
#include "SqlCriteria.h"
#include "SqlPhotoCriteria.h"
#include "SqlThumbnail.h"
#include "SqlThumbnailVideo.h"
#include "SqlFindCriteria.h"
#include "SqlFacePhoto.h"
#include "SqlFaceDescriptor.h"
#include "SqlFaceLabel.h"
#include "SqlFaceRecognition.h"
#include "SqlThumbnailVideo.h"
#include "SqlPhotoCategorieUsenet.h"
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
	CSqlThumbnail sqlThumbnail;
	sqlThumbnail.EraseThumbnail();

	//Suppression des critères des photos
	CSqlCriteria sqlCriteria;
	sqlCriteria.DeleteCriteriaCatalog(numCatalog);

	//Suppression des critères des photos
	CSqlPhotoCriteria sqlPhotoCriteria;
	sqlPhotoCriteria.DeletePhotoCriteria();

	//Suppression des photos du catalog
	CSqlPhotos sqlPhoto;
	sqlPhoto.DeletePhotoCatalog(numCatalog);
	sqlPhoto.DeletePhotoSearch();

	//Suppression des répertoires du catalog
	CSqlFolderCatalog sqlFolderCatalog;
	sqlFolderCatalog.DeleteCatalog(numCatalog);

	CSqlFacePhoto sqlFacePhoto;
	sqlFacePhoto.DeleteFaceDatabase();
	sqlFacePhoto.DeleteFaceTreatmentDatabase();

	CSqlFaceDescriptor sqlFaceDescriptor;
	sqlFaceDescriptor.DeleteFaceDescriptorDatabase();

	CSqlFaceRecognition sqlFaceRecognition;
	sqlFaceRecognition.DeleteFaceRecognitionDatabase();

	CSqlFaceLabel sqlFaceLabel;
	sqlFaceLabel.DeleteFaceLabelDatabase();

	CSqlPhotoCategorieUsenet photoCategorie;
	photoCategorie.DeletePhotoProcessingDatabase();

	//Suppression du catalog
	//CSqlCatalog sqlCatalog;
	//sqlCatalog.DeleteCatalog(numCatalog);

	return 0;
}

bool CSQLRemoveData::DeleteFaceDatabase()
{
	CSqlFacePhoto sqlFacePhoto;
	sqlFacePhoto.DeleteFaceDatabase();
	sqlFacePhoto.DeleteFaceTreatmentDatabase();

	CSqlFaceDescriptor sqlFaceDescriptor;
	sqlFaceDescriptor.DeleteFaceDescriptorDatabase();

	CSqlFaceRecognition sqlFaceRecognition;
	sqlFaceRecognition.DeleteFaceRecognitionDatabase();

	CSqlFaceLabel sqlFaceLabel;
	sqlFaceLabel.DeleteFaceLabelDatabase();

	return 0;
}

////////////////////////////////////////////////////////////////////
//Suppression de toutes les données d'un répertoire
////////////////////////////////////////////////////////////////////
bool CSQLRemoveData::DeleteFolder(const int &numFolder)
{
	vector<wxString> listPhoto;

	
	CSqlThumbnail sqlThumbnail;
	sqlThumbnail.EraseFolderThumbnail(numFolder);

	CSqlThumbnailVideo sqlThumbnailVideo;
	sqlThumbnailVideo.EraseFolderThumbnail(numFolder);
	   
	CSqlPhotoCriteria sqlPhotoCriteria;
	sqlPhotoCriteria.DeleteFolderCriteria(numFolder);
	   	
	//Suppression des répertoires du catalog
	CSqlFolderCatalog sqlFolderCatalog;
	sqlFolderCatalog.DeleteFolder(numFolder);

	//Suppression des photos du catalog
	CSqlPhotos sqlPhoto;
	listPhoto = sqlPhoto.GetPhotoFromFolder(numFolder);
	sqlPhoto.DeletePhotoFolder(numFolder);

	CSqlFacePhoto sqlFacePhoto;
	sqlFacePhoto.DeleteListOfPhoto(listPhoto);

	//Suppression des critères des photos
	CSqlCriteria sqlCriteria;
	sqlCriteria.DeleteCriteriaAlone();


	CSqlPhotoCategorieUsenet photoCategorie;
	for(wxString photoPath : listPhoto)
		photoCategorie.DeletePhotoProcessing(photoPath);

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
    CSqlThumbnail sqlThumbnail;
    CSqlThumbnailVideo sqlThumbnailVideo;
	CSqlFacePhoto sqlFacePhoto;
	
	

	sqlFacePhoto.DeleteListOfPhoto(listPhoto);

	for (auto photo : listPhoto)
	{
		
		sqlThumbnail.DeleteThumbnail(photo );
        sqlThumbnailVideo.DeleteThumbnail(photo);


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


////////////////////////////////////////////////////////////////////
//Suppression de toutes les données d'un répertoire
////////////////////////////////////////////////////////////////////
bool CSQLRemoveData::DeletePhoto(const int & idPhoto)
{
	CSqlPhotoCriteria sqlPhotoCriteria;
	CSqlPhotos sqlPhoto;
	CSqlCriteria sqlCriteria;

	vector<int> listPhoto;
	listPhoto.push_back(idPhoto);

	CSqlFacePhoto sqlFacePhoto;
	sqlFacePhoto.DeleteListOfPhoto(listPhoto);

	//Suppression des critères des photos		
	sqlPhotoCriteria.DeletePhoto(idPhoto);
	//Suppression des photos du catalog
	sqlPhoto.DeletePhoto(idPhoto);
	//Suppression des critères orphelins
	sqlCriteria.DeleteCriteriaAlone();
	return 0;
}