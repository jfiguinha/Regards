#include <header.h>
#include "GpsEngine.h"
using namespace Regards::Window;

std::unique_ptr<CListOfWindow> CGpsEngine::listofWindow = nullptr;

CListOfWindow::CListOfWindow()
{
}

void CListOfWindow::AddWindow(CWindowMain* windowMain)
{
	if (windowMain != nullptr)
		listOfWinListener.push_back(windowMain);
}

void CListOfWindow::SendMessageToWindow(const wxString& filename, const int& typeData)
{
	for (CWindowMain* window : listOfWinListener)
	{
		if (window != nullptr)
		{
			auto gpsInfos = new wxString(filename);
			auto event = new wxCommandEvent(wxEVENT_UPDATEGPSINFOS);
			event->SetInt(typeData);
			event->SetClientData(gpsInfos);
			wxQueueEvent(window, event);
		}
	}
}

CListOfWindow* CGpsEngine::getInstance()
{
	Initialize();
	return listofWindow.get();
}

void CGpsEngine::Initialize()
{
	if (listofWindow == nullptr)
	{
		listofWindow = std::make_unique<CListOfWindow>();
	}
}
