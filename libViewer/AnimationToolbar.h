#pragma once
#include "ToolbarWindow.h"
#include "SliderInterface.h"
#include <ToolbarInterface.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Viewer
	{
		class CAnimationToolbar : public CToolbarWindow
		{
		public:
			CAnimationToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, CToolbarInterface * toolbarInterface, const bool& vertical);
			~CAnimationToolbar();
			void AnimationStart();
			void AnimationStop();

		private:

			void NextPicture();
			void PreviousPicture();
			void EventManager(const int &id);
            
			CToolbarInterface * toolbarInterface;
			CToolbarButton * imagePlayDiaporama;
			CToolbarButton * imageStopDiaporama;
           // CToolbarButton * fullscreen;
		};
	}
}

