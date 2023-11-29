#include <header.h>
#include "TestFrame.h"
#include <window_id.h>
#include <ViewerParam.h>
#include <LibResource.h>

#define TIMER_FPS 0x10001


#ifndef wxHAS_IMAGES_IN_RESOURCES
#ifdef __WXGTK__
#include "../Resource/sample.xpm"
#else
#include "../../Resource/sample.xpm"
#endif
#endif


// constants:
enum
{
	ID_Hello = 1,
	ID_Folder = 2,
	ID_Configuration = 3,
	ID_OpenCL = 4,
	ID_SIZEICONLESS = 5,
	ID_SIZEICONMORE = 6,
	ID_ERASEDATABASE = 7,
	ID_THUMBNAILRIGHT = 8,
	ID_THUMBNAILBOTTOM = 9,
	ID_FACEDETECTION = 10,
	ID_INTERPOLATIONFILTER = 11,
	ID_EXPORT = 12,
	ID_SCANNER = 13,
	ID_CATEGORYDETECTION = 14,
	ID_ASSOCIATE = 15,
	ID_ExternalProgram = 16,
	ID_WINDOWFACE = 17,
	ID_WINDOWFOLDER = 18,
	ID_WINDOWVIEWER = 19,
	ID_WINDOWPICTURE = 20,
	ID_VIDEO = 1018,
	ID_AUDIO = 1019,
	ID_SUBTITLE = 1020,
	WXSCAN_PAGE,
	WXPRINT_PAGE_SETUP,
	WXPRINT_PAGE_SETUP_PS,
#ifdef __WXMAC__
	WXPRINT_PAGE_MARGINS,
#endif
};

CTestFrame::CTestFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(nullptr, FRAMEVIEWER_ID, title, pos, size)
{

	SetIcon(wxICON(sample));

	bool openFirstFile = true;
	videoWindow = new CVideoControlSoft(nullptr, this, nullptr);
	bitmapWindowRender = new CBitmapWnd3D(this, wxUSE_ANY);
	bitmapWindowRender->SetBitmapRenderInterface(videoWindow);

	auto menuFile = new wxMenu;

	wxString labelDecreaseIconSize = CLibResource::LoadStringFromResource(L"labelDecreaseIconSize", 1);
	//L"Decrease Icon Size";
	wxString labelDecreaseIconSize_link = CLibResource::LoadStringFromResource(L"labelDecreaseIconSize_link", 1);
	//L"&Decrease Icon Size";
	wxString labelEnlargeIconSize = CLibResource::LoadStringFromResource(L"labelEnlargeIconSize", 1);
	//L"Enlarge Icon Size";
	wxString labelEnlargeIconSize_link = CLibResource::LoadStringFromResource(L"labelEnlargeIconSize_link", 1);
	//L"&Enlarge Icon Size";
	wxString labelConfiguration = CLibResource::LoadStringFromResource(L"labelConfiguration", 1); //L"Configuration";
	wxString labelConfiguration_link = CLibResource::LoadStringFromResource(L"labelConfiguration_link", 1);
	//L"&Configuration";
	wxString labelEraseDataBase = CLibResource::LoadStringFromResource(L"labelEraseDataBase", 1); //L"Erase Database";
	wxString labelEraseDataBase_link = CLibResource::LoadStringFromResource(L"labelEraseDataBase_link", 1);
	//L"&Erase Database";
	wxString labelThumbnailRight = CLibResource::LoadStringFromResource(L"labelThumbnailRight", 1); //L"Right Position";
	wxString labelThumbnailRight_link = CLibResource::LoadStringFromResource(L"labelThumbnailRight_link", 1);
	//L"&Right Position";
	wxString labelThumbnailBottom = CLibResource::LoadStringFromResource(L"labelThumbnailBottom", 1);
	//L"Bottom Position";
	wxString labelThumbnailBottom_link = CLibResource::LoadStringFromResource(L"labelThumbnailBottom_link", 1);
	//L"&Bottom Position";
	wxString labelPageSetup = CLibResource::LoadStringFromResource(L"labelPageSetup", 1); //L"Page setup";
	wxString labelPageSetup_link = CLibResource::LoadStringFromResource(L"labelPageSetup_link", 1); //L"Page Set&up...";
	wxString labelPageMargins = CLibResource::LoadStringFromResource(L"labelPageMargins", 1); // L"Page margins";
	wxString labelPageMargins_link = CLibResource::LoadStringFromResource(L"labelPageMargins_link", 1);
	//L"Page margins...";
	wxString labelFile = CLibResource::LoadStringFromResource(L"labelFile", 1); //L"&File";
	wxString labelParameter = CLibResource::LoadStringFromResource(L"labelParameter", 1); //L"&Parameter";
	wxString labelSizeIcon = CLibResource::LoadStringFromResource(L"labelSizeIcon", 1); //L"&Icon Size";
	wxString labelWindow = CLibResource::LoadStringFromResource(L"labelWindow", 1); //L"&Icon Size";
	wxString labelThumbnail = CLibResource::LoadStringFromResource(L"labelThumbnail", 1); //L"&Thumbnail";
	wxString labelHelp = CLibResource::LoadStringFromResource(L"labelHelp", 1); //L"&Help";

	auto menuWindow = new wxMenu;
	wxString labelWindowFace = CLibResource::LoadStringFromResource(L"labelWindowFace", 1);
	wxString labelWindowFaceLink = CLibResource::LoadStringFromResource(L"labelWindowFaceLink", 1);
	wxString labelWindowFolder = CLibResource::LoadStringFromResource(L"labelWindowFolder", 1);
	wxString labelWindowFolderLink = CLibResource::LoadStringFromResource(L"labelWindowFolderLink", 1);
	wxString labelWindowViewer = CLibResource::LoadStringFromResource(L"labelWindowViewer", 1);
	wxString labelWindowViewerLink = CLibResource::LoadStringFromResource(L"labelWindowViewerLink", 1);
	wxString labelWindowPicture = CLibResource::LoadStringFromResource(L"labelWindowPicture", 1);
	wxString labelWindowPictureLink = CLibResource::LoadStringFromResource(L"labelWindowPictureLink", 1);

	menuWindow->Append(ID_WINDOWFACE, labelWindowFaceLink, labelWindowFace);
	menuWindow->Append(ID_WINDOWFOLDER, labelWindowFolderLink, labelWindowFolder);
	menuWindow->Append(ID_WINDOWVIEWER, labelWindowViewerLink, labelWindowViewer);
	menuWindow->Append(ID_WINDOWPICTURE, labelWindowPictureLink, labelWindowPicture);

	auto menuSizeIcon = new wxMenu;
	menuSizeIcon->Append(ID_SIZEICONLESS, labelDecreaseIconSize_link, labelDecreaseIconSize);
	menuSizeIcon->Append(ID_SIZEICONMORE, labelEnlargeIconSize_link, labelEnlargeIconSize);

	menuFile->Append(ID_EXPORT, "&Export", "Export");
#ifdef WIN32
	menuFile->Append(ID_ASSOCIATE, "&Associate", "Associate");
#endif

	menuFile->AppendSeparator();
	menuFile->Append(WXPRINT_PAGE_SETUP, labelPageSetup_link, labelPageSetup);
#ifdef __WXMAC__
	menuFile->Append(WXPRINT_PAGE_MARGINS, labelPageMargins_link, labelPageMargins);
#endif
	menuFile->Append(wxID_PRINT, wxT("&Print..."), wxT("Print"));
	menuFile->AppendSeparator();
	menuFile->Append(ID_Configuration, labelConfiguration_link, labelConfiguration);
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);
	auto menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	menuHelp->Append(wxID_HELP);
	auto menuBar = new wxMenuBar;
	menuBar->Append(menuFile, labelFile);
	menuBar->Append(menuSizeIcon, labelSizeIcon);
	menuBar->Append(menuWindow, labelWindow);
	menuBar->Append(menuHelp, labelHelp);
	wxFrameBase::SetMenuBar(menuBar);

	wxWindow::SetLabel(wxT("Regards Viewer"));

	videoWindow->PlayFirstMovie(false);
	bitmapWindowRender->Show(true);
	videoWindow->ShrinkVideo();

	Connect(wxTIMER_DIAPORAMA, wxEVT_TIMER, wxTimerEventHandler(CTestFrame::OnStop), nullptr, this);
	Connect(wxEVT_SIZE, wxSizeEventHandler(CTestFrame::OnSize));

	stopMovie = new wxTimer(this, wxTIMER_DIAPORAMA);
}

void CTestFrame::OnStop(wxTimerEvent& event)
{
	videoWindow->QuitMovie();
	wxSleep(1);
	videoWindow->PlayMovie(filename, true);
	stopMovie->StartOnce(10000);
}

void CTestFrame::OnSize(wxSizeEvent& event)
{
	int _width = event.GetSize().GetX();
	int _height = event.GetSize().GetY();

	bitmapWindowRender->SetSize(0, 0, _width, _height);
	bitmapWindowRender->Refresh();

}


void CTestFrame::PlayMovie(const wxString& openfile)
{
	videoWindow->PlayMovie(openfile, true);
	filename = openfile;
	stopMovie->StartOnce(10000);
}

void CTestFrame::StopMovie()
{
	videoWindow->QuitMovie();
}

CTestFrame::~CTestFrame()
{

	if (videoWindow != nullptr)
		delete(videoWindow);

	if (bitmapWindowRender != nullptr)
		delete(bitmapWindowRender);


}
