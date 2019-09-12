00001 /***************************************************************
00002  * Name:      wxiscanframe.h
00003  * Purpose:   Code for application frame (the main) window
00004  * Author:    Daniel Nell (daniel.nell@nellresearch.de)
00005  * Created:   2008-05-04
00006  * Copyright: Daniel Nell (www.nellresearch.de)
00007  * License:   GPL
00008  **************************************************************/
00009 
00010 #ifndef WXISCANFRAME_H
00011 #define WXISCANFRAME_H
00012 
00013 //(*Headers(wxIScanFrame)
00014 #include <wx/toolbar.h>
00015 #include <wx/menu.h>
00016 #include <wx/scrolwin.h>
00017 #include <wx/splitter.h>
00018 #include <wx/statusbr.h>
00019 #include <wx/frame.h>
00020 #include <wx/textctrl.h>
00021 //*)
00022 
00023 // Additional wxWidgets header files
00024 #include <wx/docview.h>
00025 #include <wx/process.h>
00026 
00027 // Private header files.
00028 #include "wxextfiledroptarget.h"
00029 #include "wximagepanel.h"
00030 #include "wxiviewcanvas.h"
00031 #include "wxiviewpaint.h"
00032 #include "wxscan.h"
00033 #if __WXPDFDOC__
00034 #   include "wxiscanhocr2pdf.h"
00035 #endif // __WXPDFDOC__
00036 
00037 /////////////////////////////////////////////////////////////////////////////
00038 //  Constants
00039 //
00040 #define wxISCAN_SCRIPTMENUPOS           5
00041 
00042 /////////////////////////////////////////////////////////////////////////////
00043 // IDs
00044 //
00045 #define wxISCAN_PROCESS                 1
00046 
00047 
00048 //////////////////////////////////////////////////////////
00049 // Predefined classes
00050 //
00051 class wxExtProcess;
00052 
00053 
00054 //////////////////////////////////////////////////////////
00055 // Class wxIScanFrame
00056 //
00057 /// \brief The frame window class.
00058 ///
00059 /// wxIScanFrame represents the scanning application's main
00060 /// window and handles most of the events (e. g. menu, timer
00061 /// etc.).
00062 ///
00063 /// Note:
00064 ///         In addition to wxFrame wxIScanFrame inherits the
00065 ///         class wxIViewPaintBase which is an interface to
00066 ///         basic printing and print previewing of an image.
00067 class wxIScanFrame: public wxFrame, public wxIViewPaintBase
00068 {
00069     public:
00070         enum
00071         {
00072             wxCANVASFULLSIZE,                                       ///< Show image full size (without any zooming).
00073             wxCANVASFITWIDTH,                                       ///< Zoom image to fit the window width.
00074             wxCANVASFITWINDOW,                                      ///< Zoom image to fit the window size.
00075             wxCANVASCUSTOMSIZE                                      ///< Zoom image to an explicit given zoom factor (m_nScaleFactor).
00076         };
00077 
00078     public:
00079         /// \brief Standard constructor.
00080         ///
00081         /// \param poParent                  a pointer to the parent window
00082         /// \param nId                       the window id
00083         wxIScanFrame( wxWindow* poParent, wxWindowID nId= wxID_ANY );
00084 
00085         /// \brief Virtual destructor.
00086         virtual ~wxIScanFrame();
00087 
00088         virtual bool CanClose( bool bCanVeto= true );               ///< Handle window closing requests.
00089         virtual bool OpenImage( const wxString &strFileName, bool bSetFileName= true );  ///< Open an image using the given name.
00090         virtual bool OnDocumentPageChanged();                       ///< (Re)loads the (zero based) page number given in m_nPage.
00091         virtual void UpdateBitmap();                                ///< Update the canvas bitmap.
00092         virtual wxImage GetImageSelection();                        ///< Get the selected sub image.
00093         virtual bool ScanImage();                                   ///< Get an image from scanner.
00094 #ifdef __WXSCANSANE__
00095         virtual void ScanForScanners( bool bAsync= false );         ///< Scan for existing SANE scanners an fill the object in the tool bar.
00096         virtual void ScanForScannersThreadEntry();                  ///< Create a new scanner object and scan for scanning devices.
00097         virtual void ScanForScannersThreadExit();                   ///< Fill the wxChoice object for scanners with a list of available scanning devices.
00098 #endif // __WXSCANSANE__
00099         virtual void SetProfile( int nProfileIndex );               ///< Select a profile (by number).
00100         virtual void ApplyProfile();                                ///< Apply the selected profile to the image.
00101 
00102         /// \brief Enhance the optical contrast by replacing white by yellow.
00103         virtual void EnhenceOpticalContrast()
00104         {
00105             m_oImage.Replace( 255, 255, 255, 255, 255, 0 );
00106             UpdateBitmap();
00107         }
00108 
00109         /// \brief Call an external OCR software and return the
00110         ///        filename of the resulting text file.
00111         ///
00112         /// \param strTempFileName           the file name of the image that should be read by OCR().
00113         /// \param bUseHocr                  flag if hOCR (as html) output should be done, defaults to false.
00114         virtual wxString OCR( const wxString &strTempFileName, bool bUseHocr= false );
00115 
00116         /// \brief Do OCR and read out the returned text.
00117         ///
00118         /// \param bAsync                    start the read out asynchronously, defaults to true.
00119         virtual bool ReadOut( bool bAsync= true );
00120 
00121         /// \brief Fork an external process.
00122         ///
00123         /// \param strCommand                the command line to be executed.
00124         /// \param bAsync                    an (optional) flag which decides if the new process
00125         ///                                  is to be executed synchronous (default) or asynchronous.
00126         /// \param strTempTextFileName       an (optional) file name of a file that should be deleted
00127         ///                                  at the end of an asynchronous running process.
00128         virtual bool Execute( const wxString &strCommand, bool bAsync= false,
00129                               const wxString &strTempTextFileName= wxEmptyString );
00130 
00131 #if __WXPDFDOC__
00132         /// \brief Create (and initialize) a new PDF document.
00133         virtual bool CreateNewPdfDocument();
00134 
00135         /// \brief Add a new image/OCR text page to the existing PDF document.
00136         virtual bool AddPdfPage();
00137 
00138         /// \brief Check if the PDF dirty flag is set to TRUE.
00139         virtual bool IsPdfDirty()
00140         {
00141             return m_bPdfDirty;
00142         }
00143 #endif // __WXPDFDOC__
00144 
00145         /// \brief State if the image is in portrait direction.
00146         virtual bool IsPortrait()
00147         {
00148             return ( m_oImage.GetHeight() > m_oImage.GetWidth() );
00149         }
00150 
00151         /// \brief Reset the filename to "unknown.jpg" etc.
00152         virtual void ResetFileName();
00153 
00154         /// \brief Write a sub processes stdout and stderr output to the program's log.
00155         virtual void LogProcessOutput( wxProcess *poProcess );
00156 
00157         /// \brief Update the page number info in the statusbar and in the toolbar.
00158         virtual void UpdatePageNumber();
00159 
00160         /// \brief Toggle full screen mode.
00161         virtual void ToggleFullScreen()
00162         {
00163             ShowFullScreen( !IsFullScreen() );
00164         }
00165 
00166         /// \brief Restore program settings from config file.
00167         virtual void RestoreSettings();
00168 
00169         /// \brief Restore sash settings from config file.
00170         virtual void RestoreSashSettings();
00171 
00172         /// \brief Save program settings in config file.
00173         virtual void SaveSettings();
00174 
00175         /// \brief Enable or disable controls according to the actual progam state.
00176         virtual void EnableControls();
00177 
00178         /// \brief Locks/Unlocks the user interface.
00179         ///
00180         /// \param bLock                    if true locks otherwise unlocks the user interface.
00181         virtual void LockUI( bool bLock= true )
00182         {
00183             m_bUILocked= bLock;
00184             EnableControls();
00185         }
00186 
00187         /// \brief Unlocks the user interface.
00188         virtual void UnlockUI()
00189         {
00190             LockUI( false );
00191         }
00192 
00193 #if __WXLUASCRIPT__
00194         /// \brief Start a Lua script from a give file.
00195         ///
00196         /// \param strScriptStartupFunction  startup function of the Lua script
00197         /// \param strScriptFileName         file name of the text file containing the Lua script
00198         ///                                  (defaults to the empty string which means, that no
00199         ///                                  script is to be loaded before running the start up
00200         ///                                  function).
00201         virtual void StartScript( const wxString &strScriptStartupFunction,
00202                                   wxString strScriptFileName= wxEmptyString );
00203 #endif // __WXLUASCRIPT__
00204 
00205 #if __WXPDFDOC__
00206     private:
00207         // Private helper functions.
00208         void AddPdfPageTraverseXmlNodes( wxXmlNode *poNode );       ///< DOM tree traverse helper function for AddPdfPage().
00209 #endif // __WXPDFDOC__
00210 
00211     public:
00212         // Printer handling (introduced by base class wxIViewPaintBase)
00213         virtual void AdjustOrientation(){ m_oPrintData.SetOrientation( IsPortrait() ? wxPORTRAIT : wxLANDSCAPE ); } ///< ...
00214         virtual bool GetPageSizeMM( int *pnWidth, int *pnHeight );  ///< Calculates the image extents in mm while printing or print previewing.
00215         virtual int  GetMaxPage(){ return m_nImageCount; }          ///< Get the (zero based) page number of the last page.
00216         virtual bool GotoPage( int nPage );                         ///< Go to a given printer page.
00217         virtual bool Paint( wxDC& oDc );                            ///< Paints the image while printing or print previewing.
00218 
00219     private:
00220         // Member variables
00221         bool m_bUILocked;                                           ///< States if the GUI should be locked by wxIScanFrame::EnableControls().
00222         wxImage m_oImage;                                           ///< Image object.
00223         wxImage m_oPrintImage;                                      ///< Image object for printing.
00224         wxString m_strImageFileName;                                ///< Image file name.
00225         int m_nPage;                                                ///< Page number.
00226         int m_nImageCount;                                          ///< The number of images in an image file.
00227         int m_nResolution;                                          ///< The (fictive) resolution of an image in dpi.
00228         wxScan *m_poScanner;                                        ///< Object representing the scanner.
00229 #ifdef __WXSCANSANE__
00230         wxCriticalSection m_oCriticalSectionScanner;                ///< critical section to protect m_poScanner.
00231 #endif // __WXSCANSANE__
00232 
00233         // Options
00234         int m_nFit;                                                 ///< Fit bitmap size according to its value.
00235         double m_nScaleFactor;                                      ///< Bitmap scalation factor.
00236         double m_nZoomFactor;                                       ///< Zoom factor by which the bitmap should be scaled.
00237         bool m_bAutoApplyProfile;                                   ///< Apply a profile automatically after scanning.
00238         bool m_bAutoApplyProfileOnMultiPage;                        ///< Apply a profile automatically after loading a page from a multi page file.
00239         int m_nProfileIndex;                                        ///< Index number of the current image profile.
00240         wxString m_strBitmapMimeType;                               ///< Image mimetype for any export but PDF export.
00241 #if __WXPDFDOC__
00242         wxString m_strPdfBitmapMimeType;                            ///< Image mimetype for PDF export.
00243 #endif // __WXPDFDOC__
00244 #if __WXLUASCRIPT__
00245         bool m_bLua;                                                ///< Enable Lua scripting.
00246         int  m_nLuaCount;                                           ///< Number of Lua scripts.
00247 #endif // __WXLUASCRIPT__
00248         bool m_bOCR;                                                ///< Enable OCR.
00249         wxString m_strOcrOutputExtension;                           ///< The output extension of the via OCR extracted text file.
00250         wxString m_strOcrOutputExtensionHocr;                       ///< The output extension of the via OCR extracted hocr (html) file.
00251         bool m_bCmdConvertEnable;                                   ///< ...
00252         wxString m_strCmdConvert;                                   ///< ...
00253         bool m_bCmdOcrEnable;                                       ///< ...
00254         wxString m_strCmdOcr;                                       ///< ...
00255         bool m_bCmdCleanUpEnable;                                   ///< ...
00256         wxString m_strCmdCleanUp;                                   ///< ...
00257         bool m_bHocrEnable;                                         ///< ...
00258         wxString m_strHocrClass;                                    ///< ...
00259         bool m_bSmartHocrEnable;                                    ///< ...
00260         wxString m_strCmdOcrUseHocr;                                ///< ...
00261         bool m_bCmdHtml2XhtmlEnable;                                ///< ...
00262         wxString m_strCmdHtml2Xhtml;                                ///< ...
00263         bool m_bCmdHocrCleanUpEnable;                               ///< ...
00264         wxString m_strCmdHocrCleanUp;                               ///< ...
00265         bool m_bCmdTtsEnable;                                       ///< ...
00266         wxString m_strCmdTts;                                       ///< ...
00267         bool m_bCmdEditEnable;                                      ///< ...
00268         wxString m_strCmdEdit;                                      ///< ...
00269 
00270         // File history
00271         wxFileHistory m_oFileHistory;                               ///< History of last opened files
00272 
00273         // Printing support.
00274         wxPrintData m_oPrintData;                                   ///< Holds related to printers and printer device contexts.
00275 
00276 #if __WXPDFDOC__
00277         // PDF support
00278         wxPdfDocument *m_poPdfDoc;                                  ///< PDF document.
00279         int m_nPdfPages;                                            ///< Number of PDF pages in memory.
00280         bool m_bPdfDirty;                                           ///< States if the PDF file is dirty (and should be saved).
00281 #endif // __WXPDFDOC__
00282 
00283 #if __WXLUASCRIPT__
00284         bool bLuaReturnFromEventHandler;                            ///< Lua return value of the event handlers.
00285 #endif // __WXLUASCRIPT__
00286 
00287         // Managed process.
00288         wxExtProcess *m_poRunningProcess;                           ///< A concurrent running process (equals to NULL if there isn't any).
00289 
00290     private:
00291         //(*Handlers(wxIScanFrame)
00292         void OnFileQuit(wxCommandEvent& event);
00293         void OnFileScanImage(wxCommandEvent& event);
00294         void OnFileOpen(wxCommandEvent& event);
00295         void OnFileSaveAs(wxCommandEvent& event);
00296         void OnFilePrint( wxCommandEvent &oEvent );
00297         void OnFilePrintPreview( wxCommandEvent &oEvent );
00298         void OnFilePrintSetup( wxCommandEvent &oEvent );
00299         void OnEditCopy(wxCommandEvent& event);
00300         void OnPdfBeginPdf(wxCommandEvent& event);
00301         void OnDocumentCopyText(wxCommandEvent& event);
00302         void OnDocumentReadText(wxCommandEvent& event);
00303         void OnDocumentFirstPage(wxCommandEvent& event);
00304         void OnDocumentPreviousPage(wxCommandEvent& event);
00305         void OnDocumentNextPage(wxCommandEvent& event);
00306         void OnDocumentLastPage(wxCommandEvent& event);
00307         void OnDocumentRotate90Left(wxCommandEvent& event);
00308         void OnDocumentRotate90Right(wxCommandEvent& event);
00309         void OnDocumentApplyProfile(wxCommandEvent& event);
00310         void OnPdfAddPage(wxCommandEvent& event);
00311         void OnPdfSavePdf(wxCommandEvent& event);
00312         void OnHelpAbout(wxCommandEvent& event);
00313         void OnCmdStop(wxCommandEvent& event);
00314         void OnClose(wxCloseEvent& event);
00315         void OnEditPaste(wxCommandEvent& event);
00316         void OnEditSettings(wxCommandEvent& event);
00317         void OnEditSettingsFile(wxCommandEvent& event);
00318         void OnDocumentCrop(wxCommandEvent& event);
00319         void OnEditClearHistory(wxCommandEvent& event);
00320         void OnEditClearLog(wxCommandEvent& event);
00321         void OnDocumentZoom(wxCommandEvent& event);
00322         void OnViewFit(wxCommandEvent& event);
00323         void OnViewFullScreen(wxCommandEvent& event);
00324         void OnEditScanScanners(wxCommandEvent& event);
00325         void OnViewCloseDebugWindow(wxCommandEvent& event);
00326         void OnViewClosePreviewPanel(wxCommandEvent& event);
00327         //*)
00328         void OnDocumentProfilesChooseProfile( wxCommandEvent& oEvent ); ///< Handle menu command "Document|Profile|[PROFILE #X]".
00329 #if __WXLUASCRIPT__
00330         void OnScriptStartScript( wxCommandEvent& oEvent );         ///< Handle menu command "Scripts|[SCRIPT NAME]".
00331 #endif // __WXLUASCRIPT__
00332         void OnMRUFiles( wxCommandEvent& oEvent );                  ///< Handle menu command "File|[MOST RECENTLY USED FILE #X]".
00333         void OnFileDrop( wxExtFileDropEvent& oEvent );              ///< Handle file dropping.
00334         void OnCanvasSize( wxIViewCanvasSizeEvent& oEvent );        ///< Handle bitmap resizing introduced by canvas resizing.
00335         void OnCanvasSelected( wxIViewCanvasSelectedEvent& oEvent ); ///< Handle canvas selection events.
00336         void OnProcessTerminated( wxProcessEvent& oEvent );         ///< Handle process termination.
00337         void OnTextCtrlPageProcessEnter( wxCommandEvent& oEvent );  ///< Handle page number editor field events.
00338 
00339 #ifdef __WXSCANSANE__
00340         /// \brief Handle the exit of another thread.
00341         ///
00342         /// \param oEvent       ... (not yet used at all).
00343         void OnThreadExit( wxCommandEvent& oEvent )
00344         {
00345             ScanForScannersThreadExit();
00346         }
00347 #endif // __WXSCANSANE__
00348 
00349         //(*Identifiers(wxIScanFrame)
00350         static const long ID_CANVAS;
00351         static const long ID_PAGES;
00352         static const long ID_VSPLITTERWINDOW;
00353         static const long ID_LOGTEXTCTRL;
00354         static const long ID_HSPLITTERWINDOW;
00355         static const long ID_FILESCANIMAGE;
00356         static const long ID_FILEOPEN;
00357         static const long ID_FILESAVEAS;
00358         static const long ID_FILEPRINT;
00359         static const long ID_FILEPRINTPREVIEW;
00360         static const long ID_FILEPRINTSETUP;
00361         static const long ID_FILEQUIT;
00362         static const long ID_EDITCOPY;
00363         static const long ID_EDITPASTE;
00364         static const long ID_EDITSETTINGS;
00365         static const long ID_EDITSETTINGSFILE;
00366         static const long ID_EDITSCANSCANNERS;
00367         static const long ID_EDITCLEARLOG;
00368         static const long ID_EDITCLEARHISTORY;
00369         static const long ID_VIEWFULLSCREEN;
00370         static const long ID_VIEWFITWIDTH;
00371         static const long ID_VIEWFITWINDOW;
00372         static const long ID_VIEWFULLSIZE;
00373         static const long ID_VIEWCUSTOMSIZE;
00374         static const long ID_VIEWZOOMIN;
00375         static const long ID_VIEWZOOMOUT;
00376         static const long ID_VIEWZOOMSELECTION;
00377         static const long ID_VIEWCLOSEDEBUGWINDOW;
00378         static const long ID_VIEWCLOSEPREVIEWPANEL;
00379         static const long ID_DOCUMENTCOPYTEXT;
00380         static const long ID_DOCUMENTREADTEXT;
00381         static const long ID_CMDSTOP;
00382         static const long ID_DOCUMENTFIRSTPAGE;
00383         static const long ID_DOCUMENTPREVIOUSPAGE;
00384         static const long ID_DOCUMENTNEXTPAGE;
00385         static const long ID_DOCUMENTLASTPAGE;
00386         static const long ID_DOCUMENTROTATE90LEFT;
00387         static const long ID_DOCUMENTROTATE90RIGHT;
00388         static const long ID_DOCUMENTCROP;
00389         static const long ID_DOCUMENTAPPLYPROFILE;
00390         static const long ID_MENUDOCUMENTPROFILES;
00391         static const long ID_PDFBEGINPDF;
00392         static const long ID_PDFADDPAGE;
00393         static const long ID_PDFSAVEASPDF;
00394         static const long ID_HELPABOUT;
00395         static const long ID_FILESCANIMAGE_TB;
00396         static const long ID_FILEOPEN_TB;
00397         static const long ID_FILESAVEAS_TB;
00398         static const long ID_FILEPRINT_TB;
00399         static const long ID_PDFBEGINPDF_TB;
00400         static const long ID_PDFADDPAGE_TB;
00401         static const long ID_PDFSAVEASPDF_TB;
00402         static const long ID_DOCUMENTFIRSTPAGE_TB;
00403         static const long ID_DOCUMENTPREVIOUSPAGE_TB;
00404         static const long ID_DOCUMENTNEXTPAGE_TB;
00405         static const long ID_DOCUMENTLASTPAGE_TB;
00406         static const long ID_DOCUMENTREADTEXT_TB;
00407         static const long ID_CMDSTOP_TB;
00408         //*)
00409         static const long ID_DOCUMENTREADTEXTSYNC;                  ///< ...
00410         static const long ID_DOCUMENTPROFILE0;                      ///< ...
00411         static const long ID_TEXTCTRLPAGE;                          ///< ...
00412 #ifdef __WXSCANSANE__
00413         static const long ID_CHOICESCANNER;                         ///< ...
00414 #endif // __WXSCANSANE__
00415 
00416         //(*Declarations(wxIScanFrame)
00417         wxIViewCanvas* m_poCanvas;
00418         wxToolBarToolBase* ToolBarItem5;
00419         wxToolBarToolBase* ToolBarItem11;
00420         wxSplitterWindow* m_poVSplitterWindow;
00421         wxToolBarToolBase* ToolBarItem6;
00422         wxImagePanel* m_poPanelPages;
00423         wxToolBarToolBase* ToolBarItem12;
00424         wxToolBarToolBase* ToolBarItem7;
00425         wxToolBarToolBase* ToolBarItem2;
00426         wxToolBarToolBase* ToolBarItem13;
00427         wxToolBarToolBase* ToolBarItem9;
00428         wxToolBarToolBase* ToolBarItem10;
00429         wxToolBarToolBase* ToolBarItem4;
00430         wxMenu* poMenuEdit;
00431         wxToolBarToolBase* ToolBarItem1;
00432         wxToolBarToolBase* ToolBarItem3;
00433         wxSplitterWindow* m_poHSplitterWindow;
00434         wxTextCtrl* m_poLogTextCtrl;
00435         wxToolBarToolBase* ToolBarItem8;
00436         //*)
00437         wxTextCtrl *m_poTextCtrlPage;                               ///< A pointer to the page number editor
00438         wxTextValidator m_oTextValidatorTextCtrlPage;               ///< The validator to the page number editor.
00439 #ifdef __WXSCANSANE__
00440         wxChoice *m_poChoiceScanner;                                ///< A pointer to the scanner choice dropdown box.
00441 #endif // __WXSCANSANE__
00442 
00443         // Friend classes.
00444 #if __WXLUASCRIPT__
00445         friend class wxIScanLuaScript;
00446 #endif // __WXLUASCRIPT__
00447 
00448         // Event table.
00449         DECLARE_EVENT_TABLE()
00450 };
00451 
00452 
00453 #ifdef __WXSCANSANE__
00454 //////////////////////////////////////////////////////////
00455 //  class wxIScanImageThread
00456 //
00457 /// \brief ...
00458 ///
00459 /// ...
00460 class wxIScanFrameScanForScannersThread : public wxExtThread
00461 {
00462   public:
00463     /// \brief Standard constructor
00464     ///
00465     /// \param poEvtHandler             A pointer to the event handler that owns the thread.
00466     /// \param nEventInt                The ID of the corresponding image (in the hash map).
00467     wxIScanFrameScanForScannersThread( wxEvtHandler *poEvtHandler )
00468       : wxExtThread( poEvtHandler )
00469     {
00470     }
00471 
00472     /// \brief Thread execution starts here.
00473     virtual ExitCode Entry()
00474     {
00475         // Run the preview image creation.
00476         ( (wxIScanFrame *)m_poEvtHandler )->ScanForScannersThreadEntry();
00477 
00478         // Return with exit code 0 (success).
00479         return 0;
00480     }
00481 };
00482 #endif // __WXSCANSANE__
00483 
00484 #endif // WXISCANFRAME_H

