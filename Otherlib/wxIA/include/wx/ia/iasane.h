/////////////////////////////////////////////////////////////////////////////
// Name:        iasane.h
// Purpose:     wxIASaneProvider
// Author:      Derry Bryson, Technology Associates, Inc.
// Modified by:
// Created:     01/24/2003
// RCS-ID:      $Id: iasane.h,v 1.2 2003/04/03 03:53:40 dbryson Exp $
// Copyright:   (c) 2003 Derry Bryson
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_IASANE_H_
#define _WX_IASANE_H_

#ifdef __GNUG__
#pragma interface "iasane.h"
#endif

#include "wx/ia/ia.h"
#include "wx/ia/sane.h"

class wxIASaneProvider : public wxIAProvider
{
public:
    wxIASaneProvider();
    virtual ~wxIASaneProvider();

    virtual bool Ok();
    virtual wxString GetName();
    virtual int GetSourceCount();
    virtual wxIASourceInfo GetSourceInfo(int i);
    virtual wxIAReturnCode SelectSource(const wxString &name = wxEmptyString,
        wxIAUIMode uiMode = wxIA_UIMODE_NORMAL, wxWindow *parent = NULL);
    virtual wxIAReturnCode SelectDefaultSource();
    virtual bool IsSourceSelected();
    virtual wxIASourceInfo GetSelSourceInfo();
    virtual wxIAReturnCode SetSelSourceConfig(const wxString &configString);
    virtual wxString GetSelSourceConfig();
    virtual wxIAReturnCode AcquireImages(int numImages,
        wxIAUIMode uiMode = wxIA_UIMODE_NORMAL, wxWindow *parent = NULL,
        wxIAMonitor *mon = NULL);

    virtual wxImage GetImage();
    virtual wxBitmap GetBitmap();

protected:
    wxIAReturnCode MapStatus(SANE_Status);

    wxSane *m_sane;
    const SANE_Device **m_deviceList;
    SANE_Int m_saneVersionCode;
    bool m_saneInit;
    int m_numDevices;
    int m_selDevice;

private:
    DECLARE_CLASS(wxIASaneProvider)
};

#endif // _WX_IASANE_H_
