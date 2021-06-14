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
			virtual void SetTransitionBitmap(const bool& start, IBitmapDisplay* bmpViewer, CImageLoadingFormat* bmpSecond);
			virtual void AfterRender(CImageLoadingFormat* nextPicture, CRenderBitmapOpenGL* renderOpenGL, IBitmapDisplay* bmpViewer, const int& etape, const float& scale_factor, const bool& isNext, float& ratio);
			virtual GLTexture * GetTexture(const int &numTexture);
			virtual void DeleteTexture();

		protected:

			virtual void GenerateTexture(CRegardsBitmap* bitmap);
			virtual void GenerateEffectTexture(CImageLoadingFormat* nextPicture, IBitmapDisplay* bmpViewer);

			CRegardsBitmap* GenerateInterpolationBitmapTexture(CImageLoadingFormat* nextPicture, IBitmapDisplay* bmpViewer);
			Regards::OpenGL::GLTexture * pictureNext;
			CRegardsBitmap * _bmpSecond;
			int width;
			int height;
			wxRect out;
		};
	}
}

