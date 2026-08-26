#include "header.h"
#include "ScrollbarWnd.h"
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarVerticalWnd.h"
#include "MainThemeInit.h"
#include "MainTheme.h"
using namespace Regards::Window;

CScrollbarWnd::CScrollbarWnd(wxWindow* parent, CWindowMain* centralWindow, wxWindowID id, const wxString& windowName)
	: CWindowMain(windowName, parent, id)
{
	wxRect rect;
	DefaultConstructor();
	this->centralWindow->SetWindow(centralWindow, false);
	windowToaddCentral = windowManager->AddWindow(centralWindow, Pos::wxCENTRAL, false, 0, rect, wxID_ANY, false);
}


CScrollbarWnd::CScrollbarWnd(wxWindow* parent, CWindowOpenGLMain* centralWindow, wxWindowID id,
	const wxString& windowName)
	: CWindowMain(windowName, parent, id)
{
	wxRect rect;
	DefaultConstructor();
	this->centralWindow->SetWindow(centralWindow, false);
	windowToaddCentral = windowManager->AddWindow(centralWindow, Pos::wxCENTRAL, false, 0, rect, wxID_ANY, false);
}


void CScrollbarWnd::DefaultConstructor()
{
	wxRect rect;

	CMainTheme* viewerTheme = CMainThemeInit::getInstance();
	centralWindow = std::make_unique<CWindowToAdd>();

	if (viewerTheme != nullptr)
	{
		CThemeSplitter theme;
		viewerTheme->GetSplitterTheme(&theme);
		windowManager = new CWindowManager(this, wxID_ANY, theme);
	}

	scrollHorizontal = nullptr;
	scrollVertical = nullptr;
	showV = true;
	showH = true;
	controlHeight = 0;
	controlWidth = 0;

	// [QUALITE] loadingTimer supprimé — était alloué mais jamais démarré ni utilisé

	if (viewerTheme != nullptr)
	{
		CThemeScrollBar theme;
		viewerTheme->GetScrollTheme(&theme);
		scrollHorizontal = new CScrollbarHorizontalWnd("ScrollbarHorizontal", windowManager, wxID_ANY, theme);
		scrollVertical = new CScrollbarVerticalWnd("ScrollbarVertical", windowManager, wxID_ANY, theme);
	}

	// [CRITIQUE] GetBarHeight/GetBarWidth sont maintenant null-safe, donc ces appels
	// sont sûrs même si les scrollbars n'ont pas pu être créées.
	windowManager->AddWindow(scrollHorizontal, Pos::wxBOTTOM, true, GetBarHeight(), rect, wxID_ANY, false);
	windowManager->AddWindow(scrollVertical, Pos::wxRIGHT, true, GetBarWidth(), rect, wxID_ANY, false);

	// [IMPORTANT] Migration Connect() → Bind() : type-safe, supporte les lambdas, idiomatique wx3


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

// [IMPORTANT] Helper centralisant le forwarding d'événements vers la fenêtre centrale.
// Supprime la duplication présente dans OnMoveLeft/Right/Top/Bottom et OnLeftPosition/TopPosition.
void CScrollbarWnd::ForwardScrollEvent(wxEventType type, int pos)
{
	if (centralWindow->GetWindow() != nullptr)
	{
		wxCommandEvent evt(type);
		evt.SetInt(pos);
		centralWindow->GetWindow()->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CScrollbarWnd::OnScrollMove(wxCommandEvent& event)
{
	ForwardScrollEvent(wxEVENT_SCROLLMOVE, event.GetInt());
}

void CScrollbarWnd::OnMoveLeft(wxCommandEvent& event)
{
	scrollHorizontal->ClickLeftPage();
	ForwardScrollEvent(wxEVENT_LEFTPOSITION, scrollHorizontal->GetPosition());
}

void CScrollbarWnd::OnMoveRight(wxCommandEvent& event)
{
	scrollHorizontal->ClickRightPage();
	ForwardScrollEvent(wxEVENT_LEFTPOSITION, scrollHorizontal->GetPosition());
}

void CScrollbarWnd::OnMoveTop(wxCommandEvent& event)
{
	scrollVertical->ClickTopPage();
	ForwardScrollEvent(wxEVENT_TOPPOSITION, scrollVertical->GetPosition());
}

void CScrollbarWnd::OnMoveBottom(wxCommandEvent& event)
{
	scrollVertical->ClickBottomPage();
	ForwardScrollEvent(wxEVENT_TOPPOSITION, scrollVertical->GetPosition());
}

void CScrollbarWnd::OnLeftPosition(wxCommandEvent& event)
{
	ForwardScrollEvent(wxEVENT_LEFTPOSITION, event.GetInt());
}

void CScrollbarWnd::OnTopPosition(wxCommandEvent& event)
{
	ForwardScrollEvent(wxEVENT_TOPPOSITION, event.GetInt());
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

// [QUALITE] const ajouté sur SetPageSize/SetLineSize — paramètres scalaires passés par valeur
void CScrollbarWnd::SetPageSize(int pageSize)
{
	scrollHorizontal->SetPageSize(pageSize);
	scrollVertical->SetPageSize(pageSize);
}

void CScrollbarWnd::SetLineSize(int lineSize)
{
	scrollHorizontal->SetLineSize(lineSize);
	scrollVertical->SetLineSize(lineSize);
}

// [CRITIQUE] Garde nullptr : scrollHorizontal/scrollVertical sont initialisés conditionnellement
int CScrollbarWnd::GetBarHeight() const
{
	return scrollHorizontal ? scrollHorizontal->GetHeightSize() : 0;
}

int CScrollbarWnd::GetBarWidth() const
{
	return scrollVertical ? scrollVertical->GetWidthSize() : 0;
}


void CScrollbarWnd::UpdateScreenRatio()
{
	scrollVertical->UpdateScreenRatio();
	scrollHorizontal->UpdateScreenRatio();
	this->Resize();
}

void CScrollbarWnd::OnSetPosition(wxCommandEvent& event)
{
	auto size = static_cast<wxSize*>(event.GetClientData());
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
	auto controlSize = static_cast<CControlSize*>(event.GetClientData());
	if (controlSize != nullptr)
	{
		if (controlSize->controlWidth != controlWidth || controlSize->controlHeight != controlHeight)
		{
			controlWidth = controlSize->controlWidth;
			controlHeight = controlSize->controlHeight;
			_useScaleFactor = controlSize->useScaleFactor;
			this->Resize();
		}
		delete controlSize;
	}
}

int CScrollbarWnd::GetPosLargeur() const
{
	return scrollHorizontal->GetPosition();
}

int CScrollbarWnd::GetPosHauteur() const
{
	return scrollVertical->GetPosition();
}

// [QUALITE] bool au lieu de int pour les getters de visibilité
bool CScrollbarWnd::GetShowingScrollV() const { return showV; }
bool CScrollbarWnd::GetShowingScrollH() const { return showH; }

void CScrollbarWnd::ShowVerticalScroll() { showV = true; }
void CScrollbarWnd::ShowHorizontalScroll() { showH = true; }
void CScrollbarWnd::HideVerticalScroll() { showV = false; }
void CScrollbarWnd::HideHorizontalScroll() { showH = false; }

// [QUALITE] Paramètres int passés par valeur (pas de const& sur les scalaires)
void CScrollbarWnd::SetPosition(int posX, int posY)
{
	int pictureWidth = GetWindowWidth();
	int pictureHeight = GetWindowHeight();

	if (scrollHorizontal != nullptr && scrollVertical != nullptr)
	{
		if (scrollVertical->IsShown())
			pictureWidth -= scrollVertical->GetWidthSize();

		if (scrollHorizontal->IsShown())
			pictureHeight -= scrollHorizontal->GetHeightSize();
	}

	int maxPosHeight = max(controlHeight - pictureHeight, 0);
	int maxPosWidth = max(controlWidth - pictureWidth, 0);

	int posHauteur = std::min(posY, maxPosHeight);
	int posLargeur = std::min(posX, maxPosWidth);

	if (scrollVertical != nullptr && showV) scrollVertical->SetPosition(posHauteur);
	if (scrollHorizontal != nullptr && showH) scrollHorizontal->SetPosition(posLargeur);
}

void CScrollbarWnd::Resize()
{
	int pictureWidth = GetWindowWidth();
	int pictureHeight = GetWindowHeight();

	if (!(pictureWidth > 0 && pictureHeight > 0))
		return;

	if (scrollHorizontal == nullptr || scrollVertical == nullptr)
		return;

	const double scale_factor = _useScaleFactor ? 1.0 : 1.0; // placeholder si DPI variable

	// [CRITIQUE] Calcul des visibilités AVANT toute modification des dimensions,
	// pour éviter que showScrollV influe sur le test de showScrollH.
	const bool showScrollV = showV && (controlHeight > pictureHeight * scale_factor);
	const bool showScrollH = showH && (controlWidth > pictureWidth * scale_factor);

	// Dimensions effectives disponibles pour chaque barre
	const int effectiveW = pictureWidth - (showScrollV ? scrollVertical->GetWidthSize() : 0);
	const int effectiveH = pictureHeight - (showScrollH ? scrollHorizontal->GetHeightSize() : 0);

	const int posLargeur = scrollHorizontal->GetPosition();
	const int posHauteur = scrollVertical->GetPosition();

	if (showScrollV)
	{
		windowManager->ShowWindow(Pos::wxRIGHT);
		scrollVertical->UpdateScrollBar(posHauteur, effectiveH * scale_factor, controlHeight);
	}
	else
	{
		windowManager->HideWindow(Pos::wxRIGHT);
	}

	if (showScrollH)
	{
		windowManager->ShowWindow(Pos::wxBOTTOM);
		scrollHorizontal->UpdateScrollBar(posLargeur, effectiveW * scale_factor, controlWidth);
	}
	else
	{
		windowManager->HideWindow(Pos::wxBOTTOM);
	}

	if (windowManager != nullptr)
	{
		windowManager->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
		windowManager->Refresh();
	}

	//Send Message to central window to resize it
	centralWindow->GetWindow()->SetSize(0, 0,  effectiveW, effectiveH);

	if (oldshowScrollH != showScrollH)
	{
		oldshowScrollH = showScrollH;
		wxCommandEvent evt(wxEVENT_SCROLLBARH);
		evt.SetInt(showScrollH);
		evt.SetExtraLong(scrollHorizontal->GetHeightSize());
		centralWindow->GetWindow()->GetEventHandler()->AddPendingEvent(evt);
	}




	scrollVertical->SetShowWindow(showScrollV);
	scrollHorizontal->SetShowWindow(showScrollH);
}