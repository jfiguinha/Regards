/////////////////////////////////////////////////////////////////////////////
// Name:        sane.cpp
// Purpose:     wxSane
// Author:      Derry Bryson
// Modified by: Jan Engelhardt, Michael Langfermann
// Created:     01/08/2003
// Copyright:   (c) 2003 Derry Bryson
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#include <header.h>

// For compilers that support precompilation, includes "wx/wx.h".
#ifdef __WXGTK__

#ifdef __GNUG__
    #pragma implementation "sane.h"
#endif
#include <wx/ia/sane.h>

IMPLEMENT_CLASS(wxSane, wxObject)

wxSane::wxSane()
{
    m_handle = 0;
    m_lastStatus = SANE_STATUS_GOOD;
}

wxSane::~wxSane()
{
    wxASSERT_MSG(!m_handle, _("wxSane not shutdown properly!"));
}

bool wxSane::IsOpen()
{
    return m_handle != 0;
}

SANE_Handle wxSane::GetHandle()
{
    return m_handle;
}

SANE_Status wxSane::GetLastStatus()
{
    return m_lastStatus;
}

SANE_Status wxSane::SaneInit(SANE_Int *version_code,
    SANE_Auth_Callback authorize)
{
    return m_lastStatus = sane_init(version_code, authorize);
}

void wxSane::SaneExit()
{
    sane_exit();
}

SANE_Status wxSane::SaneGetDevices(const SANE_Device ***device_list,
    bool local_only)
{
    return m_lastStatus = sane_get_devices(device_list, (SANE_Bool)local_only);
}

SANE_Status wxSane::SaneOpen(const wxString &name)
{
    if (m_handle)
        SaneClose();
    m_lastStatus = sane_open(name.ToUTF8(), &m_handle);
    if (m_lastStatus != SANE_STATUS_GOOD)
        m_handle = 0;
    return m_lastStatus;
}

void wxSane::SaneClose()
{
    sane_close(m_handle);
    m_handle = 0;
}

const SANE_Option_Descriptor *wxSane::SaneGetOptionDescriptor(SANE_Int option)
{
    return sane_get_option_descriptor(m_handle, option);
}

SANE_Status wxSane::SaneControlOption(SANE_Int option, SANE_Action action,
    void *value, SANE_Int *info)
{
    return m_lastStatus = sane_control_option(m_handle, option, action, value, info);
}

SANE_Status wxSane::SaneGetParameters(SANE_Parameters *params)
{
    return m_lastStatus = sane_get_parameters(m_handle, params);
}

SANE_Status wxSane::SaneStart()
{
    return m_lastStatus = sane_start(m_handle);
}

SANE_Status wxSane::SaneRead(SANE_Byte *data, SANE_Int max_length, SANE_Int *length)
{
    return m_lastStatus = sane_read(m_handle, data, max_length, length);
}

void wxSane::SaneCancel()
{
    sane_cancel(m_handle);
}

SANE_Status wxSane::SaneSetIOMode(bool non_blocking)
{
    return m_lastStatus = sane_set_io_mode(m_handle, (SANE_Bool)non_blocking);
}

SANE_Status wxSane::SaneGetSelectFD(SANE_Int *fd)
{
    return m_lastStatus = sane_get_select_fd(m_handle, fd);
}

wxString wxSane::SaneStrStatus(SANE_Status status)
{
    return wxString(sane_strstatus(status));
}

#endif