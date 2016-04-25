#pragma once
#include "BitmapWnd.h"
#include <RenderBitmapInterface.h>
#include <BitmapInterface.h>
#include <StatusBarInterface.h>
#include <Draw.h>

class CEffectParameter;
using namespace Regards::FiltreEffet;
using namespace Regards::Window;

namespace Regards
{
	namespace Control
	{
		class CBitmapWndViewer : public CBitmapWnd
		{
		public:
			CBitmapWndViewer(wxWindow* parent, wxWindowID id, CSliderInterface * slider, CWindowMain * main, const CThemeBitmapWindow & theme, CBitmapInterface * bitmapInterface, IStatusBarInterface * statusBarInterface);
			~CBitmapWndViewer();

			void SendEmail();
			void SavePicture();
			void PrintPicture();
			void SetTransitionBitmap(CRegardsBitmap * bmpSecond);
			void FixArrowNavigation(const bool &fix);
			void StopTransition();
			void SetBitmapPreviewEffect(const int &effect);
			void SetBitmapPreviewEffect(const int &effect, CEffectParameter * effectParameter);
			bool SetBitmapEffect(const int &effect, CEffectParameter * effectParameter = nullptr);
            bool NeedAfterDrawBitmap();
			void SetDiaporamaMode();
			void SetNormalMode();

		private:

            void ApplyEffectWithParameter(const int &effect, CEffectParameter * effectParameter);
			void SetDessinRatio();
			void DeterminePos(wxRect &rc, const int &nTailleAffichageWidth, const int &nTailleAffichageHeight, int &left, int &top);
			void AfterDrawBitmap(wxDC * deviceContext);
			void Resize(const int &width, const int &height);
			
#ifdef KeyPress
#undef KeyPress
#endif
			
			void KeyPress(const int &key);
			void MouseMove(const int &xPos, const int &yPos);
			void MouseClick(const int &xPos, const int &yPos);
			void MouseRelease(const int &xPos, const int &yPos);
			void AfterRenderBitmap(CRenderBitmapInterface * renderInterface);
			void AfterSetBitmap();
			void ShowArrowNext(CRenderBitmapInterface * renderInterface);
			void ShowArrowPrevious(CRenderBitmapInterface * renderInterface);
			void EndTransition();
			void OnTransition(wxTimerEvent& event);
			void onIdle(wxIdleEvent& evt);
            void ShowLoadingBitmap(const int &stepLoading);
            void LoadingResource();
            
			wxCursor hCursorCross;
			CDraw * m_cDessin;
			CRegardsBitmap * nextPicture;
			CRegardsBitmap * arrowPrevious;
			CRegardsBitmap * arrowNext;
            vector<CRegardsBitmap *> vecLoadingBitmap;
			bool startTransition;
			int step;
			bool fixArrow;
			CBitmapInterface * bitmapInterface;

			//Preview Parameter
			int preview;	
			CEffectParameter * effectParameter;
			IStatusBarInterface * statusBarInterface = nullptr;
			wxPoint oldMouse;

			//Thread Parameter
			bool m_bTransition = false;
			void OnThreadStep();
			bool isDiaporama = false;
			bool isInUse = false;
			bool invertColor = false;
			wxTimer * transitionTimer = nullptr;
			wxTimer * selectEffectTimer = nullptr;
		};
	}
}

