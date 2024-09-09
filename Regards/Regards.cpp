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
#include <ffplaycore.h>
#include <picture_utility.h>
#include <ImageVideoThumbnail.h>
#include <OpenCLContext.h>
#include <wx/progdlg.h>
#include <DownloadFile.h>
#include <ncnn/gpu.h>
#include <locale>
string platformName = "";
bool isOpenCLInitialized = false;
bool firstElementToShow = true;
int numElementToLoad = 5;
cv::ocl::OpenCLExecutionContext clExecCtx;

using namespace Regards::Internet;
using namespace cv;
using namespace Regards::Picture;
using namespace Regards::Video;

extern int Start(int argc, char **argv);

void MyApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	cout << "Application Parameter : " << endl;
	parser.SetDesc(g_cmdLineDesc);
	// must refuse '/' as parameter starter or cannot use "/path" style paths
	parser.SetSwitchChars(wxT("-"));
    parser.EnableLongOptions();
    if (parser.Parse() != 0)
        return;
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
	
	if(parser.Found("program", &appName))
		cout << "App : " << appName << endl;
	// then do what you need with them.
	
	cout << "Application Parameter : " << endl;
	cout << "File : " << fileToOpen << endl;
	
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

	if (frameStart != nullptr)
		frameStart->Destroy();


	sqlite3_shutdown();
#ifdef USECURL
	curl_global_cleanup();
#endif
	this->Exit();

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

	//exit(0);

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

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
	std::set_terminate(onTerminate);

	// call the base class initialization method, currently it only parses a
	// few common command-line options but it could be do more in the future
	if (!wxApp::OnInit())
		return false;

	if (wxWebView::IsBackendAvailable(wxWebViewBackendEdge))
	{
		printf("toto");
	}

#ifdef __APPLE__
	wxSystemOptions::SetOption(wxOSX_FILEDIALOG_ALWAYS_SHOW_TYPES, 1);
    //wxSystemOptions::SetOption( wxMAC_WINDOW_PLAIN_TRANSITION, 0 );
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

	wxString resourcePath = CFileUtility::GetResourcesFolderPath();
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
    
    wxString regardsdb_path = CFileUtility::GetResourcesFolderPathWithExt("regards.db");
    wxString regardsdocumentdb_path = CFileUtility::GetDocumentFolderPathWithExt("regards.db");

	if(!LocaleMakeDir("Thumbnail"))
	{
		printf("Unable to make folder Thumbnail");
		exit(0);
	}	
	if(!LocaleMakeDir("ThumbnailVideo"))
	{
		printf("Unable to make folder ThumbnailVideo");
		exit(0);
	}	
	if(!LocaleMakeDir("temp"))
	{
		printf("Unable to make folder temp");
		exit(0);
	}	
	if(!LocaleMakeDir("Face"))
	{
		printf("Unable to make folder Face");
		exit(0);
	}
	if (!LocaleMakeDir("model"))
	{
		printf("Unable to make folder Face");
		exit(0);
	}
    
    if(!wxFileExists(regardsdocumentdb_path))
        wxCopyFile(regardsdb_path, regardsdocumentdb_path);

	sqlite3_initialize();

	wxInitAllImageHandlers();

	CLibPicture::InitFreeImage();

	wxSocketBase::Initialize();

	CPrintEngine::Initialize();


#ifdef GLUT
#ifndef __APPLE__
	int argc = 1;
	char* argv[1] = {wxString((wxTheApp->argv)[0]).char_str()};
	glutInit(&argc, argv);
#endif
#endif

#ifndef NDEBUG
    ::wxMessageBox("toto");
#endif

	//task_scheduler_init init;
	//int n = tbb::task_scheduler_init::default_num_threads();
	bool configFileExist = CParamInit::IsConfigFileExist();

	//Chargement des paramËtres de l'application
	regardsParam = new CRegardsConfigParam();
	CParamInit::Initialize(regardsParam);


	bool dataInMemory = regardsParam->GetDatabaseInMemory();

	CLibResource::InitializeSQLServerDatabase(resourcePath);
	CSqlInit::InitializeSQLServerDatabase(documentPath, dataInMemory);

	CFilterWindowParam::InitFilterOpenCLCompatible();

	//Chargement des ressources
	wxXmlResource::Get()->InitAllHandlers();

	CFiltreData::CreateFilterList();

	if (!configFileExist)
	{
		if (!ocl::haveOpenCL())
			regardsParam->SetIsOpenCLSupport(false);
		else
			regardsParam->SetIsOpenCLSupport(true);
	}


	if (ocl::haveOpenCL())
	{
		int select = 0;
		cl_uint numPlatforms = ncnn::get_gpu_count();
		for (int i = 0; i < numPlatforms; i++)
		{
			const ncnn::GpuInfo & pguInfo = ncnn::get_gpu_info(i);
			string deviceName = pguInfo.device_name();
            for(auto& c : deviceName)
            {
               c = tolower(c);
            }
			if (deviceName.find("nvidia") != std::string::npos)
				select = i;
			if (deviceName.find("amd") != std::string::npos)
				select = i;
		}


		vkdev = ncnn::get_gpu_device(select);
		
	}

    
	if (regardsParam->GetIsOpenCLSupport() && !regardsParam->GetIsOpenCLOpenGLInteropSupport())
	{
		if (!ocl::haveOpenCL())
		{
			cout << "OpenCL is not avaiable..." << endl;
		}
		else
		{
            COpenCLContext::CreateDefaultOpenCLContext();
		}

		if (!isOpenCLInitialized)
		{
			regardsParam->SetIsOpenCLSupport(false);
		}
	}
    

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
    
    defaultPicture.LoadFile(CLibResource::GetPhotoCancel(), wxBITMAP_TYPE_ANY);
	defaultPictureThumbnailPicture = CLibResource::CreatePictureFromSVG("IDB_PHOTOTEMP", svgWidth, svgHeight).ConvertToDisabled();
	defaultPictureThumbnailVideo = CLibResource::CreatePictureFromSVG("IDB_MOVIE", svgWidth, svgHeight).ConvertToDisabled();

	if(appName == "RegardsConverter")
	{
		wxDisplay display;
		wxRect screen = display.GetClientArea();
		frameVideoConverter = new CVideoConverterFrame("RegardsConverter", wxDefaultPosition, wxSize(50,50), this);
		frameVideoConverter->Centre(wxBOTH);
		frameVideoConverter->Show(true);
		frameVideoConverter->ExportVideo(fileToOpen);
	}
	else if(appName == "RegardsPDF")
	{
		wxDisplay display;
		wxRect screen = display.GetClientArea();
		//	CScannerFrame(const wxString &title, ISCannerInterface * mainInterface, const wxPoint &pos, const wxSize &size, long style = wxDEFAULT_FRAME_STYLE);
		framePDF = new CScannerFrame("RegardsPDF", this,  wxDefaultPosition, wxSize(screen.GetWidth(), screen.GetHeight()));
		framePDF->Centre(wxBOTH);
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
