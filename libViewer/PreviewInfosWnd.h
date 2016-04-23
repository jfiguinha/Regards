#pragma once
#include <Splitter.h>
#include <Pane.h>
#include <ClickToolbar.h>
#include <ToolbarInterface.h>
#include <EffectVideoParameter.h>
#include "PanelInfosWnd.h"
#include "PreviewWnd.h"
#include <string>
#include <thread>
using namespace std;
using namespace Regards::Window;
using namespace Regards::Control;




namespace Regards
{
	namespace Viewer
	{
		class CPreviewInfosWnd :public CSplitter, public CPaneInterface, public CToolbarInterface
		{
		public:
			CPreviewInfosWnd(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeSplitter & theme, CVideoEffectParameter * videoEffectParameter, const bool &horizontal = true);
			~CPreviewInfosWnd();

			void ClosePane(const int &id);
            bool SetAnimation(const wxString &filename);
			bool SetBitmap(CRegardsBitmap * bitmap, const bool &isThumbnail);
			void SetVideo(const wxString &filename);

			void SetEffect(const bool &effect);
			void TransitionEnd();
			void ImageSuivante();
			void ImagePrecedente();

			void FullscreenMode();
			void ScreenMode();

			void ShowPanelInfos();
			void HidePanelInfos();
			bool IsPanelInfosVisible();

			void ShowToolbar();
			void HideToolbar();
			bool IsToolbarVisible();

			void ResizeWindow();
            
            void UpdateScreenRatio();
            void StartLoadingPicture();

			void SetDiaporamaMode();
			void SetNormalMode();

		protected:

			void ClickShowButton(const int &id);
			void SetPanelInfos(const bool &isThumbnail);
			wxRect GetWindowRect();

			void RedrawBarPos();

			CPane * paneInfos = nullptr;
			CPane * panePreview = nullptr;

			CPanelInfosWnd * panelInfosWindow = nullptr;
			CPreviewWnd * previewWindow = nullptr;

			CClickToolbar * clickInfoToolbar = nullptr;
            
            //Chargement des infos de géolocalisation
            CFileGeolocation * fileGeolocalisation;

			bool isVideo = false;
			bool isAnimation = false;
			int posBarInfos = 0;
			bool fullscreen;
			bool isDiapoarama = false;
            bool clickToobarShow = false;
            bool paneInfosShow = false;
			wxString filename;
			CRegardsBitmap * bitmap;
            bool isThumbnail = false;
		};

	}
}