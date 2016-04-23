#pragma once
#include <Pane.h>
#include <ShowVideo.h>
#include <ShowBitmap.h>
#include <ToolbarInterface.h>
#include <BitmapInfos.h>
#include <BitmapInterface.h>
#include <EffectVideoParameter.h>
#include "PreviewToolbar.h"
#include "MainWindow.h"
#include <WindowMain.h>
#include <thread>
#include <mutex>
#include <string>
#include <wx/animate.h>
using namespace std;
using namespace Regards::Window;
using namespace Regards::Control;
using namespace Regards::Video;

namespace Regards
{
	namespace Explorer
	{
		class CPanelPreview;

		class CThreadPictureData
		{
		public:

			CThreadPictureData(){};

			~CThreadPictureData(){};

			CPanelPreview * panelPreview = nullptr;
			wxString picture;
			bool isThumbnail = false;
			bool isVisible = false;
            thread * threadPicture = nullptr;
            CRegardsBitmap * bitmap = nullptr;
		};

		class CPanelPreview : public CWindowMain, public CBitmapInterface, public CToolbarInterface
		{
		public:
			CPanelPreview(wxWindow* parent, wxWindowID id, CVideoEffectParameter * videoEffectParameter, IStatusBarInterface * statusBarInterface, CFileGeolocation * fileGeolocalisation, const bool &horizontal = true);
			~CPanelPreview();

            void UpdateScreenRatio();
			bool SetPhoto(const wxString &filename);
			bool SetVideo(const wxString &filename);
            bool SetAnimation(const wxString &filename);
			void TransitionEnd();
			void ImageSuivante();
			void ImagePrecedente();
			void Resize();
			void SetVideoPosition(const int &videoTime);

			void ShowToolbar();
			void HideToolbar();
			bool IsToolbarVisible();

			void ClickShowButton(const int &id);
			void ShowToolbar(const bool &visible);

		protected:
            void DisplayPicture(wxCommandEvent& event);
            
            static void LoadPicture(CThreadPictureData * pictureData);

			void ImageDebut();
			void ImageFin();

			void OnPaint(wxPaintEvent& event);

            wxAnimationCtrl * m_animationCtrl = nullptr;
			CShowBitmap * showBitmapWindow = nullptr;
			CShowVideo * showVideoWindow = nullptr;
			CPreviewToolbar * previewToolbar = nullptr;
			CBitmapInfos * bitmapInfos = nullptr;
			CVideoEffectParameter * videoEffectParameter = nullptr;
            CThemeBitmapWindow themeBitmap;
            
            bool showToolbar = true;
			bool isVideo = false;
            bool isAnimation = false;
            bool isBitmap = true;
            
			wxString filenameToShow = L"";
			CRegardsConfigParam * config = nullptr;
			//bool needToRedraw = false;
		};
	}
}
