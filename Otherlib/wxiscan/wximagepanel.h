00001 /***************************************************************
00002  * Name:      wximagepanel.h
00003  * Purpose:   Side panel for PDF document priviewing.
00004  * Author:    Daniel Nell (daniel.nell@nellresearch.de)
00005  * Created:   2011-01-26
00006  * Copyright: Daniel Nell (www.nellresearch.de)
00007  * License:   GPL
00008  **************************************************************/
00009 
00010 #ifndef WXIMAGEPANEL_H
00011 #define WXIMAGEPANEL_H
00012 
00013 // Private headers
00014 #include "wxextthread.h"
00015 
00016 
00017 //////////////////////////////////////////////////////////
00018 // Hash map wxImageHash (for images)
00019 //
00020 WX_DECLARE_HASH_MAP( int, wxImage, wxIntegerHash, wxIntegerEqual, wxImageHash );
00021 
00022 
00023 //////////////////////////////////////////////////////////
00024 // Class wxImagePanel
00025 //
00026 /// \brief A panel that can display a list of images
00027 ///        (e. g. pages) and scroll.
00028 ///
00029 /// ...
00030 class wxImagePanel : public wxScrolledWindow
00031 {
00032     public:
00033         /// \brief Standard constructor
00034         ///
00035         /// \param poParent     a pointer to the dialog windows's parent window.
00036         /// \param nId          the window id, default: 1
00037         /// \param oPos         the window position, default: wxDefaultPosition
00038         /// \param oSize        the window size, default: wxDefaultSize
00039         /// \param nStyle       window style (default: wxHSCROLL | wxVSCROLL | wxSUNKEN_BORDER)
00040         /// \param strName      class name (default: wxT( "wxIViewCanvas" ))
00041         wxImagePanel( wxWindow *poParent,
00042                        wxWindowID nId= wxID_ANY,
00043                        const wxPoint &oPos= wxDefaultPosition,
00044                        const wxSize &oSize= wxDefaultSize,
00045                        long nStyle= wxHSCROLL | wxVSCROLL | wxSUNKEN_BORDER,
00046                        const wxString& strName= wxT( "wxImagePanel" )
00047                      );
00048 
00049         /// Virtual destructor
00050         virtual ~wxImagePanel();
00051 
00052         /// \brief Empty the panel.
00053         virtual void Reset()
00054         {
00055             DestroyChildren();
00056             SetScrollbars( 1, 1, 1, 1 );
00057             m_nPanelPagesCount= 0;
00058         }
00059 
00060         /// \brief Add images to the panel.
00061         ///
00062         /// \param oImage       an image to convert to a thumbnail
00063         ///
00064         /// Note: I moved all GUI functions from AddImage() to OnThreadExit().
00065         ///       It should therefore be possible to call AddImage() from
00066         ///       another (non-GUI-)thread.
00067         ///
00068         virtual void AddImage( wxImage oImage );
00069 
00070         /// \brief Create a thumbnail from the corresponding image in another
00071         ///        thread.
00072         ///
00073         /// \param nId          ID of the to image (in the hash map) to rescale.
00074         ///
00075         virtual void OnThreadEntry( int nId );
00076 
00077     private:
00078         // Event handlers
00079         //
00080         /// \brief Handle the exit of another thread.
00081         ///
00082         /// \param oEvent       Event containing the ID of the corresponding
00083         ///                     image.
00084         ///
00085         /// Delete the corresponding image and adjust the scroll bar(s).
00086         ///
00087         void OnThreadExit( wxCommandEvent& oEvent );
00088 
00089     protected:
00090          // Member variables
00091         int m_nPanelPagesCount;             ///< The number of the pages in the pages panel.
00092         wxImageHash m_oImageHash;           ///< A hash map to get the image corresponding to event ID in OnThreadEntry().
00093 
00094         // Options
00095         int m_nPanelPagesBitmapSize;        ///< Side ot the square that the image should fit to after resizing in pixels.
00096         int m_nPanelPagesBitmapSpace;       ///< Space to surround the thumb nail image (padding).
00097 
00098     private:
00099         // Event table.
00100         DECLARE_EVENT_TABLE()
00101 };
00102 
00103 
00104 //////////////////////////////////////////////////////////
00105 //  class wxImagePanelThread
00106 //
00107 /// \brief A second thread to create the thumb nail for wxImagePanel.
00108 ///
00109 /// ...
00110 class wxImagePanelThread : public wxExtThread
00111 {
00112     public:
00113         /// \brief Standard constructor
00114         ///
00115         /// \param poEvtHandler             A pointer to the event handler that owns the thread.
00116         /// \param nEventInt                The ID of the corresponding image (in the hash map).
00117         ///
00118         wxImagePanelThread( wxEvtHandler *poEvtHandler, int nEventInt )
00119           : wxExtThread( poEvtHandler, nEventInt )
00120         {
00121         }
00122 
00123         /// \brief Thread execution starts here.
00124         virtual ExitCode Entry()
00125         {
00126             // Run the preview image creation.
00127             ( (wxImagePanel *)m_poEvtHandler )->OnThreadEntry( m_nEventInt );
00128 
00129             // Return with exit code 0 (success).
00130             return 0;
00131         }
00132 };
00133 
00134 #endif // WXISCANMAIN_H
00135 

