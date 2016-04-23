#pragma once
#include <ToolbarInterface.h>
#include <FileGeolocation.h>
#include <RegardsBitmap.h>
#include "FiltreEffect.h"
#include "InfoEffect.h"
#include "ToolbarInfos.h"
#include "ThumbnailViewerEffect.h"
#include "ModificationManager.h"
#include <InfosFile.h>
#include <wx/webview.h>
#include <ScrollbarWnd.h>
#include <EffectVideoWnd.h>
#include <string>
#include <thread>   
#include <mutex>
#include <wx/animate.h>
using namespace std;
using namespace Regards::Window;
using namespace Regards::Internet;
#define INFOS_WINDOW  1
#define HISTORY_WINDOW 2
#define WEB_WINDOW 3
#define EFFECT_WINDOW 4
#define SETTINGS_WINDOW 5
#define WM_UPDATE_GPSINFOS 10
#define WM_UPDATE_DATETIMEINFOS 11

namespace Regards
{
	namespace Viewer
	{
        class CPanelInfosWnd;
        
        class CThreadLoadInfos
        {
        public:
            CPanelInfosWnd * panelInfos = nullptr;
            CInfosFile * infosFileWnd = nullptr;
            wxString filename;
            std::thread * threadLoadInfos = nullptr;
        };
        
        class CThreadLoadThumbnailEffect
        {
        public:
            CPanelInfosWnd * panelInfos = nullptr;
            CInfosFile * infosFileWnd = nullptr;
            wxString filename;
            std::thread * threadLoadInfos = nullptr;
        };
        
		class CPanelInfosWnd : public CWindowMain, public CToolbarInterface
		{
		public:
			CPanelInfosWnd(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, CVideoEffectParameter * videoEffectParameter, CFileGeolocation * fileGeolocalisation);
			~CPanelInfosWnd();
			void ShowInfos();
			void ShowHistory();
			void ShowEffect();
			void ShowMap();
			void ShowSettings();
			void OnFiltreOk(const int &numFiltre);
			void OnFiltreCancel();
			void SetBitmapFile(const wxString &filename, CRegardsBitmap * bitmap, const bool &isThumbnail);
			void SetVideoFile(const wxString &filename);
			void SetAnimationFile(const wxString &filename);
			void ApplyEffect(const int &numItem);
			wxString GetFilename();
            void UpdateScreenRatio();
            
            
            void StartLoadingPicture(wxWindow * window);
            void StopLoadingPicture(wxWindow * window);
            
		protected:
            
            static void GenerateTreeInfos(CThreadLoadInfos * threadInfos);            
            void UpdateTreeInfosEvent(wxCommandEvent &event);


			wxString MapsUpdate();
			void EffectUpdate();
			void HistoryUpdate();
			void InfosUpdate();
			void LoadInfo();

			void DisplayURL(const wxString &url);

			void ClickShowButton(const int &id);

			void OnSize(wxSizeEvent& event);
            void OnPaint(wxPaintEvent& event);

			void Resize();

			void ShowFiltre(const wxString &title);
			void HideAllWindow();
            
            wxAnimationCtrl * m_animationCtrl = nullptr;

			CScrollbarWnd * InfosFileScroll = nullptr;
			CTreeWindow * treeInfos = nullptr;
			CInfosFile * infosFileWndOld = nullptr;

#ifdef EFFECT_VIDEO
			CScrollbarWnd * effectVideoWndScroll = nullptr;
			CTreeWindow * treeEffectVideo = nullptr;
			CEffectVideoWnd * effectVideoWndOld = nullptr;
            
            bool openGLVideoMode = false;
#endif
			CScrollbarWnd * historyEffectScroll = nullptr;
			CTreeWindow * treeHistoryEffect = nullptr;
			CInfoEffect * historyEffectOld = nullptr;

			CScrollbarWnd * thumbnailEffectScroll = nullptr;
			CThumbnailViewerEffect * thumbnailEffect = nullptr;

			CScrollbarWnd * filtreEffectScroll = nullptr;
			CTreeWindow * treeFiltreEffect = nullptr;
			CFiltreEffect * filtreEffectOld = nullptr;

			wxWebView * webBrowser = nullptr;
			CToolbarInfos * infosToolbar = nullptr;

			//Effect Parameter
			CEffectParameter * effectParameter = nullptr;
			CVideoEffectParameter * videoEffectParameter = nullptr;
			CModificationManager * modificationManager = nullptr;

            bool isThumbnail = false;
			bool isVideo = false;
			wxString filename = L"";
			CRegardsBitmap * bitmap = new CRegardsBitmap();
			int width = 0;
			int height = 0;
            wxString url = "http://www.google.fr";
			
			CFileGeolocation * fileGeolocalisation;
			int windowVisible = INFOS_WINDOW;
            CThemeBitmapWindow themeBitmap;
		};

	}
}