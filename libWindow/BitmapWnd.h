#pragma once

#include "WindowMain.h"
#include <RegardsConfigParam.h>
#include <Theme.h>
#include "ScrollInterface.h"
#include "SliderInterface.h"
#include <RenderBitmapInterface.h>
#include <RenderBitmapInterfaceOpenGL.h>
#include <RenderBitmapInterfaceGDI.h>
#include "BitmapWindowContext.h"
using namespace Regards::Render;

#define MOVEPICTURE 2
#define ZOOMPICTURE 3

namespace Regards
{
	namespace Window
	{
		class CBitmapWnd : public wxGLCanvas, public CScrollInterface
		{
		public:
			CBitmapWnd(wxWindow* parent, wxWindowID id, CSliderInterface * slider, CWindowMain * main, const CThemeBitmapWindow & theme);
			virtual ~CBitmapWnd(void);

			//Attribut
			void SetZoom(bool active);
			bool GetZoom();
			void SetZoomPosition(const int &position);

			int GetWidth();
			int GetHeight();

			wxWindow * GetWindow(){
				return this;
			}
            
            void UpdateScreenRatio();

			void SetBitmap(CRegardsBitmap * bitmap, const bool &copy = false);
			void SetBitmapParameter(const bool &externBitmap, const bool &addToTexture);
			CRegardsBitmap * GetBitmap();
			void GetInfosBitmap(wxString &filename, int &widthPicture, int &heightPicture);

			int GetPosRatio();
			void SetRatioPos(const int &pos);

			int GetBitmapWidth();
			int GetBitmapHeight();

			void FlipVertical();
			void Rotate90();
			void Rotate270();
			void FlipHorizontal();

			void ZoomOn();
			void ZoomOut();

			void SetShowScroll(bool visible);
			bool GetShowScroll();

			void SetShrinkImage(bool active);
			bool GetShrinkImage();
			void ShrinkImage(const bool &redraw = true);

			void SetInterpolation(int interpolation);
			int GetInterpolation();

			float GetRatio();
	
			void SetTool(const int &tool);
			virtual void SetToolOption(const int &tool, const int &iKey){};
			int GetTool();

			void SetKey(const int &iKey);
			virtual void SetKeyOption(const int &iKey){};
			int GetKey();

			void SetPosition(const int &left, const int &top);

			bool IsGpGpuCompatible();
			void SetFullscreen(const bool &fullscreen);
			void InitRenderInterface();

            void StopLoadingBitmap();
            void StartLoadingBitmap();
            
		protected:

            int UpdateResized();
			void Update();

			void CalculScreenPosFromReal(const int &xReal, const int &yReal, int &xScreen, int &yScreen);
			void CalculRealPosFromScreen(const int &xScreen, const int &yScreen, int &xReal, int &yReal);
			float CalculPictureRatio(const int &pictureWidth, const int &pictureHeight);
            
            void OnLoading(wxTimerEvent& event);
            CBitmapWindowContext& GetContext(wxGLCanvas *canvas);

			void OnMouseCaptureLost(wxMouseEvent& event){};
			void OnPaint(wxPaintEvent& event);
			void OnSize(wxSizeEvent& event);
			void OnMouseMove(wxMouseEvent& event);
			void OnLButtonDown(wxMouseEvent& event);
			void OnRButtonDown(wxMouseEvent& event);
			void OnLButtonUp(wxMouseEvent& event);
			void OnLDoubleClick(wxMouseEvent& event);
			void OnMouseWheel(wxMouseEvent& event);
			void OnKeyDown(wxKeyEvent& event);
			void OnKeyUp(wxKeyEvent& event);
		    void OnEraseBackground(wxEraseEvent& event){};
            
#ifdef KeyPress
#undef KeyPress
#endif			
			virtual void KeyPress(const int &key) = 0;
			virtual void MouseClick(const int &xPos, const int &yPos);
			virtual void MouseRelease(const int &xPos, const int &yPos);
			virtual void MouseMove(const int &xPos, const int &yPos) = 0;
			virtual void Resize(const int &width, const int &height) = 0;

			virtual void BeforeDrawBitmap(CRenderBitmapInterface * renderInterface){};
			virtual void DrawBitmap(wxDC * deviceContext);
			virtual void AfterSetBitmap(){};
			virtual void RenderBitmap(CRenderBitmapInterface * renderInterface);
			virtual void AfterRenderBitmap(CRenderBitmapInterface * renderInterface){};
			virtual void AfterDrawBitmap(wxDC * deviceContext){};
           virtual bool NeedAfterDrawBitmap();
			virtual void Initialize(){};

			void TestMaxPosition(); //Test si les limites sont atteintes ou non

			float GetBitmapWidthWithRatio();
			float GetBitmapHeightWithRatio();
			void TestMaxX();
			void TestMaxY();	
					
			void GetPositionPicture(int &left, int &top);
			void UpdateScrollBar(bool &update);
			float CalculRatio(const int &pictureWidth, const int &pictureHeight);

			void CalculCenterPicture();
			void CalculPositionPicture(const float &x, const float &y);

			void CalculRectPictureInterpolation(wxRect &picture, int &widthInterpolationSize, int &heightInterpolationSize, int &left, int &top);

			int interpolation;
			bool zoom;
			bool zoomOn;
			bool mouseBlock;
			bool shrinkImage;
				
			float ratio;
			int posRatio;
			bool postEffect;
				
			CRegardsBitmap * bitmap = nullptr;
				
			bool isInUse;
			int toolOption;

			wxCursor hCursorHand;
			wxCursor hCursorZoomIn;
			wxCursor hCursorZoomOut;

			int iKey;

			static const long Max;
			static const float TabRatio[];

			wxString filename;

			float centerX;
			float centerY;
			bool showScroll;

			int width;
			int height;

			int mouseScrollX;
			int mouseScrollY;

			bool fastRender;
			bool addToTexture;
			bool externBitmap;


			bool directComputeCompatible = false;
			CRenderBitmapInterface * renderInterface = nullptr;
			CWindowMain * windowMain = nullptr;
			CSliderInterface * sliderInterface = nullptr;
			CThemeBitmapWindow themeBitmap;
			CRegardsConfigParam * config = nullptr;
            CBitmapWindowContext * m_glContext = nullptr;
            wxTimer * loadingTimer = nullptr;
            bool showLoadingBitmap = false;
            int stepLoading = 0;
            bool useLoadingPicture = false;
		};
	}
}

