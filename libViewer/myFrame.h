#pragma once
#include <header.h>
#include <wx/aui/framemanager.h>
#include <WindowManager.h>
#include "MainThemeInit.h"
#include "MainTheme.h"
#include "PanelPhotoWnd.h"
using namespace Regards::Window;

class MyFrame : public wxFrame {

	// constants:
	enum
	{
		ID_SHOWLEFT = 1,
		ID_SHOWRIGHT = 2,
		ID_SHOWTOP = 3,
		ID_SHOWBOTTOM = 4,
		ID_HIDELEFT = 5,
		ID_HIDERIGHT = 6,
		ID_HIDETOP = 7,
		ID_HIDEBOTTOM = 8,
	};

public:
	MyFrame(wxWindow* parent) : wxFrame(parent, -1, _("wxAUI Test"),
		wxDefaultPosition, wxSize(800, 600),
		wxDEFAULT_FRAME_STYLE)
	{
		CMainTheme * viewerTheme = new CMainTheme();
		CMainThemeInit::Initialize(viewerTheme);
		CThemeSplitter theme;
		viewerTheme->GetSplitterTheme(&theme);
		windowManager = new CWindowManager(this, wxID_ANY, theme);

		
		wxRect rect;
		/*
		// create several text controls
		wxTextCtrl* text1 = new wxTextCtrl(this, -1, _("Pane 1 - sample text"),
			wxDefaultPosition, wxSize(200, 150),
			wxNO_BORDER | wxTE_MULTILINE);

		wxTextCtrl* text2 = new wxTextCtrl(this, -1, _("Pane 2 - sample text"),
			wxDefaultPosition, wxSize(200, 150),
			wxNO_BORDER | wxTE_MULTILINE);

		wxTextCtrl* text3 = new wxTextCtrl(this, -1, _("Main content window"),
			wxDefaultPosition, wxSize(200, 150),
			wxNO_BORDER | wxTE_MULTILINE);

		wxTextCtrl* text4 = new wxTextCtrl(this, -1, _("Pane 3 - sample text"),
			wxDefaultPosition, wxSize(200, 150),
			wxNO_BORDER | wxTE_MULTILINE);

		wxTextCtrl* text5 = new wxTextCtrl(this, -1, _("Pane 5 - sample text"),
			wxDefaultPosition, wxSize(200, 150),
			wxNO_BORDER | wxTE_MULTILINE);
		*/
		// add the panes to the manager
		windowManager->AddPanel(CreateWindow(), Pos::wxLEFT, false, 0, rect, "test 1", "TextTest1", true, WM_USER + 2000);
		//windowManager->AddWindow(text1, Pos::wxLEFT, false, 0, rect);
		//windowManager->AddWindow(text2, Pos::wxBOTTOM, false, 0, rect);
		windowManager->AddPanel(CreateWindow(), Pos::wxBOTTOM, false, 0, rect, "test 2", "TextTes2", true, WM_USER + 2001);
		windowManager->AddWindow(CreateWindow(), Pos::wxCENTRAL, false, 0, rect);
		windowManager->AddPanel(CreateWindow(), Pos::wxTOP, false, 0, rect, "test 3", "TextTes3", true, WM_USER + 2002);
		windowManager->AddPanel(CreateWindow(), Pos::wxRIGHT, false, 0, rect, "test 4", "TextTes4", true, WM_USER + 2003);

		/*
		windowManager->AddPanel(CreateWindow(), Pos::wxLEFT, false, 0, rect, "test 1", "TextTest1", true, WM_USER + 2000);
		windowManager->AddPanel(CreateWindow(), Pos::wxBOTTOM, false, 0, rect, "test 2", "TextTes2", true, WM_USER + 2001);
		windowManager->AddWindow(text3, Pos::wxCENTRAL, false, 0, rect);
		windowManager->AddPanel(CreateWindow(), Pos::wxTOP, false, 0, rect, "test 3", "TextTes3", true, WM_USER + 2002);
		windowManager->AddPanel(CreateWindow(), Pos::wxRIGHT, false, 0, rect, "test 4", "TextTes4", true, WM_USER + 2003);
		*/
		Connect(wxEVT_SIZE, wxSizeEventHandler(MyFrame::OnSize));


		wxMenu *menuShow = new wxMenu;
		menuShow->Append(ID_SHOWLEFT, "&Show Left", "Show Left");
		menuShow->Append(ID_SHOWRIGHT, "&Show Right", "Show Right");
		menuShow->Append(ID_SHOWTOP, "&Show Top", "Show Top");
		menuShow->Append(ID_SHOWBOTTOM, "&Show Bottom", "Show Bottom");

		wxMenu *menuHide = new wxMenu;
		menuHide->Append(ID_HIDELEFT, "&Hide Left", "Hide Left");
		menuHide->Append(ID_HIDERIGHT, "&Hide Right", "Hide Right");
		menuHide->Append(ID_HIDETOP, "&Hide Top", "Hide Top");
		menuHide->Append(ID_HIDEBOTTOM, "&Hide Bottom", "Hide Bottom");

		wxMenuBar *menuBar = new wxMenuBar;
		menuBar->Append(menuShow, "Show");
		menuBar->Append(menuHide, "Hide");

		SetMenuBar(menuBar);


		Connect(ID_SHOWLEFT, wxEVT_MENU, wxCommandEventHandler(MyFrame::OnShowLeft));
		Connect(ID_SHOWRIGHT, wxEVT_MENU, wxCommandEventHandler(MyFrame::OnShowRight));
		Connect(ID_SHOWTOP, wxEVT_MENU, wxCommandEventHandler(MyFrame::OnShowTop));
		Connect(ID_SHOWBOTTOM, wxEVT_MENU, wxCommandEventHandler(MyFrame::OnShowBottom));

		Connect(ID_HIDELEFT, wxEVT_MENU, wxCommandEventHandler(MyFrame::OnHideLeft));
		Connect(ID_HIDERIGHT, wxEVT_MENU, wxCommandEventHandler(MyFrame::OnHideRight));
		Connect(ID_HIDETOP, wxEVT_MENU, wxCommandEventHandler(MyFrame::OnHideTop));
		Connect(ID_HIDEBOTTOM, wxEVT_MENU, wxCommandEventHandler(MyFrame::OnHideBottom));
	}

	void OnShowLeft(wxCommandEvent& event)
	{
		windowManager->ShowWindow(Pos::wxLEFT);
	}

	void OnShowRight(wxCommandEvent& event)
	{
		windowManager->ShowWindow(Pos::wxRIGHT);
	}

	void OnShowTop(wxCommandEvent& event)
	{
		windowManager->ShowWindow(Pos::wxTOP);
	}

	void OnShowBottom(wxCommandEvent& event)
	{
		windowManager->ShowWindow(Pos::wxBOTTOM);
	}

	void OnHideLeft(wxCommandEvent& event)
	{
		windowManager->HideWindow(Pos::wxLEFT);
	}

	void OnHideRight(wxCommandEvent& event)
	{
		windowManager->HideWindow(Pos::wxRIGHT);
	}

	void OnHideTop(wxCommandEvent& event)
	{
		windowManager->HideWindow(Pos::wxTOP);
	}

	void OnHideBottom(wxCommandEvent& event)
	{
		windowManager->HideWindow(Pos::wxBOTTOM);
	}

	CWindowMain * CreateWindow()
	{
		CMainTheme * viewerTheme = new CMainTheme();
		CPanelPhotoWnd * panelPhotoWnd = nullptr;
		//----------------------------------------------------------------------------------------
		//Panel Thumbnail Video
		//----------------------------------------------------------------------------------------
		if (viewerTheme != nullptr)
		{
			panelPhotoWnd = new CPanelPhotoWnd(this, CRITERIAFOLDERWINDOWID);
		}
		return panelPhotoWnd;
	}

	void OnSize(wxSizeEvent& event)
	{
		wxSize size = event.GetSize();
		windowManager->SetSize(0, 0, size.GetWidth() - 20, size.GetHeight() - 60);

	}


	~MyFrame()
	{
		// deinitialize the frame manager
		windowManager->UnInit();
	}

private:
	CWindowManager * windowManager;
};