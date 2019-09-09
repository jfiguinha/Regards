/////////////////////////////////////////////////////////////////////////////
// Name:        twainmsw.h
// Author:      Derry Bryson
// Created:     1/8/2003
// Copyright:   2003 (c) Derry Bryson
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////
#ifndef __WXTWAINMSW_H
#define __WXTWAINMSW_H

#if defined(__GNUG__) && !defined(__APPLE__)
    #pragma interface "twainmsw.h"
#endif

#include "wx/ia/twain.h"
#include "wx/ia/twainbase.h"

class wxTwainMSW;

class wxTwainMSWClient : public wxTwainClient
{
public:
    wxTwainMSWClient(wxTwainMSW *twain);
    virtual ~wxTwainMSWClient();

    virtual void SetTwain(wxTwainMSW *twain);
    virtual wxTwainMSW *GetTwain();

private:
    wxTwainMSW *m_twain;
};

class wxTwainMSW : public wxTwainBase
{
public:
    wxTwainMSW(wxTwainMSWClient *client,
        const wxString &appName = wxEmptyString,
        const wxString &appFamily = wxEmptyString,
        const wxString &appManufacturer = wxEmptyString,
        int versionMajor = 0, int versionMinor = 0,
        const wxString &versionInfo = wxEmptyString,
        int language = TWLG_ENGLISH_USA, int country = TWCY_USA);
    virtual ~wxTwainMSW();

    virtual bool Startup();
    virtual void Shutdown();

    virtual bool GetCapability(TW_CAPABILITY &twCap, TW_UINT16 cap,
        TW_UINT16 conType = TWON_DONTCARE16);
    virtual bool GetCapability(TW_UINT16 cap, TW_UINT32 &value);
    virtual bool SetCapability(TW_UINT16 cap, TW_UINT16 value, bool sign = FALSE);
    virtual bool SetCapability(TW_CAPABILITY &twCap);

    virtual HBITMAP GetHBitmap();

protected:
    virtual bool DoMessageLoop(bool showUI = TRUE);
    virtual void DisableSource();

    HINSTANCE m_hTwainDLL;
    TW_HANDLE m_hParent;
    HBITMAP m_hBitmap;
    wxWindowDisabler *m_windowDisabler;

    virtual bool GetImage(TW_IMAGEINFO &info);

private:
    DECLARE_CLASS(wxTwainMSW)
};

#endif // __WXTWAINMSW_H
