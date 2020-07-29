#include <header.h>
#include "SqlFacePhoto.h"
#include "SqlFaceLabel.h"
#include "SqlFaceRecognition.h"
#include <PictureData.h>
#include <wx/mstream.h>
using namespace Regards::Sqlite;

CSqlFacePhoto::CSqlFacePhoto()
	: CSqlExecuteRequest(L"RegardsDB")
{

}


CSqlFacePhoto::~CSqlFacePhoto()
{
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

vector<CPictureData *> CSqlFacePhoto::GetAllFace()
{
	type = 3;
	listFace.clear();
	ExecuteRequest("SELECT FullPath, width, height, Face FROM FACEPHOTO");
	return listFace;
}

vector<CPictureData *> CSqlFacePhoto::GetAllFace(const int &numFace)
{
	type = 3;
	listFace.clear();
	ExecuteRequest("SELECT FullPath, width, height, Face FROM FACEPHOTO where NumFace != " + to_string(numFace));
	return listFace;
}

CPictureData * CSqlFacePhoto::GetFacePicture(const int &numFace)
{
	type = 3;
	listFace.clear();
	ExecuteRequest("SELECT FullPath, width, height, Face FROM FACEPHOTO where NumFace = " + to_string(numFace));
	if (listFace.size() > 0)
		return listFace[0];
	return nullptr;
}

bool CSqlFacePhoto::DeleteListOfPhoto(const vector<int> & listNumPhoto)
{
	type = 2;
	for(int i = 0;i < listNumPhoto.size();i++)
	{
		int numPhoto = listNumPhoto[i];
		ExecuteRequest("SELECT NumFace FROM FACEPHOTO WHERE fullpath in (select fullpath from Photos where NumPhoto = " + to_string(numPhoto) + ")");
		ExecuteRequestWithNoResult("DELETE FROM FACEPHOTO WHERE NumFace = " + to_string(numFace));
		//ExecuteRequestWithNoResult("DELETE FROM FACE_NAME WHERE NumFace = " + to_string(numFace));
		ExecuteRequestWithNoResult("DELETE FROM FACE_RECOGNITION WHERE NumFace = " + to_string(numFace));
		ExecuteRequestWithNoResult("DELETE FROM FACEDESCRIPTOR WHERE NumFace = " + to_string(numFace));
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
		ExecuteRequest("SELECT numFace FROM FACE_RECOGNITION WHERE NumFaceCompatible = " + to_string(oldNumFace) + " ASC LIMIT 1");
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
		ExecuteRequest("SELECT NumFace FROM FACEPHOTO WHERE fullpath = '" + fullpath + "'");
		ExecuteRequestWithNoResult("DELETE FROM FACEPHOTO WHERE NumFace = " + to_string(numFace));
		//ExecuteRequestWithNoResult("DELETE FROM FACE_NAME WHERE NumFace = " + to_string(numFace));
		ExecuteRequestWithNoResult("DELETE FROM FACE_RECOGNITION WHERE NumFace = " + to_string(numFace));
		ExecuteRequestWithNoResult("DELETE FROM FACEDESCRIPTOR WHERE NumFace = " + to_string(numFace));
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
	ExecuteInsertBlobData("INSERT INTO FACEPHOTO (FullPath, Numberface, width, height, Pertinence, Face) VALUES('" + fullpath + "'," + to_string(numberface) + "," + to_string(width) + "," + to_string(height) + "," + value + ", ? )", 7, zBlob, nBlob);
	return GetNumFace(path, numberface);
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
	bitmap.Destroy();
	type = 0;
	ExecuteRequest("SELECT FullPath, width, height, Face FROM FACEPHOTO WHERE NumFace = " + to_string(numFace));
	return bitmap;
}



bool CSqlFacePhoto::DeletePhotoFaceDatabase(const wxString & path)
{
	wxString fullpath = path;
	fullpath.Replace("'", "''");
	return (ExecuteRequestWithNoResult("DELETE FROM FACEPHOTO WHERE FullPath = '" + fullpath + "'") != -1) ? true : false;
}

bool CSqlFacePhoto::DeleteFaceDatabase()
{
	return (ExecuteRequestWithNoResult("DELETE FROM FACEPHOTO") != -1) ? true : false;
}

int CSqlFacePhoto::TraitementResult(CSqlResult * sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		CPictureData * picture = nullptr;
		if (type == 3)
			picture = new CPictureData();

		for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
		{
			if(type == 0)
			{
				switch (i)
				{
					case 0:
						filename = sqlResult->ColumnDataText(i);
						break;
					case 1:
						width = sqlResult->ColumnDataInt(i);
						break;
					case 2:
						height = sqlResult->ColumnDataInt(i);
						break;
					case 3:
					{
						int size = sqlResult->ColumnDataBlobSize(i);
						if (size > 0)
						{		
							//const int req_comps = 4;
							//int actual_comps = 4;
							uint8_t * data = new uint8_t[size];
							sqlResult->ColumnDataBlob(i, (void * &)data, size);
							wxMemoryInputStream jpegStream(data, size);
							bitmap.LoadFile(jpegStream, wxBITMAP_TYPE_JPEG);
							delete[] data;
						}
					}
				}
			}
			else if(type == 1)
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
			else if (type == 3)
			{
				
				switch (i)
				{
				case 0:
					picture->SetFilename(sqlResult->ColumnDataText(i));
					break;
				case 1:
					picture->SetWidth(sqlResult->ColumnDataInt(i));
					break;
				case 2:
					picture->SetHeight(sqlResult->ColumnDataInt(i));
					break;
				case 3:
				{
					int size = sqlResult->ColumnDataBlobSize(i);
					if (size > 0)
					{
						uint8_t * data = new uint8_t[size];
						sqlResult->ColumnDataBlob(i, (void * &)data, size);
						if (data != nullptr)
						{
							picture->SetData(data, size);

							delete[] data;
							data = nullptr;
						}
					}
				}
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
		}

		if (type == 3)
			listFace.push_back(picture);
		nbResult++;
	}
	return nbResult;
}