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

int match(string filename, string templatename)
{
	Mat ref = cv::imread(filename,0);
	Mat tpl = cv::imread(templatename, 0);
	if (ref.empty() || tpl.empty())
	{
		cout << "Error reading file(s)!" << endl;
		return -1;
	}

	//imshow("file", ref);
//	imshow("template", tpl);

	Mat res_32f(ref.rows - tpl.rows + 1, ref.cols - tpl.cols + 1, CV_32FC1);
	matchTemplate(ref, tpl, res_32f, TM_CCORR_NORMED);

	Mat res;
	res_32f.convertTo(res, CV_8U, 255.0);
	//imshow("result", res);

	int size = ((tpl.cols + tpl.rows) / 4) * 2 + 1; //force size to be odd
	adaptiveThreshold(res, res, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, size, -128);
//	imshow("result_thresh", res);

	while (true)
	{
		double minval, maxval, threshold = 0.8;
		Point minloc, maxloc;
		minMaxLoc(res, &minval, &maxval, &minloc, &maxloc);

		if (maxval > 0)
		{
			rectangle(ref, maxloc, Point(maxloc.x + tpl.cols, maxloc.y + tpl.rows), CV_RGB(0, 255, 0), 2);
			floodFill(res, maxloc, 0); //mark drawn blob
		}
		else
			break;
	}

	imshow("final", ref);
	waitKey(0);
	exit(0);
	return 0;
}

void DetectCircle(string filename)
{
	Mat src = cv::imread(filename);
	Mat gray;
	cvtColor(src, gray, COLOR_BGR2GRAY);
//	cv::morphologyEx(gray, gray, 4, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(150, 150)));
//	imshow("detected circles", gray);
//	waitKey();

	//cv::threshold(gray, gray, 0, 255, cv::THRESH_OTSU);
	//imshow("detected circles", gray);
	//waitKey();
	//GaussianBlur(gray, gray, Size(7, 7), 2, 2);


	//medianBlur(gray, gray, 5);
	//resize(gray, gray, Size(300, 300));
	vector<Vec3f> circles;
	
	//cv::HoughCircles(gray, circles, HOUGH_GRADIENT, 2, gray.rows / 4);

	
	HoughCircles(gray, circles, HOUGH_GRADIENT, 19,
		30,  // change this value to detect circles with different distances to each other
		10, 10, 1, 10 // change the last two parameters
	//(min_radius & max_radius) to detect larger circles
	);
	
	//HoughCircles(gray, circles, HOUGH_GRADIENT, 2, 100.0, 30, 150, 1, 10);
	for (size_t i = 0; i < circles.size(); i++)
	{
		Vec3i c = circles[i];
		Point center = Point(c[0], c[1]);
		// circle center
		circle(src, center, 1, Scalar(0, 100, 100), 3, LINE_AA);
		// circle outline
		int radius = c[2];
		circle(src, center, radius, Scalar(255, 0, 255), 3, LINE_AA);
	}
	imshow("detected circles", src);
	waitKey();
	exit(0);
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

#ifdef TEST
	vector<cv::Mat> valuePic;
	vector<cv::Point> value;
	bool isOk = false;
	wxArrayString listFiles;
	wxDir::GetAllFiles("C:\\Users\\jfigu\\Documents\\Regards\\Face", &listFiles);

	for (int i = 0; i < listFiles.size(); i++)
	{
		Mat face = imread(listFiles[i].ToStdString(), 0);
	//	for (int j = 0; j < 4; j++)
	//	{
			Mat copy;
			face.copyTo(copy);
			int angle = 0;

			vector<int> ListAngle;

			CEyeDetect eyeDetect;
			int oldAngle = 360;
			int angleRot = 0;

			Mat thresh;
			threshold(copy, thresh, 50, 255, THRESH_BINARY);



			/*
			// detect the contours on the binary image using cv2.CHAIN_APPROX_NONE
			vector<vector<Point>> contours;
			vector<Vec4i> hierarchy;
			findContours(thresh, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
			//findContours(thresh2, contours4, hierarchy4, RETR_EXTERNAL, CHAIN_APPROX_NONE);

			// draw contours on the original image
			Mat image_copy = copy.clone();
			drawContours(image_copy, contours, -1, Scalar(0, 255, 0), 2);
			imshow("None approximation", image_copy);
			waitKey(0);
			*/

			/*
			int erosion_elem = 0;
			int erosion_type = 2;
			int erosion_size = 0;
			if (erosion_elem == 0) { erosion_type = MORPH_RECT; }
			else if (erosion_elem == 1) { erosion_type = MORPH_CROSS; }
			else if (erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }
			Mat element = getStructuringElement(erosion_type,
				Size(2 * erosion_size + 1, 2 * erosion_size + 1),
				Point(erosion_size, erosion_size));
			dilate(thresh, thresh, element);
			*/
			// Since MORPH_X : 2,3,4,5 and 6
			int morph_elem = 1;
			int morph_size = 5;
			int operation = 2;
			Mat element = getStructuringElement(morph_elem, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
			morphologyEx(thresh, thresh, operation, element);
			erode(thresh, thresh, getStructuringElement(MORPH_RECT, Size(3, 3)));
			/**
			vector<vector<cv::Point>> contours;
			vector<Vec4i> hierarchy;
			findContours(thresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);

			for (int i = 0; i < contours.size(); i++) {
				vector<cv::Point> contour = contours[i];

				std::sort(contour.begin(), contour.end(), [](const cv::Point& a, const cv::Point& b) { return a.x < b.x; });
				cv::Point minX = contour[0];
				cv::circle(thresh, minX, 5, Scalar(255, 0, 255), FILLED);

				cv::Point maxX = contour.back();
				cv::circle(thresh, maxX, 5, Scalar(255, 0, 0), FILLED);

				std::sort(contour.begin(), contour.end(), [](const cv::Point& a, const cv::Point& b) { return a.y < b.y; });
				cv::Point minY = contour[0];
				cv::circle(thresh, minY, 5, Scalar(0, 0, 255), FILLED);

				cv::Point maxY = contour.back();
				cv::circle(thresh, maxY, 5, Scalar(255, 255, 0), FILLED);
			}
			*/

			
			int sizeKernel = 3;
			for (int y = 0; y < thresh.size().height - sizeKernel; y += sizeKernel)
			{
				bool find_black = false;
				int xStart = 0;
				int xEnd = 0;
				for (int x = 0; x < thresh.size().width - sizeKernel; x+=sizeKernel)
				{
					int nbBlack = 0;
					for (int i = 0; i < sizeKernel; i++)
					{
						for (int j = 0; j < sizeKernel; j++)
						{
							int position = (y + i) * thresh.size().width + x + j;
							if (thresh.data[position] == 0)
								nbBlack++;
						}
					}

					if (nbBlack < 4)
					{
						for (int i = 0; i < sizeKernel; i++)
						{
							for (int j = 0; j < sizeKernel; j++)
							{
								int position = (y + i) * thresh.size().width + x + j;
								thresh.data[position] = 255;
							}
						}
					}

				}
			}
			

			/*
			int pos = 0;
			for (int y = 0; y < thresh.size().height; y++)
			{
				bool find_black = false;
				int xStart = 0;
				int xEnd = 0;
				for (int x = 0; x < thresh.size().width; x++, pos++)
				{
					if (thresh.data[pos] == 0 && !find_black)
					{
						xStart = x;
						find_black = true;
					}
					else if (thresh.data[pos] != 0)
					{
						if (find_black)
						{
							int xSize = x - xStart;
							if (xSize < 8)
							{
								for (int i = 0; i < xSize; i++)
								{
									thresh.data[pos - i] = 255;
								}
							}
							find_black = false;
						}
					}
				}
			}
			*/

			imshow("Binary mage", thresh);
			waitKey(0);
			//imwrite("d:\\test_eye.png", thresh);
			/*
		//	int thresh = 100;
			RNG rng(12345);
			vector<vector<Point> > contours;
			vector<Vec4i> hierarchy;
			findContours(thresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
			Mat drawing = Mat::zeros(thresh.size(), CV_8UC3);
			for (size_t i = 0; i < contours.size(); i++)
			{
				Rect roi = boundingRect(contours[i]); // This is a OpenCV function


				Mat mask = Mat::zeros(drawing.size(), CV_8UC1);
				Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
				//drawContours(mask, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
				drawContours(mask, contours, -1, Scalar(255), FILLED);

				// Extract region using mask for region 
				Mat contourRegion;
				Mat imageROI;
				thresh.copyTo(imageROI, mask); // 'image' is the image you used to compute the contours.
				contourRegion = imageROI(roi);

				imshow("Binary contourRegion", contourRegion);
				waitKey(0);

				break;
			}
			*/
			/*
			Mat result;
			int match_method = TM_SQDIFF_NORMED;
			Mat templ = imread("d:\\eye.png", 0);
			matchTemplate(thresh, templ, result, TM_SQDIFF_NORMED);

			normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
			double minVal; double maxVal; Point minLoc; Point maxLoc;
			Point matchLoc;
			minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
			if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED)
			{
				matchLoc = minLoc;
			}
			else
			{
				matchLoc = maxLoc;
			}
			rectangle(thresh, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
			rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
			imshow("image_window", thresh);
			imshow("result_window", result);

 			imshow("Face", result);
			waitKey();
			*/
   			angle = eyeDetect.findEyesSource(thresh);
		//	cv::rotate(face, face, cv::ROTATE_90_CLOCKWISE);

			/*
			if (angle == 90)
				cv::rotate(face, face, cv::ROTATE_90_CLOCKWISE);
			if (angle == 270)
				cv::rotate(face, face, cv::ROTATE_90_COUNTERCLOCKWISE);
			*/
			//imshow("Face", copy);
			//waitKey();

		//}
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

	if (regardsParam->GetIsOpenCLSupport())
	{
		openclContext = Regards::OpenCL::COpenCLEngine::CreateInstance();
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
