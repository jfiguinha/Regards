#pragma once

#include "WindowMain.h"
#include "WindowOpenGLMain.h"
#include <RegardsConfigParam.h>
#include <OpenCLEngine.h>
#include <theme.h>
#include "ScrollInterface.h"
#include "RenderBitmapInterfaceOpenGL.h"
#include "SliderInterface.h"
using namespace std;
using namespace Regards::OpenCL;

class CRegardsBitmap;
class CFiltreEffet;
class CImageLoadingFormat;
class CRegardsFloatBitmap;

#define MOVEPICTURE 2
#define ZOOMPICTURE 3

namespace Regards
{
	namespace Window
	{
		class CBitmapWnd : public CWindowOpenGLMain, public CScrollInterface
		{
		public:
			CBitmapWnd(wxWindow* parent, wxWindowID id, CSliderInterface * slider, wxWindowID idMain, const CThemeBitmapWindow & theme);
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
			void SetFilterInterpolation(const int &filter);
            void SetIsBitmapThumbnail(const bool &isThumbnail);
			void SetBitmap(CImageLoadingFormat * bitmap, const bool &copy = false);
			void SetBitmapParameter(const bool &externBitmap, const bool &addToTexture);
			CRegardsBitmap * GetBitmap(const bool &source);
            CRegardsFloatBitmap * GetFloatBitmap(const bool &source);
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

			void SetFullscreen(const bool &fullscreen);

            void StopLoadingBitmap();
            void StartLoadingBitmap();
            
		protected:

			virtual void CreateContext(){};
			virtual void ApplyPreviewEffect(){};
			virtual void AfterRender(){};
            int UpdateResized();
			void Update();
			virtual bool NeedAfterRenderBitmap(){ return false; };
			void CalculScreenPosFromReal(const int &xReal, const int &yReal, int &xScreen, int &yScreen);
			void CalculRealPosFromScreen(const int &xScreen, const int &yScreen, int &xReal, int &yReal);
			float CalculPictureRatio(const int &pictureWidth, const int &pictureHeight);
            
            void OnLoading(wxTimerEvent& event);
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
            
            void RefreshWindow();
            virtual void KeyPress(const int &key){};
			virtual void MouseClick(const int &xPos, const int &yPos);
			virtual void MouseRelease(const int &xPos, const int &yPos);
			virtual void MouseMove(const int &xPos, const int &yPos) {};

			virtual void AfterSetBitmap(){};
			virtual wxImage RenderBitmap(wxDC * deviceContext){ return wxImage();};
			virtual void AfterDrawBitmap(wxDC * deviceContext){};
			virtual void AfterRenderBitmap(wxDC * deviceContext){};
			virtual bool NeedAfterDrawBitmap();
			void GenerateScreenBitmap(CFiltreEffet * filtre, int &widthOutput, int &heightOutput);
			void GenerateExifPosition(int & localAngle, int & localflipHorizontal, int & localflipVertical);
			void TestMaxPosition(); //Test si les limites sont atteintes ou non

			float GetBitmapWidthWithRatio();
			float GetBitmapHeightWithRatio();

			virtual int GetRawBitmapWidth();
			virtual int GetRawBitmapHeight();
			virtual int GetOrientation();

			void TestMaxX();
			void TestMaxY();	

			void UpdateScrollBar(bool &update);
			float CalculRatio(const int &pictureWidth, const int &pictureHeight);

			void CalculCenterPicture();
			void CalculPositionPicture(const float &x, const float &y);
			void CalculRectPictureInterpolation(wxRect &picture, int &widthInterpolationSize, int &heightInterpolationSize, int &left, int &top, const bool &invert = true);
			int interpolation;
			bool zoom;
			//bool zoomOn;
			bool mouseBlock;
			bool shrinkImage;
				
			float ratio;
			int posRatio;
			bool postEffect;
				
			
            bool isThumbnail;
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
			//bool addToTexture;
			bool externBitmap;
			//wxImage render;
			//CRegardsBitmap * bitmap;
			wxWindowID idWindowMain;
			CSliderInterface * sliderInterface;
			CThemeBitmapWindow themeBitmap;
			CRegardsConfigParam * config;

			COpenCLEngine * openCLEngine;
			COpenCLContext * openclContext;
			CFiltreEffet * filtreEffet;
			int filterInterpolation;
			CRenderBitmapInterfaceOpenGL * renderOpenGL;
			GLTexture * glTexture;
			bool isOpenGL;

			int orientation;
			int flipVertical;
			int flipHorizontal;
			int angle;
			int bitmapwidth;
			int bitmapheight;
			bool bitmapUpdate;
			CImageLoadingFormat * source;
			bool bitmapLoad;
            mutex muBitmap;
            bool updateContext = true;
            bool timerUpdate = false;
            //bool fastRenderOpenGL = false;
            bool updateFilter = false;
		};
	}
}

