#include "header.h"
#include "Pane.h"
using namespace Regards::Window;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPane::CPane(wxWindow* parent, wxWindowID id, CPaneInterface * paneInterface, const int &idPane, const CThemePane & theme, const bool &refreshButton) :
CWindowMain("CPane",parent, id)
{
	titleBar = nullptr;
    hWndOther = nullptr;
	hMainWndOther = nullptr;
	titleBarVisible = true;
	this->paneInterface = paneInterface;
	titleBar = new CTitleBar(this, wxID_ANY, this);
	titleBar->SetRefresh(refreshButton);
	this->idPane = idPane;
	this->themePane = themePane;
	//titleBar->Show(false)
	Connect(wxEVT_MOTION, wxMouseEventHandler(CPane::OnMouseMove));
	Connect(wxEVENT_REFRESHDATA, wxCommandEventHandler(CPane::RefreshData));
	//Connect(wxEVT_SHOW, wxShowEventHandler(CPane::OnShow));
	//Connect(wxEVT_PAINT, wxPaintEventHandler(CPane::OnPaint));
	//Connect(wxEVT_IDLE, wxIdleEventHandler(CPane::OnIdle));
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

void CPane::OnShow(wxShowEvent& event)
{
	if (hWndOther != nullptr)
	{
		if (IsShown())
		{
			hWndOther->Show(true);
		}
		else
		{
			hWndOther->Show(false);
		}
	}
}
void CPane::OnPaint(wxPaintEvent& event)
{
    int width = GetWindowWidth();
    int height = GetWindowHeight();
    if(width == 0 || height == 0)
        return;
    
	titleBar->Redraw();
	if (hWndOther != nullptr)
	{
		hWndOther->Update();
	}
}

void CPane::OnIdle(wxIdleEvent& evt)
{
#if defined(WIN32) && defined(_DEBUG)
	wxString toShow = "CPane OnIdle size x : " + to_string(GetWindowWidth()) + " y : " + to_string(GetWindowHeight()) + "\n";
	OutputDebugString(toShow.ToStdWstring().c_str());
	if (hWndOther != nullptr)
	{
		wxSize otherSize = hWndOther->GetSize();
		wxString toShow = "CPane OnIdle otherSize size x : " + to_string(otherSize.x) + " y : " + to_string(otherSize.y) + "\n";
		OutputDebugString(toShow.ToStdWstring().c_str());
	}
#endif
}

CPane::~CPane()
{
	delete(titleBar);
}

void CPane::UpdateScreenRatio()
{
    titleBar->UpdateScreenRatio();
    if(hMainWndOther != nullptr)
        hMainWndOther->UpdateScreenRatio();
    this->Resize();
}

void CPane::SetClosable(const bool &value)
{
	titleBar->SetClosable(value);
}

int CPane::GetTitleHeight()
{
	return themePane.GetHeight();
}

void CPane::Resize()
{
	printf("CPane Resize size x : %d y : %d \n", GetWindowWidth(), GetWindowHeight());
	if (titleBarVisible)
	{
		titleBar->SetSize(0, 0, GetWindowWidth(), titleBar->GetWindowHeight());
		if (hWndOther != nullptr)
		{
            int height =  GetWindowHeight() - titleBar->GetWindowHeight();
			hWndOther->SetSize(0, titleBar->GetWindowHeight(), GetWindowWidth(), height);
		}
	}
	else if (hWndOther != nullptr)
	{
		hWndOther->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
	}
	Refresh();
	Update();
}

void CPane::OnMouseMove(wxMouseEvent& event)
{
	//int xPos = event.GetX();
	//int yPos = event.GetY();
}


void CPane::SetTitleBarVisibility(const bool &visible)
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

void CPane::SetTitle(const wxString & title)
{
	titleBar->SetTitle(title);
}

int CPane::SetTooltipText(const wxString & tooltip)
{
	return titleBar->SetTooltipText(tooltip);
}
