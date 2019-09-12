00001 /////////////////////////////////////////////////////////////////////////////
00002 // Project:     wxIView
00003 // Purpose:     Complex wxWidgets sample
00004 // Name:        wxextfiledroptarget.h
00005 // Author:      Daniel Nell
00006 // Created:     2006/02/16
00007 // Copyright:   (c) Daniel Nell
00008 // Licence:     wxWindows licence
00009 // Modified by:
00010 /////////////////////////////////////////////////////////////////////////////
00011 
00012 #ifndef __WXEXTFILEDROPTARGET_H
00013 #define __WXEXTFILEDROPTARGET_H
00014 
00015 // Include additional wxWidgets headers.
00016 #include <wx/dnd.h>
00017 
00018 
00019 /////////////////////////////////////////////////////////////////////////////
00020 //  Class wxExtFileDropEvent
00021 //
00022 BEGIN_DECLARE_EVENT_TYPES()
00023     DECLARE_LOCAL_EVENT_TYPE( wxEVT_EXTFILEDROP, 0 )
00024 END_DECLARE_EVENT_TYPES()
00025 
00026 /// \brief Generic file drop event.
00027 ///
00028 /// This class extends wxEvent to transport a file name to the event handler
00029 /// that proecesses the file drop event. File drop events are created by
00030 /// wxExtFileDropTarget objects.
00031 class wxExtFileDropEvent : public wxEvent
00032 {
00033   public:
00034     /// Constructor
00035     wxExtFileDropEvent( wxEventType oType= wxEVT_EXTFILEDROP )
00036       : wxEvent( 0, oType )
00037     {
00038         m_strFileName.Clear();
00039     }
00040 
00041     /// Copy constructor.
00042     wxExtFileDropEvent( const wxExtFileDropEvent& oEvent )
00043       : wxEvent( oEvent )
00044     {
00045         SetFileName( oEvent.m_strFileName );
00046     }
00047 
00048     /// Return a copy of this event.
00049     virtual wxEvent *Clone() const
00050     {
00051         return new wxExtFileDropEvent( *this );
00052     }
00053 
00054     /// \brief Set the file name.
00055     ///
00056     /// \param strFileName      File name to add to the event.
00057     void SetFileName( wxString strFileName )
00058     {
00059         m_strFileName= strFileName;
00060     }
00061 
00062     /// Get the filename.
00063     wxString GetFileName()
00064     {
00065         return m_strFileName;
00066     }
00067 
00068   private:
00069     wxString m_strFileName;         ///< Contains the filename of the event.
00070 };
00071 
00072 typedef void (wxEvtHandler::*wxExtFileDropEventEventFunction)( wxExtFileDropEvent& );
00073 
00074 #define wxExtFileDropEventEventHandler(func) \
00075     (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent( wxExtFileDropEventEventFunction, &func )
00076 
00077 #define EVT_EXTFILEDROP(func) \
00078    wx__DECLARE_EVT0( wxEVT_EXTFILEDROP, wxExtFileDropEventEventHandler( func ) )
00079 
00080 
00081 /////////////////////////////////////////////////////////////////////////////
00082 //  Class wxExtFileDropTarget
00083 //
00084 /// \brief The file drop handler (file drop target).
00085 ///
00086 /// This is a generic helper class for file dropping. Use
00087 ///     SetDropTarget( new wxExtFileDropTarget( this ) );
00088 /// and install a handler for EVT_EXTFILEDROP( XXX::OnFileDrop ) in a class XXX
00089 /// derived from wxWindow (or at least wxEvtHandler) to use this class.
00090 class wxExtFileDropTarget : public wxFileDropTarget
00091 {
00092   public:
00093     /// \brief Standard constructor
00094     ///
00095     /// \param poOwner          a pointer to the owner event handler.
00096     wxExtFileDropTarget( wxEvtHandler *poOwner )
00097     {
00098         m_poOwner= poOwner;
00099     }
00100 
00101     /// \brief Handler for file dropping.
00102     ///
00103     /// \param x                Unused.
00104     /// \param y                Unused.
00105     /// \param astrFilenames    string array of the dropped file names..
00106     virtual bool OnDropFiles( wxCoord x, wxCoord y, const wxArrayString& astrFilenames );
00107 
00108   private:
00109     // Member variables
00110     wxEvtHandler *m_poOwner;            ///< Internal pointer to the owner event handler.
00111 };
00112 
00113 #endif // __WXEXTFILEDROPTARGET_H

