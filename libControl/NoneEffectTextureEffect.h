#pragma once
#include "BitmapFusionFilter.h"
#include "BitmapDisplay.h"
#include <effect_id.h>
namespace Regards
{
	namespace Filter
	{
		class CNoneEffectTextureEffect : public CBitmapFusionFilter
		{
		public:
			CNoneEffectTextureEffect()
			{
			};

			~CNoneEffectTextureEffect() override
			{
			};

			int GetTypeFilter() override
			{
				return IDM_AFTEREFFECT_NONE;
			}

			void SetTransitionBitmap(const bool& start, IBitmapDisplay* bmpViewer,
			                         CImageLoadingFormat* bmpSecond) override
			{
				bmpViewer->StopTransitionEffect(bmpSecond);
			}
		};
	}
}
