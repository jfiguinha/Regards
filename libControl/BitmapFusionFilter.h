#pragma once
#include "AfterEffect.h"


namespace Regards::Filter
{
	class CBitmapFusionFilter : public IAfterEffect
	{
	public:
		CBitmapFusionFilter();
		~CBitmapFusionFilter() override;
		int GetTypeFilter() override;
		void SetTransitionBitmap(const bool& start, IBitmapDisplay* bmpViewer,
		                         CImageLoadingFormat* bmpSecond) override;
		void AfterRender(CImageLoadingFormat* nextPicture, CRenderBitmapOpenGL* renderOpenGL,
		                 IBitmapDisplay* bmpViewer, const int& etape, const float& scale_factor, const bool& isNext,
		                 float& ratio) override;
		GLTexture* GetTexture(const int& numTexture) override;


	protected:
		virtual void GenerateTexture(CImageLoadingFormat* bitmap);
		virtual void GenerateEffectTexture(CImageLoadingFormat* nextPicture, IBitmapDisplay* bmpViewer);


		CImageLoadingFormat* GenerateInterpolationBitmapTexture(CImageLoadingFormat* nextPicture,
		                                                        IBitmapDisplay* bmpViewer);

		CImageLoadingFormat* _bmpSecond;
		int width;
		int height;
		wxRect out;
	};
}
