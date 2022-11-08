// ReSharper disable All
#pragma once
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include "MasterWindow.h"

namespace Regards
{
	namespace Window
	{
		class CWindowMain : public wxWindow, public CMasterWindow
		{
		public:
			CWindowMain(const wxString& name, wxWindow* parent, const wxWindowID id);

			~CWindowMain() override
			{
			}


			virtual void SaveParameter()
			{
			}

			virtual void OnResize(wxCommandEvent& event)
			{
				this->Resize();
			}

			virtual void OnRefresh(wxCommandEvent& event)
			{
                needToRefresh = true;
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

			void Resize() override
			{
                needToRefresh = true;
			}

			virtual void SetSizeWindow(const int& x, const int& y, const int& width, const int& height)
			{
				this->SetSize(x, y, width, height);
				needToRefresh = true;
			}

			virtual void OnEraseBackground(wxEraseEvent& event)
			{
			}

			virtual void OnSize(wxSizeEvent& event)
			{
				this->ProcessOnSizeEvent(event);
                needToRefresh = true;
			}

			wxString GetWaitingMessage() override
			{
				return "Window waiting : " + to_string(this->GetId());
			}
            
            virtual void IdleFunction() override
            {
                
            }
            
            virtual void OnIdle(wxIdleEvent& evt)
            {
                if(needToRefresh)
                {
                    this->Refresh();
                    needToRefresh = false;
                }  
                
                IdleFunction();
            }

            bool needToRefresh = false;
			wxString name_;
		};
        


		inline CWindowMain::CWindowMain(const wxString& name, wxWindow* parent, const wxWindowID id):
			wxWindow(parent, id, wxPoint(0, 0), wxSize(0, 0), 0), name_(std::move(name))
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
			Connect(wxEVENT_IDLETHREADING, wxEVT_COMMAND_TEXT_UPDATED,
			        wxCommandEventHandler(CWindowMain::OnProcessIdleEnd));
            Connect(wxEVT_IDLE, wxIdleEventHandler(CWindowMain::OnIdle));
		}

		using WindowMainVector = std::vector<CWindowMain*>;
	}
}
