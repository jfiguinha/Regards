#pragma once
#include <ToolbarInterface.h>
#include <Photos.h>
#include <WindowMain.h>
#include <PanelWithClickToolbar.h>
#include "PreviewInfosWnd.h"
#include "ToolbarPDF.h"
#include "ThumbnailMultiPage.h"
#include "ThumbnailViewerPicture.h"
#include "EffectVideoParameter.h"
#include "ViewerParam.h"
#include "ImageList.h"
using namespace std;
using namespace Regards::Window;
using namespace Regards::Control;
class CImageLoadingFormat;
namespace Regards
{
	namespace Viewer
	{
		class CViewerPDF : public CWindowMain, public CPaneInterface, public CBitmapInterface
		{
		public:
			CViewerPDF(wxWindow* parent, wxWindowID idCTreeWithScrollbarInterface);
			~CViewerPDF();

			bool IsPanelInfosVisible();

			void ShowToolbar();
			void HideToolbar();
			
			void FullscreenMode();
			void ScreenMode();

			void AnimationPictureNext();
			void AnimationPicturePrevious();

			void UpdateScreenRatio();
			void SetPosition(const long& timePosition);

			void LoadFile(const wxString &filename);
			void SetImage(const wxImage &imageFile);

		private:
			
			bool GetProcessEnd();

			void ClosePane(const int& id) {};
			void RefreshPane(const int& id) {};
		
			void ShowPanelVideoThumbnail();
			void HidePanel();
			void Resize();

			void OnResize(wxCommandEvent& event);
			void LoadAnimationBitmap(const wxString &filename, const int &numFrame);
			void OnSize(wxSizeEvent& event);
			void AnimationSetPosition(wxCommandEvent& event);

			void RedrawBarPos();
			wxRect GetWindowRect();
			int nbThumbnail;

			//Thumbnail Video
			CPanelWithClickToolbar * panelVideo;
			CScrollbarWnd * scrollVideoWindow;
			CThumbnailMultiPage * thumbnailVideo;

			//Toolbar
			CToolbarPDF * toolbarPDF;

			//CPreviewThumbnailSplitter * previewThumbnailSplitter;
			CShowBitmap * showBitmapWindow;

			CViewerParam * viewerParam;
			bool checkValidity;
			bool isFullscreen;
			wxString filename;
			int oldAnimationPosition = -1;
			wxString oldFilename = L"";
			int width;
			int height;

			bool showToolbar;
			int animationPosition;
			vector<CImageVideoThumbnail *> pageThumbnail;
		};


	}
}
