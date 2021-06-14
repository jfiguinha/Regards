#pragma once
#include "BitmapFusionFilter.h"
#include "BitmapDisplay.h"
#include <FilterData.h>
namespace Regards
{
	namespace Filter
	{
		class CNoneEffectTextureEffect : public CBitmapFusionFilter
		{
		public:
			CNoneEffectTextureEffect() {};
			~CNoneEffectTextureEffect() {};

			int GetTypeFilter()
			{
				return IDM_AFTEREFFECT_NONE;
			}

			virtual void SetTransitionBitmap(const bool& start, IBitmapDisplay* bmpViewer, CImageLoadingFormat* bmpSecond)
			{
				bmpViewer->StopTransitionEffect(bmpSecond);
			}
		};
	}
}