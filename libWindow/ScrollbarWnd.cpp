#include "header.h"
#include "ScrollbarWnd.h"
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarVerticalWnd.h"
#include "MainThemeInit.h"
#include "MainTheme.h"
using namespace Regards::Window;

#define TIMER_HIDE 1

CScrollbarWnd::CScrollbarWnd(wxWindow* parent, CWindowMain * centralWindow, wxWindowID id, const wxString & windowName)
	: CWindowMain(windowName, parent, id)
{
	wxRect rect;
	DefaultConstructor();
	this->centralWindow->SetWindow(centralWindow);
	windowManager->AddWindow(centralWindow, Pos::wxCENTRAL, false, 0, rect, wxID_ANY, false);
	centralWindow->Reparent(this);

	
}

CScrollbarWnd::CScrollbarWnd(wxWindow* parent, CWindowOpenGLMain * centralWindow, wxWindowID id, const wxString & windowName)
	: CWindowMain(windowName, parent, id)
{
	wxRect rect;

	DefaultConstructor();
	this->centralWindow->SetWindow(centralWindow);
	windowManager->AddWindow(centralWindow, Pos::wxCENTRAL, false, 0, rect, wxID_ANY, false);
	centralWindow->Reparent(this);
}


void CScrollbarWnd::DefaultConstructor()
{
	wxRect rect;
	   
	CMainTheme * viewerTheme = CMainThemeInit::getInstance();
	centralWindow = new CWindowToAdd();

	if (viewerTheme != nullptr)
	{
		CThemeSplitter theme;
		viewerTheme->GetSplitterTheme(&theme);
		windowManager = new CWindowManager(this, wxID_ANY, theme);
	}

	scrollHorizontal = nullptr;
	scrollVertical = nullptr;
	//scrollInterface = nullptr;
	showV = true;
	showH = true;
	posHauteur = 0;
	posLargeur = 0;
	controlHeight = 0;
	controlWidth = 0;
	loadingTimer = new wxTimer(this, TIMER_HIDE);


	if (viewerTheme != nullptr)
	{
		CThemeScrollBar theme;
		viewerTheme->GetScrollTheme(&theme);
		scrollHorizontal = new CScrollbarHorizontalWnd("ScrollbarHorizontal", windowManager, wxID_ANY, theme);
		scrollVertical = new CScrollbarVerticalWnd("ScrollbarVertical", windowManager, wxID_ANY, theme);
	}
	windowManager->AddWindow(scrollHorizontal, Pos::wxBOTTOM, true, GetBarHeight(), rect, wxID_ANY, false);
	windowManager->AddWindow(scrollVertical, Pos::wxRIGHT, true, GetBarWidth(), rect, wxID_ANY, false);

	Connect(wxEVENT_REFRESHDATA, wxCommandEventHandler(CScrollbarWnd::RefreshData));
	Connect(wxEVENT_LEFTPOSITION, wxCommandEventHandler(CScrollbarWnd::OnLeftPosition));
	Connect(wxEVENT_TOPPOSITION, wxCommandEventHandler(CScrollbarWnd::OnTopPosition));
	Connect(wxEVENT_SETCONTROLSIZE, wxCommandEventHandler(CScrollbarWnd::OnControlSize));
	Connect(wxEVENT_SETPOSITION, wxCommandEventHandler(CScrollbarWnd::OnSetPosition));

	Connect(wxEVENT_MOVELEFT, wxCommandEventHandler(CScrollbarWnd::OnMoveLeft));
	Connect(wxEVENT_MOVERIGHT, wxCommandEventHandler(CScrollbarWnd::OnMoveRight));
	Connect(wxEVENT_MOVETOP, wxCommandEventHandler(CScrollbarWnd::OnMoveTop));
	Connect(wxEVENT_MOVEBOTTOM, wxCommandEventHandler(CScrollbarWnd::OnMoveBottom));

	Connect(wxEVENT_SCROLLMOVE, wxCommandEventHandler(CScrollbarWnd::OnScrollMove));

	defaultPageSize = 50;
	defaultLineSize = 5;
}

void CScrollbarWnd::OnScrollMove(wxCommandEvent& event)
{
	if (centralWindow->GetWindow() != nullptr)
	{
		wxCommandEvent evt(wxEVENT_SCROLLMOVE);
		evt.SetInt(event.GetInt());
		centralWindow->GetWindow()->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CScrollbarWnd::OnMoveLeft(wxCommandEvent& event)
{
	int pageSize = defaultPageSize;
	if (scrollVertical != nullptr)
	{
		pageSize = scrollHorizontal->GetPageSize();
	}
	posLargeur -= pageSize;
	TestMaxX();
	scrollHorizontal->SetPosition(posHauteur);

	if (centralWindow->GetWindow() != nullptr)
	{
		wxCommandEvent evt(wxEVENT_LEFTPOSITION);
		evt.SetInt(posLargeur);
		centralWindow->GetWindow()->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CScrollbarWnd::OnMoveRight(wxCommandEvent& event)
{
	int pageSize = defaultPageSize;
	if (scrollHorizontal != nullptr)
	{
		pageSize = scrollHorizontal->GetPageSize();
	}
	posLargeur += pageSize;
	TestMaxX();
	scrollHorizontal->SetPosition(posLargeur);

	if (centralWindow->GetWindow() != nullptr)
	{
		wxCommandEvent evt(wxEVENT_LEFTPOSITION);
		evt.SetInt(posLargeur);
		centralWindow->GetWindow()->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CScrollbarWnd::OnMoveTop(wxCommandEvent& event)
{
	int pageSize = defaultPageSize;
	if (scrollVertical != nullptr)
	{
		pageSize = scrollVertical->GetPageSize();
	}
	posHauteur -= pageSize;
	TestMaxY();
	scrollVertical->SetPosition(posHauteur);


	if (centralWindow->GetWindow() != nullptr)
	{
		wxCommandEvent evt(wxEVENT_TOPPOSITION);
		evt.SetInt(posHauteur);
		centralWindow->GetWindow()->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CScrollbarWnd::OnMoveBottom(wxCommandEvent& event)
{
	int pageSize = defaultPageSize;
	if (scrollVertical != nullptr)
	{
		pageSize = scrollVertical->GetPageSize();
	}
	posHauteur += pageSize;
	TestMaxY();
	scrollVertical->SetPosition(posHauteur);

	if (centralWindow->GetWindow() != nullptr)
	{
		wxCommandEvent evt(wxEVENT_TOPPOSITION);
		evt.SetInt(posHauteur);
		centralWindow->GetWindow()->GetEventHandler()->AddPendingEvent(evt);
	}
}


void CScrollbarWnd::OnLeftPosition(wxCommandEvent& event)
{
	if (centralWindow->GetWindow() != nullptr)
	{
		wxCommandEvent evt(wxEVENT_LEFTPOSITION);
		evt.SetInt(event.GetInt());
		centralWindow->GetWindow()->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CScrollbarWnd::OnTopPosition(wxCommandEvent& event)
{
	if (centralWindow->GetWindow() != nullptr)
	{
		wxCommandEvent evt(wxEVENT_TOPPOSITION);
		evt.SetInt(event.GetInt());
		centralWindow->GetWindow()->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CScrollbarWnd::RefreshData(wxCommandEvent& event)
{
	if (centralWindow->GetWindow() != nullptr)
	{
		wxCommandEvent evt(wxEVENT_REFRESHDATA);
		evt.SetExtraLong(1);
		centralWindow->GetWindow()->GetEventHandler()->AddPendingEvent(evt);
	}
}

int CScrollbarWnd::GetHeight()
{
	return windowManager->GetHeight();
}

int CScrollbarWnd::GetWidth()
{
	return windowManager->GetWidth();
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CScrollbarWnd::TestMaxX()
{
	TRACE();
	if (centralWindow->GetWindow() != nullptr)
	{
		CMasterWindow * window = centralWindow->GetMasterWindowPt();

		if (posLargeur >= controlWidth - window->GetWindowWidth())
			posLargeur = controlWidth - window->GetWindowWidth();

		if (posLargeur < 0)
			posLargeur = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CScrollbarWnd::TestMaxY()
{
	TRACE();

	if (centralWindow->GetWindow() != nullptr)
	{
		CMasterWindow * window = centralWindow->GetMasterWindowPt();


		if (posHauteur >= controlHeight - window->GetWindowHeight())
			posHauteur = controlHeight - window->GetWindowHeight();

		if (posHauteur < 0)
			posHauteur = 0;
	}
}

void CScrollbarWnd::SetPageSize(const int &pageSize)
{
	scrollHorizontal->SetPageSize(pageSize);
	scrollVertical->SetPageSize(pageSize);
}

void CScrollbarWnd::SetLineSize(const int &lineSize)
{
	scrollHorizontal->SetLineSize(lineSize);
	scrollVertical->SetLineSize(lineSize);
}

int CScrollbarWnd::GetBarHeight()
{
	return scrollHorizontal->GetHeightSize();
}

int CScrollbarWnd::GetBarWidth()
{
	return scrollVertical->GetWidthSize();
}


CScrollbarWnd::~CScrollbarWnd()
{
	if (windowManager != nullptr)
		delete windowManager;
}

void CScrollbarWnd::UpdateScreenRatio()
{
    scrollVertical->UpdateScreenRatio();
    scrollHorizontal->UpdateScreenRatio();
    this->Resize();
}

void CScrollbarWnd::OnSetPosition(wxCommandEvent& event)
{
	wxSize * size = (wxSize*)event.GetClientData();
	if (size != nullptr)
	{
		int posX = size->x;
		int posY = size->y;
		SetPosition(posX, posY);
		delete size;
	}
}

void CScrollbarWnd::OnControlSize(wxCommandEvent& event)
{
	CControlSize * controlSize = (CControlSize *)event.GetClientData();
	if (controlSize != nullptr)
	{
		if (controlSize->controlWidth != controlWidth || controlSize->controlHeight != controlHeight)
		{
			controlWidth = controlSize->controlWidth;
			controlHeight = controlSize->controlHeight;
			_useScaleFactor = controlSize->useScaleFactor;
			Resize();
		}
		delete controlSize;
	}
}

int CScrollbarWnd::GetPosLargeur()
{
	int pictureWidth = GetWindowWidth();

    if(scrollVertical != nullptr && showV)
    {
        bool valueV = scrollVertical->IsShown();
        if (valueV)
            pictureWidth -= scrollVertical->GetWidthSize();
    }

	int maxPosWidth = max(controlWidth - pictureWidth, 0);
	posLargeur = max(posLargeur, 0);
	posLargeur = min(posLargeur, maxPosWidth);
	return posLargeur;
}

int CScrollbarWnd::GetPosHauteur()
{
	int pictureHeight = GetWindowHeight();

    if(scrollHorizontal != nullptr && showH)
    {
        bool valueH = scrollHorizontal->IsShown();
        if (valueH)
            pictureHeight -= scrollHorizontal->GetHeightSize();
    }


	int maxPosHeight = max(controlHeight - pictureHeight, 0);

	posHauteur = max(posHauteur, 0);
	posHauteur = min(posHauteur, maxPosHeight);

	return posHauteur;
}

void CScrollbarWnd::ShowVerticalScroll()
{
	showV = true;
}
void CScrollbarWnd::ShowHorizontalScroll()
{
	showH = true;
}
void CScrollbarWnd::HideVerticalScroll()
{
	showV = false;
}
void CScrollbarWnd::HideHorizontalScroll()
{
	showH = false;
}

void CScrollbarWnd::SetPosition(const int &posX, const int &posY)
{
	posHauteur = std::max(posY, 0);
	posLargeur = std::max(posX, 0);

	int pictureWidth = GetWindowWidth();
	int pictureHeight = GetWindowHeight();

    if(scrollHorizontal != nullptr && scrollVertical != nullptr)
    {
        bool valueH = scrollHorizontal->IsShown();
        bool valueV = scrollVertical->IsShown();
        if (valueV)
            pictureWidth -= scrollVertical->GetWidthSize();

        if (valueH)
            pictureHeight -= scrollHorizontal->GetHeightSize();
    }


	int maxPosHeight = max(controlHeight - pictureHeight, 0);
	int maxPosWidth = max(controlWidth - pictureWidth, 0);

	posHauteur = std::min(posY, maxPosHeight);
	posLargeur = std::min(posX, maxPosWidth);

	if (scrollVertical != nullptr && showV)
	{
		scrollVertical->SetPosition(posHauteur);
	}
	if (scrollHorizontal != nullptr && showH)
	{
		scrollHorizontal->SetPosition(posLargeur);
	}
}

void CScrollbarWnd::Resize()
{
	//CDeferPos deferpos;
	wxRect rect;
	int pictureWidth = GetWindowWidth();
	int pictureHeight = GetWindowHeight();
	bool _showScrollV = false;
	bool _showScrollH = false;
    
#ifdef __WXGTK__
    double scale_factor = GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif
    

    if(!_useScaleFactor)
        scale_factor = 1.0f;
    printf("CScrollbarWnd::Resize() controlHeight : %d pictureHeight : %d \n", controlHeight, pictureHeight);
    printf("CScrollbarWnd::Resize() controlWidth : %d pictureWidth : %d \n", controlWidth, pictureWidth);
    if(scrollHorizontal != nullptr && scrollVertical != nullptr)
    {   
		bool valueH = false;
		bool valueV = false;
		if(showV && (controlHeight > pictureHeight * scale_factor))
			valueV = true;

		if(showH && (controlWidth > pictureWidth * scale_factor))
			valueH = true;
		      
		if (valueV && showV)
		{
			pictureWidth -= scrollVertical->GetWidthSize();
			_showScrollV = true;
		}

		if (valueH && showH)
		{
			pictureHeight -= scrollHorizontal->GetHeightSize();
			_showScrollH = true;
		}

		if (scrollVertical != nullptr && _showScrollV)
		{
			windowManager->ShowWindow(Pos::wxRIGHT);
			scrollVertical->UpdateScrollBar(posHauteur, pictureHeight * scale_factor, controlHeight);
		}
		else
			windowManager->HideWindow(Pos::wxRIGHT);

		if (scrollHorizontal != nullptr && _showScrollH)
		{
			windowManager->ShowWindow(Pos::wxBOTTOM);
			scrollHorizontal->UpdateScrollBar(posLargeur, pictureWidth * scale_factor, controlWidth);
		}
		else
		{
			windowManager->HideWindow(Pos::wxBOTTOM);
		}
        
		if (windowManager != nullptr)
		{
			windowManager->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
			windowManager->Resize();
		}

    }
}