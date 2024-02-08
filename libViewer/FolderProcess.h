#pragma once
#include "MainWindow.h"
#include "InfosSeparationBarExplorer.h"
#include "TreatmentData.h"

using namespace Regards::Window;


namespace Regards::Viewer
{
	class CMainWindow;

	class CFolderProcess
	{
	public:
		CFolderProcess(CMainWindow* mainWindow);
		~CFolderProcess();
		void UpdateCriteria(bool criteriaSendMessage);
		void RefreshFolder(bool& folderChange, int& nbFile);

	private:
		CMainWindow* mainWindow;
		wxString oldRequest = "";
	};
}