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
using namespace Regards::Window;
using namespace Regards::Viewer;
using namespace Regards::Sqlite;




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
void CFolderProcess::UpdateFolder(CThreadPhotoLoading* threadData)
{
	wxString requestSql = "";
	CSqlFindPhotos sqlFindPhotos;

	auto categoryFolder = static_cast<CCategoryFolderWindow*>(mainWindow->FindWindowById(
		CATEGORYFOLDERWINDOWID));
	if (categoryFolder != nullptr)
		requestSql = categoryFolder->GetSqlRequest();

	if (requestSql != "" && mainWindow->GetInit())
	{
		if (oldRequest != requestSql)
			sqlFindPhotos.SearchPhotos(requestSql);
		oldRequest = requestSql;
	}

	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
	{
		threadData->typeAffichage = config->GetTypeAffichage();
	}

	sqlFindPhotos.SearchPhotosByCriteria(threadData->_pictures);

	threadData->iconeListThumbnail = mainWindow->GetPreGenerateList(threadData);

	threadData->_pictures->clear();
	sqlFindPhotos.SearchPhotosByCriteriaFolder(threadData->_pictures);

	threadData->iconeListLocal = mainWindow->GetIconeList(threadData);

	wxCommandEvent event(wxEVENT_UPDATEPHOTOFOLDER);
	event.SetClientData(threadData);
	wxPostEvent(threadData->mainWindow, event);
}