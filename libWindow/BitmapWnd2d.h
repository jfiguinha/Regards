#pragma once
#include "WindowMain.h"
#include <RegardsConfigParam.h>
#include <theme.h>
#include "IBitmapWnd.h"
#include "SliderInterface.h"
#include <BitmapDisplay.h>
using namespace std;
class CRegardsBitmap;
class CFiltreEffet;
class CImageLoadingFormat;
class CRegardsFloatBitmap;
class IMouseUpdate;
class CEffectParameter;

#define MOVEPICTURE 2
#define ZOOMPICTURE 3

using namespace Regards::Window;

namespace Regards
{
	namespace Window
	{
		class CBitmapWndRender;

		class CBitmapWnd2D : public CWindowMain, public IBitmapWnd
		{
		public:
			CBitmapWnd2D(wxWindow* parent, wxWindowID id);
			~CBitmapWnd2D(void) override;

			virtual void SetBitmapRenderInterface(IBitmapRenderInterface* bitmapWndRender) override;

			virtual void UpdateRenderInterface(IBitmapRenderInterface* bitmapWndRender) override;

			virtual IBitmapRenderInterface * GetWndPt() override;

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
			void OnMouseLeave(wxMouseEvent& event);
			void OnTimer(wxTimerEvent& event);
			void OnCommand(wxCommandEvent& event);

			IBitmapRenderInterface* bitmapWndRender = nullptr;

		};
	}
}
