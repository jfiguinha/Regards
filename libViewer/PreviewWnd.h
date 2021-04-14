#pragma once
#include <WindowMain.h>
#include <BitmapInterface.h>
#include <ToolbarInterface.h>
;

class CRegardsBitmap;
class CImageLoadingFormat;

namespace Regards
{
	namespace Window
	{
		class CBitmapInfos;
	}
	using namespace Regards::Window;
    
	namespace Video
	{
		class CShowVideo;
	}
	using namespace Regards::Video;

	namespace Control
	{
		class CShowBitmap;
		class CFiltreToolbar;
	}
	using namespace Regards::Control;


	namespace Viewer
	{
		class CPreviewToolbar;
		class CAnimationToolbar;

		class CPreviewWnd : public CWindowMain, public CBitmapInterface, public Regards::Window::CToolbarInterface
		{
		public:
			CPreviewWnd(wxWindow* parent, wxWindowID id, const bool &horizontal = true);
			~CPreviewWnd();
			bool SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail, const bool &isAnimation = false);
			bool SetVideo(const wxString &filename, const bool &play = false);
			void SetEffect(const bool &effect);
			void StopVideo();
			void TransitionEnd();
			void ImageSuivante();
			void ImagePrecedente();
			void Resize();
			//void SetVideoPosition(const int &videoTime);
            void UpdateScreenRatio();

			bool IsToolbarVisible();

			void ShowValidationToolbar(const bool &visible, const int &filtre);
			void HideValidationToolbar();

			void ClickShowButton(const int &id, const int &refresh = 1);
			void SetFullscreen(const bool &fullscreen);

			void SetDiaporamaMode();
			void SetNormalMode();
			void SetFaceMode();

            void UpdateInfos();

            
		protected:

			void HideSaveButton(wxCommandEvent& event);
			void ShowSaveButton(wxCommandEvent& event);
			void OnFiltreOK(wxCommandEvent& event);
			void OnFiltreCancel(wxCommandEvent& event);
			void ShowToolbar(wxCommandEvent& event);
			void HideToolbar(wxCommandEvent& event);
			void OnShowToolbar(wxCommandEvent& event);
			void StartAnimation(wxCommandEvent& event);
            
			CShowBitmap * showBitmapWindow;
			CShowVideo * showVideoWindow;

			CAnimationToolbar * animationToolbar;
			CPreviewToolbar * previewToolbar;
			CBitmapInfos * bitmapInfos;
			CFiltreToolbar * filtreToolbar;

			bool isAnimation;
            bool isBitmap;
			bool isVideo;
			bool isEffect;
            bool showToolbar;
			bool fullscreen;
			bool isDiaporama;
			int isOldState = 0;
            wxString oldfilename;
			CThemeBitmapWindow themeBitmap;

		};
	}
}
