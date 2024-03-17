#include <header.h>
#include "MyFrameIntro.h"
#include "IntroTheme.h"
#include "TitleIntro.h"
#include <LibResource.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <FileUtility.h>
#include <wx/progdlg.h>
#include "DownloadFile.h"
using namespace Regards::Introduction;
using namespace Regards::Internet;

#ifndef wxHAS_IMAGES_IN_RESOURCES
#include "../Resource/sample.xpm"
#endif

MyFrameIntro::MyFrameIntro(const wxString& title, const wxString& appTitle, const wxPoint& pos, const wxSize& size,
	ISCannerInterface * mainInterface)
	: wxFrame(nullptr, wxID_ANY, title, pos, size, wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
{
	CThemeScrollBar themeScroll;
	CThemeTree theme;
	CThemeIntroLogo themeIntro;


	SetIcon(wxICON(sample));
	this->mainInterface = mainInterface;
	auto introTheme = new CIntroTheme();


	titleIntro = nullptr;

	if (introTheme != nullptr)
	{
		introTheme->GetScrollTheme(&themeScroll);
		introTheme->GetTreeTheme(&theme);

		myCentralWindow = new CMyCentralWindowIntro(this, wxID_ANY, themeScroll, theme);
		introTheme->GetAboutTexteTheme(&themeIntro);

		titleIntro = new CTitleIntro(this, wxID_ANY, themeIntro);
		titleIntro->SetTitle(appTitle);


		delete introTheme;
	}

	diaporamaTimer = new wxTimer(this, wxTIMER_DIAPORAMA);

	Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MyFrameIntro::OnClose));
	Connect(wxEVT_SIZE, wxSizeEventHandler(MyFrameIntro::on_size));
	Connect(wxTIMER_DIAPORAMA, wxEVT_TIMER, wxTimerEventHandler(MyFrameIntro::OnTimeShowViewer), nullptr, this);
	//CreateStatusBar();

	diaporamaTimer->Start(200, wxTIMER_ONE_SHOT);
}

void MyFrameIntro::on_size(wxSizeEvent& event)
{
	wxSize size = event.GetSize();
	titleIntro->SetSize(0, 0, size.GetWidth(), titleIntro->GetHeight());
	myCentralWindow->SetSize(0, titleIntro->GetHeight(), size.GetWidth(), size.GetHeight() - titleIntro->GetHeight());
}



void MyFrameIntro::NewModelsAvailable()
{
	bool fileExist = false;
	cout << "modelUpdate" << endl;
	wxString localVersion = CLibResource::LoadStringFromResource("LBLMODELHASH", 1);
	wxString line = "";

#ifdef WIN32
	wxString fileHash = CFileUtility::GetResourcesFolderPath() + "\\model\\hash.txt";
#else
	wxString fileHash = CFileUtility::GetResourcesFolderPath() + "/model/hash.txt";
#endif

	if (wxFileExists(fileHash))
	{
		wxFileInputStream input(fileHash);
		wxTextInputStream text(input, wxT("\x09"), wxConvUTF8);
		while (input.IsOk() && !input.Eof())
		{
			line = text.ReadLine();
			break;
		}

		fileExist = true;
	}


	if (!fileExist || localVersion != line)
	{
		/*
		wxProgressDialog dialog("Downloading models ...", "Please wait...", 100, this, wxPD_APP_MODAL | !wxPD_CAN_ABORT | wxPD_SMOOTH);
		wxString serverURL = CLibResource::LoadStringFromResource("LBLWEBSITEMODELDOWNLOAD", 1);
		wxString tempModel = CFileUtility::GetTempFile("model.zip", true);

#ifdef WIN32
		wxString resourcePath = CFileUtility::GetResourcesFolderPath() + "\\model";
#else
		wxString resourcePath = CFileUtility::GetResourcesFolderPath() + "/model";
#endif
		CDownloadFile _checkVersion(serverURL);
		_checkVersion.DownloadFile(&dialog, tempModel, CFileUtility::GetResourcesFolderPathWithExt("ca-bundle.crt"));
		_checkVersion.ExtractZipFiles(tempModel, resourcePath, &dialog, this);

		dialog.Close();
		*/
		wxString pathProgram = "";
#ifdef __APPLE__
		//ExportVideo(this->centralWnd->GetFilename());
		pathProgram = CFileUtility::GetProgramFolderPath() + "/RegardsViewer -p DownloadModels";
		cout << "Path Program" << pathProgram << endl;
#else
#ifdef __WXMSW__
		pathProgram = "RegardsDownloader.exe";
#else
		pathProgram = "./RegardsViewer -p DownloadModels";
		cout << "Path Program" << pathProgram << endl;
#endif

#endif
		wxExecute(pathProgram, wxEXEC_SYNC);
	}


}


void MyFrameIntro::OnTimeShowViewer(wxTimerEvent& event)
{
	NewModelsAvailable();

#ifdef WIN32
	wxString fileHash = CFileUtility::GetResourcesFolderPath() + "\\model\\hash.txt";
#else
	wxString fileHash = CFileUtility::GetResourcesFolderPath() + "/model/hash.txt";
#endif

	if (wxFileExists(fileHash) && mainInterface != nullptr)
			mainInterface->ShowViewer();
	else
	{
		wxMessageBox(wxT("IA model not found. Program can't be started."), wxT("Error"), wxICON_ERROR);
		mainInterface->Close();
	}
}

MyFrameIntro::~MyFrameIntro()
{
	delete(myCentralWindow);
	if (titleIntro != nullptr)
		delete(titleIntro);

	if (diaporamaTimer->IsRunning())
		diaporamaTimer->Stop();
	delete diaporamaTimer;
}

void MyFrameIntro::OnClose(wxCloseEvent& event)
{
	if (mainInterface != nullptr)
		mainInterface->HideAbout();
}
