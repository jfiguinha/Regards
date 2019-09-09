/////////////////////////////////////////////////////////////////////////////
// Name:        ia.cpp
// Purpose:     wxIA - Image Acquisition Classes
// Author:      Derry Bryson, Technology Associates, Inc.
// Modified by: Michael Langfermann
// Created:     01/23/2003
// Copyright:   (c) 2003 Derry Bryson
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "ia.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#if defined(__BORLANDC__)
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/image.h>
#include <wx/ia/ia.h>

#ifdef __WXMSW__
    #include <wx/ia/twainbase.h>
    #include <wx/ia/twainmsw.h>
    #include <wx/ia/iatwainmsw.h>
#endif

#ifdef __WXGTK__
    #include <wx/ia/sane.h>
    #include <wx/ia/iasane.h>
#endif

//
//  wxIASourceInfo - Descriptive information about a source.
//
wxIASourceInfo::wxIASourceInfo()
{
    m_type = wxIA_SOURCE_UNKNOWN;
}

wxIASourceInfo::wxIASourceInfo(const wxString &name, const wxString &model,
    const wxString &vendor, wxIASourceType type)
{
    m_name = name;
    m_model = model;
    m_vendor = vendor;
    m_type = type;
}

void wxIASourceInfo::SetName(const wxString &name)
{
    m_name = name;
}

wxString wxIASourceInfo::GetName() const
{
    return m_name;
}

void wxIASourceInfo::SetModel(const wxString &model)
{
    m_model = model;
}

wxString wxIASourceInfo::GetModel() const
{
    return m_model;
}

void wxIASourceInfo::SetVendor(const wxString &vendor)
{
    m_vendor = vendor;
}

wxString wxIASourceInfo::GetVendor() const
{
    return m_vendor;
}

void wxIASourceInfo::SetType(wxIASourceType type)
{
    m_type = type;
}

wxIASourceType wxIASourceInfo::GetType() const
{
    return m_type;
}

wxIASourceInfo wxNullIASourceInfo;

IMPLEMENT_CLASS(wxIAProvider, wxObject)

wxIAProvider::wxIAProvider()
{
    m_evtHandler = NULL;
    m_transferMode = wxIA_TRANSFERMODE_NATIVE;
    m_transferFileType = wxIA_FILETYPE_TIFF;
}

wxIAProvider::~wxIAProvider()
{
}

bool wxIAProvider::Ok()
{
    return FALSE;
}

wxString wxIAProvider::GetName()
{
    return wxEmptyString;
}

int wxIAProvider::GetSourceCount()
{
    return 0;
}

wxIASourceInfo wxIAProvider::GetSourceInfo(int i)
{
    return wxNullIASourceInfo;
}

wxIAReturnCode wxIAProvider::SelectSource(const wxString &name,
    wxIAUIMode uiMode, wxWindow *parent)
{
    return wxIA_RC_NOTSUPPORTED;
}

wxIAReturnCode wxIAProvider::SelectDefaultSource()
{
    return wxIA_RC_NOTSUPPORTED;
}

bool wxIAProvider::IsSourceSelected()
{
    return FALSE;
}

wxIASourceInfo wxIAProvider::GetSelSourceInfo()
{
    return wxNullIASourceInfo;
}

wxIAReturnCode wxIAProvider::SetSelSourceConfig(const wxString &configString)
{
    return wxIA_RC_NOTSUPPORTED;
}

wxString wxIAProvider::GetSelSourceConfig()
{
    return wxEmptyString;
}

wxIAReturnCode wxIAProvider::AcquireImage(wxIAUIMode uiMode, wxWindow *parent,
                           wxIAMonitor *mon)
{
    return AcquireImages(1, uiMode, parent, mon);
}

wxIAReturnCode wxIAProvider::AcquireImages(int numImages, wxIAUIMode uiMode,
    wxWindow *parent, wxIAMonitor* mon)
{
    return wxIA_RC_NOTSUPPORTED;
}

void wxIAProvider::SetEvtHandler(wxEvtHandler *evtHandler)
{
    m_evtHandler = evtHandler;
}

wxEvtHandler *wxIAProvider::GetEvtHandler()
{
    return m_evtHandler;
}

wxImage wxIAProvider::GetImage()
{
    return wxNullImage;
}

wxBitmap wxIAProvider::GetBitmap()
{
    return wxNullBitmap;
}

bool wxIAProvider::SaveSettings(wxConfigBase *config)
{
    return TRUE;
}

bool wxIAProvider::LoadSettings(wxConfigBase *config)
{
    return TRUE;
}

wxIAReturnCode wxIAProvider::SetTransferMode(wxIATransferMode mode)
{
    m_transferMode = mode;
    return wxIA_RC_SUCCESS;
}

wxIATransferMode wxIAProvider::GetTransferMode()
{
    return m_transferMode;
}

wxIAReturnCode wxIAProvider::SetTransferFilename(const wxString &filename,
    wxIATransferFileType type)
{
    m_transferFilename = filename;
    m_transferFileType = type;
    return wxIA_RC_SUCCESS;
}

wxString wxIAProvider::GetTransferFilename()
{
    return m_transferFilename;
}

wxIATransferFileType wxIAProvider::GetTransferFileType()
{
    return m_transferFileType;
}

wxIAReturnCode wxIAProvider::SetTransferBuffer(void *buffer, size_t size)
{
    m_transferBuffer = buffer;
    m_transferBufferSize = size;
    return wxIA_RC_SUCCESS;
}

void *wxIAProvider::GetTransferBuffer()
{
    return m_transferBuffer;
}

size_t wxIAProvider::GetTransferBufferSize()
{
    return m_transferBufferSize;
}

IMPLEMENT_CLASS(wxIAManager, wxObject)

bool wxIAManager::s_init = FALSE;

wxIAProvider *wxIAManager::s_defaultProvider = NULL;

wxIAManager &wxIAManager::Get()
{
    static wxIAManager manager;

    if(!s_init)
    {
#ifdef __WXMSW__
        manager.AddProvider(s_defaultProvider = new wxIATwainMSWProvider());
#endif

#ifdef __WXGTK__
        manager.AddProvider(s_defaultProvider = new wxIASaneProvider());
#endif
        s_init = TRUE;
    }

    return manager;
}

void wxIAManager::AddProvider(wxIAProvider *provider)
{
    m_providers.Add(provider);
}

wxIAProviderPtrArray &wxIAManager::GetProviders()
{
    return m_providers;
}

wxIAProvider *wxIAManager::GetDefaultProvider()
{
    return s_defaultProvider;
}

wxString wxIAManager::GetReturnCodeDesc(wxIAReturnCode rc)
{
    static wxString
        desc[] =
        {
          _("Success"),
          _("Unknown error"),
          _("Operation not supported"),
          _("Operation cancelled by user"),
          _("Device not ready"),
          _("Device busy"),
          _("Invalid data or argument"),
          _("Document feeder jammed"),
          _("Document feeder out of documents"),
          _("Scanner cover is open"),
          _("Error during device I/O"),
          _("Out of memory"),
          _("Access denied"),
          _("No source selected"),
          _("Not initialized")
        };

    if(rc >= 0 && rc < (int)wxIA_RC_NOTINITIALIZED)
      return(desc[(int)rc]);

    return wxEmptyString;
}

wxIAManager::wxIAManager()
{
}

wxIAManager::~wxIAManager()
{
    for(unsigned int i = 0; i < m_providers.GetCount(); i++)
        delete m_providers[i];
}

DEFINE_EVENT_TYPE(wxEVT_IA_GETIMAGE)
DEFINE_EVENT_TYPE(wxEVT_IA_UPDATE)

wxIAEvent::wxIAEvent(wxEventType type, wxIAProvider *provider) :
    wxEvent(-1, type)
{
    m_provider = provider;
    m_abort = FALSE;
}

wxIAEvent::wxIAEvent(wxEventType type, wxIAProvider *provider,
    const wxString &text, size_t quantum, size_t span) :
    wxEvent(-1, type)
{
    m_provider = provider;
    m_text = text;
    m_quantum = quantum;
    m_span = span;
    m_abort = FALSE;
}

wxIAEvent::wxIAEvent(const wxIAEvent &event) :
    wxEvent(event)
{
    m_provider = event.m_provider;
    m_text = event.m_text;
    m_quantum = event.m_quantum;
    m_span = event.m_span;
    m_abort = event.m_abort;
}
