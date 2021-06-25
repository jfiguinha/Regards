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
			CTabWindowData() : windowName(0)
			{
			};

			~CTabWindowData()
			{
			};

			void SetWindow(wxWindow* window)
			{
				this->window = window;
			}

			void SetWindow(CWindowMain* window)
			{
				this->windowMain = window;
			}

			void UpdateScreenRatio() const
			{
				if (windowMain != nullptr)
					windowMain->UpdateScreenRatio();
			}

			bool IsShown() const
			{
				if (window != nullptr)
					return window->IsShown();
				if (windowMain != nullptr)
					return windowMain->IsShown();

				return false;
			}

			void ShowWindow(const bool& show = true) const
			{
				if (window != nullptr)
					window->Show(show);
				if (windowMain != nullptr)
					windowMain->Show(show);
			}

			wxWindow* GetWindow() const
			{
				if (window != nullptr)
					return window;
				if (windowMain != nullptr)
					return windowMain;

				return nullptr;
			}

			int GetId() const
			{
				return windowName;
			}

			void SetId(const int& id)
			{
				windowName = id;
			}

		private:
			wxWindow* window = nullptr;
			CWindowMain* windowMain = nullptr;
			int windowName;
		};

		class CTabWindow : public CWindowMain, public CToolbarInterface
		{
		public:
			CTabWindow(const wxString& windowName, wxWindow* parent, wxWindowID id);
			~CTabWindow() override;
			void UpdateScreenRatio() override;

		private:
			void OnSize(wxSizeEvent& event) override;
			void on_paint(wxPaintEvent& event);

		protected:
			void Resize() override;
			void ClickShowButton(const int& id, const int& refresh = 1) override;
			void HideAllWindow();
			virtual void LoadInfo() = 0;
			CModificationManager* modificationManager;
			vector<CTabWindowData*> listWindow;
			CToolbarWindow* toolbarWindow = nullptr;
			int windowVisible;
			int width;
			int height;
			CThemeBitmapWindow themeBitmap;
		};
	}
}
