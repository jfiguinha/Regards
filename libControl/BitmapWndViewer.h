#pragma once
#include "BitmapWnd.h"
//#include <RenderPreviewBitmap.h>
#include <BitmapInterface.h>
#include <Draw.h>
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
			CBitmapWndViewer(wxWindow* parent, wxWindowID id, CSliderInterface * slider, wxWindowID mainViewerId, const CThemeBitmapWindow & theme, CBitmapInterface * bitmapInterface);
			~CBitmapWndViewer();
			CEffectParameter * GetParameter();
			void SetListener(IMouseUpdate * mouseUpdate);
			virtual void RemoveListener();
			void SendEmail();
			void ExportPicture();
			void SavePicture();
			void PrintPicture();
			void SetTransitionBitmap(CImageLoadingFormat * bmpSecond);
			void FixArrowNavigation(const bool &fix);
			void StopTransition();
			void SetBitmapPreviewEffect(const int &effect);
			void SetDiaporamaMode();
			void SetNormalMode();
			void SetNextPictureMove(const bool& value);
			void UpdateFiltre(CEffectParameter * effectParameter);
			void OnFiltreOk();
			void OnFiltreCancel();
			CRgbaquad GetBackColor();
			int GetOrientation();
			CDraw * GetDessinPt();
			void StartTransitionEffect(CImageLoadingFormat* bmpSecond, const bool& setPicture);
			void StopTransitionEffect(CImageLoadingFormat* bmpSecond);
			wxPoint GetMousePosition();
			void CalculCenterPositionPicture();

		private:


			bool IsOpenCLCompatible();
			IAfterEffect * AfterEffectPt(const int& numFilter);
			virtual void RenderTexture(const bool &invertPos);
			virtual bool ApplyPreviewEffect(int & widthOutput, int & heightOutput);

			virtual void AfterRender();
            virtual void DeleteTexture();

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
			void OnClick(wxTimerEvent& event);
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
			//CImageLoadingFormat* bmpSecond;

			
			IAfterEffect * afterEffect;

			wxPoint oldMouse;

			//Thread Parameter
			bool m_bTransition;

			bool isDiaporama;
			bool isInUse;
			bool invertColor;
			wxTimer * transitionTimer;
			wxTimer * selectEffectTimer;
			wxTimer * clickTimer;
			int typeClick = 0;

			bool isNext = false;
			int oldTransNumEffect = -1;		

		};
	}
}

