#include <header.h>
#include "SqlThumbnail.h"
#include "SqlLib.h"
#include "SqlEngine.h"
#include "SqlResult.h"
#include <RegardsBitmap.h>
#include <wx/mstream.h>
#include <turbojpeg.h>
#include <ConvertUtility.h>
#include <PictureData.h>
#include <FileUtility.h>
#include <libPicture.h>
#include <wx/file.h>
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

/*
CPictureData * CSqlThumbnail::GetJpegThumbnail(const wxString & path)
{
	type = 5;
	wxString thumbnail = CFileUtility::GetThumbnailPath(path);
	if (wxFileExists(thumbnail))
	{
		CLibPicture libPicture;
		libPicture.LoadPicture(thumbnail);
	}

	wxString fullpath(path);
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT FullPath, width, height, thumbnail FROM PHOTOSTHUMBNAIL WHERE FullPath = '" + fullpath + "'");
	return picture;
}
*/

bool CSqlThumbnail::TestThumbnail(const wxString & path, const wxString &hash)
{
	type = 2;
    wxString fullpath(path);
    fullpath.Replace("'", "''");
	ExecuteRequest("SELECT NumPhoto FROM PHOTOSTHUMBNAIL WHERE FullPath = '" + fullpath + "' and hash = '" + hash + "'");
	if (!find)
	{
		DeleteThumbnail(path);
	}
	return find;
}

bool CSqlThumbnail::TestThumbnail(const wxString & path)
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



bool CSqlThumbnail::InsertThumbnail(const wxString & path, const uint8_t * zBlob, const int &nBlob, const int & width, const int &height, const wxString &hash)
{
	bool returnValue = true;
	type = 6;
	wxString fullpath(path);
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT NumPhoto FROM PHOTOS WHERE FullPath = '" + fullpath + "'");
	
	wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(numPhoto));
	if (!wxFileExists(thumbnail))
	{
		wxFile fileOut;
		fileOut.Create(thumbnail, true);
		fileOut.Write(zBlob, nBlob);
		fileOut.Close();




		returnValue = ExecuteRequestWithNoResult("INSERT INTO PHOTOSTHUMBNAIL (NumPhoto, FullPath, width, height, hash) VALUES(" + to_string(numPhoto) + ",'" + fullpath + "'," + to_string(width) + "," + to_string(height) + ",'" + hash + "')");

	}
	return returnValue;
}

wxImage CSqlThumbnail::GetThumbnail(const wxString & path)
{
	type = 6;
	wxString fullpath(path);
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT NumPhoto FROM PHOTOS WHERE FullPath = '" + fullpath + "'");

	wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(numPhoto));
	wxImage image;
	if (wxFileExists(thumbnail))
		image.LoadFile(thumbnail, wxBITMAP_TYPE_JPEG);
	return image;
	/*
	type = 1;
	wxString fullpath(path);
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT FullPath, width, height, hash, thumbnail FROM PHOTOSTHUMBNAIL WHERE FullPath = '" + fullpath + "'");
	return bitmap;
	*/
}

CImageLoadingFormat * CSqlThumbnail::GetPictureThumbnail(const wxString & path)
{
	type = 6;
	wxString fullpath(path);
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT NumPhoto FROM PHOTOS WHERE FullPath = '" + fullpath + "'");

	CImageLoadingFormat * picture = nullptr;
	wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(numPhoto));
	if (wxFileExists(thumbnail))
	{
		CLibPicture libPicture;
		picture = libPicture.LoadPicture(thumbnail);
		picture->SetFilename(thumbnail);
	}
	return picture;

	/*
	type = 3;
	wxString fullpath(path);
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT FullPath, width, height, hash, thumbnail FROM PHOTOSTHUMBNAIL WHERE FullPath = '" + fullpath + "'");
    if(regardsBitmap != nullptr)
        regardsBitmap->SetFilename(path);
	return regardsBitmap;
	*/
}

bool CSqlThumbnail::DeleteThumbnail(const wxString & path)
{
	type = 6;
	wxString fullpath(path);
	fullpath.Replace("'", "''");
	ExecuteRequest("SELECT NumPhoto FROM PHOTOS WHERE FullPath = '" + fullpath + "'");
	wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(numPhoto));
	if (wxFileExists(thumbnail))
	{
		wxRemoveFile(thumbnail);
	}
	return (ExecuteRequestWithNoResult("DELETE FROM PHOTOSTHUMBNAIL WHERE FullPath = '" + fullpath + "'") != -1) ? true : false;
}

bool CSqlThumbnail::DeleteThumbnail(const int & numPhoto)
{
	wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(numPhoto));
	if (wxFileExists(thumbnail))
	{
		wxRemoveFile(thumbnail);
	}

	return (ExecuteRequestWithNoResult("DELETE FROM PHOTOSTHUMBNAIL WHERE FullPath in (SELECT FullPath FROM PHOTOS WHERE NumPhoto = " + to_string(numPhoto) + ")") != -1) ? true : false;
}

bool CSqlThumbnail::EraseThumbnail()
{
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
#ifdef WIN32
	documentPath.append("\\Thumbnail");
#else
	documentPath.append("/Thumbnail");
#endif

	wxRmdir(documentPath);
	return (ExecuteRequestWithNoResult("DELETE FROM PHOTOSTHUMBNAIL") != -1) ? true : false;
}

bool CSqlThumbnail::EraseFolderThumbnail(const int &numFolder)
{
	type = 7;
	listPhoto.clear();
	ExecuteRequest("SELECT NumPhoto FROM PHOTOS WHERE NumFolderCatalog = " + to_string(numFolder));
	for (int idPhoto : listPhoto)
	{
		wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(idPhoto));
		if (wxFileExists(thumbnail))
		{
			wxRemoveFile(thumbnail);
		}
	}
	return (ExecuteRequestWithNoResult("DELETE FROM PHOTOSTHUMBNAIL WHERE FullPath in (SELECT FullPath FROM PHOTOS WHERE NumFolderCatalog = " + to_string(numFolder) + ")") != -1) ? true : false;
}

int CSqlThumbnail::TraitementResult(CSqlResult * sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		int width;
		int height;
		wxString hash = "";
		wxString filename = "";
		switch (type)
		{
		case 3:
		case 1:
			for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
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
					hash = sqlResult->ColumnDataText(i);
					break;

				/*
				case 4:
				{
					int size = sqlResult->ColumnDataBlobSize(i);
					if (size > 0)
					{
						uint8_t* data = new uint8_t[size];
						sqlResult->ColumnDataBlob(i, (void*&)data, size);
						if (type == 3)
						{
							tjhandle _jpegDecompressor = tjInitDecompress();
							regardsBitmap = new CRegardsBitmap(width, height);

							tjDecompress2(_jpegDecompressor, data, size, regardsBitmap->GetPtBitmap(), regardsBitmap->GetBitmapWidth(), 0, regardsBitmap->GetBitmapHeight(), TJPF_BGRX, TJFLAG_FASTDCT | TJFLAG_BOTTOMUP);

							tjDestroy(_jpegDecompressor);

						}
						else
						{
							wxMemoryInputStream jpegStream(data, size);
							bitmap.LoadFile(jpegStream, wxBITMAP_TYPE_JPEG);	
						}
						delete[] data;
					}

				}
				break;
				*/
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
			/*
		case 5:
			picture = new CPictureData();
			for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
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
					
					find = true;

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
					break;
				}
			}
			break;
			*/
		}

		nbResult++;
	}
	return nbResult;
};