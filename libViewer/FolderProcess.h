#pragma once
#include "MainWindow.h"
#include "InfosSeparationBarExplorer.h"
#include "TreatmentData.h"

using namespace Regards::Window;


namespace Regards::Viewer
{
	class CMainWindow;


	class CFolderFiles
	{
	public:
		vector<wxString> pictureFiles;
		wxString folderName;
	};

	class CThreadVideoData
	{
	public:
		CThreadVideoData()
		{
			mainWindow = nullptr;
		}

		~CThreadVideoData();

		CMainWindow* mainWindow;
		wxString video;
	};

	class CThreadCheckFile
	{
	public:
		CThreadCheckFile()
		{
			mainWindow = nullptr;
		}

		~CThreadCheckFile()
		{
		};

		static void CheckFile(void* param);


		std::unique_ptr<std::thread> checkFile = nullptr;
		CMainWindow* mainWindow;
		int pictureSize;
		int numFile;
	};



	class CThreadPhotoLoading
	{
	public:
		CThreadPhotoLoading()
		{
			_pictures = new PhotosVector();
			_listSeparator = new InfosSeparationBarVector();
		}

		~CThreadPhotoLoading() {};

		Regards::Viewer::CMainWindow* mainWindow;
		CIconeList* iconeListLocal;
		InfosSeparationBarVector* _listSeparator;
		CIconeList* iconeListThumbnail;
		int typeAffichage;
		PhotosVector* _pictures;
	};

	class CFolderProcess
	{
	public:
		CFolderProcess(CMainWindow* mainWindow);
		~CFolderProcess() = default;
		void RefreshFolder(bool& folderChange, int& nbFile);
		//void UpdateFolderStatic();


	private:
		CMainWindow* mainWindow;
		//void UpdateFolder(CThreadPhotoLoading* threadData);

		wxString oldRequest = "";
	};


	
}