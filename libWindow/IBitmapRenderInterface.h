#pragma once
#include <GLcontext.h>

class IBitmapRenderInterface
{
public:
	virtual void OnPaint3D(wxGLCanvas* canvas) = 0;
	virtual void OnPaint2D(wxWindow * gdi) = 0;
	virtual void OnMouseMove(wxMouseEvent& event) = 0;
	virtual void OnLButtonDown(wxMouseEvent& event) = 0;
	virtual void OnRButtonDown(wxMouseEvent& event) = 0;
	virtual void OnLButtonUp(wxMouseEvent& event) = 0;
	virtual void OnLDoubleClick(wxMouseEvent& event) = 0;
	virtual void OnMouseWheel(wxMouseEvent& event) = 0;
	virtual void OnKeyDown(wxKeyEvent& event) = 0;
	virtual void OnKeyUp(wxKeyEvent& event) = 0;
	virtual void OnIdle(wxIdleEvent& evt) = 0;
	virtual void OnTimer(wxTimerEvent& event) = 0;
	virtual void OnCommand(wxCommandEvent& event) = 0;
	virtual vector<int> GetListTimer() = 0;
	virtual vector<int> GetListCommand() = 0;
	virtual int UpdateResized() = 0;
	virtual void UpdateScreenRatio() = 0;
	virtual bool GetProcessEnd() = 0;
	virtual void SetParent(wxWindow* parent) = 0;
	virtual void Resize() = 0;
	virtual void SetEndProgram(const bool& endProgram) = 0;
};
