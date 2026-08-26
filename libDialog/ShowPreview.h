#pragma once

#include <ThemeParam.h>
#include "PreviewToolbar.h"
#include "SliderVideoPreview.h"
#include "ScrollbarWnd.h"
#include <VideoCompressOption.h>

#include <atomic>
#include <cstdint>
#include <memory>
#include <thread>

class CFFmpegDecodeFrameFilter;
class CFFmpegTranscoding;
class CRegardsConfigParam;
class CImageLoadingFormat;

namespace Regards
{
    namespace OpenCL
    {
        class COpenCLContext;
    }

    namespace Window
    {
        class CBitmapWndRender;
        class CBitmapWnd3D;
    }

    namespace Video
    {
        class CVideoThumb;
    }

    namespace Control
    {
        class CRenderPreview
        {
        public:
            CRenderPreview() = default;
            ~CRenderPreview() = default;

            cv::Mat decodeFrameOriginal;
            cv::Mat decodeFrame;

            wxWindow* parent = nullptr;
            CVideoOptionCompress videoOption;

            bool compressIsOK = false;

            wxString filename;
            wxString extension;

            int position = 0;
            int ret = -1;

            // Generation of the request which produced this result.
            uint64_t requestId = 0;
        };

        class CShowPreview : public CWindowMain, public CSliderInterface
        {
        public:
            CShowPreview(
                wxWindow* parent,
                wxWindowID id,
                CThemeParam* config,
                CVideoOptionCompress* videoOptionPt);

            ~CShowPreview() override;

            void SetErrorCompressionHandler(std::function<void(int)> handler)
            {
                errorCompressionHandler = handler;
            }

            void SetParameter(const wxString& videoFilename);

            void UpdateScreenRatio() override;

            void SlidePosChange(
                const int& position,
                const wxString& key) override;

            void ZoomPos(const int& position) override
            {}

            void MoveSlider(const int64_t& position) override;

            void ClickButton(const int& id) override
            {}

            void SetTrackBarPosition(const int& iPos) override
            {}

            void UpdateBitmap(
                const wxString& extension,
                bool updatePicture = true);

        private:
            void ShowOriginal();
            void ShowNew();

            void StopThread();

            void SetBitmapToViewer(
                CImageLoadingFormat* bitmap,
                bool isUpdate);

            void OnViewerZoomIn(wxCommandEvent& event);
            void OnViewerZoomOut(wxCommandEvent& event);

            void Resize() override;

            void OnControlSize(wxCommandEvent& event);
            void OnSetPosition(wxCommandEvent& event);

            void OnMoveLeft(wxCommandEvent& event);
            void OnMoveRight(wxCommandEvent& event);
            void OnMoveTop(wxCommandEvent& event);
            void OnMoveBottom(wxCommandEvent& event);

            void ShowPicture(
                cv::Mat& decodeFrame,
                const wxString& label);

            void OnShowOriginal(wxCommandEvent& event);
            void OnShowNew(wxCommandEvent& event);
            void OnUpdatePicture(wxCommandEvent& event);

            bool SetBitmap(CImageLoadingFormat* bitmap);

            static void ThreadLoading(void* data);

        private:
            std::unique_ptr<CScrollbarWnd> scrollbar;
            std::unique_ptr<CPreviewToolbar> previewToolbar;
            std::unique_ptr<CSliderVideoPreview> sliderVideo;
            std::unique_ptr<CBitmapWndRender> bitmapWindow;
            std::unique_ptr<CBitmapWnd3D> bitmapWindowRender;

            // The object is owned by the caller.
            // UpdateBitmap() copies it into CRenderPreview before starting
            // the worker thread.
            CVideoOptionCompress* videoOption = nullptr;

            CRegardsConfigParam* configRegards = nullptr;

            bool defaultToolbar = true;
            bool defaultViewer = true;

            cv::Mat decodeFrame;
            cv::Mat decodeFrameOriginal;

            wxString extension;
            bool transitionEnd = false;
            wxString filename;

            int progressValue = 0;
            double timeTotal = 0.0;
            int position = 0;

            bool showOriginal = false;
            bool isFirstPicture = true;

            std::unique_ptr<std::thread> threadStart;

            bool moveSlider = false;
            bool oldShowOriginal = false;
            bool firstTime = true;
            bool compressIsOK = true;

            wxString key;
            int orientation = 0;
            std::function<void(int)> errorCompressionHandler;
            /*
             * Each call to UpdateBitmap() increments this value.
             * A result is displayed only if its requestId is still the
             * current one when the GUI receives it.
             */
            std::atomic<uint64_t> requestId{ 0 };
        };
    }
}