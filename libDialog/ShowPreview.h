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
#ifdef NOTENCODE_FRAME
class CFFmpegDecodeFrameFilter;
#else
#include <VideoCompressOption.h>
class CFFmpegTranscodingPimpl;
class CFFmpegDecodeFrame;
#endif

namespace Regards
{
	namespace Control
	{
		class CShowPreview : public CWindowMain, public CSliderInterface
		{
		public:

#ifdef NOTENCODE_FRAME
			CShowPreview(wxWindow* parent, wxWindowID id, wxWindowID bitmapViewerId, wxWindowID mainViewerId, CThemeParam * config, const wxString &videoFilename, COpenCLEngine * openCLEngine, CVideoEffectParameter * videoEffectParameter);
#else
			CShowPreview(wxWindow* parent, wxWindowID id, wxWindowID bitmapViewerId, wxWindowID mainViewerId, CThemeParam * config, const wxString &videoFilename, COpenCLEngine * openCLEngine, CVideoOptionCompress * videoOptionCompress);
#endif
			~CShowPreview();
			
			//bool SetBitmap(CImageLoadingFormat* bitmap, const bool& isThumbnail);
			//CRegardsBitmap* GetBitmap(const bool& source);
			void UpdateScreenRatio();
			void SlidePosChange(const int &position, const wxString &key) {};
			void ZoomPos(const int &position) {};
			void MoveSlider(const int64_t &position);
			void ClickButton(const int &id) {};
			void SetTrackBarPosition(const int &iPos) {};
#ifdef NOTENCODE_FRAME
			void UpdateBitmap();
#else
			void UpdateBitmap(CVideoOptionCompress * videoOptionCompress);
#endif
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
			
			bool defaultToolbar;
			bool defaultViewer;
			bool bitmapWndLocal;
#ifdef NOTENCODE_FRAME
			CFFmpegDecodeFrameFilter * ffmpegTranscoding;
			CVideoEffectParameter * videoEffectParameter;
#else
			CVideoOptionCompress videoOptionCompress;
			COpenCLEngine * openCLEngine;
#endif
			bool transitionEnd;
			wxString filename;
			int progressValue;
			double timeTotal;
			int position = 0;
		};
	}
}
