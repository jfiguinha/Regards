#pragma once
#include <theme.h>
#include <EffectVideoParameter.h>
#include <SliderInterface.h>
#include "WindowMain.h"
using namespace Regards::Window;

#define PLAYBUTTONID 1
#define PAUSEBUTTONID 2
#define SPEAKERBUTTONID 3
#define ZOOMBUTTONID 4
#define SCREENFORMATID 5

class CRegardsBitmap;

namespace Regards
{
	namespace Control
	{
		class CSliderVideoPreview : public CWindowMain
		{
		public:
			CSliderVideoPreview(wxWindow* parent, wxWindowID id, CSliderInterface * sliderEvent, const CThemeSlider & themeSlider);
			~CSliderVideoPreview();

			int GetWidth();
			int GetHeight();

			void SetPlay();
			void SetPause();

			float GetTimePos();

			void SetTotalSecondTime(const int64_t &millisecondTime);
			void SetPastSecondTime(const int64_t &millisecondTime);
			void UpdatePositionEvent();

			bool IsMouseOver();

            void UpdateScreenRatio();


		protected:

			//Slider Event
			CSliderInterface * sliderEvent;


		private:
            
			void OnMouseMove(wxMouseEvent& event);
			void OnPaint(wxPaintEvent& event);
			void OnLButtonDown(wxMouseEvent& event);
			void OnLButtonUp(wxMouseEvent& event);
			void OnMouseCaptureLost(wxMouseEvent& event){};
			void OnMouseLeave(wxMouseEvent& event);
			void OnMouseHover(wxMouseEvent& event);

			void ClickLeftPage(const int &x);
			void ClickRightPage(const int &x);

			void DrawShapeElement(wxDC * context, const wxRect &rc);
			int DrawTimePast(wxDC * context, const wxString &libelle);

			void CalculPositionButton();
			void CalculTimePosition(const int &x);
			void CalculPositionButton(const int &x);
			void Draw(wxDC * context);

			int DrawVolumeLibelle(wxDC * context, const wxString &libelle);
			int DrawTotalTimeLibelle(wxDC * context, const wxString &libelle, const int &volumePos);


			wxImage button;
            
            wxColor colorToReplace;
            wxColor colorActifReplacement;
            wxColor colorInactifReplacement;

			bool isPlay;
			bool m_bMouseOver;
			bool m_bTracking;

			wxRect positionButton;
			wxRect positionTextePast;
			wxRect positionTexteTotal;
			wxRect positionSlider;
			
			wxCursor hCursorHand;
			wxString timePast;
			wxString totalTime;
			float secondTimePast;
			float secondTotalTime;
			bool mouseBlock;
			int positionXSlider;
			int positionYSlider;
			
			CThemeSliderVideo themeSlider;
		};

	}
}