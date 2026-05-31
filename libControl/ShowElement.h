#pragma once
#include <ThemeParam.h>
#include "BitmapInterface.h"
#include "BitmapToolbar.h"
#include "ScrollbarWnd.h"
#include <StreamInfo.h>
#include "SliderVideo.h"
#include "SliderInterface.h"
#include "VideoInterface.h"
#include <FiltreUpdate.h>
#include <memory>

using namespace Regards::Window;
using namespace Regards::Video;

class CVideoControlSoft;
class CRegardsConfigParam;

namespace Regards
{
    namespace Window { class CBitmapWnd3D; }
    namespace Video  { class CSlideToolbar; }

    namespace Control
    {
        class CBitmapWndViewer;

        // ---------------------------------------------------------------------------
        // Interface unifiée image / vidéo – évite les if(!isVideo) partout
        // ---------------------------------------------------------------------------
        class IViewerMode
        {
        public:
            virtual ~IViewerMode() = default;
            virtual void FlipVertical()   = 0;
            virtual void FlipHorizontal() = 0;
            virtual void Rotate90()       = 0;
            virtual void Rotate270()      = 0;
        };

        class CShowElement : public CWindowMain,
                             public CSliderInterface,
                             public IVideoInterface,
                             public IFiltreUpdate
        {
        public:
            CShowElement(wxWindow* parent,
                         wxWindowID id,
                         wxWindowID bitmapViewerId,
                         wxWindowID mainViewerId,
                         CBitmapInterface* bitmapInterface,
                         CThemeParam* config,
                         const bool& exportPicture);
            ~CShowElement() override;

            // Navigation
            void IsNextPicture(const bool& value);

            // Toolbar visibility
            void HideToolbar();
            void ShowToolbar();

            // Transformations – déléguées via IViewerMode
            void FlipVertical();
            void Rotate90();
            void Rotate270();
            void FlipHorizontal();

            // Image
            bool SetBitmap(CImageLoadingFormat* bitmap, const bool& isThumbnail);
            void SetBitmapPreviewEffect(const int& effect);
            void StopTransition();
            void TransitionEnd();
            CImageLoadingFormat* GetBitmap(const bool& source);
            void SavePicture();

            // Affichage / thème
            bool IsToolbarMouseOver();
            void SetFullscreen(const bool& fullscreen);
            void ReloadResource();
            void UpdateScreenRatio() override;
            void SetDiaporamaMode();
            void SetNormalMode();

            // Vidéo
            bool SetVideo(const wxString& filename, const int& rotation, const bool& play);
            void SetPosition(const int64_t& timePosition);
            void SetVideoDuration(const int64_t& position) override;
            void SetTimePosition(const int64_t& timePosition);
            bool IsPause();
            cv::Mat GetVideoBitmap();
            void SetVideoPreviewEffect(CEffectParameter* effectParameter);
            CEffectParameter* GetParameter() override;
            void UpdateFiltre(CEffectParameter* effectParameter) override;
            void StopVideo(const wxString& photoName);
            void PlayVideo();
            void PauseVideo();
            void ChangeAudio(const int& langue);
            void SetStreamInfo(vector<CStreamInfo>& listAudio,
                               vector<CStreamInfo>& listVideo,
                               vector<CStreamInfo>& listSubtitle) override;
            CVideoControlSoft* GetVideoControl();

            // CSliderInterface
            void SlidePosChange(const int& /*position*/, const wxString& /*key*/) override {}
            void ZoomPos(const int& /*position*/) override {}
            void MoveSlider(const int64_t& position) override;
            void ClickButton(const int& id) override;
            void SetTrackBarPosition(const int& /*iPos*/) override {}

        protected:
            // IVideoInterface
            void OnVideoEnd()                          override;
            void OnVideoStart()                        override;
            void OnVideoStop()                         override;
            void OnVideoPause()                        override;
            void OnAfterOpenVideo()                    override;
            void OnPositionVideo(const int64_t& position) override;

            void RepeatVideo();
            static int GetAngleAndFlipFromExif(int exif, int& angle, int& flipH, int& flipV);

        private:
            // ---- Layout helpers ----
            void IdleFunction() override;
            void Resize() override;
            void ResizeImage(int w, int h);   // sous-routine de Resize() mode image
            void ResizeVideo(int w, int h);   // sous-routine de Resize() mode vidéo

            // ---- Thread rotation ----
            static void RotateRecognition(void* param);

            // ---- Event handlers ----
            void OnViewerDblClick(wxCommandEvent& event);
            void OnViewerZoomIn(wxCommandEvent& event);
            void OnViewerZoomOut(wxCommandEvent& event);
            void OnRotateDetect(wxCommandEvent& event);
            void OnControlSize(wxCommandEvent& event);
            void OnSetPosition(wxCommandEvent& event);
            void OnPauseVideo(wxCommandEvent& event);
            void OnMoveLeft(wxCommandEvent& event);
            void OnMoveRight(wxCommandEvent& event);
            void OnMoveTop(wxCommandEvent& event);
            void OnMoveBottom(wxCommandEvent& event);
            void OnSave(wxCommandEvent& event);
            void OnClose(wxCommandEvent& event);
            void OnShrink(wxCommandEvent& event);
            void OnValueChange(wxCommandEvent& event);
            void OnValueShrinkChange(wxCommandEvent& event);

            // ---- Helpers vidéo ----
            void ShowSliderToolbar(bool show);
            void ShowSlider(bool show);
            void InitControl();

            // ---- Helper dispatch IViewerMode ----
            IViewerMode* ActiveViewer() const;   // renvoie bitmapWindow ou videoWindow selon isVideo

            // ============================================================
            // Membres – raw owning pointers remplacés par unique_ptr.
            // Les enfants wxWidgets (bitmapWindow, scrollbar, etc.) restent
            // en raw ptr car wxWidgets gère leur durée de vie (parent/child).
            // En revanche les objets non-wxWidgets utilisent unique_ptr.
            // ============================================================
            CScrollbarWnd* scrollbar = nullptr;
            CBitmapToolbar* pictureToolbar = nullptr;
            CBitmapWndViewer* bitmapWindow = nullptr;
            CBitmapWnd3D* bitmapWindowRender = nullptr;
            CBitmapInterface* bitmapInterface = nullptr;
            CRegardsConfigParam* configRegards = nullptr;

            // tempImage : propriété claire, géré via unique_ptr
            std::unique_ptr<CImageLoadingFormat> tempImage;

            CSlideToolbar* slideToolbar = nullptr;
            CSliderVideo* videoSlider = nullptr;
            CVideoControlSoft* videoWindow = nullptr;
            CWindowMain* windowMain = nullptr;
            

            wxString filename;
            int  progressValue  = 0;
            int  height         = 0;
            int  width          = 0;
            int  videoPosOld    = 0;
            int64_t videoTotalTime = 0;


            bool defaultToolbar  = true;
            bool defaultViewer   = true;
            bool isDiaporama     = false;
            bool transitionEnd   = true;
            bool fullscreen      = false;
            bool showToolbar     = true;
            bool firstElement    = true;
            bool showToolbarSup  = false;
            bool play            = false;
            bool toolbarOutside  = false;
            bool softRender      = false;
            bool refreshSlider   = false;
            bool isVideo         = false;

            vector<CStreamInfo> listStream;
            std::vector<int>    sound_value;
        };
    }
}
