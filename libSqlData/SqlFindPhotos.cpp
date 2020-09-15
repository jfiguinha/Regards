#include <header.h>
#include "SqlFindPhotos.h"
#include "SqlResult.h"
#include <LibResource.h>
#include <libPicture.h>
#include <ConvertUtility.h>
using namespace Regards::Sqlite;

CSqlFindPhotos::CSqlFindPhotos()
	: CSqlExecuteRequest(L"RegardsDB")
{
	typeResult = 0;
}


CSqlFindPhotos::~CSqlFindPhotos()
{
}

bool CSqlFindPhotos::GetAllPhotos(PhotosVector * photosVector)
{
	typeResult = 0;
	m_photosVector = photosVector;
	return (ExecuteRequest("SELECT NumPhoto,FullPath FROM PHOTOS") != -1) ? true : false;
}

void CSqlFindPhotos::UpdatePhotosExtension()
{
    CLibPicture libPicture;
	typeResult = 0;
    PhotosVector photosVector;
	m_photosVector = &photosVector;
    ExecuteRequest("SELECT NumPhoto,FullPath FROM PHOTOS");
    for(CPhotos photo : photosVector)
    {
        int indexId = libPicture.TestImageFormat(photo.GetPath());
        bool result = (ExecuteRequestWithNoResult("UPDATE PHOTOS SET ExtensionId = " + to_string(indexId) + " WHERE NumPhoto = " + to_string(photo.GetId())) != -1) ? true : false;
        printf("CSqlFindPhotos::UpdatePhotosExtension \n");
    }
}

bool CSqlFindPhotos::GetAllVideo(PhotosVector * photosVector)
{
	typeResult = 0;
	m_photosVector = photosVector;
	return (ExecuteRequest("SELECT NumPhoto,FullPath FROM PHOTOS WHERE FullPath not in (SELECT FullPath FROM VIDEOTHUMBNAIL)") != -1) ? true : false;
}


bool CSqlFindPhotos::SearchPhotos(PhotosVector * photosVector, const wxString &libelleCriteria)
{
	typeResult = 0;
	m_photosVector = photosVector;
	return (ExecuteRequest("SELECT NumPhoto,FullPath, CreateDate, GeoGps FROM PHOTOSSEARCHCRITERIA WHERE CreateDate like '" + libelleCriteria + "%'") != -1) ? true : false;
}

bool CSqlFindPhotos::SearchPhotos(PhotosVector * photosVector, const wxString &localisation, const wxString &libelleCriteria)
{
	m_photosVector = photosVector;
	return (ExecuteRequest("Select NumPhoto,FullPath, CreateDate, GeoGps from PHOTOSSEARCHCRITERIA WHERE GeoGps = '" + localisation + "' and CreateDate = '" + libelleCriteria + "')") != -1) ? true : false;
}

bool CSqlFindPhotos::SearchPhotos(PhotosVector * photosVector)
{
	typeResult = 0;
	m_photosVector = photosVector;
	return (ExecuteRequest("SELECT NumPhoto,FullPath, CreateDate, GeoGps FROM PHOTOSSEARCHCRITERIA ORDER BY FullPath, GeoGps") != -1) ? true : false;
}

bool CSqlFindPhotos::SearchPhotos(vector<int> * listPhoto)
{
	typeResult = 1;
	m_listPhoto = listPhoto;
	return (ExecuteRequest("SELECT NumPhoto FROM PHOTOSSEARCHCRITERIA") != -1) ? true : false;
}


bool CSqlFindPhotos::SearchPhotosByTypeAffichage(PhotosVector * photosVector, const int &typeAffichage, const int &numCatalog)
{
	typeResult = 0;
	m_photosVector = photosVector;
	//return (ExecuteRequest("SELECT NumPhoto,FullPath, CreateDate, GeoGps FROM  PHOTOSSEARCHCRITERIA ORDER BY CreateDate desc, GeoGps") != -1) ? true : false;
	return (ExecuteRequest("SELECT NumPhoto,FullPath, CreateDate, GeoGps FROM  PHOTOSSEARCHCRITERIA ORDER BY FullPath, GeoGps") != -1) ? true : false;
}

wxString CSqlFindPhotos::GetSearchSQL(vector<int> list)
{
	wxString req;
	for (vector<int>::iterator fit = list.begin(); fit != list.end(); fit++)
	{
		req.append(to_string(*fit));
		req.append(",");
	}
	req = req.substr(0, req.length() - 1);
	req.append(")");
	return req;
}

wxString CSqlFindPhotos::GenerateSqlRequest(const int &numCatalog, vector<int> & listFolder, vector<int> & listCriteriaNotIn, vector<int> & listFaceNotIn, vector<int> & listFaceSelected, vector<int> & listStarSelected, vector<int> & listStarNotSelected, vector<int> & listKeywordSelected, vector<int> & listKeywordNotSelected, const double & pertinence)
{
    //Request In
	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	wxString month = to_string(now->tm_mon + 1);
	if(month.size() < 2)
		month = "0" + month;
	wxString day = to_string(now->tm_mday);
	if(day.size() < 2)
		day = "0" + day;

	wxString createDate = to_string(now->tm_year + 1900) + "." + month + "." + day;
	wxString libelle = CLibResource::LoadStringFromResource("LBLNOTGEO", 1);
	wxString reqSQIn = "INSERT INTO PHOTOSSEARCHCRITERIA (NumPhoto,FullPath, CreateDate, GeoGps) ";
	reqSQIn += "SELECT NumPhoto, FullPath, \"" + createDate + "\" as CreateDate, \"" + libelle + "\" as GeoGps FROM PHOTOS WHERE CriteriaInsert = 0";
	reqSQIn += " UNION ";
	reqSQIn += "SELECT distinct PH.NumPhoto, PH.FullPath, ";
	reqSQIn += "(select Libelle FROM PHOTOS as PHOTO INNER JOIN FOLDERCATALOG as FC ON PHOTO.NumFolderCatalog = FC.NumFolderCatalog INNER JOIN PHOTOSCRITERIA as PHCR ON PHOTO.NumPhoto = PHCR.NumPhoto INNER JOIN CRITERIA as CR ON CR.NumCriteria = PHCR.NumCriteria  where PHOTO.NumPhoto = PH.NumPhoto AND NUMCATEGORIE = 3) as CreateDate, ";
	reqSQIn += "(select Libelle FROM PHOTOS as PHOTO INNER JOIN FOLDERCATALOG as FC ON PHOTO.NumFolderCatalog = FC.NumFolderCatalog INNER JOIN PHOTOSCRITERIA as PHCR ON PHOTO.NumPhoto = PHCR.NumPhoto INNER JOIN CRITERIA as CR ON CR.NumCriteria = PHCR.NumCriteria  where PHOTO.NumPhoto = PH.NumPhoto AND NUMCATEGORIE = 1) as GeoGps ";
	reqSQIn += "FROM PHOTOS as PH INNER JOIN FOLDERCATALOG as FC ON PH.NumFolderCatalog = FC.NumFolderCatalog INNER JOIN PHOTOSCRITERIA as PHCR ON PH.NumPhoto = PHCR.NumPhoto INNER JOIN CRITERIA as CR ON CR.NumCriteria = PHCR.NumCriteria WHERE FC.NumCatalog = " + to_string(numCatalog);
    
    //Construction du In
    if (listFolder.size() > 0)
    {
        reqSQIn.append(" and PH.NumFolderCatalog in (");
        reqSQIn.append(GetSearchSQL(listFolder));
        //Construction du NotIn
        if (listCriteriaNotIn.size() > 0)
        {
            reqSQIn.append(" and PH.NumPhoto not in (");
            reqSQIn.append("SELECT distinct PH.NumPhoto FROM PHOTOS as PH INNER JOIN FOLDERCATALOG as FC ON PH.NumFolderCatalog = FC.NumFolderCatalog INNER JOIN PHOTOSCRITERIA as PHCR ON PH.NumPhoto = PHCR.NumPhoto INNER JOIN CRITERIA as CR ON CR.NumCriteria = PHCR.NumCriteria WHERE FC.NumCatalog = ");
            reqSQIn.append(to_string(numCatalog));
            reqSQIn.append(" and PH.NumFolderCatalog in (");
            reqSQIn.append(GetSearchSQL(listFolder));
            reqSQIn.append(" and CR.NumCriteria in (");
            reqSQIn.append(GetSearchSQL(listCriteriaNotIn));
            reqSQIn.append(")");
        }

		if(listFaceNotIn.size() > 0)
		{
			wxString value = wxString::Format(wxT("%f"), pertinence);
            reqSQIn.append(" and PH.NumPhoto in (");
            reqSQIn.append("select NumPhoto From Photos where FullPath in (Select distinct FullPath From FACE_RECOGNITION INNER JOIN FACEPHOTO ON FACEPHOTO.NumFace = FACE_RECOGNITION.NumFace WHERE FACEPHOTO.Pertinence > " + value + " and NumFaceCompatible in (");
            reqSQIn.append(GetSearchSQL(listFaceSelected));
            reqSQIn.append("))");
		}

		if (listStarNotSelected.size()  != 0 || listKeywordNotSelected.size() != 0)
		{
			reqSQIn.append(" and PH.NumPhoto in (");
			reqSQIn.append("SELECT distinct PH.NumPhoto FROM PHOTOS as PH INNER JOIN PHOTOSCRITERIA as PHCR ON PH.NumPhoto = PHCR.NumPhoto INNER JOIN CRITERIA as CR ON CR.NumCriteria = PHCR.NumCriteria WHERE CR.NumCriteria in (");
			if (listStarNotSelected.size() != 0)
				reqSQIn.append(GetSearchSQL(listStarSelected));
			if(listKeywordNotSelected.size() != 0)
				reqSQIn.append(GetSearchSQL(listKeywordSelected));
			reqSQIn.append(")");
		}

        printf("Requete Photos Search Criteria : %s \n", CConvertUtility::ConvertToUTF8(reqSQIn));
        return reqSQIn;
    }



    return "";
}

bool CSqlFindPhotos::SearchPhotos(const wxString & sqlRequest)
{
    if(sqlRequest != "")
    {
        DeleteAllInSearchPhotos();
        return (ExecuteRequestWithNoResult(sqlRequest) != -1) ? true : false;
    }
	return false;
}

bool CSqlFindPhotos::DeleteAllInSearchPhotos()
{
	return (ExecuteRequestWithNoResult("Delete From PHOTOSSEARCHCRITERIA") != -1) ? true : false;
}

bool CSqlFindPhotos::SearchPhotos(PhotosVector * photosVector, const int &numCategorie, const int &numCatalog)
{
	m_photosVector = photosVector;
	return (ExecuteRequest("SELECT distinct PH.NumPhoto, PH.FullPath FROM PHOTOS as PH INNER JOIN FOLDERCATALOG as FC ON PH.NumFolderCatalog = FC.NumFolderCatalog INNER JOIN PHOTOSCRITERIA as PHCR ON PH.NumPhoto = PHCR.NumPhoto INNER JOIN CRITERIA as CR ON CR.NumCriteria = PHCR.NumCriteria WHERE FC.NumCatalog = " + to_string(numCatalog) + " AND CR.NumCategorie = " + to_string(numCategorie)) != -1) ? true : false;
}

int CSqlFindPhotos::TraitementResult(CSqlResult * sqlResult)
{
	int nbResult = 0;
	switch (typeResult)
	{
	case 0:
		nbResult = TraitementResultPhoto(sqlResult);
		break;
	case 1:
		nbResult = TraitementResultNumPhoto(sqlResult);
		break;
	}

	return nbResult;
}

int CSqlFindPhotos::TraitementResultPhoto(CSqlResult * sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		CPhotos _cPhoto;
		for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
		{

			switch (i)
			{
			case 0:
				_cPhoto.SetId(sqlResult->ColumnDataInt(i));
				break;
			case 1:
				_cPhoto.SetPath(sqlResult->ColumnDataText(i));
				break;
			case 2:
				_cPhoto.SetCreateDate(sqlResult->ColumnDataText(i));
				break;
			case 3:
				_cPhoto.SetGpsInfos(sqlResult->ColumnDataText(i));
				break;
			}
		}
		m_photosVector->push_back(_cPhoto);
		nbResult++;
	}
	return nbResult;
}

int CSqlFindPhotos::TraitementResultNumPhoto(CSqlResult * sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
		{

			switch (i)
			{
			case 0:
				m_listPhoto->push_back(sqlResult->ColumnDataInt(i));
				break;
			}
		}
		nbResult++;
	}
	return nbResult;
}