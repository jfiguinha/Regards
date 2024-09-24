#include <header.h>
#include "SqlThumbnail.h"
#include "SqlResult.h"
#include <FileUtility.h>
#include <libPicture.h>
#include <wx/file.h>
#include <wx/dir.h>
#include "ThumbnailBuffer.h"
#include <LoadingResource.h>
#include <ConvertUtility.h>
#include <ImageLoadingFormat.h>
using namespace Regards::Sqlite;
using namespace Regards::Picture;

CSqlThumbnail::CSqlThumbnail()
	: CSqlExecuteRequest(L"RegardsDB")
{
	//regardsBitmap = nullptr;
	type = 0;
	find = false;
}


CSqlThumbnail::~CSqlThumbnail()
{
}

bool CSqlThumbnail::TestThumbnail(const wxString& path, const wxString& hash)
{
	type = 2;
	wxString fullpath(path);
	fullpath.Replace("'", "''");
	ExecuteRequest(
		"SELECT NumPhoto FROM PHOTOSTHUMBNAIL WHERE FullPath = '" + fullpath + "' and hash = '" + hash + "'");
	if (!find)
	{
		DeleteThumbnail(path);
	}
	return find;
}

bool CSqlThumbnail::TestThumbnail(const wxString& path)
{
	type = 2;
	wxString fullpath(path);
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT NumPhoto FROM PHOTOSTHUMBNAIL WHERE FullPath = '" + fullpath + "'");
	if (!find)
	{
		DeleteThumbnail(path);
	}
	return find;
}


wxString CSqlThumbnail::InsertThumbnail(const wxString& path, const int& width, const int& height,
                                    const wxString& hash)
{
	bool returnValue = true;
	type = 6;
	wxString fullpath(path);
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT NumPhoto FROM PHOTOS WHERE FullPath = '" + fullpath + "'");
	if (numPhoto > 0)
	{
		wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(numPhoto));
		if (wxFileExists(thumbnail))
		{
#ifdef WIN32
			std::remove(thumbnail);
#else
			wxRemoveFile(thumbnail);
#endif
		}

		if (!wxFileExists(thumbnail))
		{
			returnValue = ExecuteRequestWithNoResult(
				"INSERT or IGNORE INTO PHOTOSTHUMBNAIL (NumPhoto, FullPath, width, height, hash) VALUES(" + to_string(numPhoto) + ",'"
				+ fullpath + "'," + to_string(width) + "," + to_string(height) + ",'" + hash + "')");
		}
		return thumbnail;
	}

	return "";
}

vector<int> CSqlThumbnail::GetAllPhotoThumbnail()
{
	type = 7;
	listPhoto.clear();
	ExecuteRequest("SELECT NumPhoto FROM PHOTOSTHUMBNAIL");
	return listPhoto;
}

cv::Mat CSqlThumbnail::GetThumbnail(const wxString& path, bool& isDefault)
{
    //printf("CSqlThumbnail::GetThumbnail : Error : %s \n", path.ToStdString().c_str());
	wxLogNull logNo;
	type = 6;
	wxString fullpath(path);
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT NumPhoto FROM PHOTOSTHUMBNAIL WHERE FullPath = '" + fullpath + "'");


	cv::Mat image;
	//if (numPhoto == 0)
	//	return image;

	wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(numPhoto));
	
	if (wxFileExists(thumbnail))
	{
        //image = CLibPicture::ReadPicture(thumbnail);
        image = cv::imread(CConvertUtility::ConvertToStdString(thumbnail), cv::IMREAD_COLOR);
	}

	if (image.empty())
		isDefault = true;
	else
		isDefault = false;

	return image;
}

CImageLoadingFormat* CSqlThumbnail::GetPictureThumbnail(const wxString& path)
{
	type = 6;
	wxString fullpath(path);
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT NumPhoto FROM PHOTOSTHUMBNAIL WHERE FullPath = '" + fullpath + "'");
	CImageLoadingFormat* picture = nullptr;
	if (numPhoto > 0)
	{
		wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(numPhoto));
		if (wxFileExists(thumbnail))
		{
			picture = new CImageLoadingFormat();
			cv::Mat image = CThumbnailBuffer::GetPicture(thumbnail);
			//CLibPicture libPicture;
			//picture = libPicture.LoadPicture(thumbnail);
			picture->SetPicture(image);
			if (picture != nullptr)
				picture->SetFilename(thumbnail);
			else
			{
				printf("error");
				DeleteThumbnail(numPhoto);
			}
		}
	}

	

	return picture;
}

bool CSqlThumbnail::DeleteThumbnail(const wxString& path)
{
	type = 6;
	wxString fullpath(path);
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT NumPhoto FROM PHOTOSTHUMBNAIL WHERE FullPath = '" + fullpath + "'");
	wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(numPhoto));
	if (wxFileExists(thumbnail))
	{
		{
#ifdef WIN32
			std::remove(thumbnail);
#else
			wxRemoveFile(thumbnail);
#endif
		}
		CThumbnailBuffer::RemovePicture(thumbnail);
	}
	return (ExecuteRequestWithNoResult("DELETE FROM PHOTOSTHUMBNAIL WHERE FullPath = '" + fullpath + "'") != -1)
		       ? true
		       : false;
}

bool CSqlThumbnail::DeleteThumbnail(const int& numPhoto)
{
	wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(numPhoto));
	if (wxFileExists(thumbnail))
	{
		{
#ifdef WIN32
			std::remove(thumbnail);
#else
			wxRemoveFile(thumbnail);
#endif
		}
		CThumbnailBuffer::RemovePicture(thumbnail);
	}

	return (ExecuteRequestWithNoResult(
		       "DELETE FROM PHOTOSTHUMBNAIL WHERE FullPath in (SELECT FullPath FROM PHOTOS WHERE NumPhoto = " +
		       to_string(numPhoto) + ")") != -1)
		       ? true
		       : false;
}

void CSqlThumbnail::EraseThumbnail(const int& numPhoto)
{
	ExecuteRequestWithNoResult(
		"INSERT INTO PHOTOSWIHOUTTHUMBNAIL (FullPath, Priority, ProcessStart) (SELECT FullPath, 1, 0 FROM PHOTOS WHERE NumPhoto = "
		+ to_string(numPhoto) + ")");
	ExecuteRequestWithNoResult(
		"DELETE FROM PHOTOSTHUMBNAIL WHERE FullPath in (SELECT FullPath FROM PHOTOS WHERE NumPhoto = " +
		to_string(numPhoto) + ")");
}

bool CSqlThumbnail::EraseThumbnail()
{
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
#ifdef WIN32
	documentPath.append("\\Thumbnail");
#else
	documentPath.append("/Thumbnail");
#endif

	wxArrayString files;
	wxDir::GetAllFiles(documentPath, &files, wxEmptyString, wxDIR_FILES);

	//tbb::parallel_for(0, (int)files.size(), 1, [=](int i)
	//	{
	for (int i = 0; i < listPhoto.size(); i++)
	{
		wxString filename = files[i];
		if (wxFileExists(filename))
		{
			{
#ifdef WIN32
				std::remove(filename);
#else
				wxRemoveFile(filename);
#endif
			}
			CThumbnailBuffer::RemovePicture(filename);
		}
	}
	//});

	return (ExecuteRequestWithNoResult("DELETE FROM PHOTOSTHUMBNAIL") != -1) ? true : false;
}

bool CSqlThumbnail::EraseFolderThumbnail(const int& numFolder)
{
	type = 7;
	listPhoto.clear();
	ExecuteRequest("SELECT NumPhoto FROM PHOTOS WHERE NumFolderCatalog = " + to_string(numFolder));

	//tbb::parallel_for(0, (int)listPhoto.size(), 1, [=](int i)
	//{
	for (int i = 0; i < listPhoto.size(); i++)
	{
		int idPhoto = listPhoto[i];
		wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(idPhoto));
		if (wxFileExists(thumbnail))
		{
			{
#ifdef WIN32
				std::remove(thumbnail);
#else
				wxRemoveFile(thumbnail);
#endif
			}
			CThumbnailBuffer::RemovePicture(thumbnail);
		}
	}
	//});
	return (ExecuteRequestWithNoResult(
		       "DELETE FROM PHOTOSTHUMBNAIL WHERE FullPath in (SELECT FullPath FROM PHOTOS WHERE NumFolderCatalog = " +
		       to_string(numFolder) + ")") != -1)
		       ? true
		       : false;
}

int CSqlThumbnail::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		switch (type)
		{
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

		case 6:
			for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
			{
				switch (i)
				{
				case 0:
					numPhoto = sqlResult->ColumnDataInt(i);
					break;
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
				}
			}
			break;
		}

		nbResult++;
	}
	return nbResult;
};
