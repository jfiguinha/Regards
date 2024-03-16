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
#include <picture_utility.h>
#include "ThumbnailBuffer.h"
#include <wx/file.h>
#include <wx/dir.h>
using namespace Regards::Sqlite;
using namespace Regards::Picture;

CSqlFacePhoto::CSqlFacePhoto()
	: CSqlExecuteRequest(L"RegardsDB"), numFace(0), type(0)
{
}


CSqlFacePhoto::~CSqlFacePhoto()
{
}

int CSqlFacePhoto::UpdateVideoFace(const int& numFace, const int& videoPosition)
{
	return ExecuteRequestWithNoResult(
		"INSERT INTO FACEVIDEO (NumFace, videoPosition) VALUES (" + to_string(numFace) + "," + to_string(videoPosition)
		+ ")");
}

int CSqlFacePhoto::GetVideoFacePosition(const int& numFaceid)
{
	numFace = 0;
	type = 2;
	ExecuteRequest("SELECT videoPosition FROM FACEVIDEO WHERE NumFace = " + to_string(numFaceid));
	return numFace;
}

bool CSqlFacePhoto::DeleteNumFaceMaster(const int& numFace)
{
	listFaceIndex.clear();
	type = 4;
	ExecuteRequest("Select distinct NumFace FROM FACE_RECOGNITION WHERE NumFaceCompatible = " + to_string(numFace));

	if (listFaceIndex.size() > 0)
	{
		for (int faceId : listFaceIndex)
		{
			wxString thumbnail = CFileUtility::GetFaceThumbnailPath(faceId);
			if (wxFileExists(thumbnail))
			{
				wxRemoveFile(thumbnail);
			}

			thumbnail = CFileUtility::GetFaceZScorePath(faceId);
			if (wxFileExists(thumbnail))
			{
				wxRemoveFile(thumbnail);
			}
			ExecuteRequestWithNoResult("DELETE FROM FACEPHOTO WHERE NumFace = " + to_string(faceId));
			ExecuteRequestWithNoResult("DELETE FROM FACEVIDEO WHERE NumFace = " + to_string(faceId));
		}
	}
	ExecuteRequestWithNoResult("DELETE FROM FACE_RECOGNITION WHERE NumFaceCompatible = " + to_string(numFace));
	ExecuteRequestWithNoResult("DELETE FROM FACE_NAME WHERE NumFace = " + to_string(numFace));
	DeleteFaceNameAlone();

	return true;
}

void CSqlFacePhoto::EraseFace(const int& numFace)
{
	type = 1;
	filename = "";
	ExecuteRequest("Select FullPath FROM FACEPHOTO WHERE NumFace = " + to_string(numFace));
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
	{
		wxRemoveFile(thumbnail);
	}

	ExecuteRequestWithNoResult("DELETE FROM FACEPHOTO WHERE NumFace = " + to_string(numFace));
	ExecuteRequestWithNoResult("DELETE FROM FACEVIDEO WHERE NumFace = " + to_string(numFace));
	ExecuteRequestWithNoResult("DELETE FROM FACE_RECOGNITION WHERE NumFace = " + to_string(numFace));
	DeleteFaceNameAlone();
}

int CSqlFacePhoto::GetFaceCompatibleRecognition(const int& numFace)
{
	listFaceIndex.clear();
	type = 4;
	ExecuteRequest("Select distinct NumFaceCompatible FROM FACE_RECOGNITION WHERE NumFace = " + to_string(numFace));
	if (listFaceIndex.size() > 0)
		return listFaceIndex[0];
	return -1;
}

vector<CFaceRecognitionData> CSqlFacePhoto::GetAllNumFaceRecognition()
{
	listFaceRecognition.clear();
	type = 5;
	ExecuteRequest("Select NumFace, NumFaceCompatible FROM FACE_RECOGNITION ORDER BY NumFaceCompatible");
	return listFaceRecognition;
}

/*
vector<int> CSqlFacePhoto::GetAllNumFaceRecognition(const int& numFace)
{
	listFaceIndex.clear();
	type = 4;
	ExecuteRequest("Select NumFace FROM FACE_RECOGNITION WHERE NumFace != " + to_string(numFace));
	return listFaceIndex;
}
*/

vector<int> CSqlFacePhoto::GetAllThumbnailFace()
{
	listFaceIndex.clear();
	type = 4;
	ExecuteRequest("Select NumFace FROM FACEPHOTO");
	return listFaceIndex;
}

vector<int> CSqlFacePhoto::GetAllNumFace()
{
	listFaceIndex.clear();
	type = 4;
	ExecuteRequest(
		"SELECT (Select NumFace FROM FACE_RECOGNITION WHERE FACE_RECOGNITION.NumFaceCompatible = FACEPHOTO.NumFace) as NumFaceCompatible FROM FACEPHOTO");
	return listFaceIndex;
}

vector<int> CSqlFacePhoto::GetAllNumFace(const int& numFace)
{
	listFaceIndex.clear();
	type = 4;
	ExecuteRequest(
		"SELECT (Select NumFace FROM FACE_RECOGNITION WHERE FACE_RECOGNITION.NumFaceCompatible = FACEPHOTO.NumFace) as NumFaceCompatible FROM FACEPHOTO where NumFace != "
		+ to_string(numFace));
	return listFaceIndex;
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
	type = 2;
	tbb::parallel_for(0, static_cast<int>(listNumPhoto.size()), 1, [=](int i)
	{
		int numPhoto = listNumPhoto[i];
		CSqlPhotos sqlPhoto;
		wxString path = sqlPhoto.GetPhotoPath(numPhoto);

		CSqlFindFacePhoto findFacePhoto;
		std::vector<CFaceName> listFace = findFacePhoto.GetListFaceNum(path);

		for (int i1 = 0; i1 < listFace.size(); ++i1)
		{
			CFaceName facename = listFace[i1];
			DeleteNumFace(facename.numFace);
		}

		ExecuteRequestWithNoResult(
			"DELETE FROM FACE_PROCESSING WHERE fullpath in (select fullpath from Photos where NumPhoto = " +
			to_string(numPhoto) + ")");
	}
    );

	RebuildLink();
	return false;
}

void CSqlFacePhoto::RebuildLink()
{
	//Recomposition des liens entre photos
	CSqlFaceLabel faceLabel;
	vector<int> listFace = faceLabel.GetFaceLabelAlone();

	/*
	for(int i = 0;i < listFace.size();i++)
	{
		int oldNumFace = listFace[i];
		numFace = -1;
		ExecuteRequest("SELECT numFace FROM FACE_RECOGNITION WHERE NumFaceCompatible = " + to_string(oldNumFace) + " ORDER BY numFace ASC LIMIT 1");
		if(numFace != -1)
		{
			faceLabel.UpdateNumFaceLabel(oldNumFace, numFace);
			faceRecognition.UpdateFaceRecognition(oldNumFace, numFace);
		}
		
	}
	*/

	tbb::parallel_for(0, static_cast<int>(listFace.size()), 1, [=](int i)
	{
		int oldNumFace = listFace[i];
		numFace = -1;
		ExecuteRequest(
			"SELECT numFace FROM FACE_RECOGNITION WHERE NumFaceCompatible = " + to_string(oldNumFace) +
			" ORDER BY numFace ASC LIMIT 1");
		if (numFace != -1)
		{
			CSqlFaceRecognition faceRecognition;
			CSqlFaceLabel faceLabel;
			faceLabel.UpdateNumFaceLabel(oldNumFace, numFace);
			faceRecognition.UpdateFaceRecognition(oldNumFace, numFace);
		}
	}   
    );

	DeleteFaceNameAlone();
}

bool CSqlFacePhoto::DeleteListOfPhoto(const vector<wxString>& listPhoto)
{
	type = 2;
	for (int i = 0; i < listPhoto.size(); i++)
	{
		wxString fullpath = listPhoto[i];
		fullpath.Replace("'", "''");
		//ExecuteRequest("SELECT NumFace FROM FACEPHOTO WHERE fullpath = '" + fullpath + "'");

		CSqlFindFacePhoto findFacePhoto;
		std::vector<CFaceName> listFace = findFacePhoto.GetListFaceNum(listPhoto[i]);

        tbb::parallel_for(0, static_cast<int>(listFace.size()), 1, [=](int k)
		{
			//changed line
			CFaceName facename = listFace[k];
			DeleteNumFace(facename.numFace);
		});


		ExecuteRequestWithNoResult("DELETE FROM FACE_PROCESSING WHERE fullpath = '" + fullpath + "'");
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
	wxString fullpath = path;
	fullpath.Replace("'", "''");
	ExecuteRequest("INSERT INTO FACE_PROCESSING (FullPath) VALUES('" + fullpath + "')");
	return 0;
}

//--------------------------------------------------------
//Chargement de toutes les données d'un album
//--------------------------------------------------------
int CSqlFacePhoto::InsertFace(const wxString& path, const wxString& gender, const wxString& age, const int& numberface, const int& width, const int& height,
                              const double& pertinence, const uint8_t* zBlob, const int& nBlob)
{
	wxString fullpath = path;
	fullpath.Replace("'", "''");
	wxString value = wxString::Format(wxT("%f"), pertinence);
	ExecuteRequest(
		"INSERT INTO FACEPHOTO (FullPath, Numberface, width, height, Pertinence, gender, age) VALUES('" + fullpath + "'," +
		to_string(numberface) + "," + to_string(width) + "," + to_string(height) + "," + value + ",'" + gender + "','" + age + "')");

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
	wxString fullpath = path;
	numFace = 0;
	type = 2;
	fullpath.Replace("'", "''");
	ExecuteRequest(
		"SELECT NumFace FROM FACEPHOTO WHERE FullPath = '" + fullpath + "' and Numberface = " + to_string(numberface));
	return numFace;
}

cv::Mat CSqlFacePhoto::GetFace(const int& numFace, bool& isDefault)
{
	wxLogNull logNo;
	/*
	bitmap.Destroy();
	type = 0;
	ExecuteRequest("SELECT FullPath, width, height, Face FROM FACEPHOTO WHERE NumFace = " + to_string(numFace));
	return bitmap;
	*/
	wxString thumbnail = CFileUtility::GetFaceThumbnailPath(numFace);
	cv::Mat image;
	if (wxFileExists(thumbnail))
	{
		//image = cv::Mat();
        cv::flip(CThumbnailBuffer::GetPicture(thumbnail),image,-1);
	}

	if (image.empty())
	{
		DeleteNumFace(numFace);
		isDefault = true;
	}
	else
		isDefault = false;

   // cv::flip(image,image,-1);
	return image;//image.Mirror(false);
}


bool CSqlFacePhoto::DeletePhotoFaceDatabase(const wxString& path)
{
	wxString fullpath = path;
	fullpath.Replace("'", "''");

	type = 7;
	listFace.clear();
	ExecuteRequest("SELECT NumFace FROM FACEPHOTO WHERE FullPath = '" + fullpath + "'");
	for (int i : listFace)
	{
		DeleteNumFace(i);
	}

	return true;
}

bool CSqlFacePhoto::DeleteFaceDatabase()
{
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
#ifdef WIN32
	documentPath.append("\\Face");
#else
	documentPath.append("/Face");
#endif

	wxArrayString files;
	wxDir::GetAllFiles(documentPath, &files, wxEmptyString, wxDIR_FILES);



	tbb::parallel_for(0, static_cast<int>(listFace.size()), 1, [=](int i)
	{
		wxString filename = files[i];
		if (wxFileExists(filename))
		{
			wxRemoveFile(filename);
		}
	});
	/*
	for (wxString filename : files)
	{
		wxRemoveFile(filename);
	}
	*/
	//wxRmdir(documentPath);

	return (ExecuteRequestWithNoResult("DELETE FROM FACEPHOTO") != -1) ? true : false;
}

int CSqlFacePhoto::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		CFaceRecognitionData face;

		for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
		{
			if (type == 1)
			{
				switch (i)
				{
				case 0:
					filename = sqlResult->ColumnDataText(i);
					break;
				default: ;
				}
				listPhoto.push_back(filename);
			}
			else if (type == 2)
			{
				switch (i)
				{
				case 0:
					numFace = sqlResult->ColumnDataInt(i);
					break;
				default: ;
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
				default: ;
				}
				listFaceIndex.push_back(numFace);
			}
			else if (type == 5)
			{
				switch (i)
				{
				case 0:
					face.numFace = sqlResult->ColumnDataInt(i);
					break;
				case 1:
					face.numFaceCompatible = sqlResult->ColumnDataInt(i);
					break;
				default: ;
				}
			}
			else if (type == 7)
			{
				for (auto clm_num = 0; clm_num < sqlResult->GetColumnCount(); clm_num++)
				{
					switch (clm_num)
					{
					case 0:
						listFace.push_back(sqlResult->ColumnDataInt(clm_num));
						break;
					default: ;
					}
				}
			}
		}
		if (type == 5)
			listFaceRecognition.push_back(face);
		nbResult++;
	}
	return nbResult;
}
