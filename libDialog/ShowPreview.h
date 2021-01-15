#pragma once
#include <ThemeParam.h>
#include "BitmapInterface.h"
#include "PreviewToolbar.h"
#include "SliderVideoPreview.h"
#include "ScrollbarWnd.h"
#include "BitmapWndViewer.h"
#include <wx/gauge.h>
using namespace Regards::OpenCL;
using namespace Regards::Window;

class CRegardsBitmap;
class CFFmpegDecodeFrameFilter;

namespace Regards
{
	namespace Control
	{
		class CShowPreview : public CWindowMain, public CSliderInterface
		{
		public:

			CShowPreview(wxWindow* parent, wxWindowID id, wxWindowID bitmapViewerId, wxWindowID mainViewerId, CThemeParam * config, const wxString &videoFilename, COpenCLEngine * openCLEngine, CVideoEffectParameter * videoEffectParameter);
			~CShowPreview();

			//bool SetBitmap(CImageLoadingFormat* bitmap, const bool& isThumbnail);
			//CRegardsBitmap* GetBitmap(const bool& source);
			void UpdateScreenRatio();
			void SlidePosChange(const int &position, const wxString &key) {};
			void ZoomPos(const int &position) {};
			void MoveSlider(const int64_t &position);
			void ClickButton(const int &id) {};
			void SetTrackBarPosition(const int &iPos) {};
			void UpdateBitmap();
		private:
			bool SetBitmap(CImageLoadingFormat * bitmap);
			void OnViewerZoomIn(wxCommandEvent& event);
			void OnViewerZoomOut(wxCommandEvent& event);
			void Resize();
			void OnControlSize(wxCommandEvent& event);
			void OnSetPosition(wxCommandEvent& event);
			void OnMoveLeft(wxCommandEvent& event);
			void OnMoveRight(wxCommandEvent& event);
			void OnMoveTop(wxCommandEvent& event);
			void OnMoveBottom(wxCommandEvent& event);

			CScrollbarWnd * scrollbar;
			CPreviewToolbar * previewToolbar;
			CSliderVideoPreview * sliderVideo;
			CBitmapWndViewer * bitmapWindow;
			CBitmapInterface * bitmapInterface;
			CRegardsConfigParam * configRegards;
			CImageLoadingFormat * tempImage;
			CVideoEffectParameter * videoEffectParameter;
			bool defaultToolbar;
			bool defaultViewer;
			bool bitmapWndLocal;
			CFFmpegDecodeFrameFilter * ffmpegTranscoding;
			bool transitionEnd;
			wxString filename;
			int progressValue;
			double timeTotal;
			int position = 0;
		};
	}
}
