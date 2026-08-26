#include "header.h"
#include "Splitter.h"
#include "SplitterImpl.h"
#include "SeparationBar.h"
#include <wx/log.h>

using namespace Regards::Window;

CSplitter::CSplitter(wxWindow* parent, wxWindowID id, const CThemeSplitter& theme, const bool& horizontal)
	: CWindowMain("CSplitter", parent, id),
	  m_impl(std::make_unique<CSplitterImpl>(theme))
{
	m_impl->horizontal = horizontal;
	m_impl->windowMinimalSize = DEFAULT_MINIMAL_WINDOW_SIZE;
	m_impl->separationBar = std::make_unique<CSeparationBar>(this, this, wxID_ANY, m_impl->themeSplitter.themeSeparation);
}


void CSplitter::GenerateHorizontalRenderBitmap()
{
	if (m_impl->window1 == nullptr && m_impl->window2 == nullptr)
		return;

	int posY = 0;
	m_impl->renderBitmap = wxBitmap(GetWindowWidth(), GetWindowHeight());
	wxMemoryDC dcSplitter(m_impl->renderBitmap);

	if (m_impl->window1->IsShown())
	{
		wxWindowDC dc(m_impl->window1);
		dcSplitter.Blit(0, 0, GetWindowWidth(), GetWindowHeight(), &dc, 0, 0);
		posY += dc.GetSize().GetHeight();
	}

	if (m_impl->separationBar->IsShown())
	{
		wxWindowDC dc(m_impl->separationBar.get());
		dcSplitter.Blit(0, posY, GetWindowWidth(), GetWindowHeight(), &dc, 0, 0);
		posY += dc.GetSize().GetHeight();
	}
	if (m_impl->window2->IsShown())
	{
		wxWindowDC dc(m_impl->window2);
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
	if (m_impl->window1 == nullptr && m_impl->window2 == nullptr)
		return;

	int posX = 0;
	m_impl->renderBitmap = wxBitmap(GetWindowWidth(), GetWindowHeight());
	wxMemoryDC dcSplitter(m_impl->renderBitmap);

	if (m_impl->window1->IsShown())
	{
		wxWindowDC dc(m_impl->window1);
		dcSplitter.Blit(0, 0, GetWindowWidth(), GetWindowHeight(), &dc, 0, 0);
		posX += dc.GetSize().GetWidth();
	}

	if (m_impl->separationBar->IsShown())
	{
		wxWindowDC dc(m_impl->separationBar.get());
		dcSplitter.Blit(posX, 0, GetWindowWidth(), GetWindowHeight(), &dc, 0, 0);
		posX += dc.GetSize().GetWidth();
	}

	if (m_impl->window2->IsShown())
	{
		wxWindowDC dc(m_impl->window2);
		dcSplitter.Blit(posX, 0, GetWindowWidth(), GetWindowHeight(), &dc, 0, 0);
	}
	dcSplitter.SelectObject(wxNullBitmap);
}

void CSplitter::SetWindow(wxWindow* window1, wxWindow* window2)
{
	m_impl->window1 = window1;
	m_impl->window2 = window2;
}

void CSplitter::SetSeparationBarVisible(const bool& visible)
{
	m_impl->separationBar->Show(visible);
	m_impl->showSeparationBar = visible;
}

bool CSplitter::GetSeparationVisibility() const
{
	return m_impl->separationBar->IsShown();
}

void CSplitter::SetPosition(const int& pos)
{
	m_impl->posBar = pos;
	this->Resize(this);
}

int CSplitter::GetPosition() const
{
	return m_impl->posBar;
}


bool CSplitter::OnLButtonDown()
{
	m_impl->moving = true;
	if (m_impl->fastRender)
	{
		wxRect rc = m_impl->window1->GetRect();
		if (m_impl->horizontal)
		{
			GenerateHorizontalRenderBitmap();
			m_impl->posWindow = rc.GetBottom() + m_impl->themeSplitter.themeSeparation.size;
		}
		else
		{
			GenerateVerticalRenderBitmap();
			m_impl->posWindow = rc.GetRight() + m_impl->themeSplitter.themeSeparation.size;
		}
		return true;
	}
	return true;
}

void CSplitter::OnLButtonUp()
{
	m_impl->moving = false;
	if (m_impl->fastRender)
	{
		Resize(this);
	}
}

void CSplitter::SetFixPosition(const bool& value)
{
	m_impl->fixPosition = value;
	if (m_impl->fixPosition == false)
	{
		m_impl->fixWindow1Size = false;
		m_impl->fixWindow2Size = false;
	}
}

void CSplitter::SetHorizontal(const bool& horizontal)
{
	m_impl->horizontal = horizontal;
	m_impl->separationBar->SetHorizontal(horizontal);
}

bool CSplitter::IsAllClose() const
{
	return !m_impl->window1->IsShown() && !m_impl->window2->IsShown();
}

void CSplitter::SetNewPosition(CSeparationBar* separationBar)
{
	if (!m_impl->fixPosition)
	{
		wxPoint mousePoint = wxGetMousePosition();
		wxPoint position = this->ScreenToClient(mousePoint);

		if (m_impl->horizontal)
		{
			m_impl->posBar = position.y;
		}
		else
		{
			m_impl->posBar = position.x;
		}
		Resize(this);
	}
}

void CSplitter::SetWindow1FixPosition(const bool& fixPosition, const int& windowSize)
{
	m_impl->window1Size = windowSize;
	m_impl->fixWindow1Size = fixPosition;
}

void CSplitter::SetWindow2FixPosition(const bool& fixPosition, const int& windowSize)
{
	m_impl->window2Size = windowSize;
	m_impl->fixWindow2Size = fixPosition;
}

void CSplitter::DrawSeparationBar(const int& x, const int& y, const int& width, const int& height)
{
	wxWindowDC dc(this);
	dc.DrawBitmap(m_impl->renderBitmap, 0, 0);

	if (m_impl->horizontal)
	{
		wxRect rc;
		rc.x = x;
		rc.y = y;
		rc.width = width;
		rc.height = height;
		dc.GradientFillLinear(rc, m_impl->themeSplitter.themeSeparation.secondColor, m_impl->themeSplitter.themeSeparation.firstColor,
							  wxSOUTH);
	}
	else
	{
		wxRect rc;
		rc.x = x;
		rc.y = y;
		rc.width = width;
		rc.height = height;
		dc.GradientFillLinear(rc, m_impl->themeSplitter.themeSeparation.secondColor, m_impl->themeSplitter.themeSeparation.firstColor,
							  wxEAST);
	}
}

void CSplitter::ResizeEvent()
{
}

void CSplitter::Resize()
{
	m_impl->windowResize = true;
	Resize(this);

	m_impl->oldWidth = GetWindowWidth();
	m_impl->oldHeight = GetWindowHeight();

	m_impl->windowResize = false;

	ResizeEvent();
}

void CSplitter::ResizeVertical()
{
	const int width = GetSize().GetX();
	const int height = GetSize().GetY();

	wxLogDebug("ResizeVertical: width=%d, height=%d", width, height);

	if (m_impl->window1 != nullptr)
	{
		const int window1Width = m_impl->window1->GetSize().GetWidth();
		const int window2Height = m_impl->window1->GetSize().GetHeight();
		wxLogDebug("ResizeVertical: window1Width=%d, window1Height=%d", window1Width, window2Height);
	}

	if (m_impl->fastRender && m_impl->moving)
	{
		int x = m_impl->posBar;
		DrawSeparationBar(x, 0, m_impl->themeSplitter.themeFast.size, GetWindowHeight());
	}
	else if (GetWindowWidth() > 0)
	{
		if (m_impl->fixWindow1Size)
		{
			m_impl->posBar = m_impl->window1Size;
			m_impl->diff = GetWindowWidth() - m_impl->posBar;
			m_impl->posWindow = m_impl->taille = m_impl->posBar;
		}
		else if (m_impl->fixWindow2Size)
		{
			m_impl->posBar = GetWindowWidth() - m_impl->window2Size;
			m_impl->diff = m_impl->window2Size;
			m_impl->posWindow = m_impl->taille = m_impl->posBar;
		}
		else
		{
			if (m_impl->windowResize && m_impl->oldWidth != 0)
			{
				if ((m_impl->posBar + m_impl->windowMinimalSize) > m_impl->oldWidth)
					m_impl->posBar = m_impl->oldWidth - m_impl->windowMinimalSize;
				else if (m_impl->posBar < m_impl->windowMinimalSize)
					m_impl->posBar = m_impl->windowMinimalSize;

				// Calculate new posBar based on percentage
				const float percent = static_cast<float>(m_impl->posBar) / static_cast<float>(m_impl->oldWidth);
				m_impl->posBar = static_cast<int>(percent * GetWindowWidth());
			}

			if (m_impl->posBar == 0)
			{
				m_impl->taille = GetWindowWidth() / 2 - (m_impl->themeSplitter.themeSeparation.size / 2);
				m_impl->posWindow = m_impl->taille + m_impl->themeSplitter.themeSeparation.size;
				m_impl->diff = GetWindowWidth() - m_impl->taille - (m_impl->themeSplitter.themeSeparation.size / 2);
				m_impl->posBar = m_impl->taille + (m_impl->themeSplitter.themeSeparation.size / 2);
			}
			else
			{
				if ((m_impl->posBar + m_impl->windowMinimalSize) > GetWindowWidth())
					m_impl->posBar = GetWindowWidth() - m_impl->windowMinimalSize;
				else if (m_impl->posBar < m_impl->windowMinimalSize)
					m_impl->posBar = m_impl->windowMinimalSize;

				m_impl->taille = m_impl->posBar - m_impl->themeSplitter.themeSeparation.size;
				m_impl->posWindow = m_impl->taille + m_impl->themeSplitter.themeSeparation.size;
				m_impl->diff = GetWindowWidth() - m_impl->posWindow;
			}
		}

		m_impl->window2->SetSize(m_impl->posWindow, 0, m_impl->diff, GetWindowHeight());
		if (!m_impl->fixWindow1Size && !m_impl->fixWindow2Size)
			m_impl->separationBar->SetSize(m_impl->taille, 0, m_impl->themeSplitter.themeSeparation.size, GetWindowHeight());
		m_impl->window1->SetSize(0, 0, m_impl->taille, GetWindowHeight());
	}
}

void CSplitter::ResizeHorizontal()
{
	if (m_impl->fastRender && m_impl->moving)
	{
		int y = m_impl->posBar + m_impl->themeSplitter.themeSeparation.size / 2 + m_impl->themeSplitter.themeFast.size;
		DrawSeparationBar(0, y, GetWindowWidth(), m_impl->themeSplitter.themeFast.size);
	}
	else if (GetWindowHeight() > 0)
	{
		if (m_impl->fixWindow1Size)
		{
			m_impl->posWindow = m_impl->taille = m_impl->posBar = m_impl->window1Size;
			m_impl->diff = GetWindowHeight() - m_impl->posBar;
		}
		else if (m_impl->fixWindow2Size)
		{
			m_impl->posWindow = m_impl->taille = m_impl->posBar = GetWindowHeight() - m_impl->window2Size;
			m_impl->diff = m_impl->window2Size;
		}
		else
		{
			if (m_impl->windowResize && m_impl->oldHeight != 0)
			{
				if ((m_impl->posBar + m_impl->windowMinimalSize) > m_impl->oldHeight)
					m_impl->posBar = m_impl->oldHeight - m_impl->windowMinimalSize;
				else if (m_impl->posBar < m_impl->windowMinimalSize)
					m_impl->posBar = m_impl->windowMinimalSize;

				// Calculate new posBar based on percentage
				const float percent = static_cast<float>(m_impl->posBar) / static_cast<float>(m_impl->oldHeight);
				m_impl->posBar = static_cast<int>(percent * GetWindowHeight());
			}

			if (m_impl->posBar == 0)
			{
				m_impl->taille = GetWindowHeight() / 2 - (m_impl->themeSplitter.themeSeparation.size / 2);
				m_impl->posWindow = m_impl->taille + m_impl->themeSplitter.themeSeparation.size;
				m_impl->diff = GetWindowHeight() - m_impl->taille - (m_impl->themeSplitter.themeSeparation.size / 2);
				m_impl->posBar = m_impl->taille + (m_impl->themeSplitter.themeSeparation.size / 2);
			}
			else
			{
				if ((m_impl->posBar + m_impl->windowMinimalSize) > GetWindowHeight())
					m_impl->posBar = GetWindowHeight() - m_impl->windowMinimalSize;
				else if (m_impl->posBar < m_impl->windowMinimalSize)
					m_impl->posBar = m_impl->windowMinimalSize;

				m_impl->taille = m_impl->posBar - m_impl->themeSplitter.themeSeparation.size;
				m_impl->posWindow = m_impl->taille + m_impl->themeSplitter.themeSeparation.size;
				m_impl->diff = GetWindowHeight() - m_impl->posWindow;
			}
		}

		m_impl->window1->SetSize(0, 0, GetWindowWidth(), m_impl->taille);
		m_impl->window1->PostSizeEvent();
		if (!m_impl->fixWindow1Size && !m_impl->fixWindow2Size)
			m_impl->separationBar->SetSize(0, m_impl->taille, GetWindowWidth(), m_impl->themeSplitter.themeSeparation.size);
		m_impl->window2->SetSize(0, m_impl->posWindow, GetWindowWidth(), m_impl->diff);
		m_impl->window2->PostSizeEvent();
	}
}

void CSplitter::Resize(wxWindow* window)
{
	if (m_impl->window1 != nullptr && m_impl->window2 != nullptr)
	{
		if (m_impl->window1->IsShown() && m_impl->window2->IsShown())
		{
			if (m_impl->fixWindow1Size || m_impl->fixWindow2Size)
			{
				if (m_impl->separationBar->IsShown())
					m_impl->separationBar->Show(false);
			}
			else if (!m_impl->separationBar->IsShown())
				m_impl->separationBar->Show(true);

			if (m_impl->horizontal)
				ResizeHorizontal();
			else
				ResizeVertical();

			if (m_impl->window1 != nullptr)
			{
				m_impl->window1->Refresh();
			}

			if (m_impl->window2 != nullptr)
			{
				m_impl->window2->Refresh();
			}
			m_impl->separationBar->Refresh();
			return;
		}
	}

	if (m_impl->separationBar)
	{
		if (m_impl->separationBar->IsShown())
			m_impl->separationBar->Show(false);
	}

	if (m_impl->window1 != nullptr)
		if (m_impl->window1->IsShown())
		{
			m_impl->window1->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
			m_impl->window1->Refresh();
		}

	if (m_impl->window2 != nullptr)
		if (m_impl->window2->IsShown())
		{
			m_impl->window2->SetSize(0, 0, GetWindowWidth(), GetWindowHeight());
			m_impl->window2->Refresh();
		}
}
