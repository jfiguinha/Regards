#pragma once
#include "ToolbarInterface.h"
#include <WindowMain.h>
#include <vector>

class CModificationManager;

namespace Regards
{
	namespace Window
	{
		class CToolbarWindow;

		class CTabWindowData
		{
		public:
			CTabWindowData() {};
			~CTabWindowData() {};

			void SetWindow(wxWindow * window)
			{
				this->window = window;
			}

			void SetWindow(CWindowMain * window)
			{
				this->windowMain = window;
			}

			void UpdateScreenRatio()
			{
				if (windowMain != nullptr)
					windowMain->UpdateScreenRatio();
			}

			bool IsShown()
			{
				if (window != nullptr)
					return window->IsShown();
				if (windowMain != nullptr)
					return windowMain->IsShown();

				return false;
			}
			
			void ShowWindow(const bool &show = true)
			{
				if (window != nullptr)
					window->Show(show);
				if (windowMain != nullptr)
					windowMain->Show(show);
			}

			wxWindow * GetWindow()
			{
				if (window != nullptr)
					return window;
				if (windowMain != nullptr)
					return windowMain;

				return nullptr;
			}

			int GetId()
			{
				return windowName;
			}

			void SetId(const int &id)
			{
				windowName = id;
			}

		private:
			wxWindow * window = nullptr;
            CWindowMain * windowMain= nullptr;
			int windowName;
		};

		class CTabWindow : public CWindowMain, public CToolbarInterface
		{
		public:

			CTabWindow(const wxString & windowName, wxWindow* parent, wxWindowID id);
			~CTabWindow();
			void UpdateScreenRatio();

        private:
            
			void OnSize(wxSizeEvent& event);
            void OnPaint(wxPaintEvent& event);       

		protected:

            void Resize();
            void ClickShowButton(const int& id);
            void HideAllWindow();
			virtual void LoadInfo() = 0;
			CModificationManager * modificationManager;
			vector<CTabWindowData*> listWindow;
			CToolbarWindow * toolbarWindow = nullptr;
			int windowVisible;
			int width;
			int height;
			CThemeBitmapWindow themeBitmap;
		};
	}
}