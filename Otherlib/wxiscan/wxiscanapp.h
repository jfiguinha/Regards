00001 /***************************************************************
00002  * Name:      wxiscanapp.h
00003  * Purpose:   Defines Application Class
00004  * Author:    Daniel Nell (daniel.nell@nellresearch.de)
00005  * Created:   2008-05-04
00006  * Copyright: Daniel Nell (www.nellresearch.de)
00007  * License:   GPL
00008  **************************************************************/
00009 
00010 #ifndef WXISCANAPP_H
00011 #define WXISCANAPP_H
00012 
00013 #include <wx/app.h>
00014 
00015 //////////////////////////////////////////////////////////
00016 // Predefined classes
00017 //
00018 class wxIScanFrame;
00019 
00020 
00021 //////////////////////////////////////////////////////////
00022 // Class wxIScanFrame
00023 //
00024 /// \brief The application object.
00025 ///
00026 /// wxIScanFrame initializes
00027 /// - the command line interface and executes the according functions,
00028 /// - a file name filter from the corresponding image handlers,
00029 /// - and the main window (an instance of wxIscanFrame class).
00030 //
00031 class wxIScanApp : public wxApp
00032 {
00033   public:
00034     /// Program initialization.
00035     virtual bool OnInit();
00036 
00037     /// Initialize command line parameter handling.
00038     virtual void OnInitCmdLine( wxCmdLineParser& oParser );
00039 
00040     /// Handle filenames on the command line.
00041     virtual bool OnCmdLineParsed( wxCmdLineParser& oParser );
00042 
00043     /// Get the file extension filter for all registered image handlers.
00044     virtual wxString GetImageFilter();
00045 
00046   private:
00047     /// \brief Adds image handlers to the image handler list.
00048     ///
00049     /// \param poHandler    a pointer to the image handler that
00050     ///                     shall be added.
00051     virtual void AddImageHandler( wxImageHandler* poHandler );
00052 
00053   private:
00054     wxLocale m_oLocale;                 ///< Locale the application is using.
00055     wxString m_strImageFilter;          ///< Current image file filter.
00056     wxString m_strImageFilterList;      ///< Image file filter list.
00057     wxIScanFrame* m_poFrame;            ///< A pointer to the application's main window.
00058 };
00059 
00060 DECLARE_APP( wxIScanApp )
00061 
00062 #endif // WXISCANAPP_H

