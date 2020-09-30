#include <header.h>
#include "GpsEngine.h"
using namespace Regards::Window;

CListOfWindow * CGpsEngine::listofWindow = nullptr;

CListOfWindow::CListOfWindow()
{
	
}

void CListOfWindow::AddWindow(Regards::Window::CWindowMain * windowMain)
{
	if (windowMain != nullptr)
		listOfWinListener.push_back(windowMain);
}
void CListOfWindow::SendMessageToWindow(const wxString &filename, const int &typeData)
{
	for (CWindowMain * window : listOfWinListener)
	{
		if (window != nullptr)
		{
			wxString * gpsInfos = new wxString(filename);
			wxCommandEvent* event = new wxCommandEvent(wxEVENT_UPDATEGPSINFOS);
			event->SetInt(typeData);
			event->SetClientData(gpsInfos);
			wxQueueEvent(window, event);
		}
	}
}



CGpsEngine::CGpsEngine()
{
	
}

CGpsEngine::~CGpsEngine()
{
	
}

CListOfWindow * CGpsEngine::getInstance()
{
	Initialize();
	return listofWindow;
}
void CGpsEngine::Initialize()
{
	if(listofWindow == nullptr)
	{
		listofWindow = new CListOfWindow();
	}
}
void CGpsEngine::kill()
{
	if (listofWindow == nullptr)
		delete listofWindow;
}