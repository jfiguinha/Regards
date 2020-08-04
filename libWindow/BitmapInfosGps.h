#pragma once
#include "WindowMain.h"
//#include <FileGeolocation.h>
//using namespace Regards::Internet;
namespace Regards
{
	namespace Window
	{
		class CBitmapInfosGps : public CWindowMain
		{
		public:
			CBitmapInfosGps(wxWindow* parent, wxWindowID id, const CThemeBitmapInfos & theme);
			~CBitmapInfosGps();
			void SetInfos(const wxString &localisation, const wxString &latitude, const wxString &longitude);
			int GetHeight();
			void Redraw();
            void UpdateScreenRatio();

            
		private:

			void OnPaint(wxPaintEvent& event);
            void DrawInformations(wxDC * dc);
            
            wxString localisation;
            wxString latitude;
            wxString longitude;
			CThemeBitmapInfos bitmapInfosTheme;


		};
	}
}

