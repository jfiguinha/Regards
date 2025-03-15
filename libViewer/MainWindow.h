#pragma once
#include <Photos.h>
#include <WindowMain.h>
#include <ToolbarInterface.h>
#include "IconeList.h"
#include <InfosSeparationBar.h>
using namespace Regards::Window;


class CRegardsBitmap;
class CImageLoadingFormat;
class CPictureCategorieLoadData;
class CPictureCategorie;
class IStatusBarInterface;
class CFFmpegTranscoding;
class CompressionAudioVideoOption;
class CThreadPhotoLoading;

namespace Regards::Viewer
{
	class CMainWindow;
	class CImageList;
	class CExportDiaporama;
	class CToolbar;
	class CMainParam;
	class CCentralWindow;
	class CToolbarViewerMode;
	class CFolderProcess;




	class CMainWindow : public CWindowMain, public CToolbarInterface
	{
	public:
		CMainWindow(wxWindow* parent, wxWindowID id, IStatusBarInterface* statusbar, const bool& openFirstFile, const wxString& fileToOpen);
		~CMainWindow() override;
		void UpdateScreenRatio() override;

		bool SetFullscreen();
		bool SetFullscreenMode();
		bool SetScreen();
		void TransitionEnd();

		bool OpenFolder(const wxString& path);
		bool IsFullscreen();
		void ShowToolbar();
		bool IsVideo();

		void SetText(const int& numPos, const wxString& libelle);
		void SetRangeProgressBar(const int& range);
		void SetPosProgressBar(const int& position);
		void SetPictureMode();
		void SetViewerMode();
		void OpenFile(const wxString& fileToOpen);

		bool GetProcessEnd() override;
		void OnOpenFileOrFolder(wxCommandEvent& event);
		wxString GetFilename();

		void SaveParameter() override;

		bool GetInit()
		{
			return init;
		}

	private:

		void UpdateFolderStatic();

		static void NewVersionAvailable(void* param);
		static void CheckFile(void* param);

		void ClickShowButton(const int& id, const int& refresh);

		void SetDataToStatusBar(void* thumbnailMessage, const wxString& message);

		bool FindNextValidFile();
		bool FindPreviousValidFile();

		void OnEndDecompressFile(wxCommandEvent& event);
		void OnEditFile(wxCommandEvent& event);

		void OnUpdateInfos(wxCommandEvent& event);
		void OnShowToolbar(wxCommandEvent& event);
		void OnStatusSetText(wxCommandEvent& event);
		void OnEndCheckFile(wxCommandEvent& event);
		void OnSetRangeProgressBar(wxCommandEvent& event);
		void OnSetValueProgressBar(wxCommandEvent& event);
		void OnCriteriaUpdate(wxCommandEvent& event);
		void OnRefreshPicture(wxCommandEvent& event);
		void OnScanner(wxCommandEvent& event);
		void OnFaceAdd(wxCommandEvent& event);
		void OnVersionUpdate(wxCommandEvent& event);
		void UpdateStatusBarMessage(wxCommandEvent& event);

		void OnDeleteFace(wxCommandEvent& event);
		void OnExit(wxCommandEvent& event);
		void InitPictures(wxCommandEvent& event);
		void PictureVideoClick(wxCommandEvent& event);
		void CriteriaChange(wxCommandEvent& event);

		void OnPrint(wxCommandEvent& event);
		void OnPictureClick(wxCommandEvent& event);

		void PrintPreview(wxCommandEvent& event);

		void OnRemoveFileFromCheckIn(wxCommandEvent& event);
		void OnCheckInUpdateStatus(wxCommandEvent& event);

		void OnFaceInfosUpdate(wxCommandEvent& event);
		void OnExportFile(wxCommandEvent& event);
		void OnUpdateExifThumbnail(wxCommandEvent& event);

		void SetScreenEvent(wxCommandEvent& event);
		void OnExportDiaporama(wxCommandEvent& event);
		void RefreshFolderList(wxCommandEvent& event);

		void OnUpdateFolder(wxCommandEvent& event);
		void OnRefreshThumbnail(wxCommandEvent& event);
		void OnProcessThumbnail(wxCommandEvent& event);
		void OnModelUpdate(wxCommandEvent& event);

		void Resize() override;
		void ExportVideo(const wxString& filename);
		void ProcessIdle() override;
		void OnIdle(wxIdleEvent& evt) override;


		//------------------------------------------------------
		void UpdateMessage(wxCommandEvent& event);
		void ProcessThumbnail();
		static void LoadPicture(void* param);
		void ProcessThumbnail(wxString filename, int type, long longWindow);
		void UpdateThumbnailIcone(wxCommandEvent& event);
		int nbElementInIconeList = 0;
		int nbPhotoElement = 0;
		int nbElement = 0;
		int nbProcess = 0;
		bool stopToGetNbElement = false;
		int thumbnailPos = 0;
		//std::map<wxString, bool> listFile;


		bool isCheckNewVersion = false;
		wxString tempVideoFile = "";
		wxString tempAudioVideoFile = "";
		bool fullscreen;
		wxGauge* progressBar;
		wxStatusBar* statusBar;
		//CToolbar* toolbar;
		CCentralWindow* centralWnd;
		CToolbarViewerMode* toolbarViewerMode;
		std::thread* versionUpdate = nullptr;

		bool isCheckingFile = false;
		IStatusBarInterface* statusBarViewer;
		wxRect posWindow;

		wxString localFilename;
		std::atomic<int> nbProcessMD5;
		std::atomic<int> endApplication;
		bool showToolbar;
		CMainParam* viewerParam;
		bool multithread;
		bool needToReload;
		int typeAffichage;
		std::atomic<bool> updateCriteria;
		bool updateFolder = false;
		bool refreshFolder;

		bool start;
		bool criteriaSendMessage;
		bool checkVersion;
		bool setViewerMode = false;
		bool setPictureMode = false;
		int faceDetection = 0;
		bool isThumbnailProcess = false;
		CExportDiaporama* exportDiaporama = nullptr;
		wxString firstFileToShow = "";
		wxString oldRequest = "";
		bool init = true;
		vector<wxString> photoList;
		CFolderProcess* folderProcess = nullptr;
		std::map<wxString, bool> listFile;
	};
}