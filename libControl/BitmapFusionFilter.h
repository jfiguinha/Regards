#pragma once
#include "AfterEffect.h"


namespace Regards
{
	namespace Filter
	{
		class CBitmapFusionFilter : public IAfterEffect
		{
		public:
			CBitmapFusionFilter();
			~CBitmapFusionFilter();
			virtual int GetTypeFilter();
			virtual void SetTransitionBitmap(const bool& openCL, const bool& start, IBitmapDisplay* bmpViewer, CImageLoadingFormat* bmpSecond);
			virtual void AfterRender(CImageLoadingFormat* nextPicture, const bool& isOpenCL, CRenderBitmapOpenGL* renderOpenGL, IBitmapDisplay* bmpViewer, const int& etape, const float& scale_factor, const bool& isNext, float& ratio);
#ifdef RENDEROPENGL
			void GenerateEffectTexture(CImageLoadingFormat* nextPicture, const bool& isOpenCL, IBitmapDisplay* bmpViewer);
			virtual void GenerateTexture(CRegardsBitmap* bitmap);
			virtual GLTexture * GetTexture(const int &numTexture);
			virtual void DeleteTexture();
#endif

		protected:

			CRegardsBitmap* GenerateInterpolationBitmapTexture(CImageLoadingFormat* nextPicture, IBitmapDisplay* bmpViewer);
			Regards::OpenGL::GLTexture * pictureNext;
			CRegardsBitmap * _bmpSecond;
			int width;
			int height;
			wxRect out;
		};
	}
}

