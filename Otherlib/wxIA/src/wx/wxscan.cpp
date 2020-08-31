/***************************************************************
 * Name:      wxscan.cpp
 * Purpose:   wxWidgets wrapper around libsane (for scanner
 *            access)
 * Author:    Daniel Nell (daniel.nell@nellresearch.de)
 * Created:   2008-05-04
 * Copyright: Daniel Nell (www.nellresearch.de)
 * License:   wxWindows license
 **************************************************************/

// Include headers.
#include <header.h>

#include <wx/wxscan.h>

#if defined( __UNIX_LIKE__ ) and not defined(__APPLE__)

#include <sane/saneopts.h>
#include <wx/progdlg.h>
//////////////////////////////////////////////////////////
// Class wxScanSane
//
// Standard constructor.
//
wxScanSane::wxScanSane( bool bOnlyLocalDevices )
{
    // Initialize members.
    m_bOnlyLocalDevices= bOnlyLocalDevices;
    m_nResolution= -1; // Invalid value for resolution.

    // Initialize SANE.
    m_SaneStatus= ::sane_init( NULL, NULL );

    // Get the device list from SANE subsystem.
    //SaneGetDevices();
    /// holds the number of SANE devices
    m_nDeviceCount = 0;

    /// holds the index of the standard SANE device
    m_nDeviceIndex = 0;
   
    //scan_mode = 0;
    
    dpi = 300.0;
    
    m_numDevices = 0;
    m_selDevice = 0;
    
    isClose = false;    
}

// Virtual destructor.
//
wxScanSane::~wxScanSane()
{

    // Close the sane device.
    Close();    
    
    // Close the SANE sub system.
    ::sane_exit();
}

/***************************** Sane option stuff *****************************/
const SANE_Option_Descriptor* wxScanSane::getOptionByName(const vectorDataScan& options, SANE_Handle handle, const std::string& name, int& index)
 {
     index = 0;
     for(wxScanDataOption data : options)
     {
         if(data.name == name)
         {
             index = data.index;
             break;
         }
     }
     
     if(index != 0)
         return sane_get_option_descriptor(handle, index);

	return nullptr;
}

bool wxScanSane::setStringOption(SANE_Handle & handle, const SANE_Option_Descriptor* option, SANE_Int option_index, const std::string& value, std::string* result) {
	if(option->type != SANE_TYPE_STRING) {
		return false;
	}

	char* v = new char[value.size() + 1]; // +1: \0
	strncpy(v, value.c_str(), value.size() + 1);
	SANE_Status status = sane_control_option(handle, option_index, SANE_ACTION_SET_VALUE, v, nullptr);
	
	if(result) { *result = v; }
	delete[] v;
	return status == SANE_STATUS_GOOD;
}

bool wxScanSane::setConstrainedStringOption(SANE_Handle & handle, const SANE_Option_Descriptor* option, SANE_Int option_index, const std::vector<std::string>& values, std::string* result) {
	if(option->type != SANE_TYPE_STRING) {
		return false;
	}
	if(option->constraint_type != SANE_CONSTRAINT_STRING_LIST) {
		return false;
	}

	for(const std::string& value : values) {
		for(int j = 0; option->constraint.string_list[j] != nullptr; ++j) {
			if(value == option->constraint.string_list[j]) {
				return setStringOption(handle, option, option_index, value, result);
			}
		}
	}
	if(result) { *result = ""; }
	return false;
}

void wxScanSane::setFixedOption(SANE_Handle handle, const SANE_Option_Descriptor* option, SANE_Int option_index, double value, double* result) {
	if(option->type != SANE_TYPE_FIXED) {
		return;
	}

	double v = value;
	if(option->constraint_type == SANE_CONSTRAINT_RANGE) {
		double min = SANE_UNFIX(option->constraint.range->min);
		double max = SANE_UNFIX(option->constraint.range->max);
		v = std::max(min, std::min(max, v));
	} else if(option->constraint_type == SANE_CONSTRAINT_WORD_LIST) {
		double distance = std::numeric_limits<double>::max();
		double nearest = 0.0;

		/* Find nearest value to requested */
		for(int i = 0; i < option->constraint.word_list[0]; ++i) {
			double x = SANE_UNFIX(option->constraint.word_list[i + 1]);
			double d = std::abs(x - v);
			if(d < distance) {
				distance = d;
				nearest = x;
			}
		}
		v = nearest;
	}

	SANE_Fixed v_fixed = SANE_FIX(v);
	SANE_Status status = sane_control_option(handle, option_index, SANE_ACTION_SET_VALUE, &v_fixed, nullptr);
	//g_debug("sane_control_option(%d, SANE_ACTION_SET_VALUE, %f) -> (%s, %f)", option_index, value, sane_strstatus(status), SANE_UNFIX(v_fixed));

	if(result != nullptr) { *result = SANE_UNFIX(v_fixed); }
}

void wxScanSane::setIntOption(SANE_Handle handle, const SANE_Option_Descriptor* option, SANE_Int option_index, int value, int* result) {
	if(option->type != SANE_TYPE_INT) {
		return;
	}

	SANE_Word v = static_cast<SANE_Word>(value);
	if(option->constraint_type == SANE_CONSTRAINT_RANGE) {
		if(option->constraint.range->quant != 0) {
			v *= option->constraint.range->quant;
		}
		v = std::max(option->constraint.range->min, std::min(option->constraint.range->max, v));
	} else if(option->constraint_type == SANE_CONSTRAINT_WORD_LIST) {
		int distance = std::numeric_limits<int>::max();
		int nearest = 0;

		/* Find nearest value to requested */
		for(int i = 0; i < option->constraint.word_list[0]; ++i) {
			int x = option->constraint.word_list[i + 1];
			int d = std::abs(x - v);
			if(d < distance) {
				distance = d;
				nearest = x;
			}
		}
		v = nearest;
	}

	SANE_Status status = sane_control_option(handle, option_index, SANE_ACTION_SET_VALUE, &v, nullptr);
	//g_debug("sane_control_option(%d, SANE_ACTION_SET_VALUE, %d) -> (%s, %d)", option_index, value, sane_strstatus(status), v);
	if(result != nullptr) { *result = v; }
}


void wxScanSane::GetScannerOption()
{
    if(options.size() == 0)
    {
        const SANE_Option_Descriptor* option;
        int index = 0;
        /* Build the option table */
        while((option = sane_get_option_descriptor(hSaneHandle, index)) != nullptr) {
            //logOption(index, option);
            if(
                /* Ignore groups */
                option->type != SANE_TYPE_GROUP &&
                /* Option disabled */
                (option->cap & SANE_CAP_INACTIVE) == 0 &&
                /* Some options are unnamed (e.g. Option 0) */
                option->name != nullptr && strlen(option->name) > 0
            ) {
                wxScanDataOption scanData;
                scanData.name = option->name;
                scanData.index = index;
                options.push_back(scanData);
            }
            ++index;
        } 
    }
}

bool wxScanSane::SetDPIModeOption(SANE_Handle & hSaneHandle)
{
	int index = 0;
	const SANE_Option_Descriptor* option;
	
    
    GetScannerOption();
    
	/* Set resolution and bit depth */
    
    
	option = getOptionByName(options, hSaneHandle, SANE_NAME_SCAN_RESOLUTION, index);
	if(option != nullptr) {
		if(option->type == SANE_TYPE_FIXED)
        {
			setFixedOption(hSaneHandle, option, index, dpi, nullptr);
		} 
        else 
        {
			setIntOption(hSaneHandle, option, index, dpi, nullptr);
		}
	}
    
        /*
		option = getOptionByName(options, hSaneHandle, SANE_NAME_BIT_DEPTH, index);
		if(option != nullptr && oSaneParameters.depth > 0) {
			setIntOption(hSaneHandle, option, index, oSaneParameters.depth, nullptr);
		}*/
}

double convert_page_size (SANE_Option_Descriptor * option, double size, double dpi)
{
    if (option->unit == SANE_UNIT_PIXEL)
        return dpi * size / 254.0;
    else if (option->unit == SANE_UNIT_MM)
        return size / 10.0;
    else
    {
        //warning ("Unable to set unsupported unit type");
        return 0.0f;
    }
}

bool wxScanSane::SetPageOption(SANE_Handle & hSaneHandle)
{
   int index = 0;
	const SANE_Option_Descriptor* option;

    GetScannerOption();
	/* Always use maximum scan area - some scanners default to using partial areas.  This should be patched in sane-backends */
	option = getOptionByName(options, hSaneHandle, SANE_NAME_SCAN_BR_X, index);
	if(option != nullptr && option->constraint_type == SANE_CONSTRAINT_RANGE) {
		if(option->type == SANE_TYPE_FIXED) {
			setFixedOption(hSaneHandle, option, index, SANE_UNFIX(option->constraint.range->max), nullptr);
		} else {
			setIntOption(hSaneHandle, option, index, option->constraint.range->max, nullptr);
		}
	}
	option = getOptionByName(options, hSaneHandle, SANE_NAME_SCAN_BR_Y, index);
	if(option != nullptr && option->constraint_type == SANE_CONSTRAINT_RANGE) {
		if(option->type == SANE_TYPE_FIXED) {
			setFixedOption(hSaneHandle, option, index, SANE_UNFIX(option->constraint.range->max), nullptr);
		} else {
			setIntOption(hSaneHandle, option, index, option->constraint.range->max, nullptr);
		}
	}

	/* Set page dimensions */
	option = getOptionByName(options, hSaneHandle, SANE_NAME_PAGE_WIDTH, index);
	if(option != nullptr && page_width > 0.0) {
		if(option->type == SANE_TYPE_FIXED) {
			setFixedOption(hSaneHandle, option, index, page_width / 10.0, nullptr);
		} else {
			setIntOption(hSaneHandle, option, index, page_width / 10, nullptr);
		}
	}
	option = getOptionByName(options, hSaneHandle, SANE_NAME_PAGE_HEIGHT, index);
	if(option != nullptr && page_height > 0.0) {
		if(option->type == SANE_TYPE_FIXED) {
			setFixedOption(hSaneHandle, option, index, page_height / 10.0, nullptr);
		} else {
			setIntOption(hSaneHandle, option, index, page_height / 10, nullptr);
		}
	}
}

bool wxScanSane::SetScanModeOption(Scanner::ScanMode scan_mode, SANE_Handle & hSaneHandle)
{
	int index = 0;
	const SANE_Option_Descriptor* option;

    GetScannerOption();   
    
	/* Scan mode (before resolution as it tends to affect that */
	option = getOptionByName(options, hSaneHandle, SANE_NAME_SCAN_MODE, index);
    
    
	if(option != nullptr) {
		// The names of scan modes often used in drivers, as taken from the sane-backends source 
		std::vector<std::string> color_scan_modes = {
			SANE_VALUE_SCAN_MODE_COLOR,
			"Color",
			"24bit Color" // Seen in the proprietary brother3 driver 
		};
		std::vector<std::string> gray_scan_modes = {
			SANE_VALUE_SCAN_MODE_GRAY,
			"Gray",
			"Grayscale",
			SANE_I18N("Grayscale"),
			"True Gray" // Seen in the proprietary brother3 driver 
		};
		std::vector<std::string> lineart_scan_modes = {
			SANE_VALUE_SCAN_MODE_LINEART,
			"Lineart",
			"LineArt",
			SANE_I18N("LineArt"),
			"Black & White",
			SANE_I18N("Black & White"),
			"Binary",
			SANE_I18N("Binary"),
			"Thresholded",
			SANE_VALUE_SCAN_MODE_GRAY,
			"Gray",
			"Grayscale",
			SANE_I18N("Grayscale"),
			"True Gray" // Seen in the proprietary brother3 driver 
		};

		switch(scan_mode) {
		case Scanner::ScanMode::COLOR:
			if(!setConstrainedStringOption(hSaneHandle, option, index, color_scan_modes, nullptr)) {
				printf("Unable to set Color mode, please file a bug");
			}
			break;
		case Scanner::ScanMode::GRAY:
			if(!setConstrainedStringOption(hSaneHandle, option, index, gray_scan_modes, nullptr)) {
				printf("Unable to set Gray mode, please file a bug");
			}
			break;
		case Scanner::ScanMode::LINEART:
			if(!setConstrainedStringOption(hSaneHandle, option, index, lineart_scan_modes, nullptr)) {
				printf("Unable to set Lineart mode, please file a bug");
			}
			break;
		default:
			break;
		}
	}
}

void wxScanSane::SetOption(const int &dpi, const Scanner::ScanMode &scanMode)
{
    this->scanMode = scanMode;
    this->dpi =dpi;
}

// Get an image from the scanning device.
//
bool wxScanSane::ScanImage( wxImage& oImage )
{
    // Check if there is a proper device index.
    if( !IsDeviceAvailable() )
    {
        // Log and signal error.
        printf( wxString( wxT( "bool wxScan::ScanImage() - " ) )
                      + _( "SANE device not available." ) );
        return false;
    }

#if defined( __WXSCANDEBUG__ ) && __WXSCANDEBUG__ >= 3
    for( int i= 0; i < m_nDeviceCount; i++ )
    {
        if( i != m_nDeviceIndex )
        {
            printf( wxT( "SANE DEVICE" ) );
            printf( wxT( "\tname == " )   + GetDeviceName(   i ) );
            printf( wxT( "\tvendor == " ) + GetDeviceVendor( i ) );
            printf( wxT( "\tmodel == " )  + GetDeviceModel(  i ) );
            printf( wxT( "\ttype == " )   + GetDeviceType(   i ) );
            printf( wxT( "============" ) );
        }
    }
    ::wxSafeYield();
#endif

#if defined( __WXSCANDEBUG__ ) && __WXSCANDEBUG__ >= 1
    printf( wxT( "DEFAULT SANE DEVICE" ));
    printf( wxT( "\tname == " )   + GetDeviceName(   m_nDeviceIndex ) );
    printf( wxT( "\tvendor == " ) + GetDeviceVendor( m_nDeviceIndex ) );
    printf( wxT( "\tmodel == " )  + GetDeviceModel(  m_nDeviceIndex ) );
    printf( wxT( "\ttype == " )   + GetDeviceType(   m_nDeviceIndex ) );
    printf( wxT( "============" ) );
    ::wxSafeYield();
#endif

    Open();

    
    if( !IsOk() )
    {
        // Log and signal error.
        printf( GetSaneStatusString() + wxT( " (SANE)" ) );
        return false;
    }
    
    SetDPIModeOption(hSaneHandle);  

    if( !IsOk() )
    {
        // Log and signal error.
        printf( GetSaneStatusString() + wxT( " (SANE)" ) );
        return false;
    }  
    
    SetScanModeOption(scanMode, hSaneHandle);
    
    if( !IsOk() )
    {
        // Log and signal error.
        printf( GetSaneStatusString() + wxT( " (SANE)" ) );
        return false;
    }
    
    SetPageOption(hSaneHandle);
    
    if( !IsOk() )
    {
        // Log and signal error.
        printf( GetSaneStatusString() + wxT( " (SANE)" ) );
        return false;
    }
    
    /*
    SetOptionToMax(scanMode, hSaneHandle);
    
    if( !IsOk() )
    {
        // Log and signal error.
        printf( GetSaneStatusString() + wxT( " (SANE)" ) );
        return false;
    }
     * */
    
    // Get scanning parameters e. g. width, height, color depth etc.
    SANE_Parameters oSaneParameters;

    //::sane_get_parameters( hSaneHandle, &oSaneParameters );
    

    // ... and start scanning.
    m_SaneStatus= ::sane_start( hSaneHandle );
    if( !IsOk() )
    {
        // Log and signal error.
        printf( GetSaneStatusString() + wxT( " (SANE)" ) );
        return false;
    }
    
    ::sane_get_parameters( hSaneHandle, &oSaneParameters );


#if defined( __WXSCANDEBUG__ ) && __WXSCANDEBUG__ >= 1
    printf( wxT( "SCANNING PARAMETERS" ) );
    printf( wxString::Format( wxT( "\tformat = %d" ), oSaneParameters.format ) );
    printf( wxString::Format( wxT( "\tlast_frame = %d" ), oSaneParameters.last_frame ) );
    printf( wxString::Format( wxT( "\tbytes_per_line = %d" ), oSaneParameters.bytes_per_line ) );
    printf( wxString::Format( wxT( "\tpixels_per_line = %d" ), oSaneParameters.pixels_per_line ) );
    printf( wxString::Format( wxT( "\tlines = %d" ), oSaneParameters.lines ) );
    printf( wxString::Format( wxT( "\tdepth = %d" ), oSaneParameters.depth ) );
    printf( wxT( "============" ) );
    ::wxSafeYield();
#endif

    // Check format information.
    //
    // Note: At the moment we only support RGB/8-bit-per-pixel scans.
    /*
    if(
           !oSaneParameters.last_frame
        || ( oSaneParameters.format == SANE_FRAME_RGB || oSaneParameters.format == SANE_FRAME_GRAY)
        || ( oSaneParameters.depth != oSaneParameters.depth )
      )
    {
        // Log and signal error.
        printf( wxString( wxT( "bool wxScan::ScanImage() - " ) )
                      + _( "Formats other than RGB with 8 bit per pixel are not yet supported." ) );
        return false;
    }
    */

    
    if(!oSaneParameters.last_frame || !( oSaneParameters.format == SANE_FRAME_RGB || oSaneParameters.format == SANE_FRAME_GRAY))
    {
        // Log and signal error.
        printf( wxString( wxT( "bool wxScan::ScanImage() - " ) )
                      + _( "Formats other than RGB with 8 bit per pixel are not yet supported." ) );
        return false;
    }
    // Create scan buffer.
    int width = oSaneParameters.pixels_per_line;
    int height = oSaneParameters.lines;
    
    int nImageBufferSize= 0;
    if(oSaneParameters.format == SANE_FRAME_RGB)
        nImageBufferSize = ( oSaneParameters.pixels_per_line * oSaneParameters.lines * 3 + 1 ) * sizeof( SANE_Byte );
    else if(oSaneParameters.format == SANE_FRAME_GRAY)
        nImageBufferSize = ( oSaneParameters.pixels_per_line * oSaneParameters.lines + 1 ) * sizeof( SANE_Byte );

    SANE_Byte *pScanBuffer= (SANE_Byte *)malloc( nImageBufferSize );

    if( !pScanBuffer )
    {
        // Log and signal error.
        printf( wxString( wxT( "bool wxScan::ScanImage() - " ) )
                      + _( "Cannot allocate read buffer." ) );
        return false;
    }

    // Read image from scanner.
    SANE_Int nScannedBytes;
    SANE_Byte *pScanBufferPtr= pScanBuffer;
	SANE_Word total_bytes = 0;
	wxProgressDialog dialog("Scanning Progress", "Receive data", nImageBufferSize, NULL, wxPD_APP_MODAL | wxPD_CAN_ABORT);

    do
    {

        m_SaneStatus= ::sane_read( hSaneHandle, pScanBufferPtr, nImageBufferSize, &nScannedBytes );
        pScanBufferPtr += nScannedBytes * sizeof( SANE_Byte );
		total_bytes += (SANE_Word)nScannedBytes;
		if (false == dialog.Update(total_bytes, "Receive data"))
			break;

    } while( ( m_SaneStatus != SANE_STATUS_EOF ) && ( m_SaneStatus == SANE_STATUS_GOOD ) ) ;
    
	
	if( ( m_SaneStatus !=  SANE_STATUS_EOF ) )
    {
        // Log, clean up, and signal error.
        printf( wxString( wxT( "bool wxScan::ScanImage() - " ) )
                      + _( "An error occured while fetching data from scanning device." ) );
        ::sane_cancel( hSaneHandle );
        free( pScanBuffer );
        return false;
    }
    m_SaneStatus= SANE_STATUS_GOOD;

    // Save the image in the result image.
    if(oSaneParameters.format == SANE_FRAME_RGB)
    {
        wxImage oNewImage( oSaneParameters.pixels_per_line, oSaneParameters.lines, pScanBuffer );
        oImage= oNewImage;
    }
    else   
    {
        wxImage oNewImage(width, height);
        int pointer = 0;
        unsigned char *imgRGB = oNewImage.GetData();    // destination RGB buffer
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                SANE_Byte value = *pScanBuffer++;
                if(oSaneParameters.depth == 1)
                {
                    if(value == 1)
                        value = 255;
                }
                imgRGB[pointer++] = value;
                imgRGB[pointer++] = value;
                imgRGB[pointer++] = value;   // B
            }
        }
        oImage= oNewImage;
    }

    

    // Get the image resoltuion;
    m_nResolution= -1;

    int nOptionCount;
    const SANE_Option_Descriptor *oSaneOptionDescriptor;

    ::sane_control_option( hSaneHandle, 0, SANE_ACTION_GET_VALUE, &nOptionCount, NULL );
    for( int i= 0; i < nOptionCount; i++ )
    {
            oSaneOptionDescriptor= ::sane_get_option_descriptor( hSaneHandle, i );

            wxString strOption( SANE_STRING_TO_WXSTRING( oSaneOptionDescriptor->name ) );

#if defined( __WXSCANDEBUG__ ) && __WXSCANDEBUG__ >= 2
            printf( wxT( "DEVICE OPTION DESCRIPTOR %d" ), i );
            printf( strOption + wxString::Format( wxT( "(%d):" ), oSaneOptionDescriptor->size ) );
            printf( wxT( "\t" ) + SANE_STRING_TO_WXSTRING( oSaneOptionDescriptor->title ) );
            printf( wxT( "\t" ) + SANE_STRING_TO_WXSTRING( oSaneOptionDescriptor->desc  ) );
#endif
            if( 0 == strOption.CompareTo( wxT( "resolution" ) ) )
            {
                switch( oSaneOptionDescriptor->type )
                {
                    case SANE_TYPE_INT :    {
                                                int nValue;

                                                ::sane_control_option( hSaneHandle, i, SANE_ACTION_GET_VALUE, &nValue, NULL );
                                                m_nResolution= (int)nValue;
                                            }
                                            break;
                    case SANE_TYPE_FIXED :  {
                                                SANE_Fixed   nValue;

                                                ::sane_control_option( hSaneHandle, i, SANE_ACTION_GET_VALUE, &nValue, NULL );
                                                m_nResolution= (int)nValue;
                                            }
                                            break;
                    default :               {
                                                // [Nothing to do, yet.]
                                            }
                }
#if defined( __WXSCANDEBUG__ ) && __WXSCANDEBUG__ >= 1
                printf( wxT( "\tresolution == %d DPI" ), m_nResolution );
#endif
            }
#if defined( __WXSCANDEBUG__ ) && __WXSCANDEBUG__ >= 2
            printf( wxT( "============" ) );
#endif
    }

    if( m_nResolution > 0 )
    {
        oImage.SetOption( wxIMAGE_OPTION_RESOLUTIONUNIT, wxIMAGE_RESOLUTION_INCHES );
        oImage.SetOption( wxIMAGE_OPTION_RESOLUTIONX,    m_nResolution );
        oImage.SetOption( wxIMAGE_OPTION_RESOLUTIONY,    m_nResolution );
        oImage.SetOption( wxIMAGE_OPTION_RESOLUTION,     m_nResolution );
    }

    Close();

    // Return the state.
    return IsOk();
}

// Select the scanning device by index number.
//
bool wxScanSane::SetDeviceIndex( int nIndex )
{
    int nOldIndex= m_nDeviceIndex;

    m_nDeviceIndex= nIndex;

    bool nRetCode= IsDeviceAvailable();

    if( !nRetCode );
    {
        m_nDeviceIndex= nOldIndex;
    }

    return nRetCode;
}

int wxScanSane::GetSelectedIndex()
{
    return m_selDevice;
}


bool wxScanSane::Open()
{
    if(!IsSourceSelected() || isClose)
    {
        // Open the sane device...
        m_SaneStatus= ::sane_open(GetDeviceName(m_nDeviceIndex).ToUTF8(), &hSaneHandle );
        if( !IsOk() )
        {
            // Log and signal error.
            printf( GetSaneStatusString() + wxT( " (SANE)" ) );
            return false;
        }
        isClose = false;
    }
    return true;
}   

bool wxScanSane::Close()
{
    if(IsSourceSelected() && !isClose)
    {
        // Close the sane device.
        ::sane_close( hSaneHandle );
        isClose = true;
        
    }
    return true;
}

bool wxScanSane::IsSourceSelected()
{
    return hSaneHandle != 0;
}

SANE_Status wxScanSane::SaneControlOption(SANE_Int option, SANE_Action action,
    void *value, SANE_Int *info)
{
    return m_SaneStatus = sane_control_option(hSaneHandle, option, action, value, info);
}

const SANE_Option_Descriptor *wxScanSane::SaneGetOptionDescriptor(SANE_Int option)
{
    Open();
    return sane_get_option_descriptor(hSaneHandle, option);
}

bool wxScanSane::SelectSource(const wxString &name, const bool &showUI, wxWindow *parent)
{
    wxString selName = name;
    m_selDevice = -1;

    if (m_nDeviceCount == 0)
        SaneGetDevices();

    if (m_nDeviceCount <= 0) {
        wxMessageBox(_("No devices available"), "Regards Viewer",
            wxOK | wxICON_EXCLAMATION | wxCENTRE);
            return wxSCAN_RC_NOSOURCE;
    }

    wxArrayString sources;
    sources.Alloc(m_numDevices);

    unsigned int sel = 0;
    for (unsigned int i = 0; i < m_nDeviceCount; i++)
    {
        sources.Add(wxString::Format("%s %s", m_ppDeviceList[i]->vendor, m_ppDeviceList[i]->model));
        if (name == wxString(m_ppDeviceList[i]->name).Strip())
            sel = i;
    }

    if (showUI)
    {
        wxSingleChoiceDialog d(parent, _("Available Devices:"),
            _("Select Source"), sources);
        d.SetSelection(sel);

        if(d.ShowModal() != wxID_OK)
            return wxSCAN_RC_CANCELLED;
        sel = d.GetSelection();
        selName = wxString(m_ppDeviceList[sel]->name).Strip();
   }
    else if(!(sel <= m_numDevices))
        return wxSCAN_RC_NOSOURCE;


    Open();
    if( !IsOk() )
    {
        // Log and signal error.
        printf( GetSaneStatusString() + wxT( " (SANE)" ) );
        return false;
    }
    m_selDevice = sel;       

    return true;
}

// Fill the SANE device list.
//
bool wxScanSane::SaneGetDevices()
{
    // Get the device list from sane subsystem...
    m_SaneStatus= ::sane_get_devices( &m_ppDeviceList, m_bOnlyLocalDevices );
    if( !IsOk() )
    {
        // Log error.
        printf( GetSaneStatusString() + wxT( " (SANE)" ) );
        return false ;
    }

    // ... and count the devices.
    for( m_nDeviceCount=0; m_ppDeviceList[m_nDeviceCount]; m_nDeviceCount++ )
    {
    }

    // Initialize device index (default device);
    m_nDeviceIndex= m_nDeviceCount > 0 ? 0 : -1;

    // Return the state.
    return IsOk();
}
#endif // __UNIX_LIKE__

#if defined( __WXMSW__ )
// Standard constructor.
//
// ...
#endif // __WXMSW__
