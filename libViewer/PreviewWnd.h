#pragma once

#include "FiltreToolbar.h"
#include "PreviewToolbar.h"
#include <ShowBitmap.h>
#include <ShowVideo.h>
#include <RegardsBitmap.h>
#include <EffectVideoParameter.h>
#include <BitmapInterface.h>
#include <ToolbarInterface.h>
#include <BitmapInfos.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <string>
#include <wx/animate.h>
using namespace std;
using namespace Regards::Window;
using namespace Regards::Control;
using namespace Regards::Video;



namespace Regards
{
	namespace Viewer
	{
		class CPreviewWnd : public CWindowMain, public CBitmapInterface, public CToolbarInterface
		{
		public:
			CPreviewWnd(wxWindow* parent, wxWindowID id, CVideoEffectParameter * videoEffectParameter, IStatusBarInterface * statusBarInterface, CFileGeolocation * fileGeolocalisation, const bool &horizontal = true);
			~CPreviewWnd();
			bool SetBitmap(CRegardsBitmap * bitmap, const bool &isThumbnail);
			bool SetVideo(const wxString &filename);
            bool SetAnimation(const wxString &filename);
			void SetEffect(const bool &effect);
			void TransitionEnd();
			void ImageSuivante();
			void ImagePrecedente();
			void Resize();
			void SetVideoPosition(const int &videoTime);
            void UpdateScreenRatio();
			void ShowToolbar();
			void HideToolbar();
			bool IsToolbarVisible();

			void ShowValidationToolbar(const bool &visible, const int &filtre);
			void HideValidationToolbar();

			void ClickShowButton(const int &id);
			void SetFullscreen(const bool &fullscreen);

			void SetDiaporamaMode();
			void SetNormalMode();
            
            void StartLoadingPicture();

            void UpdateInfos();
            
		protected:

            void OnPaint(wxPaintEvent& event);
            
            wxAnimationCtrl * m_animationCtrl = nullptr;
			CShowBitmap * showBitmapWindow = nullptr;
			CShowVideo * showVideoWindow = nullptr;

			CPreviewToolbar * previewToolbar = nullptr;
			CBitmapInfos * bitmapInfos = nullptr;
			CVideoEffectParameter * videoEffectParameter = nullptr;
			CFiltreToolbar * filtreToolbar = nullptr;

            bool isBitmap = false;
            bool isAnimation = false;
			bool isVideo = false;
			bool isEffect = false;
            bool showToolbar = true;
			bool fullscreen = false;
			bool isDiaporama = false;
            wxString oldfilename;
			CThemeBitmapWindow themeBitmap;
		};
	}
}
