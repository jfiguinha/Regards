#include <header.h>

#include "ShowPreview.h"

#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif

#include <BitmapWnd3d.h>
#include <BitmapWndRender.h>
#include <FileUtility.h>
#include <ImageLoadingFormat.h>
#include <ParamInit.h>
#include <appcontext.h>
#include <libPicture.h>
#include <videothumb.h>
#include <wx/filename.h>
#include <RenderBitmapOpenGL.h>
#include <algorithm>
#include <cstdint>
#include <FiltreEffet.h>
#include <IFiltreEffet.h>
#include <VideoStabilization.h>
#include "CompressionAudioVideoOption.h"
#include "FFmpegTranscoding.h"

extern AppContext application_context;

using namespace Regards::Picture;
using namespace Regards::Window;
using namespace Regards::Control;
using namespace Regards::Video;

namespace {
    // Keep ownership of the event payload explicit. The payload is deleted
    // by CShowPreview::OnUpdatePicture(), after the event has been processed.
    inline CRenderPreview* GetRenderPreview(wxCommandEvent& event) {
        return static_cast<CRenderPreview*>(event.GetClientData());
    }
}  // namespace

void CShowPreview::UpdateScreenRatio() {
    if (scrollbar) scrollbar->UpdateScreenRatio();

    if (previewToolbar) previewToolbar->UpdateScreenRatio();

    if (bitmapWindow) bitmapWindow->UpdateScreenRatio();

    Resize();
}

CShowPreview::CShowPreview(wxWindow* parent, wxWindowID id, CThemeParam* config,
    CVideoOptionCompress* videoOptionPt)
    : CWindowMain("ShowBitmap", parent, id),
    videoOption(videoOptionPt),
    configRegards(CParamInit::getInstance()),
    defaultToolbar(true),
    defaultViewer(true),
    transitionEnd(false),
    progressValue(0),
    timeTotal(0.0),
    position(0),
    showOriginal(false),
    isFirstPicture(true),
    moveSlider(false),
    oldShowOriginal(false),
    firstTime(true),
    compressIsOK(true),
    key(""),
    orientation(0) {
    CThemeBitmapWindow themeBitmap;
    CThemeScrollBar themeScroll;
    CThemeToolbar themeToolbar;
    CThemeSlider themeSlider;

    const std::vector<int> value = {
        1,   2,   3,   4,   5,   6,   8,   12,  16,  25,  33,  50,   66,  75,
        100, 133, 150, 166, 200, 300, 400, 500, 600, 700, 800, 1200, 1600 };

    if (config) {
        config->GetBitmapToolbarTheme(&themeToolbar);
        config->GetBitmapWindowTheme(&themeBitmap);
        config->GetScrollTheme(&themeScroll);
        config->GetVideoSliderTheme(&themeSlider);
    }

    previewToolbar = std::make_unique<CPreviewToolbar>(
        this, wxID_ANY, BITMAPWINDOWVIEWERIDDLG, themeToolbar, false);

    previewToolbar->SetTabValue(value);

    themeBitmap.colorScreen = wxColour("black");

    bitmapWindow =
        std::make_unique<CBitmapWndRender>(previewToolbar.get(), 0, themeBitmap);

    bitmapWindowRender =
        std::make_unique<CBitmapWnd3D>(this, BITMAPWINDOWVIEWERIDDLG);

    bitmapWindowRender->SetBitmapRenderInterface(bitmapWindow.get());
    bitmapWindow->SetTabValue(value);
    bitmapWindow->SetPreview(1);

    scrollbar = std::make_unique<CScrollbarWnd>(this, bitmapWindowRender.get(),
        wxID_ANY, "BitmapScroll");

    sliderVideo =
        std::make_unique<CSliderVideoPreview>(this, wxID_ANY, this, themeSlider);

    Connect(wxEVT_BITMAPZOOMIN, wxEVT_COMMAND_TEXT_UPDATED,
        wxCommandEventHandler(CShowPreview::OnViewerZoomIn));

    Connect(wxEVT_BITMAPZOOMOUT, wxEVT_COMMAND_TEXT_UPDATED,
        wxCommandEventHandler(CShowPreview::OnViewerZoomOut));

    Connect(wxEVENT_MOVELEFT, wxCommandEventHandler(CShowPreview::OnMoveLeft));
    Connect(wxEVENT_MOVERIGHT, wxCommandEventHandler(CShowPreview::OnMoveRight));
    Connect(wxEVENT_MOVETOP, wxCommandEventHandler(CShowPreview::OnMoveTop));
    Connect(wxEVENT_MOVEBOTTOM,
        wxCommandEventHandler(CShowPreview::OnMoveBottom));
    Connect(wxEVENT_SETCONTROLSIZE,
        wxCommandEventHandler(CShowPreview::OnControlSize));
    Connect(wxEVENT_SETPOSITION,
        wxCommandEventHandler(CShowPreview::OnSetPosition));
    Connect(wxEVENT_SHOWORIGINAL,
        wxCommandEventHandler(CShowPreview::OnShowOriginal));
    Connect(wxEVENT_SHOWNEW, wxCommandEventHandler(CShowPreview::OnShowNew));
    Connect(wxEVENT_UPDATEPICTURE,
        wxCommandEventHandler(CShowPreview::OnUpdatePicture));
}

void CShowPreview::SetParameter(const wxString& videoFilename) {
    isFirstPicture = true;
    firstTime = true;
    filename = videoFilename;
    position = 0;

    if (!sliderVideo || filename.empty()) return;

    CVideoThumb video(filename);

    timeTotal = video.GetMovieDuration();
    orientation = video.GetOrientation();

    sliderVideo->SetTotalSecondTime(static_cast<int64_t>(timeTotal * 1000.0));

    MoveSlider(0);
}

void CShowPreview::SetBitmapToViewer(CImageLoadingFormat* bitmap,
    bool isUpdate) {
    if (!bitmap || !bitmapWindowRender) return;

    auto* event =
        new wxCommandEvent(isUpdate ? wxEVENT_UPDATEBITMAP : wxEVENT_SETBITMAP);

    event->SetClientData(bitmap);

    wxQueueEvent(bitmapWindowRender.get(), event);
}

void CShowPreview::ShowPicture(cv::Mat& bitmap, const wxString& label) {
    if (bitmap.empty() || !bitmapWindow) return;

    auto* imageLoadingFormat = new CImageLoadingFormat();
    imageLoadingFormat->SetPicture(bitmap);
    imageLoadingFormat->SetOrientation(orientation);

    SetBitmapToViewer(imageLoadingFormat, !isFirstPicture);

    if (isFirstPicture) bitmapWindow->ShrinkImage();

    if (auto* dlg = static_cast<CompressionAudioVideoOption*>(
        FindWindowByName("CompressionAudioVideoOption"))) {
        dlg->ChangeLabelPicture(label);
    }

    isFirstPicture = false;
}

void CShowPreview::ShowOriginal() {
    ShowPicture(decodeFrameOriginal, "Original Video");
}

void CShowPreview::ShowNew() { ShowPicture(decodeFrame, "New Video"); }

void CShowPreview::OnShowOriginal(wxCommandEvent& WXUNUSED(event)) {
    showOriginal = true;
    oldShowOriginal = true;
    ShowOriginal();
}

void CShowPreview::OnShowNew(wxCommandEvent& WXUNUSED(event)) {
    showOriginal = false;
    oldShowOriginal = false;
    ShowNew();
}

void CShowPreview::OnUpdatePicture(wxCommandEvent& event) {
    auto* renderPreview = GetRenderPreview(event);

    if (!renderPreview) return;

    // A result can arrive after the user has moved the slider again.
    // Do not display an obsolete frame.
    const bool isCurrentRequest = renderPreview->parent == this &&
        renderPreview->filename == filename &&
        renderPreview->position == position;

    if (isCurrentRequest) {
        if (!renderPreview->compressIsOK) {
            if (errorCompressionHandler) {
                // Preferred path: explicit hook set via
                // SetErrorCompressionHandler(), no dependency on the wx
                // window hierarchy shape.
                errorCompressionHandler(renderPreview->ret);
            }
            else if (renderPreview->parent && renderPreview->parent->GetParent() &&
                renderPreview->parent->GetParent()->GetParent()) {
                // Legacy fallback, kept for callers that have not wired the
                // handler above yet. Fragile: assumes a fixed depth of 2
                // wx window levels between this control and the receiver.
                wxCommandEvent evt(wxEVENT_ERRORCOMPRESSION);
                evt.SetInt(renderPreview->ret);

                renderPreview->parent->GetParent()
                    ->GetParent()
                    ->GetEventHandler()
                    ->AddPendingEvent(evt);
            }

            ShowOriginal();
        }
        else {
            decodeFrame = renderPreview->decodeFrame;
            decodeFrameOriginal = renderPreview->decodeFrameOriginal;

            if (showOriginal)
                ShowOriginal();
            else
                ShowNew();
        }

        if (firstTime) {
            if (previewToolbar && bitmapWindow)
                previewToolbar->SetTrackBarPosition(bitmapWindow->GetPosRatio());

            firstTime = false;
        }

        if (sliderVideo) sliderVideo->Stop();
    }

    delete renderPreview;
}

void CShowPreview::SlidePosChange(const int& newPosition,
    const wxString& newKey) {
    if (newKey == "Move") {
        key = newKey;
        moveSlider = true;
        showOriginal = true;
        position = newPosition;
        UpdateBitmap("");
    }
    else {
        key = newKey;
        showOriginal = oldShowOriginal;
        moveSlider = false;
        position = newPosition;
        UpdateBitmap("");
    }
}

void CShowPreview::MoveSlider(const int64_t& newPosition) {
    showOriginal = oldShowOriginal;
    moveSlider = false;

    // The original member is an int. Keep the conversion explicit.
    position = static_cast<int>(newPosition);

    UpdateBitmap("");
}

void CShowPreview::UpdateBitmap(const wxString& newExtension,
    bool updatePicture) {
    if (!sliderVideo || !videoOption || filename.empty()) return;

    extension = newExtension;

    sliderVideo->Start();

    /*
     * The existing class owns a single worker thread. We must join it before
     * replacing it. This preserves the current header/API while avoiding
     * detached threads and dangling CRenderPreview pointers.
     *
     * Important: this still blocks the GUI while the previous EncodeFrame()
     * finishes. A fully asynchronous/cancellable implementation requires a
     * small header change (persistent worker + cancellation/request ID).
     */
    StopThread();

    auto* renderPreview = new CRenderPreview();

    renderPreview->extension = extension;
    renderPreview->filename = filename;
    renderPreview->position = position;
    renderPreview->parent = this;
    renderPreview->videoOption = *videoOption;
    renderPreview->compressIsOK = false;
    renderPreview->ret = -1;

    threadStart = std::make_unique<std::thread>(ThreadLoading, renderPreview);
}

void CShowPreview::ThreadLoading(void* data) {
    auto* renderPreview = static_cast<CRenderPreview*>(data);

    if (!renderPreview) return;

    COpenCLContext openCLContext;
    openCLContext.CreateDefaultOpenCLContext();

    wxString fileTemp;

    if (renderPreview->extension.empty()) {
        const wxFileName fileName(renderPreview->filename);
        renderPreview->extension = fileName.GetExt();
    }

    fileTemp =
        CFileUtility::GetTempFile("video_temp." + renderPreview->extension);

    if (fileTemp.empty()) {
        renderPreview->ret = -1;
        renderPreview->compressIsOK = false;
    }
    else {

        CFFmpegTranscoding ffmpegtranscoding(&openCLContext);
        int ret = ffmpegtranscoding.EncodeOneFrame(nullptr, renderPreview->filename, fileTemp, renderPreview->position, &renderPreview->videoOption);

        // Was previously never written back to renderPreview->ret, which
        // stayed at its -1 init value forever: the success branch below was
        // therefore dead code and the "new video" preview never displayed.
        renderPreview->ret = ret;

        if (!ffmpegtranscoding.GetFrameOutput().empty())
        {
            //renderPreview->decodeFrameOriginal = ffmpegtranscoding.GetFrameOutput();
            renderPreview->decodeFrameOriginal = ffmpegtranscoding.GetFrameOutputWithOutEffect();
        }

        if (renderPreview->ret == 0) {
            CVideoThumb video(fileTemp);

            renderPreview->decodeFrame = video.GetVideoFramePos(0, 0, 0);

            if (renderPreview->decodeFrame.empty())
                renderPreview->decodeFrame = application_context.GetDefaultPicture();

            renderPreview->compressIsOK = true;
        }
        else {
            renderPreview->compressIsOK = false;
        }

        // The temporary encoded frame is no longer needed after CVideoThumb
        // has been destroyed.
        wxRemoveFile(fileTemp);
    }

    // The GUI object must still exist when posting the event.
    // CShowPreview::StopThread() ensures that destruction does not race with
    // the worker in the normal destruction path.
    if (renderPreview->parent) {
        wxCommandEvent evt(wxEVENT_UPDATEPICTURE);
        evt.SetClientData(renderPreview);

        renderPreview->parent->GetEventHandler()->AddPendingEvent(evt);
    }
    else {
        delete renderPreview;
    }
}

void CShowPreview::OnControlSize(wxCommandEvent& event) {
    if (!scrollbar) return;

    wxCommandEvent evt(wxEVENT_SETCONTROLSIZE);
    evt.SetClientData(event.GetClientData());

    scrollbar->GetEventHandler()->AddPendingEvent(evt);
}

void CShowPreview::OnSetPosition(wxCommandEvent& event) {
    if (!scrollbar) return;

    wxCommandEvent evt(wxEVENT_SETPOSITION);
    evt.SetClientData(event.GetClientData());

    scrollbar->GetEventHandler()->AddPendingEvent(evt);
}

void CShowPreview::OnMoveLeft(wxCommandEvent& event) {
    if (!scrollbar) return;

    wxCommandEvent evt(wxEVENT_MOVELEFT);
    evt.SetInt(event.GetInt());

    scrollbar->GetEventHandler()->AddPendingEvent(evt);
}

void CShowPreview::OnMoveRight(wxCommandEvent& event) {
    if (!scrollbar) return;

    wxCommandEvent evt(wxEVENT_MOVERIGHT);
    evt.SetInt(event.GetInt());

    scrollbar->GetEventHandler()->AddPendingEvent(evt);
}

void CShowPreview::OnMoveTop(wxCommandEvent& event) {
    if (!scrollbar) return;

    wxCommandEvent evt(wxEVENT_MOVETOP);
    evt.SetInt(event.GetInt());

    scrollbar->GetEventHandler()->AddPendingEvent(evt);
}

void CShowPreview::OnMoveBottom(wxCommandEvent& event) {
    if (!scrollbar) return;

    wxCommandEvent evt(wxEVENT_MOVEBOTTOM);
    evt.SetInt(event.GetInt());

    scrollbar->GetEventHandler()->AddPendingEvent(evt);
}

void CShowPreview::StopThread() {
    if (!threadStart) return;

    if (threadStart->joinable()) threadStart->join();

    threadStart.reset();
}

CShowPreview::~CShowPreview() { StopThread(); }

void CShowPreview::Resize() {
    const int width = GetWindowWidth();
    const int height = GetWindowHeight();

    if (width <= 0 || height <= 0) return;

    if (!scrollbar || !previewToolbar || !sliderVideo) return;

    scrollbar->ShowVerticalScroll();
    scrollbar->ShowHorizontalScroll();

    const int pictureWidth = width;

    const int pictureHeight = std::max(
        0, height - previewToolbar->GetHeight() - sliderVideo->GetHeight());

    scrollbar->SetSize(0, 0, pictureWidth, pictureHeight);

    scrollbar->Refresh();

    previewToolbar->SetSize(0, pictureHeight, width, previewToolbar->GetHeight());

    previewToolbar->Refresh();

    sliderVideo->SetSize(0, pictureHeight + sliderVideo->GetHeight(), width,
        sliderVideo->GetHeight());

    sliderVideo->Refresh();
}

bool CShowPreview::SetBitmap(CImageLoadingFormat* bitmap) {
    if (!bitmap || !bitmapWindow) return false;

    if (previewToolbar)
        previewToolbar->SetTrackBarPosition(bitmapWindow->GetPosRatio());

    bitmap->SetOrientation(orientation);

    SetBitmapToViewer(bitmap, false);

    return true;
}

void CShowPreview::OnViewerZoomIn(wxCommandEvent& WXUNUSED(event)) {
    if (previewToolbar) previewToolbar->ChangeZoomInPos();
}

void CShowPreview::OnViewerZoomOut(wxCommandEvent& WXUNUSED(event)) {
    if (previewToolbar) previewToolbar->ChangeZoomOutPos();
}