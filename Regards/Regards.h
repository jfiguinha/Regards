/////////////////////////////////////////////////////////////////////////////
// Name:        minimal.cpp
// Purpose:     Minimal wxWidgets sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
// ReSharper disable All
#pragma once
#include <header.h>
#include "wx/xrc/xmlres.h"
#include "wx/url.h"
#include <PrintEngine.h>
#include <MainInterface.h>
#include <RegardsConfigParam.h>
#include <MyFrameIntro.h>
#include <ViewerFrame.h>
#include "wx/sysopt.h"
#include <wx/cmdline.h>
#include <wx/display.h>
#include <VideoConverterFrame.h>
#include <ScannerFrame.h>
#include "TestFrame.h"
#include <ncnn/gpu.h>
#include <signal.h>

//#define TEST_WINDOWMANAGER
#ifdef __WXGTK__
 #include <X11/Xlib.h>   
#endif

#ifdef TEST_WINDOWMANAGER
#include <myFrame.h>
#endif
#ifdef USECURL
#include <curl/curl.h>
#endif


#ifdef SDL2
#include <SDL2/SDL.h>
#endif

#ifdef __WXMSW__
//#include <gdiplus.h>
// pour terminer, voici les lignes à inclure dans votre code source :

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define ISOLATION_AWARE_ENABLED
#endif

#if not defined(WIN32) && defined(LIBBPG)
#include <dlfcn.h>
#endif

#include <wx/glcanvas.h>

int args[] = {
	wx_GL_COMPAT_PROFILE,
	WX_GL_SAMPLES, 4,
	WX_GL_RGBA,
	WX_GL_DOUBLEBUFFER,
	WX_GL_DEPTH_SIZE, 16,
	0
};

using namespace std;
using namespace Regards::Print;
using namespace Regards::Introduction;
using namespace Regards::Viewer;


[[noreturn]] void onTerminate() noexcept
{
	if (auto exc = std::current_exception())
	{
		// we have an exception
		try
		{
			rethrow_exception(exc); // throw to recognize the type
		}
		catch (const std::runtime_error& err)
		{
			std::cout << "caught a runtime_error. what(): " <<
				err.what() << std::endl;
		}
		catch (const std::exception& exception)
		{
			std::cout << "caught an exception. what(): " <<
				exception.what() << std::endl;
		}
		catch (...)
		{
			std::cout << "unknown exception occured." << std::endl;
		}
	}

	std::_Exit(EXIT_FAILURE);
}




// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp, public IMainInterface, public IVideoConverterInterface
{
public:
	// override base class virtuals
	// ----------------------------
	MyApp(
	)
	{

		logNo = new wxLogNull();
		//Init x11
		regardsParam = nullptr;
		frameStart = nullptr;
		//frameViewer = nullptr;
#ifdef USECURL
		curl_global_init(CURL_GLOBAL_ALL);
#endif

#ifdef __WXGTK__
		int result = XInitThreads();
#endif

		int flags = SDL_INIT_AUDIO | SDL_INIT_TIMER;
		//------SDL------------------------
		//³õÊ¼»¯
		if (SDL_Init(flags))
		{
			std::cerr << "unable to init SDL: " << SDL_GetError() << '\n';
			wxMessageBox(_T("Could not initialize SDL Audio"));
			//exit(1);
		}
	}



	void OnInitCmdLine(wxCmdLineParser& parser) override;
	bool OnCmdLineParsed(wxCmdLineParser& parser) override;

	// this one is called on application startup and is a good place for the app
	// initialization (doing it here and not in the ctor allows to have an error
	// return: if OnInit() returns false, the application terminates)
	bool OnInit() override;
	int Close() override;
#ifdef __APPLE__
	virtual void MacOpenFile(const wxString &fileName);
#endif


	void ShowViewer() override
	{
#ifdef TEST_WINDOWMANAGER
		wxFrame* frame = new MyFrame(NULL);
		SetTopWindow(frame);
		frame->Show();
#else

		wxDisplay display;
		wxRect screen = display.GetClientArea();

		frameViewer = new CViewerFrame("Regards Viewer", wxDefaultPosition,
		                               wxSize(screen.GetWidth(), screen.GetHeight()), this, fileToOpen);
		frameViewer->Centre(wxBOTH);
		frameViewer->Show(true);
#endif
	}

	void ShowAbout() override
	{
		if (frameStart != nullptr)
		{
			frameStart->Show(true);
			frameStart->Raise();
		}
        else
        {
            frameStart = new MyFrameIntro("Welcome to Regards", "REGARDS V2", wxPoint(50, 50), wxSize(450, 340), this);
            frameStart->Centre(wxBOTH);
            frameStart->Show(true);
        }
	}

	void HideAbout() override
	{
		if (frameStart != nullptr)
			frameStart->Show(false);
	}

	wxString GetAppName() override
	{
		return "RegardsViewer";
	}

	virtual void AddImageHandler(wxImageHandler* poHandler);
	wxString GetImageFilter() override;
	virtual void OnEventLoopEnter(wxEventLoopBase* WXUNUSED(loop)) override;

private:

	void CheckGeolocalisationServiceAvailability();
	void CheckOpenCLAvailability(bool configFileExist);

	CRegardsConfigParam* regardsParam= nullptr;

	MyFrameIntro* frameStart= nullptr;
	CViewerFrame* frameViewer= nullptr;
	CScannerFrame* framePDF = nullptr;
	CVideoConverterFrame* frameVideoConverter = nullptr;
	CTestFrame* testFrame = nullptr;

	wxString fileToOpen;
	wxString appName = "";
	wxString m_strImageFilterList;
	wxString m_strImageFilter;
	wxLogNull* logNo;

};

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

static const wxCmdLineEntryDesc g_cmdLineDesc[] =
{

	{
		wxCMD_LINE_PARAM, nullptr, nullptr, "input file", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL
	},
	{ wxCMD_LINE_OPTION, "p", "program", "Program Name" },
	{wxCMD_LINE_NONE}
};
