#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <ThemeParam.h>
#include "BitmapInterface.h"
#include "BitmapToolbar.h"
#include "ScrollbarWnd.h"
#include "BitmapWndViewer.h"
#include <StatusBarInterface.h>
using namespace Regards::Window;

class CRegardsBitmap;

namespace Regards
{
	namespace Control
	{
		class CShowBitmap : public wxWindow
		{
		public:

			CShowBitmap(wxWindow* parent, wxWindowID id, wxWindowID bitmapViewerId, CWindowMain * main, CBitmapInterface * bitmapInterface, IStatusBarInterface * statusBarInterface, CThemeParam * config);
			~CShowBitmap();
			bool SetPhoto(const wxString &filename);
			CRegardsBitmap * GetBitmap();
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
			bool SetBitmap(CRegardsBitmap * bitmap);
			bool GetShrinkImage();
			void SetShrinkImage(const bool &value);
			void ShrinkImage();
			void PrintPicture();
			void ExecuteRegardsShop();
			void SendEmail();
			void HideToolbar();
			void ShowToolbar();
			bool IsToolbarMouseOver();
			void SetBitmapPreviewEffect(const int &effect);
			//void ShowCropButton();
			//void HideCropButton();
			void SetFullscreen(const bool &fullscreen);
			bool IsOpenGLCompatible();
			void ChangerRenderInterface();
			void SetTheme(CThemeBitmapWindow * theme);
			CBitmapWndViewer * GetBitmapViewer();
            void StartLoadingPicture();
            void StopLoadingPicture();
            void UpdateScreenRatio();
            
			void SetDiaporamaMode();
			void SetNormalMode();

		private:

			void OnSize(wxSizeEvent& event);
            void OnViewerDblClick(wxCommandEvent& event);
			void Resize();

			CScrollbarWnd * scrollbar = nullptr;
			CBitmapToolbar * pictureToolbar = nullptr;
			CBitmapWndViewer * bitmapWindow = nullptr;
			CBitmapInterface * bitmapInterface = nullptr;
			CWindowMain * main = nullptr;
			CRegardsConfigParam * configRegards = nullptr;
			bool defaultToolbar = true;
			bool defaultViewer = true;
			bool bitmapWndLocal;
			bool isDiaporama = false;
			int width = 0;
			int height = 0;
			
		};
	}
}
