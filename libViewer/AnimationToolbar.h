#pragma once
#include "ToolbarWindow.h"
//#include "SliderInterface.h"
//#include <ToolbarInterface.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Window
	{
		class CToolbarInterface;
		class CToolbarButton;
	}

	namespace Viewer
	{
		
		class CAnimationToolbar : public Regards::Window::CToolbarWindow
		{
		public:
			CAnimationToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, Regards::Window::CToolbarInterface * toolbarInterface, const bool& vertical);
			~CAnimationToolbar();
			void AnimationStart();
			void AnimationStop();

		private:

			void NextPicture();
			void PreviousPicture();
			void EventManager(const int &id);
            
			Regards::Window::CToolbarInterface * toolbarInterface;
			Regards::Window::CToolbarButton * imagePlayDiaporama;
			Regards::Window::CToolbarButton * imageStopDiaporama;
           // CToolbarButton * fullscreen;
		};
	}
}

