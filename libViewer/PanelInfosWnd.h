#pragma once
#include <ToolbarInterface.h>
#include <FileGeolocation.h>
#include <RegardsBitmap.h>
#include "ToolbarInfos.h"
#include "ModificationManager.h"
#include <wx/webview.h>
#include <ScrollbarWnd.h>
#include <StatusBarInterface.h>
#include <wx/animate.h>
#include <CriteriaTreeWnd.h>
#include <FilterWindowParam.h>
#include <InfosFileWnd.h>
#include "FiltreEffect.h"
using namespace std;
using namespace Regards::Window;
using namespace Regards::Control;
using namespace Regards::Internet;
#define INFOS_WINDOW  1
#define HISTORY_WINDOW 2
#define WEB_WINDOW 3
#define EFFECT_WINDOW 4
#define SETTINGS_WINDOW 5
#define AUDIOVIDEO_WINDOW 6
#define WM_UPDATE_GPSINFOS 10
#define WM_UPDATE_DATETIMEINFOS 11


class CImageLoadingFormat;
namespace Regards
{
	namespace Viewer
	{
        class CInfoEffectWnd;
        class CFiltreEffectScrollWnd;
        class CThumbnailViewerEffectWnd;
        
		class CPanelInfosWnd : public CWindowMain, public CToolbarInterface
		{
		public:
			CPanelInfosWnd(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, CFileGeolocation * fileGeolocalisation);
			~CPanelInfosWnd();
			void ShowInfos();
			void ShowHistory();
			void ShowEffect();
			void ShowMap();
			void ShowAudioVideo();
			void ShowVideoEffect();
			void ShowSettings();
			void OnFiltreOk(const int &numFiltre);
			void OnFiltreCancel();
			void SetBitmapFile(const wxString &filename, const bool &isThumbnail);
			void SetVideoFile(const wxString &filename);
			void SetAnimationFile(const wxString &filename);
			void ApplyEffect(const int &numItem);
			wxString GetFilename();
            void UpdateScreenRatio();
            void ShowFiltre(const wxString &title);
            void ShowCriteria();
            void UpdateData();
            void StartLoadingPicture(wxWindow * window);
            void StopLoadingPicture(wxWindow * window);
            CFiltreEffect * GetFilterWindow(int &numFiltre);

		protected:
            
			wxString MapsUpdate();
			void EffectUpdate();
			void HistoryUpdate();
			void VideoEffectUpdate();
			void InfosUpdate();
			void LoadInfo();
			void AudioVideoUpdate();
			void DisplayURL(const wxString &url);

			void ClickShowButton(const int &id);

			void OnSize(wxSizeEvent& event);
            void OnPaint(wxPaintEvent& event);

			void Resize();

			
			void HideAllWindow();
            
            wxAnimationCtrl * m_animationCtrl;

			CInfosFileWnd * infosFileWnd;
          
            CInfoEffectWnd * historyEffectWnd;
			CThumbnailViewerEffectWnd * thumbnailEffectWnd;
			CFiltreEffectScrollWnd * filtreEffectWnd;
            CCriteriaTreeWnd * criteriaTreeWnd;

			wxWebView * webBrowser;
			CToolbarInfos * infosToolbar;
			CModificationManager * modificationManager;

            bool isThumbnail;
			bool isVideo;
			wxString filename;
			//CRegardsBitmap * bitmap;
			int width;
			int height;
            wxString url;
			
			CFileGeolocation * fileGeolocalisation;
			int windowVisible;
            CThemeBitmapWindow themeBitmap;
			
		};

	}
}