#include <header.h>
#include "SqlThumbnailVideo.h"
#include "SqlLib.h"
#include "SqlEngine.h"
#include "SqlResult.h"
#include <RegardsBitmap.h>
#include <ImageVideoThumbnail.h>
#include <wx/mstream.h>
using namespace Regards::Sqlite;

CSqlThumbnailVideo::CSqlThumbnailVideo()
	: CSqlExecuteRequest(L"RegardsDB")
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

bool CSqlThumbnailVideo::TestThumbnail(const wxString & path, const int &numVideo)
{
	type = 2;
	find = false;
	wxString fullpath = path;
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT FullPath FROM VIDEOTHUMBNAIL WHERE FullPath = '" + fullpath + "' and numVideo = " + to_string(numVideo));
	return find;
}



bool CSqlThumbnailVideo::InsertThumbnail(const wxString & path, const uint8_t * zBlob, const int &nBlob, const int & width, const int &height, const int &numPicture, const int &rotation, const int &percent, const int &timePosition)
{
    if(!TestThumbnail(path, numPicture))
    {
        wxString fullpath = path;
        fullpath.Replace("'", "''");
        return ExecuteInsertBlobData("INSERT INTO VIDEOTHUMBNAIL (FullPath, numVideo, rotation, percent, timePosition, width, height, thumbnail) VALUES('" + fullpath + "'," + to_string(numPicture) + "," + to_string(rotation) + "," + to_string(percent) + "," + to_string(timePosition) + "," + to_string(width) + "," + to_string(height) + ", ? )", 7, zBlob, nBlob);
    }
    return false;
}

CImageVideoThumbnail * CSqlThumbnailVideo::GetPictureThumbnail(const wxString & path, const int &numVideo)
{
	type = 0;
    videoThumbnail = nullptr;
	wxString fullpath = path;
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT FullPath, numVideo, rotation, percent, timePosition, width, height, thumbnail FROM VIDEOTHUMBNAIL WHERE FullPath = '" + fullpath + "' and numVideo = " + to_string(numVideo));
	return videoThumbnail;
}

wxImage CSqlThumbnailVideo::GetThumbnail(const wxString & path, const int &numVideo)
{
	type = 1;
	wxString fullpath = path;
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT FullPath, numVideo, width, height, thumbnail FROM VIDEOTHUMBNAIL WHERE FullPath = '" + fullpath + "' and numVideo = " + to_string(numVideo));
	return bitmap;
}


bool CSqlThumbnailVideo::DeleteThumbnail(const wxString & path)
{
	wxString fullpath = path;
	fullpath.Replace("'", "''");
	return (ExecuteRequestWithNoResult("DELETE FROM VIDEOTHUMBNAIL WHERE FullPath = '" + fullpath + "'") != -1) ? true : false;
}

bool CSqlThumbnailVideo::DeleteThumbnail(const int & numPhoto)
{
	return (ExecuteRequestWithNoResult("DELETE FROM VIDEOTHUMBNAIL WHERE FullPath in (SELECT FullPath FROM PHOTOS WHERE NumPhoto = " + to_string(numPhoto) + ")") != -1) ? true : false;
}

bool CSqlThumbnailVideo::EraseThumbnail()
{
	return (ExecuteRequestWithNoResult("DELETE FROM VIDEOTHUMBNAIL") != -1) ? true : false;
}

bool CSqlThumbnailVideo::EraseFolderThumbnail(const int &numFolder)
{
	//return (ExecuteRequestWithNoResult("DELETE FROM VIDEOTHUMBNAIL") != -1) ? true : false;
	return (ExecuteRequestWithNoResult("DELETE FROM VIDEOTHUMBNAIL WHERE FullPath in (SELECT FullPath FROM PHOTOS WHERE NumFolderCatalog = " + to_string(numFolder) + ")") != -1) ? true : false;
}

int CSqlThumbnailVideo::TraitementResult(CSqlResult * sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		int width;
		int height;
        int numVideo;
        //int percent;
      // int timePosition;
		wxString filename = "";
        
		switch (type)
		{
        case 1:
            
			for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
			{
                // FullPath, numVideo, rotation, percent, timePosition, width, height
				switch (i)
				{
				case 0:
					filename = sqlResult->ColumnDataText(i);
					break;
                case 1:
                    numVideo = sqlResult->ColumnDataInt(i);
                    break;
				case 2:
					width = sqlResult->ColumnDataInt(i);
					break;
				case 3:
					height = sqlResult->ColumnDataInt(i);
					break;
				case 4:
				{
					int size = sqlResult->ColumnDataBlobSize(i);
					if (size > 0)
					{		
						const int req_comps = 4;
						int actual_comps = 4;
						uint8_t * data = new uint8_t[size];
						sqlResult->ColumnDataBlob(i, (void * &)data, size);
                        
                        wxMemoryInputStream jpegStream(data, size);
                        bitmap.LoadFile(jpegStream, wxBITMAP_TYPE_JPEG);                        

						delete[] data;
					}
				}
				break;
				}
			}
			break;
		case 0:
            videoThumbnail = new CImageVideoThumbnail();
			for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
			{
                // FullPath, numVideo, rotation, percent, timePosition, width, height
				switch (i)
				{
				case 0:
					filename = sqlResult->ColumnDataText(i);
					break;
                case 1:
                    numVideo = sqlResult->ColumnDataInt(i);
                    break;
                case 2:
                    videoThumbnail->rotation = sqlResult->ColumnDataInt(i);
                    break;
                case 3:
                    videoThumbnail->percent = sqlResult->ColumnDataInt(i);
                    break;
                case 4:
                    videoThumbnail->timePosition = sqlResult->ColumnDataInt(i);
                    break;
				case 5:
					width = sqlResult->ColumnDataInt(i);
					break;
				case 6:
					height = sqlResult->ColumnDataInt(i);
					break;
				case 7:
				{
					int size = sqlResult->ColumnDataBlobSize(i);
					if (size > 0)
					{		
						const int req_comps = 4;
						int actual_comps = 4;
						uint8_t * data = new uint8_t[size];
						sqlResult->ColumnDataBlob(i, (void * &)data, size);
                        wxImage * bitmap = new wxImage();
                        wxMemoryInputStream jpegStream(data, size);
                        bitmap->LoadFile(jpegStream, wxBITMAP_TYPE_JPEG);                                               
                        videoThumbnail->image = new CImageLoadingFormat();
                        videoThumbnail->image->SetPicture(bitmap);

						delete[] data;
					}
				}
				break;
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
				}
			}
			break;            
		}

		nbResult++;
	}
	return nbResult;
};