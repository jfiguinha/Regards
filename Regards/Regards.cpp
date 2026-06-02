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

#include "header.h"
#include <Regards.h>
#include <wx/window.h>
#include "SqlInit.h"
#include <libPicture.h>
#include <wx/webview.h>
#include <LibResource.h>
#include <FileUtility.h>
#include <ParamInit.h>
#include <FilterWindowParam.h>
#include <FilterData.h>
#include <OpenCLContext.h>
#include <appcontext.h>
#include <Gps.h>
#ifdef __WXGTK__
#include <fontconfig/fontconfig.h>
#endif
#include <ncnn/gpu.h>

AppContext application_context;
ncnn::VulkanDevice* vkdev = nullptr;

using namespace cv;
using namespace Regards::Picture;
using namespace Regards::Video;
using namespace Regards::OpenCL;

extern int Start(int argc, char **argv);

// Simple logging helpers to unify output (uses std::cout / std::cerr as requested)
static void LogInfo(const wxString& msg)
{
	std::cout << msg.ToStdString() << std::endl;
}

static void LogError(const wxString& msg)
{
	std::cerr << msg.ToStdString() << std::endl;
}

MyApp::MyApp()
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

void MyApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	LogInfo("Application Parameter :");
	parser.SetDesc(g_cmdLineDesc);
	// must refuse '/' as parameter starter or cannot use "/path" style paths
	parser.SetSwitchChars(wxT("-"));
    parser.EnableLongOptions();
    if (parser.Parse() != 0)
        return;
}

void MyApp::OnEventLoopEnter(wxEventLoopBase* WXUNUSED(loop))
{
	if (frameViewer != nullptr)
	{
		frameViewer->CreateWatcherIfNecessary();
	}
}

bool MyApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	// to get at your unnamed parameters use
	wxArrayString files;
	for (auto i = 0; i < parser.GetParamCount(); i++)
	{
		//printf("Files to show : %s \n", CConvertUtility::ConvertToUTF8(parser.GetParam(i)));
		files.Add(parser.GetParam(i));
		break;
	}

	// and other command line parameters
	if (files.Count() > 0)
	{
		fileToOpen = files[0];
	}
	
	if (parser.Found("program", &appName))
		LogInfo("App : " + appName);
	// then do what you need with them.
	
	LogInfo("Application Parameter :");
	LogInfo("File : " + fileToOpen);
	
	return true;
}

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------
#ifdef __APPLE__
void MyApp::MacOpenFile(const wxString &fileName)
{
    wxString message = "Mac Open Files : " + fileName;
    fileToOpen = fileName;
	//wxMessageBox(message);
	//wxMessageBox(fileName);
	//frameViewer->OpenFile(fileName);
}
#endif



int MyApp::Close()
{
   
	CSqlInit::KillSqlEngine();
	CPrintEngine::Kill();

	CLibResource::KillSqlEngine();

	if (regardsParam != nullptr)
		regardsParam->SaveFile();

	// Reset unique_ptrs; their deleters will call Destroy() on wx windows
	// (frameStart may be a top-level window)

	sqlite3_shutdown();
#ifdef USECURL
	curl_global_cleanup();
#endif
	//this->Exit();

	CWindowMain::listMainWindow.clear();

	CLibPicture::UninitFreeImage();


#ifdef __WXMSW__
	CoUninitialize();

#endif

#ifdef FFMPEG
	avformat_network_deinit();
	//av_lockmgr_register(nullptr);
#endif

#if defined(__WXMSW__) && defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif

	// unique_ptr will free resources when reset/destroyed
	frameStart.reset();
	frameViewer.reset();
	framePDF.reset();
	frameVideoConverter.reset();
	testFrame.reset();
        
	if(vkdev != nullptr)
		ncnn::destroy_gpu_instance();

	// Signal the main loop to exit instead of abruptly terminating the process
	ExitMainLoop();

	return 0;
}

bool LocaleMakeDir(wxString folder)
{
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
#ifdef WIN32
	documentPath.append("\\" + folder);
#else
	documentPath.append("/" + folder);
#endif

	if (!wxDirExists(documentPath))
	{
		return wxDir::Make(documentPath);
	}
	return true;	
}

wxBitmap GetIcon(const wxArtID& id, const wxSize& sz)
{
    wxBitmap bmp = wxArtProvider::GetBitmap(id, wxART_CMN_DIALOG, sz);
    return bmp;
}

void SaveIcon(const wxArtID& id, wxString filename)
{
    wxSize m_size = wxSize(16, 16);
    wxBitmap bmp = GetIcon(id, m_size);
    wxImage image = bmp.ConvertToImage();
    image.SaveFile(filename, wxBITMAP_TYPE_PNG);
}

void MyApp::CheckGeolocalisationServiceAvailability()
//Verify if Geolocalisation is available
{
	wxString urlServer = "";
	wxString apiKey = "";
	int ShowInfosGeolocUnavailable = 1;
	//Géolocalisation
	CRegardsConfigParam* param = CParamInit::getInstance();
	if (param != nullptr)
	{
		urlServer = param->GetGeoLocUrlServer();
		apiKey = param->GetApiKey();
		ShowInfosGeolocUnavailable = param->ShowInfosGeolocUnavailable();
	}
	bool result = Regards::Internet::CGps::IsLocalisationAvailable(urlServer, apiKey);
	if (!result)
		application_context.isGPsAvailable = false;
	else
		application_context.isGPsAvailable = true;

	if (!result && ShowInfosGeolocUnavailable)
	{
		wxMessageBox(_("Geolocalisation service is not available. Please check your geoplugin.net API key locate in the configuration window."));
		param->SetShowInfosGeolocUnavailable(0);
	}
}

void MyApp::CheckOpenCLAvailability(bool configFileExist)
{

	bool testOpenCL = true;
	regardsParam->SetIsCudaSupport(0);

	if (testOpenCL)
	{
		if (!ocl::haveOpenCL())
		{
			LogInfo("OpenCL is not available...");
		}
		else
		{
			LogInfo("OpenCL is available...");
		}

		if (!configFileExist)
		{
			if (!ocl::haveOpenCL())
			{
				regardsParam->SetIsOpenCLSupport(false);
				regardsParam->SetIsOpenCLOpenGLInteropSupport(false);
			}
			else
			{
				regardsParam->SetIsOpenCLSupport(true);
				regardsParam->SetIsOpenCLOpenGLInteropSupport(true);
			}
		}


		if (regardsParam->GetIsOpenCLSupport() && !regardsParam->GetIsOpenCLOpenGLInteropSupport())
		{
			if (!ocl::haveOpenCL())
			{
				LogInfo("OpenCL is not available...");
			}
			else
			{
				COpenCLContext::CreateDefaultOpenCLContext();
			}

			if (!application_context.isOpenCLInitialized)
			{
				regardsParam->SetIsOpenCLSupport(false);
				LogInfo("OpenCL is not Initialized...");
			}
		}
	}
	application_context.openclOpenGLInterop = regardsParam->GetIsOpenCLOpenGLInteropSupport();
}


bool MyApp::InitializeLocale()
{
	// 
	// call the base class initialization method, currently it only parses a
	// few common command-line options but it could be do more in the future
	if (!wxApp::OnInit())
		return false;

	if (wxWebView::IsBackendAvailable(wxWebViewBackendEdge))
	{
		LogInfo("WebView Edge backend available");
	}

#ifdef __APPLE__
	wxSystemOptions::SetOption(wxOSX_FILEDIALOG_ALWAYS_SHOW_TYPES, 1);
	//wxSystemOptions::SetOption( wxMAC_WINDOW_PLAIN_TRANSITION, 0 );
#endif

#ifdef __WXGTK__
	FcBool result = FcInit();
	if (result)
		LogInfo("Font Config Initialized");
#endif
	/*


	wxInitAllImageHandlers();
	// folder:
	SaveIcon(wxART_FOLDER, "folder.png");
	SaveIcon(wxART_FOLDER_OPEN, "folder_open.png");
	SaveIcon(wxART_HARDDISK, "harddisk.png");
	SaveIcon(wxART_CDROM, "cdrom.png");
	SaveIcon(wxART_FLOPPY, "floppy.png");
	SaveIcon(wxART_REMOVABLE, "removable.png");
	SaveIcon(wxART_NORMAL_FILE, "normal.png");

	exit(0);
	*/

#ifdef WIN32
	LCID lcid = GetThreadLocale();
	wchar_t name[LOCALE_NAME_MAX_LENGTH];
	if (LCIDToLocaleName(lcid, name, LOCALE_NAME_MAX_LENGTH, 0) == 0)
		printf("%s", GetLastError());
	std::wcout << L"Locale name = " << name << std::endl;

	char buffer[64];
	int ret;
	ret = wcstombs(buffer, name, sizeof(buffer));
	setlocale(LC_ALL, buffer);

#elif __APPLE__
	std::locale::global(std::locale(""));

#else

	//std::locale loc;
	//string locName = loc.name();
	//setlocale(LC_ALL, locName.c_str());

#endif

#ifdef __APPLE__
	setlocale(LC_NUMERIC, "en_US.UTF-8");
#elif defined(WIN32)
	std::setlocale(LC_NUMERIC, "en_US.UTF-8");
#endif

	return true;
}

bool MyApp::InitializeDirectories()
{
	wxString programPath = CFileUtility::GetProgramFolderPath();

	cout << "Program Path : " << programPath << endl;

	wxString regardsdb_path = CFileUtility::GetResourcesFolderPathWithExt("regards.db");
	wxString regardsdocumentdb_path = CFileUtility::GetDocumentFolderPathWithExt("regards.db");

	if (!LocaleMakeDir("Thumbnail"))
	{
		printf("Unable to make folder Thumbnail");
		exit(0);
	}
	if (!LocaleMakeDir("ThumbnailVideo"))
	{
		printf("Unable to make folder ThumbnailVideo");
		exit(0);
	}
	if (!LocaleMakeDir("temp"))
	{
		printf("Unable to make folder temp");
		exit(0);
	}
	if (!LocaleMakeDir("Face"))
	{
		printf("Unable to make folder Face");
		exit(0);
	}
	if (!LocaleMakeDir("model"))
	{
		printf("Unable to make folder Face");
		exit(0);
	}

	if (!wxFileExists(regardsdocumentdb_path))
		wxCopyFile(regardsdb_path, regardsdocumentdb_path);

	return true;
}

bool MyApp::InitializeDatabase()
{
	sqlite3_initialize();
	return true;
}

void MyApp::LoadPicture(const int& svgWidth, const int& svgHeight)
{
	application_context.LoadWxDefaultPicture(CLibResource::GetPhotoCancel());
	application_context.LoadWxDefaultPictureThumbnail(CLibResource::GetPhotoCancel());
	application_context.LoadWxDefaultPictureThumbnailVideo(CLibResource::GetPhotoCancel());		
}


bool MyApp::InitializeResources()
{

	wxString resourcePath = CFileUtility::GetResourcesFolderPath();
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
	//task_scheduler_init init;
	//int n = tbb::task_scheduler_init::default_num_threads();


	wxInitAllImageHandlers();

	CLibPicture::InitFreeImage();

	wxSocketBase::Initialize();

	CPrintEngine::Initialize();


#ifdef GLUT
#ifndef __APPLE__

	if (argc > 1)
	{
		if (argc == 3)
		{
			wxString parameter = wxTheApp->argv[1];
			if (parameter == "-p")
				appName = wxTheApp->argv[argc - 1];
		}
	}


	int _argc = 1;
	char* argv[1] = { wxString((wxTheApp->argv)[0]).char_str() };
	glutInit(&_argc, argv);
#endif
#endif

#ifndef NDEBUG
	::wxMessageBox("toto");
#endif

	//Chargement des paramËtres de l'application
	regardsParam = new CRegardsConfigParam();
	CParamInit::Initialize(regardsParam);


	bool dataInMemory = regardsParam->GetDatabaseInMemory();

	if (!CLibResource::InitializeSQLServerDatabase(resourcePath))
	{
		wxMessageBox("Unable to initialize Resource.db SQL database");
		exit(0);
	}

	if (!CSqlInit::InitializeSQLServerDatabase(documentPath, dataInMemory))
	{
		wxMessageBox("Unable to initialize Regards.db SQL database");
		exit(0);
	}

	CFilterWindowParam::InitFilterOpenCLCompatible();

	//Chargement des ressources
	wxXmlResource::Get()->InitAllHandlers();

	CFiltreData::CreateFilterList();

	bool configFileExist = CParamInit::IsConfigFileExist();

	bool firstInitialisation = true;
	std::set_terminate(onTerminate);

	CheckOpenCLAvailability(configFileExist);

	CheckGeolocalisationServiceAvailability();


	wxString toto = CLibResource::GetOpenGLShaderFromDB("IDR_GLSL_SHADER_BILINEAR");

#ifdef WIN32
	wxString numIdLang = "\\" + to_string(regardsParam->GetNumLanguage()) + "\\msw";
#else
#ifdef __APPLE__
	wxString numIdLang = "/" + to_string(regardsParam->GetNumLanguage()) + "/osx";
#else
	wxString numIdLang = "/" + to_string(regardsParam->GetNumLanguage()) + "/linux";
#endif
#endif
	wxXmlResource::Get()->LoadAllFiles(resourcePath + numIdLang);

	int svgWidth = 256;
	int svgHeight = 256;

	LoadPicture(svgWidth, svgHeight);	

	return true;
}

void MyApp::LaunchApplication()
{

	if (appName == "RegardsConverter")
	{
		wxDisplay display;
		wxRect screen = display.GetClientArea();
		frameVideoConverter = WxPtr<CVideoConverterFrame>(new CVideoConverterFrame("RegardsConverter", wxDefaultPosition, wxSize(50, 50), this), [](CVideoConverterFrame* p){ if(p) p->Destroy(); });
		frameVideoConverter->Centre(wxBOTH);
		frameVideoConverter->SetFocus();  // focus on my window
		frameVideoConverter->Raise();  // bring window to front
		frameVideoConverter->Show(true);
		frameVideoConverter->ExportVideo(fileToOpen);
	}
	else if (appName == "RegardsTest")
	{
		wxDisplay display;
		wxRect screen = display.GetClientArea();
		testFrame = WxPtr<CTestFrame>(new CTestFrame("RegardsTest", wxDefaultPosition, wxSize(50, 50)), [](CTestFrame* p){ if(p) p->Destroy(); });
		testFrame->Centre(wxBOTH);
		testFrame->SetFocus();  // focus on my window
		testFrame->Raise();  // bring window to front
		testFrame->Show(true);
	}
	else if (appName == "RegardsPDF")
	{
		wxDisplay display;
		wxRect screen = display.GetClientArea();
		//	CScannerFrame(const wxString &title, ISCannerInterface * mainInterface, const wxPoint &pos, const wxSize &size, long style = wxDEFAULT_FRAME_STYLE);
		framePDF = WxPtr<CScannerFrame>(new CScannerFrame("RegardsPDF", this, wxDefaultPosition, wxSize(screen.GetWidth(), screen.GetHeight())), [](CScannerFrame* p){ if(p) p->Destroy(); });
		framePDF->Centre(wxBOTH);
		framePDF->SetFocus();  // focus on my window
		framePDF->Raise();  // bring window to front
		framePDF->Show(true);
		framePDF->OnOpen();
	}
	else
	{
		/*
		frameStart = new MyFrameIntro("Welcome to Regards", "REGARDS V2", wxPoint(50, 50), wxSize(450, 340), this);
		frameStart->Centre(wxBOTH);
		frameStart->Show(true);
		*/
		ShowViewer();

		CViewerFrame::SetViewerMode(true);
	}
}

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{


	if (!wxApp::OnInit())
		return false;
	// Ensure unique_ptr frame members are explicitly null-initialized
	// (unique_ptrs default to nullptr, but be explicit for clarity)
	frameStart.reset();
	frameViewer.reset();
	framePDF.reset();
	frameVideoConverter.reset();
	testFrame.reset();

	if (!InitializeLocale()) return false;
	if (!InitializeDirectories()) return false;
	if (!InitializeDatabase()) return false;
	if (!InitializeResources()) return false;

	LaunchApplication();

	return true;

	// success: wxApp::OnRun() will be called which will enter the main message
	// loop and the application will run. If we returned false here, the
	// application would exit immediately.
	return true;
}


// Get the file extension filter for all registered image handlers.
//
wxString MyApp::GetImageFilter()
{
	return _("All image files") + wxString(wxT("|"))
		+ m_strImageFilter + wxT("|")
		+ m_strImageFilterList
		+ _("All files") + wxT("|*.*");
}


// Image handler and extension filter handling.
//
// Adds image handlers to the image handler list.
//
void MyApp::AddImageHandler(wxImageHandler* poHandler)
{
	if (poHandler)
	{
		wxString strExtension = poHandler->GetExtension();

		// TODO: Is there a posibility to get files by extension fully
		//       case insensitive?
		//
#ifdef __WXMSW__
		m_strImageFilter += wxT("*.") + strExtension + wxT(";");
		m_strImageFilterList += poHandler->GetMimeType() + wxT(" (*.")
			+ strExtension
			+ wxT(")|*.") + strExtension + wxT("|");
#else
        m_strImageFilter += wxT( "*." ) + strExtension.Lower() + wxT( ";" )
                         +  wxT( "*." ) + strExtension.Upper() + wxT( ";" );
        m_strImageFilterList += poHandler->GetMimeType() + wxT( " (*." )
                                + strExtension
                                + wxT( ")|*." )
                                + strExtension.Lower()
                                + wxT( ";*." )
                                + strExtension.Upper()
                                + wxT( "|" );
#endif
		if (wxImage::FindHandler(poHandler->GetName()))
		{
			delete poHandler;
		}
		else
		{
			wxImage::AddHandler(poHandler);
		}
	}
}
