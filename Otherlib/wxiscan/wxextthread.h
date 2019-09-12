00001 /***************************************************************
00002  * Name:      wxextthread.h
00003  * Purpose:   Extended thread class.
00004  * Author:    Daniel Nell (daniel.nell@nellresearch.de)
00005  * Created:   2011-02-08
00006  * Copyright: Daniel Nell (www.nellresearch.de)
00007  * License:   GPL
00008  **************************************************************/
00009 
00010 #ifndef WXEXTTHREAD_H
00011 #define WXEXTTHREAD_H
00012 
00013 //////////////////////////////////////////////////////////
00014 //  class wxExtThread
00015 //
00016 /// \brief Extended thread class.
00017 ///
00018 /// wxExtThread extends the wxThread class by some methods that send messages
00019 /// to the owning wxEventHandler object in order to signal thread entry and
00020 /// exit.
00021 //
00022 class wxExtThread : public wxThread
00023 {
00024   public:
00025     /// \brief Standard constructor
00026     ///
00027     /// \param poEvtHandler             A pointer to the event handler that owns the thread.
00028     /// \param nEventInt                A possible ID to post to the event handler.
00029     wxExtThread( wxEvtHandler *poEvtHandler, int nEventInt= 0 )
00030       : wxThread()
00031     {
00032         m_poEvtHandler= poEvtHandler;
00033         m_nEventInt= nEventInt;
00034     }
00035 
00036     /// Virtual destructor.
00037     virtual ~wxExtThread()
00038     {
00039     }
00040 
00041     /// \brief Thread execution starts here.
00042     virtual ExitCode Entry()
00043     {
00044         // Send the entry event.
00045         AddPendingEvent( ID_ENTRY );
00046 
00047         // Return with exit code 0 (success).
00048         return 0;
00049     }
00050 
00051     /// \brief Thread termination.
00052     ///
00053     /// Note: This handler function ist called when the thread exits whether it
00054     ///       terminates normally or is stopped by a call to Delete() (but NOT
00055     ///       when it is KILLed!).
00056     virtual void OnExit()
00057     {
00058         // Send the exit event.
00059         AddPendingEvent( ID_EXIT );
00060     }
00061 
00062     /// \brief Helper function to send events to the owning event handler.
00063     ///
00064     /// \param nEventId                 multipurpose interface event (e. g. an identifier for
00065     ///                                 the thread)
00066     virtual void AddPendingEvent( const long nEventId )
00067     {
00068         wxCommandEvent oEvent( wxEVT_COMMAND_MENU_SELECTED, nEventId );
00069 
00070         oEvent.SetInt( m_nEventInt );
00071         m_poEvtHandler->AddPendingEvent( oEvent );
00072     }
00073 
00074   protected:
00075     // Member varables
00076     wxEvtHandler *m_poEvtHandler;   ///< A pointer to the event handler that owns this thread.
00077     int m_nEventInt;                ///< An event value (e. g. for identifying the thread).
00078 
00079   public:
00080     // Command IDs
00081     enum
00082     {
00083         ID_ENTRY= 65312,
00084         ID_EXIT
00085     };
00086 };
00087 
00088 #endif // WXEXTTHREAD_H

