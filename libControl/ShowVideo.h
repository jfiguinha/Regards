#pragma once
#ifdef FFMPEG

#include <StreamInfo.h>
#include "SliderVideo.h"
#include "SliderInterface.h"
#include "VideoInterface.h"
#include "ScrollbarWnd.h"
#include <ThemeParam.h>
#include <OpenCLContext.h>
#include <FiltreUpdate.h>
using namespace Regards::OpenCL;
using namespace Regards::Window;
using namespace Regards::Video;

class CVideoEffectParameter;
class CVideoControlSoft;


namespace Regards
{
	namespace Video
	{
		class CSlideToolbar;

		class CShowVideo : public wxWindow, public CSliderInterface, public IVideoInterface, public IFiltreUpdate
		{
		public:
			CShowVideo(wxWindow* parent, wxWindowID id, CWindowMain * windowMain, CThemeParam * config);
			~CShowVideo();
			bool SetVideo(const wxString &filename, const int &rotation, const bool &play);

			void SetPosition(const int64_t &timePosition);
			void SetVideoDuration(const int64_t &position);
			void SetTimePosition(const int64_t &timePosition);
			void SavePicture();
			void ShowToolbar();
			void HideToolbar();

			void Resize();

			void SetVideoPreviewEffect(CEffectParameter * effectParameter);
			CEffectParameter * GetParameter();
			void UpdateFiltre(CEffectParameter * effectParameter);

			void SlidePosChange(const int &position, const wxString &key) {};
			void ZoomPos(const int &position) {};
			void MoveSlider(const int64_t &position);
			void ClickButton(const int &id);
			void SetTrackBarPosition(const int &iPos) {};
			void Rotate90();
			void Rotate270();
			void FlipVertical();
			void FlipHorizontal();
			CVideoControlSoft * GetVideoControl();

			void StopVideo(wxString photoName);
			void PlayVideo();
			void PauseVideo();

			void ChangeAudio(const int &langue);
			bool IsToolbarMouseOver();

			void SetDiaporamaMode();
			void SetNormalMode();

			void UpdateScreenRatio();
			void SetStreamInfo(vector<CStreamInfo> & listAudio, vector<CStreamInfo> & listVideo, vector<CStreamInfo> & listSubtitle);

		protected:

			//Interface Video
			void OnVideoEnd();
			void OnVideoStart();
			void OnVideoStop();
			void OnVideoPause();
			void OnAfterOpenVideo();
			void OnPositionVideo(const int64_t &position);

		private:
			void OnClose(wxCommandEvent& event);
			void OnShrink(wxCommandEvent& event);
			void OnSize(wxSizeEvent& event);
			void OnValueChange(wxCommandEvent& event);

			void ShowSliderToolbar(const bool &show);
			void ShowSlider(const bool &show);
			void InitControl();
			CSlideToolbar * slideToolbar;
			CScrollbarWnd * scrollbar;
			CSliderVideo * videoSlider;
			CVideoControlSoft * videoWindow;
			CVideoEffectParameter * videoEffectParameter;
			CWindowMain * windowMain;
			bool showToolbarSup = false;
			bool play;
			bool toolbarOutside;
			int height;
			int width;
			int videoPosOld;
			bool isDiaporama;
			bool softRender = false;
			int64_t videoTotalTime = 0;
			vector<CStreamInfo> listStream;
			std::vector<int> value;
		};
	}
}



#endif