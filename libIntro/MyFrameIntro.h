#pragma once
#include <MainInterface.h>
#include "MyCentralWindowIntro.h"

namespace Regards
{
	namespace Introduction
	{
		class MyFrameIntro : public wxFrame
		{
		public:
			MyFrameIntro(const wxString& title, const wxPoint& pos, const wxSize& size, IMainInterface * mainInterface);
			~MyFrameIntro();
			
		private:

			void OnClose(wxCloseEvent& event);
			void OnSize(wxSizeEvent& event);
			void OnTimeShowViewer(wxTimerEvent& event);

			CTitleIntro * titleIntro;
			CMyCentralWindowIntro * myCentralWindow;
			IMainInterface * mainInterface;
			wxTimer * diaporamaTimer;
		};
	}
}
