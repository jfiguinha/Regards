#pragma once
#include "WindowMain.h"
#include <FileGeolocation.h>
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
                       
            wxImage bitmapCheckOn;
			wxImage bitmapCheckOff;
            
            bool isSelected;
            
            
			wxString filename;
			CThemeBitmapInfos bitmapInfosTheme;
            bool checkboxVisibility;
            int xPos;
            int yPos;

		};
	}
}

