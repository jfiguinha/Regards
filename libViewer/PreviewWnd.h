#pragma once

#include "FiltreToolbar.h"
#include "PreviewToolbar.h"
#include "AnimationToolbar.h"
#include <ShowBitmap.h>
#include <EffectVideoParameter.h>
#include <BitmapInterface.h>
#include <ToolbarInterface.h>
#include <BitmapInfos.h>
#include <wx/animate.h>
using namespace std;
using namespace Regards::Window;
using namespace Regards::Control;
#ifndef FFMPEG
#include <ShowVideo.h>
using namespace Regards::Video;
#else
class CShowVideo;
#endif

class CRegardsBitmap;

namespace Regards
{
	namespace Viewer
	{
		class CPreviewWnd : public CWindowMain, public CBitmapInterface, public CToolbarInterface
		{
		public:
			CPreviewWnd(wxWindow* parent, wxWindowID id, CFileGeolocation * fileGeolocalisation, const bool &horizontal = true);
			~CPreviewWnd();
			bool SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail, const bool &isAnimation = false);
			bool SetVideo(const wxString &filename);
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

			void ClickShowButton(const int &id);
			void SetFullscreen(const bool &fullscreen);

			void SetDiaporamaMode();
			void SetNormalMode();

            void UpdateInfos();
            
		protected:


			void OnFiltreOK(wxCommandEvent& event);
			void OnFiltreCancel(wxCommandEvent& event);
			void ShowToolbar(wxCommandEvent& event);
			void HideToolbar(wxCommandEvent& event);
			void OnShowToolbar(wxCommandEvent& event);

            
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
            wxString oldfilename;
			CThemeBitmapWindow themeBitmap;

		};
	}
}
