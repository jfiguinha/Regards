#include <header.h>
#include "wxFFmpegView.h"
#ifdef __WXGTK__
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#endif
#include <wx/glcanvas.h>

#include "glmanager.h"
#include "movie.h"

class wxFFmpegInnerView : public wxGLCanvas {
public:
#if wxCHECK_VERSION(3, 1, 5)
    wxFFmpegInnerView(wxWindow *parent,
                      const wxGLAttributes &dispAttrs,
                      wxWindowID winid = wxID_ANY,
                      const wxPoint &pos = wxDefaultPosition,
                      const wxSize &size = wxDefaultSize,
                      long style = 0,
                      const wxString &name = wxGLCanvasName,
                      const wxPalette &palette = wxNullPalette);
#else
    wxFFmpegInnerView(wxWindow *parent,
                      wxWindowID winid = wxID_ANY,
                      const int *attribList = NULL,
                      const wxPoint &pos = wxDefaultPosition,
                      const wxSize &size = wxDefaultSize,
                      long style = 0,
                      const wxString &name = wxGLCanvasName,
                      const wxPalette &palette = wxNullPalette);
#endif

    ~wxFFmpegInnerView();

    void open(std::string filename);
    void start();
    void play();
    void pause();
    void stop();
    void close();
    void mute();
    void plus();
    void minus();

private:
    void OnPaint(wxPaintEvent &event);
    void OnSize(wxSizeEvent &event);
    void OnRenderTimer(wxTimerEvent &event);

    wxGLContext *glContext_;
    GLManager *glManager_{nullptr};
    wxTimer renderTimer_;
    Movie *movie_{nullptr};
    int64_t pts_;

    wxDECLARE_EVENT_TABLE();
};

// clang-format off
wxBEGIN_EVENT_TABLE(wxFFmpegInnerView, wxGLCanvas)
    EVT_PAINT(wxFFmpegInnerView::OnPaint)
    EVT_SIZE(wxFFmpegInnerView::OnSize)
wxEND_EVENT_TABLE();
// clang-format on

namespace {} // namespace

#if wxCHECK_VERSION(3, 1, 5)
wxFFmpegInnerView::wxFFmpegInnerView(wxWindow *parent,
                                     const wxGLAttributes &dispAttrs,
                                     wxWindowID winid,
                                     const wxPoint &pos,
                                     const wxSize &size,
                                     long style,
                                     const wxString &name,
                                     const wxPalette &palette)
    : wxGLCanvas(parent, dispAttrs, winid, pos, size, style, name, palette)
{
    wxGLContextAttrs ctxAttrs;
    ctxAttrs.PlatformDefaults().CoreProfile().OGLVersion(3, 3).EndList();
    glContext_ = new wxGLContext(this, nullptr, &ctxAttrs);
    if (!glContext_->IsOK()) {
        delete glContext_;
        glContext_ = nullptr;
        return;
    }

    renderTimer_.Bind(wxEVT_TIMER, &wxFFmpegInnerView::OnRenderTimer, this);
    movie_ = new Movie;
}
#else
wxFFmpegInnerView::wxFFmpegInnerView(wxWindow *parent,
                                     wxWindowID winid,
                                     const int *attribList,
                                     const wxPoint &pos,
                                     const wxSize &size,
                                     long style,
                                     const wxString &name,
                                     const wxPalette &palette)
    : wxGLCanvas(parent, winid, attribList, pos, size, style, name, palette) {
    glContext_ = new wxGLContext(this);

    renderTimer_.Bind(wxEVT_TIMER, &wxFFmpegInnerView::OnRenderTimer, this);
    movie_ = new Movie;
}
#endif

wxFFmpegInnerView::~wxFFmpegInnerView() {
    if (glContext_) {
        SetCurrent(*glContext_);
    }

    if (movie_) {
        movie_->close();
        delete movie_;
    }

    if (glManager_) {
        delete glManager_;
    }

    if (glContext_) {
        delete glContext_;
    }
}

void wxFFmpegInnerView::mute()
{
    movie_->mute();
}

void wxFFmpegInnerView::plus()
{
    movie_->plus();
}

void wxFFmpegInnerView::minus()
{
    movie_->minus();
}

void wxFFmpegInnerView::open(std::string filename) {
    movie_->open(filename);
    pts_ = std::numeric_limits<int64_t>::min();
    renderTimer_.Start(10);
}

void wxFFmpegInnerView::start() {
    movie_->start();
    pts_ = std::numeric_limits<int64_t>::min();
    renderTimer_.Start(10);
}

void wxFFmpegInnerView::stop() {
    movie_->stop();
    renderTimer_.Stop();
}

void wxFFmpegInnerView::play() {
    movie_->play();
}

void wxFFmpegInnerView::pause() {
    movie_->pause();
}

void wxFFmpegInnerView::close() {
    movie_->close();
    renderTimer_.Stop();
}

void wxFFmpegInnerView::OnPaint(wxPaintEvent &event) {
    wxPaintDC dc(this);

    if (!glManager_ || !glManager_->ok()) {
        return;
    }
}

void wxFFmpegInnerView::OnSize(wxSizeEvent &event) {
    event.Skip();

    if (!IsShownOnScreen()) {
        return;
    }

    if (!glContext_) {
        return;
    }

    SetCurrent(*glContext_);

    if (!glManager_) {
        glManager_ = new GLManager;
    }

    if (!glManager_->ok()) {
        return;
    }

    wxSize size = event.GetSize() * GetContentScaleFactor();
    glManager_->setViewport(0, 0, size.x, size.y);
}

void wxFFmpegInnerView::OnRenderTimer(wxTimerEvent &event) {
    auto [frame, pts] = movie_->currentFrame();
    if (frame && pts_ != pts) {
        pts_ = pts;
        SetCurrent(*glContext_);
        glManager_->draw(frame->width,
                         frame->height,
                         frame->data,
                         frame->linesize);
        SwapBuffers();
    }
}

wxFFmpegView::wxFFmpegView(wxWindow *parent,
                           wxWindowID winid,
                           const wxPoint &pos,
                           const wxSize &size,
                           long style,
                           const wxString &name)
    : wxPanel(parent, winid, pos, size, style, name) {
#if wxCHECK_VERSION(3, 1, 5)
    wxGLAttributes attrs;
    attrs.PlatformDefaults().Defaults().EndList();
    innerView_ = new wxFFmpegInnerView(this, attrs);
#else
    int attribList[] = {
            WX_GL_CORE_PROFILE,
            WX_GL_MAJOR_VERSION,
            3,
            WX_GL_MINOR_VERSION,
            3,
            WX_GL_DOUBLEBUFFER,
            0,
    };
    innerView_ = new wxFFmpegInnerView(this, wxID_ANY, attribList);
#endif

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(innerView_, 1, wxEXPAND);
    SetSizer(sizer);
}

wxFFmpegView::~wxFFmpegView() {
}

void wxFFmpegView::mute()
{
    innerView_->mute();
}

void wxFFmpegView::plus()
{
    innerView_->plus();
}

void wxFFmpegView::minus()
{
    innerView_->minus();
}

void wxFFmpegView::open(std::string filename) {
    innerView_->open(filename);
}

void wxFFmpegView::close() {
    innerView_->close();
}

void wxFFmpegView::pause()
{
    innerView_->pause();
}

void wxFFmpegView::play()
{
    innerView_->play();
}

void wxFFmpegView::stop()
{
    innerView_->stop();
}

void wxFFmpegView::start()
{
    innerView_->start();
}