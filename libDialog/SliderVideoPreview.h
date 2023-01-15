#pragma once
#include <theme.h>
#include <SliderInterface.h>
#include "WindowMain.h"
#include <wx/activityindicator.h>
using namespace Regards::Window;

#define PLAYBUTTONID 1
#define PAUSEBUTTONID 2
#define SPEAKERBUTTONID 3
#define ZOOMBUTTONID 4
#define SCREENFORMATID 5

class CRegardsBitmap;

namespace Regards::Control
{
	class CSliderVideoPreview : public CWindowMain
	{
	public:
		CSliderVideoPreview(wxWindow* parent, wxWindowID id, CSliderInterface* sliderEvent,
		                    const CThemeSlider& themeSlider);
		~CSliderVideoPreview() override;

		int GetWidth() override;
		int GetHeight() override;

		void SetPlay();
		void SetPause();

		void Start();
		void Stop();

		float GetTimePos();

		void SetTotalSecondTime(const int64_t& millisecondTime);
		void SetPastSecondTime(const int64_t& millisecondTime);
		void UpdatePositionEvent();

		bool IsMouseOver();

		void UpdateScreenRatio() override;

	protected:
		//Slider Event
		CSliderInterface* sliderEvent;

	private:
		void InsertPreviousButton(const int& xStart, wxDC* context);
		void InsertNextButton(const int& xStart, wxDC* context);

		void OnMouseMove(wxMouseEvent& event);
		void on_paint(wxPaintEvent& event);
		void OnLButtonDown(wxMouseEvent& event);
		void OnLButtonUp(wxMouseEvent& event);

		void OnMouseCaptureLost(wxMouseEvent& event)
		{
		};
		void OnMouseLeave(wxMouseEvent& event);
		void OnMouseHover(wxMouseEvent& event);

		void ClickLeftPage(const int& x);
		void ClickRightPage(const int& x);

		void DrawShapeElement(wxDC* context, const wxRect& rc);
		int DrawTimePast(wxDC* context, const wxString& libelle);

		void CalculPositionButton();
		void CalculTimePosition(const int& x);
		void CalculPositionButton(const int& x);
		void Draw(wxDC* context);

		int DrawVolumeLibelle(wxDC* context, const wxString& libelle);
		int DrawTotalTimeLibelle(wxDC* context, const wxString& libelle, const int& volumePos);
		void OnMouseClickTimer(wxTimerEvent& event);

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
		wxTimer* mouseTimer;
		bool isNextButton = false;
		CThemeSliderVideo themeSlider;
		wxActivityIndicator* m_animation;

		wxImage buttonImageNext;
		wxImage buttonImagePrevious;

		bool buttonNextActif = false;
		wxRect positionNextButton;

		bool buttonPreviousActif = false;
		wxRect positionPreviousButton;
	};
}
