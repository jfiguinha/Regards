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
#ifdef USECURL
#include <curl/curl.h>
#endif
#include <OpenCLEngine.h>
using namespace Regards::OpenCL;

#ifdef SDL2
#include <SDL.h>
#include <SDL_audio.h> 
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

    switch(op)
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
        mutex * m =  static_cast<mutex*>(*_mutex);
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
#endif //GLEW_MX

#include <wx/glcanvas.h>
int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };

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
        vector<OpenCLPlatform *> listPlatform = COpenCLPlatformList::GetPlatform();
        if(listPlatform.size() == 0)
        {
			wxMessageBox(wxT("OpenCL device Required. Regards can't run."), wxT("Fatal Error"), wxOK | wxICON_ERROR);            
            exit(0);
        }
        
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
		frameViewer = new CViewerFrame("Regards Viewer", wxPoint(50, 50), wxSize(1200, 800), this, fileToOpen);
		frameViewer->Centre(wxBOTH);
		frameViewer->Show(true);
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

private:
	CRegardsConfigParam * regardsParam;
	MyFrameIntro * frameStart;
	CViewerFrame * frameViewer;
	wxString fileToOpen;

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


void MyApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.SetDesc(g_cmdLineDesc);
	// must refuse '/' as parameter starter or cannot use "/path" style paths
	parser.SetSwitchChars(wxT("-"));
}

bool MyApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    /*
    wxString nbArgument = to_string(wxGetApp().argc);
    wxMessageBox(nbArgument);
	//silent_mode = parser.Found(wxT("s"));
    wxString  par2(wxGetApp().argv[0]);
    wxMessageBox(par2);
    wxString par(wxGetApp().argv[1]);
    wxMessageBox(par);
     * */
	// to get at your unnamed parameters use
	wxArrayString files;
	for (auto i = 0; i < parser.GetParamCount(); i++)
	{
        printf("Files to show : %s \n",CConvertUtility::ConvertToUTF8(parser.GetParam(i)));
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
    wxString message = "Mac Open Files : " + fileName;
	//wxMessageBox(message);
	//wxMessageBox(fileName);
	frameViewer->OpenFile(fileName);
}
#endif

int MyApp::Close()
{

	//COpenCLEngine::kill();

	CSqlInit::KillSqlEngine();
	CPrintEngine::Kill();

	CLibResource::KillSqlEngine();

	if (regardsParam != nullptr)
		regardsParam->SaveFile();

	if (frameStart != nullptr)
		frameStart->Destroy();

	//if (frameViewer != nullptr)
	//	frameViewer->Destroy();
    

	sqlite3_shutdown();
#ifdef USECURL
    curl_global_cleanup();
#endif
	this->Exit();

	CWindowMain::listMainWindow.clear();
    
    CLibPicture::Uninitx265Decoder();
    
#if not defined(WIN32) && defined(LIBBPG)
    CLibPicture::UnloadBpgDll();
#endif
#ifdef FFMPEG
    av_lockmgr_register(nullptr);
#endif
	return 0;
}


// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
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
    
    printf("Document Path %s \n",CConvertUtility::ConvertToUTF8(documentPath));
    printf("Resource Path %s \n",CConvertUtility::ConvertToUTF8(resourcePath));

    CLibPicture::Initx265Decoder();

#if not defined(WIN32) && defined(LIBBPG)
    printf("LoadBpgDll\n");
    CLibPicture::LoadBpgDll();
#endif

#ifndef NDEBUG
    ::wxMessageBox("toto");
#endif

	//Chargement des paramËtres de l'application
	regardsParam = new CRegardsConfigParam();
	CParamInit::Initialize(regardsParam);

	bool dataInMemory = regardsParam->GetDatabaseInMemory();

	CLibResource::InitializeSQLServerDatabase(resourcePath);
	CSqlInit::InitializeSQLServerDatabase(documentPath, dataInMemory);


	//Chargement des ressources
	wxXmlResource::Get()->InitAllHandlers();

    

#ifdef WIN32

    wxString numIdLang = "\\" + to_string(regardsParam->GetNumLanguage());
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\ConfigRegards.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\CopyFileDlg.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\OpenCLDialog.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\GifOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\JpegOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\TiffOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\PngOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\CompressionOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\ExportFile.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\WebpOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\bpgOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\ChangeLabel.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\MoveFace.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\PertinenceValue.xrc");
    wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\IndexGenerator.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\InterpolationFilter.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\bm3d.xrc");
#else
    wxString numIdLang = "/" + to_string(regardsParam->GetNumLanguage());
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/ConfigRegards.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/CopyFileDlg.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/OpenCLDialog.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/GifOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/JpegOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/TiffOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/PngOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/SaveFileFormat.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/CompressionOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/ExportFile.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/webpoption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/bpgoption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/ChangeLabel.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/MoveFace.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/PertinenceValue.xrc");
    wxXmlResource::Get()->Load(resourcePath + numIdLang + "/IndexGenerator.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/InterpolationFilter.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/bm3d.xrc");
#endif
    
     
    //wxXmlResource::Get()->LoadAllFiles("rc");
	
	frameStart = new MyFrameIntro("Welcome to Regards", wxPoint(50, 50), wxSize(450, 340), this);
	frameStart->Centre(wxBOTH);
	frameStart->Show(true);
    
	CViewerFrame::SetViewerMode(true);

   

	// success: wxApp::OnRun() will be called which will enter the main message
	// loop and the application will run. If we returned false here, the
	// application would exit immediately.
	return true;
}
