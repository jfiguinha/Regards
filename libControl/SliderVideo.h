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
	namespace Video
	{
		class CSliderVideo : public CWindowMain
		{
		public:
			CSliderVideo(wxWindow* parent, wxWindowID id, CSliderInterface * sliderEvent, const CThemeSlider & themeSlider);
			~CSliderVideo();

			int GetWidth();
			int GetHeight();

			void SetPlay();
			void SetPause();

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
			void InsertPlayButton(wxDC * context);
			void InsertSpeakerButton(const int &xStart, wxDC * context);
			void InsertZoomButton(const int &xStart, wxDC * context);
			void InsertScreenFormatButton(const int &xStart, wxDC * context);

			wxImage buttonPlay;
			wxImage buttonPause;
			wxImage buttonVolumeUp;
			wxImage buttonVolumeDown;
			wxImage buttonSpeaker;
			wxImage button;
            
			/*
            wxString buttonPlayVector;
            wxString buttonPauseVector;
            wxString buttonVolumeUpVector;
            wxString buttonVolumeDownVector;
            wxString buttonSpeakerVector;
            wxString buttonVector;
            */

            wxColor colorToReplace;
            wxColor colorActifReplacement;
            wxColor colorInactifReplacement;

			bool buttonPlayActif;
			bool buttonPauseActif;
			bool buttonScreenActif;
			bool buttonZoomActif;
			bool buttonSpeakerActif;
			bool isPlay;
			bool m_bMouseOver;
			bool m_bTracking;

			wxRect positionButton;
			wxRect positionPlayOrPauseButton;
			wxRect positionZoomButton;
			wxRect positionSpeakerButton;
			wxRect positionScreenFormatButton;
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
