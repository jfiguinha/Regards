#pragma once
#include "BitmapFusionFilter.h"

class CRegardsBitmap;
class CImageLoadingFormat;
class IBitmapDisplay;

namespace Regards
{
	namespace Filter
	{
		class CPageCurlFilter : public CBitmapFusionFilter
		{
		public:
			CPageCurlFilter();
			~CPageCurlFilter();
			virtual int GetTypeFilter();
			
			virtual GLTexture * GetTexture(const int &numTexture);
			virtual void DeleteTexture();
			virtual void SetTransitionBitmap(const bool& openCL, const bool& start, IBitmapDisplay* bmpViewer, CImageLoadingFormat* bmpSecond);
			void AfterRender(CImageLoadingFormat* nextPicture, const bool& isOpenCL, CRenderBitmapOpenGL* renderOpenGL, IBitmapDisplay* bmpViewer, const int& etape, const float& scale_factor, const bool& isNext, float& ratio) {};
			virtual bool RenderTexture(CImageLoadingFormat* nextPicture, CImageLoadingFormat* source, IBitmapDisplay* bmpViewer, CRenderBitmapOpenGL* renderOpenGL, const float& scale_factor, const int& etape);
		private:

			void GenerateTexture(CImageLoadingFormat* nextPicture, CImageLoadingFormat* source, IBitmapDisplay* bmpViewer);
			void RenderTexture(CRenderBitmapOpenGL* renderOpenGL, const float& time, const float& invert, const int& width, const int& height, const int& left, const int& top);

			Regards::OpenGL::GLTexture * pictureFirst;
			bool initTexture = false;
		};
	}
}

