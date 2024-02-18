#include "header.h"
#include "WindowToAdd.h"
#include "MasterWindow.h"
#include "PanelWithClickToolbar.h"
using namespace Regards::Window;

CWindowToAdd::CWindowToAdd(): position(), size(0), size_old(0), fixe(false), fixe_old(false), id(0), isTop(false),
                              isHide(false),
                              isPanel(false),
                              separationBar(nullptr)
{
	window = nullptr;
	windowOpengl = nullptr;
}

wxWindow* CWindowToAdd::GetWindow()
{
	if (window != nullptr)
		return window;
	return windowOpengl;
}

CMasterWindow* CWindowToAdd::GetMasterWindowPt()
{
	if (window != nullptr)
		return window;
	return windowOpengl;
}

CPanelWithClickToolbar* CWindowToAdd::GetPanel()
{
	if (isPanel)
	{
		if (window != nullptr)
			return static_cast<CPanelWithClickToolbar*>(window);
	}
	return nullptr;
}

void CWindowToAdd::SetWindow(CWindowMain* window, bool isPanel)
{
	this->windowOpengl = nullptr;
	this->window = window;
	this->isPanel = isPanel;
}

void CWindowToAdd::SetWindow(CWindowOpenGLMain* window, bool isPanel)
{
	this->window = nullptr;
	this->windowOpengl = window;
	this->isPanel = isPanel;
}

wxSize CWindowToAdd::GetSize()
{
	wxSize size;
	if (this->window != nullptr)
	{
		size.x = this->window->GetWidth();
		size.y = this->window->GetHeight();
	}
	if (this->windowOpengl != nullptr)
	{
		size.x = this->windowOpengl->GetWidth();
		size.y = this->windowOpengl->GetHeight();
	}
	return size;
}
