/////////////////////////////////////////////////////////////////////////////
// Name:        twainmsw.cpp
// Purpose:     wxTwainMSW - MSW version wxTwain
// Author:      Derry Bryson
// Modified by: Jan Engelhardt, Michael Langfermann
// Created:     01/08/2003
// Copyright:   (c) Derry Bryson
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "twainmsw.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#if defined(__BORLANDC__)
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/log.h>
#include <wx/ia/twainbase.h>
#include <wx/ia/twainmsw.h>
#include <wx/msw/private.h>

#define TWAIN_DLL_NAME      _T("TWAIN_32.DLL")

wxTwainMSWClient::wxTwainMSWClient(wxTwainMSW *twain)
{
    m_twain = twain;
}

wxTwainMSWClient::~wxTwainMSWClient()
{
}

void wxTwainMSWClient::SetTwain(wxTwainMSW *twain)
{
    m_twain = twain;
}

wxTwainMSW *wxTwainMSWClient::GetTwain()
{
    return m_twain;
}

IMPLEMENT_CLASS(wxTwainMSW, wxTwainBase);

wxTwainMSW::wxTwainMSW(wxTwainMSWClient *client, const wxString &appName,
    const wxString &appFamily, const wxString &appManufacturer,
    int majorVersion, int minorVersion, const wxString &versionInfo,
    int language, int country) :
    wxTwainBase(client, appName, appFamily, appManufacturer, majorVersion,
        minorVersion, versionInfo, language, country)
{
    m_hTwainDLL = NULL;
    if (wxTheApp->GetTopWindow())
        m_hParent = (TW_HANDLE)wxTheApp->GetTopWindow()->GetHWND();
    else
        m_hParent = 0;
    m_hParentPtr = &m_hParent;
    m_hBitmap = 0;
    m_windowDisabler = NULL;
}

wxTwainMSW::~wxTwainMSW()
{
    wxLogDebug("wxTwainMSW::~wxTwainMSW()");
}

bool wxTwainMSW::Startup()
{
    wxLogDebug("wxTwainMSW::Startup");
    if (!Ok())
    {
        m_hTwainDLL = ::LoadLibrary(TWAIN_DLL_NAME);
        if (m_hTwainDLL)
        {
            wxLogDebug("twain lib loaded");
            m_DSMEntryProc = (DSMENTRYPROC)::GetProcAddress(m_hTwainDLL, MAKEINTRESOURCEA(1));
            if (!m_DSMEntryProc)
            {
                ::FreeLibrary(m_hTwainDLL);
                m_hTwainDLL = NULL;
            }
            else
                m_ok = TRUE;
        }
        else
            wxLogDebug("Unable to load twain lib '%s'", TWAIN_DLL_NAME);
        if (Ok())
            return OpenSourceManager();
    }

    return Ok();
}

void wxTwainMSW::Shutdown()
{
    wxLogDebug("wxTwainMSW::Shutdown()");
    if (!Ok())
        return;
    wxLogDebug("shutting down wxTwainMSW");
    CloseSource();
    CloseSourceManager();
    ::FreeLibrary(m_hTwainDLL);
    m_hTwainDLL = NULL;
    m_DSMEntryProc = NULL;
    m_ok = FALSE;
}

bool wxTwainMSW::GetCapability(TW_CAPABILITY &twCap, TW_UINT16 cap,
    TW_UINT16 conType)
{
    if (IsSourceOpen())
    {
        twCap.Cap = cap;
        twCap.ConType = conType;
        twCap.hContainer = NULL;

        if (CallDSMEntryProc(&m_appId, &m_sourceId, DG_CONTROL, DAT_CAPABILITY,
            MSG_GET, (TW_MEMREF)&twCap))
            return TRUE;
    }
    return FALSE;
}

bool wxTwainMSW::GetCapability(TW_UINT16 cap, TW_UINT32 &value)
{
    TW_CAPABILITY twCap;

    if (GetCapability(twCap, cap))
    {
        pTW_ONEVALUE val = (pTW_ONEVALUE)GlobalLock(twCap.hContainer);
        if (val)
        {
            value = val->Item;
            GlobalUnlock(val);
            GlobalFree(twCap.hContainer);
            return TRUE;
        }
    }
    return FALSE;
}

bool wxTwainMSW::SetCapability(TW_UINT16 cap, TW_UINT16 value, bool sign)
{
    if (IsSourceOpen())
    {
        TW_CAPABILITY twCap;
        pTW_ONEVALUE val;
        bool retval = FALSE;

        twCap.Cap = cap;
        twCap.ConType = TWON_ONEVALUE;
        twCap.hContainer = GlobalAlloc(GHND, sizeof(TW_ONEVALUE));
        if (twCap.hContainer)
        {
            val = (pTW_ONEVALUE)GlobalLock(twCap.hContainer);
            val->ItemType = sign ? TWTY_INT16 : TWTY_UINT16;
            val->Item = (TW_UINT32)value;
            GlobalUnlock(twCap.hContainer);
            retval = SetCapability(twCap);
            GlobalFree(twCap.hContainer);
        }
        return retval;
    }
    return FALSE;
}

bool wxTwainMSW::SetCapability(TW_CAPABILITY &twCap)
{
    if(IsSourceOpen())
        return CallDSMEntryProc(&m_appId, &m_sourceId, DG_CONTROL,
            DAT_CAPABILITY, MSG_SET, (TW_MEMREF)&twCap);

    return FALSE;
}

HBITMAP wxTwainMSW::GetHBitmap()
{
    return m_hBitmap;
}

bool wxTwainMSW::GetImage(TW_IMAGEINFO &info)
{
    wxLogDebug("GetImage");
    CallDSMEntryProc(&m_appId, &m_sourceId, DG_IMAGE, DAT_IMAGENATIVEXFER,
        MSG_GET, &m_hBitmap);
    switch(GetReturnCode())
    {
        case TWRC_XFERDONE:
            if (m_client)
            {
                wxLogDebug("m_hBitmap = %p", m_hBitmap);
                if (!m_client->HandleImage(info) ||
                    !m_client->UpdateStatus(_("Acquiring image(s)"),
                    m_imagesAcquired, m_imageCount))
                {
                    CancelTransfer();
                    CloseSource();
                    return FALSE;
                }
            }
            ::GlobalFree(m_hBitmap);
            m_hBitmap = 0;
        break;

        case TWRC_CANCEL:
        break;

        case TWRC_FAILURE:
            CancelTransfer();
            return FALSE;
    }
    return EndTransfer();
}

bool wxTwainMSW::DoMessageLoop(bool showUI)
{
    if (showUI)
        m_windowDisabler = new wxWindowDisabler(static_cast<wxWindow *>(NULL));

    MSG msg;

    while (IsSourceEnabled() && ::GetMessage((LPMSG)&msg, NULL, 0, 0))
    {
        wxLogDebug("processing msg");
        TW_EVENT twEvent;

        twEvent.pEvent = (TW_MEMREF)&msg;
        twEvent.TWMessage = MSG_NULL;
        CallDSMEntryProc(&m_appId, &m_sourceId, DG_CONTROL, DAT_EVENT,
            MSG_PROCESSEVENT, (TW_MEMREF)&twEvent);
        if (GetReturnCode() != TWRC_NOTDSEVENT)
        {
            switch(twEvent.TWMessage)
            {
                case MSG_XFERREADY :
                  wxLogDebug("transfering image");
                  TransferImage();
                break;

                case MSG_CLOSEDSREQ :
                  wxLogDebug("close ds req");
                  CloseSource();
                break;

                case MSG_CLOSEDSOK :
                  wxLogDebug("close ds ok");
                break;

                case MSG_NULL :
                  wxLogDebug("msg null");
                break;

                default :
                  wxLogDebug("unknown msg %d", twEvent.TWMessage);
                break;
            }
        }
    }
    wxLogDebug("leaving wxTwainMSW message loop");

    delete m_windowDisabler;
    m_windowDisabler = NULL;

    return m_imageCount == m_imagesAcquired;
}

void wxTwainMSW::DisableSource()
{
    delete m_windowDisabler;
    m_windowDisabler = NULL;
    wxTwainBase::DisableSource();
}
