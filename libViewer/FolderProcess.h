#pragma once
#include "InfosSeparationBarExplorer.h"
#include "TreatmentData.h"




namespace Regards::Viewer
{
	class CMainWindow;
}

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


namespace Regards::Viewer
{
	class CFolderProcess
	{
	public:
		CFolderProcess(CMainWindow* mainWindow);
		~CFolderProcess();
		void UpdateCriteria(bool criteriaSendMessage);
		void RefreshFolder(bool& folderChange, int& nbFile);
		void UpdateFolderStatic();


	private:
		CMainWindow* mainWindow;
		void UpdateFolder(CThreadPhotoLoading* threadData);

		wxString oldRequest = "";
	};
}