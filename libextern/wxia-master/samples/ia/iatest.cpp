/////////////////////////////////////////////////////////////////////////////
// Name:        iatest.cpp
// Purpose:     wxIA test
// Author:      Derry Bryson
// Modified by: Michael Langfermann
// Created:     02/13/03
// Copyright:   (c) 2003 Derry Bryson
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include <wx/log.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/image.h>
#include <wx/numdlg.h>
#include <wx/ia/ia.h>

// Define to use IA events :)
#define USE_IA_EVENTS

#define MAX_ZOOM	10.0
#define MIN_ZOOM	0.1

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
};

// Define a simple image viewer window
class ImageWindow : public wxScrolledWindow
{
public:
    ImageWindow(wxWindow *parent, wxWindowID id,
        const wxImage &image = wxNullImage,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxSUNKEN_BORDER | wxWANTS_CHARS);

    void SetImage(const wxImage &image);

    void SetZoomFactor(double factor) { m_zoomFactor = factor; DoSetScrollbars(); }
    double GetZoomFactor() { return m_zoomFactor; }

private:
    void DoSetScrollbars(bool refresh = TRUE);

    // event handlers (these functions should _not_ be virtual)
    void OnEraseBackground(wxEraseEvent& event);
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);

    wxBitmap m_bitmap, m_dbBitmap;
    double m_zoomFactor;
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size,
        long style = wxDEFAULT_FRAME_STYLE);

private:
    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnAcquireImage(wxCommandEvent& event);
    void OnAcquireImages(wxCommandEvent& event);
    void OnSelectSource(wxCommandEvent& event);
#ifdef USE_IA_EVENTS
    void OnGetImage(wxIAEvent& event);
    void OnUpdateStatus(wxIAEvent& event);
#endif
    void OnUpdateUI(wxUpdateUIEvent& event);
    void OnZoomIn(wxCommandEvent& event);
    void OnZoomOut(wxCommandEvent& event);

    ImageWindow *m_imageWin;
    int m_imageCount;
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    ID_ACQUIREIMAGE,
    ID_ACQUIREIMAGES,
    ID_ACQUIREIMAGENOUI,
    ID_ACQUIREIMAGESNOUI,
    ID_SELECTSOURCE,
    ID_ZOOMIN,
    ID_ZOOMOUT,
    ID_PROMPTONGETIMAGE
};

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // create the main application window
    MyFrame *frame = new MyFrame(_("wxIA Image Acquistion Test Program"),
        wxPoint(-1, -1), wxSize(450, 340));

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(TRUE);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned FALSE here, the
    // application would exit immediately.
    return TRUE;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size,
    long style) :
    wxFrame(NULL, wxID_ANY, title, pos, size, style)
{
    // create a menu bar
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_ACQUIREIMAGE, _("&Acquire Image..."), _("Acquire an image"));
    menuFile->Append(ID_ACQUIREIMAGENOUI, _("Acquire Image (no ui)..."), _("Acquire an image without user interface"));
#ifdef USE_IA_EVENTS
    menuFile->Append(ID_ACQUIREIMAGES, _("Acquire Images..."), _("Acquire multiple images"));
    menuFile->Append(ID_ACQUIREIMAGESNOUI, _("Acquire Images (no ui)..."), _("Acquire multiple images without user interface"));
#endif
    menuFile->Append(ID_SELECTSOURCE, _("&Select Source..."), _("Select source"));
#ifdef USE_IA_EVENTS
    menuFile->AppendSeparator();
    menuFile->AppendCheckItem(ID_PROMPTONGETIMAGE, _("Prompt on wxEVT_IA_GETIMAGE event"), _("Display prompt after acquiring an image"));
    menuFile->Check(ID_PROMPTONGETIMAGE, TRUE);
#endif
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, _("E&xit\tAlt-X"), _("Quit this program"));

    wxMenu *menuView = new wxMenu;
    menuView->Append(ID_ZOOMIN, _("Zoom &In"), _("Zoom in"));
    menuView->Append(ID_ZOOMOUT, _("Zoom &Out"), _("Zoom out"));

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, _("&About...\tF1"), _("Show about dialog"));

    // now append the freshly created menu to the menu bar…
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, _("&File"));
    menuBar->Append(menuView, _("&View"));
    menuBar->Append(helpMenu, _("&Help"));

    // … and attach this menu bar to the frame
    SetMenuBar(menuBar);

    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(1);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_imageWin = new ImageWindow(this, wxID_ANY), 1, wxEXPAND);
    SetSizer(sizer);

    // dynamically connect all event handles
    Connect(wxID_EXIT, wxEVT_MENU, wxCommandEventHandler(MyFrame::OnQuit));
    Connect(wxID_ABOUT, wxEVT_MENU, wxCommandEventHandler(MyFrame::OnAbout));
    Connect(ID_ACQUIREIMAGE, wxEVT_MENU, wxCommandEventHandler(MyFrame::OnAcquireImage));
    Connect(ID_ACQUIREIMAGENOUI, wxEVT_MENU, wxCommandEventHandler(MyFrame::OnAcquireImage));
#ifdef USE_IA_EVENTS
    Connect(ID_ACQUIREIMAGES, wxEVT_MENU, wxCommandEventHandler(MyFrame::OnAcquireImages));
    Connect(ID_ACQUIREIMAGESNOUI, wxEVT_MENU, wxCommandEventHandler(MyFrame::OnAcquireImages));
#endif
    Connect(ID_SELECTSOURCE, wxEVT_MENU, wxCommandEventHandler(MyFrame::OnSelectSource));
    Connect(ID_ZOOMIN, wxEVT_MENU, wxCommandEventHandler(MyFrame::OnZoomIn));
    Connect(ID_ZOOMOUT, wxEVT_MENU, wxCommandEventHandler(MyFrame::OnZoomOut));
    Connect(wxID_ANY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MyFrame::OnUpdateUI));
#ifdef USE_IA_EVENTS
    Connect(wxEVT_IA_GETIMAGE, wxIAEventHandler(MyFrame::OnGetImage));
    Connect(wxEVT_IA_UPDATE, wxIAEventHandler(MyFrame::OnUpdateStatus));
#endif

#ifdef USE_IA_EVENTS
    if (wxIAManager::Get().GetDefaultProvider())
        wxIAManager::Get().GetDefaultProvider()->SetEvtHandler(this);
#endif
}

// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg = _("wxIA Image Acquisition Test Program.\n"
        "Written by Derry Bryson");

    wxMessageBox(msg, _T("About"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnSelectSource(wxCommandEvent& WXUNUSED(event))
{
    wxIAProvider *provider = wxIAManager::Get().GetDefaultProvider();
    if (!provider)
        return;

    wxIAReturnCode rc = provider->SelectSource();
    if (rc != wxIA_RC_SUCCESS)
    {
        wxLogError(wxIAManager::Get().GetReturnCodeDesc(rc));
        return;
    }

    wxIASourceInfo source = provider->GetSelSourceInfo();

    wxString msg = wxString::Format(_("Name: %s\nModel: %s\nVendor: %s\nType: %d"),
        source.GetName(), source.GetModel(),
        source.GetVendor(), source.GetType());

    wxMessageBox(msg, _("Selected Source Information"), wxOK, this);
}

void MyFrame::OnAcquireImage(wxCommandEvent& event)
{
    wxIAProvider *provider = wxIAManager::Get().GetDefaultProvider();

    if (!provider || !provider->IsSourceSelected())
        return;
    wxIAUIMode uiMode;
    wxIAReturnCode rc;

    if (event.GetId() == ID_ACQUIREIMAGE)
        uiMode = wxIA_UIMODE_NORMAL;
    else
        uiMode = wxIA_UIMODE_NONE;

    m_imageCount = 1;

    rc = provider->AcquireImage(uiMode, this);

#ifndef USE_IA_EVENTS
    if (rc == wxIA_RC_SUCCESS)
        m_imageWin->SetImage(provider->GetImage());
#endif
    if (rc != wxIA_RC_SUCCESS)
        wxLogError(wxIAManager::Get().GetReturnCodeDesc(rc));
}

#ifdef USE_IA_EVENTS
void MyFrame::OnAcquireImages(wxCommandEvent& event)
{
    wxIAProvider *provider = wxIAManager::Get().GetDefaultProvider();

    if (!provider || !provider->IsSourceSelected())
        return;

    m_imageCount = wxGetNumberFromUser(_("Enter number of images to acquire"),
        _("Count:"), _("Acquire Images"), 1, 0, 10, this);
    if (m_imageCount <= 0)
        return;

    wxIAUIMode uiMode = event.GetId() == ID_ACQUIREIMAGES ?
        wxIA_UIMODE_NORMAL : wxIA_UIMODE_NONE;

    wxIAReturnCode rc = provider->AcquireImages(m_imageCount, uiMode, this);
    if (rc != wxIA_RC_SUCCESS)
        wxLogError(wxIAManager::Get().GetReturnCodeDesc(rc));

}
#endif

void MyFrame::OnUpdateUI(wxUpdateUIEvent& event)
{
    switch(event.GetId())
    {
        case ID_ACQUIREIMAGE :
        case ID_ACQUIREIMAGENOUI :
#ifdef USE_IA_EVENTS
        case ID_ACQUIREIMAGES :
        case ID_ACQUIREIMAGESNOUI :
#endif
            event.Enable(wxIAManager::Get().GetDefaultProvider()  &&
                wxIAManager::Get().GetDefaultProvider()->IsSourceSelected());
        break;

        case ID_SELECTSOURCE :
            event.Enable(wxIAManager::Get().GetDefaultProvider());
        break;

        case ID_ZOOMIN :
            event.Enable(m_imageWin->GetZoomFactor() < MAX_ZOOM);
        break;

        case ID_ZOOMOUT :
            event.Enable(m_imageWin->GetZoomFactor() > MIN_ZOOM);
        break;
    }
}

#ifdef USE_IA_EVENTS
void MyFrame::OnGetImage(wxIAEvent& event)
{
    m_imageCount--;
    m_imageWin->SetImage(event.GetProvider()->GetImage());

    if (GetMenuBar()->IsChecked(ID_PROMPTONGETIMAGE) &&
        m_imageCount && wxMessageBox(_("Got image, continue?"), _("Acquire Image"), wxYES_NO, this) != wxYES)
        event.Abort(TRUE);
}

void MyFrame::OnUpdateStatus(wxIAEvent& event)
{
    SetStatusText(wxString::Format(_("%s %d of %d"), event.GetText(),
        event.GetQuantum(), event.GetSpan()));
}
#endif

void MyFrame::OnZoomIn(wxCommandEvent& WXUNUSED(event))
{
    if (m_imageWin->GetZoomFactor() < MAX_ZOOM)
        m_imageWin->SetZoomFactor(m_imageWin->GetZoomFactor() * 2.0);
}

void MyFrame::OnZoomOut(wxCommandEvent& WXUNUSED(event))
{
    if (m_imageWin->GetZoomFactor() > MIN_ZOOM)
        m_imageWin->SetZoomFactor(m_imageWin->GetZoomFactor() / 2.0);
}

// ----------------------------------------------------------------------------
// image viewer window
// ----------------------------------------------------------------------------
ImageWindow::ImageWindow(wxWindow *parent, wxWindowID id, const wxImage &image,
    const wxPoint &pos, const wxSize &size, long style) :
    wxScrolledWindow(parent, id, pos, size, style)
{
    m_zoomFactor = 1.0;
    if (image.Ok())
    {
        m_bitmap = wxBitmap(image);
        DoSetScrollbars(FALSE);
    }
    SetForegroundColour(*wxWHITE);

    Connect(wxEVT_PAINT, wxPaintEventHandler(ImageWindow::OnPaint));
    Connect(wxEVT_SIZE, wxSizeEventHandler(ImageWindow::OnSize));
    Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(ImageWindow::OnEraseBackground));
}

void ImageWindow::DoSetScrollbars(bool refresh)
{
    int x, y;
    GetViewStart(&x, &y);

    if (m_bitmap.Ok())
        SetScrollbars(1, 1, m_bitmap.GetWidth() * m_zoomFactor,
            m_bitmap.GetHeight() * m_zoomFactor, x, y, TRUE);

    if (refresh)
        Refresh();
}

void ImageWindow::SetImage(const wxImage &image)
{
    m_bitmap = wxNullBitmap;

    if (image.Ok())
        m_bitmap = wxBitmap(image);

    DoSetScrollbars(TRUE);
}

void ImageWindow::OnEraseBackground(wxEraseEvent& event)
{
    // Don't do anything here, we redraw the entire window in OnPaint()
}

void ImageWindow::OnPaint(wxPaintEvent& event)
{
    wxPaintDC pdc(this);
    int x, y, w, h;

    GetClientSize(&w, &h);
    if (!m_dbBitmap.Ok() || m_dbBitmap.GetWidth() != w || m_dbBitmap.GetHeight() != h)
        m_dbBitmap = wxBitmap(w, h);

    if (!m_dbBitmap.Ok())
        return;

    wxMemoryDC dc;
    dc.SelectObject(m_dbBitmap);
    GetViewStart(&x, &y);

    //
    //  Clear the window
    //
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(wxBrush(GetBackgroundColour(), wxSOLID));
    dc.SetBackground(wxBrush(GetBackgroundColour(), wxSOLID));
    dc.DrawRectangle(0, 0, w, h);
    dc.SetBrush(wxBrush(GetForegroundColour(), wxCROSSDIAG_HATCH));
    dc.DrawRectangle(0, 0, w, h);

    if (m_bitmap.Ok())
    {
        wxMemoryDC memdc;
        int bx, by, zw, zh, bw, bh;

        x = (int)(x / m_zoomFactor + 0.5);
        y = (int)(y / m_zoomFactor + 0.5);
        zw = (int)((w / m_zoomFactor) + 0.5);
        zh = (int)((h / m_zoomFactor) + 0.5);
        bw = (int)(m_bitmap.GetWidth() * m_zoomFactor + 0.5);
        bh = (int)(m_bitmap.GetHeight() * m_zoomFactor + 0.5);

        if (m_bitmap.GetWidth() < zw)
        {
            bx = (zw - m_bitmap.GetWidth()) / 2;
            zw = m_bitmap.GetWidth();
        }
        else
            bx = 0;

        if (m_bitmap.GetHeight() < zh)
        {
            by = (zh - m_bitmap.GetHeight()) / 2;
            zh = m_bitmap.GetHeight();
        }
        else
            by = 0;

        memdc.SelectObject(m_bitmap);
        dc.SetUserScale(m_zoomFactor, m_zoomFactor);
        dc.Blit(bx, by, zw + 1, zh + 1, &memdc, x, y);
        memdc.SelectObject(wxNullBitmap);
    }
    dc.SetUserScale(1.0, 1.0);
    pdc.Blit(0, 0, w, h, &dc, 0, 0);
    dc.SelectObject(wxNullBitmap);
}

void ImageWindow::OnSize(wxSizeEvent& event)
{
    DoSetScrollbars();
}
