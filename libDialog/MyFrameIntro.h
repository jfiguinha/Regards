#pragma once
#include <MainInterface.h>
#include "MyCentralWindowIntro.h"

namespace Regards
{
	namespace Introduction
	{
		class CTitleIntro;
		
		class MyFrameIntro : public wxFrame
		{
		public:
			MyFrameIntro(const wxString& title, const wxString& appTitle, const wxPoint& pos, const wxSize& size,
			             IMainInterface* mainInterface);
			~MyFrameIntro() override;

		private:
			void OnClose(wxCloseEvent& event);
			void on_size(wxSizeEvent& event);
			void OnTimeShowViewer(wxTimerEvent& event);

			CTitleIntro* titleIntro;
			CMyCentralWindowIntro* myCentralWindow;
			IMainInterface* mainInterface;
			wxTimer* diaporamaTimer;
		};
	}
}
