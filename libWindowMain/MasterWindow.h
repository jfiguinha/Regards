#pragma once
#include <theme.h>
#include <Tracing.h>
class CWindowMainPimpl;

namespace Regards
{
	namespace Window
	{
		class CMasterWindow
		{
		public:
			CMasterWindow(void);
			virtual ~CMasterWindow(void);

			wxRect GetWindowRect();
			void SetWindowHeight(const int &height);
			virtual int GetWindowHeight();
			void SetWindowWidth(const int &width);
			virtual int GetWindowWidth();

			virtual void UpdateScreenRatio() = 0;


			virtual int GetWidth() { return this->GetWindowWidth(); };
			virtual int GetHeight() { return this->GetWindowHeight(); };

			void StartThread();

			static void SetEndProgram();

			virtual bool GetProcessEnd();

			static void FillRect(wxDC * dc, const wxRect &rc, const wxColour &color);

			static void DrawTexte(wxDC * dc, const wxString &libelle, const int &xPos, const int &yPos, const CThemeFont &font);

			static wxSize GetSizeTexte(wxDC * dc, const wxString &libelle, const CThemeFont &font);

			virtual wxString GetWaitingMessage(){ return wxString();};
            
            virtual void CallRefresh(wxWindow * window);

			virtual void Resize() = 0;
            
		protected:

			static void ThreadIdle(void * data);
			virtual void ProcessOnIdleEndEvent(wxCommandEvent& event);
			virtual void ProcessOnSizeEvent(wxSizeEvent& event);
			virtual void ProcessIdle(){};
			
			virtual void PushThreadIdleEvent(){};


			CWindowMainPimpl * windowMainPimpl;
			wxString name;
			bool processEnd;
			bool processIdle;
		public:
        
            double scaleFactor;
			static bool endProgram;
			static vector<CMasterWindow *> listMainWindow;
		};
	}
}

