#include <header.h>
#include "SqlFindPhotos.h"
#include "SqlResult.h"
#include <picture_utility.h>
#include <libPicture.h>
#include <SqlResource.h>
#include <ConvertUtility.h>
using namespace Regards::Picture;
using namespace Regards::Sqlite;

CSqlFindPhotos::CSqlFindPhotos()
	: CSqlExecuteRequest(L"RegardsDB"), m_photosVector(nullptr), m_listPhoto(nullptr)
{
	typeResult = 0;
}


CSqlFindPhotos::~CSqlFindPhotos()
{
}

bool CSqlFindPhotos::SearchPhotosByCriteria(PhotosVector* photosVector)
{
	typeResult = 2;
	m_photosVector = photosVector;
	wxString sqlRequest = "SELECT * FROM SEARCH_VIEW Order By Year desc, Month desc, Day desc, DayOfWeek desc";
	return (ExecuteRequest(sqlRequest) != -1) ? true : false;
}

bool CSqlFindPhotos::GetAllPhotos(PhotosVector* photosVector)
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
	for (CPhotos photo : photosVector)
	{
		int indexId = libPicture.TestImageFormat(photo.GetPath());
		ExecuteRequestWithNoResult(
			"UPDATE PHOTOS SET ExtensionId = " + to_string(indexId) + " WHERE NumPhoto = " + to_string(photo.GetId()));
		printf("CSqlFindPhotos::UpdatePhotosExtension \n");
	}
}

bool CSqlFindPhotos::GetAllVideo(PhotosVector* photosVector)
{
	typeResult = 0;
	m_photosVector = photosVector;
	return (ExecuteRequest(
			       "SELECT NumPhoto,FullPath FROM PHOTOS WHERE FullPath not in (SELECT FullPath FROM VIDEOTHUMBNAIL)")
		       != -
		       1)
		       ? true
		       : false;
}


bool CSqlFindPhotos::SearchPhotos(PhotosVector* photosVector, const wxString& libelleCriteria)
{
	typeResult = 0;
	m_photosVector = photosVector;
	return (ExecuteRequest(
		       "SELECT NumPhoto,FullPath, CreateDate, GeoGps FROM PHOTOSSEARCHCRITERIA WHERE CreateDate like '" +
		       libelleCriteria + "%'") != -1)
		       ? true
		       : false;
}

bool CSqlFindPhotos::SearchPhotos(PhotosVector* photosVector, const wxString& localisation,
                                  const wxString& libelleCriteria)
{
	m_photosVector = photosVector;
	return (ExecuteRequest(
		       "Select NumPhoto,FullPath, CreateDate, GeoGps from PHOTOSSEARCHCRITERIA WHERE GeoGps = '" + localisation
		       + "' and CreateDate = '" + libelleCriteria + "')") != -1)
		       ? true
		       : false;
}

bool CSqlFindPhotos::SearchPhotos(PhotosVector* photosVector)
{
	typeResult = 0;
	m_photosVector = photosVector;
	return (ExecuteRequest(
			       "SELECT NumPhoto,FullPath, CreateDate, GeoGps FROM PHOTOSSEARCHCRITERIA Group By NumPhoto ORDER BY FullPath, GeoGps")
		       != -1)
		       ? true
		       : false;
}

bool CSqlFindPhotos::SearchPhotos(vector<int>* listPhoto)
{
	typeResult = 1;
	m_listPhoto = listPhoto;
	return (ExecuteRequest("SELECT NumPhoto FROM PHOTOSSEARCHCRITERIA") != -1) ? true : false;
}


bool CSqlFindPhotos::SearchPhotosByTypeAffichage(PhotosVector* photosVector, const int& typeAffichage,
                                                 const int& numCatalog)
{
	typeResult = 0;
	m_photosVector = photosVector;
	//return (ExecuteRequest("SELECT NumPhoto,FullPath, CreateDate, GeoGps FROM  PHOTOSSEARCHCRITERIA ORDER BY CreateDate desc, GeoGps") != -1) ? true : false;
	return (ExecuteRequest(
			       "SELECT NumPhoto,FullPath, CreateDate, GeoGps FROM  PHOTOSSEARCHCRITERIA Group By NumPhoto ORDER BY FullPath, GeoGps")
		       != -1)
		       ? true
		       : false;
}

wxString CSqlFindPhotos::GetSearchSQL(vector<int> list)
{
	wxString req;
	for (auto fit = list.begin(); fit != list.end(); ++fit)
	{
		req.append(to_string(*fit));
		req.append(",");
	}
	req = req.substr(0, req.length() - 1);
	req.append(")");
	return req;
}

bool CSqlFindPhotos::FindIfViewExist()
{
	typeResult = 4;
	table_name = "";
	wxString testview = "SELECT name FROM sqlite_master WHERE type = 'view' AND name = 'PHOTOSSEARCHCRITERIA'";
	ExecuteRequest(testview);
	return (table_name != "") ? true : false;
}

wxString CSqlFindPhotos::GenerateSqlRequest(const int& numCatalog, vector<int>& listFolder,
                                            vector<int>& listCriteriaNotIn, vector<int>& listFaceNotIn,
                                            vector<int>& listFaceSelected, vector<int>& listStarSelected,
                                            vector<int>& listStarNotSelected, vector<int>& listKeywordSelected,
                                            vector<int>& listKeywordNotSelected, const wxString& libelleNotGeo,
                                            const double& pertinence)
{
	//Request In
	time_t t = time(nullptr); // get time now
	struct tm* now = localtime(&t);
	wxString month = to_string(now->tm_mon + 1);
	if (month.size() < 2)
		month = "0" + month;
	wxString day = to_string(now->tm_mday);
	if (day.size() < 2)
		day = "0" + day;

	wxString createDate = to_string(now->tm_year + 1900) + "." + month + "." + day;
	wxString libelle = libelleNotGeo; //CLibResource::LoadStringFromResource("LBLNOTGEO", 1);
	wxString reqSQIn = "";
	//if(FindIfViewExist())
	//	ExecuteRequestWithNoResult("DROP VIEW PHOTOSSEARCHCRITERIA");

	reqSQIn = "CREATE VIEW PHOTOSSEARCHCRITERIA (NumPhoto,FullPath, CreateDate, GeoGps) AS ";
	reqSQIn += "SELECT * FROM (";
	reqSQIn += "SELECT NumPhoto, FullPath, \"" + createDate + "\" as CreateDate, \"" + libelle +
		"\" as GeoGps FROM PHOTOS WHERE CriteriaInsert = 0";
	reqSQIn += " UNION ";
	reqSQIn += "SELECT distinct PH.NumPhoto, PH.FullPath, ";
	reqSQIn +=
		"(select Libelle FROM PHOTOS as PHOTO INNER JOIN FOLDERCATALOG as FC ON PHOTO.NumFolderCatalog = FC.NumFolderCatalog INNER JOIN PHOTOSCRITERIA as PHCR ON PHOTO.NumPhoto = PHCR.NumPhoto INNER JOIN CRITERIA as CR ON CR.NumCriteria = PHCR.NumCriteria  where PHOTO.NumPhoto = PH.NumPhoto AND NUMCATEGORIE = 3) as CreateDate, ";
	reqSQIn +=
		"(select Libelle FROM PHOTOS as PHOTO INNER JOIN FOLDERCATALOG as FC ON PHOTO.NumFolderCatalog = FC.NumFolderCatalog INNER JOIN PHOTOSCRITERIA as PHCR ON PHOTO.NumPhoto = PHCR.NumPhoto INNER JOIN CRITERIA as CR ON CR.NumCriteria = PHCR.NumCriteria  where PHOTO.NumPhoto = PH.NumPhoto AND NUMCATEGORIE = 1) as GeoGps ";
	reqSQIn +=
		"FROM PHOTOS as PH INNER JOIN FOLDERCATALOG as FC ON PH.NumFolderCatalog = FC.NumFolderCatalog INNER JOIN PHOTOSCRITERIA as PHCR ON PH.NumPhoto = PHCR.NumPhoto INNER JOIN CRITERIA as CR ON CR.NumCriteria = PHCR.NumCriteria WHERE FC.NumCatalog = "
		+ to_string(numCatalog);

	//Construction du In
	if (listFolder.size() > 0)
	{
		reqSQIn.append(" and PH.NumFolderCatalog in (");
		reqSQIn.append(GetSearchSQL(listFolder));
		//Construction du NotIn
		if (listCriteriaNotIn.size() > 0)
		{
			reqSQIn.append(" and PH.NumPhoto not in (");
			reqSQIn.append(
				"SELECT distinct PH.NumPhoto FROM PHOTOS as PH INNER JOIN FOLDERCATALOG as FC ON PH.NumFolderCatalog = FC.NumFolderCatalog INNER JOIN PHOTOSCRITERIA as PHCR ON PH.NumPhoto = PHCR.NumPhoto INNER JOIN CRITERIA as CR ON CR.NumCriteria = PHCR.NumCriteria WHERE FC.NumCatalog = ");
			reqSQIn.append(to_string(numCatalog));
			reqSQIn.append(" and PH.NumFolderCatalog in (");
			reqSQIn.append(GetSearchSQL(listFolder));
			reqSQIn.append(" and CR.NumCriteria in (");
			reqSQIn.append(GetSearchSQL(listCriteriaNotIn));
			reqSQIn.append(")");
		}

		if (listFaceNotIn.size() > 0)
		{
			wxString value = wxString::Format(wxT("%f"), pertinence / 100.0f);
			reqSQIn.append(" and PH.NumPhoto in (");
			reqSQIn.append(
				"select NumPhoto From Photos where FullPath in (Select distinct FullPath From FACE_RECOGNITION INNER JOIN FACEPHOTO ON FACEPHOTO.NumFace = FACE_RECOGNITION.NumFace WHERE FACEPHOTO.Pertinence > "
				+ value + " and NumFaceCompatible in (");
			reqSQIn.append(GetSearchSQL(listFaceSelected));
			reqSQIn.append("))");
		}

		if (listStarSelected.size() > 0 && listKeywordSelected.size() > 0)
		{
			reqSQIn.append(" and PH.NumPhoto in (");
			reqSQIn.append(
				"SELECT distinct PH.NumPhoto FROM PHOTOS as PH INNER JOIN PHOTOSCRITERIA as PHCR ON PH.NumPhoto = PHCR.NumPhoto INNER JOIN CRITERIA as CR ON CR.NumCriteria = PHCR.NumCriteria WHERE CR.NumCriteria in (");
			reqSQIn.append(GetSearchSQL(listStarSelected));
			reqSQIn.append(" INTERSECT ");
			reqSQIn.append(
				"SELECT distinct PH.NumPhoto FROM PHOTOS as PH INNER JOIN PHOTOSCRITERIA as PHCR ON PH.NumPhoto = PHCR.NumPhoto INNER JOIN CRITERIA as CR ON CR.NumCriteria = PHCR.NumCriteria WHERE CR.NumCriteria in (");
			reqSQIn.append(GetSearchSQL(listKeywordSelected));
			reqSQIn.append(")");
		}
		else if (listStarSelected.size() > 0 || listKeywordSelected.size() > 0)
		{
			reqSQIn.append(" and PH.NumPhoto in (");
			reqSQIn.append(
				"SELECT distinct PH.NumPhoto FROM PHOTOS as PH INNER JOIN PHOTOSCRITERIA as PHCR ON PH.NumPhoto = PHCR.NumPhoto INNER JOIN CRITERIA as CR ON CR.NumCriteria = PHCR.NumCriteria WHERE CR.NumCriteria in (");
			if (listStarSelected.size() != 0)
				reqSQIn.append(GetSearchSQL(listStarSelected));
			else if (listKeywordSelected.size() != 0)
				reqSQIn.append(GetSearchSQL(listKeywordSelected));
			reqSQIn.append(")");
		}

		reqSQIn += ") Group By NumPhoto";
		printf("Requete Photos Search Criteria : %s \n", CConvertUtility::ConvertToUTF8(reqSQIn));
		//ExecuteRequest(reqSQIn);
		return reqSQIn;
	}


	return "";
}

bool CSqlFindPhotos::SearchPhotos(const wxString& sqlRequest)
{
	if (sqlRequest != "" && !FindIfViewExist())
	{
		//DeleteAllInSearchPhotos();
		int pos = sqlRequest.find("INSERT INTO PHOTOSSEARCHCRITERIA");
		if (pos > 0)
		{
			wxString sql = sqlRequest;
			sql.Replace("INSERT INTO PHOTOSSEARCHCRITERIA (NumPhoto,FullPath, CreateDate, GeoGps)",
			            "CREATE VIEW PHOTOSSEARCHCRITERIA (NumPhoto,FullPath, CreateDate, GeoGps) AS ");
			return (ExecuteRequestWithNoResult(sql) != -1) ? true : false;
		}
		ExecuteRequestWithNoResult(sqlRequest);
		return true;
	}
	if (FindIfViewExist())
	{
		ExecuteRequestWithNoResult("DROP VIEW PHOTOSSEARCHCRITERIA");
		ExecuteRequestWithNoResult(sqlRequest);
		return true;
	}


	return false;
}

bool CSqlFindPhotos::DeleteAllInSearchPhotos()
{
	return true; // return (ExecuteRequestWithNoResult("Delete From PHOTOSSEARCHCRITERIA") != -1) ? true : false;
}

bool CSqlFindPhotos::SearchPhotos(PhotosVector* photosVector, const int& numCategorie, const int& numCatalog)
{
	m_photosVector = photosVector;
	return (ExecuteRequest(
		       "SELECT distinct PH.NumPhoto, PH.FullPath FROM PHOTOS as PH INNER JOIN FOLDERCATALOG as FC ON PH.NumFolderCatalog = FC.NumFolderCatalog INNER JOIN PHOTOSCRITERIA as PHCR ON PH.NumPhoto = PHCR.NumPhoto INNER JOIN CRITERIA as CR ON CR.NumCriteria = PHCR.NumCriteria WHERE FC.NumCatalog = "
		       + to_string(numCatalog) + " AND CR.NumCategorie = " + to_string(numCategorie)) != -1)
		       ? true
		       : false;
}

int CSqlFindPhotos::TraitementResult(CSqlResult* sqlResult)
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
	case 2:
		nbResult = TraitementResultPhotoDataCriteria(sqlResult);
		break;
	case 4:
		{
			while (sqlResult->Next())
			{
				for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
				{
					switch (i)
					{
					case 0:
						table_name = sqlResult->ColumnDataText(i);
						break;
					default: ;
					}
				}
				nbResult++;
			}
		}
		break;
	default: ;
	}

	return nbResult;
}

int CSqlFindPhotos::TraitementResultPhotoDataCriteria(CSqlResult* sqlResult)
{
	CSqlResource sqlResource;
	wxString listMonth = sqlResource.GetLibelle(L"LBLMONTHNAME", 1);
	vector<wxString> MonthName = CConvertUtility::split(listMonth, ',');
	wxString listDay = sqlResource.GetLibelle(L"LBLDAYNAME", 1);
	vector<wxString> DayName = CConvertUtility::split(listDay, ',');

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
			case 4:
				_cPhoto.year = sqlResult->ColumnDataInt(i);
				break;
			case 5:
				_cPhoto.month = sqlResult->ColumnDataInt(i);
				if (_cPhoto.month > 0)
					_cPhoto.monthName = MonthName[_cPhoto.month - 1];
				else
					_cPhoto.monthName = "";
				break;
			case 6:
				_cPhoto.day = sqlResult->ColumnDataInt(i);
				break;
			case 7:
				_cPhoto.dayofweek = sqlResult->ColumnDataInt(i);
				_cPhoto.dayName = DayName[_cPhoto.dayofweek];
				break;
			default: ;
			}
		}
		m_photosVector->push_back(_cPhoto);
		nbResult++;
	}
	return nbResult;
}

int CSqlFindPhotos::TraitementResultPhoto(CSqlResult* sqlResult)
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
			default: ;
			}
		}
		m_photosVector->push_back(_cPhoto);
		nbResult++;
	}
	return nbResult;
}

int CSqlFindPhotos::TraitementResultNumPhoto(CSqlResult* sqlResult)
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
			default: ;
			}
		}
		nbResult++;
	}
	return nbResult;
}
