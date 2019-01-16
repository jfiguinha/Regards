#include "MasterWindow.h"
#include "WindowUtility.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif

class CWindowMainPimpl
{
public:
	CWindowMainPimpl()
	{
		threadIdle = nullptr;
		width = 0;
		height = 0;
		sleeptime = 50;
	}

	void DeleteThread()
	{
		if (threadIdle != nullptr)
		{
			threadIdle->join();
			delete(threadIdle);
			threadIdle = nullptr;
		}
	}

	thread * threadIdle;
	int width;
	int height;
	unsigned long sleeptime;
};

using namespace Regards::Window;

void CMasterWindow::ThreadIdle(void * data)
{
	CMasterWindow * main = (CMasterWindow *)data;
	if (main != nullptr && !endProgram)
		main->ProcessIdle();
	main->PushThreadIdleEvent();
}

void CMasterWindow::ProcessOnIdleEndEvent(wxCommandEvent& event)
{
	windowMainPimpl->DeleteThread();
	processEnd = true;
}

void CMasterWindow::ProcessOnSizeEvent(wxSizeEvent& event)
{               
	int _width = event.GetSize().GetX();
	int _height = event.GetSize().GetY();
	if (_width <= 20 && _height <= 20)
	{
		//not corrected size
		//Resize();
	}
	else
	{
		windowMainPimpl->width = _width * scaleFactor;
		windowMainPimpl->height = _height * scaleFactor;
		Resize();
	}
	
#if defined(WIN32) && defined(_DEBUG)
	wxString toShow = name + " size x : " + to_string(_width) + " y : " + to_string(_height) + "\n";
	OutputDebugString(toShow.ToStdWstring().c_str());
#endif
}

CMasterWindow::CMasterWindow(void)
{
	windowMainPimpl = new CWindowMainPimpl();
	processEnd = true;
	processIdle = false;
	listMainWindow.push_back(this);

}

CMasterWindow::~CMasterWindow(void)
{
	if(windowMainPimpl != nullptr)
		delete windowMainPimpl;
}

void CMasterWindow::CallRefresh(wxWindow * window)
{
    wxCommandEvent event(wxEVENT_VIDEOREFRESH);
    wxPostEvent(window, event);  
}

void CMasterWindow::FastRefresh(wxWindow * window,const bool &special)
{
    if(window != nullptr)
    {
    #if defined(__APPLE__)
        if(special)
        {
            wxPaintEvent event(wxEVT_PAINT);
            wxPostEvent(window, event);            
        }
        else
            window->Refresh();
    #elif defined(__WXGTK__)
        wxCommandEvent event(wxEVENT_VIDEOREFRESH);
        wxPostEvent(window, event);  
    #else
        window->Refresh();
    #endif 
    }
}
 
 void CMasterWindow::OnRefresh(wxCommandEvent& event)
 {
     
 }

void CMasterWindow::FillRect(wxDC * dc, const wxRect &rc, const wxColour &color)
{
	CWindowUtility winUtility;
	winUtility.FillRect(dc, rc, color);
}

void CMasterWindow::DrawTexte(wxDC * dc, const wxString &libelle, const int &xPos, const int &yPos, const CThemeFont &font)
{
	CWindowUtility winUtility;
	winUtility.DrawTexte(dc, libelle, xPos, yPos, font);
}

wxSize CMasterWindow::GetSizeTexte(wxDC * dc, const wxString &libelle, const CThemeFont &font)
{
	CWindowUtility winUtility;
	return winUtility.GetSizeTexte(dc, libelle, font);
}




wxRect CMasterWindow::GetWindowRect()
{
	wxRect rc = wxRect(0,0, windowMainPimpl->width,windowMainPimpl->height);
	return rc;
}

void CMasterWindow::SetWindowHeight(const int &height)
{

	windowMainPimpl->height = height;
}

int CMasterWindow::GetWindowHeight()
{
	return windowMainPimpl->height;
}

void CMasterWindow::SetWindowWidth(const int &width)
{
	windowMainPimpl->width = width;
}

int CMasterWindow::GetWindowWidth()
{
	return windowMainPimpl->width;
}

void CMasterWindow::StartThread()
{
	if (processIdle && windowMainPimpl->threadIdle == nullptr && !endProgram)
	{
		windowMainPimpl->threadIdle = new thread(ThreadIdle, this);
		processEnd = false;
	}
}

void CMasterWindow::SetEndProgram()
{
	endProgram = true;
}


bool CMasterWindow::GetProcessEnd()
{
	return processEnd;
}

