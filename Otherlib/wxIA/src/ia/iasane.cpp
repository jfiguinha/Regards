/////////////////////////////////////////////////////////////////////////////
// Name:        iasane.cpp
// Purpose:     wxIASaneProvider
// Author:      Derry Bryson, Technology Associates, Inc.
// Modified by: Jan Engelhardt, Michael Langfermann
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


#include <header.h>

#ifdef __WXGTK__

#ifdef __GNUG__
#pragma implementation "iasane.h"
#endif

#include <wx/ia/ia.h>
#include <wx/ia/iasane.h>
#include <wx/ia/sane.h>
#include <wx/ia/iasanedlg.h>

IMPLEMENT_CLASS(wxIASaneProvider, wxIAProvider)

wxIASaneProvider::wxIASaneProvider()
{
    m_saneInit = FALSE;
    m_deviceList = NULL;
    m_numDevices = 0;
    m_selDevice = -1;
    m_saneVersionCode = 0;
    m_sane = new wxSane();

    if (m_sane)
        m_saneInit = m_sane->SaneInit(&m_saneVersionCode, NULL) == SANE_STATUS_GOOD;
}

wxIASaneProvider::~wxIASaneProvider()
{
    if (m_sane)
    {
        if (m_saneInit)
        {
            if (m_sane->IsOpen())
                m_sane->SaneClose();
            m_sane->SaneExit();
        }
    }

    delete m_sane;
}

bool wxIASaneProvider::Ok()
{
    return m_sane && m_saneInit;
}

wxString wxIASaneProvider::GetName()
{
    return _T("SANE");
}

int wxIASaneProvider::GetSourceCount()
{
    wxCHECK_MSG(Ok(), 0, _T("wxIASane not valid!"));

    if (!m_numDevices)
    {
        wxBusyCursor busy;

        wxYield();

        if (m_sane->SaneGetDevices(&m_deviceList, FALSE) == SANE_STATUS_GOOD &&
           m_deviceList)
            for (unsigned int i = 0; m_deviceList[i]; i++)
                m_numDevices++;
    }

    return m_numDevices;
}

struct typemap
{
    const char* typeName;
    wxIASourceType type;
};

wxIASourceInfo wxIASaneProvider::GetSourceInfo(int i)
{
    static struct typemap
        typeMaps[] =
        {
            { "file scanner", wxIA_SOURCE_FILMSCANNER },
            { "flatbed scanner", wxIA_SOURCE_FLATBEDSCANNER },
            { "frame grabber", wxIA_SOURCE_FRAMEGRABBER },
            { "handheld scanner", wxIA_SOURCE_HANDHELDSCANNER },
            { "multi-function peripheral", wxIA_SOURCE_MULTIFUNCTION },
            { "sheetfed scanner", wxIA_SOURCE_SHEETFEDSCANNER },
            { "still camera", wxIA_SOURCE_STILLCAMERA },
            { "video camera", wxIA_SOURCE_VIDEOCAMERA },
            { "virtual device", wxIA_SOURCE_UNKNOWN },
            { NULL, wxIA_SOURCE_UNKNOWN }
        };

    wxCHECK_MSG(Ok(), wxNullIASourceInfo, _T("wxIASane not valid!"));

    if (!m_deviceList)
        GetSourceCount();

    if (i < m_numDevices)
    {
        wxIASourceInfo sourceInfo;

        sourceInfo.SetName(m_deviceList[i]->name);
        sourceInfo.SetModel(m_deviceList[i]->model);
        sourceInfo.SetVendor(m_deviceList[i]->vendor);
        sourceInfo.SetType(wxIA_SOURCE_UNKNOWN);

        for (unsigned int j = 0; typeMaps[j].typeName; j++)
            if (!wxStrcmp(m_deviceList[i]->type, typeMaps[j].typeName))
            {
                sourceInfo.SetType(typeMaps[j].type);
                break;
            }

       return sourceInfo;
    }

    return wxNullIASourceInfo;
}

wxIAReturnCode wxIASaneProvider::SelectSource(const wxString &name,
    wxIAUIMode uiMode, wxWindow *parent)
{
    wxCHECK_MSG(Ok(), wxIA_RC_NOTINITIALIZED, _T("wxIASane not valid!"));

    wxString selName = name;
    m_selDevice = -1;

    if (!m_deviceList)
        GetSourceCount();

    if (m_numDevices <= 0) {
        wxMessageBox(_("No devices available"), wxTheApp->GetAppName(),
            wxOK | wxICON_EXCLAMATION | wxCENTRE);
            return wxIA_RC_NOSOURCE;
    }

    wxArrayString sources;
    sources.Alloc(m_numDevices);

    unsigned int sel = 0;
    for (unsigned int i = 0; i < m_numDevices; i++)
    {
        sources.Add(wxString::Format("%s %s", m_deviceList[i]->vendor, m_deviceList[i]->model));
        if (name == wxString(m_deviceList[i]->name).Strip())
            sel = i;
    }

    if (uiMode == wxIA_UIMODE_NORMAL)
    {
        wxSingleChoiceDialog d(parent, _("Available Devices:"),
            _("Select Source"), sources);
        d.SetSelection(sel);

        if(d.ShowModal() != wxID_OK)
            return wxIA_RC_CANCELLED;
        sel = d.GetSelection();
        selName = wxString(m_deviceList[sel]->name).Strip();
   }
    else if(!(sel <= m_numDevices))
        return wxIA_RC_NOSOURCE;

    SANE_Status rc = m_sane->SaneOpen(selName);

    if(rc == SANE_STATUS_GOOD)
        m_selDevice = sel;

    return MapStatus(rc);
}

wxIAReturnCode wxIASaneProvider::SelectDefaultSource()
{
    wxCHECK_MSG(Ok(), wxIA_RC_NOTINITIALIZED, _T("wxIASane not valid!"));
    return wxIA_RC_NOTSUPPORTED;
}

bool wxIASaneProvider::IsSourceSelected()
{
    wxCHECK_MSG(Ok(), FALSE, _T("wxIASane not valid!"));
    return m_sane->IsOpen();
}

wxIASourceInfo wxIASaneProvider::GetSelSourceInfo()
{
    wxCHECK_MSG(Ok(), wxNullIASourceInfo, _T("wxIASane not valid!"));
    return GetSourceInfo(m_selDevice);
}

wxIAReturnCode wxIASaneProvider::SetSelSourceConfig(const wxString &configString)
{
    wxCHECK_MSG(Ok(), wxIA_RC_NOTINITIALIZED, _T("wxIASane not valid!"));
    return wxIA_RC_NOTSUPPORTED;
}

wxString wxIASaneProvider::GetSelSourceConfig()
{
    wxCHECK_MSG(Ok(), wxEmptyString, _T("wxIASane not valid!"));
    return wxEmptyString;
}

wxIAReturnCode wxIASaneProvider::AcquireImages(int numImages, wxIAUIMode uiMode,
    wxWindow *parent, wxIAMonitor *mon)
{
    wxCHECK_MSG(Ok(), wxIA_RC_NOTINITIALIZED, _T("wxIASane not valid!"));

    if (uiMode == wxIA_UIMODE_NORMAL)
    {
        wxIASaneAcquireDialog d(parent, -1, _("Acquire"), m_sane);

        if (d.ShowModal() == wxID_OK)
            ;
    }

    return wxIA_RC_NOTSUPPORTED;
}

wxImage wxIASaneProvider::GetImage()
{
    wxCHECK_MSG(Ok(), wxNullImage, _T("wxIASane not valid!"));
    return wxNullImage;
}

wxBitmap wxIASaneProvider::GetBitmap()
{
    wxCHECK_MSG(Ok(), wxNullBitmap, _T("wxIASane not valid!"));
    return wxNullBitmap;
}

wxIAReturnCode wxIASaneProvider::MapStatus(SANE_Status status)
{
    switch(status)
    {
        case SANE_STATUS_GOOD :
            return wxIA_RC_SUCCESS;

        case SANE_STATUS_UNSUPPORTED :
            return wxIA_RC_NOTSUPPORTED;

        case SANE_STATUS_CANCELLED :
            return wxIA_RC_CANCELLED;

        case SANE_STATUS_DEVICE_BUSY :
            return wxIA_RC_DEVICEBUSY;

        case SANE_STATUS_INVAL :
            return wxIA_RC_INVAL;

        case SANE_STATUS_JAMMED :
            return wxIA_RC_JAMMED;

        case SANE_STATUS_NO_DOCS :
            return wxIA_RC_NODOCS;

        case SANE_STATUS_COVER_OPEN :
            return wxIA_RC_COVEROPEN;

        case SANE_STATUS_IO_ERROR :
            return wxIA_RC_IOERROR;

        case SANE_STATUS_NO_MEM :
            return wxIA_RC_NOMEM;

        case SANE_STATUS_ACCESS_DENIED :
            return wxIA_RC_ACCESSDENIED;

        default :
            return wxIA_RC_UNKNOWNERROR;
    }
}

#endif