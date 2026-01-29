#include <header.h>
#include "FolderProcess.h"
#include <IconeList.h>
#include "InfosSeparationBar.h"
#include <Photos.h>
#include <FolderCatalog.h>
#include <SqlFindFolderCatalog.h>
#include <SQLRemoveData.h>
#include <SqlFindPhotos.h>
#include <SqlThumbnail.h>
#include <SqlThumbnailVideo.h>
#include <window_id.h>
#include <LibResource.h>
#include <MainWindow.h>
#include <MainParam.h>
#include <MainParamInit.h>
#include <SqlInsertFile.h>
#include <CategoryFolderWindow.h>
#include <ThumbnailBuffer.h>
#include <config_id.h>
#include <TreatmentData.h>
#include <ThumbnailDataSQL.h>
#include <theme.h>
#include <MainTheme.h>
#include <MainThemeInit.h>
#include <ThreadLoadingBitmap.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <libPicture.h>
#include <CentralWindow.h>
#include <ImageLoadingFormat.h>
using namespace Regards::Window;
using namespace Regards::Viewer;
using namespace Regards::Sqlite;
using namespace Regards::Picture;

void CThreadCheckFile::CheckFile(void* param)
{
	CThreadCheckFile* checkFile = (CThreadCheckFile*)param;
	if (checkFile != nullptr)
	{
		int numElementTraitement = 0;

		PhotosVector* _pictures = new PhotosVector();
		CSqlFindPhotos sqlFindPhotos;
		sqlFindPhotos.SearchPhotosByCriteriaFolder(_pictures);

		wxString nbElement = to_string(_pictures->size());
		for (int i = 0; i < _pictures->size(); i++)
		{
			CPhotos photo = _pictures->at(i);

			int nbProcesseur = 1;

			if (wxFileName::FileExists(photo.GetPath()))
			{
				//Test si thumbnail valide
				CMainParam* config = CMainParamInit::getInstance();
				if (config != nullptr)
				{
					if (config->GetCheckThumbnailValidity() && checkFile->mainWindow->nbProcessMD5 < nbProcesseur)
					{
						CSqlThumbnail sqlThumbnail;
						CSqlThumbnailVideo sqlThumbnailVideo;
						wxFileName file(photo.GetPath());
						wxULongLong sizeFile = file.GetSize();
						wxString md5file = sizeFile.ToString();

						bool result = sqlThumbnail.TestThumbnail(photo.GetPath(), md5file);
						if (!result)
						{
							//Remove thumbnail
							sqlThumbnail.DeleteThumbnail(photo.GetPath());
							sqlThumbnailVideo.DeleteThumbnail(photo.GetPath());
						}

						wxCommandEvent evt(wxEVENT_UPDATECHECKINFOLDER);
						checkFile->mainWindow->GetEventHandler()->AddPendingEvent(evt);
					}
				}
			}
			else
			{
				//Remove file
				CSQLRemoveData::DeletePhoto(photo.GetId());
				wxCommandEvent evt(wxEVENT_UPDATECHECKINFOLDER);
				checkFile->mainWindow->GetEventHandler()->AddPendingEvent(evt);
			}

			numElementTraitement++;

			wxCommandEvent evt(wxEVENT_UPDATECHECKINSTATUS);
			evt.SetInt(numElementTraitement);
			evt.SetString(nbElement);
			checkFile->mainWindow->GetEventHandler()->AddPendingEvent(evt);

			if (checkFile->mainWindow->processEnd)
				break;
			std::this_thread::sleep_for(50ms);
		}

		delete _pictures;
	}


	wxCommandEvent evt(wxEVENT_ENDCHECKFILE);
	evt.SetClientData(checkFile);
	checkFile->mainWindow->GetEventHandler()->AddPendingEvent(evt);
}


CFolderProcess::CFolderProcess(CMainWindow* mainWindow)
{
	this->mainWindow = mainWindow;
}

CFolderProcess::~CFolderProcess()
{

}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CFolderProcess::UpdateCriteria(bool criteriaSendMessage)
{
	wxWindow* window = mainWindow->FindWindowById(CRITERIAFOLDERWINDOWID);
	if (window)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_UPDATECRITERIA);
		evt.SetExtraLong((criteriaSendMessage == true) ? 1 : 0);
		window->GetEventHandler()->AddPendingEvent(evt);
	}
}



//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CFolderProcess::RefreshFolder(bool & folderChange, int & nbFile)
{
	FolderCatalogVector folderList;
	CSqlFindFolderCatalog folderCatalog;
	folderCatalog.GetFolderCatalog(&folderList, NUMCATALOGID);



	//Test de la validité des répertoires
	for (CFolderCatalog folderlocal : folderList)
	{
		if (!wxDirExists(folderlocal.GetFolderPath()))
		{
			//Remove Folder
			CSQLRemoveData::DeleteFolder(folderlocal.GetNumFolder());
			folderChange = true;
		}
	}

	//Test de la validité des fichiers
	PhotosVector photoList;
	CSqlFindPhotos findphotos;
	findphotos.GetAllPhotos(&photoList);
	for (CPhotos photo : photoList)
	{
		if (!wxFileExists(photo.GetPath()))
		{
			//Remove Folder
			CSQLRemoveData::DeletePhoto(photo.GetId());
			folderChange = true;
		}
	}

	

	for (CFolderCatalog folderlocal : folderList)
	{
		wxString fichier;
		CSqlInsertFile sqlInsertFile;
		nbFile += sqlInsertFile.ImportFileFromFolder(folderlocal.GetFolderPath(), folderlocal.GetNumFolder(),
			fichier);
	}

	if (folderChange || nbFile > 0)
	{
		auto viewerParam = CMainParamInit::getInstance();
		wxString sqlRequest = viewerParam->GetLastSqlRequest();

		CSqlFindPhotos sqlFindPhotos;
		sqlFindPhotos.SearchPhotos(sqlRequest);
	}

}


void CThumbnailProcess::ProcessThumbnail(wxString filename, int type, long longWindow, int &nbProcess)
{
	int nbProcesseur = 1;
	if (CRegardsConfigParam* config = CParamInit::getInstance(); config != nullptr)
		nbProcesseur = config->GetThumbnailProcess() + 1;

	if (nbProcess >= nbProcesseur)
		return;


	if (filename != "")
	{
		nbProcess++;
		auto pLoadBitmap = new CThreadLoadingBitmap();
		pLoadBitmap->filename = filename;
		pLoadBitmap->window = parent;
		pLoadBitmap->longWindow = longWindow;
		pLoadBitmap->type = type;
		pLoadBitmap->_thread = new thread(LoadPicture, pLoadBitmap);

	}
	else
	{
		printf("error");
	}

}


void CThumbnailProcess::LoadPicture(void* param)
{

	//std::thread* t1 = nullptr;
	CLibPicture libPicture;
	auto threadLoadingBitmap = static_cast<CThreadLoadingBitmap*>(param);
	if (threadLoadingBitmap == nullptr)
		return;

	CImageLoadingFormat* imageLoad = libPicture.LoadThumbnail(threadLoadingBitmap->filename);
	if (imageLoad != nullptr)
	{
		threadLoadingBitmap->bitmapIcone = imageLoad->GetMatrix().getMat();
		delete imageLoad;
	}

	if (!threadLoadingBitmap->bitmapIcone.empty())
	{
		//Enregistrement en base de données
		CSqlThumbnail sqlThumbnail;
		wxFileName file(threadLoadingBitmap->filename);
		wxULongLong sizeFile = file.GetSize();
		wxString hash = sizeFile.ToString();
		wxString localName = sqlThumbnail.InsertThumbnail(threadLoadingBitmap->filename, threadLoadingBitmap->bitmapIcone.size().width, threadLoadingBitmap->bitmapIcone.size().height, hash);
		if (localName != "")
		{
			//threadLoadingBitmap->bitmapIcone.SaveFile(localName, wxBITMAP_TYPE_JPEG);
			cv::imwrite(CConvertUtility::ConvertToStdString(localName), threadLoadingBitmap->bitmapIcone);
		}

	}

	auto event = new wxCommandEvent(wxEVENT_ICONEUPDATE);
	event->SetClientData(threadLoadingBitmap);
	wxQueueEvent(threadLoadingBitmap->window, event);
}
