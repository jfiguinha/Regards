#pragma once
#include "ToolbarInterface.h"
#include <WindowMain.h>
#include <vector>

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
			CWindowMain* window;
			int windowName;
		};

		class CTabWindow : public CWindowMain, public CToolbarInterface
		{
		public:

			CTabWindow(wxWindow* parent, wxWindowID id, const CThemeBitmapWindow& themeBitmap);
			~CTabWindow();
			void ClickShowButton(const int& id);

		protected:

			virtual void LoadInfo() = 0;
			void Resize();
			void HideAllWindow();
			void OnSize(wxSizeEvent& event);
			void UpdateScreenRatio();
			void OnPaint(wxPaintEvent& event);
			vector<CTabWindowData*> listWindow;
			CToolbarWindow * toolbarWindow;
			int windowVisible;
			int width;
			int height;
			CThemeBitmapWindow themeBitmap;
		};
	}
}