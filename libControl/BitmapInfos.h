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

        
		class CBitmapInfos : public CWindowMain
		{
		public:
			CBitmapInfos(wxWindow* parent, wxWindowID id, const CThemeBitmapInfos & theme, CFileGeolocation * fileGeolocalisation);
			~CBitmapInfos();

			void SetFilename(const wxString &filename);
			int GetHeight();
			void Redraw();
            void UpdateScreenRatio();

            
		private:
			void SetDateInfos(const wxString &dataInfos, char seperator);
			void Resize();
			void OnPaint(wxPaintEvent& event);
			void OnIdle(wxIdleEvent& evt);
			int Dayofweek(int d, int m, int y);

            
			wxString filename;
			wxString gpsInfos;
			wxString dateInfos;
			bool gpsInfosUpdate = false;
			CThemeBitmapInfos bitmapInfosTheme;
			CFileGeolocation * fileGeolocalisation;

		};
	}
}

