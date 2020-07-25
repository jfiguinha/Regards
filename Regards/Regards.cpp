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
#include <Regards.h>
#include <wx/filename.h>
#include <DeepLearning.h>
#ifdef wxUSE_PDF
#include <wx/wxpoppler.h>
#endif


using namespace Regards::DeepLearning;

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
    fileToOpen = fileName;
	//wxMessageBox(message);
	//wxMessageBox(fileName);
	//frameViewer->OpenFile(fileName);
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
    
#ifndef RENDEROPENGL  

	if (openCLEngine != nullptr)
		delete openCLEngine;

#endif

	sqlite3_shutdown();
#ifdef USECURL
    curl_global_cleanup();
#endif
	this->Exit();

	CWindowMain::listMainWindow.clear();
    
    CLibPicture::Uninitx265Decoder();

    CLibPicture::UninitFreeImage();
	

#ifdef __WXMSW__
	

	CoUninitialize();

#endif


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

	CLibPicture::InitFreeImage();

#ifdef wxUSE_PDF
	    AddImageHandler(new wxPDFHandler);
#endif
    
#ifdef __WXMSW__

	// Initialize the COM library 
	
	//HRESULT hr = CoInitialize(NULL);

	///////////////////////////////////////////////////////////////////
	// Initialize common controls this is for the progress bar control
	//
    //InitCommonControls(); 
#endif

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

	vector<OpenCLPlatform*> listPlatform = COpenCLPlatformList::GetPlatform();
	if (listPlatform.size() == 0)
	{
		regardsParam->SetIsOpenCLSupport(false);
	}
	else
	{
		regardsParam->SetIsOpenCLSupport(true);


	}


#ifndef RENDEROPENGL  

	openCLEngine = new COpenCLEngine();

	if (openCLEngine != nullptr)
		openclContext = openCLEngine->GetInstance();

#endif

#ifdef WIN32

    wxString numIdLang = "\\" + to_string(regardsParam->GetNumLanguage());
	
	/*
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\CategoryDetection.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\FaceDetection.xrc");
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
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\BmpOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\ExrOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\J2kOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\JxrOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\PDFOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "\\Association.xrc");
	*/
#else
    wxString numIdLang = "/" + to_string(regardsParam->GetNumLanguage());
	/*
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/CategoryDetection.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/FaceDetection.xrc");
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
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/BmpOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/ExrOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/J2kOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/JxrOption.xrc");
	wxXmlResource::Get()->Load(resourcePath + numIdLang + "/PDFOption.xrc");
	*/
#endif
	wxXmlResource::Get()->LoadAllFiles(resourcePath + numIdLang);
	

#ifdef PREPARE_DATA_TRAINING
	//Create image learning
	//int i = 0;
	CLibPicture libPicture;
	wxArrayString files;
	wxString folder = "train";
	wxDir::GetAllFiles("F:\\images_source\\0", &files, wxEmptyString, wxDIR_FILES);
	for (wxString file : files)
	{
		folder = "train";
		/*
		if (i < 40000)
		{
			folder = "train";
		}
		else if (i < 50000)
		{
			folder = "validation";
		}
		else
			break;
		*/
		for (int i = 0; i < 4; i++)
		{
			if (libPicture.TestImageFormat(file) != 0)
			{
				wxString libelle;
				int type = i % 4;
				wxFileName filePath(file);
				CImageLoadingFormat * image = libPicture.LoadPicture(file);
				float ratio = float(image->GetWidth()) / float(image->GetHeight());
				image->Resize(1000, image->GetHeight() * ratio, 0);
				switch (type)
				{
				case 1:
					image->Rotate90();
					image->Mirror();
					break;
				case 2:
					image->Mirror();
					image->Flip();
					break;
				case 3:
					image->Rotate90();
					image->Flip();
					break;
				}
				switch (type)
				{
				case 0:
					libelle = wxString("F:\\picture_training\\raw-data\\" + folder + "\\0\\" + filePath.GetName() + ".jpg");
					break;
				case 1:
					libelle = wxString("F:\\picture_training\\raw-data\\" + folder + "\\90\\" + filePath.GetName() + ".jpg");
					break;
				case 2:
					libelle = wxString("F:\\picture_training\\raw-data\\" + folder + "\\180\\" + filePath.GetName() + ".jpg");
					break;
				case 3:
					libelle = wxString("F:\\picture_training\\raw-data\\" + folder + "\\270\\" + filePath.GetName() + ".jpg");
					break;
				}
				image->SaveToJpeg(libelle);
				delete image;
				/*
				cv::Mat image = cv::imread(file.ToStdString());// test\\photo.jpg");


				float ratio = float(image.rows) / float(image.cols);
				cv::resize(image, image, cv::Size(1000, image.rows * ratio));
				/*
				int sizeCrop = 0;

				if (image.cols > image.rows)
					sizeCrop = image.rows;
				else
					sizeCrop = image.cols;

				int width = 224;
				int height = 224;

				int x1 = int((image.cols - sizeCrop) * 0.5);
				//int x2 = int(image_center[0] + width * 0.5);
				int y1 = int((image.rows - sizeCrop) * 0.5);
				//int y2 = int(image_center[1] + height * 0.5);
				cv::Rect myROI(x1, y1, sizeCrop, sizeCrop);

				try
				{
					cv::Mat croppedImage = image(myROI);
					cv::resize(croppedImage, image, cv::Size(224, 224));
				}
				catch (cv::Exception& e)
				{
					const char* err_msg = e.what();
					std::cout << "exception caught: " << err_msg << std::endl;
					std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
				}
				*/

				/*
				cv::Mat dst;      //Mat object for output image file
				cv::Point2f pt(image.cols / 2., image.rows / 2.);          //point from where to rotate    
				cv::Mat r;       //Mat object for storing after rotation
					///applie an affine transforation to image.

				switch (type)
				{
				case 0:
					r = cv::getRotationMatrix2D(pt, 0, 1.0);      //Mat object for storing after rotation
					warpAffine(image, dst, r, cv::Size(image.cols, image.rows));
					break;
				case 1:
					r = cv::getRotationMatrix2D(pt, 90, 1.0);      //Mat object for storing after rotation	
					warpAffine(image, dst, r, cv::Size(image.rows, image.cols));
					break;
				case 2:
					r = cv::getRotationMatrix2D(pt, 180, 1.0);      //Mat object for storing after rotation
					warpAffine(image, dst, r, cv::Size(image.cols, image.rows));
					break;
				case 3:
					r = cv::getRotationMatrix2D(pt, 270, 1.0);      //Mat object for storing after rotation
					warpAffine(image, dst, r, cv::Size(image.rows, image.cols));
					break;
				}

				


				//image->Resize(1000, image->GetHeight() * ratio, 0);
				//int orientation = image->GetOrientation();
				wxString libelle;
				switch (type)
				{
				case 0:
					libelle = wxString("F:\\picture_training\\raw-data\\" + folder + "\\0\\" + filePath.GetName() + ".jpg");
					break;
				case 1:
					libelle = wxString("F:\\picture_training\\raw-data\\" + folder + "\\90\\" + filePath.GetName() + ".jpg");
					break;
				case 2:
					libelle = wxString("F:\\picture_training\\raw-data\\" + folder + "\\180\\" + filePath.GetName() + ".jpg");
					break;
				case 3:
					libelle = wxString("F:\\picture_training\\raw-data\\" + folder + "\\270\\" + filePath.GetName() + ".jpg");
					break;
				}
				cv::imwrite(libelle.ToStdString(), dst);
				//delete image;
				//i++;
				*/
			}
		}
	}

	Exit();
    //wxXmlResource::Get()->LoadAllFiles("rc");
#endif
	
	frameStart = new MyFrameIntro("Welcome to Regards","REGARDS V2", wxPoint(50, 50), wxSize(450, 340), this);
	frameStart->Centre(wxBOTH);
	frameStart->Show(true);
    
	CViewerFrame::SetViewerMode(true);

   

	// success: wxApp::OnRun() will be called which will enter the main message
	// loop and the application will run. If we returned false here, the
	// application would exit immediately.
	return true;
}


// Get the file extension filter for all registered image handlers.
//
wxString MyApp::GetImageFilter()
{
    return _( "All image files" ) + wxString( wxT( "|" ) )
           + m_strImageFilter + wxT( "|" )
           + m_strImageFilterList
           + _( "All files" ) + wxT( "|*.*" );
}



// Image handler and extension filter handling.
//
// Adds image handlers to the image handler list.
//
void MyApp::AddImageHandler( wxImageHandler* poHandler )
{
    if( poHandler )
    {
        wxString strExtension= poHandler->GetExtension();

        // TODO: Is there a posibility to get files by extension fully
        //       case insensitive?
        //
#ifdef __WXMSW__
        m_strImageFilter += wxT( "*." ) + strExtension + wxT( ";" );
        m_strImageFilterList += poHandler->GetMimeType() + wxT( " (*." )
                                + strExtension
                                + wxT( ")|*." ) + strExtension + wxT( "|" );
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
        if( wxImage::FindHandler( poHandler->GetName() ) )
        {
            delete poHandler;
        }
        else
        {
            wxImage::AddHandler( poHandler );
        }
    }
}