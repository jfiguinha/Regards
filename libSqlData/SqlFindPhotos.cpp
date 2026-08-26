#include <header.h>
#include "SqlFindPhotos.h"
#include "SqlResult.h"
#include <libPicture.h>
#include <SqlResource.h>
#include <ConvertUtility.h>
#include <SQLRemoveData.h>
#include <SqlParameter.h>
using namespace Regards::Picture;
using namespace Regards::Sqlite;

CSqlFindPhotos::CSqlFindPhotos()
	: CSqlExecuteRequest(L"RegardsDB"), m_photosVector(nullptr), m_listPhoto(nullptr)
{
	typeResult = 0;
}

bool CSqlFindPhotos::SearchPhotosByCriteriaFolder(PhotosVector* photosVector)
{
	typeResult = 2;
	m_photosVector = photosVector;
	if (m_photosVector == nullptr)
		return false;

	m_photosVector->clear();
	wxString sqlRequest = "SELECT NumPhoto, FullPath, CreateDate, GeoGps FROM PHOTOSSEARCHCRITERIA  Order By CreateDate";//  Order By Year, Month asc, Day asc, DayOfWeek asc, FullPath";
	bool result = (ExecuteRequest(sqlRequest) != -1) ? 1 : 0;

	if (result)
	{
		if (m_idlistPhotoToDelete.size() > 0)
		{
			CSQLRemoveData::DeleteListPhoto(m_idlistPhotoToDelete, nullptr);
		}
	}

	return result;
}


bool CSqlFindPhotos::GetAllPhotos(PhotosVector* photosVector)
{
	typeResult = 0;
	m_photosVector = photosVector;
	if (m_photosVector == nullptr)
		return false;

	m_photosVector->clear();
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

		std::vector<std::unique_ptr<CSqlParameter>> parameter;
		parameter.push_back(std::make_unique<CSqlInt>(indexId));
		parameter.push_back(std::make_unique<CSqlInt>(photo.GetId()));
		ExecuteSqlWithStatementNoResult("UPDATE PHOTOS SET ExtensionId =  ? WHERE NumPhoto = ? ", parameter);
	}
}

bool CSqlFindPhotos::GetAllVideo(PhotosVector* photosVector)
{
	typeResult = 0;
	m_photosVector = photosVector;
	if (m_photosVector == nullptr)
		return false;

	m_photosVector->clear();
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
	if (m_photosVector == nullptr)
		return false;

	m_photosVector->clear();
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(libelleCriteria + "%"));
	return ExecuteSqlWithStatement("SELECT NumPhoto,FullPath, CreateDate, GeoGps FROM PHOTOSSEARCHCRITERIA WHERE CreateDate like ?", parameter);
}

bool CSqlFindPhotos::SearchPhotos(PhotosVector* photosVector, const wxString& localisation,
                                  const wxString& libelleCriteria)
{
	m_photosVector = photosVector;
	if (m_photosVector == nullptr)
		return false;

	m_photosVector->clear();
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(localisation));
	parameter.push_back(std::make_unique<CSqlString>(libelleCriteria));
	return ExecuteSqlWithStatement("Select NumPhoto,FullPath, CreateDate, GeoGps from PHOTOSSEARCHCRITERIA WHERE GeoGps = ? and CreateDate = ?", parameter);
}

bool CSqlFindPhotos::SearchPhotos(PhotosVector* photosVector)
{
	typeResult = 0;
	m_photosVector = photosVector;
	if (m_photosVector == nullptr)
		return false;

	m_photosVector->clear();
	return (ExecuteRequest(
			       "SELECT NumPhoto,FullPath, CreateDate, GeoGps FROM PHOTOSSEARCHCRITERIA Group By NumPhoto ORDER BY FullPath, GeoGps")
		       != -1)
		       ? true
		       : false;
}

bool CSqlFindPhotos::SearchPhotos(vector<wxString> * fileList)
{
	typeResult = 3;
	this->fileList = fileList;
	if (fileList == nullptr)
		return false;

	fileList->clear();
	return (ExecuteRequest(
		"SELECT FullPath FROM PHOTOSSEARCHCRITERIA Group By NumPhoto ORDER BY FullPath, GeoGps")
		!= -1)
		? true
		: false;
}


bool CSqlFindPhotos::SearchPhotos(vector<int>* listPhoto)
{
	typeResult = 1;
	m_listPhoto = listPhoto;
	if (listPhoto == nullptr)
		return false;

	listPhoto->clear();
	return (ExecuteRequest("SELECT NumPhoto FROM PHOTOSSEARCHCRITERIA") != -1) ? true : false;
}


bool CSqlFindPhotos::SearchPhotosByTypeAffichage(PhotosVector* photosVector, const int& typeAffichage,
                                                 const int& numCatalog)
{
	typeResult = 0;
	m_photosVector = photosVector;
	if (m_photosVector == nullptr)
		return false;

	photosVector->clear();
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



wxString CSqlFindPhotos::GenerateSqlRequest(
	const int& numCatalog,
	vector<int>& listFolder,
	vector<int>& listCriteriaNotIn,
	vector<int>& listFaceNotIn,
	vector<int>& listFaceSelected,
	vector<int>& listStarSelected,
	vector<int>& listStarNotSelected,
	vector<int>& listKeywordSelected,
	vector<int>& listKeywordNotSelected,
	const wxString& libelleNotGeo,
	const double& pertinence)
{
	if (listFolder.empty())
		return "";

	// ---------------------------------------------------------------------
	// Date courante au format YYYY.MM.DD
	// ---------------------------------------------------------------------
	const wxDateTime now = wxDateTime::Now();

	const wxString createDate = wxString::Format(
		"%04d.%02d.%02d",
		now.GetYear(),
		static_cast<int>(now.GetMonth()) + 1,
		now.GetDay());

	wxString reqSQIn;

	// ---------------------------------------------------------------------
	// Création de la vue
	// ---------------------------------------------------------------------
	reqSQIn =
		"CREATE VIEW PHOTOSSEARCHCRITERIA "
		"(NumPhoto, NumFolder, FullPath, CreateDate, GeoGps) AS ";

	reqSQIn += "SELECT * FROM (";

	// =====================================================================
	// 1. Photos sans critères
	// =====================================================================

	reqSQIn +=
		"SELECT "
		"NumPhoto, "
		"NumFolderCatalog, "
		"FullPath, "
		"'" + createDate + "' AS CreateDate, "
		"'" + libelleNotGeo + "' AS GeoGps "
		"FROM PHOTOS "
		"WHERE CriteriaInsert = 0 "
		"AND NumFolderCatalog IN (";

	// GetSearchSQL() ferme déjà la parenthèse
	reqSQIn += GetSearchSQL(listFolder);

	// =====================================================================
	// 2. Photos avec critères
	// =====================================================================

	reqSQIn +=
		" UNION "
		"SELECT "
		"PH.NumPhoto, "
		"PH.NumFolderCatalog, "
		"PH.FullPath, "
		"MAX(CASE "
		"WHEN CR.NumCategorie = 3 THEN CR.Libelle "
		"END) AS CreateDate, "
		"MAX(CASE "
		"WHEN CR.NumCategorie = 1 THEN CR.Libelle "
		"END) AS GeoGps "
		"FROM PHOTOS AS PH "
		"INNER JOIN FOLDERCATALOG AS FC "
		"ON PH.NumFolderCatalog = FC.NumFolderCatalog "
		"INNER JOIN PHOTOSCRITERIA AS PHCR "
		"ON PH.NumPhoto = PHCR.NumPhoto "
		"INNER JOIN CRITERIA AS CR "
		"ON CR.NumCriteria = PHCR.NumCriteria "
		"WHERE FC.NumCatalog = ";

	reqSQIn += to_string(numCatalog);

	reqSQIn +=
		" AND PH.NumFolderCatalog IN (";

	// GetSearchSQL() ferme déjà la parenthèse
	reqSQIn += GetSearchSQL(listFolder);

	// =====================================================================
	// 3. Critères exclus
	// =====================================================================

	if (!listCriteriaNotIn.empty())
	{
		reqSQIn +=
			" AND PH.NumPhoto NOT IN ("
			"SELECT DISTINCT PH.NumPhoto "
			"FROM PHOTOS AS PH "
			"INNER JOIN FOLDERCATALOG AS FC "
			"ON PH.NumFolderCatalog = FC.NumFolderCatalog "
			"INNER JOIN PHOTOSCRITERIA AS PHCR "
			"ON PH.NumPhoto = PHCR.NumPhoto "
			"INNER JOIN CRITERIA AS CR "
			"ON CR.NumCriteria = PHCR.NumCriteria "
			"WHERE FC.NumCatalog = ";

		reqSQIn += to_string(numCatalog);

		reqSQIn +=
			" AND PH.NumFolderCatalog IN (";

		reqSQIn += GetSearchSQL(listFolder);

		reqSQIn +=
			" AND CR.NumCriteria IN (";

		reqSQIn += GetSearchSQL(listCriteriaNotIn);

		reqSQIn += ")";
	}

	// =====================================================================
	// 4. Reconnaissance faciale
	//
	// On conserve exactement la logique originale :
	// test sur listFaceNotIn mais utilisation de listFaceSelected.
	// =====================================================================

	if (!listFaceNotIn.empty())
	{
		const wxString value =
			wxString::Format("%.6f", pertinence / 100.0);

		reqSQIn +=
			" AND PH.NumPhoto IN ("
			"SELECT NumPhoto "
			"FROM PHOTOS "
			"WHERE FullPath IN ("
			"SELECT DISTINCT FullPath "
			"FROM FACE_RECOGNITION "
			"INNER JOIN FACEPHOTO "
			"ON FACEPHOTO.NumFace = FACE_RECOGNITION.NumFace "
			"WHERE FACEPHOTO.Pertinence > ";

		reqSQIn += value;

		reqSQIn +=
			" AND NumFaceCompatible IN (";

		reqSQIn += GetSearchSQL(listFaceSelected);

		reqSQIn += "))";
	}

	// =====================================================================
	// 5. Etoiles + mots-clés
	// =====================================================================

	if (!listStarSelected.empty() &&
		!listKeywordSelected.empty())
	{
		reqSQIn +=
			" AND PH.NumPhoto IN ("
			"SELECT DISTINCT PH.NumPhoto "
			"FROM PHOTOS AS PH "
			"INNER JOIN PHOTOSCRITERIA AS PHCR "
			"ON PH.NumPhoto = PHCR.NumPhoto "
			"INNER JOIN CRITERIA AS CR "
			"ON CR.NumCriteria = PHCR.NumCriteria "
			"WHERE CR.NumCriteria IN (";

		reqSQIn += GetSearchSQL(listStarSelected);

		reqSQIn +=
			" INTERSECT "
			"SELECT DISTINCT PH.NumPhoto "
			"FROM PHOTOS AS PH "
			"INNER JOIN PHOTOSCRITERIA AS PHCR "
			"ON PH.NumPhoto = PHCR.NumPhoto "
			"INNER JOIN CRITERIA AS CR "
			"ON CR.NumCriteria = PHCR.NumCriteria "
			"WHERE CR.NumCriteria IN (";

		reqSQIn += GetSearchSQL(listKeywordSelected);

		reqSQIn += ")";
	}
	else if (!listStarSelected.empty() ||
		!listKeywordSelected.empty())
	{
		reqSQIn +=
			" AND PH.NumPhoto IN ("
			"SELECT DISTINCT PH.NumPhoto "
			"FROM PHOTOS AS PH "
			"INNER JOIN PHOTOSCRITERIA AS PHCR "
			"ON PH.NumPhoto = PHCR.NumPhoto "
			"INNER JOIN CRITERIA AS CR "
			"ON CR.NumCriteria = PHCR.NumCriteria "
			"WHERE CR.NumCriteria IN (";

		if (!listStarSelected.empty())
			reqSQIn += GetSearchSQL(listStarSelected);
		else
			reqSQIn += GetSearchSQL(listKeywordSelected);

		reqSQIn += ")";
	}

	// =====================================================================
	// Une ligne par photo dans la deuxième branche
	// =====================================================================

	reqSQIn +=
		" GROUP BY "
		"PH.NumPhoto, "
		"PH.NumFolderCatalog, "
		"PH.FullPath";

	// =====================================================================
	// Fermeture du SELECT *
	// =====================================================================

	reqSQIn += ") GROUP BY NumPhoto";

	return reqSQIn;
}

/*


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

	reqSQIn = "CREATE VIEW PHOTOSSEARCHCRITERIA (NumPhoto, NumFolder, FullPath, CreateDate, GeoGps) AS ";
	reqSQIn += "SELECT * FROM (";
	reqSQIn += "SELECT NumPhoto, NumFolderCatalog, FullPath, \"" + createDate + "\" as CreateDate, \"" + libelle +
		"\" as GeoGps FROM PHOTOS WHERE CriteriaInsert = 0";
    if (listFolder.size() > 0)
    {
       reqSQIn.append(" and NumFolderCatalog in (");
       reqSQIn.append(GetSearchSQL(listFolder));
    }
	reqSQIn += " UNION ";
	reqSQIn += "SELECT distinct PH.NumPhoto, PH.NumFolderCatalog, PH.FullPath, ";
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

		return reqSQIn;
	}


	return "";
}
*/

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

bool CSqlFindPhotos::SearchPhotos(PhotosVector* photosVector, const int& numCategorie, const int& numCatalog)
{
	m_photosVector = photosVector;
	if (photosVector == nullptr)
		return false;

	photosVector->clear();
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numCatalog));
	parameter.push_back(std::make_unique<CSqlInt>(numCategorie));
	return ExecuteSqlWithStatement("SELECT distinct PH.NumPhoto, PH.FullPath FROM PHOTOS as PH INNER JOIN FOLDERCATALOG as FC ON PH.NumFolderCatalog = FC.NumFolderCatalog INNER JOIN PHOTOSCRITERIA as PHCR ON PH.NumPhoto = PHCR.NumPhoto INNER JOIN CRITERIA as CR ON CR.NumCriteria = PHCR.NumCriteria WHERE FC.NumCatalog = ? AND CR.NumCategorie = ?", parameter);
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
	case 3:
		nbResult = TraitementResultFilename(sqlResult);
		break;
	case 4:
		{
			while (sqlResult->Next())
			{
				table_name = sqlResult->ColumnDataText(0);
				nbResult++;
			}
		}
		break;
	default: ;
	}

	return nbResult;
}

int CSqlFindPhotos::TraitementResultFilename(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		fileList->push_back(sqlResult->ColumnDataText(0));	
		nbResult++;
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

	m_idlistPhotoToDelete.clear();

	int nbResult = 0;
	while (sqlResult->Next())
	{
		wxString filePath = sqlResult->ColumnDataText(1);
		if (!wxFileExists(filePath))
		{
			m_idlistPhotoToDelete.push_back(sqlResult->ColumnDataInt(0));
			continue;
		}
			

		CPhotos _cPhoto;
		_cPhoto.SetId(sqlResult->ColumnDataInt(0));
		_cPhoto.SetPath(sqlResult->ColumnDataText(1));
		_cPhoto.SetCreateDate(sqlResult->ColumnDataText(2));
		_cPhoto.SetGpsInfos(sqlResult->ColumnDataText(3));
		if (_cPhoto.month > 0)
			_cPhoto.monthName = MonthName[_cPhoto.month - 1];
		else
			_cPhoto.monthName = "";
		_cPhoto.dayName = DayName[_cPhoto.GetDayOfWeek()];
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
		_cPhoto.SetId(sqlResult->ColumnDataInt(0));
		_cPhoto.SetPath(sqlResult->ColumnDataText(1));
		_cPhoto.SetCreateDate(sqlResult->ColumnDataText(2));
		_cPhoto.SetGpsInfos(sqlResult->ColumnDataText(3));
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
		m_listPhoto->push_back(sqlResult->ColumnDataInt(0));
		nbResult++;
	}
	return nbResult;
}
