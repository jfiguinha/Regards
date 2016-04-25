#pragma once
#include "Toolbar.h"
#include <StatusBarInterface.h>
#include "CentralWnd.h"
#include <EffectVideoParameter.h>
#include <thread>
#include <WindowMain.h>

using namespace Regards::Window;
using namespace std;

class CRegardsBitmap;

#ifdef __APPLE__
class CAppleSelectFile;
#endif



namespace Regards
{
	namespace Viewer
	{

		class CMainWindow;
        
        class CThreadVideoData
        {
        public:
            
            CThreadVideoData()
            {
                
            }
            
            ~CThreadVideoData()
            {
            }
            
            CMainWindow * mainWindow = nullptr;
            wxString video;

        };

		class CThreadPictureData
		{
		public:

			CThreadPictureData()
			{

			}

			~CThreadPictureData()
			{
			}

			CMainWindow * mainWindow = nullptr;
			wxString picture;
			bool isVisible = false;
            std::thread * myThread = nullptr;
		};
        
        class CBitmapReturn
        {
        public:
            CRegardsBitmap * bitmap = nullptr;
            bool isThumbnail = false;
            std::thread * myThread = nullptr;
        };

		class CMainWindow : public CWindowMain
		{
		public:
			CMainWindow(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarViewer);
			~CMainWindow();
			void SetDatabase(const wxString & folderPath = "", const wxString & requestSql = "");
			void SetFolder(const wxString & folderPath, const wxString &filename = L"");
			void SetPictureFile(const wxString & filePath);

            void UpdateScreenRatio();
			void StartDiaporama();
			void StopDiaporama();
			void ImageSuivante();
			void ImagePrecedente();
			void ImageFin();
			void ImageDebut();
			void Reload();
			void SetFullscreen();
			void SetFullscreenMode();
			void PictureClick(const int &numPhoto);
			void PictureVideoClick(const int &timePosition);
			void SetScreen();
			void TransitionEnd();
			void VideoEnd();
			void OpenFolder();
			bool IsFullscreen();
			void ShowToolbar();
			void Exit();
			void OnShowToolbar(wxCommandEvent& event);
			void SetText(const int &numPos, const wxString &libelle);
			void SetRangeProgressBar(const int &range);
			void SetPosProgressBar(const int &position);
			void SetVideoPos(wxCommandEvent& event);
            bool RefreshFolder(const wxString &sqlRequest = "");
            void InitSaveParameter();
		private:
            
#ifdef VIEWER
            void CriteriaChange(wxCommandEvent& event);
#endif
			void RefreshPictureList(wxCommandEvent& event);
            void ThumbnailPosPicture(wxCommandEvent& event);
			void OnKeyDown(wxKeyEvent& event);
			void ShowPicture(wxCommandEvent& event);
			void EndPictureThread(wxCommandEvent& event);
			void OnVideoEnd(wxCommandEvent& event);
            void OnVideoStart(wxCommandEvent& event);
			void EndThumbnailThread(wxCommandEvent& event);
            void EndVideoThread(wxCommandEvent& event);
			void OnTimerDiaporama(wxTimerEvent& event);
			void Resize();

            static void PositionPicture(CMainWindow * mainWnd);
            static void LoadingVideo(CThreadVideoData * videoData);
			static void LoadingThumbnail(CThreadPictureData * pictureData);
			static void LoadingNewPicture(CThreadPictureData * pictureData);

			void CheckRenderVideo();
			void CheckRenderEffect();
			void CheckRenderPreview();
			void CheckRenderTransition();

			void SetPicture(CRegardsBitmap * bitmap, const bool &isThumbnail);
			void LoadingPicture(const wxString &filenameToShow);
			void LoadPicture(const int &numElement);
			void LoadPictureInThread(const wxString &filename);
			void UpdateInfos(CRegardsBitmap * bitmap);
			void ExecuteRegardsShop(const wxString &filename);
            void LoadThumbnail(const wxString &filename);
			bool fullscreen = false;

			wxGauge * progressBar = nullptr;
			wxStatusBar * statusBar = nullptr;
			CToolbar * toolbar = nullptr;
			CCentralWnd * centralWnd = nullptr;
			CVideoEffectParameter * videoEffectParameter = nullptr;
			wxTimer * diaporamaTimer = nullptr;
			IStatusBarInterface * statusBarViewer = nullptr;

			wxRect posWindow;
			wxString filename;
			bool startDiaporama = false;
			std::thread * threadloadThumbnail = nullptr;
			std::thread * threadloadPicture = nullptr;
            std::thread * threadloadVideo = nullptr;
            std::thread * threadPosPicture = nullptr;
			std::mutex muThumbnail;
			std::mutex muPicture;
            std::mutex muVideo;
            
            wxString GetFilePath(const int &numElement);
            void CleanPictureFiles();
            void AddPicturesFiles(const wxString &filepath);
            void FindFileIndex(const wxString & filename);
            void GetFolderCredential(const wxString & folderPath);
            void OnTimerLoadPicture(wxTimerEvent& event);
            
#ifdef __APPLE__
            
            CAppleSelectFile * selectFolder = nullptr;
            CAppleSecurityScopeData * appleSecurityScopeData = nullptr;
#endif
            vector<wxString> pictureFiles;
			//int nbElement = 0;
			int numElement = 0;
			wxString firstFileToShow;
			bool showToolbar = true;
			CViewerParam * viewerParam = nullptr;
            int idFolder = 0;
            wxString folderPath;
            bool multithread = true;
            bool needToReload = false;
            wxTimer * loadPictureTimer;
            wxString filenameTimer;
		};
	}

}