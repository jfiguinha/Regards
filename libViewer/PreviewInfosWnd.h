#pragma once
#include <Splitter.h>
#include <Pane.h>
#include <ClickToolbar.h>
#include <ToolbarInterface.h>
#include <EffectVideoParameter.h>
#include "PanelInfosWnd.h"
#include "PreviewWnd.h"
using namespace Regards::Window;
using namespace Regards::Control;

class CImageLoadingFormat;


namespace Regards
{
	namespace Viewer
	{
		class CPreviewInfosWnd :public CSplitter, public CPaneInterface, public CToolbarInterface
		{
		public:
			CPreviewInfosWnd(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeSplitter & theme, const bool &horizontal = true);
			~CPreviewInfosWnd();

			void ClosePane(const int &id);
			bool SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail, const bool &isAnimation);
			void SetVideo(const wxString &filename);

			void SetEffect(const bool &effect);
			void TransitionEnd();
			void ImageSuivante();
			void ImagePrecedente();

			void FullscreenMode();
			void ScreenMode();

			bool IsPanelInfosVisible();
			bool IsToolbarVisible();

			void ResizeWindow();
            
            void UpdateScreenRatio();
            void StartLoadingPicture();

			void SetDiaporamaMode();
			void SetNormalMode();
            
           void HidePanel(); 

		protected:

			void ShowPanelInfos(wxCommandEvent& event);
			void HidePanelInfos(wxCommandEvent& event);
			void ClickShowButton(const int &id);
			void SetPanelInfos(const bool &isThumbnail);

			void RedrawBarPos();

			CPane * paneInfos;
			CPanelInfosWnd * panelInfosWindow;
			CPreviewWnd * previewWindow;
			CClickToolbar * clickInfoToolbar;
            
            //Chargement des infos de géolocalisation
            CFileGeolocation * fileGeolocalisation;

			bool isVideo;
			int posBarInfos;
			bool fullscreen;
			bool isDiapoarama;
            bool clickToobarShow;
            bool paneInfosShow;
			wxString filename;
            bool isThumbnail;
		};

	}
}