#include <header.h>
#include "FolderProcess.h"
#include <Photos.h>
#include <FolderCatalog.h>
#include <window_id.h>
#include <MainWindow.h>
#include <SqlThumbnail.h>
#include <libPicture.h>
#include <MainParam.h>
#include <MainParamInit.h>
#include <ImageLoadingFormat.h>
#include <ThreadLoadingBitmap.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include "SqlBatchOps.h"
#include "FileSystemValidator.h"
#include "UIEventBatcher.h"
using namespace Regards::Window;
using namespace Regards::Viewer;
using namespace Regards::Sqlite;
using namespace Regards::Picture;

void CThreadCheckFile::CheckFile(void* param)
{
	auto* checkFile = static_cast<CThreadCheckFile*>(param);
	if (!checkFile)
		return;

	PhotosVector pictures;
	CSqlBatchOps::LoadPhotosByCriteria(pictures);

	CUIEventBatcher batcher(checkFile->mainWindow,
		wxEVENT_UPDATECHECKINSTATUS,
		/*batchSize=*/50);
	batcher.SetTotal(static_cast<int>(pictures.size()));

	// --- 1. Collect stale photo ids (no disk access inside SQL layer) -------
	std::vector<int>      staleIds = CFileSystemValidator::FindStalePhotos(pictures);
	std::vector<wxString> badThumbs;

	// --- 2. Optionally find thumbnails with wrong hash ----------------------
	CMainParam* config = CMainParamInit::getInstance();
	if (config && config->GetCheckThumbnailValidity())
		badThumbs = CFileSystemValidator::FindInvalidThumbnails(pictures);

	// --- 3. Batch-delete stale DB records -----------------------------------
	CSqlBatchOps::DeletePhotosBatch(staleIds);

	if (!staleIds.empty() || !badThumbs.empty())
		batcher.SendSignal(wxEVENT_UPDATECHECKINFOLDER);

	// --- 4. Batch-delete invalid thumbnails ---------------------------------
	if (!badThumbs.empty())
		CSqlBatchOps::DeleteThumbnailsBatch(badThumbs);

	// --- 5. Progress events (batched) ---------------------------------------
	for (int i = 0; i < static_cast<int>(pictures.size()); ++i)
	{
		if (checkFile->mainWindow->processEnd)
			break;

		batcher.Tick(i);   // fires event only every 50 items or on last item
	}

	// --- 6. Signal completion -----------------------------------------------
	wxCommandEvent done(wxEVENT_ENDCHECKFILE);
	done.SetClientData(checkFile);
	checkFile->mainWindow->GetEventHandler()->AddPendingEvent(done);
}


CFolderProcess::CFolderProcess(CMainWindow* mainWindow)
{
	this->mainWindow = mainWindow;
}

//---------------------------------------------------------------
//
//---------------------------------------------------------------
void CFolderProcess::RefreshFolder(bool & folderChange, int & nbFile)
{
	// --- 1. Load current catalog --------------------------------------------
	FolderCatalogVector folderList;
	PhotosVector        photoList;
	CSqlBatchOps::LoadAllFolders(folderList);
	CSqlBatchOps::LoadAllPhotos(photoList);

	// --- 2. Find stale records ----------------------------------------------
	std::vector<int> staleFolders = CFileSystemValidator::FindStaleFolders(folderList);
	std::vector<int> stalePhotos = CFileSystemValidator::FindStalePhotos(photoList);

	// --- 3. Batch-delete in one pass ----------------------------------------
	if (!staleFolders.empty())
	{
		CSqlBatchOps::DeleteFoldersBatch(staleFolders);
		folderChange = true;
	}

	if (!stalePhotos.empty())
	{
		CSqlBatchOps::DeletePhotosBatch(stalePhotos);
		folderChange = true;
	}

	// --- 4. Import new files from still-valid folders -----------------------
	// Remove stale folder entries from local list before import.
	FolderCatalogVector validFolders;
	validFolders.reserve(folderList.size());
	for (CFolderCatalog& f : folderList)
	{
		bool isStale = std::find(staleFolders.begin(), staleFolders.end(),
			f.GetNumFolder()) != staleFolders.end();
		if (!isStale)
			validFolders.push_back(f);
	}

	nbFile += CSqlBatchOps::ImportNewFiles(validFolders);

	// --- 5. Refresh search results only if something changed ----------------
	if (folderChange || nbFile > 0)
		CSqlBatchOps::RefreshSearchResults();

}
