#pragma once
#include "ToolbarWindow.h"
#include "SliderInterface.h"
#include <ToolbarInterface.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Viewer
	{
		class CPreviewToolbar : public CToolbarWindow
		{
		public:
			CPreviewToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, CToolbarInterface * toolbarInterface, const bool& vertical);
			~CPreviewToolbar();

            void SetFullscreen();
            void SetScreen();
			void DisableNavigationButton();
			void EnableNavigationButton();

		private:

			void DiaporamaStart();
			void DiaporamaStop();
			void Rotate90();
			void Rotate270();
			void FlipVertical();
			void FlipHorizontal();
			void Fullscreen();
			void NextPicture();
			void PreviousPicture();
			void FirstPicture();
			void LastPicture();
			void EventManager(const int &id);
            
			CToolbarInterface * toolbarInterface;
			CToolbarButton * imagePlayDiaporama;
			CToolbarButton * imageStopDiaporama;
            CToolbarButton * fullscreen;
			CToolbarButton * imageEnd;
			CToolbarButton * imageSuiv;
			CToolbarButton * imageFirst;
			CToolbarButton * imagePrec;
		};
	}
}

