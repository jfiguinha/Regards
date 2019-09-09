/////////////////////////////////////////////////////////////////////////////
// Name:        twainbase.cpp
// Purpose:     wxTwainBase
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
    #pragma implementation "twainbase.h"
#endif

#include <header.h>

#ifdef WIN32

#include <wx/ia/twainbase.h>
#include <wx/log.h>

IMPLEMENT_CLASS(wxTwainBase, wxObject)

wxTwainBase::wxTwainBase(wxTwainClient *client, const wxString &appName,
    const wxString &appFamily, const wxString &appManufacturer,
    int versionMajor, int versionMinor, const wxString &versionInfo,
    int language, int country)
{
    Init(client, appName, appFamily, appManufacturer, versionMajor,
        versionMinor, versionInfo, language, country);
}

wxTwainBase::~wxTwainBase()
{
    wxLogDebug("wxTwainBase::~wxTwainBase()");
    //  Can't call ShutDown() here as we would like because it is virtual.
    //  NOTE: ShutDown must be called for this destructor!
    wxASSERT_MSG(!m_sourceIsOpen, _("TWAIN source is still open, Shutdown() not called!"));
    wxASSERT_MSG(!m_sourceManagerIsOpen, _("TWAIN source manager is still open, Shutdown() not called!"));
    wxASSERT_MSG(!m_ok, _("TWAIN is still open, Shutdown() not called!"));

    DeleteAvailableSources();
}

void wxTwainBase::Init(wxTwainClient *client, const wxString &appName,
    const wxString &appFamily, const wxString &appManufacturer,
    int versionMajor, int versionMinor, const wxString &versionInfo,
    int language, int country)
{
    m_client = client;

    m_DSMEntryProc = NULL;

    memset(&m_appId, 0, sizeof(m_appId));
    memset(&m_sourceId, 0, sizeof(m_sourceId));

    m_appId.Id = 0;
    m_appId.Version.MajorNum = versionMajor;
    m_appId.Version.MinorNum = versionMinor;
    m_appId.Version.Language = language;
    m_appId.Version.Country = country;
    SetStringValue(m_appId.Version.Info, versionInfo, 32);
    m_appId.ProtocolMajor = 1; //TWON_PROTOCOLMAJOR;
    m_appId.ProtocolMinor = 7; //TWON_PROTOCOLMINOR;
    m_appId.SupportedGroups = DG_IMAGE | DG_CONTROL;
    SetStringValue(m_appId.ProductName, appName, 32);
    SetStringValue(m_appId.ProductFamily, appFamily, 32);
    SetStringValue(m_appId.Manufacturer, appManufacturer, 32);

    m_sourceManagerIsOpen = FALSE;
    m_sourceIsOpen = FALSE;
    m_ok = FALSE;
    m_sourceSelected = FALSE;
    m_sourceEnabled = FALSE;
    m_modalUI = FALSE;

    m_hParentPtr = 0;
}

void wxTwainBase::SetClient(wxTwainClient *client)
{
    m_client = client;
}

wxTwainClient *wxTwainBase::GetClient()
{
    return m_client;
}

bool wxTwainBase::Ok()
{
    return m_ok;
}

TW_STATUS wxTwainBase::GetStatus()
{
    return m_status;
}

TW_INT16 wxTwainBase::GetReturnCode()
{
    return m_returnCode;
}

bool wxTwainBase::OpenSourceManager()
{
    wxLogDebug("wxTwainBase::OpenSourceManager()");
    if (Ok() && !IsSourceManagerOpen())
    {
        wxLogDebug("calling DSMEntryProc to open source manager, m_hParent = %p", m_hParentPtr);
        m_sourceManagerIsOpen = CallDSMEntryProc(&m_appId, NULL, DG_CONTROL,
            DAT_PARENT, MSG_OPENDSM, (TW_MEMREF)m_hParentPtr);

        if (m_sourceManagerIsOpen)
            GetAvailableSources();
    }
    wxLogDebug("m_sourceManagerIsOpen = %d", m_sourceManagerIsOpen);
    return m_sourceManagerIsOpen;
}

void wxTwainBase::CloseSourceManager()
{
    if (Ok() && IsSourceManagerOpen())
    {
        CallDSMEntryProc(&m_appId, NULL, DG_CONTROL, DAT_PARENT, MSG_CLOSEDSM,
            (TW_MEMREF)m_hParentPtr);
        m_sourceManagerIsOpen = FALSE;
    }
}

bool wxTwainBase::IsSourceManagerOpen()
{
    return m_sourceManagerIsOpen;
}

int wxTwainBase::GetSourceCount()
{
    if (IsSourceManagerOpen())
        return m_sources.GetCount();
    return 0;
}

const TW_IDENTITY* wxTwainBase::GetSourceIdentity(int i)
{
    if (IsSourceManagerOpen() && i < (int)m_sources.GetCount())
        return m_sources[i];
    return NULL;
}

bool wxTwainBase::OpenSource()
{
    if (Ok() && IsSourceManagerOpen())
    {
        if (!IsSourceSelected())
            SelectDefaultSource();
        m_sourceIsOpen = CallDSMEntryProc(&m_appId, NULL, DG_CONTROL,
            DAT_IDENTITY, MSG_OPENDS, (TW_MEMREF)&m_sourceId);
        return m_sourceIsOpen;
    }
    return FALSE;
}

void wxTwainBase::CloseSource()
{
    if (Ok() && IsSourceManagerOpen() && IsSourceOpen())
    {
        DisableSource();
        CallDSMEntryProc(&m_appId, NULL, DG_CONTROL, DAT_IDENTITY, MSG_CLOSEDS,
            (TW_MEMREF)&m_sourceId);
        m_sourceIsOpen = FALSE;
    }
}

bool wxTwainBase::IsSourceOpen()
{
    return m_sourceIsOpen;
}

void wxTwainBase::SetAppLanguage(int language)
{
    m_appId.Version.Language = language;
}

int wxTwainBase::GetAppLanguage()
{
    return m_appId.Version.Language;
}

void wxTwainBase::SetAppCountry(int country)
{
    m_appId.Version.Country = country;
}

int wxTwainBase::GetAppCountry()
{
    return m_appId.Version.Country;
}

void wxTwainBase::SetAppVersion(int major, int minor, const wxString &versionInfo)
{
    m_appId.Version.MajorNum = major;
    m_appId.Version.MinorNum = minor;
    SetStringValue(m_appId.Version.Info, versionInfo, 32);
}

void wxTwainBase::GetAppVersion(int* major, int* minor, wxString* versionInfo)
{
    if (major)
        *major = m_appId.Version.MajorNum;
    if (minor)
        *minor = m_appId.Version.MinorNum;
    if (versionInfo)
        *versionInfo = GetStringValue(m_appId.Version.Info);
}

void wxTwainBase::SetAppManufacturer(const wxString &manufacturer)
{
    SetStringValue(m_appId.Manufacturer, manufacturer, 32);
}

wxString wxTwainBase::GetAppManufacturer()
{
    return GetStringValue(m_appId.Manufacturer);
}

void wxTwainBase::SetAppProductFamily(const wxString &family)
{
    SetStringValue(m_appId.ProductFamily, family, 32);
}

wxString wxTwainBase::GetProductFamily()
{
    return GetStringValue(m_appId.ProductFamily);
}

void wxTwainBase::SetAppProductName(const wxString &name)
{
    SetStringValue(m_appId.ProductName, name, 32);
}

wxString wxTwainBase::GetAppProductName()
{
    return GetStringValue(m_appId.ProductName);
}

bool wxTwainBase::SelectDefaultSource()
{
    m_sourceSelected = CallDSMEntryProc(&m_appId, NULL, DG_CONTROL,
        DAT_IDENTITY, MSG_GETDEFAULT, &m_sourceId);
    return m_sourceSelected;
}

bool wxTwainBase::UserSelectSource(const wxString &defSource)
{
    memset(&m_sourceId, 0, sizeof(m_sourceId));
    if (!SelectSource(defSource))
        CallDSMEntryProc(&m_appId, NULL, DG_CONTROL, DAT_IDENTITY,
            MSG_GETDEFAULT, &m_sourceId);
    m_sourceSelected = CallDSMEntryProc(&m_appId, NULL, DG_CONTROL,
        DAT_IDENTITY, MSG_USERSELECT, &m_sourceId);
    wxLogDebug("m_sourceSelected = %d", m_sourceSelected);
    return m_sourceSelected;
}

bool wxTwainBase::SelectSource(const wxString &source)
{
    m_sourceSelected = FALSE;
    if (IsSourceManagerOpen())
        for (unsigned int i = 0; i < m_sources.GetCount(); i++)
            if (GetStringValue(m_sources[i]->ProductName) == source)
                return SelectSource(i);

    return FALSE;
}

bool wxTwainBase::SelectSource(int i)
{
    m_sourceSelected = FALSE;

    if (IsSourceManagerOpen() && i < (int)m_sources.GetCount())
    {
        memcpy(&m_sourceId, m_sources[i], sizeof(TW_IDENTITY));
        m_sourceSelected = TRUE;
    }

    return m_sourceSelected;
}

bool wxTwainBase::IsSourceSelected()
{
    return m_sourceSelected;
}

bool wxTwainBase::CallDSMEntryProc(pTW_IDENTITY origin, pTW_IDENTITY dest,
    TW_UINT32 dg, TW_UINT16 dat, TW_UINT16 msg, TW_MEMREF data)
{
    wxCHECK_MSG(Ok(), FALSE, _("TWAIN not started!"));

    if (Ok())
    {
        wxLogDebug("calling DSMEntryProc = %p", m_DSMEntryProc);
        m_returnCode = (*m_DSMEntryProc)(origin, dest, dg, dat, msg, data);
        if(m_returnCode != TWRC_SUCCESS)
            (*m_DSMEntryProc)(origin, dest, DG_CONTROL, DAT_STATUS, MSG_GET,
                &m_status);
    }
    else
        m_returnCode = TWRC_FAILURE;

    return m_returnCode == TWRC_SUCCESS;
}

bool wxTwainBase::DoMessageLoop(bool showUI)
{
    return TRUE;
}

bool wxTwainBase::EnableSource(bool showUI)
{
    wxLogDebug("wxTwainBase::EnableSource");
    if (Ok() && IsSourceManagerOpen() && IsSourceOpen() && !IsSourceEnabled())
    {
        wxLogDebug("wxTwainBase::EnableSource 2");
        TW_USERINTERFACE twUI;
        twUI.ShowUI = showUI;
        twUI.ModalUI = showUI;
        if (m_hParentPtr)
            twUI.hParent = *m_hParentPtr;
        else
            twUI.hParent = 0;
        if (CallDSMEntryProc(&m_appId, &m_sourceId, DG_CONTROL,
            DAT_USERINTERFACE, MSG_ENABLEDS, (TW_MEMREF)&twUI))
        {
            m_sourceEnabled = TRUE;
            m_modalUI = twUI.ModalUI;
        }
        else
        {
            m_sourceEnabled = FALSE;
            m_modalUI = TRUE;
        }
        if (m_sourceEnabled)
            wxLogDebug("Source enabled!");
        else
            wxLogDebug("Source not enabled, rc = %d, status = %d",
                GetReturnCode(), GetStatus().ConditionCode);
        return m_sourceEnabled;
    }
    else
        wxLogDebug("Source or source manager not open!");
    return FALSE;
}

void wxTwainBase::DisableSource()
{
    if (m_sourceEnabled)
    {
        TW_USERINTERFACE twUI;
        CallDSMEntryProc(&m_appId, &m_sourceId, DG_CONTROL,
            DAT_USERINTERFACE, MSG_DISABLEDS, &twUI);
        m_sourceEnabled = FALSE;
    }
}

bool wxTwainBase::IsSourceEnabled()
{
  return m_sourceEnabled;
}

void wxTwainBase::GetSourceVersion(int* major, int *minor, wxString* versionInfo)
{
    if (major)
        *major = m_sourceId.Version.MajorNum;
    if (minor)
        *minor = m_sourceId.Version.MinorNum;
    if (versionInfo)
        *versionInfo = GetStringValue(m_sourceId.Version.Info);
}

wxString wxTwainBase::GetSourceProductName()
{
    return GetStringValue(m_sourceId.ProductName);
}

wxString wxTwainBase::GetSourceProductFamily()
{
    return GetStringValue(m_sourceId.ProductFamily);
}

wxString wxTwainBase::GetSourceManufacturer()
{
    return GetStringValue(m_sourceId.Manufacturer);
}

void wxTwainBase::SetStringValue(char *str, const wxString &value, int maxLen)
{
#ifdef __WXMSW__
    strncpy(str, value.c_str(), maxLen);
#endif
}

wxString wxTwainBase::GetStringValue(const char *str)
{
#ifdef __WXMSW__
    return wxString(str);
#endif
    return wxEmptyString;
}

bool wxTwainBase::SetImageCount(int count)
{
    if (SetCapability(CAP_XFERCOUNT, (TW_UINT16)count, TRUE))
    {
        m_imageCount = count;
        return TRUE;
    }
    else
    {
        if (GetReturnCode() == TWRC_CHECKSTATUS)
        {
            TW_UINT32 curcount;
            if (GetCapability(CAP_XFERCOUNT, curcount))
            {
                if (SetCapability(CAP_XFERCOUNT, curcount))
                {
                    m_imageCount = curcount;
                    return TRUE;
                }
            }
        }
    }
    wxLogDebug("wxTwainBase::SetImageCount failed!");
    return FALSE;
}

void wxTwainBase::TransferImage()
{
    wxLogDebug("TransferImage");
    TW_IMAGEINFO info;
    bool done = FALSE;

    while (!done)
    {
        if (GetImageInfo(info))
        {
            m_imagesAcquired++;
            done = !GetImage(info);
        }
    }

    //
    //  As I read the spec this shouldn't be needed, but without it
    //  the scanners I have tested just continue scanning.
    //
    if (m_imagesAcquired == m_imageCount)
        CloseSource();
}

bool wxTwainBase::EndTransfer()
{
    wxLogDebug("EndTransfer");
    TW_PENDINGXFERS twPend;
    if(CallDSMEntryProc(&m_appId, &m_sourceId, DG_CONTROL, DAT_PENDINGXFERS,
        MSG_ENDXFER, (TW_MEMREF)&twPend))
    {
    wxLogDebug("twPend.Count = %d", twPend.Count);
    #if 0
        if(!twPend.Count)
            CloseSource();
    #endif
            return twPend.Count != 0;
    }
    return FALSE;
}

void wxTwainBase::CancelTransfer()
{
    wxLogDebug("CancelTransfer");
    TW_PENDINGXFERS twPend;

    CallDSMEntryProc(&m_appId, &m_sourceId, DG_CONTROL, DAT_PENDINGXFERS,
        MSG_RESET, (TW_MEMREF)&twPend);
}

bool wxTwainBase::GetImageInfo(TW_IMAGEINFO& info)
{
    if (IsSourceEnabled())
        return CallDSMEntryProc(&m_appId, &m_sourceId, DG_IMAGE,
            DAT_IMAGEINFO, MSG_GET, (TW_MEMREF)&info);

    return FALSE;
}

bool wxTwainBase::AcquireImage(bool showUI)
{
    wxLogDebug("wxTwainBase::AcquireImage");
    return AcquireImages(1, showUI);
}

bool wxTwainBase::AcquireImages(int numImages, bool showUI)
{
    wxLogDebug("wxTwainBase::AcquireImages(%d, %d)", numImages, showUI);
    if (IsSourceOpen() || OpenSource())
    {
        if (SetImageCount(numImages))
        {
            m_imagesAcquired = 0;
            if (m_client && !m_client->UpdateStatus(_("Acquiring image(s)"),
                m_imagesAcquired, m_imageCount))
                return FALSE;

            if(EnableSource())
            {
                wxLogDebug("Acquiring Image");
                return DoMessageLoop(showUI);
            }
        }
    }
    return FALSE;
}

bool wxTwainBase::GetAvailableSources()
{
    //
    //  Get the available sources.  Note, the source manager seems to use
    //  the data in the id structure to figure out which source is next
    //  so you must keep the last value in the id structure or else you
    //  will get the right number of sources, but duplicate data.
    //
    TW_IDENTITY id;

    if (CallDSMEntryProc(&m_appId, NULL, DG_CONTROL, DAT_IDENTITY,
        MSG_GETFIRST, (TW_MEMREF)&id))
    {
        TW_IDENTITY *newId = new TW_IDENTITY;
        memcpy(newId, &id, sizeof(TW_IDENTITY));
        m_sources.Add(newId);
        while(CallDSMEntryProc(&m_appId, NULL, DG_CONTROL, DAT_IDENTITY,
            MSG_GETNEXT, (TW_MEMREF)&id))
        {
            newId = new TW_IDENTITY;
            memcpy(newId, &id, sizeof(TW_IDENTITY));
            m_sources.Add(newId);
        }
    }
    else
        return FALSE;

    return TRUE;
}

void wxTwainBase::DeleteAvailableSources()
{
    for(unsigned int i = 0; i < m_sources.GetCount(); i++)
        delete m_sources[i];

    m_sources.Clear();
}

#endif