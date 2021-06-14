#pragma once
#include "BitmapFusionFilter.h"

namespace Regards
{
	namespace Filter
	{
		class CMoveEffectTextureEffect : public CBitmapFusionFilter
		{
		public:
			CMoveEffectTextureEffect() {};
			~CMoveEffectTextureEffect() {};

			virtual void AfterRender(CImageLoadingFormat* nextPicture, CRenderBitmapOpenGL* renderOpenGL, IBitmapDisplay* bmpViewer, const int& etape, const float& scale_factor, const bool& isNext, float& ratio);

			virtual void RenderMoveTexture(int& x, int& y, GLTexture* glTexture, const int& etape, const bool& isNext);

			int GetTypeFilter();

		};
	}
}