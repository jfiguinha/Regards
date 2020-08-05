#include "header.h"
#include "Splitter.h"
using namespace Regards::Window;

CSplitter::CSplitter(wxWindow* parent, wxWindowID id, const CThemeSplitter & theme, const bool &horizontal)
	: CWindowMain("CSplitter",parent, id)
{
	posBar = 0;
	oldWidth = 0;
	oldHeight = 0;
	window1 = nullptr;
	window2 = nullptr;
	separationBar = nullptr;
	fixWindow1Size = false;
	window1Size = 0;
	fixWindow2Size = false;
	window2Size = 0;
	windowResize = false;
	fastRender = true;
	moving = false;
	windowMinimalSize = 100;
	showSeparationBar = true;
	this->themeSplitter = theme;
	posBar = 0;
	separationBar = new CSeparationBar(this, this, wxID_ANY, themeSplitter.themeSeparation);
	this->horizontal = horizontal;
	fixPosition = false;
	moving = false;
	taille = 0;
	posWindow = 0;
#ifdef WIN32
    fastRender = true;
#else
	fastRender = false;
#endif
	diff = 0;
	//Connect(wxEVT_PAINT, wxPaintEventHandler(CSplitter::OnPaint));

}

void CSplitter::GenerateHorizontalRenderBitmap()
{
	if (window1 == nullptr && window2 == nullptr)
		return;

	//int posX = 0;
	int posY = 0;
	renderBitmap = wxBitmap(GetWindowWidth(), GetWindowHeight());
	wxMemoryDC dcSplitter(renderBitmap);

	if (window1->IsShown())
	{
		wxWindowDC dc(window1);
		dcSplitter.Blit(0, 0, GetWindowWidth(), GetWindowHeight(), &dc, 0, 0);
		posY += dc.GetSize().GetHeight();
	}

	if (separationBar->IsShown())
	{
		wxWindowDC dc(separationBar);
		dcSplitter.Blit(0, posY, GetWindowWidth(), GetWindowHeight(), &dc, 0, 0);
		posY += dc.GetSize().GetHeight();
	}
	if (window2->IsShown())
	{
		wxWindowDC dc(window2);
		dcSplitter.Blit(0, posY, GetWindowWidth(), GetWindowHeight(), &dc, 0, 0);
	}
    dcSplitter.SelectObject(wxNullBitmap);
}
/*
void CSplitter::OnPaint(wxPaintEvent& event)
{
    int width = GetWindowWidth();
    int height = GetWindowHeight();
    if(width == 0 || height == 0)
        return;

    
	//wxPaintDC dc(this);
	wxRect rc = GetWindowRect();
	FillRect(&dc, rc, themeSplitter.colorBack);
}
*/
void CSplitter::GenerateVerticalRenderBitmap()
{
	if (window1 == nullptr && window2 == nullptr)
		return;

	int posX = 0;
	//int posY = 0;
	renderBitmap = wxBitmap(GetWindowWidth(), GetWindowHeight());
	wxMemoryDC dcSplitter(renderBitmap);
	
	if (window1->IsShown())
	{
		wxWindowDC dc(window1);
		dcSplitter.Blit(0, 0, GetWindowWidth(), GetWindowHeight(), &dc, 0, 0);
		posX += dc.GetSize().GetWidth();
	}

	if (separationBar->IsShown())
	{
		wxWindowDC dc(separationBar);
		dcSplitter.Blit(posX, 0, GetWindowWidth(), GetWindowHeight(), &dc, 0, 0);
		posX += dc.GetSize().GetWidth();
	}

	if (window2->IsShown())
	{
		wxWindowDC dc(window2);
		dcSplitter.Blit(posX, 0, GetWindowWidth(), GetWindowHeight(), &dc, 0, 0);
	}
    dcSplitter.SelectObject(wxNullBitmap);
}

void CSplitter::SetWindow(wxWindow * window1, wxWindow * window2)
{
	this->window1 = window1;
	this->window2 = window2;

}

void CSplitter::SetSeparationBarVisible(const bool& visible)
{
	separationBar->Show(visible);
	showSeparationBar = visible;
}

bool CSplitter::GetSeparationVisibility()
{
	return separationBar->IsShown();

}

void CSplitter::SetPosition(const int &pos)
{
	posBar = pos;
	this->Resize(this);
}

int CSplitter::GetPosition()
{
	return posBar;
}


bool CSplitter::OnLButtonDown()
{
	moving = true;
	if (fastRender)
	{
		wxRect rc = window1->GetRect();
		if (horizontal)
		{	
			GenerateHorizontalRenderBitmap();
			posWindow = rc.GetBottom() + themeSplitter.themeSeparation.size;
		}
		else
		{
			GenerateVerticalRenderBitmap();
			posWindow = rc.GetRight() + themeSplitter.themeSeparation.size;
		}
		return true;
	}
	return true;
}

void CSplitter::OnLButtonUp()
{
	moving = false;
	if (fastRender)
	{
		Resize(this);
	}
}

void CSplitter::SetFixPosition(const bool &value)
{
	fixPosition = value;
	if (fixPosition == false)
	{
		fixWindow1Size = false;
		fixWindow2Size = false;
	}
}

void CSplitter::SetHorizontal(const bool &horizontal)
{
	this->horizontal = horizontal;
	separationBar->SetHorizontal(horizontal);
}

bool CSplitter::IsAllClose()
{
	if (!window1->IsShown() && !window2->IsShown())
		return true;

	return false;
}

CSplitter::~CSplitter()
{
	delete(separationBar);
}


void CSplitter::SetNewPosition(Regards::Window::CSeparationBar * separationBar)
{
	if (!fixPosition)
	{
		wxPoint mousePoint = wxGetMousePosition();
		wxPoint position = this->ScreenToClient(mousePoint);

		if (horizontal)
		{
			posBar = position.y;
		}
		else
		{
			posBar = position.x;
		}
		Resize(this);
	}
}

void CSplitter::SetWindow1FixPosition(const bool &fixPosition, const int &windowSize)
{
	window1Size = windowSize;
	fixWindow1Size = fixPosition;
}

void CSplitter::SetWindow2FixPosition(const bool &fixPosition, const int &windowSize)
{
	window2Size = windowSize;
	fixWindow2Size = fixPosition;
}

void CSplitter::DrawSeparationBar(const int &x, const int &y, const int &width, const int &height)
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

void CSplitter::ResizeEvent()
{

}

void CSplitter::Resize()
{
	windowResize = true;
	Resize(this);

	oldWidth = GetWindowWidth();
	oldHeight = GetWindowHeight();

	windowResize = false;

	ResizeEvent();
}

void CSplitter::ResizeVertical()
{
	int width = GetSize().GetX();
	int height = GetSize().GetY();

#if defined(WIN32) 
	wchar_t Temp[10];
	swprintf_s(Temp, L"%d", width);
	OutputDebugString(L"width : ");
	OutputDebugString(Temp);
	OutputDebugString(L"\n");

	swprintf_s(Temp, L"%d", height);
	OutputDebugString(L"height : ");
	OutputDebugString(Temp);
	OutputDebugString(L"\n");

	int window1Width = window1->GetSize().GetWidth();
	int window2Height = window1->GetSize().GetHeight();

	swprintf_s(Temp, L"%d", window1Width);
	OutputDebugString(L"window1Width : ");
	OutputDebugString(Temp);
	OutputDebugString(L"\n");

	swprintf_s(Temp, L"%d", window2Height);
	OutputDebugString(L"window2Height : ");
	OutputDebugString(Temp);
	OutputDebugString(L"\n");
#endif


	if (fastRender && moving)
	{
		int x = posBar;// +themeSplitter.themeSeparation.size / 2 + themeSplitter.themeFast.size;
		DrawSeparationBar(x, 0, themeSplitter.themeFast.size, GetWindowHeight());
	}
	else if (GetWindowWidth() > 0)
	{
		if (fixWindow1Size)
		{
			posBar = window1Size;
			diff = GetWindowWidth() - posBar;
			posWindow = taille = posBar;
		}
		else if (fixWindow2Size)
		{
			posBar = GetWindowWidth() - window2Size;
			diff = window2Size;
			posWindow = taille = posBar;
		}
		else
		{
			if (windowResize && oldWidth != 0)
			{
				if ((posBar + windowMinimalSize) > oldWidth)
					posBar = oldWidth - windowMinimalSize;
				else if (posBar < windowMinimalSize)
					posBar = windowMinimalSize;

				//Calcul new posBar
				float percent = (float)posBar / (float)oldWidth;
				posBar = percent * GetWindowWidth();
			}


			if (posBar == 0)
			{
				taille = GetWindowWidth() / 2 - (themeSplitter.themeSeparation.size / 2);
				posWindow = taille + themeSplitter.themeSeparation.size;
				diff = GetWindowWidth() - taille - (themeSplitter.themeSeparation.size / 2);
				posBar = taille + (themeSplitter.themeSeparation.size / 2);
			}
			else
			{
				if ((posBar + windowMinimalSize) > GetWindowWidth())
					posBar = GetWindowWidth() - windowMinimalSize;
				else if (posBar < windowMinimalSize)
					posBar = windowMinimalSize;

				taille = posBar - themeSplitter.themeSeparation.size;
				posWindow = taille + themeSplitter.themeSeparation.size;
				diff = GetWindowWidth() - posWindow;
			}
		}

		window2->SetSize(posWindow, 0, diff, GetWindowHeight());
		if (!fixWindow1Size && !fixWindow2Size)
			separationBar->SetSize(taille, 0, themeSplitter.themeSeparation.size, GetWindowHeight());
		window1->SetSize(0, 0, taille, GetWindowHeight());

	}
}

void CSplitter::ResizeHorizontal()
{
	int width = GetSize().GetX();
	int height = GetSize().GetY();

	if (fastRender && moving)
	{
		int y = posBar + themeSplitter.themeSeparation.size / 2 + themeSplitter.themeFast.size;
		DrawSeparationBar(0, y, GetWindowWidth(), themeSplitter.themeFast.size);
	}
	else if (GetWindowHeight() > 0)
	{
		if (fixWindow1Size)
		{
			posWindow = taille = posBar = window1Size;
			diff = GetWindowHeight() - posBar;
		}
		else if (fixWindow2Size)
		{
			posWindow = taille = posBar = GetWindowHeight() - window2Size;
			diff = window2Size;
		}
		else
		{
			if (windowResize && oldHeight != 0)
			{
				if ((posBar + windowMinimalSize)  > oldHeight)
					posBar = oldHeight - windowMinimalSize;
				else if (posBar < windowMinimalSize)
					posBar = windowMinimalSize;

				//Calcul new posBar
				float percent = (float)posBar / (float)oldHeight;
				posBar = percent * GetWindowHeight();
			}

			if (posBar == 0)
			{
				taille = GetWindowHeight() / 2 - (themeSplitter.themeSeparation.size / 2);
				posWindow = taille + themeSplitter.themeSeparation.size;
				diff = GetWindowHeight() - taille - (themeSplitter.themeSeparation.size / 2);
				posBar = taille + (themeSplitter.themeSeparation.size / 2);
			}
			else
			{
				if ((posBar + windowMinimalSize) > GetWindowHeight())
					posBar = GetWindowHeight() - windowMinimalSize;
				else if (posBar < windowMinimalSize)
					posBar = windowMinimalSize;

				taille = posBar - themeSplitter.themeSeparation.size;
				posWindow = taille + themeSplitter.themeSeparation.size;
				diff = GetWindowHeight() - posWindow;
			}
		}

		window1->SetSize(0, 0, GetWindowWidth(), taille);
		window1->PostSizeEvent();
		if (!fixWindow1Size && !fixWindow2Size)
			separationBar->SetSize(0, taille, GetWindowWidth(), themeSplitter.themeSeparation.size);
		window2->SetSize(0, posWindow, GetWindowWidth(), diff);
		window2->PostSizeEvent();

	}
}

void CSplitter::Resize(wxWindow * window)
{
	if (window1 != nullptr && window2 != nullptr)
	{
		if (window1->IsShown() && window2->IsShown())
		{
			if (fixWindow1Size || fixWindow2Size)
			{
				if (separationBar->IsShown())
					separationBar->Show(false);
			}
			else if (!separationBar->IsShown())
				separationBar->Show(true);

			if (horizontal)
				ResizeHorizontal();
			else
				ResizeVertical();
                
            if(window1 != nullptr)
            {
				window1->Refresh();
            }
        
            if(window2 != nullptr)
            {
				window2->Refresh();
            }
			separationBar->Refresh();
			return;
		}
	}

	if (separationBar != nullptr)
		if (separationBar->IsShown())
			separationBar->Show(false);

	if (window1 != nullptr)
		if (window1->IsShown())
		{
			window1->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
			window1->Refresh();
		}

	if (window2 != nullptr)
		if (window2->IsShown())
		{
			window2->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
			window2->Refresh();
		}

}

