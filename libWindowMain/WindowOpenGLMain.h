#pragma once
#ifdef RENDEROPENGL
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
				: wxGLCanvas(parent, id, args, wxDefaultPosition, wxDefaultSize)
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
				Connect(wxEVENT_IDLETHREADING, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CWindowOpenGLMain::OnProcessIdleEnd));
			}

			virtual ~CWindowOpenGLMain()
			{

			}
            
             void OnRefresh(wxCommandEvent& event)
             {
                this->Refresh();
             }                 
            
			virtual void PushThreadIdleEvent()
			{
				wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_IDLETHREADING);
				GetEventHandler()->AddPendingEvent(evt);
			}

			virtual void OnProcessIdleEnd(wxCommandEvent& event)
			{
				this->ProcessOnIdleEndEvent(event);
			}

			virtual void OnEraseBackground(wxEraseEvent& event){};

			virtual void OnSize(wxSizeEvent& event)
			{
				this->ProcessOnSizeEvent(event);
			}

			virtual wxString GetWaitingMessage()
			{ 
				return "Window waiting : " + to_string(this->GetId());
			}

		};
	}
}
#endif