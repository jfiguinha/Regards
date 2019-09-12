00001 /***************************************************************
00002  * Name:      wxiscanapp.cpp
00003  * Purpose:   Code for Application Class
00004  * Author:    Daniel Nell (daniel.nell@nellresearch.de)
00005  * Created:   2008-05-04
00006  * Copyright: Daniel Nell (www.nellresearch.de)
00007  * License:   GPL
00008  **************************************************************/
00009 
00010 #include "wx_pch.h"
00011 
00012 //(*AppHeaders
00013 #include "wxiscanframe.h"
00014 #include <wx/image.h>
00015 //*)
00016 
00017 // wxWidgets headers
00018 #include <wx/cmdline.h>
00019 #include <wx/config.h>
00020 #include "wx/fileconf.h"
00021 #include <wx/filename.h>
00022 #include <wx/splash.h>
00023 
00024 // Private headers
00025 #include "wxiscanapp.h"
00026 #if wxUSE_PDF
00027 #   include "wxpoppler.h"
00028 #endif // wxUSE_PDF
00029 #include "wxiscanresources.h"
00030 
00031 
00032 //////////////////////////////////////////////////////////
00033 // The application object.
00034 //
00035 // Implement the wxWindows initialization.
00036 //
00037 IMPLEMENT_APP( wxIScanApp );
00038 
00039 
00040 // Application initialization.
00041 //
00042 // Program initialization.
00043 //
00044 bool wxIScanApp::OnInit()
00045 {
00046     // Make sure that the settings are written to a configuration file
00047     // (instead of e. g. the registry on MSW).
00048     wxConfigBase::Set( new wxFileConfig );
00049 
00050     // Get application configuration.
00051     wxConfigBase *poConfig = wxConfigBase::Get();
00052 
00053     poConfig->SetRecordDefaults();
00054     poConfig->SetPath( wxT( "/Global" ) );
00055     poConfig->Write( wxT( "App" ), argv[0] );
00056     poConfig->Write( wxT( "Locale" ), wxLocale::GetLanguageInfo( wxLocale::GetSystemLanguage() )->CanonicalName );
00057 
00058     // Display splash screen.
00059     if( poConfig->Read( wxT( "Splashscreen" ), (long)true ) )
00060     {
00061         new wxSplashScreen( wxBitmap( wxiscansplash_xpm ), wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT, 500, NULL, wxID_ANY );
00062     }
00063 
00064     // Set locale.
00065     if( poConfig->Read( wxT( "I18n" ), (long)true ) )
00066     {
00067 #ifdef __UNIX__
00068         // Add lookup path for subdirectories direct below the
00069         // application's installation directory.
00070         //
00071         // NOTE: We do this because Un*x like OSes do not search
00072         //       the application's directory automatically!
00073         //
00074         wxFileName oFileName( argv[0] );
00075 
00076         oFileName.Normalize();
00077         m_oLocale.AddCatalogLookupPathPrefix( oFileName.GetPath() );
00078 #endif // __UNIX__
00079 
00080         // Initialize locale.
00081         m_oLocale.Init( wxLocale::GetSystemLanguage() );
00082 
00083         // Add application catalog.
00084         m_oLocale.AddCatalog( GetAppName() );
00085 
00086         // Add standard wxWidgets translation catalog.
00087         m_oLocale.AddCatalog( wxT( "wxstd" ) );
00088     }
00089 
00090 
00091    // Install image handlers.
00092     AddImageHandler( new wxBMPHandler );
00093 #if wxUSE_LIBPNG
00094     AddImageHandler( new wxPNGHandler );
00095 #endif
00096 #if wxUSE_LIBJPEG
00097     AddImageHandler( new wxJPEGHandler );
00098 #endif
00099 #if wxUSE_LIBTIFF
00100     AddImageHandler( new wxTIFFHandler );
00101 #endif
00102 #if wxUSE_GIF
00103     AddImageHandler( new wxGIFHandler );
00104 #endif
00105 #if wxUSE_PCX
00106     AddImageHandler( new wxPCXHandler );
00107 #endif
00108 #if wxUSE_PNM
00109     AddImageHandler( new wxPNMHandler );
00110 #endif
00111 #if wxUSE_XPM
00112     AddImageHandler( new wxXPMHandler );
00113 #endif
00114 #if wxUSE_ICO_CUR
00115     AddImageHandler( new wxICOHandler );
00116     AddImageHandler( new wxCURHandler );
00117     AddImageHandler( new wxANIHandler );
00118 #endif
00119 #if wxUSE_PDF
00120     AddImageHandler( new wxPDFHandler );
00121 #endif
00122 
00123     m_poFrame = new wxIScanFrame( NULL );
00124     if( m_poFrame == NULL )
00125     {
00126         ::wxLogFatalError( _( "Failed to create main window." ) );
00127 
00128         // Program termination.
00129         return false;
00130     }
00131     m_poFrame->Show();
00132     m_poFrame->RestoreSashSettings();
00133     SetTopWindow( m_poFrame );
00134 
00135     // Call the base class's OnInit() function for commandline processing,
00136     // and do some error checking.
00137     //
00138     // Note: A return value of 'false' will cause the application to terminate
00139     //       immediately!
00140     //
00141     if( !wxApp::OnInit() )
00142     {
00143         // Program termination.
00144         return false;
00145     }
00146 
00147     return true;
00148 }
00149 
00150 // Command line handling functions.
00151 //
00152 // Initialize command line parameter handling.
00153 //
00154 void wxIScanApp::OnInitCmdLine( wxCmdLineParser& oParser )
00155 {
00156     // Call the base class's OnInitCmdLine function for default
00157     // parameter handling ( --help).
00158     wxApp::OnInitCmdLine( oParser );
00159 
00160     // Add an optional filename parameter to the parameter list.
00161     oParser.AddParam( wxT( "[Filename #1] [... [Filename #n]]]]" ),
00162                       wxCMD_LINE_VAL_STRING,
00163                       wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_PARAM_MULTIPLE );
00164 }
00165 
00166 // Handle filenames on the command line.
00167 //
00168 bool wxIScanApp::OnCmdLineParsed( wxCmdLineParser& oParser )
00169 {
00170     // Call the base class's OnCmdLineParsed function for default
00171     // parameter handling.
00172     if( !wxApp::OnCmdLineParsed( oParser ) )
00173     {
00174         // Return error.
00175         return false;
00176     }
00177 
00178     // Handle filename parameters by calling the top window's
00179     // OpemImage() function.
00180     int nCount= oParser.GetParamCount();
00181 
00182     for( int i= 0; i < nCount; i++ )
00183     {
00184         m_poFrame->OpenImage( oParser.GetParam( i ) );
00185     }
00186 
00187     // Return OK.
00188     return true;
00189 }
00190 
00191 // Image handler and extension filter handling.
00192 //
00193 // Adds image handlers to the image handler list.
00194 //
00195 void wxIScanApp::AddImageHandler( wxImageHandler* poHandler )
00196 {
00197     if( poHandler )
00198     {
00199         wxString strExtension= poHandler->GetExtension();
00200 
00201         // TODO: Is there a posibility to get files by extension fully
00202         //       case insensitive?
00203         //
00204 #ifdef __WXMSW__
00205         m_strImageFilter += wxT( "*." ) + strExtension + wxT( ";" );
00206         m_strImageFilterList += poHandler->GetMimeType() + wxT( " (*." )
00207                                 + strExtension
00208                                 + wxT( ")|*." ) + strExtension + wxT( "|" );
00209 #else
00210         m_strImageFilter += wxT( "*." ) + strExtension.Lower() + wxT( ";" )
00211                          +  wxT( "*." ) + strExtension.Upper() + wxT( ";" );
00212         m_strImageFilterList += poHandler->GetMimeType() + wxT( " (*." )
00213                                 + strExtension
00214                                 + wxT( ")|*." )
00215                                 + strExtension.Lower()
00216                                 + wxT( ";*." )
00217                                 + strExtension.Upper()
00218                                 + wxT( "|" );
00219 #endif
00220         if( wxImage::FindHandler( poHandler->GetName() ) )
00221         {
00222             delete poHandler;
00223         }
00224         else
00225         {
00226             wxImage::AddHandler( poHandler );
00227         }
00228     }
00229 }
00230 
00231 // Get the file extension filter for all registered image handlers.
00232 //
00233 wxString wxIScanApp::GetImageFilter()
00234 {
00235     return _( "All image files" ) + wxString( wxT( "|" ) )
00236            + m_strImageFilter + wxT( "|" )
00237            + m_strImageFilterList
00238            + _( "All files" ) + wxT( "|*.*" );
00239 }

