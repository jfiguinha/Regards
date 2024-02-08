#pragma once
#include <ThemeParam.h>
#include "BitmapInterface.h"
#include "BitmapToolbar.h"
#include "ScrollbarWnd.h"
#include <StreamInfo.h>
#include "SliderVideo.h"
#include "SliderInterface.h"
#include "VideoInterface.h"
#include "ScrollbarWnd.h"
#include <ThemeParam.h>
#include <FiltreUpdate.h>
using namespace Regards::Window;
using namespace Regards::Video;

class CVideoControlSoft;
class CRegardsConfigParam;

namespace Regards
{
	namespace Window
	{
		class CBitmapWnd3D;
	}

	namespace Video
	{
		class CSlideToolbar;
	}

	namespace Control
	{
		class CBitmapWndViewer;

		class CShowElement : public CWindowMain, public CSliderInterface, public IVideoInterface, public IFiltreUpdate
		{
		public:
			CShowElement(wxWindow* parent, wxWindowID id, wxWindowID bitmapViewerId, wxWindowID mainViewerId,
			             CBitmapInterface* bitmapInterfaceCTreeWithScrollbarInterface, CThemeParam* config,
			             const bool& exportPicture);
			~CShowElement() override;
			void IsNextPicture(const bool& value);
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
			CImageLoadingFormat* GetBitmap(const bool& source);
			void ReloadResource();
			void UpdateScreenRatio() override;
			void SetDiaporamaMode();
			void SetNormalMode();
			void SavePicture();

			//--------------------------------------------------------------------------
			//Video
			//--------------------------------------------------------------------------

			bool SetVideo(const wxString& filename, const int& rotation, const bool& play);
			void SetPosition(const int64_t& timePosition);
			void SetVideoDuration(const int64_t& position) override;
			void SetTimePosition(const int64_t& timePosition);
			bool IsPause();

			cv::Mat GetVideoBitmap();
			void SetVideoPreviewEffect(CEffectParameter* effectParameter);
			CEffectParameter* GetParameter() override;
			void UpdateFiltre(CEffectParameter* effectParameter) override;

			void SlidePosChange(const int& position, const wxString& key) override
			{
			};

			void ZoomPos(const int& position) override
			{
			};
			void MoveSlider(const int64_t& position) override;
			void ClickButton(const int& id) override;

			void SetTrackBarPosition(const int& iPos) override
			{
			};

			CVideoControlSoft* GetVideoControl();

			void StopVideo(wxString photoName);
			void PlayVideo();
			void PauseVideo();

			void ChangeAudio(const int& langue);

			void SetStreamInfo(vector<CStreamInfo>& listAudio, vector<CStreamInfo>& listVideo,
			                   vector<CStreamInfo>& listSubtitle) override;

		protected:
			//Interface Video
			void OnVideoEnd() override;
			void OnVideoStart() override;
			void OnVideoStop() override;
			void OnVideoPause() override;
			void OnAfterOpenVideo() override;
			void OnPositionVideo(const int64_t& position) override;
			void RepeatVideo();
			int GetAngleAndFlipFromExif(const int& exif, int& angle, int& flipH, int& flipV);

		private:
			void OnIdle(wxIdleEvent& evt) override;
			void OnViewerDblClick(wxCommandEvent& event);
			void OnViewerZoomIn(wxCommandEvent& event);
			void OnViewerZoomOut(wxCommandEvent& event);
			void Resize() override;
			void OnRotateDetect(wxCommandEvent& event);
			void OnControlSize(wxCommandEvent& event);
			void OnSetPosition(wxCommandEvent& event);
			void OnPauseVideo(wxCommandEvent& event);

			static void RotateRecognition(void* param);

			void OnMoveLeft(wxCommandEvent& event);
			void OnMoveRight(wxCommandEvent& event);
			void OnMoveTop(wxCommandEvent& event);
			void OnMoveBottom(wxCommandEvent& event);

			CScrollbarWnd* scrollbar;
			CBitmapToolbar* pictureToolbar;
			CBitmapWndViewer* bitmapWindow;
			CBitmapWnd3D* bitmapWindowRender;
			CBitmapInterface* bitmapInterface;
			CRegardsConfigParam* configRegards;
			CImageLoadingFormat* tempImage;
			bool defaultToolbar;
			bool defaultViewer;
			//bool bitmapWndLocal;
			bool isDiaporama;

			bool transitionEnd;
			wxString filename;
			int progressValue;
			bool fullscreen;
			bool showToolbar;

			//--------------------------------------------------------------------------
			//Video
			//--------------------------------------------------------------------------
			void OnSave(wxCommandEvent& event);
			void OnClose(wxCommandEvent& event);
			void OnShrink(wxCommandEvent& event);
			void OnValueChange(wxCommandEvent& event);
			void OnValueShrinkChange(wxCommandEvent& event);

			void ShowSliderToolbar(const bool& show);
			void ShowSlider(const bool& show);
			void InitControl();
			CSlideToolbar* slideToolbar;
			CSliderVideo* videoSlider;
			CVideoControlSoft* videoWindow;
			CWindowMain* windowMain;
			bool showToolbarSup = false;
			bool play;
			bool toolbarOutside;
			int height;
			int width;
			int videoPosOld;
			bool softRender = false;
			int64_t videoTotalTime = 0;
			vector<CStreamInfo> listStream;
			std::vector<int> sound_value;

			bool refreshSlider = false;
			bool isVideo = false;
		};
	}
}
