/***************************************************************
 * Name:      wxiscanframe.h
 * Purpose:   wxWidgets wrapper around libsane (for scanner
 *            access)
 * Author:    Daniel Nell (daniel.nell@nellresearch.de)
 * Created:   2008-05-04
 * Copyright: Daniel Nell (www.nellresearch.de)
 * License:   wxWindows license
**************************************************************/

#ifndef WXSCAN_H_INCLUDED
#define WXSCAN_H_INCLUDED

// Define some macros for several scanning environments.
#if defined( __UNIX_LIKE__ ) and not defined(__APPLE__)
#   define __WXSCANSANE__   1
#elif defined( __WXMSW__ )
#   define wxScan           wxScanTwain
#   define __WXSCANTWAIN__  1
#endif

// Include additional headers.
#if __WXSCANSANE__
#   include <sane/sane.h>
#elif __WXSCANTWAIN__
#
#else
#
#endif
#include <wx/arrstr.h>
#include <vector>
#include "Scanner.h"
using namespace std;

class wxScanDataOption
{
public:
    wxString name;
    int index;
};

typedef vector<wxScanDataOption> vectorDataScan;

//////////////////////////////////////////////////////////
// Class wxScanBase
//
/// \brief Abstract base class for all wxScan... classes.
///
/// This class represents an interface to the various
/// wxScan... classes. Note that every member function
/// has to be implemented in the subclasses.
class wxScanBase
{
public:
    
enum wxSCANReturnCode
{
    //! Operation completed successfully
    wxSCAN_RC_SUCCESS = 0,
    //! Unknown error
    wxSCAN_RC_UNKNOWNERROR,
    //! Operation, mode, or value not supported
    wxSCAN_RC_NOTSUPPORTED,
    //! Operation was cancelled by user
    wxSCAN_RC_CANCELLED,
    //! Device is not ready
    wxSCAN_RC_NOTREADY,
    //! Device is busy
    wxSCAN_RC_DEVICEBUSY,
    //! Invalid data or argument
    wxSCAN_RC_INVAL,
    //! Document feeder jammed
    wxSCAN_RC_JAMMED,
    //! Document feeder out of documents
    wxSCAN_RC_NODOCS,
    //! Scanner cover is open
    wxSCAN_RC_COVEROPEN,
    //! Error during device I/O
    wxSCAN_RC_IOERROR,
    //! Out of memory
    wxSCAN_RC_NOMEM,
    //! Access to resource has been denied
    wxSCAN_RC_ACCESSDENIED,
    //! No source selected
    wxSCAN_RC_NOSOURCE,
    //! Provider not initialized
    wxSCAN_RC_NOTINITIALIZED,
};    
    
    
    /// Standard constructor
    wxScanBase(){}

    /// Virtual destructor
    virtual ~wxScanBase(){}

    /// Check if the error state is 'OK'. This function returns 'true' on 'OK'.
    virtual bool IsOk() = 0;

    /// Check if the scan device is available.
    virtual bool IsDeviceAvailable() = 0;

    /// Get the scanning resolution
    virtual int  GetResolution() = 0;

    /// \brief Scan an image.
    ///
    /// \param oImage   a reference to the image object that should hold the image
    ///
    /// The result is in the parameter oImage.
    virtual bool ScanImage( wxImage &oImage ) = 0;
};

#if __WXSCANSANE__

// Define some convenience macros.
    #define SANE_STRING_TO_WXSTRING( SaneString )                                  \
        wxString( (const char *)SaneString, wxConvISO8859_1 )

#define GET_DEVICE_PARAM( PARAM )                                              \
    if( nIndex < 0 )                                                           \
    {                                                                          \
        nIndex= m_nDeviceIndex;                                                \
    }                                                                          \
    if( ( nIndex >= 0 ) && ( nIndex < m_nDeviceCount ) )                       \
    {                                                                          \
        return SANE_STRING_TO_WXSTRING( m_ppDeviceList[nIndex]->PARAM );       \
    }                                                                          \
    return wxEmptyString;

//////////////////////////////////////////////////////////
// Class wxScanSane
//
/// \brief SANE incarnation of the the wxScan... classes.
///
/// This class is the default class (wxScan) for UNIX like
/// systems or any other system using SANE as its scanning
/// interface.
///
/// Note: There should be a SANE implementation for MS
///       Windows, too, at least for network based
///       scanning.
class wxScanSane : public wxScanBase
{
  public:
    /// \brief Standard constructor
    ///
    /// \param bOnlyLocalDevices    flag to use only local scanning devices
    ///                             (default) or devices via network, too.
    wxScanSane( bool bOnlyLocalDevices= false );
    
    

    /// Virtual destructor.
    virtual ~wxScanSane();

    /// Check if the error state is 'OK'. This function returns 'true' on 'OK'.
    virtual bool IsOk()
    {
        return ( m_SaneStatus == SANE_STATUS_GOOD ) ? true : false;
    }

    /// Check if the scan device is available.
    virtual bool IsDeviceAvailable()
    {
        return ( m_nDeviceCount <= 0 ) || ( m_nDeviceIndex < 0 ) || ( m_nDeviceIndex >= m_nDeviceCount ) ? false : true;
    }

    /// \brief Scan an image.
    ///
    /// \param oImage   a reference to the image object that should hold the image
    ///
    /// The result is in the parameter oImage.
    virtual bool ScanImage( wxImage &oImage );

    /// Get the scanning resolution
    virtual int GetResolution()
    {
        return m_nResolution;
    }

    /// Get the count of the detected scanning devices.
    virtual int GetDeviceCount()
    {
        return m_nDeviceCount;
    }

    /// Get the count of the detected scanning devices.
    virtual int GetDeviceIndex()
    {
        return m_nDeviceIndex;
    }

    /// Get the scanning device name by index number.
    virtual wxString GetDeviceName( int nIndex= -1 )
    {
        GET_DEVICE_PARAM( name )
    }

    /// Get the scanning device vender by index number.
    virtual wxString GetDeviceVendor( int nIndex= -1 )
    {
        GET_DEVICE_PARAM( vendor )
    }

    /// Get the scanning device model name by index number.
    virtual wxString GetDeviceModel( int nIndex= -1 )
    {
        GET_DEVICE_PARAM( model )
    }

    /// Get the scanning device type by index number.
    virtual wxString GetDeviceType( int nIndex= -1 )
    {
        GET_DEVICE_PARAM( type )
    }

    /// Select the scanning device by index number.
    virtual bool SetDeviceIndex( int nIndex );

    /// Get the SANE status.
    virtual SANE_Status GetSaneStatus()
    {
        return m_SaneStatus;
    }

    /// Geth the SANE status as string.
    virtual wxString GetSaneStatusString()
    {
        return SANE_STRING_TO_WXSTRING( ::sane_strstatus( GetSaneStatus() ) );
    }

    /// Get the scan device list.
    virtual bool SaneGetDevices();
    
    virtual bool SelectSource(const wxString &name, const bool &showUI, wxWindow *parent);

    virtual int GetSelectedIndex();
    
    const SANE_Option_Descriptor *SaneGetOptionDescriptor(SANE_Int option);
    
    SANE_Status SaneControlOption(SANE_Int option, SANE_Action action,
        void *value = NULL, SANE_Int *info = NULL);    
        
    bool IsSourceSelected();
    
    void SetOption(const int &dpi, const Scanner::ScanMode &scanMode);

private:

    bool Open();
    bool Close();
    
    void GetScannerOption();
    
    void setIntOption(const SANE_Option_Descriptor* option, SANE_Int option_index, int value, int* result);

    void setFixedOption(const SANE_Option_Descriptor* option, SANE_Int option_index, double value, double* result);

    void DoSetOptions();
  
    bool setStringOption(const SANE_Option_Descriptor* option, SANE_Int option_index, const std::string& value, std::string* result);
  
    bool setConstrainedStringOption(const SANE_Option_Descriptor* option, SANE_Int option_index, const std::vector<std::string>& values, std::string* result);
  
    const SANE_Option_Descriptor* getOptionByName(const vectorDataScan& options, const std::string& name, int& index);
    
    /// Flag ...
    bool m_bOnlyLocalDevices;
    
    SANE_Handle hSaneHandle = nullptr;

    /// holds the SANE status
    SANE_Status m_SaneStatus;
    
    Scanner::ScanMode scanMode = Scanner::ScanMode::GRAY;

    /// holds the SANE device list
    const SANE_Device **m_ppDeviceList;

    /// holds the number of SANE devices
    int m_nDeviceCount = 0;

    /// holds the index of the standard SANE device
    int m_nDeviceIndex = 0;

    /// holds the scanning resolution
    int m_nResolution;
    
    //int scan_mode = 0;
    
    double dpi = 300.0;
    
    int m_numDevices = 0;
    int m_selDevice = 0;
    int depth = 0;
    
    bool isClose = false;
    
    double page_width = 0;
    double page_height = 0;
    
    vectorDataScan options;

};

// Define the standard wxScan object.
typedef wxScanSane wxScan;
#endif // __WXSCANSANE__

#if __WXSCANTWAIN__
//////////////////////////////////////////////////////////
// Class wxScanTwain
//
/// \brief TWAIN incarnation of the the wxScan... classes.
///
/// This class is the default class (wxScan) for Windows
/// systems.
///
/// Note: At the moment this class is only a stub for to
///       start porting wxScan base applications to
///       MS Windows.
class wxScanTwain
{
  public:
    /// Standard constructor
    wxScanTwain(){}

    /// Virtual destructor
    virtual ~wxScanTwain(){}

    /// Check if the error state is 'OK'. This function returns 'true' on 'OK'.
    ///
    /// Note: This function is currently only a stub and returns a dummy value.
    virtual bool IsOk(){ return false; }

    /// Check if the scan device is available.
    ///
    /// Note: This function is currently only a stub and returns a dummy value.
    virtual bool IsDeviceAvailable(){ return false; }

    /// Get the scanning resolution
    ///
    /// Note: This function is currently only a stub and returns a dummy value.
    virtual int  GetResolution(){ return 0; }

    /// \brief Scan an image.
    ///
    /// \param oImage   a reference to the image object that should hold the image
    ///
    /// The result is in the parameter oImage.
    ///
    /// Note: This function is currently only a stub and returns a dummy value
    ///       (false).
    virtual bool ScanImage( wxImage &oImage ){ return false; };
};

// Define the standard wxScan object.
typedef wxScanTwain wxScan;
#endif // __WXSCANTWAIN__

#endif // WXSCAN_H_INCLUDED


