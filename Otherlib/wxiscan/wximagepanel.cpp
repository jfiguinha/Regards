00001 /***************************************************************
00002  * Name:      wximagepanel.cpp
00003  * Purpose:   Side panel for PDF document priviewing.
00004  * Author:    Daniel Nell (daniel.nell@nellresearch.de)
00005  * Created:   2011-01-26
00006  * Copyright: Daniel Nell (www.nellresearch.de)
00007  * License:   GPL
00008  **************************************************************/
00009 
00010 // Include precompiled headers.
00011 #include "wx_pch.h"
00012 
00013 // wxWidgets headers
00014 #include <wx/config.h>
00015 #include <wx/hashmap.h>
00016 #include <wx/statbmp.h>
00017 
00018 // Private headers
00019 #include "wximagepanel.h"
00020 
00021 
00022 //////////////////////////////////////////////////////////
00023 // Class wxImagePanel
00024 //
00025 // Event table
00026 //
00027 BEGIN_EVENT_TABLE( wxImagePanel, wxScrolledWindow )
00028     EVT_MENU( wxExtThread::ID_EXIT, wxImagePanel::OnThreadExit )
00029 END_EVENT_TABLE()
00030 
00031 
00032 // Standard constructor
00033 //
00034 wxImagePanel::wxImagePanel( wxWindow *poParent, wxWindowID nId,
00035                             const wxPoint &oPos, const wxSize &oSize,
00036                             long nStyle, const wxString& strName )
00037 : wxScrolledWindow( poParent, nId, oPos, oSize, nStyle, strName )
00038 {
00039     // Empty the panel.
00040     Reset();
00041 
00042     // Reset window settings.
00043     wxConfigBase *poConfig = wxConfigBase::Get();
00044     poConfig->SetRecordDefaults();
00045     poConfig->SetPath( wxT( "/" )  );
00046     poConfig->SetPath( GetName() );
00047 
00048     // Reset bitmap and spacer size.
00049     m_nPanelPagesBitmapSize= poConfig->Read(  wxT( "PanelPagesBitmapSize" ),  160 );
00050     m_nPanelPagesBitmapSpace= poConfig->Read( wxT( "PanelPagesBitmapSpace" ),   6 );
00051 }
00052 
00053 // Virtual destructor
00054 //
00055 wxImagePanel::~wxImagePanel()
00056 {
00057     // Save configuration data.
00058     wxConfigBase *poConfig= wxConfigBase::Get();
00059 
00060     poConfig->SetPath( wxT( "/" ) );
00061     poConfig->SetPath( GetName() );
00062 
00063     // Save bitmap and spacer size.
00064     poConfig->Write( wxT( "PanelPagesBitmapSize" ), (long)m_nPanelPagesBitmapSize );
00065     poConfig->Write( wxT( "PanelPagesBitmapSpace" ), (long)m_nPanelPagesBitmapSpace );
00066 }
00067 
00068 // Add images to the panel.
00069 //
00070 void wxImagePanel::AddImage( wxImage oImage )
00071 {
00072     m_oImageHash[m_nPanelPagesCount]= oImage;
00073     wxImagePanelThread * poThread=new wxImagePanelThread( this, m_nPanelPagesCount );
00074 
00075     poThread->Create();             // Create the thread instance.
00076     poThread->SetPriority( 0 );     // Set the thread to the lowest possible priority.
00077     poThread->Run();                // Run the the thread.
00078     m_nPanelPagesCount++;
00079 }
00080 
00081 // Create a thumbnail from the corresponding image in another thread.
00082 //
00083 void wxImagePanel::OnThreadEntry( int nId )
00084 {
00085     wxImage oImage= m_oImageHash[nId];
00086 
00087     double xfactor= (double)m_nPanelPagesBitmapSize
00088                     / (double)( ( oImage.GetWidth() > oImage.GetHeight() ) ? oImage.GetWidth() : oImage.GetHeight() );
00089 
00090     if( xfactor < 1.0 )
00091     {
00092         oImage.Rescale( (int)( (double)oImage.GetWidth()  * xfactor ),
00093                         (int)( (double)oImage.GetHeight() * xfactor ),
00094                         wxIMAGE_QUALITY_HIGH );
00095         m_oImageHash[nId]= oImage;
00096     }
00097 }
00098 
00099 // Handle the exit of another thread.
00100 //
00101 void wxImagePanel::OnThreadExit( wxCommandEvent& oEvent )
00102 {
00103     int nId= oEvent.GetInt();
00104 
00105     wxImage oImage= m_oImageHash[nId];
00106 
00107     m_oImageHash.erase( nId );
00108 
00109     wxPoint oStartingPoint( m_nPanelPagesBitmapSpace,
00110                             m_nPanelPagesBitmapSpace + ( m_nPanelPagesBitmapSpace + m_nPanelPagesBitmapSize ) * nId );
00111 
00112     SetScrollbars( 1, 1,
00113                    m_nPanelPagesBitmapSpace + m_nPanelPagesBitmapSize + m_nPanelPagesBitmapSpace,
00114                    m_nPanelPagesBitmapSpace + ( m_nPanelPagesBitmapSpace + m_nPanelPagesBitmapSize ) * m_nPanelPagesCount );
00115 
00116     new wxStaticBitmap( this, wxID_ANY, wxBitmap( oImage ), oStartingPoint );
00117 //    new wxBitmapButton( this, wxID_ANY, wxBitmap( oImage ), oStartingPoint );
00118 
00119 #if wxCHECK_VERSION( 2, 9, 0 )
00120 #else  // wxCHECK_VERSION( 2, 9, 0 )
00121     Scroll( 0, m_nPanelPagesBitmapSpace + ( m_nPanelPagesBitmapSpace + m_nPanelPagesBitmapSize ) * ( m_nPanelPagesCount - 1 ) );
00122 #endif // wxCHECK_VERSION( 2, 9, 0 )
00123 }
00124 

