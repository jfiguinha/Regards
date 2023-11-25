#pragma once
#include "BitmapFusionFilter.h"

class CRegardsBitmap;
class CImageLoadingFormat;
class IBitmapDisplay;

namespace Regards::Filter
{
	class CPageCurlFilter : public CBitmapFusionFilter
	{
	public:
		CPageCurlFilter();
		~CPageCurlFilter() override;
		int GetTypeFilter() override;

		GLTexture* GetTexture(const int& numTexture) override;
		void DeleteTexture() override;
		void SetTransitionBitmap(const bool& start, IBitmapDisplay* bmpViewer,
		                         CImageLoadingFormat* bmpSecond) override;

		void AfterRender(CImageLoadingFormat* nextPicture, CRenderBitmapOpenGL* renderOpenGL,
		                 IBitmapDisplay* bmpViewer, const int& etape, const float& scale_factor, const bool& isNext,
		                 float& ratio) override
		{
		};
		bool RenderTexture(CImageLoadingFormat* nextPicture, CImageLoadingFormat* source, IBitmapDisplay* bmpViewer,
		                   CRenderBitmapOpenGL* renderOpenGL, const float& scale_factor, const int& etape) override;

	private:
		void GenerateTexture(CImageLoadingFormat* nextPicture, CImageLoadingFormat* source,
		                     IBitmapDisplay* bmpViewer);
		virtual void RenderTexture(CRenderBitmapOpenGL* renderOpenGL, const float& time, const float& invert,
		                           const int& width, const int& height, const int& left, const int& top);

		GLTexture* pictureFirst;
		bool initTexture = false;
	};
}
