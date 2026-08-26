#include "header.h"
#include "Pane.h"
using namespace Regards::Window;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPane::CPane(wxWindow* parent, wxWindowID id, CPaneInterface* paneInterface, const int& idPane, const CThemePane& theme,
             const bool& refreshButton) :
	CWindowMain("CPane", parent, id)
{
	titleBar = nullptr;
	hWndOther = nullptr;
	hMainWndOther = nullptr;
	titleBarVisible = true;

	this->paneInterface = paneInterface;
	this->idPane = idPane;
	this->themePane = theme;

	titleBar = std::make_unique<CTitleBar>(this, wxID_ANY, this);
	titleBar->SetRefresh(refreshButton);
	titleBar->SetTheme(const_cast<CThemeTitleBar*>(&theme.themeTitle));

   
	//titleBar->Show(false)
	Connect(wxEVT_MOTION, wxMouseEventHandler(CPane::OnMouseMove));
	Connect(wxEVENT_REFRESHDATA, wxCommandEventHandler(CPane::RefreshData));
}

void CPane::RefreshData(wxCommandEvent& event)
{
	if (hWndOther != nullptr)
	{
		wxCommandEvent evt(wxEVENT_REFRESHDATA);
		evt.SetExtraLong(1);
		hWndOther->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CPane::UpdateScreenRatio()
{
	if (titleBar)
		titleBar->UpdateScreenRatio();
	if (hMainWndOther != nullptr)
		hMainWndOther->UpdateScreenRatio();
	this->Resize();
}

void CPane::ShowOtherWindow()
{
	if (hMainWndOther != nullptr)
		hMainWndOther->Show(true);
	if (hWndOther != nullptr)
		hWndOther->Show(true);
}

void CPane::SetClosable(const bool& value)
{
	if (titleBar)
		titleBar->SetClosable(value);
}

int CPane::GetTitleHeight()
{
	return themePane.GetHeight();
}

void CPane::Resize()
{
	//printf("CPane Resize size x : %d y : %d \n", GetWindowWidth(), GetWindowHeight());
	if (titleBarVisible)
	{
		titleBar->SetSize(0, 0, GetWindowWidth(), titleBar->GetWindowHeight());
		if (hWndOther != nullptr)
		{
			int height = GetWindowHeight() - titleBar->GetWindowHeight();
			hWndOther->SetSize(0, titleBar->GetWindowHeight(), GetWindowWidth(), height);
			if (hMainWndOther != nullptr)
				hMainWndOther->Refresh();
		}
	}
	else if (hWndOther != nullptr)
	{
		hWndOther->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
		if (hMainWndOther != nullptr)
			hMainWndOther->Refresh();
	}
}

void CPane::OnMouseMove(wxMouseEvent& event)
{
	//int xPos = event.GetX();
	//int yPos = event.GetY();
}


void CPane::SetTitleBarVisibility(const bool& visible)
{
	if (titleBar)
	{
		titleBarVisible = visible;
		titleBar->Show(visible);
		Resize();
	}
}

void CPane::ClosePane()
{
	if(paneInterface)
		paneInterface->ClosePane(idPane);
}

void CPane::RefreshPane()
{
	if (paneInterface)
		paneInterface->RefreshPane(idPane);
}

void CPane::SetTitle(const wxString& title)
{
	if (titleBar)
		titleBar->SetTitle(title);
}

int CPane::SetTooltipText(const wxString& tooltip)
{
	if (titleBar)
		return titleBar->SetTooltipText(tooltip);
	return 0;
}
