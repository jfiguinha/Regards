#pragma once
#include "BitmapWnd.h"
#include <RenderPreviewBitmap.h>
#include <BitmapInterface.h>
#include <Draw.h>
#include <FiltreUpdate.h>
#include <OpenCLContext.h>
#include <OpenCLEffectVideo.h>
#include <AfterEffect.h>

using namespace Regards::OpenCL;
class CDecodeRawPicture;
class CEffectParameter;
class IMouseUpdate;
class CImageLoadingFormat;
using namespace Regards::FiltreEffet;
using namespace Regards::Window;
using namespace Regards::OpenCL;

namespace Regards
{
	namespace Control
	{
		class CBitmapWndViewer : public CBitmapWnd, public IFiltreUpdate
		{
		public:
			CBitmapWndViewer(wxWindow* parent, wxWindowID id, CSliderInterface * slider, wxWindowID mainViewerId, const CThemeBitmapWindow & theme, CBitmapInterface * bitmapInterface);
			~CBitmapWndViewer();
			CEffectParameter * GetParameter();
			void SetListener(IMouseUpdate * mouseUpdate);
			void RemoveListener();
			void SendEmail();
			void SavePicture();
			void PrintPicture();
			void SetTransitionBitmap(CImageLoadingFormat * bmpSecond);
			void FixArrowNavigation(const bool &fix);
			void StopTransition();
			void SetBitmapPreviewEffect(const int &effect);
			void SetDiaporamaMode();
			void SetNormalMode();
			void UpdateFiltre(CEffectParameter * effectParameter);
			void OnFiltreOk();
			void OnFiltreCancel();
			CRgbaquad GetBackColor();
			int GetOrientation();
			CDraw * GetDessinPt();
			int IsSupportOpenCL();
			wxPoint GetMousePosition();

		private:
			virtual void ApplyPreviewEffect(int & widthOutput, int & heightOutput);
			void AfterRender();
			void SetDessinRatio();
			void DeterminePos(wxRect &rc, const int &nTailleAffichageWidth, const int &nTailleAffichageHeight, int &left, int &top);
			
			void Resize();
			
#ifdef KeyPress
#undef KeyPress
#endif

			void KeyPress(const int &key);
			void MouseMove(const int &xPos, const int &yPos);
			void MouseClick(const int &xPos, const int &yPos);
			void MouseRelease(const int &xPos, const int &yPos);
			int GetRawBitmapWidth();
			int GetRawBitmapHeight();
			//int GetOrientation();
			void AfterSetBitmap();

			void EndTransition();
			void OnTransition(wxTimerEvent& event);
			
            void LoadingResource();
            
			wxCursor hCursorCross;
			CDraw * m_cDessin;
			CImageLoadingFormat * nextPicture;
			wxImage arrowPrevious;
			wxImage arrowNext;
			bool startTransition;
			int etape;
			bool fixArrow;
			CBitmapInterface * bitmapInterface;

			//Preview Parameter
			int preview;
			CEffectParameter * effectParameter;
			IAfterEffect * afterEffect;

			wxPoint oldMouse;

			//Thread Parameter
			bool m_bTransition;

			bool isDiaporama;
			bool isInUse;
			bool invertColor;
			wxTimer * transitionTimer;
			wxTimer * selectEffectTimer;
			GLTexture * pictureNext;
			IMouseUpdate * mouseUpdate;

		};
	}
}

