#pragma once
#include <theme.h>
class CWindowMainPimpl;

namespace Regards::Window
{


	class CMasterWindow
	{
	public:
		CMasterWindow(void);
		virtual ~CMasterWindow(void);

		wxRect GetWindowRect();
		void SetWindowHeight(const int& height);
		virtual int GetWindowHeight();
		void SetWindowWidth(const int& width);
		virtual int GetWindowWidth();
		bool GetEndProgram();
		virtual void IdleFunction() = 0;

		virtual void UpdateScreenRatio() = 0;

		static void StopAllProcess(const wxString& title, const wxString& message, wxWindow* parentWindow,
		                           const int& nbTry = 10);

		virtual int GetWidth() { return this->GetWindowWidth(); };
		virtual int GetHeight() { return this->GetWindowHeight(); };

		static void SetEndProgram();
		static void SetStopProcess(const bool& state);

		void SetStartProcess();

		virtual bool GetProcessEnd();

		static void FillRect(wxDC* dc, const wxRect& rc, const wxColour& color);

		static void DrawTexte(wxDC* dc, const wxString& libelle, const int& xPos, const int& yPos,
		                      const CThemeFont& font);

		static wxSize GetSizeTexte(wxDC* dc, const wxString& libelle, const CThemeFont& font);

		virtual wxString GetWaitingMessage() { return wxString(); };

		virtual void Resize() = 0;

		virtual bool GetProcessStop();

		bool processEnd;

	protected:

		virtual void ProcessOnSizeEvent(wxWindow* window, wxSizeEvent& event);

		virtual void ProcessIdle()
		{
		};

		virtual void PushThreadIdleEvent()
		{
		};


		wxString name;
		
		std::atomic_bool processStop;
		std::atomic_bool processIdle;
		int id;
		int width;
		int height;
	public:
		double scaleFactor;
		static std::atomic_bool endProgram;
		static std::atomic_bool stopProcess;
		static tbb::concurrent_vector<CMasterWindow*> listMainWindow;
		static tbb::concurrent_vector<CMasterWindow*> listProcessWindow;
	};
}
