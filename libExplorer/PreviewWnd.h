#pragma once
#include <Splitter.h>
#include <ToolbarWindow.h>
#include <EffectVideoParameter.h>
#include <Pane.h>
#include <string>
#include <ClickToolbar.h>
#include "PanelInfosWnd.h"
#include "PanelPreview.h"
#include "ExplorerParam.h"
using namespace std;
using namespace Regards::Control;
using namespace Regards::Window;

namespace Regards
{
	namespace Explorer
	{
		class CPreviewWnd :public CSplitter, public CPaneInterface, public CToolbarInterface
		{
		public:

			CPreviewWnd(wxWindow* parent, wxWindowID id, const CThemeSplitter & theme, CVideoEffectParameter * videoEffectParameter, IStatusBarInterface * statusBarInterface, const bool &horizontal = true);
			~CPreviewWnd();

            void SetAnimation(const wxString &filename);
			void SetPhoto(const wxString &filename);
			void SetVideo(const wxString &filename);
			void ClosePane(const int &id);
			void TransitionEnd();
			void ImageSuivante();
			void ImagePrecedente();
			void ShowToolbar();
			void HideToolbar();
			void ClickShowButton(const int &id);
			void ShowInfos();
            void UpdateScreenRatio();
            
		protected:

            //void OnIdle(wxIdleEvent& evt);
            
			CPane * paneInfos = nullptr;
			CPane * panePreview = nullptr;
			CClickToolbar * clickPanelInfos = nullptr;
			CPanelInfosWnd * panelInfos = nullptr;
			CPanelPreview * panelPreview = nullptr;
			CExplorerParam * explorerconfig = nullptr;
            CFileGeolocation * fileGeolocalisation;

			int oldPos = 0;
			wxString filename = L"";

		};
	}
}
