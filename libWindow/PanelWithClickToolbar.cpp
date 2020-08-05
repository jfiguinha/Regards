#include <header.h>
#include "PanelWithClickToolbar.h"
#include "MainTheme.h"
#include "MainThemeInit.h"
using namespace Regards::Window;



CPanelWithClickToolbar::CPanelWithClickToolbar(wxWindow* parent, const wxString & windowName, wxWindowID id, const CThemePane & themePane, const CThemeToolbar & themeToolbar, const wxString & paneLibelle, const bool & isPanelVisible, const bool& refreshButton, const bool& vertical)
	: CWindowMain(windowName,parent,id)
{
	paneWindow = nullptr;
	mainWindow = nullptr;
	clickWindow = nullptr;
	this->isVertical = isVertical;
	this->isPanelVisible = isPanelVisible;

	//----------------------------------------------------------------------------------------
	//Panel Thumbnail Video
	//----------------------------------------------------------------------------------------
	paneWindow = new CPane(this, wxID_ANY, this, PANE_WITHCLICKTOOLBAR, themePane, refreshButton);
	paneWindow->SetTitle(paneLibelle);
	//vertical = true;
	clickWindow = new CClickToolbar(this, wxID_ANY, themeToolbar, this, PANE_WITHCLICKTOOLBAR, vertical);

	//#define wxEVENT_SHOWPANE 214
	//#define wxEVENT_CLOSEPANE 215
	Connect(wxEVENT_SHOWPANE, wxCommandEventHandler(CPanelWithClickToolbar::ShowPane));
	Connect(wxEVENT_CLOSEPANE, wxCommandEventHandler(CPanelWithClickToolbar::ClosePane));
	Connect(wxEVENT_REFRESHDATA, wxCommandEventHandler(CPanelWithClickToolbar::RefreshData));

	if(isPanelVisible)
	{
		clickWindow->Show(false);
		paneWindow->Show(true);
	}
	else
	{
		clickWindow->Show(true);
		paneWindow->Show(false);
	}
}

void CPanelWithClickToolbar::RefreshData(wxCommandEvent& event)
{
	if (paneWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_REFRESHDATA);
		evt.SetExtraLong(1);
		paneWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CPanelWithClickToolbar::ShowPane(wxCommandEvent& event)
{
	ClickShowButton(PANE_WITHCLICKTOOLBAR);
}

void CPanelWithClickToolbar::ClosePane(wxCommandEvent& event)
{
	ClosePane(PANE_WITHCLICKTOOLBAR);
}

wxWindow * CPanelWithClickToolbar::GetPaneWindow()
{
	return paneWindow;
}

wxWindow * CPanelWithClickToolbar::GetWindow()
{
	if (isPanelVisible)
		return paneWindow;
	else
		return clickWindow;
}


CPanelWithClickToolbar * CPanelWithClickToolbar::CreatePanel(wxWindow * parent, const wxString &panelLabel, const wxString &windowName, const bool &isVisible, const int &idPanel, const bool &isVertical, const bool &refreshButton)
{
	CMainTheme * viewerTheme = CMainThemeInit::getInstance();
	CThemePane theme;
	viewerTheme->GetPaneTheme(&theme);
	CThemeToolbar themeClickInfosToolbar;
	viewerTheme->GetClickToolbarTheme(&themeClickInfosToolbar);
	CPanelWithClickToolbar * panel = new CPanelWithClickToolbar(parent, windowName, idPanel, theme, themeClickInfosToolbar, panelLabel, isVisible, refreshButton, isVertical);
	return panel;
}

void CPanelWithClickToolbar::SetWindow(CWindowMain * windowMain)
{
	mainWindow = windowMain;
	mainWindow->Reparent(paneWindow);
	paneWindow->SetOtherWindow(windowMain);
}

CPanelWithClickToolbar::~CPanelWithClickToolbar()
{
	delete(clickWindow);
	delete(paneWindow);
}

void CPanelWithClickToolbar::UpdateScreenRatio()
{
	clickWindow->UpdateScreenRatio();
	mainWindow->UpdateScreenRatio();
	paneWindow->UpdateScreenRatio();
    this->Resize();
}

void CPanelWithClickToolbar::ClickShowButton(const int &id, const int &refresh)
{
	switch (id)
	{
	case PANE_WITHCLICKTOOLBAR:
		{
			isPanelVisible = true;
			clickWindow->Show(false);
			paneWindow->Show(true);
			paneWindow->ShowOtherWindow();
			wxCommandEvent* event = new wxCommandEvent(wxEVENT_RESIZE);
			event->SetId(this->GetId());
			event->SetInt(1);
			event->SetExtraLong(refresh);
			wxQueueEvent(this->GetParent(), event);
		}
		break;
	}
	Resize();
}

void CPanelWithClickToolbar::RefreshPane(const int& id)
{
	switch (id)
	{
		case PANE_WITHCLICKTOOLBAR:
		{
			wxCommandEvent* event = new wxCommandEvent(wxEVENT_REFRESHDATA);
			event->SetId(this->GetId());
			wxQueueEvent(this->GetParent(), event);
		}
		break;
	}
}


void CPanelWithClickToolbar::ClosePane(const int &id, const int &refresh)
{
	switch (id)
	{
	case PANE_WITHCLICKTOOLBAR:
		{
			isPanelVisible = false;
			paneWindow->Show(false);
			clickWindow->Show(true);
			wxCommandEvent* event = new wxCommandEvent(wxEVENT_RESIZE);
			event->SetId(this->GetId());
			event->SetInt(0);
			event->SetExtraLong(refresh);
			wxQueueEvent(this->GetParent(), event);
		}
		break;
	}

	Resize();
}

void CPanelWithClickToolbar::Resize()
{
	if (clickWindow->IsShown())
	{
		clickWindow->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
		clickWindow->Refresh();

	}
	else if (paneWindow->IsShown())
	{
		paneWindow->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
		paneWindow->Refresh();
	}
}

int CPanelWithClickToolbar::GetHeight()
{
	if(!isPanelVisible)
		return clickWindow->GetHeight();
	return mainWindow->GetHeight() + paneWindow->GetTitleHeight();
}

bool CPanelWithClickToolbar::IsPanelVisible()
{
	return isPanelVisible;
}

int CPanelWithClickToolbar::GetWidth()
{
	if (!isPanelVisible)
		return clickWindow->GetWidth();
	return mainWindow->GetWidth();
}

void CPanelWithClickToolbar::HidePanel(const bool &refresh)
{
	this->Show(false);
	clickWindow->Show(false);
	paneWindow->Show(false);
	/*
	if(refresh)
	{
		wxCommandEvent* event = new wxCommandEvent(wxEVENT_RESIZE);
		wxQueueEvent(this->GetParent(), event);
	}
	*/
}

void CPanelWithClickToolbar::ShowPanel()
{
	this->Show(true);

	if(isPanelVisible)
	{
		clickWindow->Show(false);
		paneWindow->Show(true);
		paneWindow->Resize();
	}
	else
	{
		clickWindow->Show(true);
		paneWindow->Show(false);
		clickWindow->Resize();
	}
}


