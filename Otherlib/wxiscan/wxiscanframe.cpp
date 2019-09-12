0001 /*************************************************************
00002  * Name:      wxiscanframe.cpp
00003  * Purpose:   Code for application frame (the main) window
00004  * Author:    Daniel Nell (daniel.nell@nellresearch.de)
00005  * Created:   2008-05-04
00006  * Copyright: Daniel Nell (www.nellresearch.de)
00007  * License:   GPL
00008  **************************************************************/
00009 
00010 // Include precompiled headers.
00011 #include "wx_pch.h"
00012 
00013 //(*InternalHeaders(wxIScanFrame)
00014 #include <wx/string.h>
00015 #include <wx/intl.h>
00016 #include <wx/bitmap.h>
00017 #include <wx/icon.h>
00018 #include <wx/image.h>
00019 #include <wx/artprov.h>
00020 //*)
00021 
00022 // wxWidgets headers
00023 #include <wx/accel.h>
00024 #include <wx/clipbrd.h>
00025 #include <wx/config.h>
00026 #include <wx/fileconf.h>
00027 #include <wx/filename.h>
00028 #if __WXPDFDOC__
00029 #   include <wx/pdfdoc.h>
00030 #endif // __WXPDFDOC__
00031 #include <wx/print.h>
00032 #include <wx/printdlg.h>
00033 #if __DEBUG__
00034 #   include <wx/stdpaths.h>
00035 #endif // __DEBUG__
00036 #include <wx/txtstrm.h>
00037 
00038 // Private headers
00039 #include "wxextprocess.h"
00040 #include "wxiscanabout.h"
00041 #include "wxiscanapp.h"
00042 #include "wxiscanframe.h"
00043 #include "wxiscanglobals.h"
00044 #if __WXLUASCRIPT__
00045 #   include "wxiscanluascript.h"
00046 #endif // __WXLUASCRIPT__
00047 #include "wxiscanresources.h"
00048 #include "wxchainlogtextctrl.h"
00049 #include "wxiviewprintout.h"
00050 #include "wxoptionsdialog.h"
00051 #include "wxtextctrlcolourizer.h"
00052 
00053 #if __WXLUASCRIPT__
00054 // Include Lua header file(s).
00055 #   include "lua5.1/lua.hpp"
00056 #endif // WXLUASCRIPT__
00057 
00058 // Resources
00059 //}
00060 
00061 //(*IdInit(wxIScanFrame)
00062 const long wxIScanFrame::ID_CANVAS = wxNewId();
00063 const long wxIScanFrame::ID_PAGES = wxNewId();
00064 const long wxIScanFrame::ID_VSPLITTERWINDOW = wxNewId();
00065 const long wxIScanFrame::ID_LOGTEXTCTRL = wxNewId();
00066 const long wxIScanFrame::ID_HSPLITTERWINDOW = wxNewId();
00067 const long wxIScanFrame::ID_FILESCANIMAGE = wxNewId();
00068 const long wxIScanFrame::ID_FILEOPEN = wxNewId();
00069 const long wxIScanFrame::ID_FILESAVEAS = wxNewId();
00070 const long wxIScanFrame::ID_FILEPRINT = wxNewId();
00071 const long wxIScanFrame::ID_FILEPRINTPREVIEW = wxNewId();
00072 const long wxIScanFrame::ID_FILEPRINTSETUP = wxNewId();
00073 const long wxIScanFrame::ID_FILEQUIT = wxNewId();
00074 const long wxIScanFrame::ID_EDITCOPY = wxNewId();
00075 const long wxIScanFrame::ID_EDITPASTE = wxNewId();
00076 const long wxIScanFrame::ID_EDITSETTINGS = wxNewId();
00077 const long wxIScanFrame::ID_EDITSETTINGSFILE = wxNewId();
00078 const long wxIScanFrame::ID_EDITSCANSCANNERS = wxNewId();
00079 const long wxIScanFrame::ID_EDITCLEARLOG = wxNewId();
00080 const long wxIScanFrame::ID_EDITCLEARHISTORY = wxNewId();
00081 const long wxIScanFrame::ID_VIEWFULLSCREEN = wxNewId();
00082 const long wxIScanFrame::ID_VIEWFITWIDTH = wxNewId();
00083 const long wxIScanFrame::ID_VIEWFITWINDOW = wxNewId();
00084 const long wxIScanFrame::ID_VIEWFULLSIZE = wxNewId();
00085 const long wxIScanFrame::ID_VIEWCUSTOMSIZE = wxNewId();
00086 const long wxIScanFrame::ID_VIEWZOOMIN = wxNewId();
00087 const long wxIScanFrame::ID_VIEWZOOMOUT = wxNewId();
00088 const long wxIScanFrame::ID_VIEWZOOMSELECTION = wxNewId();
00089 const long wxIScanFrame::ID_VIEWCLOSEDEBUGWINDOW = wxNewId();
00090 const long wxIScanFrame::ID_VIEWCLOSEPREVIEWPANEL = wxNewId();
00091 const long wxIScanFrame::ID_DOCUMENTCOPYTEXT = wxNewId();
00092 const long wxIScanFrame::ID_DOCUMENTREADTEXT = wxNewId();
00093 const long wxIScanFrame::ID_CMDSTOP = wxNewId();
00094 const long wxIScanFrame::ID_DOCUMENTFIRSTPAGE = wxNewId();
00095 const long wxIScanFrame::ID_DOCUMENTPREVIOUSPAGE = wxNewId();
00096 const long wxIScanFrame::ID_DOCUMENTNEXTPAGE = wxNewId();
00097 const long wxIScanFrame::ID_DOCUMENTLASTPAGE = wxNewId();
00098 const long wxIScanFrame::ID_DOCUMENTROTATE90LEFT = wxNewId();
00099 const long wxIScanFrame::ID_DOCUMENTROTATE90RIGHT = wxNewId();
00100 const long wxIScanFrame::ID_DOCUMENTCROP = wxNewId();
00101 const long wxIScanFrame::ID_DOCUMENTAPPLYPROFILE = wxNewId();
00102 const long wxIScanFrame::ID_MENUDOCUMENTPROFILES = wxNewId();
00103 const long wxIScanFrame::ID_PDFBEGINPDF = wxNewId();
00104 const long wxIScanFrame::ID_PDFADDPAGE = wxNewId();
00105 const long wxIScanFrame::ID_PDFSAVEASPDF = wxNewId();
00106 const long wxIScanFrame::ID_HELPABOUT = wxNewId();
00107 const long wxIScanFrame::ID_FILESCANIMAGE_TB = wxNewId();
00108 const long wxIScanFrame::ID_FILEOPEN_TB = wxNewId();
00109 const long wxIScanFrame::ID_FILESAVEAS_TB = wxNewId();
00110 const long wxIScanFrame::ID_FILEPRINT_TB = wxNewId();
00111 const long wxIScanFrame::ID_PDFBEGINPDF_TB = wxNewId();
00112 const long wxIScanFrame::ID_PDFADDPAGE_TB = wxNewId();
00113 const long wxIScanFrame::ID_PDFSAVEASPDF_TB = wxNewId();
00114 const long wxIScanFrame::ID_DOCUMENTFIRSTPAGE_TB = wxNewId();
00115 const long wxIScanFrame::ID_DOCUMENTPREVIOUSPAGE_TB = wxNewId();
00116 const long wxIScanFrame::ID_DOCUMENTNEXTPAGE_TB = wxNewId();
00117 const long wxIScanFrame::ID_DOCUMENTLASTPAGE_TB = wxNewId();
00118 const long wxIScanFrame::ID_DOCUMENTREADTEXT_TB = wxNewId();
00119 const long wxIScanFrame::ID_CMDSTOP_TB = wxNewId();
00120 //*)
00121 #if __WXLUASCRIPT__
00122 const long wxIScanFrame::ID_DOCUMENTREADTEXTSYNC= wxNewId();
00123 #endif // __WXLUASCRIPT__
00124 const long wxIScanFrame::ID_TEXTCTRLPAGE= wxNewId();
00125 #ifdef __WXSCANSANE__
00126 const long wxIScanFrame::ID_CHOICESCANNER= wxNewId();
00127 #endif // __WXSCANSANE__
00128 const long wxIScanFrame::ID_DOCUMENTPROFILE0= wxNewId();
00129 
00130 
00131 //////////////////////////////////////////////////////////
00132 // Strings and other (global) constants
00133 //
00134 static const wxString cstrwxFrame= wxT( "wxIScanFrame" );
00135 
00136 
00137 //////////////////////////////////////////////////////////
00138 // Class wxIScanFrame
00139 //
00140 // Event table
00141 //
00142 BEGIN_EVENT_TABLE( wxIScanFrame, wxFrame )
00143     //(*EventTable(wxIScanFrame)
00144     //*)
00145     // File history handling.
00146     EVT_MENU_RANGE( wxID_FILE1, wxID_FILE9, wxIScanFrame::OnMRUFiles )
00147 #if __WXLUASCRIPT__
00148     EVT_MENU_RANGE( ID_DOCUMENTPROFILE0 + 1000, ID_DOCUMENTPROFILE0 + 1999, wxIScanFrame::OnScriptStartScript )
00149     EVT_MENU( ID_DOCUMENTREADTEXTSYNC, wxIScanFrame::OnDocumentReadText )
00150 #endif // __WXLUASCRIPT__
00151     // File drop event
00152     EVT_EXTFILEDROP( wxIScanFrame::OnFileDrop )
00153     // Handle canvas size changes.
00154     EVT_IVIEWCANVASSIZE( wxIScanFrame::OnCanvasSize )
00155     // Handle canvas selection changes.
00156     EVT_IVIEWCANVASSELECTED( wxIScanFrame::OnCanvasSelected )
00157     // Handle process events.
00158     EVT_END_PROCESS( wxISCAN_PROCESS, wxIScanFrame::OnProcessTerminated )
00159     // Handle page number editor field events.
00160     EVT_TEXT_ENTER( ID_TEXTCTRLPAGE, wxIScanFrame::OnTextCtrlPageProcessEnter )
00161 #ifdef __WXSCANSANE__
00162     // Handle thread events.
00163     EVT_MENU( wxExtThread::ID_EXIT, wxIScanFrame::OnThreadExit )
00164 #endif // __WXSCANSANE__
00165 END_EVENT_TABLE()
00166 
00167 
00168 // Standard constructor.
00169 //
00170 wxIScanFrame::wxIScanFrame( wxWindow* parent, wxWindowID id )
00171   :
00172   m_bUILocked( false ),
00173   m_poScanner( NULL ),
00174   m_nScaleFactor( 1.0 ),
00175   m_nZoomFactor( 0.1 ),
00176   m_nProfileIndex( 0 ),
00177 #if __WXPDFDOC__
00178   m_poPdfDoc( NULL ),
00179   m_nPdfPages( 0 ),
00180   m_bPdfDirty( false ),
00181 #endif
00182 #if __WXLUASCRIPT__
00183   bLuaReturnFromEventHandler( true ),
00184 #endif // __WXLUASCRIPT__
00185   m_poRunningProcess( NULL ),
00186   m_oTextValidatorTextCtrlPage( wxFILTER_NUMERIC )
00187 {
00188     //(*Initialize(wxIScanFrame)
00189     wxMenuItem* poOnDocumentZoom;
00190     wxMenu* poMenuHelp;
00191     wxMenuItem* poMenuSavePdf;
00192     wxMenuItem* poViewCustomSize;
00193     wxMenuItem* poDocumentRotate90Left;
00194     wxMenuItem* poViewZoomOut;
00195     wxMenuItem* poMenuFileOpen;
00196     wxMenuItem* poMenuFileSaveAs;
00197     wxMenuItem* poHelpAbout;
00198     wxMenuItem* poMenuCmdStop;
00199     wxMenuItem* poFileQuit;
00200     wxMenuItem* poMenuPdfBeginPdf;
00201     wxMenuItem* poMenuFileScanImage;
00202     wxMenuItem* poViewFitWidth;
00203     wxMenuItem* poMenuDocumentPreviousPage;
00204     wxMenuItem* poEditSettingsFile;
00205     wxMenuItem* poMenuFilePrint;
00206     wxMenuItem* poViewFullSize;
00207     wxMenuItem* poOnDocumentCrop;
00208     wxMenuItem* poEditClearHistory;
00209     wxMenu* poMenuDocument;
00210     wxMenuItem* poMenuFilePrintPreview;
00211     wxStatusBar* poStatusBar;
00212     wxMenuItem* poViewClosePreviewPanel;
00213     wxMenuItem* poMenuDocumentLastPage;
00214     wxMenuItem* poMenuDocumentReadText;
00215     wxMenuItem* poViewFullScreen;
00216     wxMenuItem* poViewFitWindow;
00217     wxMenuItem* poMenuPdfAddPage;
00218     wxMenuItem* poViewZoomIn;
00219     wxMenuItem* poEditSettings;
00220     wxToolBar* poToolBar;
00221     wxMenu* poMenuDocumentProfiles;
00222     wxMenuItem* poViewCloseDebugWindow;
00223     wxMenuItem* poDocumentApplyProfile;
00224     wxMenuItem* poMenuDocumentCopyText;
00225     wxMenuItem* poEditCopy;
00226     wxMenu* poMenuFile;
00227     wxMenu* poMenuPdf;
00228     wxMenuBar* poMenuBar;
00229     wxMenu* poView;
00230     wxMenuItem* poMenuFilePrintSetup;
00231     wxMenuItem* poEditPaste;
00232     wxMenuItem* poEditClearLog;
00233     wxMenuItem* poDocumentRotate90Right;
00234     wxMenuItem* poMenuDocumentFirstPage;
00235     wxMenuItem* poEditScanScanners;
00236     wxMenuItem* poMenuDocumentNextPage;
00237 
00238     Create(parent, wxID_ANY, _("wxIScan"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
00239     {
00240         wxIcon FrameIcon;
00241         FrameIcon.CopyFromBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_NORMAL_FILE")),wxART_FRAME_ICON));
00242         SetIcon(FrameIcon);
00243     }
00244     m_poHSplitterWindow = new wxSplitterWindow(this, ID_HSPLITTERWINDOW, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_LIVE_UPDATE, _T("ID_HSPLITTERWINDOW"));
00245     m_poHSplitterWindow->SetMinSize(wxSize(1,1));
00246     m_poHSplitterWindow->SetMinimumPaneSize(1);
00247     m_poHSplitterWindow->SetSashGravity(0);
00248     m_poVSplitterWindow = new wxSplitterWindow(m_poHSplitterWindow, ID_VSPLITTERWINDOW, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_LIVE_UPDATE, _T("ID_VSPLITTERWINDOW"));
00249     m_poVSplitterWindow->SetMinSize(wxSize(1,1));
00250     m_poVSplitterWindow->SetMinimumPaneSize(1);
00251     m_poVSplitterWindow->SetSashGravity(0);
00252     m_poCanvas = new wxIViewCanvas(m_poVSplitterWindow, ID_CANVAS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxVSCROLL|wxHSCROLL, _T("ID_CANVAS"));
00253     m_poPanelPages = new wxImagePanel(m_poVSplitterWindow, ID_PAGES, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxVSCROLL|wxHSCROLL, _T("ID_PAGES"));
00254     m_poVSplitterWindow->SplitVertically(m_poCanvas, m_poPanelPages);
00255     m_poVSplitterWindow->SetSashPosition(1);
00256     m_poLogTextCtrl = new wxTextCtrl(m_poHSplitterWindow, ID_LOGTEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH|wxTE_AUTO_URL|wxTE_DONTWRAP, wxDefaultValidator, _T("ID_LOGTEXTCTRL"));
00257     m_poHSplitterWindow->SplitHorizontally(m_poVSplitterWindow, m_poLogTextCtrl);
00258     m_poHSplitterWindow->SetSashPosition(250);
00259     poMenuBar = new wxMenuBar();
00260     poMenuFile = new wxMenu();
00261     poMenuFileScanImage = new wxMenuItem(poMenuFile, ID_FILESCANIMAGE, _("Scan &Image\tCtrl+I"), _("Scan an image"), wxITEM_NORMAL);
00262     poMenuFile->Append(poMenuFileScanImage);
00263     poMenuFileScanImage= poMenuFile->Remove( ID_FILESCANIMAGE );
00264     poMenuFileScanImage->SetBitmap( filescanimagebmp_xpm );
00265     poMenuFile->Append( poMenuFileScanImage );
00266     poMenuFile->AppendSeparator();
00267     poMenuFileOpen = new wxMenuItem(poMenuFile, ID_FILEOPEN, _("&Open...\tCtrl+O"), _("Open an image file."), wxITEM_NORMAL);
00268     poMenuFileOpen->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FILE_OPEN")),wxART_MENU));
00269     poMenuFile->Append(poMenuFileOpen);
00270     poMenuFileSaveAs = new wxMenuItem(poMenuFile, ID_FILESAVEAS, _("Save &as...\tCtrl+S"), _("Save scanned image to file."), wxITEM_NORMAL);
00271     poMenuFileSaveAs->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FILE_SAVE_AS")),wxART_MENU));
00272     poMenuFile->Append(poMenuFileSaveAs);
00273     poMenuFileSaveAs->Enable(false);
00274     poMenuFile->AppendSeparator();
00275     poMenuFilePrint = new wxMenuItem(poMenuFile, ID_FILEPRINT, _("&Print...\tCtrl+P"), _("Print the current image."), wxITEM_NORMAL);
00276     poMenuFilePrint->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_PRINT")),wxART_MENU));
00277     poMenuFile->Append(poMenuFilePrint);
00278     poMenuFilePrint->Enable(false);
00279     poMenuFilePrintPreview = new wxMenuItem(poMenuFile, ID_FILEPRINTPREVIEW, _("Print pre&view\tCtrl+Alt+P"), _("Show a print preview of the current image."), wxITEM_NORMAL);
00280     poMenuFile->Append(poMenuFilePrintPreview);
00281     poMenuFilePrintPreview->Enable(false);
00282     poMenuFilePrintPreview= poMenuFile->Remove( ID_FILEPRINTPREVIEW );
00283     poMenuFilePrintPreview->SetBitmap( fileprintpreviewbmp_xpm );
00284     poMenuFile->Append( poMenuFilePrintPreview );
00285     poMenuFilePrintSetup = new wxMenuItem(poMenuFile, ID_FILEPRINTSETUP, _("Page setup"), _("Show page setup dialog."), wxITEM_NORMAL);
00286     poMenuFile->Append(poMenuFilePrintSetup);
00287     poMenuFilePrintSetup= poMenuFile->Remove( ID_FILEPRINTSETUP );
00288     poMenuFilePrintSetup->SetBitmap( fileprintsetupbmp_xpm );
00289     poMenuFile->Append( poMenuFilePrintSetup );
00290     poMenuFile->AppendSeparator();
00291     poFileQuit = new wxMenuItem(poMenuFile, ID_FILEQUIT, _("&Quit\tCtrl+Q"), _("Quit the application"), wxITEM_NORMAL);
00292     poFileQuit->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_QUIT")),wxART_MENU));
00293     poMenuFile->Append(poFileQuit);
00294     poMenuBar->Append(poMenuFile, _("&File"));
00295     poMenuEdit = new wxMenu();
00296     poEditCopy = new wxMenuItem(poMenuEdit, ID_EDITCOPY, _("&Copy\tCtrl+C"), _("Copy image to clipboard"), wxITEM_NORMAL);
00297     poEditCopy->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_COPY")),wxART_MENU));
00298     poMenuEdit->Append(poEditCopy);
00299     poEditCopy->Enable(false);
00300     poEditPaste = new wxMenuItem(poMenuEdit, ID_EDITPASTE, _("&Paste\tCtrl-V"), _("Inserts the content of the clipboard."), wxITEM_NORMAL);
00301     poEditPaste->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_PASTE")),wxART_TOOLBAR));
00302     poMenuEdit->Append(poEditPaste);
00303     poMenuEdit->AppendSeparator();
00304     poEditSettings = new wxMenuItem(poMenuEdit, ID_EDITSETTINGS, _("&Settings...\tCtrl-Alt-O"), _("Edit (most) settings in a dialog window."), wxITEM_NORMAL);
00305     poEditSettings->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_EXECUTABLE_FILE")),wxART_MENU));
00306     poMenuEdit->Append(poEditSettings);
00307     poEditSettingsFile = new wxMenuItem(poMenuEdit, ID_EDITSETTINGSFILE, _("Edit settings &file"), _("Edit settings file in a text editor."), wxITEM_NORMAL);
00308     poEditSettingsFile->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_HELP_SETTINGS")),wxART_MENU));
00309     poMenuEdit->Append(poEditSettingsFile);
00310     poMenuEdit->AppendSeparator();
00311     poEditScanScanners = new wxMenuItem(poMenuEdit, ID_EDITSCANSCANNERS, _("Scan for scanne&rs\tF4"), _("Scan for available scanning devices (USB, network etc.)."), wxITEM_NORMAL);
00312     poMenuEdit->Append(poEditScanScanners);
00313     poEditScanScanners= poMenuEdit->Remove( ID_EDITSCANSCANNERS );
00314     poEditScanScanners->SetBitmap( filescanimagebmp_xpm );
00315     poMenuEdit->Append( poEditScanScanners );
00316     poMenuEdit->AppendSeparator();
00317     poEditClearLog = new wxMenuItem(poMenuEdit, ID_EDITCLEARLOG, _("Clear &log\tF5"), _("Clear log window."), wxITEM_NORMAL);
00318     poEditClearLog->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_DELETE")),wxART_MENU));
00319     poMenuEdit->Append(poEditClearLog);
00320     poEditClearHistory = new wxMenuItem(poMenuEdit, ID_EDITCLEARHISTORY, _("Clear file &history"), _("Clear the file history."), wxITEM_NORMAL);
00321     poEditClearHistory->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_DEL_BOOKMARK")),wxART_MENU));
00322     poMenuEdit->Append(poEditClearHistory);
00323     poMenuBar->Append(poMenuEdit, _("&Edit"));
00324     poView = new wxMenu();
00325     poViewFullScreen = new wxMenuItem(poView, ID_VIEWFULLSCREEN, _("&Full screen\tF12"), _("View in full screen mode (no frame, no caption etc.)"), wxITEM_CHECK);
00326     poView->Append(poViewFullScreen);
00327     poView->AppendSeparator();
00328     poViewFitWidth = new wxMenuItem(poView, ID_VIEWFITWIDTH, _("Fit image to window w&idth\tI"), _("Zoom image to fit the window width."), wxITEM_CHECK);
00329     poView->Append(poViewFitWidth);
00330     poViewFitWindow = new wxMenuItem(poView, ID_VIEWFITWINDOW, _("Fit image to &window\tW"), _("Zoom image to fit the window size."), wxITEM_CHECK);
00331     poView->Append(poViewFitWindow);
00332     poViewFullSize = new wxMenuItem(poView, ID_VIEWFULLSIZE, _("View image in &original size\tO"), _("Show image in its original size (without any zooming)."), wxITEM_CHECK);
00333     poView->Append(poViewFullSize);
00334     poViewCustomSize = new wxMenuItem(poView, ID_VIEWCUSTOMSIZE, _("View image in &custom size\tC"), _("Zoom image to an explicit given (fixed) zoom factor."), wxITEM_CHECK);
00335     poView->Append(poViewCustomSize);
00336     poView->AppendSeparator();
00337     poViewZoomIn = new wxMenuItem(poView, ID_VIEWZOOMIN, _("Zoom &in\t+"), _("Zoom into the image."), wxITEM_NORMAL);
00338     poView->Append(poViewZoomIn);
00339     poViewZoomOut = new wxMenuItem(poView, ID_VIEWZOOMOUT, _("Zoom &out\t-"), _("Zoom out of the image."), wxITEM_NORMAL);
00340     poView->Append(poViewZoomOut);
00341     poOnDocumentZoom = new wxMenuItem(poView, ID_VIEWZOOMSELECTION, _("&Zoom selection\tZ"), _("Zoom the image along selection rectangle."), wxITEM_NORMAL);
00342     poView->Append(poOnDocumentZoom);
00343     poOnDocumentZoom->Enable(false);
00344     poView->AppendSeparator();
00345     poViewCloseDebugWindow = new wxMenuItem(poView, ID_VIEWCLOSEDEBUGWINDOW, _("Close the &debug window\tF2"), _("Close the debug window."), wxITEM_NORMAL);
00346     poView->Append(poViewCloseDebugWindow);
00347     poViewClosePreviewPanel = new wxMenuItem(poView, ID_VIEWCLOSEPREVIEWPANEL, _("Close the preview &panel\tF3"), _("Close the PDF preview panel."), wxITEM_NORMAL);
00348     poView->Append(poViewClosePreviewPanel);
00349     poMenuBar->Append(poView, _("&View"));
00350     poMenuDocument = new wxMenu();
00351     poMenuDocumentCopyText = new wxMenuItem(poMenuDocument, ID_DOCUMENTCOPYTEXT, _("Copy &text\tCtrl-Alt-C"), _("Copies via OCR detected text to the clipboard."), wxITEM_NORMAL);
00352     poMenuDocumentCopyText->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_COPY")),wxART_MENU));
00353     poMenuDocument->Append(poMenuDocumentCopyText);
00354     poMenuDocumentCopyText->Enable(false);
00355     poMenuDocumentReadText = new wxMenuItem(poMenuDocument, ID_DOCUMENTREADTEXT, _("&Read out document\tCtrl+Alt+R"), _("Reads the document through TTS."), wxITEM_NORMAL);
00356     poMenuDocument->Append(poMenuDocumentReadText);
00357     poMenuDocumentReadText->Enable(false);
00358     poMenuDocumentReadText= poMenuDocument->Remove( ID_DOCUMENTREADTEXT );
00359     poMenuDocumentReadText->SetBitmap( documentreadtext_xpm );
00360     poMenuDocument->Append( poMenuDocumentReadText );
00361     poMenuCmdStop = new wxMenuItem(poMenuDocument, ID_CMDSTOP, _("&Stop\tCtrl-Alt-E"), _("Stop background action."), wxITEM_NORMAL);
00362     poMenuDocument->Append(poMenuCmdStop);
00363     poMenuCmdStop->Enable(false);
00364     poMenuCmdStop= poMenuDocument->Remove( ID_CMDSTOP );
00365     poMenuCmdStop->SetBitmap( cmdstop_xpm );
00366     poMenuDocument->Append( poMenuCmdStop );
00367     poMenuDocument->AppendSeparator();
00368     poMenuDocumentFirstPage = new wxMenuItem(poMenuDocument, ID_DOCUMENTFIRSTPAGE, _("&First page\tCtrl+Home"), _("Go to first document page."), wxITEM_NORMAL);
00369     poMenuDocument->Append(poMenuDocumentFirstPage);
00370     poMenuDocumentFirstPage->Enable(false);
00371     poMenuDocumentFirstPage= poMenuDocument->Remove( ID_DOCUMENTFIRSTPAGE );
00372     poMenuDocumentFirstPage->SetBitmap( documentfirstpage_xpm );
00373     poMenuDocument->Append( poMenuDocumentFirstPage );
00374     poMenuDocumentPreviousPage = new wxMenuItem(poMenuDocument, ID_DOCUMENTPREVIOUSPAGE, _("&Previous page\tCtrl+PageUp"), _("Go to previous document page."), wxITEM_NORMAL);
00375     poMenuDocument->Append(poMenuDocumentPreviousPage);
00376     poMenuDocumentPreviousPage->Enable(false);
00377     poMenuDocumentPreviousPage= poMenuDocument->Remove( ID_DOCUMENTPREVIOUSPAGE );
00378     poMenuDocumentPreviousPage->SetBitmap( documentpreviouspage_xpm );
00379     poMenuDocument->Append( poMenuDocumentPreviousPage );
00380     poMenuDocumentNextPage = new wxMenuItem(poMenuDocument, ID_DOCUMENTNEXTPAGE, _("&Next page\tCtrl+PageDown"), _("Go to next document page."), wxITEM_NORMAL);
00381     poMenuDocument->Append(poMenuDocumentNextPage);
00382     poMenuDocumentNextPage->Enable(false);
00383     poMenuDocumentNextPage= poMenuDocument->Remove( ID_DOCUMENTNEXTPAGE );
00384     poMenuDocumentNextPage->SetBitmap( documentnextpage_xpm );
00385     poMenuDocument->Append( poMenuDocumentNextPage );
00386     poMenuDocumentLastPage = new wxMenuItem(poMenuDocument, ID_DOCUMENTLASTPAGE, _("&Last page\tCtrl+End"), _("Go to last document page."), wxITEM_NORMAL);
00387     poMenuDocument->Append(poMenuDocumentLastPage);
00388     poMenuDocumentLastPage->Enable(false);
00389     poMenuDocumentLastPage= poMenuDocument->Remove( ID_DOCUMENTLASTPAGE );
00390     poMenuDocumentLastPage->SetBitmap( documentlastpage_xpm );
00391     poMenuDocument->Append( poMenuDocumentLastPage );
00392     poMenuDocument->AppendSeparator();
00393     poDocumentRotate90Left = new wxMenuItem(poMenuDocument, ID_DOCUMENTROTATE90LEFT, _("Rotate 90 degress &left\tL"), _("Rotate 90 degress left"), wxITEM_NORMAL);
00394     poDocumentRotate90Left->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_UNDO")),wxART_MENU));
00395     poMenuDocument->Append(poDocumentRotate90Left);
00396     poDocumentRotate90Left->Enable(false);
00397     poDocumentRotate90Right = new wxMenuItem(poMenuDocument, ID_DOCUMENTROTATE90RIGHT, _("Rotate 90 degress &right\tR"), _("Rotate 90 degress right"), wxITEM_NORMAL);
00398     poDocumentRotate90Right->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_REDO")),wxART_MENU));
00399     poMenuDocument->Append(poDocumentRotate90Right);
00400     poDocumentRotate90Right->Enable(false);
00401     poOnDocumentCrop = new wxMenuItem(poMenuDocument, ID_DOCUMENTCROP, _("&Crop selection\tCtrl+Y"), _("Crop the image along selection rectangle."), wxITEM_NORMAL);
00402     poOnDocumentCrop->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_CUT")),wxART_MENU));
00403     poMenuDocument->Append(poOnDocumentCrop);
00404     poOnDocumentCrop->Enable(false);
00405     poMenuDocument->AppendSeparator();
00406     poDocumentApplyProfile = new wxMenuItem(poMenuDocument, ID_DOCUMENTAPPLYPROFILE, _("&Apply profile\tp"), _("Apply the current selected image profile to the image."), wxITEM_NORMAL);
00407     poMenuDocument->Append(poDocumentApplyProfile);
00408     poDocumentApplyProfile->Enable(false);
00409     poMenuDocumentProfiles = new wxMenu();
00410     poMenuDocumentProfiles->AppendSeparator();
00411     poMenuDocument->Append(ID_MENUDOCUMENTPROFILES, _("&Profiles"), poMenuDocumentProfiles, wxEmptyString);
00412     poMenuBar->Append(poMenuDocument, _("&Document"));
00413     poMenuPdf = new wxMenu();
00414     poMenuPdfBeginPdf = new wxMenuItem(poMenuPdf, ID_PDFBEGINPDF, _("&Begin PDF\tCtrl+Alt+N"), _("Start a new PDF document."), wxITEM_NORMAL);
00415     poMenuPdfBeginPdf->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_NEW")),wxART_MENU));
00416     poMenuPdf->Append(poMenuPdfBeginPdf);
00417     poMenuPdfBeginPdf->Enable(false);
00418     poMenuPdfAddPage = new wxMenuItem(poMenuPdf, ID_PDFADDPAGE, _("&Add Page\tCtrl++"), _("Add image as new page to the PDF document."), wxITEM_NORMAL);
00419     poMenuPdfAddPage->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_MENU));
00420     poMenuPdf->Append(poMenuPdfAddPage);
00421     poMenuPdfAddPage->Enable(false);
00422     poMenuPdf->AppendSeparator();
00423     poMenuSavePdf = new wxMenuItem(poMenuPdf, ID_PDFSAVEASPDF, _("Save &PDF...\tCtrl+Shift+S"), _("Save image as PDF file."), wxITEM_NORMAL);
00424     poMenuSavePdf->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FILE_SAVE")),wxART_MENU));
00425     poMenuPdf->Append(poMenuSavePdf);
00426     poMenuSavePdf->Enable(false);
00427     poMenuBar->Append(poMenuPdf, _("&PDF"));
00428     poMenuHelp = new wxMenu();
00429     poHelpAbout = new wxMenuItem(poMenuHelp, ID_HELPABOUT, _("&About\tCtrl-Alt-I"), _("Show info about this application"), wxITEM_NORMAL);
00430     poHelpAbout->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_INFORMATION")),wxART_MENU));
00431     poMenuHelp->Append(poHelpAbout);
00432     poMenuBar->Append(poMenuHelp, _("&Help"));
00433     SetMenuBar(poMenuBar);
00434     poStatusBar = new wxStatusBar(this, wxID_ANY, 0, _T("wxID_ANY"));
00435     int __wxStatusBarWidths_1[4] = { -3, 80, 80, -2 };
00436     int __wxStatusBarStyles_1[4] = { wxSB_FLAT, wxSB_FLAT, wxSB_FLAT, wxSB_FLAT };
00437     poStatusBar->SetFieldsCount(4,__wxStatusBarWidths_1);
00438     poStatusBar->SetStatusStyles(4,__wxStatusBarStyles_1);
00439     SetStatusBar(poStatusBar);
00440     poToolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_VERTICAL|wxNO_BORDER, _T("wxID_ANY"));
00441     ToolBarItem1 = poToolBar->AddTool(ID_FILESCANIMAGE_TB, _("Scan"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_MISSING_IMAGE")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Scan"), _("Scan an image"));
00442     poToolBar->AddSeparator();
00443     ToolBarItem2 = poToolBar->AddTool(ID_FILEOPEN_TB, _("Open"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FILE_OPEN")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Open file"), _("Open an image file."));
00444     ToolBarItem3 = poToolBar->AddTool(ID_FILESAVEAS_TB, _("Save as"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FILE_SAVE_AS")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Save as"), _("Save scanned image to file."));
00445     ToolBarItem4 = poToolBar->AddTool(ID_FILEPRINT_TB, _("Print"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_PRINT")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Print"), _("Print the current image."));
00446     poToolBar->AddSeparator();
00447     ToolBarItem5 = poToolBar->AddTool(ID_PDFBEGINPDF_TB, _("Begin PDF"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_NEW")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Begin PDF"), _("Start a new PDF document."));
00448     ToolBarItem6 = poToolBar->AddTool(ID_PDFADDPAGE_TB, _("Add Page"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Add PDF Page"), _("Add image as new page to the PDF document."));
00449     ToolBarItem7 = poToolBar->AddTool(ID_PDFSAVEASPDF_TB, _("Save PDF"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FILE_SAVE_AS")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Save PDF"), _("Save image as PDF file."));
00450     poToolBar->AddSeparator();
00451     ToolBarItem8 = poToolBar->AddTool(ID_DOCUMENTFIRSTPAGE_TB, _("First page"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_GO_BACK")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("First page"), _("Go To first document page."));
00452     ToolBarItem9 = poToolBar->AddTool(ID_DOCUMENTPREVIOUSPAGE_TB, _("Previous page"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_GO_BACK")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Previous page"), _("Go to previous document page."));
00453     ToolBarItem10 = poToolBar->AddTool(ID_DOCUMENTNEXTPAGE_TB, _("Next page"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_GO_FORWARD")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Next page"), _("Go to next document page."));
00454     ToolBarItem11 = poToolBar->AddTool(ID_DOCUMENTLASTPAGE_TB, _("Last page"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_GO_FORWARD")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Last page"), _("Go to last document page."));
00455     poToolBar->AddSeparator();
00456     ToolBarItem12 = poToolBar->AddTool(ID_DOCUMENTREADTEXT_TB, _("Read out"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_HELP_BOOK")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Read out document via TTS."), _("Read out document via text-to-speach system."));
00457     ToolBarItem13 = poToolBar->AddTool(ID_CMDSTOP_TB, _("Stop"), wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ERROR")),wxART_TOOLBAR), wxNullBitmap, wxITEM_NORMAL, _("Stop background action."), _("Stop background action."));
00458     poToolBar->Realize();
00459     SetToolBar(poToolBar);
00460 
00461     Connect(ID_FILESCANIMAGE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnFileScanImage);
00462     Connect(ID_FILEOPEN,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnFileOpen);
00463     Connect(ID_FILESAVEAS,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnFileSaveAs);
00464     Connect(ID_FILEPRINT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnFilePrint);
00465     Connect(ID_FILEPRINTPREVIEW,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnFilePrintPreview);
00466     Connect(ID_FILEPRINTSETUP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnFilePrintSetup);
00467     Connect(ID_FILEQUIT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnFileQuit);
00468     Connect(ID_EDITCOPY,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnEditCopy);
00469     Connect(ID_EDITPASTE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnEditPaste);
00470     Connect(ID_EDITSETTINGS,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnEditSettings);
00471     Connect(ID_EDITSETTINGSFILE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnEditSettingsFile);
00472     Connect(ID_EDITSCANSCANNERS,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnEditScanScanners);
00473     Connect(ID_EDITCLEARLOG,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnEditClearLog);
00474     Connect(ID_EDITCLEARHISTORY,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnEditClearHistory);
00475     Connect(ID_VIEWFULLSCREEN,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnViewFullScreen);
00476     Connect(ID_VIEWFITWIDTH,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnViewFit);
00477     Connect(ID_VIEWFITWINDOW,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnViewFit);
00478     Connect(ID_VIEWFULLSIZE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnViewFit);
00479     Connect(ID_VIEWCUSTOMSIZE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnViewFit);
00480     Connect(ID_VIEWZOOMIN,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnViewFit);
00481     Connect(ID_VIEWZOOMOUT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnViewFit);
00482     Connect(ID_VIEWZOOMSELECTION,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnDocumentZoom);
00483     Connect(ID_VIEWCLOSEDEBUGWINDOW,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnViewCloseDebugWindow);
00484     Connect(ID_VIEWCLOSEPREVIEWPANEL,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnViewClosePreviewPanel);
00485     Connect(ID_DOCUMENTCOPYTEXT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnDocumentCopyText);
00486     Connect(ID_DOCUMENTREADTEXT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnDocumentReadText);
00487     Connect(ID_CMDSTOP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnCmdStop);
00488     Connect(ID_DOCUMENTFIRSTPAGE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnDocumentFirstPage);
00489     Connect(ID_DOCUMENTPREVIOUSPAGE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnDocumentPreviousPage);
00490     Connect(ID_DOCUMENTNEXTPAGE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnDocumentNextPage);
00491     Connect(ID_DOCUMENTLASTPAGE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnDocumentLastPage);
00492     Connect(ID_DOCUMENTROTATE90LEFT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnDocumentRotate90Left);
00493     Connect(ID_DOCUMENTROTATE90RIGHT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnDocumentRotate90Right);
00494     Connect(ID_DOCUMENTCROP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnDocumentCrop);
00495     Connect(ID_DOCUMENTAPPLYPROFILE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnDocumentApplyProfile);
00496     Connect(ID_PDFBEGINPDF,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnPdfBeginPdf);
00497     Connect(ID_PDFADDPAGE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnPdfAddPage);
00498     Connect(ID_PDFSAVEASPDF,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnPdfSavePdf);
00499     Connect(ID_HELPABOUT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxIScanFrame::OnHelpAbout);
00500     Connect(ID_FILESCANIMAGE_TB,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&wxIScanFrame::OnFileScanImage);
00501     Connect(ID_FILEOPEN_TB,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&wxIScanFrame::OnFileOpen);
00502     Connect(ID_FILESAVEAS_TB,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&wxIScanFrame::OnFileSaveAs);
00503     Connect(ID_FILEPRINT_TB,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&wxIScanFrame::OnFilePrint);
00504     Connect(ID_PDFBEGINPDF_TB,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&wxIScanFrame::OnPdfBeginPdf);
00505     Connect(ID_PDFADDPAGE_TB,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&wxIScanFrame::OnPdfAddPage);
00506     Connect(ID_PDFSAVEASPDF_TB,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&wxIScanFrame::OnPdfSavePdf);
00507     Connect(ID_DOCUMENTFIRSTPAGE_TB,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&wxIScanFrame::OnDocumentFirstPage);
00508     Connect(ID_DOCUMENTPREVIOUSPAGE_TB,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&wxIScanFrame::OnDocumentPreviousPage);
00509     Connect(ID_DOCUMENTNEXTPAGE_TB,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&wxIScanFrame::OnDocumentNextPage);
00510     Connect(ID_DOCUMENTLASTPAGE_TB,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&wxIScanFrame::OnDocumentLastPage);
00511     Connect(ID_DOCUMENTREADTEXT_TB,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&wxIScanFrame::OnDocumentReadText);
00512     Connect(ID_CMDSTOP_TB,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&wxIScanFrame::OnCmdStop);
00513     Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&wxIScanFrame::OnClose);
00514     //*)
00515 
00516     // Custom tool bar icons.
00517     GetToolBar()->SetToolNormalBitmap( ID_CMDSTOP_TB,              wxBitmap( cmdstop_xpm              ) );
00518     GetToolBar()->SetToolNormalBitmap( ID_DOCUMENTREADTEXT_TB,     wxBitmap( documentreadtext_xpm     ) );
00519     GetToolBar()->SetToolNormalBitmap( ID_DOCUMENTFIRSTPAGE_TB,    wxBitmap( documentfirstpage_xpm    ) );
00520     GetToolBar()->SetToolNormalBitmap( ID_DOCUMENTPREVIOUSPAGE_TB, wxBitmap( documentpreviouspage_xpm ) );
00521     GetToolBar()->SetToolNormalBitmap( ID_DOCUMENTNEXTPAGE_TB,     wxBitmap( documentnextpage_xpm     ) );
00522     GetToolBar()->SetToolNormalBitmap( ID_DOCUMENTLASTPAGE_TB,     wxBitmap( documentlastpage_xpm     ) );
00523     GetToolBar()->SetToolNormalBitmap( ID_FILESCANIMAGE_TB,        wxBitmap( filescanimagebmp_xpm     ) );
00524 
00525     // Insert a text editor control to switch to a given page.
00526     m_poTextCtrlPage= new wxTextCtrl( GetToolBar(), ID_TEXTCTRLPAGE,
00527                                       wxEmptyString, wxDefaultPosition,
00528                                       wxSize( GetToolBar()->IsVertical() ? 22 : 44, -1 ),
00529                                       wxTE_PROCESS_ENTER,
00530                                       m_oTextValidatorTextCtrlPage );
00531     GetToolBar()->InsertControl( 12, m_poTextCtrlPage );
00532     GetToolBar()->SetToolShortHelp( ID_TEXTCTRLPAGE, _( "Current page" ) );
00533 
00534 #if __WXSCANSANE__
00535     // Insert a wxChoice object to change the scanning device in the toolbar.
00536     m_poChoiceScanner= new wxChoice( GetToolBar(), ID_CHOICESCANNER,
00537                                      wxDefaultPosition,
00538                                      wxSize( GetToolBar()->IsVertical() ? 22 : 150, -1 ) );
00539     GetToolBar()->InsertControl( 1, m_poChoiceScanner );
00540     GetToolBar()->SetToolShortHelp( ID_CHOICESCANNER, _( "Select a scanner" ) );
00541 #endif // __WXSCANSANE__
00542 
00543     // Validate the toolbar.
00544     GetToolBar()->Realize();
00545 
00546     // Custom splitter window (sash) configuration.
00547     m_poHSplitterWindow->SetSashGravity( 1.0 );
00548     m_poVSplitterWindow->SetSashGravity( 1.0 );
00549 
00550 //    // Custom key accelerators.
00551 //    wxAcceleratorEntry aoAcceleratorEntries[1];
00552 //    aoAcceleratorEntries[0].Set( wxACCEL_NORMAL, 'a', ID_CMDSTOP_TB );
00553 //    wxAcceleratorTable oAcceleratorTable( WXSIZEOF( aoAcceleratorEntries ),
00554 //                                          aoAcceleratorEntries );
00555 //    SetAcceleratorTable( oAcceleratorTable );
00556 
00557     // Custom Initializations:
00558     //
00559     m_poCanvas->SetOptions(   wxIViewCanvas::wxICANVAS_MOUSE_SCROLL
00560                             | wxIViewCanvas::wxICANVAS_MOUSEWHEEL_SCROLL
00561                             | wxIViewCanvas::wxICANVAS_MOUSE_SELECTION );
00562 
00563     // Reset the image count, the page number and the image filename.
00564     ResetFileName();
00565 
00566 #if defined( __WXPDFDOC__ ) && __WXPDFDOC__ >= 1
00567     // Initialize a new pdf document object.
00568     if( !CreateNewPdfDocument() )
00569     {
00570         wxLogFatalError( _( "Failed to create wxPdfDocument object." ) );
00571     }
00572 #endif
00573 
00574     // Complete status bar information.
00575     SetStatusText( cstrStatusCopyRight, 3 );
00576 
00577     // Give the window an icon.
00578     SetIcon( wxiscanico_xpm );
00579 
00580     // Restore (most) settings.
00581     RestoreSettings();
00582 
00583     // Restore some additional settings (which should not be initialized twice).
00584     wxConfigBase *poConfig = wxConfigBase::Get();
00585     poConfig->SetRecordDefaults();
00586     poConfig->SetPath( wxT( "/" )  );
00587     poConfig->SetPath( cstrwxFrame );
00588 
00589     // Set style attributes to log text control:
00590     //
00591     // Read the font size from config file, ...
00592     int nFontSize= poConfig->Read( wxT( "FontSize" ), 8 );
00593 
00594     // ... get default text style parameters, and ...
00595     wxTextAttr oStyle( m_poLogTextCtrl->GetDefaultStyle() );
00596 
00597     // ... set some styling options (e. g. font).
00598     wxFont oFont( nFontSize, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL );
00599 
00600     oStyle.SetFont( oFont );
00601     m_poLogTextCtrl->SetDefaultStyle( oStyle );
00602 
00603     // Redirect logging to text control.
00604     int nMaxLogLevel= poConfig->Read( wxT( "MaxLogLevel" ), 2 );
00605     new wxChainLogTextCtrl( m_poLogTextCtrl, nMaxLogLevel );
00606 
00607     // Output a program version info to the log.
00608     wxLogMessage( wxT( "%s %s" ), cstrAppName.c_str(), cstrVersionString.c_str() );
00609     wxLogMessage( wxT( "----------------------------------------------------------" ) );
00610 
00611     // Restore profile settings.
00612     int nProfileIndex= poConfig->Read( wxT( "ProfileIndex" ), (long)0 );
00613 
00614     for( int i= 0; i < 999; i++ )
00615     {
00616         wxString strMenuItemText;
00617         wxString strProfileConfigPath= wxString::Format( wxT( "/Profile/%d" ), i );
00618 
00619         if( !poConfig->Exists( strProfileConfigPath ) )
00620         {
00621             break;
00622         }
00623 
00624         poConfig->SetPath( strProfileConfigPath );
00625 
00626         wxString strProfileName= poConfig->Read( wxT( "ProfileName" ), wxString::Format( wxT( "%d" ), i ) );
00627 
00628         if( i <= 9 )
00629         {
00630             strMenuItemText= wxString::Format( wxT( "&%d " ), i ) + strProfileName + wxString::Format( wxT( "\tCtrl+%d" ), i );
00631         }
00632         else
00633         {
00634             strMenuItemText= wxString::Format( wxT( "%d " ), i ) + strProfileName;
00635         }
00636 
00637         int nMenuID= ID_DOCUMENTPROFILE0 + i;
00638 
00639         poMenuDocumentProfiles->AppendCheckItem( nMenuID, strMenuItemText );
00640         Connect( nMenuID, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&wxIScanFrame::OnDocumentProfilesChooseProfile );
00641     }
00642     SetProfile( nProfileIndex );
00643 
00644 #if __WXLUASCRIPT__
00645     GetMenuBar()->Insert( wxISCAN_SCRIPTMENUPOS, new wxMenu, _( "&Scripts" ) );
00646 
00647     // Restore scripting settings.
00648     m_nLuaCount= 0;
00649     for( int i= 0; i < 999; i++ )
00650     {
00651         if( !poConfig->Exists( wxString::Format( wxT( "/Script/%d" ), i ) ) )
00652         {
00653             break;
00654         }
00655 
00656         // Switch to the scripting sub section.
00657         poConfig->SetPath( wxString::Format( wxT( "/Script/%d" ), i ) );
00658 
00659         // Get the string for the menu entry and its help string.
00660         wxString strMenuItemText= poConfig->Read( wxT( "MenuEntry" ), wxString::Format( wxT( "Script%d" ), i ) );
00661         wxString strMenuItemHelpText= poConfig->Read( wxT( "MenuEntryHelp" ), wxEmptyString );
00662 
00663         // Get the menu from config file where the new menu entry should appear, ...
00664         wxString strMenuPosition= poConfig->Read( wxT( "MenuPosition" ), wxT( "&Scripts" ) );
00665 
00666         // ... get its position number in the menu bar, ...
00667         int nMenuPos= GetMenuBar()->FindMenu( ::wxGetTranslation( strMenuPosition ) );
00668 
00669         // ... check its existence, ...
00670         if( nMenuPos == wxNOT_FOUND )
00671         {
00672             wxLogError( _( "Cannot find menu '%s'. "
00673                              "The according script will not be reachable by menu entry '%s'." ),
00674                           strMenuPosition.c_str(),
00675                           strMenuItemText.c_str()
00676                         );
00677             break;
00678         }
00679 
00680         // ... gets the position of the menu item after it, and ..
00681         int nMenuItem= poConfig->Read( wxT( "MenuItem" ), 0L );
00682 
00683         // ... gets its menu ID.
00684         int nMenuID= ID_DOCUMENTPROFILE0 + 1000 + i;
00685 
00686         // Create the menu entry and connect its ID to the scripting event handler.
00687         GetMenuBar()->GetMenu( nMenuPos )->Insert( nMenuItem, nMenuID, strMenuItemText, strMenuItemHelpText );
00688         Connect( nMenuID, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&wxIScanFrame::OnScriptStartScript );
00689 
00690         // Adjust script number counter.
00691         m_nLuaCount++;
00692     }
00693 #endif // __WXLUASCRIPT__
00694 
00695     // Reset file history.
00696     poConfig->SetPath( wxT( "/Files" ) );
00697     m_oFileHistory.Load( *poConfig );
00698 
00699     // Delete file history entries of files that do not exist.
00700     for( int i= m_oFileHistory.GetCount() - 1; i >= 0; i-- )
00701     {
00702         wxString strFileName= m_oFileHistory.GetHistoryFile( i );
00703 
00704         if( !::wxFileExists( strFileName ) )
00705         {
00706             m_oFileHistory.RemoveFileFromHistory( i );
00707         }
00708     }
00709 
00710     // Add file history to the 'files' menu.
00711     m_oFileHistory.AddFilesToMenu( poMenuFile );
00712     m_oFileHistory.UseMenu( poMenuFile );
00713 
00714     // Set drop target.
00715     SetDropTarget( new wxExtFileDropTarget( this ) );
00716 
00717 #if __WXSCANSANE__
00718     // Scan for scanning devices and create a scanner object.
00719     //
00720     // Note: A new wxScan object is created and EnableControls() called
00721     //       implicidly in an independent thread by ScanForScanners().
00722     //       So we don't have to do it here again.
00723     ScanForScanners( true );
00724 #else
00725     // Create a scanner object.
00726     m_poScanner= new wxScan;
00727 
00728     // Switch (mostly disable) controls.
00729     EnableControls();
00730 #endif // __WXSCANSANE__
00731 }
00732 
00733 // Destructor.
00734 //
00735 wxIScanFrame::~wxIScanFrame()
00736 {
00737     // Destroy an existing scanner object.
00738     if( m_poScanner )
00739     {
00740         delete m_poScanner;
00741         m_poScanner= NULL;
00742     }
00743 
00744     // Terminate a possibly running concurrent process.
00745     if( m_poRunningProcess )
00746     {
00747         m_poRunningProcess->Kill( wxSIGTERM, wxKILL_CHILDREN );
00748         delete m_poRunningProcess;
00749         m_poRunningProcess= NULL;
00750     }
00751 
00752 #if __WXPDFDOC__
00753     // Destroy pending PDF documents.
00754     if( m_poPdfDoc )
00755     {
00756         delete m_poPdfDoc;
00757         m_poPdfDoc= NULL;
00758     }
00759 #endif // __WXPDFDOC__
00760 
00761     // Save configuration data.
00762     SaveSettings();
00763 
00764     //(*Destroy(wxIScanFrame)
00765     //*)
00766 
00767     // Save the log to a text file.
00768     if( m_poLogTextCtrl )
00769     {
00770         wxFileName oFileName( wxStandardPaths::Get().GetDocumentsDir(), ::wxGetApp().GetAppName(), wxT( "log" ) );
00771 
00772         m_poLogTextCtrl->SaveFile( oFileName.GetFullPath() );
00773     }
00774 }
00775 
00776 // Event handlers.
00777 //
00778 /// \brief Handle window closing requests.
00779 //
00780 void wxIScanFrame::OnClose( wxCloseEvent& oEvent )
00781 {
00782     // Check if the content is "dirty" and veto closing
00783     // (if it is not forbidden).
00784     if( CanClose( oEvent.CanVeto() ) )
00785     {
00786         this->Destroy();
00787     }
00788     else
00789     {
00790         if( oEvent.CanVeto() )
00791         {
00792             oEvent.Veto();
00793         }
00794         else
00795         {
00796             this->Destroy();
00797         }
00798     }
00799 }
00800 
00801 // Handle file dropping events (by opening the given file name).
00802 //
00803 void wxIScanFrame::OnFileDrop( wxExtFileDropEvent &oEvent )
00804 {
00805     // Open file.
00806     OpenImage( oEvent.GetFileName() );
00807 }
00808 
00809 /// \brief Handle menu command "File|Scan image".
00810 //
00811 void wxIScanFrame::OnFileScanImage( wxCommandEvent& oEvent )
00812 {
00813     // Show hourglass cursor.
00814     wxBusyCursor oWait;
00815 
00816     // Scan image.
00817 #if __WXLUASCRIPT__
00818     bLuaReturnFromEventHandler= ScanImage();
00819 #else
00820     ScanImage();
00821 #endif // __WXLUASCRIPT__
00822 
00823     // Enable (additional) menu options after the 1st scan.
00824     EnableControls();
00825 }
00826 
00827 /// \brief Handle menu command "File|Open...".
00828 //
00829 void wxIScanFrame::OnFileOpen( wxCommandEvent& oEvent )
00830 {
00831     // Open an "Open file" dialog an let the user choose a filename.
00832     wxFileDialog oDlg( this,
00833                        _( "Open file" ),
00834                        wxT( "" ),
00835                        wxT( "" ),
00836                        ::wxGetApp().GetImageFilter(),
00837 #if wxCHECK_VERSION( 2, 9, 0 )
00838                        wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR );
00839 #else
00840                        wxOPEN | wxFILE_MUST_EXIST | wxCHANGE_DIR );
00841 #endif // wxCHECK_VERSION
00842 
00843     if( oDlg.ShowModal() == wxID_OK )
00844     {
00845         // Open file.
00846         OpenImage( oDlg.GetPath() );
00847     }
00848 }
00849 
00850 /// \brief Handle menu command "File|Save as...".
00851 //
00852 void wxIScanFrame::OnFileSaveAs( wxCommandEvent& oEvent )
00853 {
00854     // Open a "Save file" dialog and let the user choose a filename.
00855     wxFileDialog oDlg( this,
00856                        _( "Save as" ),
00857                        wxT( "" ),
00858                        m_strImageFileName,
00859                        ::wxGetApp().GetImageFilter(),
00860 #if wxCHECK_VERSION( 2, 9, 0 )
00861                        wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR );
00862 #else
00863                        wxSAVE | wxOVERWRITE_PROMPT | wxCHANGE_DIR );
00864 #endif // wxCHECK_VERSION
00865 
00866     if( oDlg.ShowModal() == wxID_OK )
00867     {
00868         // Show hourglass cursor.
00869         wxBusyCursor oWait;
00870 
00871         // Save file.
00872         if( !GetImageSelection().SaveFile( oDlg.GetPath() ) )
00873         {
00874             // On error warn the user.
00875             wxLogError( wxT( "Image cannot be written to file '%s'." ), oDlg.GetPath().c_str() );
00876             return;
00877         }
00878 
00879         // Save the filename.
00880         m_strImageFileName= oDlg.GetPath();
00881         SetTitle( m_strImageFileName + wxT( " - " ) + cstrAppName );
00882     }
00883 }
00884 
00885 /// \brief Handle menu command "File|Print".
00886 //
00887 void wxIScanFrame::OnFilePrint( wxCommandEvent& oEvent )
00888 {
00889     // ...
00890     AdjustOrientation();
00891 
00892     // Data object representing the print dialog's values.
00893     wxPrintDialogData oPrintDialogData( m_oPrintData );
00894 
00895     // Set the printer dialog's maximal page.
00896     oPrintDialogData.SetMaxPage( m_nImageCount );
00897 
00898     // Printer object.
00899     wxPrinter oPrinter( &oPrintDialogData );
00900 
00901     // ...
00902     wxIViewPrintout oPrintout( this, m_strImageFileName, false, m_nResolution, m_nResolution );
00903 
00904     // Create cancel dialog.
00905     oPrinter.CreateAbortWindow( this, &oPrintout );
00906 
00907     // Print.
00908     if( oPrinter.Print( this, &oPrintout, true ) )
00909     {
00910         m_oPrintData= oPrinter.GetPrintDialogData().GetPrintData();
00911     }
00912     else
00913     {
00914         if( wxPrinter::GetLastError() == wxPRINTER_ERROR )
00915         {
00916             wxLogError( _( "An error occured while printing." ) );
00917 #if __WXLUASCRIPT__
00918             // Signal error to Lua.
00919             bLuaReturnFromEventHandler= false;
00920 #endif // __WXLUASCRIPT__
00921         }
00922     }
00923 
00924     // Destroy the print image (for to release MBs of memory).
00925     m_oPrintImage.Destroy();
00926 }
00927 
00928 /// \brief Handle menu command "File|Print Preview".
00929 //
00930 void wxIScanFrame::OnFilePrintPreview( wxCommandEvent& oEvent )
00931 {
00932     // ...
00933     AdjustOrientation();
00934 
00935     // Data object representing the print dialog's values.
00936     wxPrintDialogData oPrintDialogData( m_oPrintData );
00937 
00938     // Pass two printout objects: for preview, and possible printing.
00939     wxPrintPreview *poPreview = new wxPrintPreview(
00940         new wxIViewPrintout( this, m_strImageFileName, false, m_nResolution, m_nResolution ),
00941         new wxIViewPrintout( this, m_strImageFileName, false, m_nResolution, m_nResolution ),
00942         &oPrintDialogData );
00943 
00944     if ( !poPreview || !poPreview->Ok() )
00945     {
00946         if( poPreview )
00947         {
00948             delete poPreview;
00949         }
00950         wxLogError( _( "An error occured while creating print preview object.") );
00951         return;
00952     }
00953 
00954     // Set the zoom factor to 100%.
00955     poPreview->SetZoom( 100 );
00956 
00957     int x, y, w, h;
00958 
00959     // Get the frame window position and size and create a print preview
00960     // window with the same postion and size.
00961     GetSize( &w, &h );
00962     GetPosition( &x, &y );
00963     wxIViewPreviewFrame *poPreviewFrame= new wxIViewPreviewFrame(
00964                                           poPreview,
00965                                           this,
00966                                           _( "Print Preview" ),
00967                                           wxPoint( x, y ),
00968                                           wxSize( w, h ) );
00969     if( !poPreviewFrame )
00970     {
00971         delete poPreview;
00972         wxLogError( _( "An error occured while creating print preview window.") );
00973         return;
00974     }
00975     poPreviewFrame->Initialize();
00976     poPreviewFrame->Show();
00977 
00978     // Destory the print image (for to save MBs memory).
00979     m_oPrintImage.Destroy();
00980 }
00981 
00982 /// \brief Handle menu command "File|Page Setup ".
00983 //
00984 void wxIScanFrame::OnFilePrintSetup( wxCommandEvent& oEvent )
00985 {
00986     // ...
00987     AdjustOrientation();
00988 
00989     wxPageSetupDialogData oPageSetupData( m_oPrintData );
00990 
00991     oPageSetupData.EnableOrientation( false );
00992 
00993     wxPageSetupDialog oPageSetupDialog( this, &oPageSetupData );
00994 
00995     if( oPageSetupDialog.ShowModal() == wxID_OK )
00996     {
00997         m_oPrintData= oPageSetupDialog.GetPageSetupData().GetPrintData();
00998     }
00999 }
01000 
01001 /// \brief Handle menu command "File|Quit".
01002 //
01003 void wxIScanFrame::OnFileQuit( wxCommandEvent& oEvent )
01004 {
01005     Close();
01006 }
01007 
01008 // Handle menu command "File|[MOST RECENTLY USED FILE #X]".
01009 //
01010 void wxIScanFrame::OnMRUFiles( wxCommandEvent &oEvent )
01011 {
01012     // Get the filename from history menu index, ...
01013     int nId= oEvent.GetId() - wxID_FILE1;
01014     wxString strFilename = m_oFileHistory.GetHistoryFile( nId );
01015 
01016     // ... check if it is valid, and reopen the image file.
01017     if( strFilename.IsEmpty() || !::wxFileExists( strFilename ) || !OpenImage( strFilename ) )
01018     {
01019         // Otherwise remove the file from the file name history, ...
01020         m_oFileHistory.RemoveFileFromHistory( nId );
01021 
01022         // and signal an error.
01023         wxLogError( wxString::Format( _( "File '%s' does not exist (any more) or cannot be opened." ),
01024                                         strFilename.c_str() ) );
01025     }
01026 }
01027 
01028 /// \brief Begin an new (empty) PDF document.
01029 //
01030 void wxIScanFrame::OnPdfBeginPdf( wxCommandEvent& oEvent )
01031 {
01032 #if defined( __WXPDFDOC__ ) && __WXPDFDOC__ >= 1
01033     if( !CanClose() )
01034     {
01035         return;
01036     }
01037 
01038     if( !CreateNewPdfDocument() )
01039     {
01040         return;
01041     }
01042 
01043     // Remove all thumbs from the the pages panel.
01044     m_poPanelPages->Reset();
01045 
01046     // Enable (additional) menu options.
01047     EnableControls();
01048 #else
01049     wxLogError( _( "PDF support is not enabled." ) );
01050 #endif
01051 }
01052 
01053 /// \brief Add the current image as a new page to the current PDF
01054 ///        document.
01055 //
01056 void wxIScanFrame::OnPdfAddPage( wxCommandEvent& oEvent )
01057 {
01058 #if defined( __WXPDFDOC__ ) && __WXPDFDOC__ >= 1
01059     bool bAddAllPages= false;
01060 
01061     // On multipage images ask for permission to add all pages.
01062     if( ( m_nImageCount > 1 ) && ( m_nPage == 0 ) )
01063     {
01064 
01065         switch( ::wxMessageBox( _( "This is a multipage image. Do you want to "
01066                                    "add all image pages to the PDF file?" ),
01067                                 ::wxGetApp().GetAppName(),
01068                                 wxYES_DEFAULT | wxYES_NO | wxCANCEL | wxICON_QUESTION  ) )
01069         {
01070             case wxYES    : bAddAllPages= true;
01071                             break;
01072             case wxCANCEL : return;
01073         }
01074     }
01075 
01076     // Show busy cursor (until the automatic variable is destroyed).
01077     wxBusyCursor oWait;
01078 
01079     // Apply a profile automatically after loading a page from a
01080     // multi page file.
01081     if( bAddAllPages and m_bAutoApplyProfileOnMultiPage )
01082     {
01083         ApplyProfile();
01084     }
01085 
01086     // Add page to PDF document.
01087     AddPdfPage();
01088 
01089     // Add the following pages (if wished).
01090     if( bAddAllPages )
01091     {
01092         while( m_nPage < ( m_nImageCount - 1 ) )
01093         {
01094             // Load next image page.
01095             OnDocumentNextPage( oEvent );
01096 
01097             // Optionally apply a profile automatically after loading
01098             // a page from a multi page file.
01099             if( m_bAutoApplyProfileOnMultiPage )
01100             {
01101                 ApplyProfile();
01102             }
01103 
01104             // Add page to PDF document.
01105             AddPdfPage();
01106         }
01107     }
01108 
01109     // Enable (additional) menu options.
01110     EnableControls();
01111 #else
01112     wxLogError( _( "PDF support is not enabled." ) );
01113 #endif
01114 }
01115 
01116 /// \brief Save the current PDF document as a PDF file.
01117 //
01118 void wxIScanFrame::OnPdfSavePdf( wxCommandEvent& oEvent )
01119 {
01120 #if defined( __WXPDFDOC__ ) && __WXPDFDOC__ >= 1
01121     if( !m_poPdfDoc )
01122     {
01123         wxLogError( _( "wxPdfDocument is NULL" ) );
01124         return;
01125     }
01126 
01127     // Open a "Save file" dialog and let the user choose a filename.
01128     wxFileDialog oDlg( this,
01129                        _( "Save as" ),
01130                        wxT( "" ),
01131                        _( "unknown.pdf" ),
01132                        _( "PDF file" ) + wxString( wxT( " (*.pdf)|*.pdf|" ) )
01133                        + _( "All files" ) + wxString( wxT( "|*.*" ) ),
01134 #if wxCHECK_VERSION( 2, 9, 0 )
01135                        wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR );
01136 #else
01137                        wxSAVE | wxOVERWRITE_PROMPT | wxCHANGE_DIR );
01138 #endif // wxCHECK_VERSION( 2, 9, 0 )
01139 
01140     if( oDlg.ShowModal() == wxID_OK )
01141     {
01142         // Show busy cursor (until the automatic variable is destroyed).
01143         wxBusyCursor oWait;
01144 
01145         // Save file.
01146         //
01147         // NOTE: At the moment there is no error checking on writing the
01148         //       PDF file. Does anyone know a (real) workaround?
01149         m_poPdfDoc->SaveAsFile( oDlg.GetPath() );
01150 
01151         // Mark PDF document as 'clean'.
01152         m_bPdfDirty= false;
01153 
01154         // Create a new PDF document.
01155         OnPdfBeginPdf( oEvent );
01156     }
01157 #else
01158     wxLogError( _( "PDF support is not enabled." ) );
01159 #endif
01160 }
01161 
01162 /// \brief Copy the (selected part of the) image to the system
01163 ///        clipboard.
01164 //
01165 void wxIScanFrame::OnEditCopy( wxCommandEvent& oEvent )
01166 {
01167     // Show hourglass cursor.
01168     wxBusyCursor oWait;
01169 
01170     // Copy the (selected part of the) image to the system clipboard.
01171     ::CopyImage2Clipboard( GetImageSelection() );
01172 }
01173 
01174 /// \brief Replace the image by the content of the system clipboard.
01175 //
01176 void wxIScanFrame::OnEditPaste( wxCommandEvent& oEvent )
01177 {
01178     // Show hourglass cursor.
01179     wxBusyCursor oWait;
01180 
01181     wxBitmapDataObject oBitmapData;
01182 
01183     // Open the clipboard.
01184     //
01185     // Note: There is a global object ::wxTheClipboard.
01186     //
01187     if( !wxTheClipboard->Open() )
01188     {
01189         wxLogError( wxString( wxT( "wxIScanFrame::CopyImage2Clipboard -- " ) )
01190               + _( "Failed to open clipboard." ) );
01191     }
01192 
01193     // If there is bitmap data in the clipboard get it.
01194     if( wxTheClipboard->GetData( oBitmapData ) )
01195     {
01196         // Get the image from the bitmap data in the clipboard.
01197         wxBitmap oBitmap= oBitmapData.GetBitmap();
01198 
01199         m_oImage= oBitmap.ConvertToImage();
01200         UpdateBitmap();
01201 
01202         // Enable/disable menu options according to the actual state.
01203         EnableControls();
01204     }
01205 
01206     // Close the clipboard.
01207     wxTheClipboard->Close();
01208 
01209     // Reset the filename.
01210     ResetFileName();
01211 }
01212 
01213 /// \brief Edit program settings (using an wxOptionsDialog window).
01214 //
01215 void wxIScanFrame::OnEditSettings( wxCommandEvent& oEvent )
01216 {
01217     // Get the global configuration object.
01218     wxConfigBase *poConfig = wxConfigBase::Get();
01219     poConfig->SetPath( wxT( "/Global" ) );
01220 
01221     // Get internationalization state flag directly from configuration.
01222     bool bI18n= (bool)poConfig->Read( wxT( "I18n" ), (long)true );
01223 
01224     // Get splashscreen state flag directly from configuration.
01225     bool bSplashscreen= (bool)poConfig->Read( wxT( "Splashscreen" ), (long)true );
01226 
01227     // Create, setup and display the options dialog (as a modal dialog).
01228 #if __NOTYETRELEASED__ >= 2
01229     wxOptionsBookDialog oDialog( this, wxT( "Common" ) );
01230 #else // __NOTYETRELEASED__
01231     wxOptionsDialog oDialog( this );
01232 #endif // __NOTYETRELEASED__
01233 
01234     // Create, setup and display the options dialog (as a modal dialog).
01235     oDialog.AppendOption( new wxOptionCheckBox( _( "Use internationalization on next program start." ), &bI18n ) );
01236     oDialog.AppendOption( new wxOptionCheckBox( _( "Show splashscreen at program startup." ), &bSplashscreen ) );
01237 #if __WXLUASCRIPT__
01238     oDialog.AppendOption( new wxOptionCheckBox( _( "Enable Lua scripting." ), &m_bLua ) );
01239 #endif // __WXLUASCRIPT__
01240     oDialog.AppendOption( new wxOptionCheckBox( _( "Automatically apply profile after scanning." ), &m_bAutoApplyProfile ) );
01241     oDialog.AppendOption( new wxOptionCheckBox( _( "Automatically apply profile after loading a page from a multi page file." ), &m_bAutoApplyProfileOnMultiPage ) );
01242     oDialog.AppendOption( new wxOptionComboBox( _( "Mime type of image exported:" ), &m_strBitmapMimeType, 150 ) );
01243     oDialog.AppendOption( new wxOptionStaticLine );
01244 #if __WXPDFDOC__
01245     oDialog.AppendOption( new wxOptionCheckBox( _( "Use OCR when adding text to a PDF page." ), &m_bOCR ) );
01246     oDialog.AppendOption( new wxOptionComboBox( _( "Mime type of image exported to a PDF page:" ), &m_strPdfBitmapMimeType, 150 ) );
01247     oDialog.AppendOption( new wxOptionStaticLine );
01248 #endif // __WXPDFDOC__
01249     oDialog.AppendOption( new wxOptionCheckBox( _( "Enable OCR command." ), &m_bCmdOcrEnable ) );
01250     oDialog.AppendOption( new wxOptionComboBox( _( "OCR command:" ), &m_strCmdOcr ) );
01251     oDialog.AppendOption( new wxOptionComboBox( _( "Output extension of the extracted text:" ), &m_strOcrOutputExtension, 150 ) );
01252     oDialog.AppendOption( new wxOptionCheckBox( _( "Enable hOCR processing." ), &m_bHocrEnable ) );
01253     oDialog.AppendOption( new wxOptionComboBox( _( "OCR command to create hOCR output:" ), &m_strCmdOcrUseHocr ) );
01254     oDialog.AppendOption( new wxOptionComboBox( _( "Output extension of the hOCR output file:" ), &m_strOcrOutputExtensionHocr, 150 ) );
01255     oDialog.AppendOption( new wxOptionComboBox( _( "hOCR class in HOCR file to extract:" ), &m_strHocrClass, 150 ) );
01256     oDialog.AppendOption( new wxOptionCheckBox( _( "Use smart hOCR processing." ), &m_bSmartHocrEnable ) );
01257     oDialog.AppendOption( new wxOptionCheckBox( _( "Enable HTML to XHTML conversion." ), &m_bCmdHtml2XhtmlEnable ) );
01258     oDialog.AppendOption( new wxOptionComboBox( _( "HTML to XHTML conversion command:" ), &m_strCmdHtml2Xhtml ) );
01259     oDialog.AppendOption( new wxOptionCheckBox( _( "Enable image convertion command." ), &m_bCmdConvertEnable ) );
01260     oDialog.AppendOption( new wxOptionComboBox( _( "Bitmap conversion command:" ), &m_strCmdConvert ) );
01261     oDialog.AppendOption( new wxOptionCheckBox( _( "Enable clean up command." ), &m_bCmdCleanUpEnable ) );
01262     oDialog.AppendOption( new wxOptionComboBox( _( "Clean up command:" ), &m_strCmdCleanUp ) );
01263     oDialog.AppendOption( new wxOptionCheckBox( _( "Enable additional clean up after hOCR processing." ), &m_bCmdHocrCleanUpEnable ) );
01264     oDialog.AppendOption( new wxOptionComboBox( _( "Additional clean up command after hOCR processing:" ), &m_strCmdHocrCleanUp ) );
01265     oDialog.AppendOption( new wxOptionStaticLine );
01266     oDialog.AppendOption( new wxOptionCheckBox( _( "Enable TTS command." ), &m_bCmdTtsEnable ) );
01267     oDialog.AppendOption( new wxOptionComboBox( _( "TTS command:" ), &m_strCmdTts, 500 ) );
01268     oDialog.AppendOption( new wxOptionStaticLine );
01269     oDialog.AppendOption( new wxOptionCheckBox( _( "Enable editor command." ), &m_bCmdEditEnable ) );
01270     oDialog.AppendOption( new wxOptionComboBox( _( "Editor command:" ), &m_strCmdEdit ) );
01271     // ... (Add another options.)
01272 
01273     oDialog.SetupDialog();
01274 
01275     if( oDialog.ShowModal() == wxID_OK )
01276     {
01277         // Get the global configuration object and set the config path.
01278         //
01279         // NOTE: We do this because wxConfigBase::Get() returns a global object
01280         //       that could have been changed by the option dialog itself.
01281         //
01282         poConfig= wxConfigBase::Get();
01283         poConfig->SetPath( wxT( "/Global" ) );
01284 
01285         // Set i18n flag state directly in configuration (file or registry).
01286         poConfig->Write( wxT( "I18n" ), (long)bI18n );
01287 
01288         // Set splashscreen flag state directly in configuration (file or registry).
01289         poConfig->Write( wxT( "Splashscreen" ), (long)bSplashscreen );
01290 
01291         // ... (Handle another options.)
01292 
01293         // Adjust control states.
01294         EnableControls();
01295     }
01296 }
01297 
01298 /// \brief Start the configured editor to edit the configuration file.
01299 //
01300 void wxIScanFrame::OnEditSettingsFile( wxCommandEvent& oEvent )
01301 {
01302     // Save programme settings.
01303     SaveSettings();
01304 
01305     // Open settings editor.
01306     wxFileConfig *poConfig = (wxFileConfig *)wxConfigBase::Get();
01307     wxString strConfigFilename= wxFileConfig::GetLocalFileName( poConfig->GetAppName() );
01308     wxString strCommand= wxString::Format( m_strCmdEdit, strConfigFilename.c_str() );
01309 
01310 #if __DEBUG__
01311     wxLogMessage( wxT( "wxIScanFrame::OnEditSettings - strCommand == '%s'" ), strCommand.c_str() );
01312 #endif
01313     Execute( strCommand );
01314 
01315     // Restore settings (from file).
01316     RestoreSettings();
01317 }
01318 
01319 /// \brief Scan for scanning devices and fill the corresponding
01320 ///        wxChoice control in the toolbar.
01321 //
01322 void wxIScanFrame::OnEditScanScanners(wxCommandEvent& event)
01323 {
01324 #ifdef __WXSCANSANE__
01325     // Scan for scanning devices.
01326     ScanForScanners( true );
01327 #endif // __WXSCANSANE__
01328 }
01329 
01330 /// \brief Wipe out the log window.
01331 //
01332 void wxIScanFrame::OnEditClearLog( wxCommandEvent& oEvent )
01333 {
01334     m_poLogTextCtrl->Clear();
01335 }
01336 
01337 /// \brief Wipe out the file name history (under file menu).
01338 //
01339 void wxIScanFrame::OnEditClearHistory( wxCommandEvent& oEvent )
01340 {
01341     // Delete file history entries.
01342     for( int i= m_oFileHistory.GetCount() - 1; i >= 0; i-- )
01343     {
01344         m_oFileHistory.RemoveFileFromHistory( i );
01345     }
01346 }
01347 
01348 /// \brief Do OCR on the loaded/scanned image and copy the text
01349 ///        to the system clipboard.
01350 //
01351 void wxIScanFrame::OnDocumentCopyText( wxCommandEvent& oEvent )
01352 {
01353     // Save image in a temporary JPEG file.
01354     wxString strTempFileName= wxFileName::CreateTempFileName( wxT( "" ) );
01355 
01356     GetImageSelection().SaveFile( strTempFileName, m_strBitmapMimeType );
01357 
01358     // Do OCR (and get the file name of the generated text file).
01359     wxString strOcrTextFileName= OCR( strTempFileName );
01360 
01361     // Remove the temporary JPEG file.
01362     ::wxRemoveFile( strTempFileName );
01363 
01364     // Get the text and copy it to the clipboard.
01365     if( !strOcrTextFileName.IsEmpty() )
01366     {
01367         // Copy text (from text file) to clipboard.
01368         ::CopyText2Clipboard( ::GetTextFromFile( strOcrTextFileName ) );
01369 
01370         // Remove the temporary text file
01371         ::wxRemoveFile( strOcrTextFileName );
01372     }
01373 #if __WXLUASCRIPT__
01374     else
01375     {
01376         // Signal error to Lua.
01377         bLuaReturnFromEventHandler= false;
01378     }
01379 #endif // __WXLUASCRIPT__
01380 }
01381 
01382 /// \brief Do OCR on the loaded/scanned image and read the text
01383 ///        out loud.
01384 //
01385 void wxIScanFrame::OnDocumentReadText( wxCommandEvent& oEvent )
01386 {
01387 #if __WXLUASCRIPT__
01388     bLuaReturnFromEventHandler= ReadOut( oEvent.GetId() != ID_DOCUMENTREADTEXTSYNC );
01389 #else
01390     ReadOut();
01391 #endif // __WXLUASCRIPT__
01392 
01393     // Enable/disable menu options according to the actual state.
01394     EnableControls();
01395 }
01396 
01397 /// \brief Go to first page of a multipage document.
01398 //
01399 void wxIScanFrame::OnDocumentFirstPage( wxCommandEvent& oEvent )
01400 {
01401     m_nPage= 0;
01402     OnDocumentPageChanged();
01403 }
01404 
01405 /// \brief Go to previous page of a multipage document.
01406 //
01407 void wxIScanFrame::OnDocumentPreviousPage( wxCommandEvent& oEvent )
01408 {
01409     if( m_nPage > 0 )
01410     {
01411         m_nPage--;
01412         OnDocumentPageChanged();
01413     }
01414 #if __WXLUASCRIPT__
01415     else
01416     {
01417             // Signal error to Lua.
01418             bLuaReturnFromEventHandler= false;
01419     }
01420 #endif // __WXLUASCRIPT__
01421 }
01422 
01423 /// \brief Go to next page of a multipage document.
01424 //
01425 void wxIScanFrame::OnDocumentNextPage( wxCommandEvent& oEvent )
01426 {
01427     if( m_nPage < ( m_nImageCount - 1 ) )
01428     {
01429         m_nPage++;
01430         OnDocumentPageChanged();
01431     }
01432 #if __WXLUASCRIPT__
01433     else
01434     {
01435             // Signal error to Lua.
01436             bLuaReturnFromEventHandler= false;
01437     }
01438 #endif // __WXLUASCRIPT__
01439 }
01440 
01441 /// \brief Go to last page of a multipage document.
01442 //
01443 void wxIScanFrame::OnDocumentLastPage( wxCommandEvent& oEvent )
01444 {
01445     m_nPage= m_nImageCount - 1;
01446     OnDocumentPageChanged();
01447 }
01448 
01449 /// \brief Crop the image to the selected rectangle.
01450 //
01451 void wxIScanFrame::OnDocumentCrop( wxCommandEvent& oEvent )
01452 {
01453     // Show hourglass cursor.
01454     wxBusyCursor oWait;
01455 
01456     // Get the selected subimage.
01457     m_oImage= GetImageSelection();
01458 
01459     // Replace the image by the selected subimage.
01460     UpdateBitmap();
01461 
01462     // Set the file name to unknown (because the main image changed).
01463     ResetFileName();
01464 
01465     // Enable/disable menu options according to the actual state.
01466     EnableControls();
01467 }
01468 
01469 /// \brief Zoom the image to the selected rectangle to fit the
01470 ///        canvas size.
01471 //
01472 void wxIScanFrame::OnDocumentZoom( wxCommandEvent& oEvent )
01473 {
01474     // If there is nothing selected there is nothing to do at all.
01475     if( !m_poCanvas->IsSelected() )
01476     {
01477         // So just return.
01478         return;
01479     }
01480 
01481     // Show hourglass cursor.
01482     wxBusyCursor oWait;
01483 
01484     // Calculate the new zoom factor.
01485     wxRect oSelection= m_poCanvas->GetSelection();
01486     wxSize oWindowSize= m_poCanvas->GetClientSize();
01487     double nScaleFactorX= (double)oWindowSize.GetWidth()  / (double)oSelection.GetWidth();
01488     double nScaleFactorY= (double)oWindowSize.GetHeight() / (double)oSelection.GetHeight();
01489     double nScaleFactor= ( nScaleFactorX < nScaleFactorY ) ? nScaleFactorX : nScaleFactorY;
01490 
01491     // Scale old zoom factor by the new one.
01492     m_nScaleFactor *= nScaleFactor;
01493 
01494     // Set zooming to fixed size.
01495     m_nFit= wxCANVASCUSTOMSIZE;
01496 
01497     // Rezoom the image.
01498     UpdateBitmap();
01499 
01500     // Scroll left upper corner of selection to the beginning of
01501     // the visuable part of the canvas.
01502     int nXUnit, nYUnit;
01503     m_poCanvas->GetScrollPixelsPerUnit( &nXUnit, &nYUnit );
01504     m_poCanvas->Scroll( (double)oSelection.GetX() * nScaleFactor / (double)nXUnit,
01505                         (double)oSelection.GetY() * nScaleFactor / (double)nYUnit );
01506 
01507     // Enable/disable menu options according to the actual state.
01508     EnableControls();
01509 }
01510 
01511 /// \brief Rotate image by 90° to the left.
01512 //
01513 void wxIScanFrame::OnDocumentRotate90Left( wxCommandEvent& oEvent )
01514 {
01515     // Show hourglass cursor.
01516     wxBusyCursor oWait;
01517 
01518     // Rotate image and change image orientation.
01519     m_oImage= m_oImage.Rotate90( false );
01520     UpdateBitmap();
01521 }
01522 
01523 /// \brief Rotate image by 90° to the right.
01524 //
01525 void wxIScanFrame::OnDocumentRotate90Right( wxCommandEvent& oEvent )
01526 {
01527     // Show hourglass cursor.
01528     wxBusyCursor oWait;
01529 
01530     // Rotate image and change image orientation.
01531     m_oImage= m_oImage.Rotate90( true );
01532     UpdateBitmap();
01533 }
01534 
01535 /// \brief Apply a previously selected profile to the image.
01536 //
01537 void wxIScanFrame::OnDocumentApplyProfile( wxCommandEvent& oEvent )
01538 {
01539     // Apply an image profile (crop, convert to monochrome etc.), and ...
01540     ApplyProfile();
01541 
01542     // ... reset the image on the canvas window.
01543     UpdateBitmap();
01544 }
01545 
01546 /// \brief Select a profile from the corresponding sub menu.
01547 //
01548 void wxIScanFrame::OnDocumentProfilesChooseProfile( wxCommandEvent& oEvent )
01549 {
01550     SetProfile( oEvent.GetId() - ID_DOCUMENTPROFILE0 );
01551 }
01552 
01553 /// \brief Select an image fitting option.
01554 //
01555 void wxIScanFrame::OnViewFit( wxCommandEvent& oEvent )
01556 {
01557     // Save the old fitting state and the old scalation
01558     // factor (for optimizing see below).
01559     int nFitOld= m_nFit;
01560     double nScaleFactorOld= m_nScaleFactor;
01561 
01562     // Transform the given event into an option value.
01563     if( oEvent.GetId() == ID_VIEWFITWIDTH )
01564     {
01565         m_nFit= wxCANVASFITWIDTH;
01566     }
01567     else if( oEvent.GetId() == ID_VIEWFITWINDOW )
01568     {
01569         m_nFit= wxCANVASFITWINDOW;
01570     }
01571     else if( oEvent.GetId() == ID_VIEWFULLSIZE )
01572     {
01573         m_nFit= wxCANVASFULLSIZE;
01574     }
01575     else if( oEvent.GetId() == ID_VIEWCUSTOMSIZE )
01576     {
01577         m_nFit= wxCANVASCUSTOMSIZE;
01578     }
01579     else if( oEvent.GetId() == ID_VIEWZOOMIN )
01580     {
01581         m_nFit= wxCANVASCUSTOMSIZE;
01582         m_nScaleFactor *= ( 1.0 + m_nZoomFactor );
01583     }
01584     else if( oEvent.GetId() == ID_VIEWZOOMOUT )
01585     {
01586         m_nFit= wxCANVASCUSTOMSIZE;
01587         m_nScaleFactor /= ( 1.0 + m_nZoomFactor );
01588     }
01589 
01590     // Optimization: If the fitting state and the scalation
01591     // factor hasn't changed just do nothing.
01592     if( ( nFitOld != m_nFit ) || ( nScaleFactorOld != m_nScaleFactor ) )
01593     {
01594         // Otherwise (re)zoom the image.
01595         UpdateBitmap();
01596     }
01597 
01598     // Enable/disable menu options according to the actual state.
01599     EnableControls();
01600 }
01601 
01602 /// \brief Toggle full screen mode.
01603 //
01604 void wxIScanFrame::OnViewFullScreen( wxCommandEvent& oEvent )
01605 {
01606     // Toggle full screen mode.
01607     ToggleFullScreen();
01608 }
01609 
01610 /// \brief Close the debug window.
01611 //
01612 void wxIScanFrame::OnViewCloseDebugWindow( wxCommandEvent& oEvent )
01613 {
01614     // Close the debug window by moving the the sash downwards.
01615     m_poHSplitterWindow->SetSashPosition( -1 );
01616 }
01617 
01618 /// \brief Close the preview panel window.
01619 //
01620 void wxIScanFrame::OnViewClosePreviewPanel( wxCommandEvent& oEvent )
01621 {
01622     // Close the preview panel window by moving the the sash right.
01623     m_poVSplitterWindow->SetSashPosition( -1 );
01624 }
01625 
01626 #if __WXLUASCRIPT__
01627 // Handle menu command "Scrips|[SCRIPT NAME]".
01628 //
01629 void wxIScanFrame::OnScriptStartScript( wxCommandEvent& oEvent )
01630 {
01631     // Show hourglass cursor.
01632     wxBusyCursor oWait;
01633 
01634     // Let the system do the showing the hourglass cursor.
01635     ::wxSafeYield();
01636 
01637     // Get the script index, ...
01638     int nIndex= oEvent.GetId() - ( ID_DOCUMENTPROFILE0 + 1000 );
01639 
01640     // ... and get the filename and the start up ("main") function.
01641     wxConfigBase *poConfig= wxConfigBase::Get();
01642 
01643     poConfig->SetPath( wxString::Format( wxT( "/Script/%d" ), nIndex ) );
01644 
01645     wxString strScriptStartupFunction= poConfig->Read( wxT( "Function" ), wxT( "" ) );
01646     wxString strScriptFileName= poConfig->Read( wxT( "Script" ), wxT( "" ) );
01647 
01648     if( strScriptStartupFunction.IsEmpty() )
01649     {
01650         // Signal error.
01651         wxLogError( wxString( wxT( "wxIScanFrame::OnScriptStartScript -- " ) )
01652                       + _( "Script startup function is empty." ) );
01653         return;
01654     }
01655     else
01656     {
01657         // Run script
01658         StartScript( strScriptStartupFunction, strScriptFileName );
01659     }
01660 
01661     // Enable/disable menu options according to the actual state.
01662     EnableControls();
01663 }
01664 #endif // __WXLUASCRIPT__
01665 
01666 /// \brief Handle (sub-)process killing.
01667 //
01668 void wxIScanFrame::OnCmdStop( wxCommandEvent& oEvent )
01669 {
01670     // Check if the (sub-)process is running.
01671     if( m_poRunningProcess )
01672     {
01673         // Log process's output.
01674         LogProcessOutput( m_poRunningProcess );
01675 
01676         // Try to terminate the (sub-)process.
01677         //
01678         // NOTE: In case the process is not terminated properly
01679         //       it is at least detached and released from the
01680         //       the main process.
01681         m_poRunningProcess->Kill( wxSIGTERM, wxKILL_CHILDREN );
01682         m_poRunningProcess->Detach();
01683 
01684         // Mark (sub-)process pointer as "clean".
01685         m_poRunningProcess= NULL;
01686    }
01687 
01688    // Enable/disable menu options according to the actual state.
01689    EnableControls();
01690 }
01691 
01692 /// \brief Handle menu command "Help|About...".
01693 //
01694 void wxIScanFrame::OnHelpAbout( wxCommandEvent& oEvent )
01695 {
01696     // Create an show the program's about box.
01697     wxIScanAbout oAboutBox( this );
01698 
01699     oAboutBox.ShowModal();
01700 }
01701 
01702 // Handle bitmap resizing introduced by canvas resizing.
01703 //
01704 void wxIScanFrame::OnCanvasSize( wxIViewCanvasSizeEvent & WXUNUSED( oEvent ) )
01705 {
01706     if( ( m_nFit == wxCANVASFITWIDTH ) || ( m_nFit == wxCANVASFITWINDOW ) )
01707     {
01708         UpdateBitmap();
01709     }
01710 }
01711 
01712 // Handle canvas selection events.
01713 //
01714 void wxIScanFrame::OnCanvasSelected( wxIViewCanvasSelectedEvent& oEvent )
01715 {
01716     // Check if the CTRL key was pressed when the selection was finished.
01717     if( oEvent.m_bCtrl )
01718     {
01719         wxCommandEvent oDummyEvent;
01720 
01721         // Zoom the selected rect to fit in the canvas window.
01722         OnDocumentZoom( oDummyEvent );
01723     }
01724 
01725     // Check if the ALT key was pressed when the selection was finished.
01726     else if( oEvent.m_bAlt )
01727     {
01728         wxCommandEvent oDummyEvent;
01729 
01730         // Crop the selected rect.
01731         OnDocumentCrop( oDummyEvent );
01732     }
01733 
01734 #if __NOTYETRELEASED__
01735     // Check if the SHIFT key was pressed when the selection was finished.
01736     else if( oEvent.m_bShift )
01737     {
01738         // ...
01739 double nHeight= (double)( oEvent.m_oSelection.GetTop() - oEvent.m_oSelection.GetBottom() );//oEvent.m_oSelection.GetHeight();
01740 double nWidth= (double)oEvent.m_oSelection.GetWidth();
01741 double nAngle= asin( nHeight / sqrt( nHeight * nHeight + nWidth * nWidth ) );
01742 m_oImage= ::RotateImage( m_oImage, nAngle );
01743 
01744         // ...
01745         UpdateBitmap();
01746     }
01747 #endif // __NOTYETRELEASED__
01748 }
01749 
01750 // Handle process termination.
01751 //
01752 void wxIScanFrame::OnProcessTerminated( wxProcessEvent & WXUNUSED( oEvent ) )
01753 {
01754     if( m_poRunningProcess )
01755     {
01756         // Log process's output.
01757         LogProcessOutput( m_poRunningProcess );
01758 
01759         // Delete the process and set its pointer to NULL.
01760         delete m_poRunningProcess;
01761         m_poRunningProcess= NULL;
01762     }
01763 
01764     // Enable/disable menu options according to the actual state.
01765     EnableControls();
01766 }
01767 
01768 // Handle page number editor field events.
01769 //
01770 void wxIScanFrame::OnTextCtrlPageProcessEnter( wxCommandEvent & WXUNUSED( oEvent ) )
01771 {
01772     long nNewPage;
01773 
01774     if( m_poTextCtrlPage->GetValue().ToLong( &nNewPage )
01775         && ( 0 < nNewPage )
01776         && ( nNewPage <=  m_nImageCount ) )
01777     {
01778         m_nPage= nNewPage - 1;
01779         OnDocumentPageChanged();
01780     }
01781 }
01782 
01783 // Handle window closing requests.
01784 //
01785 bool wxIScanFrame::CanClose( bool bCanVeto )
01786 {
01787 #if __WXPDFDOC__
01788     if( IsPdfDirty() )
01789     {
01790         switch( ::wxMessageBox( _( "The changes to the current PDF document have not yet been saved. "
01791                                    "Do you want to save the PDF document now?" ),
01792                                 ::wxGetApp().GetAppName(),
01793                                 wxYES_NO | ( bCanVeto ? wxCANCEL : 0 ) | wxICON_QUESTION ) )
01794         {
01795             case wxYES: {
01796                             wxCommandEvent oDummyEvent( wxEVT_COMMAND_MENU_SELECTED, ID_PDFSAVEASPDF );
01797 
01798                             OnPdfSavePdf  ( oDummyEvent );
01799                         }
01800                         return !IsPdfDirty();
01801             case wxNO : return true;
01802             default:    return false;
01803         }
01804     }
01805     else
01806     {
01807         return true;
01808     }
01809 #else
01810     return true;
01811 #endif
01812 }
01813 
01814 // Helper functions.
01815 //
01816 // Open an image using the given name.
01817 //
01818 bool wxIScanFrame::OpenImage( const wxString &strFileName, bool bSetFileName )
01819 {
01820     // Show hourglass cursor.
01821     wxBusyCursor oWait;
01822 
01823     // Save the filename.
01824     wxFileName oFileName( strFileName );
01825     oFileName.Normalize();
01826     wxString strFileNameFullPath= oFileName.GetFullPath();
01827 
01828     // Open file.
01829     if( !m_oImage.LoadFile( strFileNameFullPath ) )
01830     {
01831         // Signal error;
01832         return false;
01833     }
01834 
01835     // Set filename to new one (if indicated).
01836     if( bSetFileName )
01837     {
01838         m_strImageFileName= strFileNameFullPath;
01839         SetTitle( m_strImageFileName + wxT( " - " ) + cstrAppName );
01840     }
01841 
01842     // Get the image count and reset the page number.
01843     m_nImageCount= m_oImage.GetImageCount( m_strImageFileName );
01844     m_nPage= 0;
01845 
01846     // Update the page number info in the statusbar and in the toolbar.
01847     UpdatePageNumber();
01848 
01849     // Get the image resolution (or use the resolution value of the last
01850     // acquired/loaded image by not changing the resolution).
01851     if( m_oImage.HasOption( wxIMAGE_OPTION_RESOLUTION ) )
01852     {
01853         m_nResolution= m_oImage.GetOptionInt( wxIMAGE_OPTION_RESOLUTION );
01854     }
01855     else if( m_oImage.HasOption( wxIMAGE_OPTION_RESOLUTIONX ) )
01856     {
01857         m_nResolution= m_oImage.GetOptionInt( wxIMAGE_OPTION_RESOLUTIONX );
01858     }
01859     else if( m_oImage.HasOption( wxIMAGE_OPTION_RESOLUTIONY ) )
01860     {
01861         m_nResolution= m_oImage.GetOptionInt( wxIMAGE_OPTION_RESOLUTIONY );
01862     }
01863 
01864     // Show the image on the canvas window.
01865     UpdateBitmap();
01866 
01867     // Enable (additional) menu options after the 1st scan.
01868     EnableControls();
01869 
01870     // Add filename to file history.
01871     m_oFileHistory.AddFileToHistory( m_strImageFileName );
01872 
01873     // Signal success.
01874     return true;
01875 }
01876 
01877 // (Re)loads the (zero based) page number given in m_nPage.
01878 //
01879 bool wxIScanFrame::OnDocumentPageChanged()
01880 {
01881     // Show hourglass cursor.
01882     wxBusyCursor oWait;
01883 
01884     // Load the image by name and page number.
01885     if( !m_oImage.LoadFile( m_strImageFileName, wxBITMAP_TYPE_ANY, m_nPage ) )
01886     {
01887         // Signal error.
01888         return false;
01889     }
01890 
01891     // Get the image direction.
01892 //    m_bPortrait= ( m_oImage.GetHeight() > m_oImage.GetWidth() );
01893 
01894     // Update the page number info in the statusbar and in the toolbar.
01895     UpdatePageNumber();
01896 
01897     // Show the image
01898     UpdateBitmap();
01899 
01900     // Update the menu and toolbar controls.
01901     EnableControls();
01902 
01903     // Signal success.
01904     return true;
01905 }
01906 
01907 // Update the canvas bitmap.
01908 //
01909 void wxIScanFrame::UpdateBitmap()
01910 {
01911     // Some little optimization for empty images.
01912     if( !m_oImage.IsOk() )
01913     {
01914         return;
01915     }
01916 
01917     // Resize image (if necessary).
01918     switch( m_nFit )
01919     {
01920         case wxCANVASFITWIDTH   : {
01921                                       int x, nDummy;
01922 
01923                                       m_poCanvas->GetClientSize( &x, &nDummy );
01924 
01925                                       m_nScaleFactor= (double)x / (double)m_oImage.GetWidth();
01926                                   }
01927                                   break;
01928         case wxCANVASFITWINDOW  : {
01929                                       int x, y;
01930 
01931                                       m_poCanvas->GetClientSize( &x, &y );
01932 
01933                                       double nScaleFactorX= (double)x / (double)m_oImage.GetWidth();
01934                                       double nScaleFactorY= (double)y / (double)m_oImage.GetHeight();
01935 
01936                                       m_nScaleFactor= nScaleFactorX > nScaleFactorY ? nScaleFactorY : nScaleFactorX;
01937                                   }
01938                                   break;
01939         case wxCANVASCUSTOMSIZE : break;
01940         default                 : m_nScaleFactor= 1.0;
01941                                   break;
01942     }
01943 
01944     // Some little optimization:
01945     //  - A scalation factor of 1.0 means there is no scalation at all.
01946     if( m_nScaleFactor == 1.0 )
01947     {
01948         m_poCanvas->SetBitmap( m_oImage );
01949     }
01950     else
01951     {
01952         // Show busy cursor (until the automatic variable is destroyed).
01953         wxBusyCursor oWait;
01954 
01955         m_poCanvas->SetBitmap( m_oImage.Scale(
01956             (int)( (double)m_oImage.GetWidth()  * m_nScaleFactor ),
01957             (int)( (double)m_oImage.GetHeight() * m_nScaleFactor ),
01958             wxIMAGE_QUALITY_HIGH ) );
01959     }
01960 }
01961 
01962 // Get the selected sub image.
01963 //
01964 wxImage wxIScanFrame::GetImageSelection()
01965 {
01966     if( m_poCanvas->IsSelected() )
01967     {
01968         wxRect oSelection= m_poCanvas->GetSelection();
01969 
01970         oSelection.x     = int( (double)oSelection.x      / m_nScaleFactor );
01971         oSelection.y     = int( (double)oSelection.y      / m_nScaleFactor );
01972         oSelection.width = int( (double)oSelection.width  / m_nScaleFactor );
01973         oSelection.height= int( (double)oSelection.height / m_nScaleFactor );
01974 
01975         int nWidth= m_oImage.GetWidth();
01976         if( nWidth < ( oSelection.x + oSelection.width ) )
01977         {
01978             oSelection.width= nWidth - oSelection.x;
01979         }
01980 
01981         int nHeight= m_oImage.GetHeight();
01982         if( nHeight < ( oSelection.y + oSelection.height ) )
01983         {
01984             oSelection.height= nHeight - oSelection.y;
01985         }
01986 
01987         return m_oImage.GetSubImage( oSelection );
01988     }
01989     else
01990     {
01991         return m_oImage;
01992     }
01993 }
01994 
01995 // Get an image from scanner.
01996 //
01997 bool wxIScanFrame::ScanImage()
01998 {
01999 #ifdef __WXSCANSANE__
02000     // Enter critical section to protect m_poScanner.
02001     m_oCriticalSectionScanner.Enter();
02002 
02003     // ...
02004     if( !m_poScanner->SetDeviceIndex( m_poChoiceScanner->GetSelection() ) )
02005     {
02006         wxLogWarning( ( m_poScanner->GetDeviceCount() ? _( "The selected scanning device is invald." )
02007                                                         : _( "No scanning device found." ) )
02008                         + wxString( wxT( " " ) )
02009                         + _( "Rescanning for scanning devices..." ) );
02010 
02011         // Leave the critical section that protects m_poScanner.
02012         //
02013         // Note: We do this because ScanForScanners() enters the
02014         //       the same critical section.
02015         //
02016         m_oCriticalSectionScanner.Leave();
02017 
02018         // Rescan for scanning devices.
02019         ScanForScanners();
02020 
02021         if( !m_poScanner->SetDeviceIndex( m_poChoiceScanner->GetSelection() ) )
02022         {
02023             wxLogError( m_poScanner->GetDeviceCount() ? _( "The selected scanning device is invald." )
02024                                                         : _( "No scanning device found." ) );
02025             return false;
02026         }
02027 
02028         // Reenter critical section to protect m_poScanner.
02029         m_oCriticalSectionScanner.Enter();
02030     }
02031 #endif // __WXSCANSANE__
02032 
02033     // Check if the scanner object initialization was successful.
02034     if( !m_poScanner->IsOk() )
02035     {
02036         wxLogError( _( "Cannot initialize scanner object." ) );
02037         return false;
02038     }
02039 
02040     // Scan an image, ...
02041     if( !m_poScanner->ScanImage( m_oImage ) )
02042     {
02043         wxLogError( _( "An error occured while scanning." ) );
02044         return false;
02045     }
02046 
02047     // ... get its resolution, ...
02048     m_nResolution= m_poScanner->GetResolution();
02049 
02050 #ifdef __WXSCANSANE__
02051     // Leave the critical section that protects m_poScanner.
02052     m_oCriticalSectionScanner.Leave();
02053 #endif // __WXSCANSANE__
02054 
02055     // ... apply an image profile (crop, convert to monochrome etc.), and ...
02056     if( m_bAutoApplyProfile )
02057     {
02058         ApplyProfile();
02059     }
02060 
02061     // ... show the image on the canvas window.
02062     UpdateBitmap();
02063 
02064     // Reset the image count, the page number and the image filename.
02065     ResetFileName();
02066 
02067     // Signal success.
02068     return true;
02069 }
02070 
02071 #ifdef __WXSCANSANE__
02072 // Scan for existing SANE scanners an fill the according wxChoice
02073 // object in the tool bar.
02074 //
02075 void wxIScanFrame::ScanForScanners( bool bAsync )
02076 {
02077     // Enter critical section to protect m_poScanner.
02078     //
02079     // Note: The corresponding ScanForScannersThreadExit().Leave() call
02080     //       is included in wxIScanFrame::ScanForScannersThreadEntry().
02081     //
02082     m_oCriticalSectionScanner.Enter();
02083 
02084     // If there is a current scanner object destroy it.
02085     if( m_poScanner )
02086     {
02087         delete m_poScanner;
02088         m_poScanner= NULL;
02089     }
02090 
02091     // Empty the wxChoice object for scanners.
02092     m_poChoiceScanner->Clear();
02093 
02094     // Enable/disable menu options according to the actual state.
02095     EnableControls();
02096 
02097     // Switch between asynchronous and synchronous version of
02098     // creating a new scanner object and filling the choice object.
02099     if( bAsync )
02100     {
02101         // Create a new thread that creates a new scanner object
02102         // (and scans therefore for available scanners).
02103         wxIScanFrameScanForScannersThread * poThread= new wxIScanFrameScanForScannersThread( this );
02104 
02105         poThread->Create();
02106         poThread->Run();
02107     }
02108     else
02109     {
02110         // Show hourglass cursor.
02111         wxBusyCursor oWait;
02112 
02113         // Create a new scanner object (and scan for scanning devices)
02114         ScanForScannersThreadEntry();
02115 
02116         // Fill the wxChoice object for scanners with a list of
02117         // available scanning devices.
02118         ScanForScannersThreadExit();
02119     }
02120 }
02121 
02122 // Create a new scanner object and scan for scanning devices.
02123 //
02124 // Note: This function should not be called directly.
02125 //
02126 void wxIScanFrame::ScanForScannersThreadEntry()
02127 {
02128     // Create a new scanner object (and scan for scanning devices)
02129     m_poScanner= new wxScan;
02130 }
02131 
02132 // Fill the wxChoice object for scanners with a list of available
02133 // scanning devices.
02134 //
02135 // Note: This function should not be called directly.
02136 //
02137 void wxIScanFrame::ScanForScannersThreadExit()
02138 {
02139     // Check...
02140     if( !m_poScanner )
02141     {
02142         // Log an error message and leave.
02143         wxLogError( _( "Cannot create a scanning device object (wxScan)." ) );
02144     }
02145 
02146     // Fill the wxChoice object for scanners with a list of
02147     // available scanning devices.
02148     else if( m_poScanner->IsOk() )
02149     {
02150         int nCount= m_poScanner->GetDeviceCount();
02151 
02152         if( nCount > 0 )
02153         {
02154             for( int i= 0; i < nCount; i++ )
02155             {
02156                 m_poChoiceScanner->Append(
02157                                            m_poScanner->GetDeviceModel( i )
02158                                            + wxT( " (" )
02159                                            + m_poScanner->GetDeviceName( i )
02160                                            + wxT( ")" )
02161                                          );
02162             }
02163             m_poChoiceScanner->SetSelection( m_poScanner->GetDeviceIndex() );
02164         }
02165         else
02166         {
02167             wxLogWarning( _( "No scanning device found." ) );
02168         }
02169     }
02170 
02171     // Leave the critical section that protects m_poScanner.
02172     m_oCriticalSectionScanner.Leave();
02173 
02174     // Enable/disable menu options according to the actual state.
02175     EnableControls();
02176 }
02177 #endif // __WXSCANSANE__
02178 
02179 // Select a profile (by number).
02180 //
02181 void wxIScanFrame::SetProfile( int nProfileIndex )
02182 {
02183     GetMenuBar()->Check( ID_DOCUMENTPROFILE0 + m_nProfileIndex, false );
02184     m_nProfileIndex= nProfileIndex;
02185     GetMenuBar()->Check( ID_DOCUMENTPROFILE0 + m_nProfileIndex, true  );
02186 }
02187 
02188 // Apply the selected profile to the image.
02189 //
02190 void wxIScanFrame::ApplyProfile()
02191 {
02192     // Show hourglass cursor.
02193     wxBusyCursor oWait;
02194 
02195     // Get profile configuration.
02196     wxConfigBase *poConfig= wxConfigBase::Get();
02197 
02198     poConfig->SetRecordDefaults();
02199     poConfig->SetPath( wxString::Format( wxT( "/Profile/%d" ), m_nProfileIndex ) );
02200 
02201 #if __WXLUASCRIPT__
02202     wxString strScriptStartupFunction= poConfig->Read( wxT( "Function" ), wxT( "" ) );
02203 
02204     if( m_bLua && !strScriptStartupFunction.IsEmpty() )
02205     {
02206         wxString strScriptFileName= poConfig->Read( wxT( "Script" ), wxT( "" ) );
02207 
02208         StartScript( strScriptStartupFunction, strScriptFileName );
02209     }
02210     else
02211 #endif // __WXLUASCRIPT__
02212     {
02213         // Crop image to the corresponding image rect.
02214         bool bCrop= (bool)poConfig->Read( wxT( "Crop" ), (long)false );
02215 
02216         if( bCrop )
02217         {
02218             int x= poConfig->Read( wxT( "x" ), 0L );
02219             int y= poConfig->Read( wxT( "y" ), 0L );
02220             int w= poConfig->Read( wxT( "w" ), (long)( 210.0 / 25.4 * (double)m_nResolution ) );
02221             int h= poConfig->Read( wxT( "h" ), (long)( 297.0 / 25.4 * (double)m_nResolution ) );
02222 
02223             m_oImage= ::CropImage( m_oImage, x, y, w, h );
02224         }
02225 
02226         // Convert the image to greyscale.
02227         bool bGreyScale= (bool)poConfig->Read( wxT( "GreyScale" ), (long)false );
02228 
02229         if( bGreyScale )
02230         {
02231             m_oImage= ::Image2Grey( m_oImage );
02232         }
02233 
02234         // Convert the image to mono chrome.
02235         bool bMonoChrome= (bool)poConfig->Read( wxT( "MonoChrome" ), (long)false );
02236 
02237         if( bMonoChrome )
02238         {
02239             int nMonoThreshold= poConfig->Read( wxT( "MonoThreshold" ), 128L );
02240 
02241             m_oImage= ::Image2Mono( m_oImage, nMonoThreshold);
02242         }
02243 
02244         // Rotate the image 90° to the right (multiple times).
02245         int nRotate90Right= poConfig->Read( wxT( "Rotate90Right" ), 0L );
02246 
02247         for( int i= 0; i < nRotate90Right; i++ )
02248         {
02249             m_oImage= m_oImage.Rotate90();
02250         }
02251     }
02252 }
02253 
02254 // Call an external OCR software and return the filename
02255 // of the resulting text file.
02256 //
02257 wxString wxIScanFrame::OCR( const wxString &strTempFileName, bool bUseHocr )
02258 {
02259     wxString strCommand;            // Command line to execute as an external process.
02260     wxString strReturnFileName;     // ...
02261 
02262     // Check if OCR command is enabled.
02263     if( !m_bCmdOcrEnable )
02264     {
02265 #if __DEBUG__
02266         wxLogWarning( wxString( wxT( "wxIScanFrame::OCR - " ) )
02267                         + _( "OCR command is disabled by option." ) );
02268 #endif
02269         return wxEmptyString;
02270     }
02271 
02272     // Convert image to a monochrome version, ...
02273     if( m_bCmdConvertEnable )
02274     {
02275         // ... either by using an external command, ...
02276         strCommand.Printf( m_strCmdConvert,
02277                            strTempFileName.c_str(),
02278                            strTempFileName.c_str() );
02279 #if __DEBUG__
02280         wxLogMessage( wxT( "wxIScanFrame::OCR - strCommand == '%s'" ), strCommand.c_str() );
02281 #endif
02282         if( !Execute( strCommand ) )
02283         {
02284             wxLogError( wxString( wxT( "wxIScanFrame::OCR - " ) )
02285                           + _( "Monochrome file creation failed." ) );
02286             strReturnFileName.Empty();
02287             goto leave;
02288         }
02289     }
02290     else
02291     {
02292         // ... or by using an internal function (Image2Mono()).
02293         //
02294         // Note: Image2Mono() replaces the image in place, i. e.
02295         //       the monochrome image shows on the screen, too.
02296         //
02297         ::Image2Mono( GetImageSelection() ).SaveFile( strTempFileName, m_strBitmapMimeType );
02298     }
02299 
02300     // Do OCR using an external command.
02301     strCommand.Printf( bUseHocr ? m_strCmdOcrUseHocr : m_strCmdOcr,
02302                        strTempFileName.c_str(),
02303                        strTempFileName.c_str() );
02304 #if __DEBUG__
02305     wxLogMessage( wxT( "wxIScanFrame::OCR - strCommand == '%s'" ), strCommand.c_str() );
02306 #endif
02307     if( !Execute( strCommand ) )
02308     {
02309         wxLogError( wxString( wxT( "wxIScanFrame::OCR - " ) )
02310                       + _( "OCR failed." ) );
02311         strReturnFileName.Empty();
02312         goto leave;
02313     }
02314 
02315     strReturnFileName= strTempFileName + wxT( "." ) + ( bUseHocr ? m_strOcrOutputExtensionHocr : m_strOcrOutputExtension );
02316 
02317 leave:
02318     // Clean up temporary image file.
02319     if(m_bCmdCleanUpEnable )
02320     {
02321 
02322         // Clean up.
02323         strCommand.Printf( m_strCmdCleanUp,
02324                            strTempFileName.c_str(),
02325                            strTempFileName.c_str() );
02326 #if __DEBUG__
02327         wxLogMessage( wxT( "wxIScanFrame::OCR - strCommand == '%s'" ), strCommand.c_str() );
02328 #endif
02329         Execute( strCommand );
02330     }
02331 
02332     // Clean up temporary hOCR file(s).
02333     if( bUseHocr && m_bCmdHocrCleanUpEnable )
02334     {
02335 
02336         // Clean up.
02337         strCommand.Printf( m_strCmdHocrCleanUp,
02338                            strTempFileName.c_str(),
02339                            strTempFileName.c_str() );
02340 #if __DEBUG__
02341         wxLogMessage( wxT( "wxIScanFrame::OCR - strCommand == '%s'" ), strCommand.c_str() );
02342 #endif
02343         Execute( strCommand );
02344     }
02345 
02346     // Return filename of the created text file (or an empty string on errors).
02347     return strReturnFileName;
02348 }
02349 
02350 // Do OCR and read out the returned text.
02351 //
02352 bool wxIScanFrame::ReadOut( bool bAsync )
02353 {
02354     // Save image in a temporary JPEG file.
02355     wxString strTempFileName= wxFileName::CreateTempFileName( wxT( "" ) );
02356 
02357     GetImageSelection().SaveFile( strTempFileName, m_strBitmapMimeType );
02358 
02359     // Do OCR, and get the temporary text file name.
02360     wxString strTempTextFileName= OCR( strTempFileName );
02361 
02362     // Remove temporary JPEG file.
02363     ::wxRemoveFile( strTempFileName );
02364 
02365     // Read the text using a custom command (if it is not the empty string).
02366     if( strTempTextFileName.IsEmpty() )
02367     {
02368         // Signal error.
02369         return false;
02370     }
02371 
02372     wxString strCommand= wxString::Format( m_strCmdTts, strTempTextFileName.c_str() );
02373 
02374 #if __DEBUG__
02375     wxLogMessage( wxT( "wxIScanFrame::ReadText - strCommand == '%s'" ), strCommand.c_str() );
02376 #endif
02377 
02378     // Execute the read out process.
02379     return Execute( strCommand, bAsync, strTempTextFileName );
02380 }
02381 
02382 // Fork an external process.
02383 //
02384 bool wxIScanFrame::Execute( const wxString &strCommand, bool bAsync, const wxString &strTempTextFileName )
02385 {
02386     if( bAsync )
02387     {
02388         // Check if an asynchronous process is still running.
02389         if( m_poRunningProcess )
02390         {
02391             // Log an error message.
02392             wxLogError( wxString( wxT( "wxIScanFrame::Execute - " ) )
02393                           + _( "Cannot create new asynchronous process. An old one is still running. "
02394                                "Please wait until it stops or press the STOP button in the taskbar." ) );
02395 
02396             // Signal error.
02397             return false;
02398         }
02399 
02400         // Create an asynchronous running process, ...
02401         if( strTempTextFileName.IsEmpty() )
02402         {
02403             m_poRunningProcess= new wxExtProcess( this, wxISCAN_PROCESS );
02404         }
02405         else
02406         {
02407             m_poRunningProcess= new wxExtProcessDelTempFile( this, wxISCAN_PROCESS, strTempTextFileName );
02408         }
02409 
02410         // ... check if is was created correctly, ...
02411         if( !m_poRunningProcess )
02412         {
02413             // Log an error message.
02414             wxLogError( wxString( wxT( "wxIScanFrame::Execute - " ) )
02415                           + _( "Cannot create process object." ) );
02416 
02417             // Signal error.
02418             return false;
02419         }
02420 
02421         // ... redirect its output, ...
02422 //        m_poRunningProcess->Redirect();
02423 
02424         // ... and execute it.
02425         if( !m_poRunningProcess->Execute( strCommand, wxEXEC_ASYNC | wxEXEC_MAKE_GROUP_LEADER ) )
02426         {
02427             // Log an error message.
02428             wxLogError( wxString( wxT( "wxIScanFrame::Execute - " ) )
02429                           + wxString::Format( _( "Cannot start asynchronous process [%s]." ),
02430                                               strCommand.c_str() ) );
02431 
02432             // Signal error.
02433             return false;
02434         }
02435     }
02436     else
02437     {
02438         // Create a process, ...
02439         wxProcess oProcess( this );
02440 
02441         // ... redirect its output, ...
02442         oProcess.Redirect();
02443 
02444         // ... execute it synchronous, and await its ending.
02445         long nExitCode= ::wxExecute( strCommand, wxEXEC_SYNC, &oProcess );
02446 
02447        // Log process's output.
02448         LogProcessOutput( &oProcess );
02449 
02450         if( nExitCode )
02451         {
02452             // Log an error message.
02453             wxLogError( wxString( wxT( "wxIScanFrame::Execute - " ) )
02454                           + wxString::Format( _( "An error occured on synchronous process [%s]." ),
02455                                               strCommand.c_str() ) );
02456 
02457             // Signal error.
02458             return false;
02459         }
02460     }
02461 
02462     // Signal success.
02463     return true;
02464 }
02465 
02466 // Reset the filename to "unknown.jpg" etc.
02467 //
02468 void wxIScanFrame::ResetFileName()
02469 {
02470     // Reset the file name.
02471     m_strImageFileName= _( "unknown.jpg" );
02472     SetTitle( cstrAppName );
02473 
02474     // Reset the image count, the page number and the image filename.
02475     m_nImageCount= 1;
02476     m_nPage= 0;
02477 
02478     // Update the page number info in the statusbar and in the toolbar.
02479     UpdatePageNumber();
02480 }
02481 
02482 // Write a sub process's stdout and stderr output to the program's log.
02483 //
02484 void wxIScanFrame::LogProcessOutput( wxProcess *poProcess )
02485 {
02486     // Save the log text control's default colour.
02487     wxTextAttr oStyle( m_poLogTextCtrl->GetDefaultStyle() );
02488     wxColour oOldColor( oStyle.GetTextColour() );
02489 
02490     // Check if there is text on the process's stdout and stderr stream
02491     // and log it.
02492     //
02493     // Process's stdout in light blue:
02494     wxTextInputStream oIn( *poProcess->GetInputStream() );
02495 
02496     oStyle.SetTextColour( wxColour( 64, 64, 255 ) );
02497     m_poLogTextCtrl->SetDefaultStyle( oStyle );
02498     while( poProcess->IsInputAvailable() )
02499     {
02500         wxLogMessage( oIn.ReadLine() );
02501     }
02502 
02503     // Process's stderr in light violet:
02504     wxTextInputStream oErr( *poProcess->GetErrorStream() );
02505 
02506     oStyle.SetTextColour( wxColour( 192, 96, 192 ) );
02507     m_poLogTextCtrl->SetDefaultStyle( oStyle );
02508     while( poProcess->IsErrorAvailable() )
02509     {
02510         wxLogMessage( oErr.ReadLine() );
02511     }
02512 
02513     // Restore the log text control's default colour.
02514     oStyle.SetTextColour( oOldColor );
02515     m_poLogTextCtrl->SetDefaultStyle( oStyle );
02516 }
02517 
02518 // Update the page number info in the statusbar and in the toolbar.
02519 //
02520 void wxIScanFrame::UpdatePageNumber()
02521 {
02522     SetStatusText( wxString::Format( wxT( "%d/%d" ),  m_nPage + 1, m_nImageCount ), 2 );
02523     m_poTextCtrlPage->SetValue( wxString::Format( wxT( "%d" ),  m_nPage + 1 ) );
02524 }
02525 
02526 // Enable or disable controls according to the actual progam state.
02527 //
02528 void wxIScanFrame::EnableControls()
02529 {
02530     // Enable/disable menu items (on first call).
02531     bool bImageOk= !m_bUILocked && m_oImage.IsOk();
02532 
02533     GetMenuBar()->Enable(     ID_FILESAVEAS,                bImageOk );
02534     GetToolBar()->EnableTool( ID_FILESAVEAS_TB,             bImageOk );
02535     GetMenuBar()->Enable(     ID_FILEPRINT,                 bImageOk );
02536     GetToolBar()->EnableTool( ID_FILEPRINT_TB,              bImageOk );
02537     GetMenuBar()->Enable(     ID_FILEPRINTPREVIEW,          bImageOk );
02538     GetMenuBar()->Enable(     ID_FILEPRINTSETUP,            bImageOk );
02539     GetMenuBar()->Enable(     ID_EDITCOPY,                  bImageOk );
02540     GetMenuBar()->Enable(     ID_VIEWZOOMIN,                bImageOk );
02541     GetMenuBar()->Enable(     ID_VIEWZOOMOUT,               bImageOk );
02542     GetMenuBar()->Enable(     ID_VIEWZOOMSELECTION,         bImageOk );
02543 #if __WXPDFDOC__
02544     GetMenuBar()->Enable(     ID_PDFBEGINPDF,               true     );
02545     GetToolBar()->EnableTool( ID_PDFBEGINPDF_TB,            true     );
02546     GetMenuBar()->Enable(     ID_PDFADDPAGE,                bImageOk );
02547     GetToolBar()->EnableTool( ID_PDFADDPAGE_TB,             bImageOk );
02548 #else
02549     GetMenuBar()->Enable(     ID_PDFBEGINPDF,               false    );
02550     GetToolBar()->EnableTool( ID_PDFBEGINPDF_TB,            false    );
02551     GetMenuBar()->Enable(     ID_PDFADDPAGE,                false    );
02552     GetToolBar()->EnableTool( ID_PDFADDPAGE_TB,             false    );
02553 #endif // __WXPDFDOC__
02554     GetMenuBar()->Enable(     ID_DOCUMENTROTATE90LEFT,      bImageOk );
02555     GetMenuBar()->Enable(     ID_DOCUMENTROTATE90RIGHT,     bImageOk );
02556     GetMenuBar()->Enable(     ID_DOCUMENTCROP,              bImageOk );
02557     GetMenuBar()->Enable(     ID_DOCUMENTAPPLYPROFILE,      bImageOk );
02558 
02559 #if __WXPDFDOC__
02560     // Enable/disable PDF file save controls.
02561     bool bCanSavePdfFile= !m_bUILocked && ( m_nPdfPages > 0 );
02562 
02563     GetMenuBar()->Enable(     ID_PDFSAVEASPDF,              bCanSavePdfFile );
02564     GetToolBar()->EnableTool( ID_PDFSAVEASPDF_TB,           bCanSavePdfFile );
02565 #else
02566     GetMenuBar()->Enable(     ID_PDFSAVEASPDF,              false );
02567     GetToolBar()->EnableTool( ID_PDFSAVEASPDF_TB,           false );
02568 #endif // __WXPDFDOC__
02569 
02570     // ...
02571     bool bCopyText= bImageOk && m_bCmdOcrEnable;
02572 
02573     GetMenuBar()->Enable(     ID_DOCUMENTCOPYTEXT,         bCopyText );
02574 
02575     // ...
02576     GetMenuBar()->Enable(     ID_EDITSETTINGSFILE ,        m_bCmdEditEnable );
02577 
02578     // Enable text-to-speach controls
02579     bool bTTS= bImageOk
02580                 && m_bCmdOcrEnable
02581                 && m_bCmdTtsEnable
02582                 && ( m_poRunningProcess ? false : true );
02583 
02584     GetMenuBar()->Enable(     ID_DOCUMENTREADTEXT,          bTTS );
02585     GetToolBar()->EnableTool( ID_DOCUMENTREADTEXT_TB,       bTTS );
02586 
02587     // Enable/disable page controls
02588     bool bNotFirstPage= ( m_nPage > 0 );
02589     bool bNotLastPage=  ( m_nPage < ( m_nImageCount - 1 ) );
02590 
02591     GetMenuBar()->Enable(     ID_DOCUMENTFIRSTPAGE,         bImageOk && bNotFirstPage );
02592     GetToolBar()->EnableTool( ID_DOCUMENTFIRSTPAGE_TB,      bImageOk && bNotFirstPage );
02593     GetMenuBar()->Enable(     ID_DOCUMENTPREVIOUSPAGE,      bImageOk && bNotFirstPage );
02594     GetToolBar()->EnableTool( ID_DOCUMENTPREVIOUSPAGE_TB,   bImageOk && bNotFirstPage );
02595     GetMenuBar()->Enable(     ID_DOCUMENTNEXTPAGE,          bImageOk && bNotLastPage  );
02596     GetToolBar()->EnableTool( ID_DOCUMENTNEXTPAGE_TB,       bImageOk && bNotLastPage  );
02597     GetMenuBar()->Enable(     ID_DOCUMENTLASTPAGE,          bImageOk && bNotLastPage  );
02598     GetToolBar()->EnableTool( ID_DOCUMENTLASTPAGE_TB,       bImageOk && bNotLastPage  );
02599     GetToolBar()->EnableTool( ID_TEXTCTRLPAGE,              bImageOk                  );
02600 
02601     // Enable process killer.
02602     GetMenuBar()->Enable(     ID_CMDSTOP,                   m_poRunningProcess );
02603     GetToolBar()->EnableTool( ID_CMDSTOP_TB,                m_poRunningProcess );
02604 
02605     // Enable scanning.
02606     bool bScanEnabled= !m_bUILocked && m_poScanner && m_poScanner->IsOk();
02607 
02608     GetMenuBar()->Enable(     ID_FILESCANIMAGE,             bScanEnabled );
02609     GetToolBar()->EnableTool( ID_FILESCANIMAGE_TB,          bScanEnabled );
02610 #ifdef __WXSCANSANE__
02611     GetMenuBar()->Enable(     ID_EDITSCANSCANNERS ,         !m_bUILocked && m_poScanner /*bScanEnabled*/ );
02612     GetToolBar()->EnableTool( ID_CHOICESCANNER,             bScanEnabled );
02613 #else // __WXSCANSANE__
02614     GetMenuBar()->Enable(     ID_EDITSCANSCANNERS ,         false );
02615 #endif // __WXSCANSANE__
02616 
02617     // Lock ...
02618     GetMenuBar()->Enable(     ID_FILEOPEN,                  !m_bUILocked );
02619     GetToolBar()->EnableTool( ID_FILEOPEN_TB,               !m_bUILocked );
02620     GetMenuBar()->Enable(     ID_EDITPASTE,                 !m_bUILocked );
02621 
02622     // Check the active viewing (fitting) option.
02623     GetMenuBar()->Check( ID_VIEWFITWIDTH,   false );
02624     GetMenuBar()->Check( ID_VIEWFITWINDOW,  false );
02625     GetMenuBar()->Check( ID_VIEWFULLSIZE,   false );
02626     GetMenuBar()->Check( ID_VIEWCUSTOMSIZE, false );
02627     switch( m_nFit )
02628     {
02629         case wxCANVASFITWIDTH   : GetMenuBar()->Check( ID_VIEWFITWIDTH,   true );
02630                                   break;
02631         case wxCANVASFITWINDOW  : GetMenuBar()->Check( ID_VIEWFITWINDOW,  true );
02632                                   break;
02633         case wxCANVASFULLSIZE   : GetMenuBar()->Check( ID_VIEWFULLSIZE,   true );
02634                                   break;
02635         case wxCANVASCUSTOMSIZE : GetMenuBar()->Check( ID_VIEWCUSTOMSIZE, true );
02636                                   break;
02637     }
02638 
02639     // ... file history... .
02640     for( unsigned i= 0; i <  m_oFileHistory.GetCount(); i++ )
02641     {
02642         GetMenuBar()->Enable( m_oFileHistory.GetBaseId() + i, !m_bUILocked );
02643     }
02644 
02645 #if __WXLUASCRIPT__
02646     // Enable/disable Lua script menu.
02647     GetMenuBar()->EnableTop( wxISCAN_SCRIPTMENUPOS, m_bLua );
02648     for( int i= 0; i < m_nLuaCount; i++ )
02649     {
02650         GetMenuBar()->Enable( ID_DOCUMENTPROFILE0 + 1000 + i, m_bLua );
02651     }
02652 #endif // __WXLUASCRIPT__
02653 
02654     // Disable (by compiler option) not implemented controls .
02655 #if defined( __WXPDFDOC__ ) && __WXPDFDOC__ >= 1
02656     GetMenuBar()->Enable(     ID_PDFBEGINPDF,               !m_bUILocked );
02657     GetToolBar()->EnableTool( ID_PDFBEGINPDF_TB,            !m_bUILocked );
02658 #else
02659     GetMenuBar()->Enable(     ID_PDFBEGINPDF,               false );
02660     GetToolBar()->EnableTool( ID_PDFBEGINPDF_TB,            false );
02661 #endif
02662 }
02663 
02664 #if __WXPDFDOC__
02665 // PDF creation functions.
02666 //
02667 // Create (and initialize) a new PDF document.
02668 //
02669 bool wxIScanFrame::CreateNewPdfDocument()
02670 {
02671     // Delete any old PDF documents.
02672     if( m_poPdfDoc  )
02673     {
02674         delete m_poPdfDoc;
02675     }
02676     SetStatusText( wxString( wxT( "PDF: " ) ) + _( "empty" ), 1 );
02677 
02678     // Create a new PDF document in portrait mode on A4 paper using mm
02679     // as measurement unit.
02680     m_poPdfDoc= new wxPdfDocument( wxPORTRAIT,
02681                                    wxString( _T( "mm" ) ),
02682                                    m_oPrintData.GetPaperId() );
02683     m_nPdfPages= 0;
02684     m_bPdfDirty= false;
02685     if( !m_poPdfDoc )
02686     {
02687         wxLogError( _( "Cannot create wxPdfDocument." ) );
02688     }
02689     m_poPdfDoc->SetCreator( ::wxGetApp().GetAppName() );
02690     m_poPdfDoc->SetAutoPageBreak( false );
02691     m_poPdfDoc->SetCompression( true );
02692     return true;
02693 }
02694 
02695 // Add a new image/OCR text page to the existing PDF document.
02696 //
02697 bool wxIScanFrame::AddPdfPage()
02698 {
02699     if( !m_poPdfDoc )
02700     {
02701         wxLogError( wxString( wxT( "wxIScanFrame::AddPdfPage - " ) )
02702                       + _( "wxPdfDocument is NULL" ) );
02703         return false;
02704     }
02705 
02706     // Save image in a temporary file.
02707     wxString strTempFileName= wxFileName::CreateTempFileName( wxEmptyString );
02708 
02709     m_oImage.SaveFile( strTempFileName, m_strPdfBitmapMimeType );
02710 
02711 #if defined( __WXSCANDEBUG__ ) && __WXSCANDEBUG__ >= 1
02712     wxLogMessage( wxT( "PDF PARAMETERS" ) );
02713     wxLogMessage( wxT( "\tGetPageWidth == %.2f mm (%f)"  ), m_poPdfDoc->GetPageWidth(),  m_poPdfDoc->GetPageWidth()  / m_oImage.GetWidth() );
02714     wxLogMessage( wxT( "\tGetPageHeight == %.2f mm (%f)" ), m_poPdfDoc->GetPageHeight(), m_poPdfDoc->GetPageHeight() / m_oImage.GetHeight() );
02715     wxLogMessage( wxT( "\tGetImageScale == %f (new: %f)" ), m_poPdfDoc->GetImageScale(), (double)m_nResolution / 72.0 );
02716     wxLogMessage( wxT( "\tGetScaleFactor == %f" ), m_poPdfDoc->GetScaleFactor() );
02717     wxLogMessage( wxT( "\tm_nResolution == %.2f DPI (%.2f/mm)" ), (double)m_nResolution, (double)m_nResolution / 25.4 );
02718     wxLogMessage( wxT( "============" ) );
02719     ::wxSafeYield();
02720 #endif
02721 
02722     // Add a page, ...
02723     m_poPdfDoc->AddPage( IsPortrait() ? wxPORTRAIT : wxLANDSCAPE );
02724 
02725     // ... do some OCR and include the recognized text if enabled, and ...
02726     if( m_bOCR )
02727     {
02728         wxString strOcrTextFileName= OCR( strTempFileName, m_bHocrEnable );
02729 
02730         if( !strOcrTextFileName.IsEmpty() )
02731         {
02732             m_poPdfDoc->SetFont( wxT( "Times" ), wxPDF_FONTSTYLE_REGULAR, 8 );
02733             m_poPdfDoc->SetTextRenderMode( wxPDF_TEXT_RENDER_INVISIBLE );
02734 
02735             if( m_bHocrEnable )
02736             {
02737                 wxXmlDocument oXmlDoc;
02738 
02739                 // Try to load the OCR result file as XML (XHTML).
02740                 {
02741                     // Suppress the error message if wxXmlDocument::Load(...) fails.
02742                     //
02743                     // Note: We want to suppress only the logging of
02744                     //       wxXmlDocument::Load(...). Therefore we use a local
02745                     //       variable oLogNull in an additional inner context
02746                     //       which is automatically destroyed after leaving this
02747                     //       context.
02748                     //
02749                     wxLogNull oLogNull;
02750 
02751                     // Load the XML file.
02752                     //
02753                     // Note: We need the flag wxXMLDOC_KEEP_WHITESPACE_NODES
02754                     //       because we need the whitespace for flushing!
02755                     oXmlDoc.Load( strOcrTextFileName, wxT( "UTF-8" ), wxXMLDOC_KEEP_WHITESPACE_NODES );
02756                 }
02757 
02758                 // If this was not successfull convert try to convert from HTML to XHTML.
02759                 if( !oXmlDoc.IsOk() && m_bCmdHtml2XhtmlEnable )
02760                 {
02761                     // Try to convert the file from HTML to XHTML.
02762                     wxString strCommand= wxString::Format( m_strCmdHtml2Xhtml, strOcrTextFileName.c_str() );
02763 #if __DEBUG__
02764                     wxLogMessage( wxT( "wxIScanFrame::AddPdfPage - strCommand == '%s'" ), strCommand.c_str() );
02765 #endif
02766                     Execute( strCommand );
02767 
02768                     // Retry to load the file as XHTML file.
02769                     {
02770                         // Suppress the error message if wxXmlDocument::Load(...)
02771                         // fails.
02772                         wxLogNull oLogNull;
02773 
02774                         // Load the XML file.
02775                         //
02776                         // Note: We need the flag wxXMLDOC_KEEP_WHITESPACE_NODES
02777                         //       because we need the whitespace for flushing!
02778                         oXmlDoc.Load( strOcrTextFileName, wxT( "UTF-8" ), wxXMLDOC_KEEP_WHITESPACE_NODES );
02779                     }
02780 
02781                     // In case of an error use the fallback action.
02782                     if( !oXmlDoc.IsOk() )
02783                     {
02784                         // Do a fallback by treating the file as unformated text.
02785                         goto FallBack;
02786                     }
02787                 }
02788 
02789                 // Traverse DOM tree and draw the text on the PDF document.
02790                 if( m_bSmartHocrEnable )
02791                 {
02792                     wxIScanSmartHocr2Pdf oHocr2Pdf( &oXmlDoc, m_poPdfDoc, m_nResolution, m_strHocrClass );
02793 
02794                     oHocr2Pdf.Run();
02795                 }
02796                 else
02797                 {
02798                     wxIScanHocr2Pdf oHocr2Pdf( &oXmlDoc, m_poPdfDoc, m_nResolution, m_strHocrClass );
02799 
02800                     oHocr2Pdf.Run();
02801                 }
02802 
02803                 // On success leave out the fall back code.
02804                 goto GoOn;
02805             }
02806 
02807 FallBack:
02808             // Handle unformated text file (non-hocr-files).
02809             //
02810             m_poPdfDoc->MultiCell( 0, 3, ::GetTextFromFile( strOcrTextFileName ), wxPDF_BORDER_NONE, wxPDF_ALIGN_LEFT, 1 );
02811 
02812 GoOn:
02813             // Remove the text file created by the OCR program.
02814             ::wxRemoveFile( strOcrTextFileName );
02815         }
02816     }
02817 
02818     // ... put the image onto the current PDF page.
02819     m_poPdfDoc->SetImageScale(  (double)m_nResolution / 72.0 );
02820     m_poPdfDoc->Image( strTempFileName, 0 , 0, 0, 0, m_strPdfBitmapMimeType );
02821 
02822     // Remove temporary file.
02823     ::wxRemoveFile( strTempFileName );
02824 
02825     // Incremement the page counter and mark the PDF document as 'dirty'.;
02826     m_nPdfPages++;
02827     m_bPdfDirty= true;
02828     SetStatusText( wxString::Format( wxT( "PDF: %d" ), m_nPdfPages ), 1 );
02829 
02830     // Add a thumb to the pages panel.
02831     m_poPanelPages->AddImage( m_oImage );
02832 
02833     // Signal success.
02834     return true;
02835 }
02836 #endif // __WXPDFDOC__
02837 
02838 #if __WXLUASCRIPT__
02839 // Start a Lua script from a given file.
02840 //
02841 void wxIScanFrame::StartScript( const wxString &strScriptStartupFunction, wxString strScriptFileName )
02842 {
02843     // Create the final script ...
02844     wxString strScriptCode; // Contains the Lua script code.
02845 
02846     if( strScriptFileName.IsEmpty() )
02847     {
02848         // ... by using the given main function call(s) (chunk).
02849         strScriptCode= strScriptStartupFunction + wxT( "\n" );
02850     }
02851     else
02852     {
02853         // Search for the directory containing the script file:
02854         //
02855         // Check...
02856         if( !::wxFileExists( strScriptFileName ) )
02857         {
02858             // If the script file does not exist replace its directory part by the
02859             // user's home directory.
02860             //
02861             // Signal a warning.
02862             wxLogWarning( wxString( wxT( "wxIScanFrame::StartScript -- " ) )
02863                             + _( "Script file does not exist. I try searching in the home directory." ) );
02864 
02865             // Replace the directory part of the script file name
02866             // by the user's home directory.
02867             wxFileName oScriptFileName( strScriptFileName );
02868 
02869             strScriptFileName= oScriptFileName.GetFullName();
02870             oScriptFileName.AssignHomeDir();
02871             oScriptFileName.SetFullName( strScriptFileName );
02872 
02873             strScriptFileName= oScriptFileName.GetFullPath();
02874     #if __DEBUG__
02875             wxLogWarning( wxT( "wxIScanFrame::StartScript -- strScriptFileName == '%s'" ),
02876                             strScriptFileName.c_str() );
02877     #endif // __DEBUG
02878 
02879                 // Check...
02880             if( !::wxFileExists( strScriptFileName ) )
02881             {
02882                 // If the script file does not exist replace its directory
02883                 // part by the data directory.
02884                 //
02885                 // Display (log) a warning.
02886                 wxLogWarning( wxString( wxT( "wxIScanFrame::StartScript -- " ) )
02887                                 + _( "Script file does not exist. I try searching in the data directory." ) );
02888 
02889                 // Replace the directory part of the script file name
02890                 // by the program's data directory.
02891                 wxFileName oScriptFileName( strScriptFileName );
02892 
02893                 strScriptFileName= oScriptFileName.GetFullName();
02894                 oScriptFileName.AssignDir( wxStandardPaths::Get().GetDataDir() );
02895                 oScriptFileName.SetFullName( strScriptFileName );
02896 
02897                 strScriptFileName= oScriptFileName.GetFullPath();
02898     #if __DEBUG__
02899                 wxLogWarning( wxT( "wxIScanFrame::StartScript -- strScriptFileName == '%s'" ),
02900                                 strScriptFileName.c_str() );
02901     #endif // __DEBUG
02902             }
02903         }
02904 
02905         // ... by concatinating the file content and the main
02906         // function call(s) (chunk).
02907         strScriptCode= ::GetTextFromFile( strScriptFileName ) + wxT( "\n\n" ) +
02908                        strScriptStartupFunction + wxT( "\n" );
02909     }
02910 
02911     // Create the Lua scripting object, ...
02912     wxIScanLuaScript oScript( strScriptCode, this, cstrAppName, &bLuaReturnFromEventHandler );
02913 
02914 #if __WXLUASCRIPT_DYNAMIC__
02915     // Check if the script object is initialized correctly. This is only done
02916     // on dynamic loading of the Lua library because only there the library
02917     // may not be loaded correctly.
02918     if( !oScript.IsOk() )
02919     {
02920         // Log an error message, ...
02921         wxLogError( wxString( wxT( "wxIScanFrame::StartScript -- " ) )
02922                       + _( "Cannot initialize wxIScanLuaScript object." ) );
02923 
02924         // ... and leave.
02925         return;
02926     }
02927 #endif // __WXLUASCRIPT_DYNAMIC__
02928 
02929     // ..., register menu id constants, ...
02930     oScript.RegisterConstant( wxT( "ID_FILESCANIMAGE" ), ID_FILESCANIMAGE );
02931     oScript.RegisterConstant( wxT( "ID_FILEOPEN" ), ID_FILEOPEN );
02932     oScript.RegisterConstant( wxT( "ID_FILESAVEAS" ), ID_FILESAVEAS );
02933     oScript.RegisterConstant( wxT( "ID_FILEPRINT" ), ID_FILEPRINT );
02934     oScript.RegisterConstant( wxT( "ID_FILEPRINTPREVIEW" ), ID_FILEPRINTPREVIEW );
02935     oScript.RegisterConstant( wxT( "ID_FILEPRINTSETUP" ), ID_FILEPRINTSETUP );
02936     oScript.RegisterConstant( wxT( "ID_FILEQUIT" ), ID_FILEQUIT );
02937     oScript.RegisterConstant( wxT( "ID_EDITCOPY" ), ID_EDITCOPY );
02938     oScript.RegisterConstant( wxT( "ID_EDITPASTE" ), ID_EDITPASTE );
02939     oScript.RegisterConstant( wxT( "ID_EDITSETTINGS" ), ID_EDITSETTINGS );
02940     oScript.RegisterConstant( wxT( "ID_EDITSETTINGSFILE" ), ID_EDITSETTINGSFILE );
02941     oScript.RegisterConstant( wxT( "ID_VIEWFITWIDTH" ), ID_VIEWFITWIDTH );
02942     oScript.RegisterConstant( wxT( "ID_VIEWFITWINDOW" ), ID_VIEWFITWINDOW );
02943     oScript.RegisterConstant( wxT( "ID_VIEWFULLSIZE" ), ID_VIEWFULLSIZE );
02944     oScript.RegisterConstant( wxT( "ID_VIEWCUSTOMSIZE" ), ID_VIEWCUSTOMSIZE );
02945     oScript.RegisterConstant( wxT( "ID_VIEWFULLSCREEN" ), ID_VIEWFULLSCREEN );
02946     oScript.RegisterConstant( wxT( "ID_DOCUMENTCOPYTEXT" ), ID_DOCUMENTCOPYTEXT );
02947     oScript.RegisterConstant( wxT( "ID_DOCUMENTREADTEXT" ), ID_DOCUMENTREADTEXT );
02948     oScript.RegisterConstant( wxT( "ID_DOCUMENTREADTEXT" ), ID_DOCUMENTREADTEXT );
02949     oScript.RegisterConstant( wxT( "ID_DOCUMENTREADTEXTSYNC" ), ID_DOCUMENTREADTEXTSYNC );
02950     oScript.RegisterConstant( wxT( "ID_DOCUMENTPREVIOUSPAGE" ), ID_DOCUMENTPREVIOUSPAGE );
02951     oScript.RegisterConstant( wxT( "ID_DOCUMENTNEXTPAGE" ), ID_DOCUMENTNEXTPAGE );
02952     oScript.RegisterConstant( wxT( "ID_DOCUMENTLASTPAGE" ), ID_DOCUMENTLASTPAGE );
02953     oScript.RegisterConstant( wxT( "ID_DOCUMENTCROP" ), ID_DOCUMENTCROP );
02954     oScript.RegisterConstant( wxT( "ID_DOCUMENTROTATE90LEFT" ), ID_DOCUMENTROTATE90LEFT );
02955     oScript.RegisterConstant( wxT( "ID_DOCUMENTROTATE90RIGHT" ), ID_DOCUMENTROTATE90RIGHT );
02956     oScript.RegisterConstant( wxT( "ID_DOCUMENTAPPLYPROFILE" ), ID_DOCUMENTAPPLYPROFILE );
02957 #if __WXPDFDOC__
02958     oScript.RegisterConstant( wxT( "ID_PDFBEGINPDF" ), ID_PDFBEGINPDF );
02959     oScript.RegisterConstant( wxT( "ID_PDFADDPAGE" ), ID_PDFADDPAGE );
02960     oScript.RegisterConstant( wxT( "ID_PDFSAVEASPDF" ), ID_PDFSAVEASPDF );
02961 #endif // __WXPDFDOC__
02962     oScript.RegisterConstant( wxT( "ID_HELPABOUT" ), ID_HELPABOUT );
02963     oScript.RegisterConstant( wxT( "ID_CMDSTOP" ), ID_CMDSTOP );
02964 
02965     // ..., register variables, ...
02966     oScript.RegisterVariable( wxT( "Resolution" ), &m_nResolution );
02967     oScript.RegisterVariable( wxT( "Page" ), &m_nPage );
02968     oScript.RegisterVariable( wxT( "ImageCount" ), &m_nImageCount );
02969     oScript.RegisterVariable( wxT( "PdfPages" ), &m_nPdfPages );
02970     oScript.RegisterVariable( wxT( "ProfileIndex" ), &m_nProfileIndex );
02971     oScript.RegisterVariable( wxT( "ImageFileName" ), &m_strImageFileName );
02972     oScript.RegisterVariable( wxT( "BitmapMimeType" ), &m_strBitmapMimeType );
02973     oScript.RegisterVariable( wxT( "ScaleFactor" ), &m_nScaleFactor );
02974     oScript.RegisterVariable( wxT( "ZoomFactor" ), &m_nZoomFactor );
02975     oScript.RegisterVariable( wxT( "AutoApplyProfile" ), &m_bAutoApplyProfile );
02976     oScript.RegisterVariable( wxT( "AutoApplyProfileOnMultiPage" ), &m_bAutoApplyProfileOnMultiPage );
02977     oScript.RegisterVariable( wxT( "OCR" ), &m_bOCR );
02978     oScript.RegisterVariable( wxT( "CmdConvertEnable" ), &m_bCmdConvertEnable );
02979     oScript.RegisterVariable( wxT( "CmdConvert" ), &m_strCmdConvert );
02980     oScript.RegisterVariable( wxT( "CmdOcrEnable" ), &m_bCmdOcrEnable );
02981     oScript.RegisterVariable( wxT( "CmdOcr" ), &m_strCmdOcr );
02982     oScript.RegisterVariable( wxT( "CmdCleanUpEnable" ), &m_bCmdCleanUpEnable );
02983     oScript.RegisterVariable( wxT( "CmdCleanUp" ), &m_strCmdCleanUp );
02984     oScript.RegisterVariable( wxT( "HocrEnable" ), &m_bHocrEnable );
02985     oScript.RegisterVariable( wxT( "HocrClass" ), &m_strHocrClass );
02986     oScript.RegisterVariable( wxT( "SmartHocrEnable" ), &m_bSmartHocrEnable );
02987     oScript.RegisterVariable( wxT( "CmdOcrUseHocr" ), &m_strCmdOcrUseHocr );
02988     oScript.RegisterVariable( wxT( "CmdHtml2XhtmlEnable" ), &m_bCmdHtml2XhtmlEnable );
02989     oScript.RegisterVariable( wxT( "CmdHtml2Xhtml" ), &m_strCmdHtml2Xhtml );
02990     oScript.RegisterVariable( wxT( "CmdHocrCleanUpEnable" ), &m_bCmdHocrCleanUpEnable );
02991     oScript.RegisterVariable( wxT( "CmdHocrCleanUp" ), &m_strCmdHocrCleanUp );
02992     oScript.RegisterVariable( wxT( "CmdTtsEnable" ), &m_bCmdTtsEnable );
02993     oScript.RegisterVariable( wxT( "CmdTts" ), &m_strCmdTts );
02994     oScript.RegisterVariable( wxT( "CmdEditEnable" ), &m_bCmdEditEnable );
02995     oScript.RegisterVariable( wxT( "CmdEdit" ), &m_strCmdEdit );
02996     oScript.RegisterVariable( wxT( "PdfDirty" ), &m_bPdfDirty );
02997 #if __WXPDFDOC__
02998     oScript.RegisterVariable( wxT( "PdfBitmapMimeType" ), &m_strPdfBitmapMimeType );
02999 #endif // __WXPDFDOC__
03000 
03001     // ... and run the script.
03002 #if __DEBUG__
03003     wxLogMessage( wxT( "wxIScanFrame::StartScript - " )
03004                     + wxString::Format( _( "Running Lua Script '%s' starting function '%s'." ),
03005                                         strScriptFileName.c_str(),
03006                                         strScriptStartupFunction.c_str() ) );
03007 #endif // __DEBUG__
03008     oScript.Run();
03009 }
03010 #endif // __WXLUASCRIPT__
03011 
03012 // Setting functions.
03013 //
03014 // Restore program settings from config file.
03015 //
03016 void wxIScanFrame::RestoreSettings()
03017 {
03018     // Reset window settings.
03019     wxConfigBase *poConfig = wxConfigBase::Get();
03020     poConfig->SetRecordDefaults();
03021     poConfig->SetPath( wxT( "/" )  );
03022     poConfig->SetPath( cstrwxFrame );
03023 
03024     int x= poConfig->Read( wxT( "x" ),   5 );
03025     int y= poConfig->Read( wxT( "y" ),   5 );
03026     int w= poConfig->Read( wxT( "w" ), 630 );
03027     int h= poConfig->Read( wxT( "h" ), 470 );
03028     bool bMaximized= (bool)poConfig->Read( wxT( "Maximized"  ), (long)false );
03029     bool bIconized= (bool)poConfig->Read( wxT( "Iconized"  ), (long)false );
03030     bool bFullScreen= (bool)poConfig->Read( wxT( "FullScreen"  ), (long)false );
03031 
03032     // Reset window position, size, maximization and full screen state.
03033     Move( x, y );
03034     SetSize( w, h );
03035     Maximize( bMaximized );
03036     Iconize( bIconized );
03037     if( bFullScreen )
03038     {
03039         ToggleFullScreen();
03040     }
03041 
03042     // Reset ...
03043     m_oPrintData.SetPaperId( (wxPaperSize)poConfig->Read( wxT( "PaperSize" ), (long)wxPAPER_A4 ) );
03044     m_nFit= poConfig->Read( wxT( "Fit" ), wxCANVASFITWINDOW );
03045     poConfig->Read( wxT( "ScaleFactor" ), &m_nScaleFactor );
03046     poConfig->Read( wxT( "ZoomFactor" ), &m_nZoomFactor );
03047 
03048     // Reset bitmap mimetype for everything but pdf export.
03049     m_strBitmapMimeType= poConfig->Read( wxT( "BitmapMimeType" ), wxT( "image/tiff" ) );
03050 
03051 #if __WXPDFDOC__
03052     // Reset bitmap mimetype for pdf export.
03053     m_strPdfBitmapMimeType= poConfig->Read( wxT( "PdfBitmapMimeType" ), wxT( "image/png" ) );
03054 #endif // __WXPDFDOC__
03055 
03056     // Get OCR and tts configuration.
03057     m_bAutoApplyProfile= (bool)poConfig->Read( wxT( "AutoApplyProfile" ), (long)false );
03058     m_bAutoApplyProfileOnMultiPage= (bool)poConfig->Read( wxT( "AutoApplyProfileOnMultiPage" ), (long)false );
03059     m_strOcrOutputExtension= poConfig->Read( wxT( "OcrOutputExtension" ), wxT( "txt" ) );
03060     m_bCmdConvertEnable= poConfig->Read( wxT( "CmdConvertEnable" ), (long)false );
03061     m_strCmdConvert= poConfig->Read( wxT( "CmdConvert" ), wxT( "convert %s -compress Group4 %s.tif" ) );
03062     m_bCmdOcrEnable= poConfig->Read( wxT( "CmdOcrEnable" ), (long)false );
03063     m_strCmdOcr= poConfig->Read( wxT( "CmdOcr" ), wxT( "cuneiform -f text -o %s.txt %s.tif" ) );
03064     m_bOCR= (bool)poConfig->Read( wxT( "OCR" ), (long)false );
03065 #if __WXLUASCRIPT__
03066     m_bLua= (bool)poConfig->Read( wxT( "Lua" ), (long)false );
03067 #endif // __WXLUASCRIPT__
03068     m_bCmdCleanUpEnable= poConfig->Read( wxT( "CmdCleanUpEnable" ), (long)false );
03069     m_strCmdCleanUp= poConfig->Read( wxT( "CmdCleanUp" ), wxT( "rm %s.tif" ) );
03070     m_bCmdTtsEnable= poConfig->Read( wxT( "CmdTtsEnable" ), (long)false );
03071     m_strCmdTts= poConfig->Read( wxT( "CmdTts" ), wxT( "espeak -f %s" ) );
03072     m_bCmdEditEnable= poConfig->Read( wxT( "CmdEditEnable" ), (long)false );
03073     m_strCmdEdit= poConfig->Read( wxT( "CmdEdit" ), wxT( "scite %s" ) );
03074     m_bHocrEnable= poConfig->Read( wxT( "HocrEnable" ), (long)false );
03075     m_strHocrClass= poConfig->Read( wxT( "HocrClass" ), wxT( "ocr_line" ) );
03076     m_bSmartHocrEnable= poConfig->Read( wxT( "SmartHocrEnable" ), (long)false );
03077     m_strCmdOcrUseHocr= poConfig->Read( wxT( "CmdOcrUseHocr" ), wxT( "cuneiform -f hocr -o %s.html %s.tif" ) );
03078     m_strOcrOutputExtensionHocr= poConfig->Read( wxT( "OcrOutputExtensionHocr" ), wxT( "html" ) );
03079     m_bCmdHtml2XhtmlEnable= poConfig->Read( wxT( "CmdHtml2XhtmlEnable" ), (long)false );
03080     m_strCmdHtml2Xhtml= poConfig->Read( wxT( "CmdHtml2Xhtml" ), wxT( "tidy -q -m -numeric -asxhtml -utf8 %s" ) );
03081     m_bCmdHocrCleanUpEnable= poConfig->Read( wxT( "CmdHocrCleanUpEnable" ), (long)false );
03082     m_strCmdHocrCleanUp= poConfig->Read( wxT( "CmdHocrCleanUp" ), wxT( "rm -r %s_files" ) );
03083 
03084     // Restore the standard resolution.
03085     m_nResolution= poConfig->Read( wxT( "Resolution" ), 300 );
03086 
03087     // Reset sash positions and splitting options.
03088     RestoreSashSettings();
03089 }
03090 
03091 // Restore sash settings from config file.
03092 //
03093 void wxIScanFrame::RestoreSashSettings()
03094 {
03095     // Reset window settings.
03096     wxConfigBase *poConfig = wxConfigBase::Get();
03097     poConfig->SetRecordDefaults();
03098     poConfig->SetPath( wxT( "/" )  );
03099     poConfig->SetPath( cstrwxFrame );
03100 
03101     // Reset sash positions and splitting options.
03102     int nHSashPos= poConfig->Read( wxT( "HSashPos" ), -1 );
03103     int nVSashPos= poConfig->Read( wxT( "VSashPos" ), -1 );
03104 
03105     m_poHSplitterWindow->SetSashPosition( nHSashPos );
03106     m_poVSplitterWindow->SetSashPosition( nVSashPos );
03107 }
03108 
03109 // Save program settings in config file.
03110 //
03111 void wxIScanFrame::SaveSettings()
03112 {
03113     // Save configuration data.
03114     wxConfigBase *poConfig= wxConfigBase::Get();
03115 
03116     poConfig->SetPath( wxT( "/" ) );
03117     poConfig->SetPath( cstrwxFrame );
03118 
03119     // Save iconifization state.
03120     poConfig->Write( wxT( "Iconized" ),  (long)IsIconized() );
03121     if( IsIconized() )
03122     {
03123         Iconize( false );
03124     }
03125 
03126     // Save maximization state.
03127     poConfig->Write( wxT( "Maximized" ),  (long)IsMaximized() );
03128     if( IsMaximized() )
03129     {
03130         Maximize( false );
03131         ::wxSafeYield();
03132         ::wxSafeYield();
03133     }
03134 
03135     // Save full screen state.
03136     poConfig->Write( wxT( "FullScreen" ),  (long)IsFullScreen() );
03137     if( IsFullScreen() )
03138     {
03139         ToggleFullScreen();
03140         ::wxSafeYield();
03141         ::wxSafeYield();
03142         ::wxSafeYield();
03143     }
03144 
03145     // Save window position and size.
03146     //
03147     int x, y, w, h;
03148 
03149     GetSize( &w, &h );
03150     GetPosition( &x, &y );
03151     poConfig->Write( wxT( "x" ), (long)x );
03152     poConfig->Write( wxT( "y" ), (long)y );
03153     poConfig->Write( wxT( "w" ), (long)w );
03154     poConfig->Write( wxT( "h" ), (long)h );
03155 
03156     // Save the sash positions.
03157     poConfig->Write( wxT( "HSashPos" ), (long)m_poHSplitterWindow->GetSashPosition() );
03158     poConfig->Write( wxT( "VSashPos" ), (long)m_poVSplitterWindow->GetSashPosition() );
03159 
03160     // Save the resolution as standard resolution.
03161     poConfig->Write( wxT( "Resolution" ), (long)m_nResolution );
03162 
03163     // ...
03164     poConfig->Write( wxT( "PaperSize" ), (long)m_oPrintData.GetPaperId() );
03165     poConfig->Write( wxT( "Fit" ), (long)m_nFit );
03166     poConfig->Write( wxT( "ScaleFactor" ), m_nScaleFactor );
03167     poConfig->Write( wxT( "ZoomFactor" ), m_nZoomFactor );
03168 
03169     // ...
03170     poConfig->Write( wxT( "BitmapMimeType" ), m_strBitmapMimeType );
03171 
03172 #if __WXPDFDOC__
03173     // ...
03174     poConfig->Write( wxT( "PdfBitmapMimeType" ), m_strPdfBitmapMimeType );
03175 #endif // __WXPDFDOC__
03176 
03177     // ...
03178     poConfig->Write( wxT( "AutoApplyProfile" ), (long)m_bAutoApplyProfile );
03179     poConfig->Write( wxT( "AutoApplyProfileOnMultiPage" ), (long)m_bAutoApplyProfileOnMultiPage );
03180     poConfig->Write( wxT( "OCR" ), (long)m_bOCR );
03181 #if __WXLUASCRIPT__
03182     poConfig->Write( wxT( "Lua" ), (long)m_bLua );
03183 #endif // __WXLUASCRIPT__
03184     poConfig->Write( wxT( "CmdConvertEnable" ), (long)m_bCmdConvertEnable );
03185     poConfig->Write( wxT( "CmdConvert" ), m_strCmdConvert );
03186     poConfig->Write( wxT( "CmdOcrEnable" ), (long)m_bCmdOcrEnable );
03187     poConfig->Write( wxT( "CmdOcr" ), m_strCmdOcr );
03188     poConfig->Write( wxT( "OcrOutputExtension" ), m_strOcrOutputExtension );
03189     poConfig->Write( wxT( "CmdCleanUpEnable" ), (long)m_bCmdCleanUpEnable );
03190     poConfig->Write( wxT( "CmdCleanUp" ), m_strCmdCleanUp );
03191     poConfig->Write( wxT( "CmdTtsEnable" ), (long)m_bCmdTtsEnable );
03192     poConfig->Write( wxT( "CmdTts" ), m_strCmdTts );
03193     poConfig->Write( wxT( "CmdEditEnable" ), (long)m_bCmdEditEnable );
03194     poConfig->Write( wxT( "CmdEdit" ), m_strCmdEdit );
03195     poConfig->Write( wxT( "HocrEnable" ), (long)m_bHocrEnable );
03196     poConfig->Write( wxT( "HocrClass" ), m_strHocrClass );
03197     poConfig->Write( wxT( "SmartHocrEnable" ), (long)m_bSmartHocrEnable );
03198     poConfig->Write( wxT( "CmdOcrUseHocr" ), m_strCmdOcrUseHocr );
03199     poConfig->Write( wxT( "OcrOutputExtensionHocr" ), m_strOcrOutputExtensionHocr );
03200     poConfig->Write( wxT( "CmdHtml2XhtmlEnable" ), (long)m_bCmdHtml2XhtmlEnable );
03201     poConfig->Write( wxT( "CmdHtml2Xhtml" ), m_strCmdHtml2Xhtml );
03202     poConfig->Write( wxT( "CmdHocrCleanUpEnable" ), (long)m_bCmdHocrCleanUpEnable );
03203     poConfig->Write( wxT( "CmdHocrCleanUp" ), m_strCmdHocrCleanUp );
03204 
03205     // ...
03206     poConfig->Write( wxT( "ProfileIndex" ), m_nProfileIndex );
03207 
03208     // Save file history to config file.
03209     poConfig->SetPath( wxT( "/Files" ) );
03210     m_oFileHistory.Save( *poConfig );
03211 }
03212 
03213 // Printing support.
03214 //
03215 // Paints the image while printing or print previewing.
03216 //
03217 // NOTE: The painting on the screen is done by void wxIViewCanvas::Paint( wxDC& oDc )!
03218 //
03219 bool wxIScanFrame::Paint( wxDC& oDc )
03220 {
03221     // If there is a bitmap connected to this canvas window (re)paint it.
03222 #if wxCHECK_VERSION( 2, 9, 0 )
03223     if( oDc.IsOk() && m_oPrintImage.Ok() )
03224 #else
03225     if( oDc.Ok() && m_oPrintImage.Ok() )
03226 #endif // wxCHECK_VERSION( 2, 9, 0 )
03227 
03228     {
03229         // Repaint the bitmap.
03230         oDc.DrawBitmap( wxBitmap( m_oPrintImage ), 0, 0 );
03231         return true;
03232     }
03233     else
03234     {
03235         return false;
03236     }
03237 }
03238 
03239 // Calculates the image extents in mm while printing or print previewing.
03240 //
03241 bool wxIScanFrame::GetPageSizeMM( int *pnWidth, int *pnHeight )
03242 {
03243      if( m_oPrintImage.Ok() )
03244      {
03245          (*pnWidth)=  (int)( (double)m_oPrintImage.GetWidth()  / (double)m_nResolution * 25.4 );
03246          (*pnHeight)= (int)( (double)m_oPrintImage.GetHeight() / (double)m_nResolution * 25.4 );
03247          return true;
03248      }
03249      else
03250      {
03251          return false;
03252      }
03253 }
03254 
03255 // Go to a given printer page.
03256 //
03257 bool wxIScanFrame::GotoPage( int nPage )
03258 {
03259     // NOTE: The wxPrintOut object passes pages one-based. We need it
03260     //        zero-based. Therefore we decrement the page number by 1.
03261     //
03262     nPage--;
03263 
03264     // Avoid loading an already loaded image.
03265     if( nPage == m_nPage )
03266     {
03267         m_oPrintImage= m_oImage;
03268         return true;
03269     }
03270     else
03271     {
03272         return m_oPrintImage.LoadFile( m_strImageFileName, wxBITMAP_TYPE_ANY, nPage );
03273     }
03274 }
