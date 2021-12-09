#pragma once
#include <WindowMain.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Viewer
	{
		class CThumbnailToolBarZoom : public CWindowMain
		{
		public:
			CThumbnailToolBarZoom(wxWindow* parent, wxWindowID id, const CThemeToolBarZoom& theme);
			~CThumbnailToolBarZoom() override;
			int GetHeight() override;
			void UpdateScreenRatio() override;

		private:
			void on_paint(wxPaintEvent& event);
			void OnLButtonDown(wxMouseEvent& event);
			//void OnMouseMove(wxMouseEvent& event);
            void OnIdle(wxIdleEvent& evt);
			void Draw(wxDC* dc);
			void DrawPreviousElement(wxDC* dc, const wxString& libelle, const CThemeFont& themeFont);
			void DrawNextElement(wxDC* dc, const wxString& libelle, const CThemeFont& themeFont);
			void DrawElement(wxDC* dc, const wxString& libelle, const CThemeFont& themeFont);

			wxRect positionPrevious;
			wxRect positionNext;

			int typeAffichage;
			CThemeToolBarZoom themeToolbarZoom;
            bool needToRefresh = false;
			wxString criteriaPrevious;
			wxString criteriaNext;
			wxString allPhoto;
			wxString byYear;
			wxString byMonth;
			wxString byDay;
			wxString byLocalisation;
		};
	}
}
