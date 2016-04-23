#pragma once
#include <Splitter.h>
#include <EmptyWindow.h>
#include <EffectVideoParameter.h>
#include <Pane.h>
#include <ClickToolbar.h>
#include "PreviewWnd.h"
#include "ListPicture.h"
using namespace Regards::Control;
using namespace Regards::Window;

namespace Regards
{
	namespace Explorer
	{
		class CPreviewThumbnail :public CSplitter, public CPaneInterface, public CToolbarInterface
		{
		public:
			CPreviewThumbnail(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusbar, const CThemeSplitter & theme, CVideoEffectParameter * videoEffectParameter);
			~CPreviewThumbnail();

			void PhotoSelect(const wxString &photoPath);
			void ClosePane(const int &id);
			void HideToolbar();
			void ShowToolbar();
			void ClickShowButton(const int &id);
			void ShowPreview();
            void UpdateScreenRatio();
            
		private:

			CExplorerParam * explorerconfig = nullptr;
			CPane * paneListPicture = nullptr;
			CPane * panePreviewPicture = nullptr;
			CPane * paneEmpty = nullptr;
			CEmptyWindow * emptyWindow = nullptr;
			CPreviewWnd * previewImage = nullptr;
			CListPicture * listPicture = nullptr;
			IStatusBarInterface * statusBar = nullptr;
			CClickToolbar * clickPreview = nullptr;
			CVideoEffectParameter * videoEffectParameter = nullptr;
			int oldPos = 0;
		};
	}
}
