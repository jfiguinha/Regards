#pragma once
#include "WindowOpenGLMain.h"
#include <RegardsConfigParam.h>
#include <theme.h>
#include "SliderInterface.h"
#include "IBitmapWnd.h"
#include <BitmapDisplay.h>
using namespace std;

#define MOVEPICTURE 2
#define ZOOMPICTURE 3

using namespace Regards::Window;

namespace Regards::Window
{
	class CBitmapWndRender;

	class CBitmapWnd3D : public CWindowOpenGLMain, public IBitmapWnd
	{
	public:
		CBitmapWnd3D(wxWindow* parent, wxWindowID id);
		~CBitmapWnd3D(void) override;

		void SetBitmapRenderInterface(IBitmapRenderInterface* bitmapWndRender) override;

		void UpdateRenderInterface(IBitmapRenderInterface* bitmapWndRender) override;

		IBitmapRenderInterface* GetWndPt() override;

		void UpdateScreenRatio() override;

		void Resize() override;

		bool GetProcessEnd() override;

	private:
		//void OnLoading(wxTimerEvent& event);
		void OnMouseCaptureLost(wxMouseEvent& event)
		{
		};

		void OnMouseLeave(wxMouseEvent& event);
		void OnPaint(wxPaintEvent& event);
		void OnMouseMove(wxMouseEvent& event);
		void OnLButtonDown(wxMouseEvent& event);
		void OnRButtonDown(wxMouseEvent& event);
		void OnLButtonUp(wxMouseEvent& event);
		void OnLDoubleClick(wxMouseEvent& event);
		void OnMouseWheel(wxMouseEvent& event);
		void OnKeyDown(wxKeyEvent& event);
		void OnKeyUp(wxKeyEvent& event);
		void OnIdle(wxIdleEvent& evt);

		void OnEraseBackground(wxEraseEvent& event) override
		{
		};

		void OnTimer(wxTimerEvent& event);
		void OnCommand(wxCommandEvent& event);
		CRenderOpenGL* renderOpenGL = nullptr;
		IBitmapRenderInterface* bitmapWndRender = nullptr;
		//cv::ocl::Context& initializeContextFromGL();
		bool openclOpenGLInterop = false;

	};
}
