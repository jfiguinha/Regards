#pragma once
#include <MainInterface.h>
#include "MyCentralWindowIntro.h"

namespace Regards::Introduction
{
	class CTitleIntro;

	class MyFrameIntro : public wxFrame
	{
	public:
		MyFrameIntro(const wxString& title, const wxString& appTitle, const wxPoint& pos, const wxSize& size,
			ISCannerInterface * mainInterface);
		~MyFrameIntro() override;

	private:
		void OnClose(wxCloseEvent& event);
		void on_size(wxSizeEvent& event);
		void OnTimeShowViewer(wxTimerEvent& event);
		void NewModelsAvailable();

		CTitleIntro* titleIntro;
		CMyCentralWindowIntro* myCentralWindow;
		ISCannerInterface* mainInterface;
		wxTimer* diaporamaTimer;
	};
}
