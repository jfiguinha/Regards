/////////////////////////////////////////////////////////////////////////////
// Name:        ia.h
// Purpose:     wxIA - Image Acquisition Classes
// Author:      Derry Bryson, Technology Associates, Inc.
// Modified by:
// Created:     01/08/2003
// RCS-ID:      $Id: ia.h,v 1.2 2003/04/03 03:53:40 dbryson Exp $
// Copyright:   (c) 2003 Derry Bryson
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_IA_H_
#define _WX_IA_H_

#ifdef __GNUG__
#pragma interface "ia.h"
#endif

#include "wx/confbase.h"

/*! \file ia.h
*/

//! IA Return Codes
/*!
*/
enum wxIAReturnCode
{
    //! Operation completed successfully
    wxIA_RC_SUCCESS = 0,
    //! Unknown error
    wxIA_RC_UNKNOWNERROR,
    //! Operation, mode, or value not supported
    wxIA_RC_NOTSUPPORTED,
    //! Operation was cancelled by user
    wxIA_RC_CANCELLED,
    //! Device is not ready
    wxIA_RC_NOTREADY,
    //! Device is busy
    wxIA_RC_DEVICEBUSY,
    //! Invalid data or argument
    wxIA_RC_INVAL,
    //! Document feeder jammed
    wxIA_RC_JAMMED,
    //! Document feeder out of documents
    wxIA_RC_NODOCS,
    //! Scanner cover is open
    wxIA_RC_COVEROPEN,
    //! Error during device I/O
    wxIA_RC_IOERROR,
    //! Out of memory
    wxIA_RC_NOMEM,
    //! Access to resource has been denied
    wxIA_RC_ACCESSDENIED,
    //! No source selected
    wxIA_RC_NOSOURCE,
    //! Provider not initialized
    wxIA_RC_NOTINITIALIZED,
};

//! IA Source Types
/*! Specifies the type of device encapsulated by the source.
*/
enum wxIASourceType
{
    //! Unknown
    wxIA_SOURCE_UNKNOWN = 0,
    //! Generic scanner - exact type is unknown, but it is a scanner
    wxIA_SOURCE_SCANNER,
    //! Flatbed scanner
    wxIA_SOURCE_FLATBEDSCANNER,
    //! Film scanner
    wxIA_SOURCE_FILMSCANNER,
    //! Still camera
    wxIA_SOURCE_STILLCAMERA,
    //! Video camera
    wxIA_SOURCE_VIDEOCAMERA,
    //! Image file
    wxIA_SOURCE_FILE,
    //! ImageBase - Image data base
    wxIA_SOURCE_IMAGEBASE,
    //! Frame grabber
    wxIA_SOURCE_FRAMEGRABBER,
    //! Multifunction device - fax/scanner/printer etc.
    wxIA_SOURCE_MULTIFUNCTION,
    //! Sheetfed scanner
    wxIA_SOURCE_SHEETFEDSCANNER,
    //! Handheld scanner
    wxIA_SOURCE_HANDHELDSCANNER,
};

//! User Interface Mode
/*! Mode of the user interface presented
*/
enum wxIAUIMode
{
    //! No user interface if possible
    wxIA_UIMODE_NONE = 0,
    //! Normal user interface.  Provides a dialog or whatever is appropriate.
    wxIA_UIMODE_NORMAL,
};

//! Source Image Transfer Mode
/*! Mode used to transfer image data from the source.
*/
enum wxIATransferMode
{
    //! Native image transfer mode
    /*! Image data will be supplied as both wxBitmap and wxImage.  For TWAIN
        the native mode supplies DIB and for Mac OS the native mode supplies
        a PICT.  No matter what native format is actually used, the user
        will retrieve the data as a wxBitmap or wxImage.
    */
    wxIA_TRANSFERMODE_NATIVE = 0,
    //! Transfer raw data a buffer at a time.
    /*! Image data will be transfered to a user supplied buffer using
        as many transfers as necessary.
    */
    wxIA_TRANSFERMODE_BUFFER,
    //! Transfer image data by storing data in a file
    wxIA_TRANSFERMODE_FILE,
};

//! Image transfer file type
enum wxIATransferFileType
{
    //! JPG - JPEG
    wxIA_FILETYPE_JPG = 0,
    //! TIFF
    wxIA_FILETYPE_TIFF,
    //! BMP - Windows/OS2 bitmap
    wxIA_FILETYPE_BMP,
};

//! Image Acquisition Source Info
/*! Descriptive information about a source.
*/
class wxIASourceInfo
{
public:
    //! Constructor
    /*! Default constructor
    */
    wxIASourceInfo();

    //! Constructor
    /*! \param name name of the source
        \param model particular model of the device or scanner
        \param vendor vendor or manufacturer
        \param type type of device or source
    */
    wxIASourceInfo(const wxString &name, const wxString &model,
        const wxString &vendor, wxIASourceType type);

    //
    //  Accessors
    //
    //! Set name
    /*! Sets the name
        \param name name of the source
    */
    void SetName(const wxString &name);
    //! Get name
    /*! \return the name
    */
    wxString GetName() const;
    //! Set model
    /*! Sets the model
        \param model particular model of the device or scanner
    */
    void SetModel(const wxString &model);
    //! Get model
    /*! \return the model
    */
    wxString GetModel() const;
    //! Set vendor
    /*! \param vendor vendor or manufacturer
    */
    void SetVendor(const wxString &vendor);
    //! Get vendor
    /*! \return the vendor
    */
    wxString GetVendor() const;
    //! Set Type
    /*! \param type type of device or source
    */
    void SetType(wxIASourceType type);
    //! Get type
    /*! \return the type
    */
    wxIASourceType GetType() const;

protected:
    wxString m_name;
    wxString m_model;
    wxString m_vendor;
    wxIASourceType m_type;
};

//! Null source info
extern wxIASourceInfo wxNullIASourceInfo;

//! Image Acquisition Monitor
/*! Class to monitor the progress of an image aquistion.
    Generally used to provide feedback to the user and allow the
    user to possibly abort the operation.

    \note Using this to provide a wxProgressDialog or similar interface
          is really only suggested when not using the user interface
          provided by the provider/source (i.e by specifying a wxUIMode
          of wxIA_UIMODE_NONE).  Otherwise your windows and/or dialogs
          may interfere with the possibly native interface being
          provided by the provider/source.
*/
class wxIAMonitor
{
public:
    //! Constructor
    /*! Default constructor
    */
    wxIAMonitor() {}
    //! Destructor
    virtual ~wxIAMonitor() {}

    //! Update method
    /*! Override this method.  This method will be called by the provider
        periodically to indicate progress and allow the user to abort the
        operation.

        \param text string describing the operation being performed

        \param quantum the position relative to span representing how much
               progress has been made towards completing the
               operation.

        \param span the span of the operation (i.e. the max value
                    for quantum).

        \return True to continue process or false to abort.
    */
    virtual bool Update(const wxString &text, const off_t quantum, const off_t span) = 0;
};

//! Image Acquisition Provider
/*! Provides one or more image sources which may be selected and used to
    acquire images.  Image sources represent devices such as scanners,
    cameras, etc. or anything else  that can provide image data (like image
    file handlers or an image database).

    Different platforms will provide different wxIAProvider's to allow
    access to platform specific image acquisition interfaces such as
    TWAIN on Windows and MacOS and SANE on Unix platforms.

    A list of available wxIAProviders is maintained by the wxIAManager
    class.
*/
class wxIAProvider : public wxObject
{
public:
    //! Constructor
    /*! Default constructor
    */
    wxIAProvider();

    //! Destructor
    virtual ~wxIAProvider();

    /*! Used to determine if the provider is initialized and ready for
        use.

       \return True if the provider is ready for use or false if not.
    */
    virtual bool Ok();

    //! Get name
    /*! \return the name of this provider
    */
    virtual wxString GetName();

    //! Get source count
    /*! \returns The number of sources available or -1 if the sources cannot
                 be enumerated.  If the sources cannot be enumerated, the
                 only way to select a source is by a call to SelectSource()
                 with the default args.
    */
    virtual int GetSourceCount();

    /*! Get the source info for the specified source.

        \param i Index of the source to retrieve info for.  Must be >= 0
                 and < GetSourceCount()

        \return the source info for the specified source.
    */
    virtual wxIASourceInfo GetSourceInfo(int i);

    /*! Select a source.

        \param name Name of the source to select or "" to select the
                    default source.

        \param uiMode User interface mode.

        \param parent Window to be parent of any dialog or user interface
                      element that may be created.  Used to disable the
                      parent (and possibly the entire app) if required
                      by the user interface.  May be NULL.

        \return wxIA_RC_SUCCESS if a source was selected or another return
        code on failure.

    */
    virtual wxIAReturnCode SelectSource(const wxString& name = wxEmptyString,
        wxIAUIMode uiMode = wxIA_UIMODE_NORMAL, wxWindow *parent = NULL);

    /*! Select the default source.

        \return wxIA_RC_SUCCESS if a source was selected or another return
        code on failure.
    */
    virtual wxIAReturnCode SelectDefaultSource();

    /*! Is a source selected?
        \return wxIA_RC_SUCCESS on success another return code on failure.
    */
    virtual bool IsSourceSelected();

    /*! Get source info for the currently selected source.
        \return the source info for the currently selected source.  If no
        source is currently selected, wxIAEmptySourceInfo will be returned.
    */
    virtual wxIASourceInfo GetSelSourceInfo();

    //! Set selected source configuration.
    /*! Configure the currently selected source from the configuration encoded
        in a wxString.

        \param configString Configuration value encoded in a wxString by
               GetSelSourceConfig().

        \return True on success or false on failure.
    */
    virtual wxIAReturnCode SetSelSourceConfig(const wxString &configString);

    //! Get the current configuration of the selected source.
    /*! \return The configuration of the currently selected source encoded as a
                wxString.  The format of the configuration string is defined by
                the source and should not be edited or modified.
    */
    virtual wxString GetSelSourceConfig();

    //! Acquire an image.
    /*! Aquire an image from the currently selected source.  Returns true on
        success or false on failure or if the user aborted.

        \param uiMode User interface mode.

        \param parent Parent window for any dialog or user interface element
                      that many be created.  Used to disable the
                      parent (and possibly the entire app) if required
                      by the user interface.  May be NULL.

        \param mon Pointer to a wxIAMonitor that will be notified as progress
                   is made.  May be NULL.
    */
    virtual wxIAReturnCode AcquireImage(wxIAUIMode uiMode = wxIA_UIMODE_NORMAL,
        wxWindow *parent = NULL, wxIAMonitor *mon = NULL);

    //! Acquire one or more images.
    /*! Aquire images from the currently selected source.  Returns true on
        success or false on failure or if the user aborted.

        \param numImages Number of images to be acquired.

        \param uiMode User interface mode.

        \param parent Parent window for any dialog or user interface element
                      that many be created.  Used to disable the
                      parent (and possibly the entire app) if required
                      by the user interface.  May be NULL.

        \param mon Pointer to a wxIAMonitor that will be notified
                    as progress is made.  May be NULL.
    */
    virtual wxIAReturnCode AcquireImages(int numImages,
        wxIAUIMode uiMode = wxIA_UIMODE_NORMAL, wxWindow *parent = NULL,
        wxIAMonitor *mon = NULL);

    //! Set event handler.
    /*! Set the event handler to be notified when wxIAEvent's occur.
        Currently there are two events generated: wxEVT_IA_GETIMAGE event
        that is sent when an image has been acquired and
        wxEVT_IA_UPDATESTATUS that is sent to allow the application
        to update the status of the acquisition.  The method handling the
        wxEVT_IA_GETIMAGE event should call GetImage() or GetBitmap() to
        retrieve the image data.

        \param evtHandler Pointer to the event handler.
    */
    virtual void SetEvtHandler(wxEvtHandler *evtHandler);


    //! Set transfer mode
    /*! Set the image data transfer mode.  The default transfer mode
        is wxIA_TRANSFERMODE_NATIVE.

        \param mode desired mode
    */
    virtual wxIAReturnCode SetTransferMode(wxIATransferMode mode);

    //! Get the current transfer mode
    /*! \returns the current transfer mode for the selected source.
    */
    virtual wxIATransferMode GetTransferMode();

    //! Set the transfer file name
    /*! \param filename name of file in which to store image data

        \param type transfer file type

        \return wxIA_RC_SUCCESS on success or another return code on failure
    */
    virtual wxIAReturnCode SetTransferFilename(const wxString &filename,
        wxIATransferFileType type);

    //! Get the transfer file name
    virtual wxString GetTransferFilename();

    //! Get the transfer file type
    virtual wxIATransferFileType GetTransferFileType();

    //! Set the transfer buffer
    /*! \param buffer pointer to buffer in which to store image data

        \param size size in bytes of buffer

        \note buffer must be valid during the entire transfer

        \return wxIA_RC_SUCCESS on success or another return code on failure
    */
    virtual wxIAReturnCode SetTransferBuffer(void *buffer, size_t size);

    //! Get the transfer buffer
    /*! \return a pointer to the transfer buffer
    */
    virtual void *GetTransferBuffer();

    //! Get the transfer buffer size
    virtual size_t GetTransferBufferSize();

    //! Get current event handler.
    /*! \return Current event handler.
    */
    wxEvtHandler *GetEvtHandler();

    //! Get the last image aqcuired as a wxImage.
    /*! \return The last image acquired as a wxImage.  Image data is destroyed
                after calling this method.
    */
    virtual wxImage GetImage();

    //! Get the last image acquired as a wxBitmap
    /*! \return The last image acquired as a wxBitmap.  Image data is destroyed
        after calling this method.
    */
    virtual wxBitmap GetBitmap();

    //! Save the current settings
    /*! Save the current settings for this provider to the specified config
        object.

        \return True on success or false on failure.
    */
    virtual bool SaveSettings(wxConfigBase *config);

    //! Load settings
    /*! Load saved settings for this provider from the specified config
        object.

        \return True on success or false on failure.
    */
    virtual bool LoadSettings(wxConfigBase *config);

protected:
    wxEvtHandler *m_evtHandler;
    wxIATransferMode m_transferMode;
    void *m_transferBuffer;
    size_t m_transferBufferSize;
    wxString m_transferFilename;
    wxIATransferFileType m_transferFileType;

private:
    DECLARE_CLASS(wxIAProvider)
};

//! Image Acquisition Provider Array
WX_DEFINE_ARRAY(wxIAProvider*, wxIAProviderPtrArray);

//! Image Acquisition Manager
/*! Maintains the list of available wxIAProvider's.  There is only one instance
    of this class and it is created the first time Get() is called.  The
    application may add it's own providers after calling Get() if desired.

    \note You should set your app name and vendor strings before calling
    wxIAManager::Get() because this information is used by at least the
    TWAIN source.
*/
class wxIAManager : public wxObject
{
public:
    //! Get the Image Acquistion Manager.
    /*! This method will create and initialize the manager the first time
        it is called, which will cause the various platform specific providers
        to be loaded and initialized.

        \return A reference to the wxIAManager.
    */
    static wxIAManager &Get();

    //! Add a provider
    /*! The provider becomes the property of the wxIAManager.

        \param provider provider to add
    */
    void AddProvider(wxIAProvider *provider);

    /*! \returns a reference to the provider array.
    */
    wxIAProviderPtrArray &GetProviders();

    //! Gets the default provider
    /*! \return the default provider for the platform or NULL if there
        is no default.

        On Windows and MAC this will probably return a TWAIN provider.
        On Unix this will probably return a SANE provider.
    */
    wxIAProvider *GetDefaultProvider();

    //! Returns a string description
    /*! \return A string describing the specified
        wxIAReturnCode for usable for displaying to the user.
    */
    wxString GetReturnCodeDesc(wxIAReturnCode rc);

private:
    //! Constructor
    wxIAManager();
    //! Destructor
    ~wxIAManager();

    static bool s_init;
    wxIAProviderPtrArray m_providers;
    static wxIAProvider *s_defaultProvider;

    DECLARE_CLASS(wxIAManager)
};

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_IA_GETIMAGE, 0)
    DECLARE_EVENT_TYPE(wxEVT_IA_UPDATE, 0)
END_DECLARE_EVENT_TYPES()

//! Image Acquisition event
/*! Event sent by a wxIAProvider
*/
class wxIAEvent : public wxEvent
{
public:
    //! Constructor
    wxIAEvent(wxEventType type, wxIAProvider *provider);
    //! Constructor
    wxIAEvent(wxEventType type, wxIAProvider *provider, const wxString &text,
        size_t quantum, size_t span);
    //! Copy constructor
    wxIAEvent(const wxIAEvent& event);

    /*! \return a pointer to the provider that posted the event
    */
    virtual wxIAProvider *GetProvider() { return m_provider; }
    /*! Set the provider
    */
    virtual void SetProvider(wxIAProvider *provider) { m_provider = provider; }
    /*! Get update status text
    */
    virtual wxString GetText() { return m_text; }
    /*! Set update status text
    */
    virtual void SetText(const wxString &text) { m_text = text; }
    /*! Get quantum - the value indicating the current status relative to
       span (GetSpan())
    */
    virtual size_t GetQuantum() { return m_quantum; }
    /*! Set the quantum
    */
    virtual void SetQuantum(size_t quantum) { m_quantum = quantum; }
    /*! Get the span
    */
    virtual size_t GetSpan() { return m_span; }
    /*! Set the span
    */
    virtual void SetSpan(size_t span) { m_span = span; }
    /*! \returns TRUE if the provider should abort the scan or FALSE if not.
    */
    virtual bool ShouldAbort() { return m_abort; }
    /*! Set val to TRUE to tell the provider it should abort the operation.

        \param val
    */
    virtual void Abort(bool val = TRUE) { m_abort = val; }

    //! Clone
    wxEvent *Clone(void) const { return new wxIAEvent(*this); }

private:
    wxIAProvider *m_provider;
    wxString m_text;
    size_t m_quantum;
    size_t m_span;
    bool m_abort;
};

typedef void (wxEvtHandler::*wxIAEventFunction)(wxIAEvent &);

#define wxIAEventHandler(func) wxEVENT_HANDLER_CAST(wxIAEventFunction, func)

#define EVT_IA_GETIMAGE(func) \
    DECLARE_EVENT_TABLE_ENTRY(wxEVT_IA_GETIMAGE, -1, -1, \
                              (wxObjectEventFunction) \
                              (wxEventFunction) \
                              (wxIAEventFunction)&func, \
                              NULL),
#define EVT_IA_UPDATE(func) \
    DECLARE_EVENT_TABLE_ENTRY(wxEVT_IA_UPDATE, -1, -1, \
                              (wxObjectEventFunction) \
                              (wxEventFunction) \
                              (wxIAEventFunction)&func, \
                              NULL),

#endif // _WX_IA_H_
