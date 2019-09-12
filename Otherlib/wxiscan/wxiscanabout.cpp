00001 /***************************************************************
00002  * Name:      wxiscanabout.cpp
00003  * Purpose:   Defines About Box Class.
00004  * Author:    Daniel Nell (daniel.nell@nellresearch.de)
00005  * Created:   2009-06-12
00006  * Copyright: Daniel Nell (www.nellresearch.de)
00007  * License:   GPL
00008  **************************************************************/
00009 
00010 // Include precompiled headers.
00011 #include "wx_pch.h"
00012 
00013 #ifndef WX_PRECOMP
00014     //(*InternalHeadersPCH(wxIScanAbout)
00015     #include <wx/string.h>
00016     #include <wx/intl.h>
00017     //*)
00018 #endif
00019 //(*InternalHeaders(wxIScanAbout)
00020 #include <wx/button.h>
00021 //*)
00022 
00023 // wxWidgets headers
00024 #include <wx/dialog.h>
00025 
00026 // Private headers
00027 #include "wxiscan.h"
00028 #include "wxiscanabout.h"
00029 #if __WXLUASCRIPT__
00030 #   include "wxluascript.h"
00031 #endif // __WXLUASCRIPT__
00032 
00033 
00034 //(*IdInit(wxIScanAbout)
00035 //*)
00036 
00037 
00038 //////////////////////////////////////////////////////////
00039 // Strings and other (global) constants
00040 //
00041 const wxString cstrAppName=            wxT( PRODUCT_NAME );
00042 const wxString cstrAppDescription=     wxT( FILE_DESCRIPTION );
00043 const wxString cstrVendor=             wxT( COMPANY_NAME );
00044 const wxString cstrStatusCopyRight=    wxT( LEGAL_COPYRIGHT );
00045 const wxString cstrVersionString=      wxString::Format( wxT( "%d.%d.%d (Build %d)" ),
00046                                                          AutoVersion::MAJOR,
00047                                                          AutoVersion::MINOR,
00048                                                          AutoVersion::BUILD,
00049                                                          AutoVersion::BUILDS_COUNT );
00050 const wxString cstrWebSite=            wxT( WEB_SITE );
00051 const wxString cstrWxVersionString=    wxVERSION_STRING;
00052 #if __WXLUASCRIPT__
00053 const wxString cstrLuaVersion=         wxT( LUA_VERSION );
00054 #endif // __WXLUASCRIPT__
00055 
00056 
00057 //////////////////////////////////////////////////////////
00058 // Class wxIScanAbout
00059 //
00060 // Event table
00061 //
00062 BEGIN_EVENT_TABLE( wxIScanAbout, wxDialog )
00063     //(*EventTable(wxIScanAbout)
00064     //*)
00065 END_EVENT_TABLE()
00066 
00067 // Standard constructor.
00068 //
00069 wxIScanAbout::wxIScanAbout( wxWindow* parent, wxWindowID id )
00070 {
00071     //(*Initialize(wxIScanAbout)
00072     wxFlexGridSizer* FlexGridSizer1;
00073     wxLinkInBrowserHtmlWindow* poHtmlWindow;
00074     wxStdDialogButtonSizer* poStdDialogButtonSizer;
00075 
00076     Create(parent, id, _("About"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
00077     SetClientSize(wxSize(460,510));
00078     FlexGridSizer1 = new wxFlexGridSizer(2, 1, 0, 0);
00079     poHtmlWindow = new wxLinkInBrowserHtmlWindow(this, wxID_ANY, wxDefaultPosition, wxSize(450,460), wxHW_SCROLLBAR_AUTO, _T("wxID_ANY"));
00080     FlexGridSizer1->Add(poHtmlWindow, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
00081     poStdDialogButtonSizer = new wxStdDialogButtonSizer();
00082     poStdDialogButtonSizer->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
00083     poStdDialogButtonSizer->Realize();
00084     FlexGridSizer1->Add(poStdDialogButtonSizer, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
00085     SetSizer(FlexGridSizer1);
00086     SetSizer(FlexGridSizer1);
00087     Layout();
00088     Center();
00089     //*)
00090 
00091     wxString strHtml= wxString::Format( wxT( "<html>"
00092                                                "<head>"
00093                                                  "<title>%s - About</title>"
00094                                                "</head>"
00095                                                "<body bgcolor=#FAF0E6>"
00096                                                  "<div>"
00097                                                    "<h3 align=\"center\">"
00098                                                         "%s"
00099                                                         "<img src=\"http://%s/pictures/nellresearch.png\">"
00100                                                         "%s"
00101 #if __DEBUG__
00102                                                         " DEBUG"
00103 #endif // __DEBUG__
00104                                                    "</h3>"
00105                                                    "<p align=\"center\"><em>%s</em></p>"
00106                                                    "<p align=\"center\">"
00107 
00108                                                      "<a href=\"http://%s\">%s</a>"
00109                                                    "</p>"
00110                                                    "<p align=\"center\">%s. All rights reserved.</p>"
00111                                                    "<p>This program is licensed under the terms of the "
00112                                                         "<a href=\"http://www.nellresearch.de/licenses/gpl.txt\">GPL</a>. "
00113                                                         "Some parts of the source code is licensed under the terms of the "
00114                                                         "<a href=\"http://www.nellresearch.de/licenses/wxl.txt\">wxWindows licence</a>. "
00115                                                         "Please refere to the individual source code file."
00116                                                    "</p>"
00117                                                    "<p>This program was written using:</p>"
00118                                                    "<ul>"
00119                                                      "<li><a href=\"http://www.wxwidgets.org\">%s</a> "
00120                                                         "published under the <a href=\"http://www.nellresearch.de/licenses/wxl.txt\">wxWindows licence</a></li>"
00121 #if wxUSE_PDF
00122                                                      "<li><a href=\"http://poppler.freedesktop.org\">Poppler</a> "
00123                                                         "published under the <a href=\"http://www.nellresearch.de/licenses/gpl-2.0.txt\">GPLv2</a></li>"
00124 #endif // wxUSE_PDF
00125 #if __WXPDFDOC__
00126                                                      "<li><a href=\"http://wxcode.sourceforge.net\">wxPdfDocument</a> "
00127                                                         "published under the <a href=\"http://www.nellresearch.de/licenses/wxl.txt\">wxWindows licence</a></li>"
00128 #endif // __WXPDFDOC__
00129 #if __WXLUASCRIPT__
00130                                                      "<li><a href=\"http://www.lua.org/\">%s</a>"
00131                                                         " published under the <a href=\"http://www.lua.org/license.html\">MIT licence</a></li>"
00132 #endif // __WXLUASCRIPT__
00133                                                    "</ul>"
00134                                                    "<p>"
00135                                                         "<a href=\"http://www.nellresearch.de/licenses/gpl.txt\">"
00136                                                             "<img src=\"http://www.nellresearch.de/licenses/gplv3-127x51.png\">"
00137                                                         "</a>"
00138                                                         "<a href=\"http://www.wxwidgets.org\">"
00139                                                             "<img src=\"http://www.wxwidgets.org/logo9.jpg\">"
00140                                                         "</a>"
00141                                                    "</p>"
00142                                                  "</div>"
00143                                                "</body>"
00144                                              "</html>" ),
00145                                         cstrAppName.c_str(),
00146                                         cstrAppName.c_str(), cstrWebSite.c_str(), cstrVersionString.c_str(),
00147                                         cstrAppDescription.c_str(),
00148                                         cstrWebSite.c_str(), cstrWebSite.c_str(),
00149                                         cstrStatusCopyRight.c_str(),
00150                                         cstrWxVersionString.c_str()
00151 #if __WXLUASCRIPT__
00152                                         , cstrLuaVersion.c_str()
00153 #endif // __WXLUASCRIPT__
00154                                       );
00155     poHtmlWindow->SetPage( strHtml );
00156     SetTitle( poHtmlWindow->GetOpenedPageTitle() );
00157 }
00158 
00159 // Virtual destructor.
00160 //
00161 wxIScanAbout::~wxIScanAbout()
00162 {
00163     //(*Destroy(wxIScanAbout)
00164     //*)
00165 }
00166 
00167 
00168 //////////////////////////////////////////////////////////
00169 // Class wxLinkInBrowserHtmlWindow
00170 //
00171 // Event table.
00172 //
00173 BEGIN_EVENT_TABLE( wxLinkInBrowserHtmlWindow, wxHtmlWindow )
00174     EVT_HTML_LINK_CLICKED( wxID_ANY, wxLinkInBrowserHtmlWindow::OnLinkClicked )
00175 END_EVENT_TABLE()
00176 
00177 
00178 //////////////////////////////////////////////////////////
00179 // Doxygen main page definition
00180 //
00181 /*! \mainpage wxIScan
00182  *
00183  * \image html wxiscan.png
00184  *
00185  * \section readme Introduction and additional information from the readme.txt file
00186  *
00187  * \verbinclude readme.txt
00188  *
00189  *
00190  * \section roadmap Roadmap
00191  *
00192  * \verbinclude roadmap.txt
00193  *
00194  *
00195  * \section licence Licence Information
00196  *
00197  * \subsection gpl Licence of wxIScan
00198  *
00199  * The whole program is licensed under the terms of the GPL.
00200  *
00201  * \verbinclude license/gpl.txt
00202  *
00203  *
00204  * \subsection wxwidgetslicence wxWindows/wxWidgets Licence
00205  *
00206  * Some parts of the source code is licensed under the terms of the wxWindows licence
00207  * which is an extended version of the GPL. See source code for further information on
00208  * which part of it the wxWindows licence is applicable.
00209  *
00210  * \verbinclude license/wxl.txt
00211  *
00212  *
00213  * ...
00214  *
00215  */

