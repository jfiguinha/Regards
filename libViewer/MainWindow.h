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

namespace Regards::Viewer
{
	class CMainWindow;
	class CImageList;

	class CToolbar;
	class CMainParam;
	class CCentralWindow;
	class CToolbarViewerMode;

	class CThreadPhotoLoading
	{
	public:
		CThreadPhotoLoading()
		{
			_pictures = new PhotosVector();
			_listSeparator = new InfosSeparationBarVector();
		}

		~CThreadPhotoLoading(){};

		CMainWindow* mainWindow;
		CIconeList* iconeListLocal;
		InfosSeparationBarVector* _listSeparator;
		CIconeList* iconeListThumbnail;
		int typeAffichage;
		PhotosVector * _pictures;
	};



	class CMainWindow : public CWindowMain, public CToolbarInterface
	{
	public:
		CMainWindow(wxWindow* parent, wxWindowID id, IStatusBarInterface* statusbar, const bool& openFirstFile);
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

		void OpenFile(const wxString& fileToOpen);

		bool GetProcessEnd() override;
		void OnOpenFileOrFolder(wxCommandEvent& event);
		wxString GetFilename();

		void SaveParameter() override;

	private:

		void ClickShowButton(const int& id, const int& refresh);

		void SetDataToStatusBar(void* thumbnailMessage, const wxString& message);

		bool FindNextValidFile();
		bool FindPreviousValidFile();

		void OnEndDecompressFile(wxCommandEvent& event);
		void OnEditFile(wxCommandEvent& event);

		void OnUpdateInfos(wxCommandEvent& event);
		void OnShowToolbar(wxCommandEvent& event);
		void OnStatusSetText(wxCommandEvent& event);

		void OnSetRangeProgressBar(wxCommandEvent& event);
		void OnSetValueProgressBar(wxCommandEvent& event);
		void OnCriteriaUpdate(wxCommandEvent& event);
		void OnRefreshPicture(wxCommandEvent& event);
		void OnScanner(wxCommandEvent& event);
		void OnFaceAdd(wxCommandEvent& event);

		void UpdateStatusBarMessage(wxCommandEvent& event);

		void OnDeleteFace(wxCommandEvent& event);
		void OnExit(wxCommandEvent& event);
		void InitPictures(wxCommandEvent& event);
		void PictureVideoClick(wxCommandEvent& event);
		void CriteriaChange(wxCommandEvent& event);

		void OnPrint(wxCommandEvent& event);
		void OnPictureClick(wxCommandEvent& event);
		void Md5Checking(wxCommandEvent& event);
		void PrintPreview(wxCommandEvent& event);

		void OnFaceInfosStatusBarUpdate(wxCommandEvent& event);
		void OnFaceInfosUpdate(wxCommandEvent& event);
		void OnExportFile(wxCommandEvent& event);
		void OnUpdateExifThumbnail(wxCommandEvent& event);

		void SetScreenEvent(wxCommandEvent& event);
		void OnExportDiaporama(wxCommandEvent& event);
		void RefreshFolderList(wxCommandEvent& event);
		void OnUpdatePhotoFolder(wxCommandEvent& event);
		void OnUpdateFolder(wxCommandEvent& event);

		void Resize() override;
		void ExportVideo(const wxString& filename, const wxString& filenameOutput = "");
		void ProcessIdle() override;
		void OnIdle(wxIdleEvent& evt) override;
		void OnEndThumbnail(wxCommandEvent& event);
		static void CheckMD5(void* param);


		void UpdateCriteria();
		void RefreshFolder();
		static void UpdateFolder(void * param);
		void PhotoProcess(CPhotos* photo);

		wxString tempVideoFile = "";
		wxString tempAudioVideoFile = "";
		bool fullscreen;
		wxGauge* progressBar;
		wxStatusBar* statusBar;
		CToolbar* toolbar;
		CCentralWindow* centralWnd;
		CToolbarViewerMode * toolbarViewerMode;

		IStatusBarInterface* statusBarViewer;
		wxRect posWindow;

		std::mutex muFolderThread;
		std::thread* updateFolderThread = nullptr;
		wxString localFilename;
		int nbProcessMD5;

		bool showToolbar;
		CMainParam* viewerParam;
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
		//CompressionAudioVideoOption* compressAudioVideoOption = nullptr;
		CFFmpegTranscoding* ffmpegEncoder = nullptr;
		wxString fileOut = "";
		wxString fileOutAudio = "";
		wxString fileOutVideo = "";
		wxString filepathVideo = "";
		wxString firstFileToShow = "";
		bool needToRemux = false;
		bool isAudio = false;
		bool init = false;
		wxString oldRequest = "";
	};
}
