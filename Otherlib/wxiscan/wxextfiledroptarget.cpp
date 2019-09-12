00001 /////////////////////////////////////////////////////////////////////////////
00002 // Project:     wxIView
00003 // Purpose:     Complex wxWidgets sample
00004 // Name:        wxextfiledroptarget.cpp
00005 // Author:      Daniel Nell
00006 // Created:     2006/02/16
00007 // Copyright:   (c) Daniel Nell
00008 // Licence:     wxWindows license
00009 // Modified by:
00010 /////////////////////////////////////////////////////////////////////////////
00011 
00012 // For compilers that support precompilation, includes "wx/wx.h".
00013 #include <wx/wxprec.h>
00014 #ifndef WX_PRECOMP
00015 #   include <wx/wx.h>
00016 #endif
00017 
00018 // Include private wxIView headers.
00019 #include "wxextfiledroptarget.h"
00020 
00021 
00022 //////////////////////////////////////////////////////////
00023 // Class wxExtFileDropTarget
00024 //
00025 DEFINE_EVENT_TYPE( wxEVT_EXTFILEDROP )
00026 
00027 // Handler for file dropping.
00028 //
00029 bool wxExtFileDropTarget::OnDropFiles( wxCoord WXUNUSED( x ),
00030                                        wxCoord WXUNUSED( y ),
00031                                        const wxArrayString& astrFilenames )
00032 {
00033     // Check if there is a valid owner window.
00034     if( m_poOwner == NULL )
00035     {
00036         // Otherwise quit and signal an error.
00037         return false;
00038     }
00039 
00040     // File drop event to post to the owner window.
00041     wxExtFileDropEvent oEvent;
00042 
00043     // Get the number of items that are to be dropped...
00044     unsigned nCount= astrFilenames.GetCount();
00045 
00046     // ... and open them one by one using the owner window's event process chain.
00047     for( unsigned i= 0; i < nCount; i++ )
00048     {
00049         // Forward a file drop event with the current file name to the owner window.
00050         oEvent.SetFileName( astrFilenames[i] );
00051 
00052         m_poOwner->ProcessEvent( oEvent );
00053     }
00054 
00055     // Signal success.
00056     return true;
00057 }

