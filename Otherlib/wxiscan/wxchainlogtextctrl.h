00001 /////////////////////////////////////////////////////////////////////////////
00002 // Project:     wxSQuery
00003 // Purpose:     SQL query tool for the SQLite3 library
00004 // Name:        wxchainlogtextctrl.h
00005 // Author:      Daniel Nell
00006 // Created:     2008/09/26
00007 // Copyright:   (c) Daniel Nell
00008 // Licence:     wxWindows licence
00009 // Modified by:
00010 /////////////////////////////////////////////////////////////////////////////
00011 
00012 #ifndef __WXCHAINLOGTEXTCTRL_H
00013 #define __WXCHAINLOGTEXTCTRL_H
00014 
00015 /////////////////////////////////////////////////////////////////////////////
00016 //  Class wxChainLogTextCtrl
00017 //
00018 /// \brief A class to split the log output into a wxLogGui and a wxTextCtrl target.
00019 ///
00020 /// This is a simple to use class in order to replace the current log target by
00021 /// twice
00022 ///     a) a GUI window log with threshold, and
00023 ///     b) a text control to log long lists of log output.
00024 class wxChainLogTextCtrl : public wxLogGui
00025 {
00026   public:
00027     /// \brief Standard constructor.
00028     ///
00029     /// \param poTextctrl    A pointer to a wxTextCtrl object, that is used as an addition
00030     /// \param nMaxLevel     Refence log level for to suppress messages with higher level,
00031     ///                      defaults to 2 (errors)
00032     wxChainLogTextCtrl( wxTextCtrl *poTextctrl, wxLogLevel nMaxLevel= 2 )
00033     {
00034         // Save log level.
00035         m_nMaxLevel= nMaxLevel;
00036 
00037         // Set this object as the new log target.
00038         wxLog::SetActiveTarget( this );
00039 
00040         // Chain the log to the wxTextCtrl object.
00041         new wxLogChain( new wxLogTextCtrl( poTextctrl ) );
00042     }
00043 
00044 
00045   protected:
00046 #if wxCHECK_VERSION( 2, 9, 0 )
00047     /// \brief Internally called function to process the log message.
00048     ///
00049     /// Suppress the log output if the given log level is higher then m_nMaxLevel.
00050     /// So it is possible to get only error messages shown in the normal dialog
00051     /// window while anything else is logged to the chained wxTextCtrl object.
00052     ///
00053     /// \param oLevel        Log level
00054     /// \param strMessage    Message to log
00055     /// \param oLogRecInfo   Information about a log record (unit of the log output).
00056     virtual void DoLogRecord( wxLogLevel nLevel,
00057                               const wxString& strMessage,
00058                               const wxLogRecordInfo& oLogRecInfo )
00059     {
00060         // Show message only if the log level is lower or equal to the reference value.
00061         if( nLevel <= m_nMaxLevel )
00062         {
00063             // Show message using parent function.
00064             wxLogGui::DoLogRecord( nLevel, strMessage, oLogRecInfo );
00065         }
00066     }
00067 #else
00068     /// \brief Internally called function to process the log message.
00069     ///
00070     /// Suppress the log output if the given log level is higher then m_nMaxLevel.
00071     /// So it is possible to get only error messages shown in the normal dialog
00072     /// window while anything else is logged to the chained wxTextCtrl object.
00073     ///
00074     /// \param oLevel        Log level
00075     /// \param pszMessage    Message to log
00076     /// \param nTimestamp    Timestamp of the message
00077     virtual void DoLog( wxLogLevel nLevel, const wxChar *pszMessage, time_t nTimestamp )
00078     {
00079         // Show message only if the log level is lower or equal to the reference value.
00080         if( nLevel <= m_nMaxLevel )
00081         {
00082             // Show message using parent function.
00083             //
00084             // Note: BASECLASS::DoLog() is protected. Therefore we cannot remember
00085             //       a pointer to the old log target and send the message but we have
00086             //       to subclass from wxLogGui and use its DoLog() function.
00087             wxLogGui::DoLog( nLevel, pszMessage, nTimestamp );
00088         }
00089     }
00090 #endif // wxCHECK_VERSION
00091 
00092   protected:
00093     // Member variable(s).
00094     wxLogLevel m_nMaxLevel;     /// Log level to propagate messages.
00095 };
00096 
00097 
00098 #endif // __WXCHAINLOGTEXTCTRL_H

