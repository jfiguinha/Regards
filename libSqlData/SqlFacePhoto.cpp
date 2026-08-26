#include <header.h>
#include "SqlFacePhoto.h"
#include "SqlFaceLabel.h"
#include "SqlFaceRecognition.h"
#include "SqlFindFacePhoto.h"
#include "SqlResult.h"
#include "SqlPhotos.h"
#include <FileUtility.h>
#include <ImageLoadingFormat.h>
#include <libPicture.h>
#include "ThumbnailBuffer.h"
#include <wx/file.h>
#include <wx/dir.h>
#include <SqlParameter.h>
#include <wx/filename.h>
using namespace Regards::Sqlite;
using namespace Regards::Picture;

CSqlFacePhoto::CSqlFacePhoto()
	: CSqlExecuteRequest(L"RegardsDB"), numFace(0), type(0)
{
}


int CSqlFacePhoto::UpdateVideoFace(const int& numFace, const int& videoPosition)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFace));
	parameter.push_back(std::make_unique<CSqlInt>(videoPosition));
	return ExecuteSqlWithStatementNoResult("INSERT INTO FACEVIDEO (NumFace, videoPosition) VALUES (?, ?)", parameter);
}

int CSqlFacePhoto::GetVideoFacePosition(const int& numFaceid)
{
	type = 2;
	videoPosition = 0;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFaceid));
	ExecuteSqlWithStatement("SELECT videoPosition FROM FACEVIDEO WHERE NumFace = ?", parameter);
	return videoPosition;
}

bool CSqlFacePhoto::DeleteNumFaceMaster(const int& numFace)
{
	listFace.clear();
	type = 7;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFace));
	ExecuteSqlWithStatement("Select distinct NumFace FROM FACE_RECOGNITION WHERE NumFaceCompatible = ?", parameter);

	if (listFace.size() > 0)
	{
		for (int faceId : listFace)
		{
			wxString thumbnail = CFileUtility::GetFaceThumbnailPath(faceId);
			if (wxFileExists(thumbnail))
				wxRemoveFile(thumbnail);

			thumbnail = CFileUtility::GetFaceZScorePath(faceId);
			if (wxFileExists(thumbnail))
				wxRemoveFile(thumbnail);

			std::vector<std::unique_ptr<CSqlParameter>> parameter;
			parameter.push_back(std::make_unique<CSqlInt>(faceId));
			ExecuteSqlWithStatementNoResult("DELETE FROM FACEPHOTO WHERE NumFace = ?", parameter);
			ExecuteSqlWithStatementNoResult("DELETE FROM FACEVIDEO WHERE NumFace = ?", parameter);
		}
	}

	{
		std::vector<std::unique_ptr<CSqlParameter>> parameter;
		parameter.push_back(std::make_unique<CSqlInt>(numFace));
		ExecuteSqlWithStatementNoResult("DELETE FROM FACE_RECOGNITION WHERE NumFaceCompatible = ?", parameter);
		ExecuteSqlWithStatementNoResult("DELETE FROM FACE_NAME WHERE NumFace = ?", parameter);
		DeleteFaceNameAlone();
	}


	return true;
}

void CSqlFacePhoto::EraseFace(const int& numFace)
{
	type = 3;
	filename = "";
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFace));
	ExecuteSqlWithStatement("Select FullPath FROM FACEPHOTO WHERE NumFace = ?", parameter);
	if (filename != "")
	{
		DeletePhotoFaceDatabase(filename);
		InsertFaceTreatment(filename);
		RebuildLink();
	}
}

void CSqlFacePhoto::DeleteNumFace(const int& numFace)
{
	wxString thumbnail = CFileUtility::GetFaceThumbnailPath(numFace);
	if (wxFileExists(thumbnail))
		wxRemoveFile(thumbnail);

	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFace));
	ExecuteSqlWithStatementNoResult("DELETE FROM FACEPHOTO WHERE NumFace = ?", parameter);
	ExecuteSqlWithStatementNoResult("DELETE FROM FACEVIDEO WHERE NumFace = ?", parameter);
	ExecuteSqlWithStatementNoResult("DELETE FROM FACE_RECOGNITION WHERE NumFace = ?", parameter);
	DeleteFaceNameAlone();
}

int CSqlFacePhoto::GetFaceCompatibleRecognition(const int& numFace)
{
	listFace.clear();
	type = 7;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFace));
	ExecuteSqlWithStatement("Select distinct NumFaceCompatible FROM FACE_RECOGNITION WHERE NumFace = ? ", parameter);
	if (listFace.size() > 0)
		return listFace[0];
	return -1;
}

vector<CFaceRecognitionData> CSqlFacePhoto::GetAllNumFaceRecognition()
{
	listFaceRecognition.clear();
	type = 5;
	ExecuteRequest("Select NumFace, NumFaceCompatible FROM FACE_RECOGNITION ORDER BY NumFaceCompatible");
	return listFaceRecognition;
}


vector<int> CSqlFacePhoto::GetAllThumbnailFace()
{
	listFace.clear();
	type = 7;
	ExecuteRequest("Select NumFace FROM FACEPHOTO");
	return listFace;
}

vector<int> CSqlFacePhoto::GetAllNumFace()
{
	listFace.clear();
	type = 7;
	ExecuteRequest("SELECT (Select NumFace FROM FACE_RECOGNITION WHERE FACE_RECOGNITION.NumFaceCompatible = FACEPHOTO.NumFace) as NumFaceCompatible FROM FACEPHOTO");
	return listFace;
}

vector<int> CSqlFacePhoto::GetAllNumFace(const int& numFace)
{
	listFace.clear();
	type = 7;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFace));
	ExecuteSqlWithStatement("SELECT (Select NumFace FROM FACE_RECOGNITION WHERE FACE_RECOGNITION.NumFaceCompatible = FACEPHOTO.NumFace) as NumFaceCompatible FROM FACEPHOTO where NumFace != ?", parameter);
	return listFace;
}



CImageLoadingFormat* CSqlFacePhoto::GetFacePicture(const int& numFace)
{
	CImageLoadingFormat* picture = nullptr;
	wxString thumbnail = CFileUtility::GetFaceThumbnailPath(numFace);
	if (wxFileExists(thumbnail))
	{
		CLibPicture libPicture;
		picture = libPicture.LoadPicture(thumbnail);
		//picture->Flip();
	}
	return picture;

}

bool CSqlFacePhoto::DeleteListOfPhoto(const vector<int>& listNumPhoto)
{
	for (int numPhoto : listNumPhoto)
	{
		CSqlPhotos sqlPhoto;
		wxString path = sqlPhoto.GetPhotoPath(numPhoto);

		CSqlFindFacePhoto findFacePhoto;
		std::vector<int> listFace = findFacePhoto.GetListFaceNum(path);

		for (int numFace : listFace)
			DeleteNumFace(numFace);

		std::vector<std::unique_ptr<CSqlParameter>> parameter;
		parameter.push_back(std::make_unique<CSqlInt>(numFace));
		ExecuteSqlWithStatementNoResult("DELETE FROM FACE_PROCESSING WHERE fullpath in (select fullpath from Photos where NumPhoto = ?)", parameter);
	}
	RebuildLink();
	return false;
}

void CSqlFacePhoto::RebuildLink()
{
	//Recomposition des liens entre photos
	CSqlFaceLabel faceLabel;
	vector<int> listFace = faceLabel.GetFaceLabelAlone();

	for (auto i = 0; i < listFace.size(); i++)
	{
		int oldNumFace = listFace[i];
		numFace = -1;
		type = 0;
		std::vector<std::unique_ptr<CSqlParameter>> parameter;
		parameter.push_back(std::make_unique<CSqlInt>(oldNumFace));
		ExecuteSqlWithStatement("SELECT numFace FROM FACE_RECOGNITION WHERE NumFaceCompatible = ? ORDER BY numFace ASC LIMIT 1", parameter);
		if (numFace != -1)
		{
			CSqlFaceRecognition faceRecognition;
			CSqlFaceLabel faceLabel;
			faceLabel.UpdateNumFaceLabel(oldNumFace, numFace);
			faceRecognition.UpdateFaceRecognition(oldNumFace, numFace);
		}
	}

	DeleteFaceNameAlone();

}

bool CSqlFacePhoto::DeleteListOfPhoto(const vector<wxString>& listPhotoToDelete)
{
	type = 2;
	for (wxString fullpath : listPhotoToDelete)
	{
		CSqlFindFacePhoto findFacePhoto;
		std::vector<int> listFace = findFacePhoto.GetListFaceNum(fullpath);

		for (int numFace : listFace)
			DeleteNumFace(numFace);
		

		std::vector<std::unique_ptr<CSqlParameter>> parameter;
		parameter.push_back(std::make_unique<CSqlString>(fullpath));
		ExecuteSqlWithStatementNoResult("DELETE FROM FACE_PROCESSING WHERE fullpath = ?", parameter);
	}
	RebuildLink();
	return false;
}

void CSqlFacePhoto::DeleteFaceNameAlone()
{
	ExecuteRequestWithNoResult("DELETE FROM FACE_NAME WHERE NumFace not in (select NumFace from FACEPHOTO)");
}

bool CSqlFacePhoto::DeleteFaceTreatmentDatabase()
{
	ExecuteRequestWithNoResult("DELETE FROM FACE_PROCESSING");
	return false;
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
	ExecuteRequest("SELECT FullPath FROM PHOTOS WHERE FullPath not in (select FullPath FROM FACE_PROCESSING) ");
	return listPhoto;
}

int CSqlFacePhoto::InsertFaceTreatment(const wxString& path)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(path));
	return ExecuteSqlWithStatementNoResult("INSERT INTO FACE_PROCESSING (FullPath) VALUES (?)", parameter);
}

//--------------------------------------------------------
//Chargement de toutes les données d'un album
//--------------------------------------------------------
int CSqlFacePhoto::InsertFace(const wxString& path, const wxString& gender, const wxString& age, const int& numberface, const int& width, const int& height,
                              const double& pertinence, const uint8_t* zBlob, const int& nBlob)
{
	wxString value = wxString::Format(wxT("%f"), pertinence);

	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(path));
	parameter.push_back(std::make_unique<CSqlInt>(numberface));
	parameter.push_back(std::make_unique<CSqlInt>(width));
	parameter.push_back(std::make_unique<CSqlInt>(height));
	parameter.push_back(std::make_unique<CSqlString>(value));
	parameter.push_back(std::make_unique<CSqlString>(gender));
	parameter.push_back(std::make_unique<CSqlString>(age));

	ExecuteSqlWithStatementNoResult("INSERT INTO FACEPHOTO (FullPath, Numberface, width, height, Pertinence, gender, age) VALUES (?,?,?,?,?,?,?)", parameter);

	int numFaceId = GetNumFace(path, numberface);

	wxString thumbnail = CFileUtility::GetFaceThumbnailPath(numFaceId);
	wxFile fileOut;
	fileOut.Create(thumbnail, true);
	fileOut.Write(zBlob, nBlob);
	fileOut.Close();

	return numFaceId;
}

int CSqlFacePhoto::GetNumFace(const wxString& path, const int& numberface)
{
	numFace = 0;
	type = 0;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(path));
	parameter.push_back(std::make_unique<CSqlInt>(numberface));
	ExecuteSqlWithStatement("SELECT NumFace FROM FACEPHOTO WHERE FullPath = ? and Numberface = ?", parameter);
	return numFace;
}

cv::Mat CSqlFacePhoto::GetFace(const int& numFace, bool& isDefault)
{
	wxLogNull logNo;
	wxString thumbnail = CFileUtility::GetFaceThumbnailPath(numFace);
	cv::Mat image;
	if (wxFileExists(thumbnail))
	{
        cv::flip(CThumbnailBuffer::GetPicture(thumbnail),image,-1);
	}

	if (image.empty())
	{
		DeleteNumFace(numFace);
		isDefault = true;
	}
	else
		isDefault = false;

	return image;
}


bool CSqlFacePhoto::DeletePhotoFaceDatabase(const wxString& path)
{
	type = 7;
	listFace.clear();
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(path));
	ExecuteSqlWithStatement("SELECT NumFace FROM FACEPHOTO WHERE FullPath = ?", parameter);
	for (int i : listFace)
	{
		DeleteNumFace(i);
	}

	return true;
}

bool CSqlFacePhoto::DeleteFaceDatabase()
{
	wxFileName documentPath(CFileUtility::GetDocumentFolderPath());
	documentPath.AppendDir("Face");

	wxArrayString files;
	wxDir::GetAllFiles(documentPath.GetFullPath(), &files, wxEmptyString, wxDIR_FILES);

	tbb::parallel_for(0, static_cast<int>(listFace.size()), 1, [=](int i)
	{
		wxString filename = files[i];
		if (wxFileExists(filename))
			wxRemoveFile(filename);
	});

	return (ExecuteRequestWithNoResult("DELETE FROM FACEPHOTO") != -1) ? true : false;
}

int CSqlFacePhoto::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		switch (type)
		{
		case 0:
			numFace = sqlResult->ColumnDataInt(0);
			break;
		case 1:
			listPhoto.push_back(sqlResult->ColumnDataText(0));
			break;
		case 2:
			videoPosition = sqlResult->ColumnDataInt(0);
			break;
		case 3:
			filename = sqlResult->ColumnDataText(0);
			break;
		case 5:
		{
			CFaceRecognitionData data;
			data.numFace = sqlResult->GetInt(0);
			data.numFaceCompatible = sqlResult->GetInt(1);
			listFaceRecognition.push_back(data);
		}
			break;
		case 7:
			listFace.push_back(sqlResult->ColumnDataInt(0));
			break;
		}
		nbResult++;
	}
	return nbResult;
}
