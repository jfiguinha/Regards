/////////////////////////////////////////////////////////////////////////////
// Name:        iatwainmsw.h
// Purpose:     wxIATwainMSWProvider
// Author:      Derry Bryson, Technology Associates, Inc.
// Modified by:
// Created:     01/24/2003
// RCS-ID:      $Id: iatwainmsw.h,v 1.1.1.1 2003/04/01 20:32:33 dbryson Exp $
// Copyright:   (c) 2003 Derry Bryson
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_IATWAINMSW_H_
#define _WX_IATWAINMSW_H_

#ifdef __GNUG__
#pragma interface "iatwainmsw.h"
#endif

#include "wx/ia/twainmsw.h"

class wxIATwainMSWProvider : public wxIAProvider, wxTwainMSWClient
{
public:
    wxIATwainMSWProvider();
    virtual ~wxIATwainMSWProvider();

    virtual bool Ok();
    virtual wxString GetName();
    virtual int GetSourceCount();
    virtual wxIASourceInfo GetSourceInfo(int i);
    virtual wxIAReturnCode SelectSource(const wxString& name = wxEmptyString,
        wxIAUIMode uiMode = wxIA_UIMODE_NORMAL, wxWindow *parent = NULL);
    virtual wxIAReturnCode SelectDefaultSource();
    virtual bool IsSourceSelected();
    virtual wxIASourceInfo GetSelSourceInfo();
    virtual wxIAReturnCode SetSelSourceConfig(const wxString& configString);
    virtual wxString GetSelSourceConfig();
    virtual wxIAReturnCode AcquireImages(int numImages,
        wxIAUIMode uiMode = wxIA_UIMODE_NORMAL, wxWindow *parent = NULL,
        wxIAMonitor *mon = NULL);

    virtual wxImage GetImage();
    virtual wxBitmap GetBitmap();

    // wxTwainClient methods
    virtual bool HandleImage(TW_IMAGEINFO& info);
    virtual bool UpdateStatus(const wxString &text, size_t quantum, size_t span);

private:
    wxIAReturnCode MapStatus(TW_STATUS status);

    HBITMAP CreateDIB(HANDLE hDib);
    HPALETTE CreateBIPalette (LPBITMAPINFOHEADER lpbi);
    int DibNumColors (void *pv);

    wxIAMonitor *m_monitor;
    wxBitmap m_bitmap;

    DECLARE_CLASS(wxIATwainMSWProvider)
};

#endif // _WX_IATWAINMSW_H_
