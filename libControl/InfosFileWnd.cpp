#include <header.h>
#include "InfosFileWnd.h"
#include <TreeWindow.h>
#include <ScrollbarWnd.h>
#include <window_id.h>


using namespace Regards::Control;
using namespace Regards::Window;

#define TAILLEMAX 4096

wxDEFINE_EVENT(EVENT_UPDATEINFOSTHREAD, wxCommandEvent);

CInfosFileWnd::CInfosFileWnd(wxWindow* parent, wxWindowID id, const CThemeScrollBar& themeScroll,
                             const CThemeTree& theme)
	: CTreeWithScrollbar("CInfosFileWnd", parent, id, themeScroll, theme)
{
	//infosFile = nullptr;
	infosFile = nullptr;
	Connect(EVENT_UPDATEINFOSTHREAD, wxCommandEventHandler(CInfosFileWnd::UpdateTreeInfosEvent));
}

void CInfosFileWnd::UpdateTreeInfosEvent(wxCommandEvent& event)
{
	std::unique_ptr<CThreadLoadInfos> threadInfos(static_cast<CThreadLoadInfos*>(event.GetClientData()));
	if(threadInfos->filename == filename)
	{
		threadInfos->infosFileWnd->CreateElement();
		//StopLoadingPicture(InfosFileScroll);
		auto wx_command_event = new wxCommandEvent(wxEVT_STOPANIMATION);
		wx_command_event->SetClientData(this);
		wxQueueEvent(this->GetParent(), wx_command_event);

		treeWindow->SetTreeControl(threadInfos->infosFileWnd.get());

		infosFile = std::move(threadInfos->infosFileWnd);
	}

	threadInfos->threadLoadInfos.join();
}

void CInfosFileWnd::InfosUpdate(const wxString& filename)
{
	if (filename != this->filename)
	{
		this->filename = filename;
		auto threadInfos = new CThreadLoadInfos();
		threadInfos->panelInfos = this;
		threadInfos->filename = filename;
		threadInfos->threadLoadInfos = std::thread(GenerateTreeInfos, threadInfos);
	}
}

void CInfosFileWnd::GenerateTreeInfos(CThreadLoadInfos* threadInfos)
{
	threadInfos->infosFileWnd = std::make_unique<CInfosFile>(threadInfos->panelInfos->treeWindow->GetTheme(), threadInfos->panelInfos->treeWindow);
	threadInfos->infosFileWnd->SetFile(threadInfos->filename);

	auto event = new wxCommandEvent(EVENT_UPDATEINFOSTHREAD);
	event->SetClientData(threadInfos);
	wxQueueEvent(threadInfos->panelInfos, event);
}
