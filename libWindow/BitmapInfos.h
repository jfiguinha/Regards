#pragma once
#include "WindowMain.h"
//#include <FileGeolocation.h>
//using namespace Regards::Internet;
namespace Regards
{
	namespace Internet
	{
		class CFileGeolocation;
	}

	namespace Window
	{
		class CBitmapInfos : public CWindowMain
		{
		public:
			CBitmapInfos(wxWindow* parent, wxWindowID id, const CThemeBitmapInfos& theme);
			~CBitmapInfos() override;
			void UpdateData();
			void SetFilename(const wxString& filename);
			int GetHeight() override;
			void Redraw();
			void UpdateScreenRatio() override;

		private:

			wxString GenerateDefaultTimeStamp();
			void OnUpdateGpsInfos(wxCommandEvent& event);
			void SetDateInfos(const wxString& dataInfos, char seperator);
			void on_paint(wxPaintEvent& event);
			int Dayofweek(int d, int m, int y);
			void DrawInformations(wxDC* dc);
			wxString filename;
			wxString gpsInfos;
			wxString dateInfos;
			bool gpsInfosUpdate;
			CThemeBitmapInfos bitmapInfosTheme;
		};
	}
}
