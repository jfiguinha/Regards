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
#include <header.h>
#include "wx/xrc/xmlres.h"
#include "wx/url.h"
#include "SqlInit.h"
#include <PrintEngine.h>
#include <MainInterface.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <LibResource.h>
#include <MyFrameIntro.h>
#include <ViewerFrame.h>
#include "wx/stdpaths.h"
#include "wx/sysopt.h"
#include <wx/cmdline.h>
#include <SqlResource.h>
#include <libPicture.h>
#include <ConvertUtility.h>
#include <wx/textfile.h>
//#define TEST_WINDOWMANAGER

#ifdef TEST_WINDOWMANAGER
#include <myFrame.h>
#endif
#ifdef USECURL
#include <curl/curl.h>
#endif
#include <OpenCLEngine.h>
using namespace Regards::OpenCL;

#ifdef SDL2
#include <SDL.h>
#include <SDL_audio.h> 
#endif

#ifdef __WXMSW__
#include <gdiplus.h>
// pour terminer, voici les lignes à inclure dans votre code source :
#define WIN32_LEAN_AND_MEAN
#define ISOLATION_AWARE_ENABLED
#endif

#if not defined(WIN32) && defined(LIBBPG)
#include <dlfcn.h>
#endif

#ifdef FFMPEG
#define CONFIG_AVFILTER 1

extern "C"
{
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
#include "libavcodec/avfft.h"
#include "libswresample/swresample.h"

#if CONFIG_AVFILTER
#include "libavfilter/avfilter.h"
#endif
}

#define YUV       0
#define YCBCR     1
#define OPP       2
#define RGB       3
#define DCT       4
#define BIOR      5
#define HADAMARD  6
#define NONE      7


static int ff_lockmgr(void ** _mutex, enum AVLockOp op)
{
	if (NULL == _mutex)
		return -1;

	switch (op)
	{
	case AV_LOCK_CREATE:
	{
		*_mutex = NULL;
		mutex * m = new mutex();
		*_mutex = static_cast<void*>(m);
		break;
	}
	case AV_LOCK_OBTAIN:
	{
		mutex * m = static_cast<mutex*>(*_mutex);
		m->lock();
		break;
	}
	case AV_LOCK_RELEASE:
	{
		mutex * m = static_cast<mutex*>(*_mutex);
		m->unlock();
		break;
	}
	case AV_LOCK_DESTROY:
	{
		mutex * m = static_cast<mutex*>(*_mutex);
		delete m;
		break;
	}
	default:
		break;
	}
	return 0;
}

#endif

using namespace std;
using namespace Regards::Print;
using namespace Regards::Introduction;
using namespace Regards::Viewer;

#ifdef RENDEROPENGL

#include <GL/glew.h>
#if defined(_WIN32)
#include <GL/wglew.h>
#elif defined(_WIN64)
#include <GL/wglew.h>
#elif defined(__APPLE__) && !defined(GLEW_APPLE_GLX)
#include <AGL/agl.h>
#else
#include <GL/glxew.h>
#endif

#include <wx/glcanvas.h>
int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };
#else

COpenCLEngine * openCLEngine = nullptr;
COpenCLContext * openclContext = nullptr;

#endif

// c: pointer to original argc
// v: pointer to original argv
// o: option name after hyphen
// d: default value (if NULL, the option takes no argument)
const char *pick_option(int *c, char **v, const char *o, const char *d) {
	int id = d ? 1 : 0;
	for (int i = 0; i < *c - id; i++) {
		if (v[i][0] == '-' && 0 == strcmp(v[i] + 1, o)) {
			char *r = v[i + id] + 1 - id;
			for (int j = i; j < *c - id; j++)
				v[j] = v[j + id + 1];
			*c -= id + 1;
			return r;
		}
	}
	return d;
}

bool CMasterWindow::endProgram = false;
vector<CMasterWindow *> CMasterWindow::listMainWindow;
float value[256];

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp, public IMainInterface
{
public:
	// override base class virtuals
	// ----------------------------
	MyApp(
	)
	{

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

#pragma omp parallel for
		for (auto i = 0; i < 256; i++)
			value[i] = (float)i;

#ifdef FFMPEG
		//Init FFMPEG
		int res = -1;
		res = av_lockmgr_register(&ff_lockmgr);
		// register all codecs, demux and protocols 
		avcodec_register_all();
#if CONFIG_AVDEVICE
		avdevice_register_all();
#endif
#if CONFIG_AVFILTER 
		avfilter_register_all();
#endif
		av_register_all();
		//Èç¹ûÃ»ÓÐ±¾¾ä»á³ö´íÎó£º
		//Warning:Using network protocols without global network initialization.
		//²âÊÔÍøÂçÊý¾ÝµÄÊ±ºòÖ±½ÓÊ¹ÓÃrtsp://169.254.197.35:8554/sh1943.mpg¸ñÊ½µÄ²ÎÊý¾ÍÐÐ
		//»òÊÇÊ¹ÓÃ rtmp://localhost/vod/sample.flv¶¼¿ÉÒÔ
		//ÒòÎª´Ë°æ±¾ffmpeg¼¯³ÉÁËlibrtmp
		//ºÍÎÄ¼þºÜÀàËÆ
		avformat_network_init();
#endif



#ifdef SDL2

		//putenv("SDL_AUDIO_CHANNELS=2");
		int flags = SDL_INIT_AUDIO | SDL_INIT_TIMER;
		//------SDL------------------------
		//³õÊ¼»¯
		if (SDL_Init(flags)) {
			std::cerr << "unable to init SDL: " << SDL_GetError() << '\n';
			wxMessageBox(_T("Could not initialize SDL "));
			exit(1);
		}

#endif


	}

	virtual void OnInitCmdLine(wxCmdLineParser& parser);
	virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

	// this one is called on application startup and is a good place for the app
	// initialization (doing it here and not in the ctor allows to have an error
	// return: if OnInit() returns false, the application terminates)
	virtual bool OnInit();
	virtual int Close();
#ifdef __APPLE__	
	virtual void MacOpenFile(const wxString &fileName);
#endif    


	virtual void ShowViewer()
	{
#ifdef TEST_WINDOWMANAGER
		wxFrame* frame = new MyFrame(NULL);
		SetTopWindow(frame);
		frame->Show();
#else
		frameViewer = new CViewerFrame("Regards Viewer", wxDefaultPosition, wxDefaultSize, this, fileToOpen);
		frameViewer->Centre(wxBOTH);
		frameViewer->Show(true);
#endif
	}

	virtual void ShowAbout()
	{

		if (frameStart != nullptr)
		{
			frameStart->Show(true);
			frameStart->Raise();
		}

	}

	virtual void HideAbout()
	{

		if (frameStart != nullptr)
			frameStart->Show(false);

	}

	virtual wxString GetAppName()
	{
		return "RegardsViewer";
	}
	virtual void AddImageHandler(wxImageHandler* poHandler);
	virtual wxString GetImageFilter();

private:
	CRegardsConfigParam * regardsParam;
	MyFrameIntro * frameStart;
	CViewerFrame * frameViewer;
	wxString fileToOpen;
	wxString m_strImageFilterList;
	wxString m_strImageFilter;

#ifdef __WXMSW__
	ULONG_PTR m_gdiplusToken;   // class member
#endif
};

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

static const wxCmdLineEntryDesc g_cmdLineDesc[] =
{
	{ wxCMD_LINE_PARAM, NULL, NULL, "input file", wxCMD_LINE_VAL_STRING,
	wxCMD_LINE_PARAM_OPTIONAL },

	{ wxCMD_LINE_NONE }
};
