#pragma once
#include <theme.h>
#include "ToolbarWindow.h"
//#include "SliderInterface.h"
//#include <ToolbarInterface.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Window
	{
		class CToolbarInterface;
	}

	namespace Viewer
	{
		class CPreviewToolbar final : public CToolbarWindow
		{
		public:
			CPreviewToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme,
			                CToolbarInterface* toolbarInterface, const bool& vertical);
			~CPreviewToolbar() = default;

			void SetFullscreen();
			void SetScreen();
			void DisableNavigationButton();
			void EnableNavigationButton();
			void EnableSaveButton();
			void DisableSaveButton();
			void EnableExportButton();
			void DisableExportButton();
			void EnableScreenButton();
			void DisableScreenButton();

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
			void Save();
			void EventManager(const int& id) override;

			CToolbarInterface* toolbarInterface;
			std::unique_ptr<CToolbarButton> imagePlayDiaporama;
			std::unique_ptr<CToolbarButton> imageStopDiaporama;
			std::unique_ptr<CToolbarButton> fullscreen;
			std::unique_ptr<CToolbarButton> imageEnd;
			std::unique_ptr<CToolbarButton> imageSuiv;
			std::unique_ptr<CToolbarButton> imageFirst;
			std::unique_ptr<CToolbarButton> imagePrec;
			std::unique_ptr<CToolbarButton> save;
            std::unique_ptr<CToolbarButton> exportFile;
            std::unique_ptr<CToolbarButton> editFile;
			std::unique_ptr<CToolbarButton> editor;
			std::unique_ptr<CToolbarButton> rotate90;
			std::unique_ptr<CToolbarButton> rotate270;
			std::unique_ptr<CToolbarButton> flipVertical;
			std::unique_ptr<CToolbarButton> flipHorizontal;
			bool navigationButtonEnable = true;
		};
	}
}
