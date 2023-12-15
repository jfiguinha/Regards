#include <header.h>
#include "FolderProcess.h"
#include <IconeList.h>
#include "InfosSeparationBar.h"
#include <Photos.h>
#include <FolderCatalog.h>
#include <SqlFindFolderCatalog.h>
#include <SQLRemoveData.h>
#include <SqlFindPhotos.h>
#include <window_id.h>
#include <LibResource.h>
#include <MainWindow.h>
#include <ThumbnailBuffer.h>
using namespace Regards::Window;
using namespace Regards::Viewer;
using namespace Regards::Sqlite;

class CThreadPhotoLoading
{
public:
	CThreadPhotoLoading()
	{
		_pictures = new PhotosVector();
		_listSeparator = new InfosSeparationBarVector();
	}

	~CThreadPhotoLoading() {};

	CMainWindow* mainWindow;
	CIconeList* iconeListLocal;
	InfosSeparationBarVector* _listSeparator;
	CIconeList* iconeListThumbnail;
	int typeAffichage;
	PhotosVector* _pictures;
};



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
void CFolderProcess::RefreshFolder()
{
	FolderCatalogVector folderList;
	CSqlFindFolderCatalog folderCatalog;
	folderCatalog.GetFolderCatalog(&folderList, NUMCATALOGID);

	bool folderChange = false;

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

	int nbFile = 0;

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
		UpdateFolderStatic();
		updateCriteria = true;
		processIdle = true;
	}
	refreshFolder = false;
	numElementTraitement = 0;
}

void CFolderProcess::UpdateFolderStatic()
{
	wxString libelle = CLibResource::LoadStringFromResource(L"LBLBUSYINFO", 1);
	//wxBusyInfo wait(libelle);
	wxBusyCursor busy;
	{
		CThreadPhotoLoading* threadData = new CThreadPhotoLoading();
		threadData->mainWindow = mainWindow;
		UpdateFolder(threadData);
	}
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CFolderProcess::UpdateFolder(void* param)
{
	CThreadPhotoLoading* threadData = (CThreadPhotoLoading*)param;
	wxString requestSql = "";
	CSqlFindPhotos sqlFindPhotos;

	auto categoryFolder = static_cast<CCategoryFolderWindow*>(FindWindowById(
		CATEGORYFOLDERWINDOWID));
	if (categoryFolder != nullptr)
		requestSql = categoryFolder->GetSqlRequest();



	if (requestSql != "" && threadData->mainWindow->init)
	{
		if (threadData->mainWindow->oldRequest != requestSql)
			sqlFindPhotos.SearchPhotos(requestSql);
		threadData->mainWindow->oldRequest = requestSql;
	}

	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
	{
		threadData->typeAffichage = config->GetTypeAffichage();
	}

	sqlFindPhotos.SearchPhotosByCriteria(threadData->_pictures);

	threadData->iconeListThumbnail = threadData->mainWindow->centralWnd->GetThumbnailPicture()->PregenerateList(threadData->_pictures);

	threadData->_pictures->clear();
	sqlFindPhotos.SearchPhotosByCriteriaFolder(threadData->_pictures);

	threadData->iconeListLocal = threadData->mainWindow->centralWnd->GetListPicture()->GetPtThumbnailFolder()->PrepareTypeAffichage(threadData->_pictures, threadData->typeAffichage, threadData->_listSeparator);

	wxCommandEvent event(wxEVENT_UPDATEPHOTOFOLDER);
	event.SetClientData(threadData);
	wxPostEvent(threadData->mainWindow, event);
}

void CFolderProcess::OnUpdatePhotoFolder(CThreadPhotoLoading* threadData)
{

	if (threadData != nullptr)
	{
		if (firstFileToShow != "")
			localFilename = firstFileToShow;
		else
			localFilename = centralWnd->GetFilename();

		CThumbnailBuffer::InitVectorList(threadData->_pictures);

		if (firstFileToShow == "")
		{
			bool isFound = false;

			if (!isFound && CThumbnailBuffer::GetVectorSize() > 0 && localFilename != "")
			{
				isFound = FindNextValidFile();
				if (!isFound)
					isFound = FindPreviousValidFile();
			}

			if (!isFound && CThumbnailBuffer::GetVectorSize() > 0)
				localFilename = CThumbnailBuffer::GetVectorValue(0).GetPath();
		}

		centralWnd->SetListeFile(localFilename, threadData);

		delete threadData;
		threadData = nullptr;

		if (updateFolderThread != nullptr)
		{
			updateFolderThread->join();
			delete updateFolderThread;
			updateFolderThread = nullptr;
		}


		firstFileToShow = "";
		numElementTraitement = 0;

		init = true;
	}
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CFolderProcess::PhotoProcess(CPhotos* photo)
{
	int nbProcesseur = 1;

	if (wxFileName::FileExists(photo->GetPath()))
	{
		//Test si thumbnail valide
		CMainParam* config = CMainParamInit::getInstance();
		if (config != nullptr)
		{
			if (config->GetCheckThumbnailValidity() && nbProcessMD5 < nbProcesseur)
			{
				auto path = new CThreadMD5();
				path->filename = photo->GetPath();
				path->mainWindow = this;
				path->thread = new thread(CheckMD5, path);
				nbProcessMD5++;
			}
			else
				numElementTraitement++;
		}
	}
	else
	{
		//Remove file
		CSQLRemoveData::DeletePhoto(photo->GetId());
		updateCriteria = true;
		UpdateFolderStatic();
		numElementTraitement++;
		processIdle = true;
	}

	wxString label = CLibResource::LoadStringFromResource(L"LBLFILECHECKING", 1);
	wxString message = label + to_string(numElementTraitement) + L"/" + to_string(CThumbnailBuffer::GetVectorSize());
	if (statusBarViewer != nullptr)
	{
		statusBarViewer->SetText(3, message);
	}
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CFolderProcess::Md5Checking(CThreadMD5 * path)
{
	if (path != nullptr)
	{
		if (path->thread != nullptr)
		{
			path->thread->join();
			delete(path->thread);
			path->thread = nullptr;
		}
		delete path;
	}
	nbProcessMD5--;
	numElementTraitement++;
	wxString label = CLibResource::LoadStringFromResource(L"LBLFILECHECKING", 1);
	wxString message = label + to_string(numElementTraitement) + L"/" + to_string(CThumbnailBuffer::GetVectorSize());
	if (statusBarViewer != nullptr)
	{
		statusBarViewer->SetText(3, message);
	}
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CFolderProcess::CheckMD5(void* param)
{
	auto path = static_cast<CThreadMD5*>(param);
	if (path != nullptr)
	{
		CSqlThumbnail sqlThumbnail;
		CSqlThumbnailVideo sqlThumbnailVideo;
		wxFileName file(path->filename);
		wxULongLong sizeFile = file.GetSize();
		wxString md5file = sizeFile.ToString();

		bool result = sqlThumbnail.TestThumbnail(path->filename, md5file);
		if (!result)
		{
			//Remove thumbnail
			sqlThumbnail.DeleteThumbnail(path->filename);
			sqlThumbnailVideo.DeleteThumbnail(path->filename);
		}

		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_MD5CHECKING);
		evt.SetClientData(path);
		path->mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}
