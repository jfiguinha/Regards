#pragma once
#include "BitmapWnd.h"
#include <RenderPreviewBitmap.h>
#include <BitmapInterface.h>
#include <StatusBarInterface.h>
#include <Draw.h>
#include <FiltreUpdate.h>
#include <OpenCLContext.h>
#include <OpenCLEffectVideo.h>
using namespace Regards::OpenCL;
class CDecodeRawPicture;
class CEffectParameter;
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
			CBitmapWndViewer(wxWindow* parent, wxWindowID id, CSliderInterface * slider, wxWindowID mainViewerId, const CThemeBitmapWindow & theme, CBitmapInterface * bitmapInterface, IStatusBarInterface * statusBarInterface);
			~CBitmapWndViewer();
			CEffectParameter * GetParameter();
			void SendEmail();
			void SavePicture();
			void PrintPicture();
			void SetTransitionBitmap(CImageLoadingFormat * bmpSecond);
			void FixArrowNavigation(const bool &fix);
			void StopTransition();
			void SetBitmapPreviewEffect(const int &effect);
			void SetBitmapPreviewEffect(const int &effect, CEffectParameter * effectParameter);
			bool SetBitmapEffect(const int &effect, CEffectParameter * effectParameter = nullptr);
            bool NeedAfterDrawBitmap();
			void SetDiaporamaMode();
			void SetNormalMode();
			void UpdateFiltre(CEffectParameter * effectParameter);
			void ApplyEffect(const int &effect);
			void OnFiltreOk();
			void OnFiltreCancel();
		private:

			void ApplyPreviewEffect();
			void AfterRender();
			void CreateContext();
			void ShowArrowNext(wxDC * deviceContext);
			void ShowArrowPrevious(wxDC * deviceContext);
			wxImage RenderBitmap(wxDC * deviceContext);
			void AfterDrawBitmap(wxDC * deviceContext);
			void AfterRenderBitmap(wxDC * deviceContext);

			bool NeedAfterRenderBitmap();
			void SetDessinRatio();
			void DeterminePos(wxRect &rc, const int &nTailleAffichageWidth, const int &nTailleAffichageHeight, int &left, int &top);
			
			void Resize();
			
#ifdef KeyPress
#undef KeyPress
#endif
			
			wxImage CreateFromSVG(const wxString & vector, const int & buttonWidth, const int & buttonHeight);
			void KeyPress(const int &key);
			void MouseMove(const int &xPos, const int &yPos);
			void MouseClick(const int &xPos, const int &yPos);
			void MouseRelease(const int &xPos, const int &yPos);
			int GetRawBitmapWidth();
			int GetRawBitmapHeight();
			int GetOrientation();
			void AfterSetBitmap();

			void EndTransition();
			void OnTransition(wxTimerEvent& event);
			void onIdle(wxIdleEvent& evt);
			
            void LoadingResource();
            
			wxCursor hCursorCross;
			CDraw * m_cDessin;
			CImageLoadingFormat * nextPicture;
			wxImage arrowPrevious;
			wxImage arrowNext;
			wxImage renderNext;
			wxImage renderPreview;
			vector<wxImage> vecLoadingBitmap;
			bool startTransition;
			int etape;
			bool fixArrow;
			CBitmapInterface * bitmapInterface;

			//Preview Parameter
			int preview;
			CEffectParameter * effectParameter;
			CRenderPreviewBitmap * renderPreviewBitmap;
			CDecodeRawPicture * rawDecoder;
			IStatusBarInterface * statusBarInterface;
			wxPoint oldMouse;

			//Thread Parameter
			bool m_bTransition;
			void OnThreadStep();
			bool isDiaporama;
			bool isInUse;
			bool invertColor;
			wxTimer * transitionTimer;
			wxTimer * selectEffectTimer;
			//CRegardsBitmap * rawBitmap;
			COpenCLEffectVideo * openclEffectVideo;
			GLTexture * pictureNext;
			cl_mem cl_nextPicture;
			CFiltreEffet * filtreraw;
			int rawWidth;
			int rawHeight;
		};
	}
}

