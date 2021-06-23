#pragma once
#include "BitmapFusionFilter.h"

namespace Regards
{
	namespace Filter
	{
		class CMoveEffectTextureEffect : public CBitmapFusionFilter
		{
		public:
			CMoveEffectTextureEffect()
			{
			};

			~CMoveEffectTextureEffect() override
			{
			};

			void AfterRender(CImageLoadingFormat* nextPicture, CRenderBitmapOpenGL* renderOpenGL,
			                 IBitmapDisplay* bmpViewer, const int& etape, const float& scale_factor, const bool& isNext,
			                 float& ratio) override;

			void RenderMoveTexture(int& x, int& y, GLTexture* glTexture, const int& etape, const bool& isNext) override;

			int GetTypeFilter() override;
		};
	}
}
