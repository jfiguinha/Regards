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
			void OnClose(wxCloseEvent& event);
		private:

			CMyCentralWindowIntro * myCentralWindow;
			IMainInterface * mainInterface;
		};
	}
}
