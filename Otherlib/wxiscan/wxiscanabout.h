00001 /***************************************************************
00002  * Name:      wxiscanabout.h
00003  * Purpose:   Defines About Box Class.
00004  * Author:    Daniel Nell (daniel.nell@nellresearch.de)
00005  * Created:   2009-06-12
00006  * Copyright: Daniel Nell (www.nellresearch.de)
00007  * License:   GPL
00008  **************************************************************/
00009 
00010 #ifndef WXISCANABOUT_H
00011 #define WXISCANABOUT_H
00012 
00013 #ifndef WX_PRECOMP
00014     //(*HeadersPCH(wxIScanAbout)
00015     #include <wx/dialog.h>
00016     #include <wx/sizer.h>
00017     //*)
00018 #endif
00019 //(*Headers(wxIScanAbout)
00020 #include <wx/html/htmlwin.h>
00021 //*)
00022 
00023 
00024 //////////////////////////////////////////////////////////
00025 // Strings and other (global) constants
00026 //
00027 extern const wxString cstrAppName;          ///< Application name string.
00028 extern const wxString cstrAppDescription;   ///< Short application description string.
00029 extern const wxString cstrVendor;           ///< Vendor information.
00030 extern const wxString cstrStatusCopyRight;  ///< Copyright string in the status bar.
00031 extern const wxString cstrVersionString;    ///< Version information.
00032 extern const wxString cstrWebSite;          ///< Web site string.
00033 extern const wxString cstrWxVersionString;  ///< wxWidgets version information.
00034 #if __WXLUASCRIPT__
00035 extern const wxString cstrLuaVersion;       ///< Lua version string.
00036 #endif // __WXLUASCRIPT__
00037 
00038 
00039 //////////////////////////////////////////////////////////
00040 // Class wxIScanAbout
00041 //
00042 /// \brief The program's about dialog.
00043 ///
00044 /// This Dialog shows information on this application and the licenses it is based
00045 /// on. The main part is a wxHtmlWindow (derived from wxHtmlWindow) so you can click
00046 /// on most elements and a browser opens the corresponding link.
00047 class wxIScanAbout: public wxDialog
00048 {
00049     public:
00050         /// \brief Standard constructor.
00051         ///
00052         /// \param poParent     a pointer to the parent window
00053         /// \param nId          the window id, defaults to wxID_ANY
00054         wxIScanAbout( wxWindow* poParent, wxWindowID nId= wxID_ANY );
00055 
00056         /// \brief Virtual destructor.
00057         virtual ~wxIScanAbout();
00058 
00059         //(*Declarations(wxIScanAbout)
00060         //*)
00061 
00062     private:
00063         //(*Identifiers(wxIScanAbout)
00064         //*)
00065 
00066         //(*Handlers(wxIScanAbout)
00067         void OnInit(wxInitDialogEvent& event);
00068         //*)
00069 
00070         DECLARE_EVENT_TABLE()
00071 };
00072 
00073 
00074 //////////////////////////////////////////////////////////
00075 // Class wxLinkInBrowserHtmlWindow
00076 //
00077 /// \brief A (slightly) extended html window class.
00078 ///
00079 /// It extends wxHtmlWindow in the way that clicks on links are caught and an
00080 /// external browser is started with the link instead of onpening the link in
00081 /// the html window itself.
00082 class wxLinkInBrowserHtmlWindow : public wxHtmlWindow
00083 {
00084   public:
00085     /// \brief Standard constructor copied from wxHtmlWindow.
00086     ///
00087     /// \param poParent     a pointer to the parent window
00088     /// \param nId          the window id, defaults to wxID_ANY
00089     /// \param oPos         the window position on the screen, defaults to wxDefaultPosition
00090     /// \param oSize        the window size, defaults to wxDefaultSize
00091     /// \param nStyle       the (html) window style, defaults to wxHW_DEFAULT_STYLE
00092     /// \param strName      the class name, defaults to "wxLinkInBrowserHtmlWindow"
00093     wxLinkInBrowserHtmlWindow(
00094                     wxWindow *poParent,
00095                     wxWindowID nId = wxID_ANY,
00096                     const wxPoint& oPos= wxDefaultPosition,
00097                     const wxSize& oSize= wxDefaultSize,
00098                     long nStyle= wxHW_DEFAULT_STYLE,
00099                     const wxString& strName= wxT( "wxLinkInBrowserHtmlWindow" )
00100                 )
00101       : wxHtmlWindow( poParent, nId, oPos, oSize, nStyle, strName )
00102     {
00103     }
00104 
00105   private:
00106     /// \brief Handler for clicking on a link events.
00107     ///
00108     /// It passes the link to an external browser (the system's default browser).
00109     /// This prevents the html window (e. g. an about dialog) to show the linked
00110     /// page itself.
00111     void OnLinkClicked( wxHtmlLinkEvent &oEvent )
00112     {
00113         ::wxLaunchDefaultBrowser( oEvent.GetLinkInfo().GetHref() );
00114     }
00115 
00116   private:
00117     DECLARE_EVENT_TABLE()
00118 };
00119 #endif

