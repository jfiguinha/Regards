#pragma once
#include <WindowMain.h>
#include <ScrollbarWnd.h>
#include <PaneInterface.h>
#include <ClickToolbar.h>
#include <ToolbarInterface.h>
#include <Photos.h>
#include <Pane.h>

#include "ThumbnailViewerVideo.h"
#include "EffectVideoParameter.h"
#include "PreviewThumbnailSplitter.h"
#include "ViewerParam.h"

using namespace Regards::Window;
using namespace Regards::Control;

namespace Regards
{
	namespace Viewer
	{

		class CThumbnailVideoPanel : public CWindowMain, public CPaneInterface, public CToolbarInterface
		{
		public:
			CThumbnailVideoPanel(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface);
			virtual ~CThumbnailVideoPanel();

			void SetVideo(const wxString &path);
			void SetAnimation(const wxString &path, vector<CImageVideoThumbnail *> * pictureThumbnail);
			void ShowVideoThumbnail(vector<CImageVideoThumbnail *> * pictureThumbnail = nullptr);
			void HideVideoThumbnail(const bool &refresh = true);

			bool IsPanelVideoThumbnailVisible();

			void FullscreenMode();
			void ScreenMode();
			
			void SetVideoPosition(const int64_t &videoTime);
			void UpdateScreenRatio();

			int GetHeight();


		protected:

			void ClickShowButton(const int &id);
			void ClosePane(const int &id);
			void Resize();
			void StartLoadingPicture();
			void StopLoadingPicture(wxCommandEvent& event);

		private:

			bool checkValidity;
			CPane * paneThumbnailVideo;
			CScrollbarWnd * scrollThumbnailVideo;
			CThumbnailViewerVideo * thumbnailVideo;
			CClickToolbar * clickThumbnailVideo;
			wxAnimationCtrl * m_animationCtrl;
			bool isPanelVisible;
			wxString filename;
		};


	}
}
