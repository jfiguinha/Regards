#include <header.h>
#include "SqlThumbnail.h"
#include "SqlResult.h"
#include <FileUtility.h>
#include <wx/dir.h>
#include "ThumbnailBuffer.h"
#include "SqlPhotos.h"
#include <ImageLoadingFormat.h>
#include <SqlParameter.h>
#include <wx/filename.h>
using namespace Regards::Sqlite;

CSqlThumbnail::CSqlThumbnail()
	: CSqlExecuteRequest(L"RegardsDB")
{
	//regardsBitmap = nullptr;
	type = 0;
}

int CSqlThumbnail::GetThumbnailId(const wxString& path)
{
	type = 0;
	numPhoto = -1;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(path));
	ExecuteSqlWithStatement("SELECT NumPhoto FROM PHOTOSTHUMBNAIL WHERE FullPath = ?", parameter);
	return numPhoto;
}

bool CSqlThumbnail::TestThumbnail(const wxString& path, const wxString& hash)
{
	type = 0;
	numPhoto = -1;
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(path));
	parameter.push_back(std::make_unique<CSqlString>(hash));
	ExecuteSqlWithStatement("SELECT NumPhoto FROM PHOTOSTHUMBNAIL WHERE FullPath = ? and hash = ?", parameter);

	if (numPhoto == -1)
	{
		DeleteThumbnail(path);
	}
	return numPhoto != -1 ? true : false;
}

bool CSqlThumbnail::TestThumbnail(const wxString& path)
{
	type = 0;
	int numPhotoId = GetThumbnailId(path);
	return numPhotoId != -1 ? true : false;
}


wxString CSqlThumbnail::InsertThumbnail(const wxString& path, const int& width, const int& height,
                                    const wxString& hash)
{
	bool returnValue = true;
	CSqlPhotos SqlPhotos;
	int photoId = SqlPhotos.GetPhotoId(path);

	if (photoId != -1)
	{
		wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(photoId));
		if (wxFileExists(thumbnail))
			wxRemoveFile(thumbnail);

		if (!wxFileExists(thumbnail))
		{
			std::vector<std::unique_ptr<CSqlParameter>> parameter;
			parameter.push_back(std::make_unique<CSqlInt>(photoId));
			parameter.push_back(std::make_unique<CSqlString>(path));
			parameter.push_back(std::make_unique<CSqlInt>(width));
			parameter.push_back(std::make_unique<CSqlInt>(height));
			parameter.push_back(std::make_unique<CSqlString>(hash));
			ExecuteSqlWithStatementNoResult("INSERT or IGNORE INTO PHOTOSTHUMBNAIL(NumPhoto, FullPath, width, height, hash) VALUES (?, ?, ?, ?, ?)", parameter);
		}
		return thumbnail;
	}

	return "";
}

vector<int> CSqlThumbnail::GetAllPhotoThumbnail()
{
	type = 1;
	listPhoto.clear();
	ExecuteRequest("SELECT NumPhoto FROM PHOTOSTHUMBNAIL");
	return listPhoto;
}

cv::Mat CSqlThumbnail::GetThumbnail(const wxString& path, bool& isDefault)
{
	cv::Mat image;
	isDefault = true;
	int photoId = GetThumbnailId(path);
	if (photoId != -1)
	{
		wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(numPhoto));

		if (wxFileExists(thumbnail))
			image = CThumbnailBuffer::GetPicture(thumbnail);
		
		if (!image.empty())
			isDefault = false;
	}

	return image;
}

CImageLoadingFormat* CSqlThumbnail::GetPictureThumbnail(const wxString& path)
{
	CImageLoadingFormat* picture = nullptr;
	int photoId = GetThumbnailId(path);
	if (photoId != -1)
	{
		wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(photoId));
		if (wxFileExists(thumbnail))
		{
			picture = new CImageLoadingFormat();
			cv::Mat image = CThumbnailBuffer::GetPicture(thumbnail);

			picture->SetPicture(image);
			if (picture != nullptr)
				picture->SetFilename(thumbnail);
			else
				DeleteThumbnail(photoId);
		}
	}
	return picture;
}

bool CSqlThumbnail::DeleteThumbnail(const wxString& path)
{
	int photoId = GetThumbnailId(path);
	wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(photoId));
	if (wxFileExists(thumbnail))
	{
		wxRemoveFile(thumbnail);
		CThumbnailBuffer::RemovePicture(thumbnail);
	}

	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlString>(path));
	return ExecuteSqlWithStatementNoResult("DELETE FROM PHOTOSTHUMBNAIL WHERE FullPath = ?", parameter);
}

bool CSqlThumbnail::DeleteThumbnail(const int& numPhoto)
{
	wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(numPhoto));
	if (wxFileExists(thumbnail))
	{
		wxRemoveFile(thumbnail);
		CThumbnailBuffer::RemovePicture(thumbnail);
	}


	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numPhoto));
	return ExecuteSqlWithStatementNoResult("DELETE FROM PHOTOSTHUMBNAIL WHERE FullPath in (SELECT FullPath FROM PHOTOS WHERE NumPhoto = ?)", parameter);
}

void CSqlThumbnail::EraseThumbnail(const int& numPhoto)
{
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numPhoto));
	ExecuteSqlWithStatementNoResult("INSERT INTO PHOTOSWIHOUTTHUMBNAIL (FullPath, Priority, ProcessStart) VALUES (SELECT FullPath, 1, 0 FROM PHOTOS WHERE NumPhoto = ?)", parameter);
	ExecuteSqlWithStatementNoResult("DELETE FROM PHOTOSTHUMBNAIL WHERE FullPath in (SELECT FullPath FROM PHOTOS WHERE NumPhoto = ?)", parameter);
}

bool CSqlThumbnail::EraseThumbnail()
{
	wxFileName documentPath = wxFileName(CFileUtility::GetDocumentFolderPath());
	documentPath.AppendDir("ThumbnailVideo");

	wxArrayString files;
	wxDir::GetAllFiles(documentPath.GetFullPath(), &files, wxEmptyString, wxDIR_FILES);

	for (int i = 0; i < files.size(); i++)
	{
		wxString filename = files[i];
		if (wxFileExists(filename))
		{
			wxRemoveFile(filename);
			CThumbnailBuffer::RemovePicture(filename);
		}
	}

	return (ExecuteRequestWithNoResult("DELETE FROM PHOTOSTHUMBNAIL") != -1) ? true : false;
}

bool CSqlThumbnail::EraseFolderThumbnail(const int& numFolder)
{
	type = 1;
	listPhoto.clear();
	std::vector<std::unique_ptr<CSqlParameter>> parameter;
	parameter.push_back(std::make_unique<CSqlInt>(numFolder));
	ExecuteSqlWithStatement("SELECT NumPhoto FROM PHOTOS WHERE NumFolderCatalog = ? ", parameter);

	for (int idPhoto : listPhoto)
	{
		wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(idPhoto));
		if (wxFileExists(thumbnail))
		{
			wxRemoveFile(thumbnail);
			CThumbnailBuffer::RemovePicture(thumbnail);
		}
	}

	return ExecuteSqlWithStatementNoResult("DELETE FROM PHOTOSTHUMBNAIL WHERE FullPath in (SELECT FullPath FROM PHOTOS WHERE NumFolderCatalog = ?)", parameter);
}

int CSqlThumbnail::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		switch (type)
		{
		case 0:
			numPhoto = sqlResult->ColumnDataInt(0);
			break;
		case 1:
			listPhoto.push_back(sqlResult->ColumnDataInt(0));
			break;
		}
		nbResult++;
	}
	return nbResult;
}
