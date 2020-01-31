#include <header.h>
#include "ToolbarWindow.h"
#include "TabWindow.h"
using namespace Regards::Window;

CTabWindow::CTabWindow(const wxString & windowName, wxWindow* parent, wxWindowID id)
	: CWindowMain(windowName, parent, id)
{
	Connect(wxEVT_SIZE, wxSizeEventHandler(CTabWindow::OnSize));
    Connect(wxEVT_PAINT, wxPaintEventHandler(CTabWindow::OnPaint));
}

void CTabWindow::OnPaint(wxPaintEvent & event)
{
	if (width == 0 || height == 0)
		return;

	wxPaintDC dc(this);
	wxRect rc = GetRect();
	this->FillRect(&dc, rc, themeBitmap.colorScreen);

}

CTabWindow::~CTabWindow()
{
    for (CTabWindowData* window : listWindow)
    {
        if(window != nullptr)
        {
            delete window;
            window = nullptr;
        }
    }
}

void CTabWindow::UpdateScreenRatio()
{
	for (CTabWindowData* window : listWindow)
	{
		if (window != nullptr)
            if(window->windowMain != nullptr)
                window->windowMain->UpdateScreenRatio();
	}

	if(toolbarWindow != nullptr)
		toolbarWindow->UpdateScreenRatio();

	this->Resize();
}

void CTabWindow::ClickShowButton(const int& id)
{
	HideAllWindow();
	for (CTabWindowData * window : listWindow)
	{
		if (window->windowName == id)
		{
			windowVisible = id;
			LoadInfo();
			window->window->Show(true);
			break;
		}
	}

	Resize();
}


void CTabWindow::OnSize(wxSizeEvent & event)
{

	int pictureWidth = event.GetSize().GetWidth();
	int pictureHeight = event.GetSize().GetHeight();

	if (pictureWidth > 0 && pictureHeight > 0)
	{
		width = pictureWidth;
		height = pictureHeight;
		Resize();
	}
}

void CTabWindow::HideAllWindow()
{
	for (CTabWindowData* window : listWindow)
	{
		if (window != nullptr)
		{
			if(window != nullptr)
				window->window->Show(false);
		}
	}
}

void CTabWindow::Resize()
{
	wxRect rcAffichageBitmap;

	rcAffichageBitmap.x = 0;
	rcAffichageBitmap.y = 0;
	rcAffichageBitmap.width = width;
	rcAffichageBitmap.height = height - toolbarWindow->GetHeight();

	if (toolbarWindow != nullptr)
	{
		toolbarWindow->SetSize(rcAffichageBitmap.x, 0, rcAffichageBitmap.width, toolbarWindow->GetHeight());
	}

	wxWindow* windowToShow = nullptr;

	for (CTabWindowData* window : listWindow)
	{
		if (window != nullptr)
		{
			if (window != nullptr && window->window->IsShown())
				windowToShow = window->window;
		}

	}

	if (windowToShow != nullptr)
	{
		windowToShow->SetSize(rcAffichageBitmap.x, toolbarWindow->GetHeight(), rcAffichageBitmap.width, rcAffichageBitmap.height);
	}

	Refresh();
	Update();
}
