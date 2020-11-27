#pragma once

//#define RENDEROPENGL

#include "WindowMain.h"
#ifdef RENDEROPENGL
#include "WindowOpenGLMain.h"
#endif
#include <RegardsConfigParam.h>
#include <theme.h>

#include "SliderInterface.h"
#include <BitmapDisplay.h>
using namespace std;
using namespace Regards::OpenCL;

class CRegardsBitmap;
class CFiltreEffet;
class CImageLoadingFormat;
class CRegardsFloatBitmap;
class IMouseUpdate;
class CEffectParameter;

#define MOVEPICTURE 2
#define ZOOMPICTURE 3

namespace Regards
{
	namespace OpenCL
	{

		class COpenCLEngine;
		class COpenCLContext;

	}

	namespace OpenGL
	{
		class CRenderPageCurlOpenGL;
		class GLTexture;
		class CRenderBitmapOpenGL;
	}
}
using namespace Regards::OpenGL;
using namespace Regards::OpenCL;
using namespace Regards::Window;
namespace Regards
{
	namespace Window
	{
#ifdef RENDEROPENGL
		class CBitmapWnd : public CWindowOpenGLMain, public IBitmapDisplay
#else
		class CBitmapWnd : public CWindowMain, public IBitmapDisplay
#endif
		{
		public:
			CBitmapWnd(wxWindow* parent, wxWindowID id, CSliderInterface* slider, wxWindowID idMain, const CThemeBitmapWindow& theme);
			virtual ~CBitmapWnd(void);

			//Attribut
			void SetZoom(bool active);
			bool GetZoom();
			void SetZoomPosition(const int& position);
			wxString GetFilename();

			int GetWidth();
			int GetHeight();
			virtual CFiltreEffet * GetFiltreEffet();

			wxWindow* GetWindow() {
				return this;
			}

			void UpdateScreenRatio();
			void SetIsBitmapThumbnail(const bool& isThumbnail);
			void UpdateBitmap(CImageLoadingFormat* bitmap, const bool& updateAll);
			void SetBitmap(CImageLoadingFormat* bitmap, const bool& copy = false);
			void SetBitmapParameter(const bool& externBitmap, const bool& addToTexture);
			CRegardsBitmap* GetBitmap(const bool& source);
			CRegardsFloatBitmap* GetFloatBitmap(const bool& source);
			void GetInfosBitmap(wxString& filename, int& widthPicture, int& heightPicture);

			int GetPosRatio();
			void SetRatioPos(const int& pos);

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
			void ShrinkImage(const bool& redraw = true);

			float GetRatio();

			void SetTool(const int& tool);
			virtual void SetToolOption(const int& tool, const int& iKey) {};
			int GetTool();

			void SetKey(const int& iKey);
			virtual void SetKeyOption(const int& iKey) {};
			int GetKey();


			void SetFullscreen(const bool& fullscreen);

			int GetHPos();
			int GetVPos();

			float CalculPictureRatio(const int& pictureWidth, const int& pictureHeight);

			COpenCLContext * GetOpenCLContext();
			//void StopLoadingBitmap();
			//void StartLoadingBitmap();
			void ReinitPicture();
			void SetOrientation(const int &exif);


		protected:

			virtual bool ApplyPreviewEffect(int & widthOutput, int & heightOutput) { return false; };
#ifdef RENDEROPENGL
			virtual void AfterRender() {};
#else
			virtual void AfterRender(wxDC * dc) {};
#endif
			int UpdateResized();

			virtual bool NeedAfterRenderBitmap() { return false; };
			void CalculScreenPosFromReal(const int& xReal, const int& yReal, int& xScreen, int& yScreen);
			void CalculRealPosFromScreen(const int& xScreen, const int& yScreen, int& xReal, int& yReal);
			
			virtual void RenderTexture(const bool &invertPos);

			//void OnLoading(wxTimerEvent& event);
			void OnMouseCaptureLost(wxMouseEvent& event) {};
			void OnPaint(wxPaintEvent& event);
			void OnMouseMove(wxMouseEvent& event);
			void OnLButtonDown(wxMouseEvent& event);
			void OnRButtonDown(wxMouseEvent& event);
			void OnLButtonUp(wxMouseEvent& event);
			void OnLDoubleClick(wxMouseEvent& event);
			void OnMouseWheel(wxMouseEvent& event);
			void OnKeyDown(wxKeyEvent& event);
			void OnKeyUp(wxKeyEvent& event);
			void OnEraseBackground(wxEraseEvent& event) {};
			void OnUpdateBitmap(wxCommandEvent& event);
			void OnLeftPosition(wxCommandEvent& event);
			void OnTopPosition(wxCommandEvent& event);
			void OnMoveLeft(wxCommandEvent& event);
			void OnMoveRight(wxCommandEvent& event);
			void OnMoveTop(wxCommandEvent& event);
			void OnMoveBottom(wxCommandEvent& event);
			void OnScrollMove(wxCommandEvent& event);

			virtual void Resize();

#ifdef KeyPress
#undef KeyPress
#endif			
#ifdef RENDEROPENGL
			void RenderToScreenWithOpenCLSupport();
			void RenderToScreenWithoutOpenCLSupport();
#else
			void RenderToScreenWithOpenCLSupport(wxDC * dc);
			void RenderToScreenWithoutOpenCLSupport(wxDC * dc);
#endif
			void RefreshWindow();
			//virtual CRegardsBitmap* RenderSpecialEffect() { return nullptr; };
            virtual void KeyPress(const int &key){};
			virtual void MouseClick(const int &xPos, const int &yPos);
			virtual void MouseRelease(const int &xPos, const int &yPos);
			virtual void MouseMove(const int &xPos, const int &yPos) {};

			virtual void AfterSetBitmap(){};

			void GenerateScreenBitmap(CFiltreEffet * filtre, int &widthOutput, int &heightOutput);
			void GenerateExifPosition(int & localAngle, int & localflipHorizontal, int & localflipVertical);
			void TestMaxPosition(); //Test si les limites sont atteintes ou non

			float GetBitmapWidthWithRatio();
			float GetBitmapHeightWithRatio();

			virtual int GetRawBitmapWidth();
			virtual int GetRawBitmapHeight();
			virtual int GetOrientation();
            virtual void DeleteTexture() {};

			float CalculRatio(const int &pictureWidth, const int &pictureHeight);

			void CalculCenterPicture();
			void CalculPositionPicture(const float &x, const float &y);
			void CalculRectPictureInterpolation(wxRect &picture, int &widthInterpolationSize, int &heightInterpolationSize, int &left, int &top, const bool &invert = true);

            int IsOpenGLDecoding();

			void UpdateScrollBar();
			void TestMaxY();
			void TestMaxX();
			void MoveTop();
			void MoveLeft();
			void MoveBottom();
			void MoveRight();

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

			//int width;
			//int height;

			int mouseScrollX;
			int mouseScrollY;

			bool externBitmap;

			wxWindowID idWindowMain;
			CSliderInterface * sliderInterface;
			CThemeBitmapWindow themeBitmap;
			CRegardsConfigParam * config;


			CFiltreEffet * filtreEffet;
			//int filterInterpolation;
#ifdef RENDEROPENGL
			CRenderBitmapOpenGL * renderOpenGL;
			CRenderPageCurlOpenGL * pageCurlOpenGL;
			GLTexture * glTexture;
			GLTexture * glTextureSrc = nullptr;
			COpenCLEngine * openCLEngine;
			COpenCLContext * openclContext;
#endif
			//bool isOpenGL;

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
            bool updateFilter = false;
			int xPosImage = 0;
			int yPosImage = 0;
			bool controlKeyPush = false;

			int posLargeur = 0;
			int posHauteur = 0;

			int oldWidth = 0;
			int oldHeight = 0;
			bool loadBitmap = false;
			int isMoving;
            bool destroyOpenGLRender = false;
			IMouseUpdate * mouseUpdate;
			CEffectParameter * effectParameter;
            bool openGLRenderBitmap = true;
		};
	}
}

