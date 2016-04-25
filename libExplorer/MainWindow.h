#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
#include <FolderCatalog.h>
#include <Photos.h>
#include <ListCriteriaPhoto.h>
#include "ExplorerParam.h"
#include "ExplorerTheme.h"
#include "CentralWnd.h"
#include "MainToolbar.h"
#include <WindowMain.h>
#include <MainInterface.h>
#include <thread>
#ifdef __APPLE__
#include <AppleSelectFile.h>
#endif
using namespace std;
using namespace Regards::Window;

namespace Regards
{
	namespace Explorer
	{
        class CMainWindow;
        
        class CFindPhotoCriteria
        {
        public:
            wxString urlServer;
            CMainWindow * mainWindow = nullptr;
            bool criteriaNew = false;

            CPhotos photo;
            std::thread * phthread = nullptr;
        };
        
#ifdef __APPLE__
        class CAccessAppleFolder
        {
        public:
            CAppleSelectFile * selectFolder = nullptr;
            CAppleSecurityScopeData * appleSecurityScopeData = nullptr;
        };
#endif
        
		class CMainWindow : public CWindowMain
		{
		public:
			CMainWindow(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarViewer, IMainInterface * mainInterface);
			~CMainWindow();

			int AddFolder();
			void RemoveFolder(const int &idFolder);
			void ShowRegardsViewer();
			void RefreshFolder();
			void ShowToolbar();
			void ShowPreview();
			void ShowFolder();
			void ShowPreviewInfos();
			void Exit();
			void RefreshViewer();

			void SetText(const int &numPos, const wxString &libelle);
			void SetRangeProgressBar(const int &range);
			void SetPosProgressBar(const int &position);
            
            void UpdateScreenRatio();
            
            void ImageSuivante();
            void ImagePrecedente();
            void ImageFin();
            void ImageDebut();
            void RefreshFilter();

		private:
            static void FindPhotoCriteria(CFindPhotoCriteria * findPhotoCriteria);
            
			void RefreshPhotoList();
            void CriteriaUpdate(wxCommandEvent& event);
            void CriteriaChange(wxCommandEvent& event);
			void OnShowToolbar(wxCommandEvent& event);
            void OnVideoStart(wxCommandEvent& event);
			void OnIdle(wxIdleEvent& evt);
			void Resize();

			void CheckRenderVideo();
			void CheckRenderTransition();
			void CheckRenderPreview();
			void OnTimerRefreshListFolder(wxTimerEvent& event);
            void OnTimerRefreshList(wxTimerEvent& event);
            
			void RefreshThreadFolder(CFolderCatalog * folder);

			bool refreshFolder = false;

			CMainToolbar * toolbar = nullptr;
			CCentralWnd * centralWnd = nullptr;
			CVideoEffectParameter * videoEffectParameter = nullptr;
			IMainInterface * mainInterface = nullptr;

			wxTimer * timerRefreshFolder = nullptr;
            wxTimer * timerRefreshList = nullptr;
			wxGauge * progressBar = nullptr;
			wxStatusBar * statusBar = nullptr;

			IStatusBarInterface * statusBarExplorer = nullptr;
			CExplorerParam * explorerParam = nullptr;
			CExplorerTheme * explorerTheme = nullptr;

			wxRect rcPositionWindow;
			bool update = false;
			PhotosVector photosVector;
			int numImage = 0;
            int numProcess = 0;
			bool showToolbar = false;
            bool traitementEnd = true;
			PhotosVector::const_iterator it;
           
            std::thread thPhoto;
            
#ifdef __APPLE__
            vector<CAccessAppleFolder> appleFolderAccessVector;
#endif
		};
	}
}
