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
#include <FilterData.h>
#include <wx/window.h>
#include "SqlInit.h"
#include <libPicture.h>
#include <FilterWindowParam.h>
#include <ConvertUtility.h>
#include <FileUtility.h>
#include "ParamInit.h"
#include <eyedetect.h>

using namespace cv;
using namespace Regards::Picture;

COpenCLContext* openclContext = nullptr;

void MyApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.SetDesc(g_cmdLineDesc);
	// must refuse '/' as parameter starter or cannot use "/path" style paths
	parser.SetSwitchChars(wxT("-"));
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

	CLibPicture::Uninitx265Decoder();

	CLibPicture::UninitFreeImage();


#ifdef __WXMSW__


	CoUninitialize();

#endif

#if not defined(WIN32) && defined(LIBBPG)
    CLibPicture::UnloadBpgDll();
#endif

#ifdef FFMPEG
	avformat_network_deinit();
	//av_lockmgr_register(nullptr);
#endif

#if defined(__WXMSW__) && defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
	std::set_terminate(onTerminate);
    
	// call the base class initialization method, currently it only parses a
	// few common command-line options but it could be do more in the future
	if (!wxApp::OnInit())
		return false;


#ifdef __APPLE__
	wxSystemOptions::SetOption(wxOSX_FILEDIALOG_ALWAYS_SHOW_TYPES, 1);
#endif
	 
	int retCode = sqlite3_config(SQLITE_CONFIG_SERIALIZED);
	if (retCode == SQLITE_OK)
	{
		printf("SQLite in serialized mode \n");
	}

#ifdef TEST_PCN
	wxArrayString listFiles;
	wxDir::GetAllFiles("C:\\Users\\jfigu\\Documents\\Regards\\face", &listFiles);

	const char* detection_model_path = "C:\\developpement\\FaceKit-master\\PCN\\model\\PCN.caffemodel";
	const char* pcn1_proto = "C:\\developpement\\FaceKit-master\\PCN\\model\\PCN-1.prototxt";
	const char* pcn2_proto = "C:\\developpement\\FaceKit-master\\PCN\\\model\\PCN-2.prototxt";
	const char* pcn3_proto = "C:\\developpement\\FaceKit-master\\PCN\\model\\PCN-3.prototxt";
	const char* tracking_model_path = "C:\\developpement\\FaceKit-master\\PCN\\model\\PCN-Tracking.caffemodel";
	const char* tracking_proto = "C:\\developpement\\FaceKit-master\\PCN\\model\\PCN-Tracking.prototxt";

	Net net_1 = readNet(pcn1_proto, detection_model_path);
	Net net_2 = readNet(pcn2_proto, detection_model_path);
	Net net_3 = readNet(pcn3_proto, detection_model_path);

	for (int i = 0; i < listFiles.size(); i++)
	{
		cv::Mat img = imread(listFiles[i].ToStdString());
		Mat paddedImg = padImg(img);

		cv::Mat img180, img90, imgNeg90;
		cv::flip(paddedImg, img180, 0);
		cv::transpose(paddedImg, img90);
		cv::flip(img90, imgNeg90, 0);

		float thresholds[] = { 0.37, 0.43, 0.95 };

		cv::TickMeter tm;
		tm.reset();
		tm.start();

		int minFaceSize = 40;
		std::vector<FaceBox> faces = PCN_1(img, paddedImg, net_1, thresholds[0], minFaceSize);
		faces = NMS(faces, true, 0.8);
		faces = PCN_2(paddedImg, img180, net_2, thresholds[1], 24, faces);
		faces = NMS(faces, true, 0.8);
		faces = PCN_3(paddedImg, img180, img90, imgNeg90, net_3, thresholds[2], 48, faces);
		faces = NMS(faces, false, 0.3);

		tm.stop();
		std::cout << "Time Cost: " << tm.getTimeMilli() << " ms" << std::endl;

		std::vector<FaceBox> preList = TransformBoxes(img, paddedImg, faces);

		for (int i = 0; i < preList.size(); i++)
		{
			DrawFace(img, preList[i]);
		}

		imshow("IMG", img);
		waitKey();
	}

#endif


#ifdef TEST
	int nbEyes = 0;
	vector<cv::Mat> valuePic;
	vector<cv::Point> value;
	bool isOk = false;
	wxArrayString listFiles;
	wxDir::GetAllFiles("C:\\Users\\jfigu\\Documents\\Regards\\Face", &listFiles);
	int angle = 0;
	for (int ui = 0; ui < listFiles.size();ui++)
	{
		Mat face = imread(listFiles[ui].ToStdString(), 0);
		for (int i = 0; i < 36; i++)
		{

			Mat tpl;
			cv::CascadeClassifier eye_cascade, mouth;
			std::vector<cv::Rect> eyes, mouthi;
			eye_cascade.load("d:\\haarcascade_eye.xml");
			eye_cascade.detectMultiScale(face, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, cv::Size(20, 20));
			mouth.load("d:\\haarcascade_mcs_mouth.xml");

			for (int p = 0; p < eyes.size(); p++)
			{
				Rect rect = eyes[p];
				if (rect.y > (face.size().height / 2))
				{
					eyes.erase(eyes.begin() + p);
					p--;
				}
			}

			for (int i = 0; i < eyes.size(); i++)
			{
				for (int j = 0; j < eyes.size(); j++)
				{
					if (i != j)
					{
						Rect interesect = eyes[i] & eyes[j];
						if (interesect.area() > 0)
						{
							eyes.erase(eyes.begin() + j);
							j--;
						}
					}
				}
			}


			int angleEye = 0;
			if (eyes.size() == 2)
			{
				cv::Point leftPupil;
				leftPupil.x = eyes[0].x + eyes[0].width / 2;
				leftPupil.y = eyes[0].y + eyes[0].height / 2;
				cv::Point rightPupil;
				rightPupil.x = eyes[1].x + eyes[1].width / 2;
				rightPupil.y = eyes[1].y + eyes[1].height / 2;
				angleEye = atan2(rightPupil.y - leftPupil.y, rightPupil.x - leftPupil.x) / M_PI * 180;
			}
			
			if (eyes.size() != 2 || (angleEye == 0))
			{
				Mat src = imread(listFiles[ui].ToStdString(), 0);
				angle += 10;
				// get rotation matrix for rotating the image around its center in pixel coordinates
				cv::Point2f center((src.cols - 1) / 2.0, (src.rows - 1) / 2.0);
				cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
				// determine bounding rectangle, center not relevant
				cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), src.size(), angle).boundingRect2f();
				// adjust transformation matrix
				rot.at<double>(0, 2) += bbox.width / 2.0 - src.cols / 2.0;
				rot.at<double>(1, 2) += bbox.height / 2.0 - src.rows / 2.0;
				cv::warpAffine(src, face, rot, bbox.size());
				continue;
			}

			imwrite("d:\\test_picture\\test" + to_string(ui) + ".jpg", face);



			bool error = false;

			if (eyes.size() > 2)
			{
				CEyeDetect eyeDetect;
				for (int p = 0; p < eyes.size(); p++)
				{
					Mat image;
					Rect rect = eyes[p];
					tpl = face(rect);
					int angle = eyeDetect.findEyes(tpl);
					imshow("test Right", tpl);
					waitKey(0);
				}
			}


			if (eyes.size() > 2)
			{
				printf("Error");
				error = true;
			}

			cv::Scalar colors[3];
			colors[0] = cv::Scalar(255, 0, 0);
			colors[1] = cv::Scalar(0, 255, 0);
			colors[2] = cv::Scalar(0, 0, 255);

			for (int k = 0; k < eyes.size(); k++)
			{
				Mat image;
				Rect rect = eyes[k];
				tpl = face(rect);
				nbEyes++;
			}

			break;

		}

		/*
		CEyeDetect eyeDetect;
   		eyes = eyeDetect.EyesPosition(face);
		Mat eyeLeft = face(eyes[0]);
		Mat eyeRight = face(eyes[1]);


		imshow("test Right", face);
		waitKey(0);

		imshow("test Left",eyeLeft);
		waitKey(0);

		imshow("test Right", eyeRight);
		waitKey(0);
		*/

	}
	exit(0);
#endif

	sqlite3_initialize();

	wxInitAllImageHandlers();

	CLibPicture::InitFreeImage();

	wxSocketBase::Initialize();

	CPrintEngine::Initialize();
	wxString resourcePath = CFileUtility::GetResourcesFolderPath();
	wxString documentPath = CFileUtility::GetDocumentFolderPath();

	printf("Document Path %s \n", CConvertUtility::ConvertToUTF8(documentPath));
	printf("Resource Path %s \n", CConvertUtility::ConvertToUTF8(resourcePath));

	CLibPicture::Initx265Decoder();




#ifdef GLUT
#ifndef __APPLE__
	int argc = 1;
	char* argv[1] = { wxString((wxTheApp->argv)[0]).char_str() };
	glutInit(&argc, argv);
#endif
#endif

#ifdef TEST_FFMPEG
	wxString tempAudio = "d:\\video\\audio.mp3";
	wxString filepathAudio = "d:\\video\\song.mp3";
	wxString extension = "mp3";
	CFFmpegApp fmpegApp;
	try
	{
		fmpegApp.CropAudio(filepathAudio, to_string(60), extension, tempAudio);
}
	catch (int e)
	{
		fmpegApp.Cleanup(e);
	}
#endif

#if not defined(WIN32) && defined(LIBBPG)
    printf("LoadBpgDll\n");
    CLibPicture::LoadBpgDll();
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
		int nbPlatform = Regards::OpenCL::COpenCLPlatformList::GetPlatformCount();
		if (nbPlatform == 0)
			regardsParam->SetIsOpenCLSupport(false);
		else
			regardsParam->SetIsOpenCLSupport(true);		
	}
#ifdef __WXGTK__

	if (regardsParam->GetIsOpenCLSupport())
	{
		openclContext = Regards::OpenCL::COpenCLEngine::CreateInstance();
	}

#endif

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


	frameStart = new MyFrameIntro("Welcome to Regards", "REGARDS V2", wxPoint(50, 50), wxSize(450, 340), this);
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
