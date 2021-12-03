#pragma once
#include <GLcontext.h>
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include "MasterWindow.h"

extern int args[];
using namespace std;


namespace Regards
{
	namespace Window
	{
		class CWindowOpenGLMain : public wxGLCanvas, public CMasterWindow
		{
		public:
			CWindowOpenGLMain(wxString name, wxWindow* parent, wxWindowID id)
				: wxGLCanvas(parent, id, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
			{
#ifdef __WXGTK__
#if wxCHECK_VERSION(3, 1, 2)
                scaleFactor = 1.0f;
#else
                scaleFactor = GetContentScaleFactor();
#endif
#else
				scaleFactor = 1.0f;
#endif
				this->name = name;
				Connect(wxEVENT_REFRESH, wxCommandEventHandler(CWindowOpenGLMain::OnRefresh));
				Connect(wxEVT_SIZE, wxSizeEventHandler(CWindowOpenGLMain::OnSize));
				Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(CWindowOpenGLMain::OnEraseBackground));
				Connect(wxEVENT_IDLETHREADING, wxEVT_COMMAND_TEXT_UPDATED,
				        wxCommandEventHandler(CWindowOpenGLMain::OnProcessIdleEnd));
			}

			~CWindowOpenGLMain() override
			{
			}

			virtual void OnRefresh(wxCommandEvent& event)  
			{
				this->Refresh();
				this->Update();
			}

			void PushThreadIdleEvent() override
			{
				wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_IDLETHREADING);
				GetEventHandler()->AddPendingEvent(evt);
			}

			virtual void OnProcessIdleEnd(wxCommandEvent& event)
			{
				this->ProcessOnIdleEndEvent(event);
			}

			virtual void OnEraseBackground(wxEraseEvent& event)
			{
			};

			virtual void OnSize(wxSizeEvent& event)
			{
				this->ProcessOnSizeEvent(event);
				//this->Refresh();
				//this->Update();
			}

			wxString GetWaitingMessage() override
			{
				return "Window waiting : " + to_string(this->GetId());
			}

			virtual void ForceRefresh()
			{
				this->Refresh();
				this->Update();
			}
		};
	}
}
