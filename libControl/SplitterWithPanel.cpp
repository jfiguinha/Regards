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

CSplitterWithPanel::CSplitterWithPanel(wxWindow* parent, wxWindowID id, const CThemeSplitter& theme,
                                       const CThemeToolbar& themeInfosToolbar, const bool& horizontal)
	: CSplitter(parent, id, theme)
{
	posBarInfos = 0;
	paneWindow1 = nullptr;
	clickWindow1Toolbar = nullptr;

	paneWindow2 = nullptr;
	clickWindow2Toolbar = nullptr;

	fullscreen = false;
	lastWindow = ALL_WINDOW;
	numWindow = 0;
	size = 0;

	showclickWindow1Toolbar = true;
	showclickWindow2Toolbar = true;

	clickWindow1Toolbar = new CClickToolbar(this, wxID_ANY, themeInfosToolbar, this, PANEL1_FILTER, !horizontal);
	clickWindow2Toolbar = new CClickToolbar(this, wxID_ANY, themeInfosToolbar, this, PANEL2_FILTER, !horizontal);

	this->horizontal = horizontal;
	separationBar->SetHorizontal(horizontal);
}


void CSplitterWithPanel::SetHorizontal(const bool& horizontal)
{
	this->horizontal = horizontal;
	separationBar->SetHorizontal(horizontal);
	clickWindow1Toolbar->SetVertical(!horizontal);
	clickWindow2Toolbar->SetVertical(!horizontal);
}


CSplitterWithPanel::~CSplitterWithPanel()
{
	if (clickWindow1Toolbar != nullptr)
		delete(clickWindow1Toolbar);
	if (clickWindow2Toolbar != nullptr)
		delete(clickWindow2Toolbar);
	if (paneWindow1 != nullptr)
		delete(paneWindow1);
	if (paneWindow2 != nullptr)
		delete(paneWindow2);
}


void CSplitterWithPanel::UpdateScreenRatio()
{
	if (paneWindow1 != nullptr)
		paneWindow1->UpdateScreenRatio();

	if (paneWindow2 != nullptr)
		paneWindow2->UpdateScreenRatio();
}

void CSplitterWithPanel::ShowWindow()
{
	if (lastWindow == ALL_WINDOW)
	{
		ShowWindow(PANEL1_FILTER, true);
		ShowWindow(PANEL2_FILTER, true);
	}
	else if (lastWindow == PANEL2_FILTER)
	{
		ShowWindow(PANEL2_FILTER, true);
	}
	else
		ShowWindow(PANEL1_FILTER, true);
	this->Resize(this);
}

void CSplitterWithPanel::ClickShowButton(const int& id)
{
	switch (id)
	{
	case PANEL1_FILTER:
		{
			if (!paneWindow1->IsShown())
			{
				ShowWindow(PANEL1_FILTER, true);
				this->Resize(this);
			}
		}
		break;

	case PANEL2_FILTER:
		{
			if (!paneWindow2->IsShown())
			{
				ShowWindow(PANEL2_FILTER, true);
				this->Resize(this);
			}
		}
		break;
	default: ;
	}
}

void CSplitterWithPanel::ClosePane(const int& id)
{
	if (paneWindow1->IsShown() && paneWindow2->IsShown())
	{
		lastWindow = ALL_WINDOW;
	}
	else if (paneWindow2->IsShown())
	{
		lastWindow = PANEL2_FILTER;
	}
	else
		lastWindow = PANEL1_FILTER;

	switch (id)
	{
	case PANEL1_FILTER:
		{
			ShowWindow(PANEL1_FILTER, false);
		}
		break;

	case PANEL2_FILTER:
		{
			ShowWindow(PANEL2_FILTER, false);
		}
		break;
	default: ;
	}

	//Test si les deux panels sont fermés
	if (!paneWindow1->IsShown() && !paneWindow2->IsShown())
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_ALLPANECLOSED);
		this->GetParent()->GetEventHandler()->AddPendingEvent(evt);
	}

	this->Resize(this);
}


int CSplitterWithPanel::GetPosition(const int& panel)
{
	int size;
	if (panel == 1)
	{
		if (!horizontal)
		{
			size = clickWindow1Toolbar->GetNavigatorWidth();
		}
		else
		{
			size = clickWindow1Toolbar->GetNavigatorHeight();
		}
	}
	else
	{
		if (!horizontal)
		{
			size = clickWindow2Toolbar->GetNavigatorWidth();
		}
		else
		{
			size = clickWindow2Toolbar->GetNavigatorHeight();
		}
	}
	return size;
}

void CSplitterWithPanel::FixWindow(const int& numWindow, const int& size)
{
	this->numWindow = numWindow;
	this->size = size;
}

void CSplitterWithPanel::ShowWindow(const int& id, const bool& show)
{
	if (paneWindow1 != nullptr && clickWindow1Toolbar != nullptr && paneWindow2 != nullptr && clickWindow2Toolbar !=
		nullptr)
	{
		switch (id)
		{
		case PANEL1_FILTER:
			{
				if (show)
				{
					paneWindow1->Show(true);
					clickWindow1Toolbar->Show(false);
					if (paneWindow2->IsShown())
					{
						this->SetWindow(paneWindow1, paneWindow2);
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
						this->SetWindow(paneWindow1, clickWindow2Toolbar);
						SetWindow2FixPosition(true, GetPosition(2));
					}
					this->SetSeparationBarVisible(true);
					posBar = posBarInfos;
				}
				else if (showclickWindow1Toolbar)
				{
					paneWindow1->Show(false);
					clickWindow1Toolbar->Show(true);
					if (paneWindow2->IsShown())
					{
						this->SetWindow(clickWindow1Toolbar, paneWindow2);
						SetWindow1FixPosition(true, GetPosition(1));
						posBarInfos = posBar;
						const int taille = GetPosition(1);
						posBar = taille;
						this->SetSeparationBarVisible(false);
					}
				}
				else
				{
					paneWindow1->Show(false);
					clickWindow1Toolbar->Show(false);
					if (paneWindow2->IsShown())
					{
						this->SetWindow(nullptr, paneWindow2);
						SetWindow1FixPosition(true, GetPosition(1));
						posBarInfos = posBar;
						const int taille = GetPosition(1);
						posBar = taille;
						this->SetSeparationBarVisible(false);
					}
				}
			}
			break;

		case PANEL2_FILTER:
			{
				if (show)
				{
					paneWindow2->Show(true);
					clickWindow2Toolbar->Show(false);
					if (paneWindow1->IsShown())
					{
						this->SetWindow(paneWindow1, paneWindow2);
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

						this->SetWindow(clickWindow1Toolbar, paneWindow2);
						SetWindow1FixPosition(true, GetPosition(1));
					}

					this->SetSeparationBarVisible(true);
					posBar = posBarInfos;
				}
				else if (showclickWindow2Toolbar)
				{
					paneWindow2->Show(false);
					clickWindow2Toolbar->Show(true);
					if (paneWindow1->IsShown())
					{
						this->SetWindow(paneWindow1, clickWindow2Toolbar);
						posBarInfos = posBar;
						int taille = GetPosition(2);
						posBar = taille;
						SetWindow2FixPosition(true, GetPosition(2));
						this->SetSeparationBarVisible(false);
					}
				}
				else
				{
					paneWindow2->Show(false);
					clickWindow2Toolbar->Show(false);
					if (paneWindow1->IsShown())
					{
						this->SetWindow(paneWindow1, nullptr);
						posBarInfos = posBar;
						int taille = GetPosition(2);
						posBar = taille;
						SetWindow2FixPosition(true, GetPosition(2));
						this->SetSeparationBarVisible(false);
					}
				}
			}
			break;
		default: ;
		}
	}
}

int CSplitterWithPanel::GetDefaultPositionForWindow2()
{
	return -1;
}

int CSplitterWithPanel::GetDefaultPositionForWindow1()
{
	return -1;
}
