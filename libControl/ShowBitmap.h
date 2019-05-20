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
		class CShowBitmap : public wxWindow
		{
		public:

			CShowBitmap(wxWindow* parent, wxWindowID id, wxWindowID bitmapViewerId, wxWindowID mainViewerId, CBitmapInterface * bitmapInterfaceCTreeWithScrollbarInterface, CThemeParam * config);
			~CShowBitmap();
			bool SetPhoto(const wxString &filename);
			CRegardsBitmap * GetBitmap(const bool &source);
			int GetBitmapWidth();
			int GetBitmapHeight();
			void DeplacementDroite();
			void DeplacementGauche();
			void DeplacementHaut();
			void DeplacementBas();
			void FlipVertical();
			void Rotate90();
			void Rotate270();
			void FlipHorizontal();
			void ZoomOn();
			void ZoomOut();
			void StopTransition();
			bool SetBitmap(CImageLoadingFormat * bitmap, const bool & isThumbnail);
			bool GetShrinkImage();
			void SetShrinkImage(const bool &value);
			void ShrinkImage();
			void PrintPicture();
			void ExecuteRegardsShop();
			void SendEmail();
			void HideToolbar();
			void ShowToolbar();
			bool IsToolbarMouseOver();
			void TransitionEnd();
			void SetBitmapPreviewEffect(const int &effect);
			//void ShowCropButton();
			//void HideCropButton();
			void SetFullscreen(const bool &fullscreen);

			void SetTheme(CThemeBitmapWindow * theme);
			CBitmapWndViewer * GetBitmapViewer();

            void UpdateScreenRatio();
			void SetDiaporamaMode();
			void SetNormalMode();

		private:

			void StartLoadingPicture(wxCommandEvent& event);
			void StopLoadingPicture(wxCommandEvent& event);
			void OnSize(wxSizeEvent& event);
			void OnIdle(wxIdleEvent& evt);
            void OnViewerDblClick(wxCommandEvent& event);
            void OnViewerZoomIn(wxCommandEvent& event);
            void OnViewerZoomOut(wxCommandEvent& event);
			void Resize();
            void OnTimerRefresh(wxTimerEvent& event);
            
            wxGauge * progressBar;
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
			int width;
			bool transitionEnd;
			int height;
            wxTimer * loadingTimer;
            int progressValue;
			
		};
	}
}
