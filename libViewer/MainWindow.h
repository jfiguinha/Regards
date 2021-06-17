#pragma once
#include "FileUtility.h"
#include <Photos.h>
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
			bool SetFullscreen();
			bool SetFullscreenMode();
			bool SetScreen();
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
			bool GetProcessEnd();
			void OnAddFolder(wxCommandEvent& event);
            void OnOpenFileOrFolder(wxCommandEvent& event);
			wxString GetFilename();
            bool IsVideo();

			void SaveParameter();
            
		private:
        

			bool FindNextValidFile();
			bool FindPreviousValidFile();

			void OnEndDecompressFile(wxCommandEvent& event);
			void OnEditFile(wxCommandEvent& event);
            void OnPicturePrevious(wxCommandEvent& event);
            void OnPictureNext(wxCommandEvent& event);
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
			void OnPictureClick(wxCommandEvent& event);
			void Md5Checking(wxCommandEvent& event);
			void PrintPreview(wxCommandEvent& event);
			void RefreshCriteriaPictureList(wxCommandEvent& event);
			void OnFaceInfosStatusBarUpdate(wxCommandEvent& event);
			void OnFaceInfosUpdate(wxCommandEvent& event);
			void OnExportFile(wxCommandEvent& event);
			void OnUpdateExifThumbnail(wxCommandEvent& event);
            void StopAnimation(wxCommandEvent& event);
            void SetScreenEvent(wxCommandEvent& event);
			void OnExportDiaporama(wxCommandEvent& event);
			void Resize();
			void ExportVideo(const wxString& filename, const wxString& filenameOutput = "");
			void ProcessIdle();
			void OnIdle(wxIdleEvent& evt);
			void OnEndThumbnail(wxCommandEvent& event);
			static void CheckMD5(void * param);

			wxString tempVideoFile = "";
			wxString tempAudioVideoFile = "";
			bool fullscreen;
			wxGauge * progressBar;
			wxStatusBar * statusBar;
			CToolbar * toolbar;
			CCentralWindow * centralWnd;
			wxTimer * diaporamaTimer;
			IStatusBarInterface * statusBarViewer;
			wxRect posWindow;
			PhotosVector pictures;
			bool startDiaporama;
			wxString localFilename;
			int nbProcessMD5;

			bool showToolbar;
			CMainParam * viewerParam;
            bool multithread;
            bool needToReload;
			int typeAffichage;
			bool updateCriteria;
			bool updateFolder;
			bool refreshFolder;
			int numElementTraitement;
			bool start;
			bool criteriaSendMessage;
            bool checkVersion;
            bool setViewerMode = false;

			CFFmpegTranscoding * ffmpegEncoder = nullptr;
		};
	}

}
