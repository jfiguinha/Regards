#include <header.h>
#include "GpsEngine.h"
#include "FileGeolocation.h"
using namespace Regards::Internet;

CListOfWindowGeo * CGpsEngine::file_geolocation_ = nullptr;

CListOfWindowGeo::CListOfWindowGeo()
{
	
}

void CListOfWindowGeo::AddWindow(Regards::Window::CWindowMain * windowMain)
{
	if (windowMain != nullptr)
		listOfWinListener.push_back(windowMain);
}
void CListOfWindowGeo::SendMessageToWindow(const wxString &filename, const int &typeData)
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

CListOfWindowGeo * CGpsEngine::getInstance()
{
	Initialize();
	return file_geolocation_;
}
void CGpsEngine::Initialize()
{
	if(file_geolocation_ == nullptr)
	{
		file_geolocation_ = new CListOfWindowGeo();
	}
}
void CGpsEngine::kill()
{
	if (file_geolocation_ == nullptr)
		delete file_geolocation_;
}