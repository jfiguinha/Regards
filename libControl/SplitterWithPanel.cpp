#include <header.h>
#include "SplitterWithPanel.h"
#include <SeparationBar.h>
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif

using namespace Regards::Window;
using namespace Regards::Control;

wxDEFINE_EVENT(EVENT_HIDDENPANE, wxCommandEvent);

// ============================================================================
// Constructor & Setup
// ============================================================================

CSplitterWithPanel::CSplitterWithPanel(wxWindow* parent, wxWindowID id, 
									   const CThemeSplitter& theme,
									   const CThemeToolbar& themeInfosToolbar, 
									   bool horizontal)
	: CSplitter(parent, id, theme),
	  clickWindow1Toolbar(std::make_unique<CClickToolbar>(
		  this, wxID_ANY, themeInfosToolbar, this, 
		  static_cast<int>(PanelId::PANEL1), !horizontal)),
	  clickWindow2Toolbar(std::make_unique<CClickToolbar>(
		  this, wxID_ANY, themeInfosToolbar, this, 
		  static_cast<int>(PanelId::PANEL2), !horizontal))
{
	SetHorizontalImpl(horizontal);
	GetSeparationBar()->SetHorizontal(horizontal);
}

void CSplitterWithPanel::SetHorizontal(bool horizontal)
{
	SetHorizontalImpl(horizontal);
	GetSeparationBar()->SetHorizontal(horizontal);
	clickWindow1Toolbar->SetVertical(!horizontal);
	clickWindow2Toolbar->SetVertical(!horizontal);
}

// ============================================================================
// Screen & Display
// ============================================================================

void CSplitterWithPanel::UpdateScreenRatio()
{
	if (paneWindow1)
		paneWindow1->UpdateScreenRatio();

	if (paneWindow2)
		paneWindow2->UpdateScreenRatio();
}

void CSplitterWithPanel::ShowWindow()
{
	switch (lastWindow)
	{
	case PanelId::ALL_WINDOWS:
		ShowWindow(static_cast<int>(PanelId::PANEL1), true);
		ShowWindow(static_cast<int>(PanelId::PANEL2), true);
		break;

	case PanelId::PANEL2:
		ShowWindow(static_cast<int>(PanelId::PANEL2), true);
		break;

	case PanelId::PANEL1:
	default:
		ShowWindow(static_cast<int>(PanelId::PANEL1), true);
		break;
	}
	Resize(this);
}

// ============================================================================
// Panel Visibility Control
// ============================================================================

void CSplitterWithPanel::ClickShowButton(int id)
{
	switch (id)
	{
	case static_cast<int>(PanelId::PANEL1):
		if (paneWindow1 && !paneWindow1->IsShown())
		{
			ShowWindow(id, true);
			Resize(this);
		}
		break;

	case static_cast<int>(PanelId::PANEL2):
		if (paneWindow2 && !paneWindow2->IsShown())
		{
			ShowWindow(id, true);
			Resize(this);
		}
		break;
	}
}

void CSplitterWithPanel::ClosePane(int id)
{
	// Update last visible panel state
	if (paneWindow1 && paneWindow2)
	{
		lastWindow = (paneWindow1->IsShown() && paneWindow2->IsShown())
			? PanelId::ALL_WINDOWS
			: (paneWindow2->IsShown() ? PanelId::PANEL2 : PanelId::PANEL1);
	}

	// Hide the requested panel
	ShowWindow(id, false);

	// Check if both panels are closed
	if (paneWindow1 && paneWindow2 && !paneWindow1->IsShown() && !paneWindow2->IsShown())
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_ALLPANECLOSED);
		GetParent()->GetEventHandler()->AddPendingEvent(evt);
	}

	Resize(this);
}

// ============================================================================
// Positioning & Sizing
// ============================================================================

int CSplitterWithPanel::GetPosition(int panel) const
{
	const bool isHorizontal = GetHorizontal();
	CClickToolbar* toolbar = (panel == 1) ? clickWindow1Toolbar.get() : clickWindow2Toolbar.get();

	if (!toolbar)
		return 0;

	return isHorizontal ? toolbar->GetNavigatorHeight() : toolbar->GetNavigatorWidth();
}

void CSplitterWithPanel::FixWindow(int numWindow, int size)
{
	this->numWindow = numWindow;
	this->size = size;
}

int CSplitterWithPanel::GetDefaultPositionForWindow1() const
{
	return -1;
}

int CSplitterWithPanel::GetDefaultPositionForWindow2() const
{
	return -1;
}

// ============================================================================
// Panel Display Logic
// ============================================================================

void CSplitterWithPanel::ShowWindow(int id, bool show)
{
	if (!paneWindow1 || !clickWindow1Toolbar || !paneWindow2 || !clickWindow2Toolbar)
		return;

	if (id == static_cast<int>(PanelId::PANEL1))
		ShowPanel1(show);
	else if (id == static_cast<int>(PanelId::PANEL2))
		ShowPanel2(show);
}

void CSplitterWithPanel::ShowPanel1(bool show)
{
	if (show)
	{
		paneWindow1->Show(true);
		clickWindow1Toolbar->Show(false);

		if (paneWindow2->IsShown())
		{
			SetWindow(paneWindow1.get(), paneWindow2.get());
			if (numWindow == 1)
				SetWindow1FixPosition(true, size);
			else
			{
				const int defaultPosition = GetDefaultPositionForWindow1();
				if (defaultPosition != -1)
					posBarInfos = defaultPosition;
				SetWindow1FixPosition(false, posBarInfos);
			}
		}
		else
		{
			if (!clickWindow2Toolbar->IsShown())
				clickWindow2Toolbar->Show(true);
			SetWindow(paneWindow1.get(), clickWindow2Toolbar.get());
			SetWindow2FixPosition(true, GetPosition(2));
		}

		SetSeparationBarVisible(true);
		GetPosBar() = posBarInfos;
	}
	else if (showclickWindow1Toolbar)
	{
		paneWindow1->Show(false);
		clickWindow1Toolbar->Show(true);

		if (paneWindow2->IsShown())
		{
			SetWindow(clickWindow1Toolbar.get(), paneWindow2.get());
			SetWindow1FixPosition(true, GetPosition(1));
			posBarInfos = GetPosBar();
			GetPosBar() = GetPosition(1);
			SetSeparationBarVisible(false);
		}
	}
	else
	{
		paneWindow1->Show(false);
		clickWindow1Toolbar->Show(false);

		if (paneWindow2->IsShown())
		{
			SetWindow(nullptr, paneWindow2.get());
			SetWindow1FixPosition(true, GetPosition(1));
			posBarInfos = GetPosBar();
			GetPosBar() = GetPosition(1);
			SetSeparationBarVisible(false);
		}
	}
}

void CSplitterWithPanel::ShowPanel2(bool show)
{
	if (show)
	{
		paneWindow2->Show(true);
		clickWindow2Toolbar->Show(false);

		if (paneWindow1->IsShown())
		{
			SetWindow(paneWindow1.get(), paneWindow2.get());
			if (numWindow == 2)
				SetWindow2FixPosition(true, size);
			else
			{
				const int defaultPosition = GetDefaultPositionForWindow2();
				if (defaultPosition != -1)
					posBarInfos = defaultPosition;
				SetWindow2FixPosition(false, posBarInfos);
			}
		}
		else
		{
			if (!clickWindow1Toolbar->IsShown())
				clickWindow1Toolbar->Show(true);
			SetWindow(clickWindow1Toolbar.get(), paneWindow2.get());
			SetWindow1FixPosition(true, GetPosition(1));
		}

		SetSeparationBarVisible(true);
		GetPosBar() = posBarInfos;
	}
	else if (showclickWindow2Toolbar)
	{
		paneWindow2->Show(false);
		clickWindow2Toolbar->Show(true);

		if (paneWindow1->IsShown())
		{
			SetWindow(paneWindow1.get(), clickWindow2Toolbar.get());
			posBarInfos = GetPosBar();
			GetPosBar() = GetPosition(2);
			SetWindow2FixPosition(true, GetPosition(2));
			SetSeparationBarVisible(false);
		}
	}
	else
	{
		paneWindow2->Show(false);
		clickWindow2Toolbar->Show(false);

		if (paneWindow1->IsShown())
		{
			SetWindow(paneWindow1.get(), nullptr);
			posBarInfos = GetPosBar();
			GetPosBar() = GetPosition(2);
			SetWindow2FixPosition(true, GetPosition(2));
			SetSeparationBarVisible(false);
		}
	}
}
