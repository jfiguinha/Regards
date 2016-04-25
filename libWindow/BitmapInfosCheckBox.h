#pragma once
#include "WindowMain.h"
#include <FileGeolocation.h>
#include <mutex>
#include <thread>
using namespace std;
using namespace Regards::Internet;
namespace Regards
{
	namespace Window
	{
		class CBitmapInfosCheckBox : public CWindowMain
		{
		public:
			CBitmapInfosCheckBox(wxWindow* parent, wxWindowID id, const CThemeBitmapInfos & theme);
			~CBitmapInfosCheckBox();
			void SetFilename(const wxString &filename);
            void SetCheckboxVisible(const bool &checkboxVisibility);
			int GetHeight();
			void Redraw();
            void UpdateScreenRatio();
            bool GetCheckState();
            
		private:

            void Resize();
			void OnPaint(wxPaintEvent& event);
            void OnLButtonDown(wxMouseEvent& event);
            void DrawInformations(wxDC * dc);
            
            wxImage CreateFromSVG(const int & buttonWidth, const int & buttonHeight, const wxString &vector);
            
            wxImage bitmapCheckOn;
            wxImage bitmapCheckOff;
            
            wxString checkOnVector;
            wxString checkOffVector;
            
            bool isSelected;
            bool isVector = true;
            
			wxString filename;
			CThemeBitmapInfos bitmapInfosTheme;
            bool checkboxVisibility = true;
            int xPos = 0;
            int yPos = 0;

		};
	}
}

