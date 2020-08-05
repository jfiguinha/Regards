#pragma once
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include "MasterWindow.h"
using namespace std;


namespace Regards
{
	namespace Window
	{
		class CWindowMain : public wxWindow, public CMasterWindow
		{
		public:
			CWindowMain(wxString name, wxWindow* parent, wxWindowID id)
				: wxWindow(parent, id, wxPoint(0, 0), wxSize(0, 0), 0)
				//: wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
			{
				this->name = name;
            #ifdef __WXGTK__
            #if wxCHECK_VERSION(3, 1, 2)
                scaleFactor = 1.0f;
            #else
                scaleFactor = GetContentScaleFactor();
            #endif
            #else
                scaleFactor = 1.0f;
            #endif
				Connect(wxEVT_SIZE, wxSizeEventHandler(CWindowMain::OnSize));
                Connect(wxEVENT_REFRESH, wxCommandEventHandler(CWindowMain::OnRefresh));
				Connect(wxEVENT_RESIZE, wxCommandEventHandler(CWindowMain::OnResize));
				Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(CWindowMain::OnEraseBackground));
				Connect(wxEVENT_IDLETHREADING, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CWindowMain::OnProcessIdleEnd));
			}

			virtual ~CWindowMain()
			{

			}


			virtual void OnResize(wxCommandEvent& event)
			{
				this->Resize();
			}
            
            virtual void OnRefresh(wxCommandEvent& event)
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

			virtual void ForceRefresh()
			{
				this->Refresh();
				this->Update();
			}

			virtual void Resize()
			{
				this->Refresh();
				this->Update();
			}

			virtual void SetSizeWindow(const int &x, const int &y, const int &width, const int &height)
			{
				this->SetSize(x,y,width,height);
				this->Refresh();
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
