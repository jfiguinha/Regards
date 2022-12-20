#pragma once
#include "WindowMain.h"
//#include <FileGeolocation.h>
//using namespace Regards::Internet;
namespace Regards
{
	namespace Window
	{
		class CBitmapInfosCheckBox : public CWindowMain
		{
		public:
			CBitmapInfosCheckBox(wxWindow* parent, wxWindowID id, const CThemeBitmapInfos& theme);
			~CBitmapInfosCheckBox() override;
			void SetFilename(const wxString& filename);
			void SetCheckboxVisible(const bool& checkboxVisibility);
			int GetHeight() override;
			void Redraw();
			void UpdateScreenRatio() override;
			bool GetCheckState();

		private:
			void on_paint(wxPaintEvent& event);
			void OnLButtonDown(wxMouseEvent& event);
			void DrawInformations(wxDC* dc);

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
