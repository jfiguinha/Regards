#include <header.h>
#include "SqlThumbnailVideo.h"
#include "SqlResult.h"
#include <ImageVideoThumbnail.h>
#include <libPicture.h>
#include <wx/dir.h>
#include "ThumbnailBuffer.h"
#include <ConvertUtility.h>
#include <FileUtility.h>
#include <appcontext.h>
#include <SqlParameter.h>
#include "SqlPhotos.h"
#include <wx/filename.h>
extern AppContext application_context;

using namespace Regards::Sqlite;
using namespace Regards::Picture;


CSqlThumbnailVideo::CSqlThumbnailVideo()
	: CSqlExecuteRequest(L"RegardsDB"), nbElement(0), type(0)
{

}

int CSqlThumbnailVideo::GetNbThumbnail(const wxString& path)
{
	type = 0;
	nbElement = 0;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(path));
	ExecuteSqlWithStatement("SELECT count(*) as nbResult FROM VIDEOTHUMBNAIL WHERE FullPath = ?", parameter);
	return nbElement;
}

bool CSqlThumbnailVideo::TestThumbnail(const int& numPhoto, const int& numVideo)
{
	type = 0;
	nbElement = -1;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numPhoto));
	parameter.push_back(std::make_unique<CSqlInt>(numVideo));
	ExecuteSqlWithStatement("SELECT NumPhoto FROM VIDEOTHUMBNAIL WHERE numPhoto = ? and numVideo = ?", parameter);
	return nbElement != -1 ? true : false;
}


wxString CSqlThumbnailVideo::InsertThumbnail(int photoId, const wxString& path, const int& width,
                                         const int& height, const int& numPicture, const int& rotation,
                                         const int& percent, const int& timePosition)
{
	wxString thumbnail = "";
	//CSqlPhotos SqlPhotos;
	//int photoId = SqlPhotos.GetPhotoId(path);

	if (photoId != -1)
	{
		thumbnail = CFileUtility::GetVideoThumbnailPath(to_string(photoId), numPicture);

		if (!wxFileExists(thumbnail))
		{
			std::vector<std::unique_ptr<CSqlParameter>> parameter;
			parameter.push_back(std::make_unique<CSqlInt>(photoId));
			parameter.push_back(std::make_unique<CSqlString>(path));
			parameter.push_back(std::make_unique<CSqlInt>(numPicture));
			parameter.push_back(std::make_unique<CSqlInt>(rotation));
			parameter.push_back(std::make_unique<CSqlInt>(percent));
			parameter.push_back(std::make_unique<CSqlInt>(timePosition));
			parameter.push_back(std::make_unique<CSqlInt>(width));
			parameter.push_back(std::make_unique<CSqlInt>(height));
			ExecuteSqlWithStatementNoResult("INSERT INTO VIDEOTHUMBNAIL (NumPhoto, FullPath, numVideo, rotation, percent, timePosition, width, height) VALUES (?, ?, ?, ?, ?, ?, ?, ?)", parameter);
		}
		
	}
	return thumbnail;
}

void CSqlThumbnailVideo::GetPictureThumbnail(int photoId, const wxString& path, const int& numVideo,
                                             CImageVideoThumbnail* videoThumbnail)
{
	if (videoThumbnail != nullptr)
	{
		this->videoThumbnail = videoThumbnail;

		if (photoId != -1)
		{
			type = 1;
			this->videoThumbnail->timePosition = -1;
			std::vector<std::unique_ptr<CSqlParameter>> parameter;
			parameter.push_back(std::make_unique<CSqlInt>(photoId));
			parameter.push_back(std::make_unique<CSqlInt>(numVideo));
			ExecuteSqlWithStatement("SELECT rotation, percent, timePosition FROM VIDEOTHUMBNAIL WHERE NumPhoto = ? and numVideo = ?", parameter);

			wxString thumbnail = CFileUtility::GetVideoThumbnailPath(to_string(photoId), numVideo);
			if (wxFileExists(thumbnail))
			{
				videoThumbnail->image = CThumbnailBuffer::GetPicture(thumbnail);
				if (videoThumbnail->image.empty())
					videoThumbnail->image = application_context.GetDefaultVideoThumbnail();
				
				videoThumbnail->filename = thumbnail;
			}
		}

	}
}

cv::Mat CSqlThumbnailVideo::GetThumbnail(int photoId, const wxString& path, const int& numVideo, bool & isDefault)
{
	cv::Mat image;
	isDefault = true;
	if (photoId != -1)
	{
		wxString thumbnail = CFileUtility::GetVideoThumbnailPath(to_string(photoId), numVideo);
		
		if (wxFileExists(thumbnail))
		{
			image = CThumbnailBuffer::GetPicture(thumbnail);
			isDefault = false;
		}
		else
		{
			thumbnail = CFileUtility::GetThumbnailPath(to_string(photoId));
			if (wxFileExists(thumbnail))
			{
				image = CThumbnailBuffer::GetPicture(thumbnail);
				isDefault = false;
			}
		}
	}
	return image;
}


bool CSqlThumbnailVideo::DeleteThumbnail(const int& numPhoto)
{
	type = 3;
	listVideo.clear();
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numPhoto));
	ExecuteSqlWithStatement("SELECT NumPhoto, NumVideo FROM VIDEOTHUMBNAIL WHERE NumPhoto = ?", parameter);

	for (auto& video : listVideo)
	{
		wxString thumbnail = CFileUtility::GetVideoThumbnailPath(to_string(video.first), video.second);
		if (wxFileExists(thumbnail))
			wxRemoveFile(thumbnail);
	}

	return ExecuteSqlWithStatementNoResult("DELETE FROM VIDEOTHUMBNAIL WHERE NumPhoto = ?", parameter);
}

bool CSqlThumbnailVideo::EraseThumbnail()
{
	wxFileName documentPath = wxFileName(CFileUtility::GetDocumentFolderPath());
	documentPath.AppendDir("ThumbnailVideo");

	wxArrayString files;
	wxDir::GetAllFiles(documentPath.GetFullPath(), &files, wxEmptyString, wxDIR_FILES);
	for (wxString filename : files)
	{
		CThumbnailBuffer::RemovePicture(filename);
			wxRemoveFile(filename);
	}
	//wxRmdir(documentPath);
	return (ExecuteRequestWithNoResult("DELETE FROM VIDEOTHUMBNAIL") != -1) ? true : false;
}

bool CSqlThumbnailVideo::EraseFolderThumbnail(const int& numFolder)
{
	type = 3;
	listVideo.clear();
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFolder));
	ExecuteSqlWithStatement("SELECT NumPhoto, NumVideo FROM VIDEOTHUMBNAIL WHERE NumPhoto in (SELECT NumPhoto FROM PHOTOS WHERE NumFolderCatalog = ?)", parameter);
	for (auto& video : listVideo)
	{
		wxString thumbnail = CFileUtility::GetVideoThumbnailPath(to_string(video.first), video.second);
		if (wxFileExists(thumbnail))
			wxRemoveFile(thumbnail);
	}

	return ExecuteSqlWithStatementNoResult("DELETE FROM VIDEOTHUMBNAIL WHERE NumPhoto in (SELECT NumPhoto FROM PHOTOS WHERE NumFolderCatalog = ?)", parameter);
}

int CSqlThumbnailVideo::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		switch (type)
		{
		case 0:
			nbElement = sqlResult->ColumnDataInt(0);
			break;
		case 1:
			if (videoThumbnail != nullptr)
			{
				videoThumbnail->rotation = sqlResult->ColumnDataInt(0);
				videoThumbnail->percent = sqlResult->ColumnDataInt(1);
				videoThumbnail->timePosition = sqlResult->ColumnDataInt(2);
			}
			break;

		case 2:
			listPhoto.push_back(sqlResult->ColumnDataInt(0));
			break;

		case 3:
			listVideo.push_back(std::make_pair(sqlResult->ColumnDataInt(0), sqlResult->ColumnDataInt(1)));
			break;

		default: ;
		}

		nbResult++;
	}
	return nbResult;
};
