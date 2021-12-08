#pragma once
#include "WindowOpenGLMain.h"
#include <RegardsConfigParam.h>
#include <theme.h>
#include "SliderInterface.h"
#include "IBitmapWnd.h"
#include <BitmapDisplay.h>
using namespace std;
using namespace Regards::OpenCL;


#define MOVEPICTURE 2
#define ZOOMPICTURE 3

using namespace Regards::Window;

namespace Regards
{
	namespace Window
	{
		class CBitmapWndRender;

		class CBitmapWnd3D : public CWindowOpenGLMain, public IBitmapWnd
		{
		public:
			CBitmapWnd3D(wxWindow* parent, wxWindowID id);
			~CBitmapWnd3D(void) override;

			virtual void SetBitmapRenderInterface(IBitmapRenderInterface* bitmapWndRender);

			virtual void UpdateRenderInterface(IBitmapRenderInterface* bitmapWndRender);

			virtual IBitmapRenderInterface * GetWndPt();

			virtual void UpdateScreenRatio() override;

			virtual void Resize() override;

			virtual bool GetProcessEnd() override;

		private:


			//void OnLoading(wxTimerEvent& event);
			void OnMouseCaptureLost(wxMouseEvent& event)
			{
			};
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
			COpenCLContext* openclContext = nullptr;
		};
	}
}
