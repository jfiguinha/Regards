#pragma once
#include "Toolbar.h"
#include <StatusBarInterface.h>
#include "CentralWindow.h"
#include "FileUtility.h"
#include <EffectVideoParameter.h>
#include <WindowMain.h>
using namespace Regards::Window;

class CRegardsBitmap;
class CImageLoadingFormat;
class CPictureCategorieLoadData;
class CPictureCategorie;

namespace Regards
{
	namespace Viewer
	{

		class CMainWindow;
        class CImageList;

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
            }
            
            CMainWindow * mainWindow;
            wxString video;

        };

		class CWaitingWindow : public CWindowMain
		{
		public:
			CWaitingWindow(wxWindow* parent, wxWindowID id) : CWindowMain("CWaitingWindow",parent, id)
			{
				textToShow = "Please wait ...";
				wxString resourcePath = CFileUtility::GetResourcesFolderPath();
				//m_animation = new wxAnimation(resourcePath + "/loading.gif");
				m_animationCtrl = new wxAnimationCtrl(this, wxID_ANY);
				m_animationCtrl->Show(true);
				Connect(wxEVT_PAINT, wxPaintEventHandler(CWaitingWindow::OnPaint));

			#ifdef WIN32
				m_animationCtrl->LoadFile(resourcePath + "\\loading.gif");
			#else
				m_animationCtrl->LoadFile(resourcePath + "//loading.gif");
			#endif

				m_animationCtrl->SetBackgroundColour(wxColour("white"));
#ifdef __WXGTK__
                m_animationCtrl->Play();
#else                
				m_animationCtrl->Play(true);
#endif
			};

			~CWaitingWindow(){
				if (m_animationCtrl != nullptr)
					delete m_animationCtrl;
			};

			void UpdateScreenRatio()
			{
				Resize();
			}

			void Resize()
			{
            #ifdef __WXGTK__
            #if wxCHECK_VERSION(3, 1, 2)
                double scale_factor = 1.0f;
            #else
                double scale_factor = GetContentScaleFactor();
            #endif
            #else
                double scale_factor = 1.0f;
            #endif
				wxRect rcAffichageBitmap;

				rcAffichageBitmap.x = 0;
				rcAffichageBitmap.y = 0;
				rcAffichageBitmap.width = GetWindowWidth() / scale_factor;
				rcAffichageBitmap.height = GetWindowHeight()  / scale_factor;

				if(m_animationCtrl->IsShown())
				{
					wxAnimation animation = m_animationCtrl->GetAnimation();
					wxSize animationSize = animation.GetSize();
        
					int xPos = rcAffichageBitmap.x + (GetWindowWidth() / scale_factor - animationSize.GetWidth()) / 2;
					int yPos = (GetWindowHeight() / scale_factor - animationSize.GetHeight()) / 2;
        
					m_animationCtrl->SetSize(xPos, yPos, animationSize.GetWidth(), animationSize.GetHeight());
				}
			};

			void SetTexte(const wxString &libelle)
			{
				textToShow = libelle;
				Refresh();
			}
			

		private:

			void OnPaint(wxPaintEvent& event)
			{
            #ifdef __WXGTK__
            #if wxCHECK_VERSION(3, 1, 2)
                double scale_factor = 1.0f;
            #else
                double scale_factor = GetContentScaleFactor();
            #endif
            #else
                double scale_factor = 1.0f;
            #endif
				CThemeFont font;
				wxPaintDC dc(this);
				//dc.SetBackgroundMode(wxTRANSPARENT);
				//dc.SetBackground(*wxTRANSPARENT_BRUSH);
				wxRect rc = GetRect();
				this->FillRect(&dc, rc, wxColour("white"));
				font.SetColorFont(wxColour("black"));
                
				wxSize size = CWindowMain::GetSizeTexte(&dc, textToShow, font);
				int xPos = (GetWindowWidth() / scale_factor - size.x) / 2;

				wxAnimation animation = m_animationCtrl->GetAnimation();
				wxSize animationSize = animation.GetSize();
				int yPos = (GetWindowHeight() / scale_factor - animationSize.GetHeight()) / 2;
				yPos -= size.y * 2;
				CWindowMain::DrawTexte(&dc, textToShow, xPos, yPos, font);

			}

			wxAnimationCtrl * m_animationCtrl;
			wxString textToShow;
		};

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
			bool OpenFolder();
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

			wxString GetFilename();
            bool IsVideo();
            
		private:

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
            //void RefreshTimer(wxCommandEvent& event);
            void StopAnimation(wxCommandEvent& event);
            void SetScreenEvent(wxCommandEvent& event);
			void Resize();
			void LoadPicture(const bool &first = false);
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
            bool isInit = false;
		};
	}

}
