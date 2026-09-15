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
	{
		if (!listOfWinListener[windowMain->GetId()])
		{
			listOfWinListener[windowMain->GetId()] = windowMain;
		}	
	}
		
}

void CListOfWindow::RemoveWindow(CWindowMain* windowMain)
{
	if (windowMain != nullptr)
	{
		auto it = listOfWinListener.find(windowMain->GetId());
		if (it != listOfWinListener.end())
		{
			listOfWinListener.erase(it);
		}
	}
}

void CListOfWindow::SendMessageToWindow(const wxString& filename, const int& typeData)
{
	for (const auto& pair : listOfWinListener)
	{
		if (pair.second != nullptr)
		{
			auto gpsInfos = new wxString(filename);
			auto event = new wxCommandEvent(wxEVENT_UPDATEGPSINFOS);
			event->SetInt(typeData);
			event->SetClientData(gpsInfos);
			wxQueueEvent(pair.second, event);
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
