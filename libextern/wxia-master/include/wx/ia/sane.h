/////////////////////////////////////////////////////////////////////////////
// Name:        sane.h
// Purpose:     wxSane
// Author:      Derry Bryson
// Modified by:
// Created:     01/08/2003
// RCS-ID:      $Id: sane.h,v 1.1.1.1 2003/04/01 20:32:33 dbryson Exp $
// Copyright:   (c) 2003 Derry Bryson
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_SANE_H_
#define _WX_SANE_H_

#ifdef __GNUG__
#pragma interface "sane.h"
#endif

#include "wx/dynlib.h"
#include "sane/sane.h"

class wxSane : public wxObject
{
public:
    wxSane();
    ~wxSane();

    bool IsOpen();

    SANE_Status GetLastStatus();
    SANE_Handle GetHandle();

    SANE_Status SaneInit(SANE_Int *version_code, SANE_Auth_Callback authorize);
    void SaneExit();
    SANE_Status SaneGetDevices(const SANE_Device *** device_list,
        bool local_only);
    SANE_Status SaneOpen(const wxString &name);
    void SaneClose();
    const SANE_Option_Descriptor *SaneGetOptionDescriptor(SANE_Int option);
    SANE_Status SaneControlOption(SANE_Int option, SANE_Action action,
        void *value = NULL, SANE_Int *info = NULL);
    SANE_Status SaneGetParameters(SANE_Parameters *params);
    SANE_Status SaneStart();
    SANE_Status SaneRead(SANE_Byte *data, SANE_Int max_length, SANE_Int *length);
    void SaneCancel();
    SANE_Status SaneSetIOMode(bool non_blocking);
    SANE_Status SaneGetSelectFD(SANE_Int *fd);
    wxString SaneStrStatus(SANE_Status status);

protected:
    SANE_Handle m_handle;
    SANE_Status m_lastStatus;

private:
    DECLARE_CLASS(wxSane)
};

#endif // _WX_SANE_H_
