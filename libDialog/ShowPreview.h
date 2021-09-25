#pragma once
#include <ThemeParam.h>
#include "PreviewToolbar.h"
#include "SliderVideoPreview.h"
#include "ScrollbarWnd.h"
#include "BitmapWnd.h"
#include <VideoCompressOption.h>
using namespace Regards::OpenCL;
using namespace Regards::Window;

class CRegardsBitmap;
class CFFmpegDecodeFrameFilter;
class CFFmpegTranscodingPimpl;
class CFFmpegDecodeFrame;


namespace Regards
{
	namespace Control
	{
		class CShowPreview : public CWindowMain, public CSliderInterface
		{
		public:
			CShowPreview(wxWindow* parent, wxWindowID id, wxWindowID bitmapViewerId, wxWindowID mainViewerId,
			             CThemeParam* config, const wxString& videoFilename,
			             CVideoOptionCompress* videoOptionCompress);
			~CShowPreview() override;

			//bool SetBitmap(CImageLoadingFormat* bitmap, const bool& isThumbnail);
			//CRegardsBitmap* GetBitmap(const bool& source);
			void UpdateScreenRatio() override;
			void SlidePosChange(const int& position, const wxString& key) override;

			void ZoomPos(const int& position) override
			{
			};
			void MoveSlider(const int64_t& position) override;

			void ClickButton(const int& id) override
			{
			};

			void SetTrackBarPosition(const int& iPos) override
			{
			};
			void UpdateBitmap(CVideoOptionCompress* videoOptionCompress, const wxString& extension,
			                  const bool& updatePicture = true);

		private:
			void ShowOriginal();
			void ShowNew();

			bool SetBitmap(CImageLoadingFormat* bitmap);
			void OnViewerZoomIn(wxCommandEvent& event);
			void OnViewerZoomOut(wxCommandEvent& event);
			void Resize() override;
			void OnControlSize(wxCommandEvent& event);
			void OnSetPosition(wxCommandEvent& event);
			void OnMoveLeft(wxCommandEvent& event);
			void OnMoveRight(wxCommandEvent& event);
			void OnMoveTop(wxCommandEvent& event);
			void OnMoveBottom(wxCommandEvent& event);
			void ShowPicture(CFFmpegDecodeFrame* decodeFrame, const wxString& label);
			void OnShowOriginal(wxCommandEvent& event);
			void OnShowNew(wxCommandEvent& event);
			void OnUpdatePicture(wxCommandEvent& event);

			static void ThreadLoading(void* data);

			CScrollbarWnd* scrollbar;
			CPreviewToolbar* previewToolbar;
			CSliderVideoPreview* sliderVideo;
			CBitmapWnd* bitmapWindow;
			CRegardsConfigParam* configRegards;
			CImageLoadingFormat* tempImage;


			bool defaultToolbar;
			bool defaultViewer;
			//bool bitmapWndLocal;

			CVideoOptionCompress videoOptionCompress;
			CFFmpegTranscodingPimpl* transcodeFFmpeg = nullptr;
			CFFmpegDecodeFrame* decodeFrame = nullptr;
			CFFmpegDecodeFrame* decodeFrameOriginal = nullptr;
			wxString extension;
			bool transitionEnd;
			wxString filename;
			int progressValue;
			double timeTotal;
			int position = 0;
			bool showOriginal = false;
			bool isFirstPicture = true;
			std::thread* threadStart = nullptr;
			bool moveSlider = false;
			bool oldShowOriginal = false;
			bool firstTime = true;
			bool compressIsOK = true;
			wxString key = "";
		};
	}
}
