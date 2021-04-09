#pragma once
#include "FileUtility.h"
#include <WindowMain.h>
using namespace Regards::Window;

class CRegardsBitmap;
class CImageLoadingFormat;
class CPictureCategorieLoadData;
class CPictureCategorie;
class IStatusBarInterface;
class CFFmpegTranscoding;

namespace Regards
{
	namespace Viewer
	{

		class CMainWindow;
        class CImageList;
		
		class CToolbar;
		class CMainParam;
		class CCentralWindow;



		class CMainWindow : public CWindowMain
		{
		public:
			CMainWindow(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusbar, const bool & openFirstFile);
			~CMainWindow();
            void UpdateScreenRatio();
			void StartDiaporama();
			void StopDiaporama();
			void ImageSuivante();
			void ImagePrecedente();
			void ImageFin();
			void ImageDebut();
			//void Reload();
			void SetFullscreen();
			void SetFullscreenMode();
			void SetScreen();
			void TransitionEnd();
			void VideoEnd();
			bool OpenFolder(const wxString &path);
			bool IsFullscreen();
			void ShowToolbar();
			
			void SetText(const int &numPos, const wxString &libelle);
			void SetRangeProgressBar(const int &range);
			void SetPosProgressBar(const int &position);
			void AddFolder(const wxString &folder);
			void OpenFile(const wxString &fileToOpen);
			void OnFacePertinence();
			void SetSelectFile(const wxString &filename);
			bool GetProcessEnd();
			void OnAddFolder(wxCommandEvent& event);
            void OnOpenFileOrFolder(wxCommandEvent& event);
			wxString GetFilename();
            bool IsVideo();

			void SaveParameter();
            
		private:
        
			void OnEndDecompressFile(wxCommandEvent& event);
			void OnEditFile(wxCommandEvent& event);
            void OnPicturePrevious(wxCommandEvent& event);
            void OnPictureNext(wxCommandEvent& event);

			void SetListeFile(wxCommandEvent& event);
			void OnUpdateInfos(wxCommandEvent& event);
			void OnShowToolbar(wxCommandEvent& event);
			void OnStatusSetText(wxCommandEvent& event);
			void OnSetRangeProgressBar(wxCommandEvent& event);
			void OnSetValueProgressBar(wxCommandEvent& event);
			void OnEndPictureLoad(wxCommandEvent& event);
			void OnRefreshPicture(wxCommandEvent& event);
			void OnScanner(wxCommandEvent& event);
			void UpdateCriteriaMessage(wxCommandEvent& event);
			void UpdateThumbnailMessage(wxCommandEvent& event);
			void UpdateFaceMessage(wxCommandEvent& event);

			void OnExit(wxCommandEvent& event);
			void InitPictures(wxCommandEvent& event);
			void PictureVideoClick(wxCommandEvent& event);
            void CriteriaChange(wxCommandEvent& event);
			void OnVideoEnd(wxCommandEvent& event);
            void OnVideoStart(wxCommandEvent& event);
			void StartDiaporamaMessage(wxCommandEvent& event);
			void OnRemoveFolder(wxCommandEvent& event);
			void OnTimerDiaporama(wxTimerEvent& event);
			void OnPrint(wxCommandEvent& event);
			//void OnTimerRefresh(wxTimerEvent& event);
			void OnPictureClick(wxCommandEvent& event);
			void Md5Checking(wxCommandEvent& event);
			void PrintPreview(wxCommandEvent& event);
			void RefreshCriteriaPictureList(wxCommandEvent& event);
			void OnFaceInfosStatusBarUpdate(wxCommandEvent& event);
			void OnFaceInfosUpdate(wxCommandEvent& event);
			void OnExportFile(wxCommandEvent& event);
            //void RefreshTimer(wxCommandEvent& event);
            void StopAnimation(wxCommandEvent& event);
            void SetScreenEvent(wxCommandEvent& event);
			void Resize();
			int LoadPicture(const bool &first = false);
			void UpdatePicture();
			void ProcessIdle();
			void OnIdle(wxIdleEvent& evt);
			void OnEndThumbnail(wxCommandEvent& event);
			static void CheckMD5(void * param);

			bool fullscreen;
			
			wxGauge * progressBar;
			wxStatusBar * statusBar;
			CToolbar * toolbar;
			CCentralWindow * centralWnd;
			wxTimer * diaporamaTimer;
			//wxTimer * refreshTimer;
			IStatusBarInterface * statusBarViewer;

			wxRect posWindow;
			wxString filename;
			bool startDiaporama;
			mutex muPicture;
			int nbProcessMD5;
			
            CImageList * imageList = nullptr;
			int numElement;
			wxString firstFileToShow;
			bool showToolbar;
			CMainParam * viewerParam;
            bool multithread;
            bool needToReload;
			int typeAffichage;
			bool updateCriteria;
			bool updateFolder;
			//bool updatePicture;
			bool refreshFolder;
			int numElementTraitement;
			bool start;
			bool criteriaSendMessage;
			//bool videoStart;
			//bool sendMessageVideoStop;
            bool checkVersion;
           // bool isInit = false;
            bool setViewerMode = false;
			wxString lastFileToShow = "";
			//bool blockImage = false;

			CFFmpegTranscoding * ffmpegEncoder = nullptr;
		};
	}

}
