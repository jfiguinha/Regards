#pragma once
#pragma once
#include "WindowOpenGLMain.h"
#include <RegardsConfigParam.h>
#include <theme.h>
#include "IBitmapRenderInterface.h"
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
		class CBitmapWndRender : public IBitmapDisplay, public IBitmapRenderInterface
		{
		public:
			CBitmapWndRender(CSliderInterface* slider, wxWindowID idMain, const CThemeBitmapWindow& theme);
			~CBitmapWndRender(void) override;

			virtual void SetParent(wxWindow* parent) override;
			int GetWidth() override;
			int GetHeight() override;
			//Attribut
			void SetZoom(bool active);
			bool GetZoom();
			void SetZoomPosition(const int& position);
			wxString GetFilename() override;
			void ReloadResource();
			CFiltreEffet* GetFiltreEffet() override;

			virtual void SetPreview(const int& value);

			void UpdateScreenRatio() override;
			void SetIsBitmapThumbnail(const bool& isThumbnail);
			void UpdateBitmap(CImageLoadingFormat* bitmap, const bool& updateAll) override;
			void SetBitmap(CImageLoadingFormat* bitmap, const bool& copy = false);
			void SetBitmapParameter(const bool& externBitmap, const bool& addToTexture);
			CRegardsBitmap* GetBitmap(const bool& source) override;
			CRegardsFloatBitmap* GetFloatBitmap(const bool& source) override;
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

			float GetRatio() override;

			void SetTool(const int& tool);

			virtual void SetToolOption(const int& tool, const int& iKey)
			{
			};
			int GetTool();

			void SetKey(const int& iKey);

			virtual void SetKeyOption(const int& iKey)
			{
			};
			int GetKey();

			void SetOpenGLOutput(const bool& value);

			void SetFullscreen(const bool& fullscreen);

			int GetHPos() override;
			int GetVPos() override;

			float CalculPictureRatio(const int& pictureWidth, const int& pictureHeight) override;

			void ReinitPicture();
			void SetOrientation(const int& exif);

			virtual void RemoveListener(const bool& applyCancel)
			{
			};

			virtual void StartTransitionEffect(CImageLoadingFormat* bmpSecond, const bool& setPicture) override
			{
			};

			virtual void StopTransitionEffect(CImageLoadingFormat* bmpSecond) override
			{
			};

			virtual void CalculCenterPositionPicture() override
			{
			};

			virtual void SetEndProgram(const bool& endProgram) override;

			virtual void OnPaint3D(wxGLCanvas* canvas, COpenCLContext* openclContext) override;
			virtual void OnPaint2D(wxWindow * gdi, COpenCLContext* openclContext) override;
			virtual void OnMouseMove(wxMouseEvent& event) override;
			virtual void OnLButtonDown(wxMouseEvent& event) override; 
			virtual void OnRButtonDown(wxMouseEvent& event) override;
			virtual void OnLButtonUp(wxMouseEvent& event) override;
			virtual void OnLDoubleClick(wxMouseEvent& event) override;
			virtual void OnMouseWheel(wxMouseEvent& event) override;
			virtual void OnKeyDown(wxKeyEvent& event) override;
			virtual void OnKeyUp(wxKeyEvent& event) override;
			virtual void OnIdle(wxIdleEvent& evt) override
			{
			};

			virtual void OnTimer(wxTimerEvent& event) override {};
			virtual vector<int> GetListTimer() override{
				vector<int> list; return list;
			};

			virtual void OnCommand(wxCommandEvent& event) override;
			virtual vector<int> GetListCommand() override;
			virtual int UpdateResized() override;
			virtual bool GetProcessEnd() override
			{
				return endProgram;
			}
		protected:

			virtual void OnUpdateBitmap(wxCommandEvent& event);
			virtual void OnLeftPosition(wxCommandEvent& event);
			virtual void OnTopPosition(wxCommandEvent& event);
			virtual void OnMoveLeft(wxCommandEvent& event);
			virtual void OnMoveRight(wxCommandEvent& event);
			virtual void OnMoveTop(wxCommandEvent& event);
			virtual void OnMoveBottom(wxCommandEvent& event);
			virtual void OnScrollMove(wxCommandEvent& event);

			int GetExifOrientation(const int& angle);
			virtual bool ApplyPreviewEffect(int& widthOutput, int& heightOutput) { return false; };

			virtual void AfterRender()
			{
			};
			

			int IsSupportOpenCL();
			virtual bool NeedAfterRenderBitmap() { return false; };
			void CalculScreenPosFromReal(const int& xReal, const int& yReal, int& xScreen, int& yScreen);
			void CalculRealPosFromScreen(const int& xScreen, const int& yScreen, int& xReal, int& yReal);

			virtual void RenderTexture(const bool& invertPos);

			void Resize() override;

			virtual CDraw* GetDessinPt() override
			{
				return nullptr;
			}

			virtual CRgbaquad GetBackColor() override;

#ifdef KeyPress
#undef KeyPress
#endif
			void RenderToScreenWithOpenCLSupport();
			void RenderToScreenWithoutOpenCLSupport();
			void RefreshWindow();
			//virtual CRegardsBitmap* RenderSpecialEffect() { return nullptr; };
			virtual void KeyPress(const int& key)
			{
			};
			virtual void MouseClick(const int& xPos, const int& yPos);
			virtual void MouseRelease(const int& xPos, const int& yPos);

			virtual void MouseMove(const int& xPos, const int& yPos)
			{
			};

			virtual void AfterSetBitmap()
			{
			};

			virtual void BeforeInterpolationBitmap()
			{
			};

			void GenerateScreenBitmap(CFiltreEffet* filtre, int& widthOutput, int& heightOutput);
			void GenerateExifPosition(int& localAngle, int& localflipHorizontal, int& localflipVertical);
			void TestMaxPosition(); //Test si les limites sont atteintes ou non

			float GetBitmapWidthWithRatio();
			float GetBitmapHeightWithRatio();

			virtual int GetRawBitmapWidth();
			virtual int GetRawBitmapHeight();
			int GetOrientation() override;

			virtual void DeleteTexture()
			{
			};

			float CalculRatio(const int& pictureWidth, const int& pictureHeight);

			void CalculCenterPicture();
			void CalculPositionPicture(const float& x, const float& y);
			void CalculRectPictureInterpolation(wxRect& picture, int& widthInterpolationSize, int& heightInterpolationSize,
				int& left, int& top, const bool& invert = true);

			int IsOpenGLDecoding();

			void UpdateScrollBar();
			void TestMaxY();
			void TestMaxX();
			void MoveTop();
			void MoveLeft();
			void MoveBottom();
			void MoveRight();
			int GetAngleFromExif();

			int interpolation;
			bool zoom;
			//bool zoomOn;
			bool mouseBlock;
			bool shrinkImage;

			float ratio;
			int posRatio;
			bool postEffect;

			//bool reloadResource = false;
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
			CSliderInterface* sliderInterface;
			CThemeBitmapWindow themeBitmap;
			CRegardsConfigParam* config;

			bool copyBitmap = false;
			CFiltreEffet* filtreEffet = nullptr;
			CRegardsBitmap* copyBmpSrc = nullptr;
			bool needUpdate = false;
			CRenderBitmapOpenGL* renderBitmapOpenGL = nullptr;
			//CRenderPageCurlOpenGL * pageCurlOpenGL = nullptr;
			GLTexture* glTexture = nullptr;
			GLTexture* glTextureSrc = nullptr;
			COpenCLContext* openclContext = nullptr;

			//Preview Parameter
			int preview = 0;

			int orientation;
			int flipVertical;
			int flipHorizontal;
			int angle;
			int bitmapwidth;
			int bitmapheight;
			bool bitmapUpdate;
			CImageLoadingFormat* source;
			bool bitmapLoad;
			mutex muBitmap;
			bool updateFilter = false;
			int xPosImage = 0;
			int yPosImage = 0;
			bool controlKeyPush = false;

			int posLargeur = 0;
			int posHauteur = 0;

			bool isOpenGLShow = true;
			bool loadBitmap = false;
			int isMoving;
			bool destroyOpenGLRender = false;
			IMouseUpdate* mouseUpdate;
			CEffectParameter* effectParameter;
			bool openGLRenderBitmap = true;

			bool endProgram = false;
			wxWindow* parentRender = nullptr;
            
            double scale_factor = 1.0f;
		};
	}
}
