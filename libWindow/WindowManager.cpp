#include "header.h"
#include "WindowManager.h"
#include "PanelWithClickToolbar.h"
#include "MainTheme.h"
#include "WindowToAdd.h"
#include "SeparationBar.h"
using namespace Regards::Window;

#define WINDOW_MINSIZE 100

#ifndef WIN32
#define WM_USER 0x4000
#endif



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
	separationBarSize = themeSplitter.themeSeparation.size;
	Connect(wxEVENT_REFRESHDATA, wxCommandEventHandler(CWindowManager::OnRefreshData));
	Connect(wxEVENT_RESIZE, wxCommandEventHandler(CWindowManager::OnResize));
    
//#ifdef WIN32
//    fastRender = true;
//#else
	fastRender = false;
//#endif
}


void CWindowManager::OnRefreshData(wxCommandEvent& event)
{
	int id = event.GetId();
	for (CWindowToAdd * windowToAdd : listWindow)
	{
		if (windowToAdd != nullptr)
		{
			if (windowToAdd->id == id)
			{
				wxWindow * window = windowToAdd->GetWindow();
				if (window != nullptr)
				{
					wxCommandEvent evt(wxEVENT_REFRESHDATA);
					evt.SetExtraLong(1);
					window->GetEventHandler()->AddPendingEvent(evt);
				}
			}
		}
	}
}

void CWindowManager::ChangeWindow(CWindowMain * window, Pos position, bool isPanel)
{
	CWindowToAdd * windowToadd = FindWindow(position);
	if (windowToadd != nullptr)
	{
		window->Reparent(this);
		windowToadd->SetWindow(window, isPanel);
	}
}

void CWindowManager::UpdateScreenRatio()
{

	for (CWindowToAdd * windowToAdd : listWindow)
	{
		if (windowToAdd != nullptr)
		{
			if (windowToAdd->GetMasterWindowPt() != nullptr)
				windowToAdd->GetMasterWindowPt()->UpdateScreenRatio();

			if (windowToAdd->separationBar != nullptr)
			{
				if (windowToAdd->separationBar->separationBar != nullptr)
					windowToAdd->separationBar->separationBar->UpdateScreenRatio();
			}
		}

	}

}

void CWindowManager::OnResize(wxCommandEvent& event)
{
	int id = event.GetId();
	int value = event.GetInt();
	int show = event.GetExtraLong();

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
					windowToAdd->size = windowToAdd->size_old;
					//Recalcul window Size
					AddDifference(windowToAdd->diffWidth, windowToAdd->diffHeight, windowToAdd->position);

				}
				else //Close
				{
					windowToAdd->fixe_old = windowToAdd->fixe;
					windowToAdd->fixe = true;
					windowToAdd->diffWidth = 0;
					windowToAdd->diffHeight = 0;
					windowToAdd->size_old = windowToAdd->size;
					if (windowToAdd->separationBar != nullptr)
					{
						if (windowToAdd->separationBar->separationBar != nullptr)
							windowToAdd->separationBar->separationBar->Show(false);
					}

					windowToAdd->rect_old = windowToAdd->rect;

					if (windowToAdd->fixe)
					{
						if (windowToAdd->position == Pos::wxLEFT || windowToAdd->position == Pos::wxRIGHT)
						{
							windowToAdd->size = windowToAdd->GetMasterWindowPt()->GetWidth();
						}
						else
						{
							windowToAdd->size = windowToAdd->GetMasterWindowPt()->GetHeight();
						}
					}
				}

				/*

				*/
				Init();
				break;
			}
		}

	}

	if(show)
		Resize();
}

bool CWindowManager::IsWindowVisible(Pos position)
{
	CWindowToAdd * window = FindWindow(position);
	if (window != nullptr)
	{
		return !window->isHide;
	}
	return false;
}

CPanelWithClickToolbar * CWindowManager::AddPanel(CWindowMain * window, const Pos &pos, bool fixe, int size, wxRect rect, const wxString &panelLabel, const wxString &windowName, const bool &isVisible, const int &idPanel, const bool &refreshButton, const bool &isTop)
{
	CPanelWithClickToolbar * panel = nullptr;
	if (pos == Pos::wxLEFT || pos == Pos::wxRIGHT)
		panel = CPanelWithClickToolbar::CreatePanel(this, panelLabel, windowName, isVisible, idPanel, true, refreshButton);
	else
		panel = CPanelWithClickToolbar::CreatePanel(this, panelLabel, windowName, isVisible, idPanel, false, refreshButton);

	//wxWindow * parent = panel->GetPaneWindow();
	//window->Reparent(parent);
	panel->SetWindow(window);
	AddWindow(panel, pos, fixe, size, rect, idPanel, true, isTop);
	return panel;
}

void CWindowManager::HideWindow(Pos position, const bool &refresh)
{
	bool needTorefresh = false;
	CWindowToAdd * window = FindWindow(position);
	if (window != nullptr)
	{
		if (!window->isHide)
		{
			needTorefresh = true;
			wxWindow * _wnd = window->GetWindow();
			if (_wnd != nullptr)
			{
				window->isHide = true;
				_wnd->Show(false);
				if (window->separationBar != nullptr)
				{
					if (window->separationBar->separationBar != nullptr)
						window->separationBar->separationBar->Show(false);
				}
				if (window->isPanel)
				{
					window->GetPanel()->Show(false);
				}
			}
		}
	}
	if (refresh && needTorefresh)
	{
		Init();
		Resize();
	}
}

void CWindowManager::HidePaneWindow(Pos position, const int &refresh)
{
	CWindowToAdd * window = FindWindow(position);

	if (window != nullptr)
	{
		if (window->isPanel)
		{
			CPanelWithClickToolbar * panel = window->GetPanel();
			if (panel != nullptr)
			{
				if (panel->IsPanelVisible())
				{
					panel->ClosePane(PANE_WITHCLICKTOOLBAR, refresh);
				}
			}
		}

	}
}

bool CWindowManager::GetWindowIsShow(Pos position)
{
	CWindowToAdd * window = FindWindow(position);

	if (window != nullptr)
	{
		return !window->isHide;
	}
	return false;
}

int CWindowManager::GetPaneState(Pos position)
{
	int paneState = 0;
	CWindowToAdd * window = FindWindow(position);

	if (window != nullptr)
	{
		if (window->isPanel)
		{
			CPanelWithClickToolbar * panel = window->GetPanel();
			if (panel != nullptr)
			{
				if (!panel->IsPanelVisible())
				{
					paneState = 0;
				}
				else
				{
					paneState = 1;
				}
			}
		}
	}
	return paneState;
}

void CWindowManager::ShowPaneWindow(Pos position, const int &refresh)
{
	CWindowToAdd * window = FindWindow(position);

	if (window != nullptr)
	{
		if (window->isPanel)
		{
			CPanelWithClickToolbar * panel = window->GetPanel();
			if (panel != nullptr)
			{
				if (!panel->IsPanelVisible())
				{
					//if (window->fixe)
					//	panel->SetSize(window->rect_old);
					panel->ClickShowButton(PANE_WITHCLICKTOOLBAR, refresh);
				}
			}
		}
	}
}

void CWindowManager::ShowWindow(Pos position, const bool &refresh)
{
	bool needTorefresh = false;
	CWindowToAdd * window = FindWindow(position);

	if (window != nullptr)
	{
		if (window->isHide)
		{
			needTorefresh = true;
			window->isHide = false;
			window->GetWindow()->Show(true);
			if (window->separationBar != nullptr)
			{
				if (window->separationBar->separationBar != nullptr)
				{
					if (!window->fixe)
						window->separationBar->separationBar->Show(true);
				}
			}
			if (window->isPanel)
			{
				window->GetPanel()->Show(true);
			}
		}
	}

	if (refresh && needTorefresh)
	{
		Init();
		Resize();
	}
}

void CWindowManager::AddWindow(CWindowOpenGLMain * window, Pos position, bool fixe, int size, wxRect rect, int id, bool isPanel, const bool &isTop)
{
	CWindowToAdd * windowToAdd = new CWindowToAdd();
	if (window != nullptr)
	{
		window->Reparent(this);
		windowToAdd->SetWindow(window, isPanel);
		AddWindow(windowToAdd, position, fixe, size, rect, id, isPanel, isTop);
	}
}

void CWindowManager::AddWindow(CWindowToAdd * windowToAdd, Pos position, bool fixe, int size, wxRect rect, int id, bool isPanel, const bool &isTop)
{
	if (windowToAdd != nullptr)
	{
		windowToAdd->position = position;
		windowToAdd->size = size;
		windowToAdd->isTop = isTop;
		windowToAdd->fixe = fixe;
		windowToAdd->rect = rect;
		windowToAdd->id = id;
		windowToAdd->isPanel = isPanel;
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

void CWindowManager::AddWindow(CWindowMain * window, Pos position, bool fixe, int size, wxRect rect, int id, bool isPanel, const bool &isTop)
{
	CWindowToAdd * windowToAdd = new CWindowToAdd();
	if (window != nullptr)
	{
		window->Reparent(this);
		windowToAdd->SetWindow(window, isPanel);
		AddWindow(windowToAdd, position, fixe, size, rect, id, isPanel, isTop);
	}
}

void CWindowManager::UnInit()
{
}

void CWindowManager::Init_bottom()
{
	int width = GetSize().x;
	int height = GetSize().y;
	CWindowToAdd * left = FindWindow(Pos::wxLEFT);
	CWindowToAdd * right = FindWindow(Pos::wxRIGHT);

	CWindowToAdd * bottom = FindWindow(Pos::wxBOTTOM);
	if (bottom != nullptr)
	{
		int x = 0;
		int bottom_width = width;
		int default_height = (height / 4);

		if (left != nullptr)
		{
			if (left->isTop && !left->isHide)
			{
				if (left->fixe)
				{
					x = left->size;
					bottom_width -= left->size;
				}
				else
				{
					x = left->rect.width;
					bottom_width -= left->rect.width;

					if (left->separationBar != nullptr)
					{
						x += left->separationBar->rect.width;
						bottom_width -= left->separationBar->rect.width;
					}
				}
			}


		}

		if (right != nullptr)
		{
			if (right->isTop && !right->isHide)
			{
				if (right->fixe)
				{
					x = right->size;
					bottom_width -= right->size;
				}
				else
				{

					bottom_width -= right->rect.width;
					if (right->separationBar != nullptr)
					{
						bottom_width -= right->separationBar->rect.width;
					}
				}
			}
		}

		if (bottom->fixe)
		{
			default_height = bottom->size;
		}

		SetWindowBottomSize(bottom, default_height, x, height - default_height, bottom_width);
	}
}

void CWindowManager::SetWindowBottomSize(CWindowToAdd * bottom, int height, int x, int y, int width)
{
	if (!bottom->fixe)
	{
		wxRect rect = bottom->rect;
		if (rect.x == 0 && rect.y == 0 && rect.width == 0 && rect.height == 0)
		{
			//Initialize value 25%
			bottom->rect.x = x;
			bottom->rect.y = y;
			bottom->rect.width = width;
			bottom->rect.height = height;
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
		bottom->rect.x = x;
		bottom->rect.y = y;
		bottom->rect.width = width;
		bottom->rect.height = height;
	}
}

void CWindowManager::Init_top()
{ 
	int width = GetSize().x;
	int height = GetSize().y;
	CWindowToAdd * left = FindWindow(Pos::wxLEFT);
	CWindowToAdd * right = FindWindow(Pos::wxRIGHT);
	CWindowToAdd * top = FindWindow(Pos::wxTOP);
	if (top != nullptr)
	{
		int x = 0;
		int top_width = width;
		int default_height = (height / 4) ;

		if (left != nullptr)
		{
			if (left->isTop && !left->isHide)
			{
				if (left->fixe)
				{
					x = left->size;
					top_width -= left->size;
				}
				else
				{
					x = left->rect.width;
					top_width -= left->rect.width;

					if (left->separationBar != nullptr)
					{
						x += left->separationBar->rect.width;
						top_width -= left->separationBar->rect.width;
					}
				}
			}
		}

		if (right != nullptr)
		{
			if (right->isTop && !right->isHide)
			{
				if (right->fixe)
				{
					top_width -= right->size;
				}
				else
				{
					top_width -= right->rect.width;

					if (right->separationBar != nullptr)
					{
						top_width -= right->separationBar->rect.width;
					}
				}
			}
		}

		if (top->fixe)
		{
			default_height = top->size;
		}

		SetWindowTopSize(top, default_height, x, top_width);
	}
}

void CWindowManager::SetWindowTopSize(CWindowToAdd * top, int height, int x, int top_width)
{
	if (!top->fixe)
	{
		wxRect rect = top->rect;
		if (rect.x == 0 && rect.y == 0 && rect.width == 0 && rect.height == 0)
		{
			//Initialize value 25%
			top->rect.x = x;
			top->rect.y = 0;
			top->rect.width = top_width;
			top->rect.height = height;
			if (top->separationBar != nullptr)
			{
				top->rect.height -= top->separationBar->size;
				top->separationBar->rect.x = x;
				top->separationBar->rect.y = top->rect.height;
				top->separationBar->rect.width = top_width;
				top->separationBar->rect.height = top->separationBar->size;
				top->separationBar->posBar = top->separationBar->rect.y + top->separationBar->size / 2;
			}
		}
	}
	else
	{
		top->rect.x = x;
		top->rect.y = 0;
		top->rect.width = top_width;
		top->rect.height = height;
	}
}

void CWindowManager::Init_left()
{
	int width = GetSize().x;
	int height = GetSize().y;
	CWindowToAdd * top = FindWindow(Pos::wxTOP);
	CWindowToAdd * bottom = FindWindow(Pos::wxBOTTOM);
	CWindowToAdd * left = FindWindow(Pos::wxLEFT);
	if (left != nullptr)
	{
		int y = 0;
		int left_height = height;
		if (left->isTop)
		{
			SetWindowLeftSize(left, width, y, left_height);
		}
		else
		{
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

			SetWindowLeftSize(left, width, y, left_height);
		}
	}

}

void CWindowManager::SetWindowLeftSize(CWindowToAdd * left, int width, int y, int left_height)
{
	if (!left->fixe)
	{
		wxRect rect = left->rect;
		if (rect.x == 0 && rect.y == 0 && rect.width == 0 && rect.height == 0)
		{
            
			//Initialize value 25%
			left->rect.x = 0;
			left->rect.y = y;
			left->rect.width = (width / 4) ;
			left->rect.height = left_height;
            printf("SetWindowLeftSize %d \n", left->rect.width);
		}

		if (left->separationBar != nullptr)
		{
			left->rect.x = 0;
			left->rect.y = y;
			left->rect.height = left_height;
			//left->rect.width -= left->separationBar->size;
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

void CWindowManager::SetWindowRightSize(CWindowToAdd * right, int width, int y, int right_height)
{
    printf("SetWindowRightSize width : %d y : %d right_height : %d  \n", width, y, right_height);
    
	if (!right->fixe)
	{
		wxRect rect = right->rect;
        printf("SetWindowRightSize rect.x : %d rect.y : %d rect.width : %d rect.height : %d \n", rect.x, rect.y, rect.width, rect.height);
		if (rect.x == 0 && rect.y == 0 && rect.width == 0 && rect.height == 0)
		{
			//Initialize value 25%
			right->rect.x = width - (width / 4);
			right->rect.y = y;
			right->rect.width = (width / 4);
			right->rect.height = right_height;
            
            printf("SetWindowRightSize %d \n", right->rect.width);
		}

		if (right->separationBar != nullptr)
		{
			right->rect.y = y;
			//right->rect.x += right->separationBar->size;
			//right->rect.width -= right->separationBar->size;
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

void CWindowManager::Init_right()
{
	int width = GetSize().x;
	int height = GetSize().y;
	CWindowToAdd * right = FindWindow(Pos::wxRIGHT);

	CWindowToAdd * top = FindWindow(Pos::wxTOP);
	CWindowToAdd * bottom = FindWindow(Pos::wxBOTTOM);


	if (right != nullptr)
	{
		int y = 0;
		int right_height = height;

		if (right->isTop)
		{
			SetWindowRightSize(right, width, y, right_height);
		}
		else
		{

			if (top != nullptr)
			{
				if (!top->isHide)
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
			}
			if (bottom != nullptr)
			{
				if (!bottom->isHide)
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
			}

			SetWindowRightSize(right, width, y, right_height);
		}
	}
}

wxRect CWindowManager::GetWindowSize(Pos position)
{
	wxRect pos = { 0,0,0,0 };
	CWindowToAdd * window = FindWindow(position);
	if (window != nullptr)
	{
		return window->rect;
	}
	return pos;
}

void CWindowManager::ResetPosition()
{
	int width = GetSize().x;
	int height = GetSize().y;
    
    printf("ResetPosition() width : %d height : %d \n",width,height);
    
    
    wxRect rect;
 	CWindowToAdd * right = FindWindow(Pos::wxRIGHT);
    right->rect = rect;
	CWindowToAdd * top = FindWindow(Pos::wxTOP);
    top->rect = rect;
	CWindowToAdd * bottom = FindWindow(Pos::wxBOTTOM);
    bottom->rect = rect;
	CWindowToAdd * central = FindWindow(Pos::wxCENTRAL);
    central->rect = rect;
	CWindowToAdd * left = FindWindow(Pos::wxLEFT);   
    left->rect = rect;
    
    init = false;
}

void CWindowManager::Init_Central()
{
	//int width = GetSize().x;
	//int height = GetSize().y;
	CWindowToAdd * right = FindWindow(Pos::wxRIGHT);
	CWindowToAdd * top = FindWindow(Pos::wxTOP);
	CWindowToAdd * bottom = FindWindow(Pos::wxBOTTOM);
	CWindowToAdd * central = FindWindow(Pos::wxCENTRAL);
	CWindowToAdd * left = FindWindow(Pos::wxLEFT);
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
				
				right->rect.x = width + x;

				if (right->separationBar != nullptr && !right->fixe)
				{
					width -= right->separationBar->size;
				}

				//Correct x pos for right
				
			}
			else
			{
				right->rect.x = width + x;
			}
			
		}

		central->rect.x = x;
		central->rect.y = y;
		central->rect.width = width;
		central->rect.height = height;
        
        printf("central x %d \n", central->rect.x);
        printf("central width %d \n", central->rect.width);
	}
}

void CWindowManager::Init()
{
	CWindowToAdd * right = FindWindow(Pos::wxRIGHT);
	//CWindowToAdd * top = FindWindow(Pos::wxTOP);
	//CWindowToAdd * bottom = FindWindow(Pos::wxBOTTOM);
	//CWindowToAdd * central = FindWindow(Pos::wxCENTRAL);
	CWindowToAdd * left = FindWindow(Pos::wxLEFT);
    
	int width = GetSize().x;
	int height = GetSize().y;
    
    printf("Init() width : %d height : %d \n",width,height);

    if(left != nullptr)
        if(left->isTop)
            Init_left();
    if(right != nullptr)
        if(right->isTop)
            Init_right();
            
    Init_top();
	Init_bottom();
    if(left != nullptr)
        if(!left->isTop)
            Init_left();
    if(right != nullptr)
        if(!right->isTop)
            Init_right();
	Init_Central();
}

void CWindowManager::GenerateRenderBitmap()
{
	renderBitmap = wxBitmap(GetWindowWidth(), GetWindowHeight());
	wxMemoryDC dCWindowManager(renderBitmap);

	for (CWindowToAdd * windowToAdd : listWindow)
	{
		if (windowToAdd != nullptr)
		{
			wxWindow * _wnd = windowToAdd->GetWindow();

			if (_wnd != nullptr)
			{
				if (_wnd->IsShown())
				{
					wxWindowDC dc(_wnd);
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

bool CWindowManager::OnLButtonDown()
{
	this->SetFocus();
	moving = true;
    if (fastRender)
		GenerateRenderBitmap();
	return true;
}

void CWindowManager::OnLButtonUp()
{
	moving = false;
	if (fastRender)
	{
		Resize();
	}
	//this->ForceRefresh();
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
	CWindowToAdd * bottom = FindWindow(Pos::wxBOTTOM);
	CWindowToAdd * top = FindWindow(Pos::wxTOP);
	CWindowToAdd * right = FindWindow(Pos::wxRIGHT);
	CWindowToAdd * central = FindWindow(Pos::wxCENTRAL);
	if (right != nullptr && central != nullptr)
	{
		central->rect.width += difference;
		right->rect.width -= difference;
		right->rect.x += difference;
		if (right->separationBar != nullptr)
			right->separationBar->rect.x += difference;

		if (right->isTop)
		{
			if (bottom != nullptr)
			{

				bottom->rect.width += difference;

				if (bottom->separationBar != nullptr)
					bottom->separationBar->rect.width += difference;
			}

			if (top != nullptr)
			{
				top->rect.width += difference;

				if (top->separationBar != nullptr)
					top->separationBar->rect.width += difference;
			}
		}


	}
}


void CWindowManager::MoveLeft(int difference)
{
	CWindowToAdd * bottom = FindWindow(Pos::wxBOTTOM);
	CWindowToAdd * top = FindWindow(Pos::wxTOP);
	CWindowToAdd * left = FindWindow(Pos::wxLEFT);
	CWindowToAdd * central = FindWindow(Pos::wxCENTRAL);
	if (left != nullptr && central != nullptr)
	{
		central->rect.x += difference;
		central->rect.width -= difference;
		left->rect.width += difference;
		if (left->separationBar != nullptr)
			left->separationBar->rect.x += difference;

		if (left->isTop)
		{
			if (bottom != nullptr)
			{
				bottom->rect.x += difference;
				bottom->rect.width -= difference;

				if (bottom->separationBar != nullptr)
				{
					bottom->separationBar->rect.x += difference;
					bottom->separationBar->rect.width += difference;
				}
			}

			if (top != nullptr)
			{
				top->rect.width -= difference;
				top->rect.x += difference;

				if (top->separationBar != nullptr)
				{
					top->separationBar->rect.x += difference;
					top->separationBar->rect.width += difference;
				}
			}
		}
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


                if (fastRender && moving)
                {
                    if (windowToAdd->separationBar->isHorizontal)
					{
						DrawSeparationBar(windowToAdd->separationBar->rect.x, position.y, windowToAdd->separationBar->rect.width, themeSplitter.themeFast.size, windowToAdd->separationBar->isHorizontal);
					}
					else
					{
						DrawSeparationBar(position.x, windowToAdd->separationBar->rect.y, themeSplitter.themeFast.size, windowToAdd->separationBar->rect.height, windowToAdd->separationBar->isHorizontal);
					}  
                }



					break;
				}
			}
		}
	}
    
    // if (!fastRender)
    //     this->ForceRefresh();
    this->Resize();
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
    
    if(width <= 0 || height <= 0)
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
			wxWindow * _wnd = windowToAdd->GetWindow();
			if (_wnd != nullptr)
			{
#ifdef _DEBUG
#ifdef WIN32
				TCHAR temp[255];
#endif
#endif
				if (_wnd->IsShown())
				{

					_wnd->SetSize(windowToAdd->rect);

#ifdef _DEBUG
#ifdef WIN32
					//wsprintf(temp, L"windowToAdd rect : x : %d, y : %d, width %d, height %d \n", windowToAdd->rect.x, windowToAdd->rect.y, windowToAdd->rect.width, windowToAdd->rect.height);
					//OutputDebugString(temp);
#endif
#endif
				}
				else
					_wnd->SetSize(rc);

				//_wnd->Refresh();
				//_wnd->Refresh();
				//_wnd->Update();

			}


			if (windowToAdd->separationBar != nullptr)
			{
				if (windowToAdd->separationBar->separationBar != nullptr)
				{
					if (windowToAdd->separationBar->separationBar->IsShown())
					{
#ifdef _DEBUG
#ifdef WIN32                        
						TCHAR temp[255];
#endif
#endif
						if (windowToAdd->separationBar->separationBar->IsShown())
						{
							windowToAdd->separationBar->separationBar->SetSize(windowToAdd->separationBar->rect);
#ifdef _DEBUG
#ifdef WIN32
							//wsprintf(temp, L"separationBar rect : x : %d, y : %d, width %d, height %d \n", windowToAdd->separationBar->rect.x, windowToAdd->separationBar->rect.y, windowToAdd->separationBar->rect.width, windowToAdd->separationBar->rect.height);
							//OutputDebugString(temp);
#endif
#endif
						}
						else
							windowToAdd->separationBar->separationBar->SetSize(rc);

						//windowToAdd->separationBar->separationBar->Refresh();
						//windowToAdd->separationBar->separationBar->Refresh();
						//windowToAdd->separationBar->separationBar->Update();
					}
				}
			}
		}
	}

	oldWidth = width;
	oldHeight = height;

}

