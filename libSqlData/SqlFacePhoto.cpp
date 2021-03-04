#include <header.h>
#include "SqlFacePhoto.h"
#include "SqlFaceLabel.h"
#include "SqlFaceRecognition.h"
#include "SqlFindFacePhoto.h"
#include "SqlResult.h"
#include "SqlPhotos.h"
#include <FileUtility.h>
#include <wx/mstream.h>
#include <libPicture.h>
#include <wx/file.h>
#include <wx/dir.h>
using namespace Regards::Sqlite;
using namespace Regards::Picture;

CSqlFacePhoto::CSqlFacePhoto()
	: CSqlExecuteRequest(L"RegardsDB")
{

}


CSqlFacePhoto::~CSqlFacePhoto()
{
}

int CSqlFacePhoto::UpdateVideoFace(const int & numFace, const int &videoPosition)
{
	return ExecuteRequestWithNoResult("INSERT INTO FACEVIDEO (NumFace, videoPosition) VALUES (" + to_string(numFace) + "," + to_string(videoPosition) + ")");
}

int CSqlFacePhoto::GetVideoFacePosition(const int & numFaceid)
{
	numFace = 0;
	type = 2;
	ExecuteRequest("SELECT videoPosition FROM FACEVIDEO WHERE NumFace = " + to_string(numFaceid));
	return numFace;
}

vector<int> CSqlFacePhoto::GetAllNumFace()
{
	listFaceIndex.clear();
	type = 4;
	ExecuteRequest("SELECT (Select NumFace FROM FACE_RECOGNITION WHERE FACE_RECOGNITION.NumFaceCompatible = FACEPHOTO.NumFace) as NumFaceCompatible FROM FACEPHOTO");
	return listFaceIndex;
}

vector<int> CSqlFacePhoto::GetAllNumFace(const int &numFace)
{
	listFaceIndex.clear();
	type = 4;
	ExecuteRequest("SELECT (Select NumFace FROM FACE_RECOGNITION WHERE FACE_RECOGNITION.NumFaceCompatible = FACEPHOTO.NumFace) as NumFaceCompatible FROM FACEPHOTO where NumFace != " + to_string(numFace));
	return listFaceIndex;
}


vector<CImageLoadingFormat *> CSqlFacePhoto::GetAllFace()
{
	/*
	type = 3;
	listFace.clear();
	ExecuteRequest("SELECT FullPath, width, height, Face FROM FACEPHOTO");
	return listFace;
	*/
	vector<CImageLoadingFormat *> listFace;
	wxArrayString files;
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
#ifdef WIN32
	documentPath.append("\\Face");
#else
	documentPath.append("/Face");
#endif

	wxDir::GetAllFiles(documentPath, &files, wxEmptyString, wxDIR_FILES);
	if (files.size() > 0)
		sort(files.begin(), files.end());

	for (wxString file : files)
	{
		CLibPicture libPicture;
		listFace.push_back(libPicture.LoadPicture(file));
	}

	return listFace;
}

vector<CImageLoadingFormat *> CSqlFacePhoto::GetAllFace(const int &numFace)
{
	/*
	type = 3;
	listFace.clear();
	ExecuteRequest("SELECT FullPath, width, height, Face FROM FACEPHOTO where NumFace != " + to_string(numFace));
	return listFace;
	*/
	vector<CImageLoadingFormat *> listFace;
	wxArrayString files;
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
#ifdef WIN32
	documentPath.append("\\Face");
#else
	documentPath.append("/Face");
#endif

	wxString thumbnail = CFileUtility::GetFaceThumbnailPath(numFace);
	wxDir::GetAllFiles(documentPath, &files, wxEmptyString, wxDIR_FILES);
	if (files.size() > 0)
		sort(files.begin(), files.end());

	for (wxString file : files)
	{
		CLibPicture libPicture;
		if(file != thumbnail)
			listFace.push_back(libPicture.LoadPicture(file));
	}

	return listFace;
}

CImageLoadingFormat * CSqlFacePhoto::GetFacePicture(const int &numFace)
{
	CImageLoadingFormat * picture = nullptr;
	wxString thumbnail = CFileUtility::GetFaceThumbnailPath(numFace);
	if (wxFileExists(thumbnail))
	{
		CLibPicture libPicture;
		picture = libPicture.LoadPicture(thumbnail);
	}
	return picture;

	/*
	type = 3;
	listFace.clear();
	ExecuteRequest("SELECT FullPath, width, height, Face FROM FACEPHOTO where NumFace = " + to_string(numFace));
	if (listFace.size() > 0)
		return listFace[0];
	return nullptr;
	*/
}

bool CSqlFacePhoto::DeleteListOfPhoto(const vector<int> & listNumPhoto)
{
	type = 2;
	for(int i = 0;i < listNumPhoto.size();i++)
	{
		int numPhoto = listNumPhoto[i];
		CSqlPhotos sqlPhoto;
		wxString path = sqlPhoto.GetPhotoPath(numPhoto);

		CSqlFindFacePhoto findFacePhoto;
		std::vector<CFaceName> listFace = findFacePhoto.GetListFaceNum(path);
		for (CFaceName facename : listFace)
		{
			wxString thumbnail = CFileUtility::GetFaceThumbnailPath(facename.numFace);
			if (wxFileExists(thumbnail))
			{
				wxRemoveFile(thumbnail);
			}

			ExecuteRequestWithNoResult("DELETE FROM FACEPHOTO WHERE NumFace = " + to_string(facename.numFace));
			ExecuteRequestWithNoResult("DELETE FROM FACE_RECOGNITION WHERE NumFace = " + to_string(facename.numFace));
			ExecuteRequestWithNoResult("DELETE FROM FACEDESCRIPTOR WHERE NumFace = " + to_string(facename.numFace));
		}
		ExecuteRequestWithNoResult("DELETE FROM FACE_PROCESSING WHERE fullpath in (select fullpath from Photos where NumPhoto = " + to_string(numPhoto) + ")");
	}
    
	RebuildLink();
	return 0;
}

void CSqlFacePhoto::RebuildLink()
{
	//Recomposition des liens entre photos
	int oldNumFace = numFace;
	CSqlFaceRecognition faceRecognition;
	CSqlFaceLabel faceLabel;
	vector<int> listFace = faceLabel.GetFaceLabelAlone();
	for(int i = 0;i < listFace.size();i++)
	{
		numFace = -1;
		ExecuteRequest("SELECT numFace FROM FACE_RECOGNITION WHERE NumFaceCompatible = " + to_string(oldNumFace) + " ORDER BY numFace ASC LIMIT 1");
		if(numFace != -1)
		{
			faceLabel.UpdateNumFaceLabel(oldNumFace, numFace);
			faceRecognition.UpdateFaceRecognition(oldNumFace, numFace);
		}
		
	}
    DeleteFaceNameAlone();
}

bool CSqlFacePhoto::DeleteListOfPhoto(const vector<wxString> & listPhoto)
{
	type = 2;
	for(int i = 0;i < listPhoto.size();i++)
	{
		wxString fullpath = listPhoto[i];
		fullpath.Replace("'", "''");
		//ExecuteRequest("SELECT NumFace FROM FACEPHOTO WHERE fullpath = '" + fullpath + "'");

		CSqlFindFacePhoto findFacePhoto;
		std::vector<CFaceName> listFace = findFacePhoto.GetListFaceNum(listPhoto[i]);
		for (CFaceName facename : listFace)
		{
			wxString thumbnail = CFileUtility::GetFaceThumbnailPath(facename.numFace);
			if (wxFileExists(thumbnail))
			{
				wxRemoveFile(thumbnail);
			}

			ExecuteRequestWithNoResult("DELETE FROM FACEPHOTO WHERE NumFace = " + to_string(facename.numFace));
			ExecuteRequestWithNoResult("DELETE FROM FACE_RECOGNITION WHERE NumFace = " + to_string(facename.numFace));
			ExecuteRequestWithNoResult("DELETE FROM FACEDESCRIPTOR WHERE NumFace = " + to_string(facename.numFace));
		}

		ExecuteRequestWithNoResult("DELETE FROM FACE_PROCESSING WHERE fullpath = '" + fullpath + "'");
	}
	RebuildLink();
	return 0;
}

void CSqlFacePhoto::DeleteFaceNameAlone()
{
    ExecuteRequestWithNoResult("DELETE FROM FACE_NAME WHERE NumFace not in (select NumFace from FACEPHOTO)");
}

bool CSqlFacePhoto::DeleteFaceTreatmentDatabase()
{
	ExecuteRequestWithNoResult("DELETE FROM FACE_PROCESSING");
	return 0;
}

vector<wxString> CSqlFacePhoto::GetPhotoList()
{
	listPhoto.clear();
	type = 1;
	ExecuteRequest("SELECT FullPath FROM PHOTOS WHERE FullPath not in (select distinct FullPath FROM FACEPHOTO)");
	return listPhoto;
}

vector<wxString> CSqlFacePhoto::GetPhotoListTreatment()
{
	listPhoto.clear();
	type = 1;
	ExecuteRequest("SELECT FullPath FROM PHOTOS WHERE FullPath not in (select FullPath FROM FACE_PROCESSING)");
	return listPhoto;
}

int CSqlFacePhoto::InsertFaceTreatment(const wxString & path)
{
	wxString fullpath = path;
	fullpath.Replace("'", "''");
	ExecuteRequest("INSERT INTO FACE_PROCESSING (FullPath) VALUES('" + fullpath + "')");
	return 0;
}

//--------------------------------------------------------
//Chargement de toutes les données d'un album
//--------------------------------------------------------
int CSqlFacePhoto::InsertFace(const wxString & path,const int &numberface, const int & width, const int &height, const double &pertinence, const uint8_t *zBlob, const int &nBlob)
{
	wxString fullpath = path;
	fullpath.Replace("'", "''");
	wxString value = wxString::Format(wxT("%f"), pertinence);
	ExecuteRequest("INSERT INTO FACEPHOTO (FullPath, Numberface, width, height, Pertinence) VALUES('" + fullpath + "'," + to_string(numberface) + "," + to_string(width) + "," + to_string(height) + "," + value + ")");

	int numFaceId = GetNumFace(path, numberface);

	wxString thumbnail = CFileUtility::GetFaceThumbnailPath(numFaceId);
	wxFile fileOut;
	fileOut.Create(thumbnail, true);
	fileOut.Write(zBlob, nBlob);
	fileOut.Close();
	
	return numFaceId;
}

int CSqlFacePhoto::GetNumFace(const wxString & path, const int &numberface)
{
	wxString fullpath = path;
	numFace = 0;
	type = 2;
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT NumFace FROM FACEPHOTO WHERE FullPath = '" + fullpath + "' and Numberface = " + to_string(numberface));
	return numFace;
}

wxImage CSqlFacePhoto::GetFace(const int &numFace)
{
	/*
	bitmap.Destroy();
	type = 0;
	ExecuteRequest("SELECT FullPath, width, height, Face FROM FACEPHOTO WHERE NumFace = " + to_string(numFace));
	return bitmap;
	*/
	wxString thumbnail = CFileUtility::GetFaceThumbnailPath(numFace);
	wxImage image;
	if (wxFileExists(thumbnail))
		image.LoadFile(thumbnail, wxBITMAP_TYPE_JPEG);
	return image;
}



bool CSqlFacePhoto::DeletePhotoFaceDatabase(const wxString & path)
{
	wxString fullpath = path;
	fullpath.Replace("'", "''");

	type = 7;
	listFace.clear();
	ExecuteRequest("SELECT NumFace FROM FACEPHOTO WHERE FullPath = '" + fullpath + "'");
	for (int idFace : listFace)
	{
		wxString thumbnail = CFileUtility::GetFaceThumbnailPath(idFace);
		if (wxFileExists(thumbnail))
		{
			wxRemoveFile(thumbnail);
		}
	}

	return (ExecuteRequestWithNoResult("DELETE FROM FACEPHOTO WHERE FullPath = '" + fullpath + "'") != -1) ? true : false;
}

bool CSqlFacePhoto::DeleteFaceDatabase()
{
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
#ifdef WIN32
	documentPath.append("\\Face");
#else
	documentPath.append("/Face");
#endif

	wxRmdir(documentPath);

	return (ExecuteRequestWithNoResult("DELETE FROM FACEPHOTO") != -1) ? true : false;
}

int CSqlFacePhoto::TraitementResult(CSqlResult * sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
		{
			if(type == 1)
			{
				switch (i)
				{
					case 0:
						filename = sqlResult->ColumnDataText(i);
						break;
				}
				listPhoto.push_back(filename);
			}
			else if(type == 2)
			{
				switch (i)
				{
					case 0:
						numFace = sqlResult->ColumnDataInt(i);
						break;
				}
			}
			else if (type == 4)
			{
				int numFace;
				switch (i)
				{
				case 0:
					numFace = sqlResult->ColumnDataInt(i);
					break;
				}
				listFaceIndex.push_back(numFace);
			}
			else if (type == 7)
			{
				for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
				{
					switch (i)
					{
					case 0:
						listFace.push_back(sqlResult->ColumnDataInt(i));
						break;
					}
				}
			}
		}
		nbResult++;
	}
	return nbResult;
}