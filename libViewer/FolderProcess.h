#pragma once

class CThreadMD5
{
public:
	CThreadMD5()
	{
		thread = nullptr;
		mainWindow = nullptr;
	}

	~CThreadMD5()
	{
	}

	wxString filename;
	std::thread* thread;
	wxWindow* mainWindow;
};

namespace Regards::Viewer
{
	class CMainWindow;

	class CFolderProcess
	{
	public:
		CFolderProcess(CMainWindow* mainWindow);
		~CFolderProcess();
		void PhotoProcess(CPhotos* photo);
		void RefreshFolder();
		void UpdateFolder(void* param);
		void OnUpdatePhotoFolder(CThreadPhotoLoading* threadData);
		void UpdateFolderStatic();
		static void CheckMD5(void* param);
		void Md5Checking(CThreadMD5* path);

		int nbProcessMD5;
		int numElementTraitement;

	private:
		CMainWindow* mainWindow;
	};
}