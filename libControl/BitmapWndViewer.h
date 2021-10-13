#pragma once
#include "BitmapWnd.h"
#include <BitmapInterface.h>
#include "AfterEffect.h"
#include <FiltreUpdate.h>
using namespace Regards::OpenCL;
class CDecodeRawPicture;
class CEffectParameter;
class IMouseUpdate;
class CImageLoadingFormat;
using namespace Regards::FiltreEffet;
using namespace Regards::Window;

namespace Regards
{
	namespace OpenCL
	{
		class COpenCLEngine;
		class COpenCLContext;
	}

	namespace OpenGL
	{
		class GLTexture;
	}

	namespace Window
	{
		class CRenderBitmapInterfaceOpenGL;
	}
}

using namespace Regards::OpenGL;
using namespace Regards::OpenCL;
using namespace Regards::Window;

namespace Regards
{
	namespace Control
	{
		class CBitmapWndViewer : public CBitmapWnd, public IFiltreUpdate
		{
		public:
			CBitmapWndViewer(wxWindow* parent, wxWindowID id, CSliderInterface* slider, wxWindowID mainViewerId,
			                 const CThemeBitmapWindow& theme, CBitmapInterface* bitmapInterface);
			~CBitmapWndViewer() override;
			CEffectParameter* GetParameter() override;
			void SetListener(IMouseUpdate* mouseUpdate);
			void RemoveListener(const bool& applyCancel = true) override;
			void SendEmail();
			void ExportPicture();
			void SavePicture();
			void PrintPicture();
			void SetTransitionBitmap(CImageLoadingFormat* bmpSecond);
			void FixArrowNavigation(const bool& fix);
			void StopTransition();
			void SetBitmapPreviewEffect(const int& effect);
			void SetDiaporamaMode();
			void SetNormalMode();
			void SetNextPictureMove(const bool& value);
			void UpdateFiltre(CEffectParameter* effectParameter) override;
			void OnFiltreOk();
			void OnFiltreCancel();
			CRgbaquad GetBackColor() override;
			int GetOrientation() override;
			CDraw* GetDessinPt() override;
			void StartTransitionEffect(CImageLoadingFormat* bmpSecond, const bool& setPicture) override;
			void StopTransitionEffect(CImageLoadingFormat* bmpSecond) override;
			wxPoint GetMousePosition();
			void CalculCenterPositionPicture() override;

		private:
			bool IsOpenCLCompatible();
			IAfterEffect* AfterEffectPt(const int& numFilter);
			void RenderTexture(const bool& invertPos) override;
			bool ApplyPreviewEffect(int& widthOutput, int& heightOutput) override;

			void AfterRender() override;
			void DeleteTexture() override;

			void SetDessinRatio();
			void DeterminePos(wxRect& rc, const int& nTailleAffichageWidth, const int& nTailleAffichageHeight,
			                  int& left, int& top);

			void Resize() override;

#ifdef KeyPress
#undef KeyPress
#endif

			void KeyPress(const int& key) override;
			void MouseMove(const int& xPos, const int& yPos) override;
			void MouseClick(const int& xPos, const int& yPos) override;
			void MouseRelease(const int& xPos, const int& yPos) override;
			int GetRawBitmapWidth() override;
			int GetRawBitmapHeight() override;
			void AfterSetBitmap() override;
			void EndTransition();
			void OnTransition(wxTimerEvent& event);
			void OnClick(wxTimerEvent& event);
			virtual void BeforeInterpolationBitmap() override;

			wxCursor hCursorCross;
			CDraw* m_cDessin;
			CImageLoadingFormat* nextPicture;
			bool startTransition;
			int etape;
			bool fixArrow;
			CBitmapInterface* bitmapInterface;
			IAfterEffect* afterEffect;
			wxPoint oldMouse;

			//Thread Parameter
			bool m_bTransition;
			bool isDiaporama;
			bool isInUse;
			bool invertColor;
			wxTimer* transitionTimer;
			wxTimer* selectEffectTimer;
			wxTimer* clickTimer;
			int typeClick = 0;
			bool isNext = false;
			int oldTransNumEffect = -1;
			wxRect arrowPrevious;
			wxRect arrowNext;
		};
	}
}
