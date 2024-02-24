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
		wxCommandEvent evt(wxEVENT_UPDATECRITERIA);
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


