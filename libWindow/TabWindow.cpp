#include <header.h>
#include "ToolbarWindow.h"
#include "TabWindow.h"
using namespace Regards::Window;

CTabWindow::CTabWindow(const wxString& windowName, wxWindow* parent, wxWindowID id)
	: CWindowMain(windowName, parent, id)
{
	Connect(wxEVT_SIZE, wxSizeEventHandler(CTabWindow::OnSize));

}


CTabWindow::~CTabWindow()
{
	for (CTabWindowData* window : listWindow)
	{
		if (window != nullptr)
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
			window->UpdateScreenRatio();
	}

	if (toolbarWindow != nullptr)
		toolbarWindow->UpdateScreenRatio();

	this->Resize();
}

void CTabWindow::ClickShowButton(const int& id, const int& refresh)
{
	HideAllWindow();
	for (CTabWindowData* window : listWindow)
	{
		if (window->GetId() == id)
		{
			windowVisible = id;
			LoadInfo();
			window->ShowWindow();
			break;
		}
	}

	Resize();
}


void CTabWindow::OnSize(wxSizeEvent& event)
{
	const int pictureWidth = event.GetSize().GetWidth();

	if (const int pictureHeight = event.GetSize().GetHeight(); pictureWidth > 0 && pictureHeight > 0)
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
			window->ShowWindow(false);
		}
	}
}

void CTabWindow::Resize()
{
	wxRect rc_affichage_bitmap;

	rc_affichage_bitmap.x = 0;
	rc_affichage_bitmap.width = width;
	rc_affichage_bitmap.height = height - toolbarWindow->GetHeight();

	if (toolbarWindow != nullptr)
	{
		toolbarWindow->SetSize(rc_affichage_bitmap.x, 0, rc_affichage_bitmap.width, toolbarWindow->GetHeight());
		toolbarWindow->Refresh();
	}

	wxWindow* windowToShow = nullptr;

	for (CTabWindowData* window : listWindow)
	{
		if (window != nullptr)
		{
			if (window != nullptr && window->IsShown())
				windowToShow = window->GetWindow();
		}
	}

	if (windowToShow != nullptr)
	{
		windowToShow->SetSize(rc_affichage_bitmap.x, toolbarWindow->GetHeight(), rc_affichage_bitmap.width,
		                      rc_affichage_bitmap.height);
		windowToShow->Refresh();
	}
}
