#pragma once
#include "BitmapWndRender.h"
#include <BitmapInterface.h>
#include "AfterEffect.h"
#include <FiltreUpdate.h>

class CDecodeRawPicture;
class CEffectParameter;
class IMouseUpdate;
class CImageLoadingFormat;
using namespace Regards::FiltreEffet;
using namespace Regards::Window;

namespace Regards
{
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
using namespace Regards::Window;

namespace Regards::Control
{
	class CBitmapWndViewer : public CBitmapWndRender, public IFiltreUpdate
	{
	public:
		CBitmapWndViewer(CSliderInterface* slider, wxWindowID mainViewerId, const CThemeBitmapWindow& theme,
		                 CBitmapInterface* bitmapInterface);
		~CBitmapWndViewer() override;
		void SetParent(wxWindow* parent) override;
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

		bool IsPictureLoad();
		void OnTransition(wxTimerEvent& event);
		void OnClick(wxTimerEvent& event);

		void OnTimer(wxTimerEvent& event) override;

		vector<int> GetListTimer() override;
		void ApplyPicturePosition(const int& angle, const int& flipH, const int& flipV);

		static IAfterEffect* AfterEffectPt(const int& numFilter);

	private:
		bool IsOpenCLCompatible();
		
		void RenderTexture(const bool& invertPos) override;
		bool ApplyPreviewEffect(int& widthOutput, int& heightOutput) override;

		void AfterRender() override;


		void SetDessinRatio() override;
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

		void BeforeInterpolationBitmap() override;

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

		int typeClick = 0;
		bool isNext = false;
		int oldTransNumEffect = -1;
		wxRect arrowPrevious;
		wxRect arrowNext;
		wxTimer* transitionTimer;
		wxTimer* clickTimer;
	};
}
