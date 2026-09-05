#include <header.h>
#include "MasterWindow.h"
#include "WindowUtility.h"
#include <wx/busyinfo.h>
#include <LibResource.h>
using namespace Regards::Window;

std::atomic_bool CMasterWindow::endProgram = false;
tbb::concurrent_vector<CMasterWindow*> CMasterWindow::listMainWindow;
tbb::concurrent_vector<CMasterWindow*> CMasterWindow::listProcessWindow;
std::atomic_bool CMasterWindow::stopProcess = false;

void CMasterWindow::StopAllProcess(const wxString& title, const wxString& message, wxWindow* parentWindow,
                                   const int& nbTry)
{
	SetStopProcess(true);
	wxMilliSleep(100);
	wxWindowDisabler disableAll;
	wxString libelle = CLibResource::LoadStringFromResource(L"LBLSTOPWORKINGMSG", 1);

	std::unique_ptr<wxBusyInfo> wait = std::make_unique<wxBusyInfo>(libelle);

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
				wxMilliSleep(20);
				i++;
				break;
			}
		}
	}
	while (!all_stop && i < nbTry);

}

void CMasterWindow::ProcessOnSizeEvent(wxWindow* window, wxSizeEvent& event)
{
	const wxSize clientSize = window->GetClientSize();
	int _width = clientSize.GetWidth();
	int _height = clientSize.GetHeight();
	if (_width <= 20 && _height <= 20)
	{
		//not corrected size
		//Resize();
	}
	else
	{
		width = _width * scaleFactor;
		height = _height * scaleFactor;
		Resize();
	}

#if defined(WIN32) && defined(_DEBUG)
	wxString toShow = name + " size x : " + to_string(_width) + " y : " + to_string(_height) + "\n";
	OutputDebugString(toShow.ToStdWstring().c_str());
#endif
}

CMasterWindow::CMasterWindow(void)
{

	processEnd = true;
	processIdle = false;
	id = listMainWindow.size();
	listMainWindow.push_back(this);
}

CMasterWindow::~CMasterWindow(void)
{
	listMainWindow.clear();
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
	auto rc = wxRect(0, 0, width, height);
	return rc;
}

void CMasterWindow::SetWindowHeight(const int& height)
{
	this->height = height;
}

int CMasterWindow::GetWindowHeight()
{
	return height;
}

void CMasterWindow::SetWindowWidth(const int& width)
{
	this->width = width;
}

int CMasterWindow::GetWindowWidth()
{
	return width;
}

void CMasterWindow::SetEndProgram()
{
	endProgram = true;
}

bool CMasterWindow::GetEndProgram()
{
	return endProgram;
}

void CMasterWindow::SetStopProcess(const bool& state)
{
	stopProcess = state;
	endProgram = false;
}

void CMasterWindow::SetStartProcess()
{
	stopProcess = false;
}

bool CMasterWindow::GetProcessEnd()
{
	return processEnd;
}

bool CMasterWindow::GetProcessStop()
{
	return stopProcess;
}
