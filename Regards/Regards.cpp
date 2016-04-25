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

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

//#include <MyFrame.h>
#include "wx/xrc/xmlres.h"
#include "wx/url.h"
#include "SqlInit.h"
#include <PrintEngine.h>
#include <MainInterface.h>
#include <OpenCLEngine.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <LibResource.h>
#include <MyFrameIntro.h>
#include <ViewerFrame.h>
#include <ExplorerFrame.h>
#include "wx/stdpaths.h"
#include "wx/sysopt.h"
#include "wx/dcsvg.h"
#include <wx/cmdline.h>
#include <thread>


#ifdef __WXGTK__
#include <SDL.h>
#include <SDL_audio.h> 
#include "config.h"

extern "C"
{
#include "libavutil/avstring.h"
#include "libavutil/colorspace.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/imgutils.h"
#include "libavutil/dict.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/avassert.h"
#include "libavutil/time.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
#include "libavcodec/avfft.h"
#include "libswresample/swresample.h"
}

#if CONFIG_AVFILTER
# include "libavfilter/avcodec.h"
# include "libavfilter/avfilter.h"
# include "libavfilter/avfiltergraph.h"
# include "libavfilter/buffersink.h"
# include "libavfilter/buffersrc.h"
#endif

#endif

using namespace std;
//#include <dlfcn.h>
//#include <bpg.h>
using namespace std;
using namespace Regards::OpenCL;
using namespace Regards::Print;
using namespace Regards::Introduction;
using namespace Regards::Viewer;
using namespace Regards::Explorer;




#ifdef __APPLE__
#else
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

#ifdef GLEW_MX
GLEWContext _glewctx;
#  define glewGetContext() (&_glewctx)
#  ifdef _WIN32
WGLEWContext _wglewctx;
#    define wglewGetContext() (&_wglewctx)
#elif defined(_WIN64)
WGLEWContext _wglewctx;
#    define wglewGetContext() (&_wglewctx)
#  elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX)
GLXEWContext _glxewctx;
#    define glxewGetContext() (&_glxewctx)
#  endif
#endif /* GLEW_MX */
#endif


#ifdef __WXGTK__
// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------
void mixaudio(void *unused, Uint8 *stream, int len) { }
void ReportEnv(const char *var) {
	const char *driver = getenv(var);
	if (NULL == driver)
		std::cerr << var << " is null" << std::endl;
	else
		std::cerr << var << ": " << driver << std::endl;
}
#endif

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

#ifdef __WXGTK__
		int result = XInitThreads();



		//Init FFMPEG

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

		int flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER;
		//------SDL------------------------
		//³õÊ¼»¯
		if (SDL_Init(flags)) {
			std::cerr << "unable to init SDL: " << SDL_GetError() << '\n';
			wxMessageBox(_T("Could not initialize SDL "));
			exit(1);
		}


		const char* driver_name = SDL_GetCurrentAudioDriver();

		if (driver_name)
		{
			printf("Audio subsystem initialized; driver = %s.\n", driver_name);
			if (SDL_AudioInit(driver_name))
			{
				printf("[SDL] Audio driver: %s\n", SDL_GetCurrentAudioDriver());
			}

			SDL_AudioSpec fmt;
			fmt.freq = 44100;
			fmt.format = AUDIO_S16;
			fmt.channels = 2;
			fmt.samples = 512;        // A good value for games 
			fmt.callback = mixaudio;
			fmt.userdata = NULL;

			if (SDL_OpenAudio(&fmt, NULL) < 0)
			{
				fprintf(stderr, "Couldn't open audio device: %s\n", SDL_GetError());
				SetDummyDriver();
			}

		}
		else
		{
			SetDummyDriver();
		}
#endif
	}

	virtual void OnInitCmdLine(wxCmdLineParser& parser);
	virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

	// this one is called on application startup and is a good place for the app
	// initialization (doing it here and not in the ctor allows to have an error
	// return: if OnInit() returns false, the application terminates)
	virtual bool OnInit();
	virtual int OnRun();
	virtual int Close();
#ifdef __APPLE__	
	virtual void MacOpenFile(const wxString &fileName);
#endif    
	virtual void ShowViewer()
	{

		if (frameViewer != nullptr)
		{
			if (!frameViewer->IsShown())
			{
				frameViewer->Show(true);
				frameViewer->SetDatabase();
			}
		}
		else
		{
			frameViewer = new CViewerFrame("Regards Viewer", wxPoint(50, 50), wxSize(1200, 800), this);
			frameViewer->Show(true);
		}

	}

	virtual void UpdateViewer()
	{

		if (frameViewer != nullptr)
		{
			if (frameViewer->IsShown())
				frameViewer->SetDatabase();
		}

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

	virtual void HideViewer()
	{
		if (CViewerFrame::GetViewerMode())
			this->Close();
		else
		{
			if (frameViewer != nullptr)
				frameViewer->Show(false);
			frameExplorer->Raise();
		}


	}

	virtual void ShowExplorer()
	{

		if (frameExplorer != nullptr)
			frameExplorer->Show(true);

	}

	virtual void HideExplorer()
	{

		if (frameExplorer != nullptr)
			frameExplorer->Show(false);

	}

#ifdef __WXGTK__
	virtual void SetDummyDriver();
#endif

private:
	CRegardsConfigParam * regardsParam = nullptr;
	MyFrameIntro * frameStart = nullptr;
	CViewerFrame * frameViewer = nullptr;
	CExplorerFrame * frameExplorer = nullptr;
	wxString fileToOpen;

};

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

#ifdef __WXGTK__

void MyApp::SetDummyDriver()
{
	wxString audioDriver = "dummy";
	if (SDL_AudioInit(audioDriver))
		exit(1);

	printf("[SDL] Audio driver: %s\n", SDL_GetCurrentAudioDriver());
}

#endif

int MyApp::OnRun() {
	// initialize SDL



	// start the main loop
	return wxApp::OnRun();
}

static const wxCmdLineEntryDesc g_cmdLineDesc[] =
{
	{ wxCMD_LINE_PARAM, NULL, NULL, "input file", wxCMD_LINE_VAL_STRING,
	wxCMD_LINE_PARAM_OPTIONAL },
	{ wxCMD_LINE_NONE }
};


void MyApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.SetDesc(g_cmdLineDesc);
	// must refuse '/' as parameter starter or cannot use "/path" style paths
	parser.SetSwitchChars(wxT("-"));
}

bool MyApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	//silent_mode = parser.Found(wxT("s"));

	// to get at your unnamed parameters use
	wxArrayString files;
	for (int i = 0; i < parser.GetParamCount(); i++)
	{
		files.Add(parser.GetParam(i));
		break;
	}

	// and other command line parameters
	if (files.Count() > 0)
		fileToOpen = files[0];
	// then do what you need with them.

	return true;
}

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------
#ifdef __APPLE__
void MyApp::MacOpenFile(const wxString &fileName)
{
	//wxMessageBox("MacOpenFiles");
	//wxMessageBox(fileName);
	frameViewer->OpenFile(fileName);
}
#endif

int MyApp::Close()
{

	COpenCLEngine::kill();

	CSqlInit::KillSqlEngine();
	CPrintEngine::Kill();

	CLibResource::KillSqlEngine();

	if (regardsParam != nullptr)
		regardsParam->SaveFile();

	if (frameStart != nullptr)
		frameStart->Destroy();

	if (frameViewer != nullptr)
		frameViewer->Destroy();


	sqlite3_shutdown();

	this->Exit();

	return 0;
}


// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{



	/*
	wxSVGFileDC svgDC (L"/Users/jacques/Pictures/test.svg", 40, 40) ;

	svgDC.SetBrush(*wxTRANSPARENT_BRUSH);
	svgDC.SetPen(*wxBLACK_PEN);
	svgDC.DrawCircle(20,20,19);
	svgDC.SetBrush(*wxBLACK_BRUSH);
	svgDC.SetPen(*wxBLACK_PEN);
	wxPoint point[3];
	point[0] = wxPoint(14,10);
	point[1] = wxPoint(14,30);
	point[2] = wxPoint(30,20);
	svgDC.DrawPolygon(3, point);

	svgDC.DrawLine(30, 10, 30, 30);


	wxSVGFileDC svgDC (L"/Users/jacques/Pictures/back.svg", 40, 40) ;

	svgDC.SetBrush(*wxTRANSPARENT_BRUSH);
	svgDC.SetPen(*wxBLACK_PEN);
	svgDC.DrawCircle(20,20,19);
	svgDC.SetBrush(*wxBLACK_BRUSH);
	svgDC.SetPen(*wxBLACK_PEN);
	wxPoint point[3];
	point[0] = wxPoint(26,10);
	point[1] = wxPoint(26,30);
	point[2] = wxPoint(10,20);
	svgDC.DrawPolygon(3, point);

	svgDC.DrawLine(10, 10, 10, 30);

	Exit();
	*/

	//Test SDL Audio



	// call the base class initialization method, currently it only parses a
	// few common command-line options but it could be do more in the future
	if (!wxApp::OnInit())
		return false;

#ifdef __APPLE__
	wxSystemOptions::SetOption(wxOSX_FILEDIALOG_ALWAYS_SHOW_TYPES, 1);
#endif

	sqlite3_initialize();

	wxInitAllImageHandlers();

	wxSocketBase::Initialize();


	CPrintEngine::Initialize();
	wxString resourcePath = CFileUtility::GetResourcesFolderPath();
	wxString documentPath = CFileUtility::GetDocumentFolderPath();

	CLibResource::InitializeSQLServerDatabase(resourcePath);
	CSqlInit::InitializeSQLServerDatabase(documentPath);

	//Chargement des paramËtres de l'application
	regardsParam = new CRegardsConfigParam();
	CParamInit::Initialize(regardsParam);

	COpenCLEngine::Init();

	//Chargement des ressources
	wxXmlResource::Get()->InitAllHandlers();

#ifdef WIN32

	wxXmlResource::Get()->Load(resourcePath + "\\ConfigRegards.xrc");
	wxXmlResource::Get()->Load(resourcePath + "\\CopyFileDlg.xrc");
	wxXmlResource::Get()->Load(resourcePath + "\\OpenCLDialog.xrc");
	wxXmlResource::Get()->Load(resourcePath + "\\GifOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + "\\JpegOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + "\\TiffOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + "\\PngOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + "\\CompressionOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + "\\ExportFile.xrc");
#else
	wxXmlResource::Get()->Load(resourcePath + "/ConfigRegards.xrc");
	wxXmlResource::Get()->Load(resourcePath + "/CopyFileDlg.xrc");
	wxXmlResource::Get()->Load(resourcePath + "/OpenCLDialog.xrc");
	wxXmlResource::Get()->Load(resourcePath + "/GifOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + "/JpegOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + "/TiffOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + "/PngOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + "/SaveFileFormat.xrc");
	wxXmlResource::Get()->Load(resourcePath + "/CompressionOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + "/ExportFile.xrc");
#endif



	//wxXmlResource::Get()->LoadAllFiles("rc");

	frameStart = new MyFrameIntro("Welcome to Regards", wxPoint(50, 50), wxSize(450, 340), this);
	frameStart->Centre(wxBOTH);
	frameStart->Show(true);

#ifdef VIEWER
	CViewerFrame::SetViewerMode(true);
	frameViewer = new CViewerFrame("Regards Viewer", wxPoint(50, 50), wxSize(1200, 800), this, fileToOpen);
	frameViewer->Centre(wxBOTH);
	frameViewer->Show(true);

#else
	CViewerFrame::SetViewerMode(false);
	frameExplorer = new CExplorerFrame("Regards Explorer", wxPoint(50, 50), wxSize(1200, 800), this);
	frameExplorer->Centre(wxBOTH);
	frameExplorer->Show(true);
#endif
	// create the main application window
	//MyFrame *frame = new MyFrame("Minimal wxWidgets App");
	//frame->SetClientSize(640, 480);
	//frame->Centre();
	//frame->Show();
	// and show it (the frames, unlike simple controls, are not shown when
	// created initially)
	//frame->Show(true);

	// success: wxApp::OnRun() will be called which will enter the main message
	// loop and the application will run. If we returned false here, the
	// application would exit immediately.
	return true;
}


