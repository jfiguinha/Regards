00001 /***************************************************************
00002  * Name:      wxextprocess.h
00003  * Purpose:
00004  * Author:    Daniel Nell (daniel.nell@nellresearch.de)
00005  * Created:   2009-05-20
00006  * Copyright: Daniel Nell (www.nellresearch.de)
00007  * License:   wxWindows licence
00008  **************************************************************/
00009 
00010 #ifndef WXEXTPROCESS_H
00011 #define WXEXTPROCESS_H
00012 
00013 // Include additional wxWidgets headers.
00014 #include <wx/process.h>
00015 
00016 
00017 //////////////////////////////////////////////////////////
00018 // Class wxExtProcess
00019 //
00020 /// \brief An extended wxProcess based class.
00021 ///
00022 /// It extends wxProcess in a way that it is possible to
00023 /// kill an external process by saving the process id so
00024 /// you do not have to know it when you want to kill the
00025 /// process. Just call its member function Kill().
00026 class wxExtProcess : public wxProcess
00027 {
00028   public:
00029     /// \brief Standard constructor copied from base class.
00030     ///
00031     /// \param poParent         a pointer to the event handler parent
00032     /// \param nId              id of the wxEVT_END_PROCESS notification event
00033     wxExtProcess( wxEvtHandler * poParent, int nId= -1 )
00034       : wxProcess( poParent, nId ), m_nProcessId( 0 )
00035     {
00036     }
00037 
00038     /// Virtual destructor.
00039     virtual ~wxExtProcess()
00040     {
00041     }
00042 
00043     /// \brief Execute the external process and save the process id.
00044     ///
00045     /// \param strCommand       command to execute
00046     /// \param nSync            flag to indicate wether to execute the
00047     ///                         process synchronously or asynchronously
00048     virtual long Execute( const wxString& strCommand, int nSync= wxEXEC_ASYNC )
00049     {
00050         m_nProcessId= ::wxExecute( strCommand, nSync, this );
00051         return m_nProcessId;
00052     }
00053 
00054     /// \brief Set the process id of the external process.
00055     ///
00056     /// \param nProcessId   process id of the external process
00057     virtual void SetProcessId( long nProcessId )
00058     {
00059         m_nProcessId= nProcessId;
00060     }
00061 
00062     /// \brief Get the process id of the external process.
00063     virtual long GetProcessId()
00064     {
00065         return m_nProcessId;
00066     }
00067 
00068     /// \brief "Kill" a running process.
00069     ///
00070     /// \param nSignal      signal (see description of wxProcess)
00071     /// \param nFlags       flags (see description of wxProcess)
00072     virtual wxKillError Kill( wxSignal nSignal= wxSIGNONE, int nFlags= wxKILL_NOCHILDREN )
00073     {
00074         return wxProcess::Kill( GetProcessId(), nSignal, nFlags );
00075     }
00076 
00077   protected:
00078     // Member variables
00079     long m_nProcessId;      ///< Holds the process id of the process.
00080 };
00081 
00082 
00083 //////////////////////////////////////////////////////////
00084 // Class wxExtProcessDelTempFile
00085 //
00086 /// \brief A slightly extended wxExtProcess class that
00087 ///        removes a temporary file in its destructor.
00088 ///
00089 /// This class is usefull if you want to be sure that a possibly
00090 /// existing temporary file should be removed after process
00091 /// termination even it the process was killed by Kill().
00092 class wxExtProcessDelTempFile : public wxExtProcess
00093 {
00094   public:
00095     /// \brief Standard constructor.
00096     ///
00097     /// \param poParent         a pointer to the event handler parent
00098     /// \param nId              id of the wxEVT_END_PROCESS notification event
00099     /// \param strTempFileName  the file name to remove after termination
00100     wxExtProcessDelTempFile( wxEvtHandler *poParent= NULL, int nId = -1,
00101                              const wxString &strTempFileName= wxEmptyString )
00102       : wxExtProcess( poParent, nId )
00103     {
00104         m_strTempFileName= strTempFileName;
00105     }
00106 
00107     /// Virtual destructor. Removes the temporary file.
00108     virtual ~wxExtProcessDelTempFile()
00109     {
00110         if( !m_strTempFileName.IsEmpty() && ::wxFileExists( m_strTempFileName ) )
00111         {
00112             // Remove the temporary file.
00113             ::wxRemoveFile( m_strTempFileName );
00114         }
00115     }
00116 
00117   protected:
00118     wxString m_strTempFileName; ///< Holds the name of the temporary file to be removed in the destructor.
00119 };
00120 
00121 #endif // WXEXTPROCESS_H

