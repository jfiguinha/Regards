#include "header.h"
#include "WindowToAdd.h"
#include "MasterWindow.h"
#include "PanelWithClickToolbar.h"
using namespace Regards::Window;

CWindowToAdd::CWindowToAdd()
{
	window = nullptr;
	windowOpengl = nullptr;
}

wxWindow * CWindowToAdd::GetWindow()
{
	if (window != nullptr)
		return window;
	return windowOpengl;
}

CMasterWindow * CWindowToAdd::GetMasterWindowPt()
{
	if (window != nullptr)
		return window;
	return windowOpengl;
}

CPanelWithClickToolbar * CWindowToAdd::GetPanel()
{
	if (isPanel)
	{
		if (window != nullptr)
			return (CPanelWithClickToolbar *)window;
	}
	return nullptr;
}

void CWindowToAdd::SetWindow(CWindowMain * window, bool isPanel)
{
	this->window = nullptr;
	this->windowOpengl = nullptr;
	this->window = window;
	this->isPanel = isPanel;
}

void CWindowToAdd::SetWindow(CWindowOpenGLMain * window, bool isPanel)
{
	this->window = nullptr;
	this->windowOpengl = nullptr;
	this->windowOpengl = window;
	this->isPanel = isPanel;
}

void CWindowToAdd::ForceRefresh()
{
	if (this->window != nullptr)
		this->window->ForceRefresh();
	else if (windowOpengl != nullptr)
		windowOpengl->ForceRefresh();
}