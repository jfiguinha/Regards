#pragma once
#include <Splitter.h>
#include <PanelWithClickToolbar.h>
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
		class CPreviewInfosWnd :public CSplitter
		{
		public:
			CPreviewInfosWnd(wxWindow* parent, wxWindowID idCTreeWithScrollbarInterface, const CThemeSplitter & theme, const bool &horizontal = true);
			~CPreviewInfosWnd();

			bool SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail, const bool &isAnimation);
			void SetVideo(const wxString &filename);

			void SetEffect(const bool &effect);
			void ShowInfos();

			void FullscreenMode();
			void ScreenMode();
			bool IsPanelInfosVisible();
			bool IsToolbarVisible();

			void ResizeWindow();
            
            void UpdateScreenRatio();
            void StartLoadingPicture();
			void StopLoadingPicture();

			void SetDiaporamaMode();
			void SetNormalMode();
            

		protected:

			void OnResize(wxCommandEvent& event);


			void SetPanelInfos(const bool &isThumbnail);

			void RedrawBarPos();

			CPanelWithClickToolbar * paneInfos;
			CPanelInfosWnd * panelInfosWindow;
			CPreviewWnd * previewWindow;
            
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