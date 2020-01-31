#pragma once
#include "WindowMain.h"
#include <FileGeolocation.h>
using namespace Regards::Internet;
namespace Regards
{
	namespace Window
	{

        
		class CBitmapInfos : public CWindowMain
		{
		public:
			CBitmapInfos(wxWindow* parent, wxWindowID id, const CThemeBitmapInfos & theme, CFileGeolocation * fileGeolocalisation);
			~CBitmapInfos();
            void UpdateData();
			void SetFilename(const wxString &filename);
			int GetHeight();
			void Redraw();
            void UpdateScreenRatio();
            
		private:
            void OnTimerGPSUpdate(wxTimerEvent& event);
			void SetDateInfos(const wxString &dataInfos, char seperator);
			void OnPaint(wxPaintEvent& event);
			int Dayofweek(int d, int m, int y);
            void DrawInformations(wxDC * dc);
            wxTimer * gpsTimer;
			wxString filename;
			wxString gpsInfos;
			wxString dateInfos;
			bool gpsInfosUpdate;
			CThemeBitmapInfos bitmapInfosTheme;
			CFileGeolocation * fileGeolocalisation;
			mutex mufileGeoloc;
            wxString urlServer = L"";
		};
	}
}

