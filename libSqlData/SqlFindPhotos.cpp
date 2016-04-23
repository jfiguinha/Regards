#include "SqlFindPhotos.h"
using namespace Regards::Sqlite;

CSqlFindPhotos::CSqlFindPhotos()
	: CSqlExecuteRequest(L"RegardsDB")
{
}


CSqlFindPhotos::~CSqlFindPhotos()
{
}

bool CSqlFindPhotos::SearchPhotos(PhotosVector * photosVector, const wxString &libelleCriteria)
{
	m_photosVector = photosVector;
	return (ExecuteRequest("SELECT distinct PH.NumPhoto, PH.FullPath FROM PHOTOSSEARCHCRITERIA as PH INNER JOIN PHOTOSCRITERIA as PHCR ON PH.NumPhoto = PHCR.NumPhoto INNER JOIN CRITERIA as CR ON CR.NumCriteria = PHCR.NumCriteria WHERE CR.Libelle like '" + libelleCriteria + "%'") != -1) ? true : false;
}

bool CSqlFindPhotos::SearchPhotos(PhotosVector * photosVector, const wxString &localisation, const wxString &libelleCriteria)
{
	m_photosVector = photosVector;
	return (ExecuteRequest("Select distinct PSC.NumPhoto, PSC.FullPath from PHOTOSSEARCHCRITERIA PSC inner join PhotosCriteria PC on PC.NumPhoto = PSC.NumPhoto inner join CRITERIA C on C.NumCriteria = PC.NumCriteria WHERE NumCategorie = 1 and Libelle = '" + localisation + "' and PSC.NumPhoto in(Select PSC.NumPhoto from PHOTOSSEARCHCRITERIA PSC inner join PhotosCriteria PC on PC.NumPhoto = PSC.NumPhoto inner join CRITERIA C on C.NumCriteria = PC.NumCriteria where C.Libelle = '" + libelleCriteria + "' and C.NumCategorie = 3)") != -1) ? true : false;
}

bool CSqlFindPhotos::SearchPhotos(PhotosVector * photosVector)
{
	m_photosVector = photosVector;
	return (ExecuteRequest("SELECT NumPhoto, FullPath FROM PHOTOSSEARCHCRITERIA") != -1) ? true : false;
}

bool CSqlFindPhotos::SearchPhotos(vector<int> * listPhoto)
{
	typeResult = 1;
	m_listPhoto = listPhoto;
	return (ExecuteRequest("SELECT NumPhoto FROM PHOTOSSEARCHCRITERIA") != -1) ? true : false;
}


bool CSqlFindPhotos::SearchPhotos(PhotosVector * photosVector, const int &numCatalog)
{
	typeResult = 0;
	m_photosVector = photosVector;
	return (ExecuteRequest("SELECT distinct PH.NumPhoto, PH.FullPath FROM PHOTOS as PH INNER JOIN FOLDERCATALOG as FC ON PH.NumFolderCatalog = FC.NumFolderCatalog INNER JOIN PHOTOSCRITERIA as PHCR ON PH.NumPhoto = PHCR.NumPhoto INNER JOIN CRITERIA as CR ON CR.NumCriteria = PHCR.NumCriteria WHERE FC.NumCatalog = " + to_string(numCatalog)) != -1) ? true : false;
}

wxString CSqlFindPhotos::GetSearchSQL(vector<int> list)
{
	wxString req;
	for (vector<int>::iterator fit = list.begin(); fit != list.end(); fit++)
	{
		req.append(std::to_string(*fit));
		req.append(",");
	}
	req = req.substr(0, req.length() - 1);
	req.append(")");
	return req;
}

bool CSqlFindPhotos::SearchPhotos(const int &numCatalog, vector<int> listFolder, vector<int> listCriteriaNotIn)
{
	DeleteAllInSearchPhotos();
	//Request In
	wxString reqSQIn = "INSERT INTO PHOTOSSEARCHCRITERIA (NumPhoto,FullPath) SELECT distinct PH.NumPhoto, PH.FullPath FROM PHOTOS as PH INNER JOIN FOLDERCATALOG as FC ON PH.NumFolderCatalog = FC.NumFolderCatalog WHERE FC.NumCatalog = ";
	reqSQIn.append(std::to_string(numCatalog));

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
			reqSQIn.append(std::to_string(numCatalog));
			reqSQIn.append(" and PH.NumFolderCatalog in (");
			reqSQIn.append(GetSearchSQL(listFolder));
			reqSQIn.append(" and CR.NumCriteria in (");
			reqSQIn.append(GetSearchSQL(listCriteriaNotIn));
			reqSQIn.append(")");
		}
		return (ExecuteRequestWithNoResult(reqSQIn) != -1) ? true : false;
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
		for (int i = 0; i < sqlResult->GetColumnCount(); i++)
		{

			switch (i)
			{
			case 0:
				_cPhoto.SetId(sqlResult->ColumnDataInt(i));
				break;
			case 1:
				_cPhoto.SetPath(sqlResult->ColumnDataText(i));
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
		for (int i = 0; i < sqlResult->GetColumnCount(); i++)
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