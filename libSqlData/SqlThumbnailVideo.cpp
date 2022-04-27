#include <header.h>
#include "SqlThumbnailVideo.h"
#include "SqlResult.h"
#include <ImageVideoThumbnail.h>
#include <libPicture.h>
#include <wx/file.h>
#include <wx/dir.h>
#include <FileUtility.h>
#include <LibResource.h>
#include <RegardsBitmap.h>
using namespace Regards::Sqlite;
using namespace Regards::Picture;

CSqlThumbnailVideo::CSqlThumbnailVideo()
	: CSqlExecuteRequest(L"RegardsDB"), nbElement(0), numPhoto(0)
{
	type = 0;
	find = false;
}


CSqlThumbnailVideo::~CSqlThumbnailVideo()
{
}

int CSqlThumbnailVideo::GetNbThumbnail(const wxString & path)
{
    type = 3;
    nbElement = 0;
	wxString fullpath = path;
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT count(*) as nbResult FROM VIDEOTHUMBNAIL WHERE FullPath = '" + fullpath + "'");
	return nbElement;
}

bool CSqlThumbnailVideo::TestThumbnail(const int & numPhoto, const int &numVideo)
{
	type = 2;
	find = false;
	ExecuteRequest("SELECT NumPhoto FROM VIDEOTHUMBNAIL WHERE numPhoto = " + to_string(numPhoto) + " and numVideo = " + to_string(numVideo));
	return find;
}



bool CSqlThumbnailVideo::InsertThumbnail(const wxString & path, const uint8_t * zBlob, const int &nBlob, const int & width, const int &height, const int &numPicture, const int &rotation, const int &percent, const int &timePosition)
{
	type = 6;
	wxString fullpath(path);
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT NumPhoto FROM PHOTOS WHERE FullPath = '" + fullpath + "'");

	wxString thumbnail = CFileUtility::GetVideoThumbnailPath(to_string(numPhoto), numPicture);

	if (!wxFileExists(thumbnail))
    {
		type = 2;
		wxFile fileOut;
		fileOut.Create(thumbnail, true);
		fileOut.Write(zBlob, nBlob);
		fileOut.Close();

        return ExecuteRequestWithNoResult("INSERT INTO VIDEOTHUMBNAIL (NumPhoto, FullPath, numVideo, rotation, percent, timePosition, width, height) VALUES(" + to_string(numPhoto) + ",'" + fullpath + "'," + to_string(numPicture) + "," + to_string(rotation) + "," + to_string(percent) + "," + to_string(timePosition) + "," + to_string(width) + "," + to_string(height) + ")");
    }
    return false;
}

void CSqlThumbnailVideo::GetPictureThumbnail(const wxString & path, const int &numVideo, CImageVideoThumbnail * & videoThumbnail)
{
	if (videoThumbnail != nullptr)
	{

		this->videoThumbnail = videoThumbnail;

		type = 6;
		wxString fullpath(path);
		fullpath.Replace("'", "''");
		ExecuteRequest("SELECT NumPhoto FROM PHOTOS WHERE FullPath = '" + fullpath + "'");

		type = 0;
		ExecuteRequest("SELECT rotation, percent, timePosition FROM VIDEOTHUMBNAIL WHERE NumPhoto = " + to_string(numPhoto) + " and numVideo = " + to_string(numVideo));

		wxString thumbnail = CFileUtility::GetVideoThumbnailPath(to_string(numPhoto), numVideo);
		if (wxFileExists(thumbnail))
		{
			CLibPicture libPicture;
			cv::Mat loadPicture;
			CRegardsBitmap * bitmap = nullptr;
			if (wxFileExists(thumbnail))
			{
				loadPicture = cv::imread(thumbnail.ToStdString());
                if(!loadPicture.empty())
                {
                    bitmap = new CRegardsBitmap();
                    bitmap->SetMatrix(loadPicture);
                    bitmap->VertFlipBuf();
                }
				//image.LoadFile(thumbnail, wxBITMAP_TYPE_JPEG);
			}

			if (bitmap != nullptr)
			{
				videoThumbnail->image = new CImageLoadingFormat();
				videoThumbnail->image->SetPicture(bitmap);
			}
			else
			{
				
				videoThumbnail->image = libPicture.LoadPicture(CLibResource::GetPhotoCancel());
				DeleteThumbnail(fullpath);
			}
			videoThumbnail->image->SetFilename(thumbnail);
		}
		else
		{
			videoThumbnail->image = nullptr;
		}
	}
	
	//return videoThumbnail;

}

wxImage CSqlThumbnailVideo::GetThumbnail(const wxString & path, const int &numVideo)
{
	wxLogNull logNo;
	type = 6;
	wxString fullpath(path);
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT NumPhoto FROM PHOTOS WHERE FullPath = '" + fullpath + "'");

	wxString thumbnail = CFileUtility::GetVideoThumbnailPath(to_string(numPhoto), numVideo);
	wxImage image;
	if (wxFileExists(thumbnail))
		image.LoadFile(thumbnail, wxBITMAP_TYPE_JPEG);
    else
    {
        printf("error GetThumbnail");
    }
	return image;
}


bool CSqlThumbnailVideo::DeleteThumbnail(const wxString & path)
{
	type = 6;
	wxString fullpath = path;
	fullpath.Replace("'", "''");

	ExecuteRequest("SELECT NumPhoto FROM PHOTOS WHERE FullPath = '" + fullpath + "'");
	for (int i = 0; i < 20; i++)
	{
		wxString thumbnail = CFileUtility::GetVideoThumbnailPath(to_string(numPhoto), i);
		if (wxFileExists(thumbnail))
		{
			wxRemoveFile(thumbnail);
		}
	}

	return (ExecuteRequestWithNoResult("DELETE FROM VIDEOTHUMBNAIL WHERE FullPath = '" + fullpath + "'") != -1) ? true : false;
}

bool CSqlThumbnailVideo::DeleteThumbnail(const int & numPhoto)
{
	for (int i = 0; i < 20; i++)
	{
		wxString thumbnail = CFileUtility::GetVideoThumbnailPath(to_string(numPhoto), i);
		if (wxFileExists(thumbnail))
		{
			wxRemoveFile(thumbnail);
		}
	}

	return (ExecuteRequestWithNoResult("DELETE FROM VIDEOTHUMBNAIL WHERE FullPath in (SELECT FullPath FROM PHOTOS WHERE NumPhoto = " + to_string(numPhoto) + ")") != -1) ? true : false;
}

bool CSqlThumbnailVideo::EraseThumbnail()
{
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
#ifdef WIN32
	documentPath.append("\\ThumbnailVideo");
#else
	documentPath.append("/ThumbnailVideo");
#endif

	wxArrayString files;
	wxDir::GetAllFiles(documentPath, &files, wxEmptyString, wxDIR_FILES);
	for (wxString filename : files)
	{
		wxRemoveFile(filename);
	}
	//wxRmdir(documentPath);
	return (ExecuteRequestWithNoResult("DELETE FROM VIDEOTHUMBNAIL") != -1) ? true : false;
}

bool CSqlThumbnailVideo::EraseFolderThumbnail(const int &numFolder)
{
	type = 7;
	listPhoto.clear();
	ExecuteRequest("SELECT NumPhoto FROM PHOTOS WHERE NumFolderCatalog = " + to_string(numFolder));
	for (int idPhoto : listPhoto)
	{
		for (int i = 0; i < 20; i++)
		{
			wxString thumbnail = CFileUtility::GetVideoThumbnailPath(to_string(idPhoto), i);
			if (wxFileExists(thumbnail))
			{
				wxRemoveFile(thumbnail);
			}
		}
	}
	return (ExecuteRequestWithNoResult("DELETE FROM VIDEOTHUMBNAIL WHERE FullPath in (SELECT FullPath FROM PHOTOS WHERE NumFolderCatalog = " + to_string(numFolder) + ")") != -1) ? true : false;
}

int CSqlThumbnailVideo::TraitementResult(CSqlResult * sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		wxString filename = "";
        
		switch (type)
		{
 		case 0:
            
			for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
			{
				switch (i)
				{
                case 0:
                    videoThumbnail->rotation = sqlResult->ColumnDataInt(i);
                    break;
                case 1:
                    videoThumbnail->percent = sqlResult->ColumnDataInt(i);
                    break;
                case 2:
                    videoThumbnail->timePosition = sqlResult->ColumnDataInt(i);
                    break;
                default: ;
				}
			}
			break;

		case 2:
			for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
			{
				switch (i)
				{
				case 0:
					find = true;
					break;
				default: ;
				}
			}
			break;
		case 3:
			for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
			{
				switch (i)
				{
				case 0:
					nbElement = sqlResult->ColumnDataInt(i);
					break;
				default: ;
				}
			}
			break;  

		case 6:
			for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
			{
				switch (i)
				{
				case 0:
					numPhoto = sqlResult->ColumnDataInt(i);
					break;
				default: ;
				}
			}
			break;

			case 7:
				for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
				{
					switch (i)
					{
					case 0:
						numPhoto = sqlResult->ColumnDataInt(i);
						listPhoto.push_back(numPhoto);
						break;
					default: ;
					}
				}
			break;
		default: ;
		}

		nbResult++;
	}
	return nbResult;
};