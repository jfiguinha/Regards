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

			vector<CTabWindowData*> listWindow;
			CToolbarWindow * toolbarWindow = nullptr;
			int windowVisible;
			int width;
			int height;
			CThemeBitmapWindow themeBitmap;
		};
	}
}