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
	titleBar = new CTitleBar(this, wxID_ANY, this);
	titleBar->SetRefresh(refreshButton);
	this->idPane = idPane;
	this->themePane = theme;
    titleBar->SetTheme((CThemeTitleBar*)&theme.themeTitle);
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


CPane::~CPane()
{
	delete(titleBar);
}

void CPane::UpdateScreenRatio()
{
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
	titleBar->SetClosable(value);
}

int CPane::GetTitleHeight()
{
	return themePane.GetHeight();
}

void CPane::Resize()
{
#ifdef _DEBUG
#ifdef WIN32
	TCHAR temp[255];
	wsprintf(temp, L"CPane Resize size x : %d y : %d \n", GetWindowWidth(), GetWindowHeight());
	OutputDebugString(temp);
#endif
#endif

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
	titleBarVisible = visible;
	titleBar->Show(visible);
	Resize();
}

void CPane::ClosePane()
{
	paneInterface->ClosePane(idPane);
}

void CPane::RefreshPane()
{
	paneInterface->RefreshPane(idPane);
}

void CPane::SetTitle(const wxString& title)
{
	titleBar->SetTitle(title);
}

int CPane::SetTooltipText(const wxString& tooltip)
{
	return titleBar->SetTooltipText(tooltip);
}
