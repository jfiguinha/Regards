#pragma once
#include <theme.h>
#include <EffectVideoParameter.h>
#include <SliderInterface.h>
#include "WindowMain.h"
using namespace Regards::Window;

class CRegardsBitmap;

class CSliderVideoSelection : public CWindowMain
{
public:
	CSliderVideoSelection(wxWindow* parent, wxWindowID id, wxWindow * eventWnd, const CThemeSlider & themeSlider);
	~CSliderVideoSelection();

	int GetWidth();
	int GetHeight();

	void SetPlay();
	void SetPause();

	long GetTimeStart();
	long GetTimeEnd();

	void SetStartTime(const long &pos);
	void SetEndTime(const long &pos);

	void SetTotalSecondTime(const int64_t &millisecondTime);
	void SetPastSecondTime(const int64_t &millisecondTime);

	bool IsMouseOver();

	void UpdateScreenRatio();

private:
	
	void OnMouseMove(wxMouseEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnLButtonDown(wxMouseEvent& event);
	void OnLButtonUp(wxMouseEvent& event);
	void OnMouseCaptureLost(wxMouseEvent& event){};
	void OnMouseLeave(wxMouseEvent& event);
	void OnMouseHover(wxMouseEvent& event);

	void DrawShapeElement(wxDC * context, const wxRect &rc);
	int DrawTimePast(wxDC * context, const wxString &libelle);

	void CalculPositionButtonTo();
	void CalculPositionButton();
	void CalculTimePosition(const int &x);
	void CalculTimePositionTo(const int &x);
	void CalculPositionButton(const int &x);
	void CalculPositionButtonTo(const int &x);
	void Draw(wxDC * context);

	int DrawTotalTimeLibelle(wxDC * context, const wxString &libelle);

	wxImage button;
	wxImage buttonTo;
	
	wxColor colorToReplace;
	wxColor colorActifReplacement;
	wxColor colorInactifReplacement;

	bool isPlay;
	bool m_bMouseOver;
	bool m_bTracking;

	wxRect positionButton;
	wxRect positionButtonTo;
	wxRect positionTextePast;
	wxRect positionTexteTotal;
	wxRect positionSlider;
	wxRect positionSliderTo;
	
	wxCursor hCursorHand;
	wxString timePast;
	wxString totalTime;
	float secondTimePast;
	float secondTimeTo;
	float secondTotalTime;
	bool mouseBlock;
	bool mouseBlockTo;
	int positionXSlider;
	int positionYSlider;
	
	int positionXSliderTo;
	int positionYSliderTo;

	CThemeSliderVideo themeSlider;
	wxWindow * eventWnd;
};
