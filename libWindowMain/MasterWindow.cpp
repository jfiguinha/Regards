#include <header.h>
#include "MasterWindow.h"
#include "WindowUtility.h"
#include <wx/busyinfo.h>
#include <window_id.h>
#include <LibResource.h>
using namespace Regards::Window;


bool CMasterWindow::endProgram = false;
vector<CMasterWindow*> CMasterWindow::listMainWindow;
vector<CMasterWindow*> CMasterWindow::listProcessWindow;
bool CMasterWindow::stopProcess = false;

void CMasterWindow::StopAllProcess(const wxString& title, const wxString& message, wxWindow* parentWindow,
                                   const int& nbTry)
{
	SetStopProcess(true);
	wxMilliSleep(100);
	wxWindowDisabler disableAll;
	wxString libelle = CLibResource::LoadStringFromResource(L"LBLSTOPWORKINGMSG", 1);


	auto wait = new wxBusyInfo(libelle);

	int i = 0;
	bool all_stop;
	do
	{
		all_stop = true;
		//int j = 0;
		for (CMasterWindow* window : listProcessWindow)
		{
			wxString message1 = window->GetWaitingMessage();
			wxTheApp->Yield();

			if (!window->GetProcessEnd())
			{
				all_stop = false;
				wxMilliSleep(500);
				i++;
				break;
			}
		}
	}
	while (!all_stop && i < nbTry);

	delete wait;
}

void CMasterWindow::ThreadIdle(void* data)
{
	auto main = static_cast<CMasterWindow*>(data);
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
	processStop = false;
	windowMainPimpl = new CWindowMainPimpl();
	processEnd = true;
	processIdle = false;
	id = listMainWindow.size();
	listMainWindow.push_back(this);
}

void CMasterWindow::CallRefresh(wxWindow* window)
{
	wxCommandEvent event(wxEVENT_REFRESH);
	wxPostEvent(window, event);
}

CMasterWindow::~CMasterWindow(void)
{
	if (id < listMainWindow.size())
		listMainWindow[id] = nullptr;

	if (windowMainPimpl != nullptr)
		delete windowMainPimpl;
}

void CMasterWindow::FillRect(wxDC* dc, const wxRect& rc, const wxColour& color)
{
	CWindowUtility winUtility;
	winUtility.FillRect(dc, rc, color);
}

void CMasterWindow::DrawTexte(wxDC* dc, const wxString& libelle, const int& xPos, const int& yPos,
                              const CThemeFont& font)
{
	CWindowUtility winUtility;
	winUtility.DrawTexte(dc, libelle, xPos, yPos, font);
}

wxSize CMasterWindow::GetSizeTexte(wxDC* dc, const wxString& libelle, const CThemeFont& font)
{
	CWindowUtility winUtility;
	return winUtility.GetSizeTexte(dc, libelle, font);
}


wxRect CMasterWindow::GetWindowRect()
{
	auto rc = wxRect(0, 0, windowMainPimpl->width, windowMainPimpl->height);
	return rc;
}

void CMasterWindow::SetWindowHeight(const int& height)
{
	windowMainPimpl->height = height;
}

int CMasterWindow::GetWindowHeight()
{
	return windowMainPimpl->height;
}

void CMasterWindow::SetWindowWidth(const int& width)
{
	windowMainPimpl->width = width;
}

int CMasterWindow::GetWindowWidth()
{
	return windowMainPimpl->width;
}

void CMasterWindow::StartThread()
{
	if (processIdle && windowMainPimpl->threadIdle == nullptr && !endProgram && !stopProcess)
	{
		windowMainPimpl->threadIdle = new thread(ThreadIdle, this);
		processEnd = false;
	}
}

void CMasterWindow::SetEndProgram()
{
	endProgram = true;
}

void CMasterWindow::SetStopProcess(const bool& state)
{
	stopProcess = state;
}

void CMasterWindow::SetStartProcess()
{
	processStop = false;
}

bool CMasterWindow::GetProcessEnd()
{
	return processEnd;
}

bool CMasterWindow::GetProcessStop()
{
	return processStop;
}
