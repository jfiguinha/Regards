#pragma once
#include <FileGeolocation.h>
#include <RegardsBitmap.h>
#include "ToolbarInfos.h"
#include "ModificationManager.h"
#include <wx/webview.h>
#include <ScrollbarWnd.h>
#include <wx/animate.h>
#include <CriteriaTreeWnd.h>
#include <FilterWindowParam.h>
#include <InfosFileWnd.h>
#include "FiltreEffect.h"
#include "TabWindow.h"
using namespace std;
using namespace Regards::Window;
using namespace Regards::Control;
using namespace Regards::Internet;
/*
#define INFOS_WINDOW  1
#define HISTORY_WINDOW 2
#define WEB_WINDOW 3
#define EFFECT_WINDOW 4
#define SETTINGS_WINDOW 5
#define AUDIOVIDEO_WINDOW 6
#define WM_UPDATE_GPSINFOS 10
#define WM_UPDATE_DATETIMEINFOS 11
*/

class CImageLoadingFormat;
namespace Regards
{
	namespace Control
	{
		class CInfoEffectWnd;
		class CFiltreEffectScrollWnd;
		class CThumbnailViewerEffectWnd;
	}
	namespace Viewer
	{        
		class CPanelInfosWnd : public CTabWindow
		{
		public:
			CPanelInfosWnd(wxWindow* parent, wxWindowID idCTreeWithScrollbarInterface, CFileGeolocation * fileGeolocalisation);
			~CPanelInfosWnd();
			void OnFiltreOk(const int &numFiltre);
			void OnFiltreCancel();
			void SetBitmapFile(const wxString &filename, const bool &isThumbnail);
			void SetVideoFile(const wxString &filename);
			void SetAnimationFile(const wxString &filename);
            void ShowFiltre(const wxString &title);
            void UpdateData();
            CFiltreEffect * GetFilterWindow(int &numFiltre);
			wxString GetFilename();

		protected:

			void ApplyEffect(wxCommandEvent& event);
			void ShowFiltreEvent(wxCommandEvent& event);

			wxString MapsUpdate();
			void EffectUpdate();
			void HistoryUpdate();
			void VideoEffectUpdate();
			void InfosUpdate();
			void LoadInfo();
			void AudioVideoUpdate();
			void DisplayURL(const wxString &url);
            
			CInfosFileWnd * infosFileWnd;
            CInfoEffectWnd * historyEffectWnd;
			CThumbnailViewerEffectWnd * thumbnailEffectWnd;
			CFiltreEffectScrollWnd * filtreEffectWnd;
            CCriteriaTreeWnd * criteriaTreeWnd;

			wxWebView * webBrowser = nullptr;
			CToolbarInfos * infosToolbar;
			CModificationManager * modificationManager;

            bool isThumbnail;
			bool isVideo;
			wxString filename;
            wxString url;
			
			CFileGeolocation * fileGeolocalisation;			
		};

	}
}