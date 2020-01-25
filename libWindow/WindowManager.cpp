#include "header.h"
#include "WindowManager.h"
#include "PanelWithClickToolbar.h"
#include <LibResource.h>
#include "MainTheme.h"
#include "MainThemeInit.h"
using namespace Regards::Window;

#define WINDOW_MINSIZE 100

CWindowManager::CWindowManager(wxWindow* parent, wxWindowID id, const CThemeSplitter & theme)
	: CWindowMain("CWindowManager", parent, id)
{
	moving = false;
	showSeparationBar = true;
	this->themeSplitter = theme;
	moving = false;
	oldWidth = 0;
	oldHeight = 0;
	init = false;
	Connect(wxEVT_PAINT, wxPaintEventHandler(CWindowManager::OnPaint));
}

void CWindowManager::OnPaint(wxPaintEvent& event)
{
	for (CWindowToAdd * windowToAdd : listWindow)
	{
		if (windowToAdd != nullptr)
		{
			if (windowToAdd->window != nullptr)
				windowToAdd->window->Refresh();
			if (windowToAdd->separationBar != nullptr)
			{
				if (windowToAdd->separationBar->separationBar != nullptr)
					windowToAdd->separationBar->separationBar->Refresh();
			}
		}
	}
}

void CWindowManager::OnResize(wxCommandEvent& event)
{
	int id = event.GetId();
	int value = event.GetInt();

	for (CWindowToAdd * windowToAdd : listWindow)
	{
		if (windowToAdd != nullptr)
		{
			if (windowToAdd->id == id)
			{
				if (value == 1) //Show
				{
					//windowToAdd->fixe = false;

					if (windowToAdd->separationBar != nullptr)
					{
						if (windowToAdd->separationBar->separationBar != nullptr)
							windowToAdd->separationBar->separationBar->Show(true);
					}

					windowToAdd->rect = windowToAdd->rect_old;
					windowToAdd->fixe = windowToAdd->fixe_old;

					//Recalcul window Size
					AddDifference(windowToAdd->diffWidth, windowToAdd->diffHeight, windowToAdd->position);

				}
				else //Close
				{
					windowToAdd->fixe_old = windowToAdd->fixe;
					windowToAdd->fixe = true;
					windowToAdd->diffWidth = 0;
					windowToAdd->diffHeight = 0;
					if (windowToAdd->separationBar != nullptr)
					{
						if (windowToAdd->separationBar->separationBar != nullptr)
							windowToAdd->separationBar->separationBar->Show(false);
					}

					windowToAdd->rect_old = windowToAdd->rect;
				}

				if (windowToAdd->fixe)
				{
					if (windowToAdd->position == Pos::wxLEFT || windowToAdd->position == Pos::wxRIGHT)
					{
						windowToAdd->size = windowToAdd->window->GetWidth();
					}
					else
					{
						windowToAdd->size = windowToAdd->window->GetHeight();
					}
				}
				
				Init();
				break;
			}
		}
			
	}

	Resize();
}

void CWindowManager::AddPanel(CWindowMain * window, const Pos &pos, bool fixe, int size, wxRect rect, const wxString &panelLabel, const wxString &windowName, const bool &isVisible, const int &idPanel)
{
	CPanelWithClickToolbar * panel = nullptr;
	if (pos == Pos::wxLEFT || pos == Pos::wxRIGHT)
		panel = CPanelWithClickToolbar::CreatePanel(this, panelLabel, windowName, isVisible, idPanel, true);
	else
		panel = CPanelWithClickToolbar::CreatePanel(this, panelLabel, windowName, isVisible, idPanel, false);
	window->Reparent(panel);
	panel->SetWindow(window);
	AddWindow(panel, pos, fixe, size, rect, idPanel);
}

void CWindowManager::HideWindow(Pos position)
{
	CWindowToAdd * window = FindWindow(position);
	if (window != nullptr)
	{
		window->isHide = true;
		window->window->Show(false);
		if (window->separationBar != nullptr)
		{
			if (window->separationBar->separationBar != nullptr)
				window->separationBar->separationBar->Show(false);
		}
		Init();
		Resize();
	}
	
}

void CWindowManager::ShowWindow(Pos position)
{
	CWindowToAdd * window = FindWindow(position);

	if (window != nullptr)
	{
		window->isHide = false;
		window->window->Show(true);
		if (window->separationBar != nullptr)
		{
			if (window->separationBar->separationBar != nullptr)
			{
				if(window->fixe)
					window->separationBar->separationBar->Show(true);
			}
		}
		Init();
		Resize();
	}
}

void CWindowManager::AddWindow(CWindowMain * window, Pos position, bool fixe, int size, wxRect rect, int id)
{
	CWindowToAdd * windowToAdd = new CWindowToAdd();
	if (window != nullptr)
	{
		windowToAdd->window = window;
		windowToAdd->position = position;
		windowToAdd->size = size;
		windowToAdd->fixe = fixe;
		windowToAdd->rect = rect;
		windowToAdd->id = id;
		windowToAdd->isHide = false;
		if (!fixe && position != Pos::wxCENTRAL)
		{
			windowToAdd->separationBar = new CSeparationBarToAdd();
			windowToAdd->separationBar->separationBarId = listWindow.size() + WM_USER + 1200;
			windowToAdd->separationBar->separationBar = new CSeparationBar(this, this, windowToAdd->separationBar->separationBarId, themeSplitter.themeSeparation);
			windowToAdd->separationBar->size = themeSplitter.themeSeparation.size;
			if (position == Pos::wxLEFT || position == Pos::wxRIGHT)
			{
				windowToAdd->separationBar->separationBar->SetHorizontal(false);
				windowToAdd->separationBar->isHorizontal = false;
			}
			else
			{
				windowToAdd->separationBar->separationBar->SetHorizontal(true);
				windowToAdd->separationBar->isHorizontal = true;
			}
		}
		else
			windowToAdd->separationBar = nullptr;

		listWindow.push_back(windowToAdd);
	}

}

void CWindowManager::UnInit()
{
}

void CWindowManager::Init()
{
	int width = GetSize().x;
	int height = GetSize().y;

	CWindowToAdd * top = FindWindow(Pos::wxTOP);
	if (top != nullptr)
	{
			if (!top->fixe)
			{
				wxRect rect = top->rect;
				if (rect.x == 0 && rect.y == 0 && rect.width == 0 && rect.height == 0)
				{
					//Initialize value 25%
					top->rect.x = 0;
					top->rect.y = 0;
					top->rect.width = width;
					top->rect.height = (height / 4);
					if (top->separationBar != nullptr)
					{
						top->rect.height -= top->separationBar->size;
						top->separationBar->rect.x = 0;
						top->separationBar->rect.y = top->rect.height;
						top->separationBar->rect.width = width;
						top->separationBar->rect.height = top->separationBar->size;
						top->separationBar->posBar = top->separationBar->rect.y + top->separationBar->size / 2;
					}
				}
			}
			else
			{
				top->rect.x = 0;
				top->rect.y = 0;
				top->rect.width = width;
				top->rect.height = top->size;
			}
	}

	CWindowToAdd * bottom = FindWindow(Pos::wxBOTTOM);
	if (bottom != nullptr)
	{
			if (!bottom->fixe)
			{
				wxRect rect = bottom->rect;
				if (rect.x == 0 && rect.y == 0 && rect.width == 0 && rect.height == 0)
				{
					//Initialize value 25%
					bottom->rect.x = 0;
					bottom->rect.y = height - (height / 4);
					bottom->rect.width = width;
					bottom->rect.height = height / 4;
					if (bottom->separationBar != nullptr)
					{
						bottom->rect.y += bottom->separationBar->size;
						bottom->rect.height -= bottom->separationBar->size;
						bottom->separationBar->rect.x = 0;
						bottom->separationBar->rect.y = bottom->rect.y - bottom->separationBar->size;
						bottom->separationBar->rect.width = width;
						bottom->separationBar->rect.height = bottom->separationBar->size;
						bottom->separationBar->posBar = bottom->separationBar->rect.y + bottom->separationBar->size / 2;
					}
				}
			}
			else
			{
				bottom->rect.x = 0;
				bottom->rect.y = height - bottom->size;
				bottom->rect.width = width;
				bottom->rect.height = bottom->size;
			}
	}

	CWindowToAdd * left = FindWindow(Pos::wxLEFT);
	if (left != nullptr)
	{
			int y = 0;
			int left_height = height;
			if (top != nullptr)
			{
				if (!top->isHide)
				{
					if (top->fixe)
					{
						y = top->size;
						left_height -= top->size;
					}
					else
					{
						y = top->rect.height;
						left_height -= top->rect.height;
						if (top->separationBar != nullptr && !top->fixe)
						{
							left_height -= top->separationBar->size;
							y += top->separationBar->size;
						}
					}
				}
			}
			if (bottom != nullptr)
			{
				if (!bottom->isHide)
				{
					if (bottom->fixe)
					{
						left_height -= bottom->size;
					}
					else
					{
						left_height -= bottom->rect.height;
						if (bottom->separationBar != nullptr && !bottom->fixe)
							left_height -= bottom->separationBar->size;
					}
				}
			}

			if (!left->fixe)
			{
				wxRect rect = left->rect;
				if (rect.x == 0 && rect.y == 0 && rect.width == 0 && rect.height == 0)
				{
					//Initialize value 25%
					left->rect.x = 0;
					left->rect.y = y;
					left->rect.width = width / 4;
					left->rect.height = left_height;
				}

				if (left->separationBar != nullptr)
				{
					left->rect.x = 0;
					left->rect.y = y;
					left->rect.height = left_height;
					left->rect.width -= left->separationBar->size;
					left->separationBar->rect.x = left->rect.width;
					left->separationBar->rect.y = y;
					left->separationBar->rect.width = left->separationBar->size;
					left->separationBar->rect.height = left_height;
					left->separationBar->posBar = left->separationBar->rect.x + left->separationBar->size / 2;
				}
			}
			else
			{
				left->rect.x = 0;
				left->rect.y = y;
				left->rect.width = left->size;
				left->rect.height = left_height;
			}
	}

	CWindowToAdd * right = FindWindow(Pos::wxRIGHT);
	if (right != nullptr)
	{
			int y = 0;
			int right_height = height;
			if (top != nullptr)
			{
				if (top->fixe)
				{
					y = top->size;
					right_height -= top->size;
				}
				else
				{
					y = top->rect.height;
					right_height -= top->rect.height;
					if (top->separationBar != nullptr && !top->fixe)
					{
						right_height -= top->separationBar->size;
						y += top->separationBar->size;
					}
				}
			}
			if (bottom != nullptr)
			{
				if (bottom->fixe)
				{
					right_height -= bottom->size;
				}
				else
				{
					right_height -= bottom->rect.height;
					if (bottom->separationBar != nullptr && !bottom->fixe)
						right_height -= bottom->separationBar->size;
				}
			}

			if (!right->fixe)
			{
				wxRect rect = right->rect;
				if (rect.x == 0 && rect.y == 0 && rect.width == 0 && rect.height == 0)
				{
					//Initialize value 25%
					right->rect.x = width - (width / 4);
					right->rect.y = y;
					right->rect.width = width / 4;
					right->rect.height = right_height;
				}

				if (right->separationBar != nullptr)
				{
					right->rect.y = y;
					right->rect.x += right->separationBar->size;
					right->rect.width -= right->separationBar->size;
					right->rect.height = right_height;
					right->separationBar->rect.x = right->rect.x - right->separationBar->size;
					right->separationBar->rect.y = y;
					right->separationBar->rect.width = right->separationBar->size;
					right->separationBar->rect.height = right_height;
					right->separationBar->posBar = right->separationBar->rect.x + right->separationBar->size / 2;
				}
			}
			else
			{
				right->rect.x = width - right->size;
				right->rect.y = y;
				right->rect.width = right->size;
				right->rect.height = right_height;
			}
	}

	CWindowToAdd * central = FindWindow(Pos::wxCENTRAL);
	if (central)
	{
		int x = 0;
		int y = 0;
		int height = this->GetSize().y;
		int width = this->GetSize().x;
		if (top != nullptr)
		{
			if (!top->isHide)
			{
				if (top->fixe)
				{
					y = top->size;
					height -= top->size;
				}
				else
				{
					y = top->rect.height;
					height -= top->rect.height;
					if (top->separationBar != nullptr && !top->fixe)
					{
						height -= top->separationBar->size;
						y += top->separationBar->size;
					}
				}
			}
		}
		if (bottom != nullptr)
		{
			if (!bottom->isHide)
			{
				if (bottom->fixe)
				{
					height -= bottom->size;
				}
				else
				{
					height -= bottom->rect.height;
					if (bottom->separationBar != nullptr && !bottom->fixe)
						height -= bottom->separationBar->size;
				}
			}
		}

		if (left != nullptr)
		{
			if (!left->isHide)
			{
				x += left->rect.width;
				width -= left->rect.width;
				if (left->separationBar != nullptr && !left->fixe)
				{
					x += left->separationBar->size;
					width -= left->separationBar->size;
				}
			}
		}

		if (right != nullptr)
		{
			if (!right->isHide)
			{
				width -= right->rect.width;
				if (right->separationBar != nullptr && !right->fixe)
				{
					width -= right->separationBar->size;
				}
			}
		}

		central->rect.x = x;
		central->rect.y = y;
		central->rect.width = width;
		central->rect.height = height;
	}
}

void CWindowManager::GenerateRenderBitmap()
{
	renderBitmap = wxBitmap(GetWindowWidth(), GetWindowHeight());
	wxMemoryDC dCWindowManager(renderBitmap);

	for (CWindowToAdd * windowToAdd : listWindow)
	{
		if (windowToAdd != nullptr)
		{
			if (windowToAdd->window != nullptr)
			{
				if (windowToAdd->window->IsShown())
				{
					wxWindowDC dc(windowToAdd->window);
					dCWindowManager.Blit(windowToAdd->rect.x, windowToAdd->rect.y, windowToAdd->rect.width, windowToAdd->rect.height, &dc, 0, 0);
				}
			}
			if (showSeparationBar)
			{
				if (windowToAdd->separationBar != nullptr)
				{
					wxWindowDC dc(windowToAdd->separationBar->separationBar);
					dCWindowManager.Blit(windowToAdd->separationBar->rect.x, windowToAdd->separationBar->rect.y, windowToAdd->separationBar->rect.width, windowToAdd->separationBar->rect.height, &dc, 0, 0);
				}
			}
		}
	}


	dCWindowManager.SelectObject(wxNullBitmap);
}

void CWindowManager::SetSeparationBarVisible(const bool& visible)
{
	for (CWindowToAdd * windowToAdd : listWindow)
	{
		if (windowToAdd->separationBar != nullptr)
			windowToAdd->separationBar->separationBar->Show(visible);
	}
	showSeparationBar = visible;
}

bool CWindowManager::GetSeparationVisibility()
{
	return showSeparationBar;

}

bool CWindowManager::OnLButtonDown(CSeparationBar * separationBar)
{
	this->SetFocus();
	moving = true;
	GenerateRenderBitmap();
	return true;
}

void CWindowManager::OnLButtonUp(CSeparationBar * separationBar)
{
	moving = false;
	Resize();
	this->Refresh();
}

CWindowManager::~CWindowManager()
{

}

CWindowToAdd * CWindowManager::FindWindow(Pos position)
{
	CWindowToAdd * central = nullptr;
	for (CWindowToAdd * windowToAdd : listWindow)
	{
		if (windowToAdd != nullptr)
		{
			if (windowToAdd->position == position)
				return windowToAdd;
		}
	}
	return central;
}

void CWindowManager::MoveTop(int difference)
{
	CWindowToAdd * left = FindWindow(Pos::wxLEFT);
	CWindowToAdd * right = FindWindow(Pos::wxRIGHT);
	CWindowToAdd * top = FindWindow(Pos::wxTOP);
	CWindowToAdd * central = FindWindow(Pos::wxCENTRAL);
	if (top != nullptr && central != nullptr)
	{
		central->rect.y -= difference;
		central->rect.height += difference;
		top->rect.height -= difference;
		if (top->separationBar != nullptr)
			top->separationBar->rect.y -= difference;
	}


	if (left != nullptr)
	{
		left->rect.y -= difference;
		left->rect.height += difference;
		if (left->separationBar != nullptr)
		{
			left->separationBar->rect.y -= difference;
			left->separationBar->rect.height += difference;
		}
	}

	if (right != nullptr)
	{
		right->rect.y -= difference;
		right->rect.height += difference;
		if (right->separationBar != nullptr)
		{
			right->separationBar->rect.y -= difference;
			right->separationBar->rect.height += difference;
		}
	}
}

void CWindowManager::MoveBottom(int difference)
{
	CWindowToAdd * bottom = FindWindow(Pos::wxBOTTOM);
	CWindowToAdd * central = FindWindow(Pos::wxCENTRAL);
	CWindowToAdd * left = FindWindow(Pos::wxLEFT);
	CWindowToAdd * right = FindWindow(Pos::wxRIGHT);
	if (bottom != nullptr && central != nullptr)
	{
		central->rect.height -= difference;
		bottom->rect.height += difference;
		bottom->rect.y -= difference;
		if (bottom->separationBar != nullptr)
			bottom->separationBar->rect.y -= difference;
	}

	if (left != nullptr)
	{
		left->rect.height -= difference;
		if (left->separationBar != nullptr)
			left->separationBar->rect.height -= difference;
	}

	if (right != nullptr)
	{
		right->rect.height -= difference;
		if (right->separationBar != nullptr)
			right->separationBar->rect.height -= difference;
	}
}

void CWindowManager::MoveRight(int difference)
{
	CWindowToAdd * right = FindWindow(Pos::wxRIGHT);
	CWindowToAdd * central = FindWindow(Pos::wxCENTRAL);
	if (right != nullptr && central != nullptr)
	{
		central->rect.width += difference;
		right->rect.width -= difference;
		right->rect.x += difference;
		if (right->separationBar != nullptr)
			right->separationBar->rect.x += difference;
	}
}


void CWindowManager::MoveLeft(int difference)
{
	CWindowToAdd * left = FindWindow(Pos::wxLEFT);
	CWindowToAdd * central = FindWindow(Pos::wxCENTRAL);
	if (left != nullptr && central != nullptr)
	{
		central->rect.x += difference;
		central->rect.width -= difference;
		left->rect.width += difference;
		if (left->separationBar != nullptr)
			left->separationBar->rect.x += difference;
	}
}

void CWindowManager::SetNewPosition(CSeparationBar * separationBar)
{
	
	wxPoint mousePoint = wxGetMousePosition();
	wxPoint position = this->ScreenToClient(mousePoint);
	int width = GetSize().GetX();
	int height = GetSize().GetY();


	for (CWindowToAdd * windowToAdd : listWindow)
	{
		if (windowToAdd != nullptr)
		{
			if (windowToAdd->separationBar != nullptr)
			{
				if (windowToAdd->separationBar->separationBarId == separationBar->GetId())
				{
					CWindowToAdd * central = FindWindow(Pos::wxCENTRAL);

					int oldPos = windowToAdd->separationBar->posBar;
					int difference = 0;
					if (windowToAdd->separationBar->isHorizontal)
					{
						if (position.y < WINDOW_MINSIZE)
							position.y = WINDOW_MINSIZE;
						if (position.y > (height - WINDOW_MINSIZE))
							position.y = height - WINDOW_MINSIZE;

						if (windowToAdd->position == Pos::wxTOP)
						{
							if (central != nullptr)
								height = (central->rect.height + central->rect.y);

							if (position.y > (height - WINDOW_MINSIZE))
								position.y = height - WINDOW_MINSIZE;
						}

						if (windowToAdd->position == Pos::wxBOTTOM)
						{
							if (position.y < (central->rect.y + WINDOW_MINSIZE))
								position.y = central->rect.y + WINDOW_MINSIZE;
						}


						windowToAdd->separationBar->posBar = position.y;
						difference = oldPos - windowToAdd->separationBar->posBar;
					}
					else
					{
						if (position.x < WINDOW_MINSIZE)
							position.x = WINDOW_MINSIZE;
						if (position.x > (width - WINDOW_MINSIZE))
							position.x = width - WINDOW_MINSIZE;

						if (windowToAdd->position == Pos::wxLEFT)
						{
							if (central != nullptr)
								width = (central->rect.width + central->rect.x);

							if (position.x > (width - WINDOW_MINSIZE))
								position.x = width - WINDOW_MINSIZE;
						}

						if (windowToAdd->position == Pos::wxRIGHT)
						{
							if (position.x < (central->rect.x + WINDOW_MINSIZE))
								position.x = central->rect.x + WINDOW_MINSIZE;
						}

						windowToAdd->separationBar->posBar = position.x;
						difference = windowToAdd->separationBar->posBar - oldPos;
					}

					if (moving)
					{
						//Update window size
						switch (windowToAdd->position)
						{
						case Pos::wxTOP:
							MoveTop(difference);
							break;
						case Pos::wxBOTTOM:
							MoveBottom(difference);
							break;
						case Pos::wxRIGHT:
							MoveRight(difference);
							break;
						case Pos::wxLEFT:
							MoveLeft(difference);
							break;
						}
					}
					
					
					if (windowToAdd->separationBar->isHorizontal)
					{
						DrawSeparationBar(windowToAdd->separationBar->rect.x, position.y, windowToAdd->separationBar->rect.width, themeSplitter.themeFast.size, windowToAdd->separationBar->isHorizontal);
					}
					else
					{
						DrawSeparationBar(position.x, windowToAdd->separationBar->rect.y, themeSplitter.themeFast.size, windowToAdd->separationBar->rect.height,  windowToAdd->separationBar->isHorizontal);
					}

					
					break;
				}
			}
		}
	}
}


void CWindowManager::DrawSeparationBar(const int &x, const int &y, const int &width, const int &height, const bool &horizontal)
{
	wxWindowDC dc(this);
	dc.DrawBitmap(renderBitmap, 0, 0);

	if (horizontal)
	{
		wxRect rc;
		rc.x = x;
		rc.y = y;
		rc.width = width;
		rc.height = height;
		dc.GradientFillLinear(rc, themeSplitter.themeSeparation.secondColor, themeSplitter.themeSeparation.firstColor, wxSOUTH);
	}
	else
	{
		wxRect rc;
		rc.x = x;
		rc.y = y;
		rc.width = width;
		rc.height = height;
		dc.GradientFillLinear(rc, themeSplitter.themeSeparation.secondColor, themeSplitter.themeSeparation.firstColor, wxEAST);
	}
}


void CWindowManager::AddDifference(const int & diffWidth, const int &diffHeight, Pos position)
{


	if (diffWidth != 0 || diffHeight != 0)
	{
		switch (position)
		{
			case Pos::wxCENTRAL:
			{
				CWindowToAdd * central = FindWindow(Pos::wxCENTRAL);
				if (central != nullptr)
				{
					central->rect.width += diffWidth;
					central->rect.height += diffHeight;
				}
			}
			break;

			case Pos::wxRIGHT:
			{
				CWindowToAdd * right = FindWindow(Pos::wxRIGHT);
				if (right != nullptr)
				{
					right->rect.x += diffWidth;
					right->rect.height += diffHeight;

					if (right->separationBar != nullptr)
					{
						if (right->separationBar->separationBar != nullptr)
						{
							right->separationBar->posBar += diffWidth;
							right->separationBar->rect.x += diffWidth;
							right->separationBar->rect.height += diffHeight;
						}
					}
				}
			}
			break;

			case Pos::wxLEFT:
			{
				CWindowToAdd * left = FindWindow(Pos::wxLEFT);
				if (left != nullptr)
				{
					left->rect.height += diffHeight;

					if (left->separationBar != nullptr)
					{
						if (left->separationBar->separationBar != nullptr)
						{
							left->separationBar->rect.height += diffHeight;
						}
					}
				}
			}
			break;

			case Pos::wxTOP:
			{
				CWindowToAdd * top = FindWindow(Pos::wxTOP);
				if (top != nullptr)
				{
					top->rect.width += diffWidth;
					if (top->separationBar != nullptr)
					{
						if (top->separationBar->separationBar != nullptr)
						{
							top->separationBar->rect.width += diffWidth;
						}
					}
				}
			}
			break;

			case Pos::wxBOTTOM:
			{
				CWindowToAdd * bottom = FindWindow(Pos::wxBOTTOM);
				if (bottom != nullptr)
				{
					bottom->rect.width += diffWidth;
					bottom->rect.y += diffHeight;
					if (bottom->separationBar != nullptr)
					{
						if (bottom->separationBar->separationBar != nullptr)
						{
							bottom->separationBar->posBar += diffHeight;
							bottom->separationBar->rect.y += diffHeight;
							bottom->separationBar->rect.width += diffWidth;
						}
					}
				}
			}
			break;
		}
	}
}

void CWindowManager::Resize()
{

	int width = GetSize().GetX();
	int height = GetSize().GetY();

	if (!(width > 20 && height > 20))
		return;


	int diffWidth = width - oldWidth;
	int diffHeight = height - oldHeight;

	if (!init)
	{
		init = true;
		Init();
		diffWidth = 0;
		diffHeight = 0;
	}


	AddDifference(diffWidth, diffHeight, Pos::wxCENTRAL);
	AddDifference(diffWidth, diffHeight, Pos::wxLEFT);
	AddDifference(diffWidth, diffHeight, Pos::wxRIGHT);
	AddDifference(diffWidth, diffHeight, Pos::wxTOP);
	AddDifference(diffWidth, diffHeight, Pos::wxBOTTOM);
	//Calcul central size if window change size
	for (CWindowToAdd * windowToAdd : listWindow)
	{
		if (windowToAdd != nullptr)
		{
			windowToAdd->diffHeight += diffHeight;
			windowToAdd->diffWidth += diffWidth;
		}
	}

	wxRect rc;
	for (CWindowToAdd * windowToAdd : listWindow)
	{
		if (windowToAdd != nullptr)
		{
			if (windowToAdd->window != nullptr)
			{
				TCHAR temp[255];
				if (windowToAdd->window->IsShown())
				{
					windowToAdd->window->SetSize(windowToAdd->rect);
					if (windowToAdd->position == Pos::wxCENTRAL)
					{
						windowToAdd->window->SetFocus();
						windowToAdd->window->Raise();  // bring window to front
						windowToAdd->window->Show(true); // show the window
					}
					wsprintf(temp, L"windowToAdd rect : x : %d, y : %d, width %d, height %d \n", windowToAdd->rect.x, windowToAdd->rect.y, windowToAdd->rect.width, windowToAdd->rect.height);
					OutputDebugString(temp);
				}
				else
					windowToAdd->window->SetSize(rc);

			}
			if (windowToAdd->separationBar != nullptr)
			{
				if (windowToAdd->separationBar->separationBar != nullptr)
				{
					if (windowToAdd->separationBar->separationBar->IsShown())
					{
						TCHAR temp[255];
						if (windowToAdd->separationBar->separationBar->IsShown())
						{
							windowToAdd->separationBar->separationBar->SetSize(windowToAdd->separationBar->rect);
							wsprintf(temp, L"separationBar rect : x : %d, y : %d, width %d, height %d \n", windowToAdd->separationBar->rect.x, windowToAdd->separationBar->rect.y, windowToAdd->separationBar->rect.width, windowToAdd->separationBar->rect.height);
							OutputDebugString(temp);
						}
						else
							windowToAdd->separationBar->separationBar->SetSize(rc);

						
					}
				}
			}
		}
	}

	oldWidth = width;
	oldHeight = height;
}

