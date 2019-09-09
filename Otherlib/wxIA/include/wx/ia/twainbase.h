/////////////////////////////////////////////////////////////////////////////
// Name:        twainbase.h
// Author:      Derry Bryson
// Created:     1/8/2003
// Copyright:   2003 (c) Derry Bryson
// Licence:     wxWindows License
/////////////////////////////////////////////////////////////////////////////
#ifndef __WXTWAINBASE_H
#define __WXTWAINBASE_H

#if defined(__GNUG__) && !defined(__APPLE__)
    #pragma interface "twainbase.h"
#endif

#ifdef __WXMSW__
#include "windows.h"
#include "wx/msw/winundef.h"
#endif

#include "wx/dynarray.h"

#undef Shutdown

#include "wx/ia/twain.h"

WX_DEFINE_ARRAY(TW_IDENTITY *, TW_IDENTITYArray);

//
//  wxTwainClient - Responsible for handling image data when it arrives and
//     updating user status if necessary for the wxTwain class.
//
class wxTwainClient
{
public:
    virtual ~wxTwainClient() {}

    //
    //  HandleImage() - called by the wxTwain class this client is
    //    attached to when an image is available.  This method must
    //    do something with the image data.
    //
    virtual bool HandleImage(TW_IMAGEINFO &info) = 0;

    //
    //  UpdateStatus - called by twain class this client is attached to
    //    to update the status and indicate if whatever operation is
    //    being done should continue.  This method might want to do something
    //    like update a progress dialog or something.  Should return TRUE
    //    if the operation should continue or NULL if not.
    //
    //    text      - text describing the operation being performed
    //
    //    quantum   - amount of the operation completed, between 0 and
    //                    (span - 1).
    //
    //    span      -
    //
    virtual bool UpdateStatus(const wxString &text, size_t quantum, size_t span) = 0;
};

class wxTwainBase : public wxObject
{
public:
    wxTwainBase(wxTwainClient *client,
        const wxString &appName = wxEmptyString,
        const wxString &appFamily = wxEmptyString,
        const wxString &appManufacturer = wxEmptyString,
        int versionMajor = 0, int versionMinor = 0,
        const wxString &versionInfo = wxEmptyString,
        int language = TWLG_ENGLISH_USA, int country = TWCY_USA);
    virtual ~wxTwainBase();

    virtual void SetClient(wxTwainClient* client);
    virtual wxTwainClient *GetClient();

    //
    //  Do whatever is necessary to setup TWAIN and open the Source Manager.
    //
    //  On Windows this means loading the dll, initializing m_DSMEntryProc,
    //  and calling OpenSourceManager().
    //
    //  For MacOS, something similar :)
    //
    //  Should set m_ok and m_sourceManagerIsOpen appropriately and
    //  return TRUE if successfull or FALSE if not.
    //
    virtual bool Startup() = 0;

    //
    //  Do whatever is necessary to close up and shutdown TWAIN including
    //  closing any open sources and the Source Manager.
    //
    //  On Windows this will close any open sources, close the source
    //  manager, and unload the DLL.
    //
    //  On MacOS???
    //
    //  Should set m_ok, m_sourceManagerIsOpen and m_sourceIsOpen
    //  appropriately.
    //
    //  NOTE:  YOU MUST CALL SHUTDOWN BEFORE THE DESTRUCTOR IS CALLED
    //  TO PROPERLY SHUT TWAIN DOWN
    //
    virtual void Shutdown() { wxLogDebug("wxTwainBase::Shutdown()"); }

    //
    //  Returns TRUE if TWAIN is loaded and ready to go or FALSE if not.
    //
    virtual bool Ok();

    //
    //  Returns last status
    //
    virtual TW_STATUS GetStatus();

    //
    //  Returns last return code from CallDSMEntryProc
    //
    virtual TW_INT16 GetReturnCode();

    virtual bool OpenSourceManager();
    virtual void CloseSourceManager();
    virtual bool IsSourceManagerOpen();
    virtual int GetSourceCount();
    virtual const TW_IDENTITY *GetSourceIdentity(int i);

    virtual bool OpenSource();
    virtual void CloseSource();
    virtual bool IsSourceOpen();

    virtual void SetAppLanguage(int language);
    virtual int GetAppLanguage();
    virtual void SetAppCountry(int country);
    virtual int GetAppCountry();
    virtual void SetAppVersion(int major, int minor, const wxString &versionInfo);
    virtual void GetAppVersion(int *major, int *minor, wxString *versionInfo);
    virtual void SetAppManufacturer(const wxString &manufacturer);
    virtual wxString GetAppManufacturer();
    virtual void SetAppProductFamily(const wxString &family);
    virtual wxString GetProductFamily();
    virtual void SetAppProductName(const wxString &name);
    virtual wxString GetAppProductName();

    virtual bool SelectDefaultSource();
    virtual bool UserSelectSource(const wxString &defSource);
    virtual bool SelectSource(const wxString &source);
    virtual bool SelectSource(int i);
    virtual bool IsSourceSelected();

    virtual void GetSourceVersion(int *major, int *minor, wxString *versionInfo);
    virtual wxString GetSourceProductName();
    virtual wxString GetSourceProductFamily();
    virtual wxString GetSourceManufacturer();

    virtual bool GetCapability(TW_CAPABILITY& twCap, TW_UINT16 cap,
                               TW_UINT16 conType = TWON_DONTCARE16) = 0;
    virtual bool GetCapability(TW_UINT16 cap, TW_UINT32& value) = 0;
    virtual bool SetCapability(TW_UINT16 cap, TW_UINT16 value, bool sign = FALSE) = 0;
    virtual bool SetCapability(TW_CAPABILITY& twCap) = 0;

    virtual bool SetImageCount(int count);

    virtual bool AcquireImage(bool showUI = TRUE);
    virtual bool AcquireImages(int numImages, bool showUI = TRUE);

    //
    //  Platform specific methods to get and set TWAIN string values.  On
    //  Windows this is just a strcpy, but the TWAIN docs indicate that
    //  something different must happen on MacOS.
    //
    static void SetStringValue(char *str, const wxString &value, int maxLen);
    static wxString GetStringValue(const char *str);

protected:
    virtual bool DoMessageLoop(bool showUI = TRUE);
    virtual bool EnableSource(bool showUI = TRUE);
    virtual void DisableSource();
    virtual bool IsSourceEnabled();

    virtual void TransferImage();
    virtual bool EndTransfer();
    virtual void CancelTransfer();
    virtual bool GetImageInfo(TW_IMAGEINFO &info);
    virtual bool GetImage(TW_IMAGEINFO &info) = 0;

    virtual bool GetAvailableSources();
    void DeleteAvailableSources();

    void Init(wxTwainClient* client, const wxString &appName,
        const wxString &appFamily, const wxString &appManufacturer,
        int versionMajor, int versionMinor, const wxString &versionInfo,
        int language, int country);


    virtual bool CallDSMEntryProc(pTW_IDENTITY origin, pTW_IDENTITY dest,
        TW_UINT32 dg, TW_UINT16 dat, TW_UINT16 msg, TW_MEMREF data);

    DSMENTRYPROC m_DSMEntryProc;

    TW_IDENTITY m_appId;        // application identity struct
    TW_IDENTITY m_sourceId;     // source identity struct
    TW_STATUS m_status;         // last status
    TW_INT16 m_returnCode;      // last return code
    TW_HANDLE *m_hParentPtr;    // ptr HWD for Windows, 0 for MacOS

    bool m_ok;                  // TWAIN is loaded and ready
    bool m_sourceManagerIsOpen; // source manager is open
    bool m_sourceIsOpen;        // source is open
    bool m_sourceSelected;      // source has been selected
    bool m_sourceEnabled;       // source wants msgs
    bool m_modalUI;             // UI is modal?

    int m_imageCount;           // num images to aquire
    int m_imagesAcquired;       // num acquired

    wxTwainClient *m_client;    // client

    TW_IDENTITYArray m_sources; // array of available sources

private:
    DECLARE_CLASS(wxTwainBase)
};

#endif // __WXTWAINBASE_H
