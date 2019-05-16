#include <header.h>
#include "PanelWithClickToolbar.h"
using namespace Regards::Window;

#define PANE_WITHCLICKTOOLBAR 1

CPanelWithClickToolbar::CPanelWithClickToolbar(wxWindow* parent, const wxString & windowName, wxWindowID id, const CThemePane & themePane, const CThemeToolbar & themeToolbar, const wxString & paneLibelle, const bool & isPanelVisible, const bool& refreshButton, const bool& vertical)
	: CWindowMain(windowName,parent,id)
{
	paneWindow = nullptr;
	mainWindow = nullptr;
	clickWindow = nullptr;

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

void CPanelWithClickToolbar::SetWindow(CWindowMain * windowMain)
{
	mainWindow = windowMain;
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

void CPanelWithClickToolbar::ClickShowButton(const int &id)
{
	switch (id)
	{
	case PANE_WITHCLICKTOOLBAR:
		{
			isPanelVisible = true;
			clickWindow->Show(false);
			paneWindow->Show(true);
			wxCommandEvent* event = new wxCommandEvent(wxEVENT_RESIZE);
			wxQueueEvent(this->GetParent(), event);
		}
		break;
	}

}

void CPanelWithClickToolbar::RefreshPane(const int& id)
{
	switch (id)
	{
		case PANE_WITHCLICKTOOLBAR:
		{
			wxCommandEvent* event = new wxCommandEvent(wxEVENT_REFRESH);
			wxQueueEvent(this->GetParent(), event);
		}
		break;
	}
}


void CPanelWithClickToolbar::ClosePane(const int &id)
{
	switch (id)
	{
	case PANE_WITHCLICKTOOLBAR:
		{
			isPanelVisible = false;
			paneWindow->Show(false);
			clickWindow->Show(true);
			wxCommandEvent* event = new wxCommandEvent(wxEVENT_RESIZE);
			wxQueueEvent(this->GetParent(), event);
		}
		break;
	}

	Resize();
}

void CPanelWithClickToolbar::Resize()
{
	if(clickWindow->IsShown())
	{
		clickWindow->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
	}
	else if(paneWindow->IsShown())
	{
		paneWindow->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
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

	if(refresh)
	{
		wxCommandEvent* event = new wxCommandEvent(wxEVENT_RESIZE);
		wxQueueEvent(this->GetParent(), event);
	}
}

void CPanelWithClickToolbar::ShowPanel()
{
	this->Show(true);

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


