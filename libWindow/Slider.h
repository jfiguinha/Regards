#pragma once
#include "WindowMain.h"
#include "SliderInterface.h"

namespace Regards
{
	namespace Window
	{
		class CSlider : public CWindowMain
		{
		public:
			CSlider(wxWindow* parent, wxWindowID id, CSliderInterface * sliderEvent, const CThemeSlider & themeSlider);
			virtual ~CSlider();

			int GetWidth();
			int GetHeight();

			void SetTotalSecondTime(const int64_t &millisecondTime);
			void SetPastSecondTime(const int64_t &millisecondTime);
			void UpdatePositionEvent();

		protected:

			//Slider Event
			CSliderInterface * sliderEvent;
			void SetPastTime(const int64_t &secondTime);
			void Resize();

		private:

			void PaintNow();
			void OnMouseMove(wxMouseEvent& event);
			void OnPaint(wxPaintEvent& event);
			void OnLButtonDown(wxMouseEvent& event);
			void OnLButtonUp(wxMouseEvent& event);
			void OnMouseCaptureLost(wxMouseEvent& event){};

			void ClickLeftPage(const int &x);
			void ClickRightPage(const int &x);
			void DrawShapeElement(wxDC * deviceContext, const wxRect &rc);
			void CalculPositionButton();
			void CalculTimePosition(const int &x);
			void CalculPositionButton(const int &x);
			void Draw(wxDC * context);

			int DrawTotalTime(wxDC * context, const wxString &libelle);
			int DrawTimePast(wxDC * context, const wxString &libelle);
	
			wxImage button;
			wxRect positionButton;
			wxRect positionTextePast;
			wxRect positionTexteTotal;
			wxRect positionSlider;
			
			wxCursor hCursorHand;
			wxString timePast;
			wxString totalTime;
			float secondTimePast;
			float secondTotalTime;
			int64_t totalTimeInMilliseconds;
			int64_t totalPastTimeInMilliseconds;
			bool mouseBlock;
			int positionXSlider;
			int positionYSlider;
            
			CThemeSlider themeSlider;
		};

	}
}
