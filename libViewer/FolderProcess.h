#pragma once
#include "MainWindow.h"
#include "InfosSeparationBarExplorer.h"
#include "TreatmentData.h"

using namespace Regards::Window;


namespace Regards::Viewer
{
	class CMainWindow;


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

		std::thread* checkFile = nullptr;
		CMainWindow* mainWindow;

	};

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

		~CThreadVideoData()
		{
		};

		CMainWindow* mainWindow;
		wxString video;
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
		~CFolderProcess();
		void UpdateCriteria(bool criteriaSendMessage);
		void RefreshFolder(bool& folderChange, int& nbFile);

	private:
		CMainWindow* mainWindow;
		//void UpdateFolder(CThreadPhotoLoading* threadData);

		wxString oldRequest = "";
	};

	class CThumbnailProcess
	{
	public:
		CThumbnailProcess(CMainWindow* parent)
		{
			this->parent = parent;
		}
		~CThumbnailProcess()
		{
		};


		void ProcessThumbnail(wxString filename, int type, long longWindow, int& nbProcess);

	private:
		static void LoadPicture(void* param);
		CMainWindow* parent = nullptr;
	};
}