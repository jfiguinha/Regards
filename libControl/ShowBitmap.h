#pragma once
#include <ThemeParam.h>
#include "BitmapInterface.h"
#include "BitmapToolbar.h"
#include "ScrollbarWnd.h"
#include "BitmapWndViewer.h"
#include <OpenCLContext.h>
#include <wx/gauge.h>
using namespace Regards::OpenCL;
using namespace Regards::Window;

class CRegardsBitmap;

namespace Regards
{
	namespace Control
	{
		class CShowBitmap : public CWindowMain
		{
		public:

			CShowBitmap(wxWindow* parent, wxWindowID id, wxWindowID bitmapViewerId, wxWindowID mainViewerId, CBitmapInterface * bitmapInterfaceCTreeWithScrollbarInterface, CThemeParam * config);
			~CShowBitmap();

			void HideToolbar();
			void ShowToolbar();
			void FlipVertical();
			void Rotate90();
			void Rotate270();
			void FlipHorizontal();
			bool SetBitmap(CImageLoadingFormat* bitmap, const bool& isThumbnail);
			bool IsToolbarMouseOver();
			void SetBitmapPreviewEffect(const int& effect);
			void SetFullscreen(const bool& fullscreen);
			void StopTransition();
			void TransitionEnd();
			CRegardsBitmap* GetBitmap(const bool& source);

			void UpdateScreenRatio();
			void SetDiaporamaMode();
			void SetNormalMode();

		private:

			void OnIdle(wxIdleEvent& evt);
            void OnViewerDblClick(wxCommandEvent& event);
            void OnViewerZoomIn(wxCommandEvent& event);
            void OnViewerZoomOut(wxCommandEvent& event);
			void Resize();
            void OnTimerRefresh(wxTimerEvent& event);
            
			void OnControlSize(wxCommandEvent& event);
			void OnSetPosition(wxCommandEvent& event);

			void OnMoveLeft(wxCommandEvent& event);
			void OnMoveRight(wxCommandEvent& event);
			void OnMoveTop(wxCommandEvent& event);
			void OnMoveBottom(wxCommandEvent& event);
			CScrollbarWnd * scrollbar;
			CBitmapToolbar * pictureToolbar;
			CBitmapWndViewer * bitmapWindow;
			CBitmapInterface * bitmapInterface;
			CRegardsConfigParam * configRegards;
			CImageLoadingFormat * tempImage;
			bool defaultToolbar;
			bool defaultViewer;
			bool bitmapWndLocal;
			bool isDiaporama;

			bool transitionEnd;

            int progressValue;
			bool fullscreen;
			bool showToolbar;
		};
	}
}
