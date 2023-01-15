#include <header.h>
#include "InfosFileWnd.h"


#include <TreeWindow.h>
#include <window_id.h>


using namespace Regards::Control;
using namespace Regards::Window;

#define TAILLEMAX 4096

wxDEFINE_EVENT(EVENT_UPDATEINFOSTHREAD, wxCommandEvent);

CInfosFileWnd::CInfosFileWnd(wxWindow* parent, wxWindowID id, const CThemeScrollBar& themeScroll,
                             const CThemeTree& theme)
	: CTreeWithScrollbar("CInfosFileWnd", parent, id, themeScroll, theme)
{
	infosFile = nullptr;
	oldInfosFileControl = nullptr;
	Connect(EVENT_UPDATEINFOSTHREAD, wxCommandEventHandler(CInfosFileWnd::UpdateTreeInfosEvent));
}

CInfosFileWnd::~CInfosFileWnd(void)
{
	if (oldInfosFileControl != nullptr)
		delete(oldInfosFileControl);
}

void CInfosFileWnd::UpdateTreeInfosEvent(wxCommandEvent& event)
{
	auto threadInfos = static_cast<CThreadLoadInfos*>(event.GetClientData());
	// if(threadInfos->filename == filename)
	{
		threadInfos->infosFileWnd->CreateElement();
		//StopLoadingPicture(InfosFileScroll);
		auto wx_command_event = new wxCommandEvent(wxEVT_STOPANIMATION);
		wx_command_event->SetClientData(this);
		wxQueueEvent(this->GetParent(), wx_command_event);

		treeWindow->SetTreeControl(threadInfos->infosFileWnd);

		delete(oldInfosFileControl);
		oldInfosFileControl = threadInfos->infosFileWnd;
	}

	threadInfos->threadLoadInfos->join();
	delete threadInfos->threadLoadInfos;
	delete threadInfos;
}

void CInfosFileWnd::InfosUpdate(const wxString& filename)
{
	if (filename != this->filename)
	{
		this->filename = filename;
		auto infosFileWnd = new CInfosFile(treeWindow->GetTheme(), treeWindow);
		auto threadInfos = new CThreadLoadInfos();
		threadInfos->infosFileWnd = infosFileWnd;
		threadInfos->panelInfos = this;
		threadInfos->filename = filename;
		threadInfos->threadLoadInfos = new thread(GenerateTreeInfos, threadInfos);

		auto event = new wxCommandEvent(wxEVT_STARTANIMATION);
		event->SetClientData(this);
		wxQueueEvent(this->GetParent(), event);
	}
}

void CInfosFileWnd::GenerateTreeInfos(CThreadLoadInfos* threadInfos)
{
	CInfosFile* infosFileWnd = threadInfos->infosFileWnd;
	infosFileWnd->SetFile(threadInfos->filename);

	auto event = new wxCommandEvent(EVENT_UPDATEINFOSTHREAD);
	event->SetClientData(threadInfos);
	wxQueueEvent(threadInfos->panelInfos, event);
}
