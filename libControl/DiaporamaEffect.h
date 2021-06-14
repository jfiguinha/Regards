#pragma once
#include "BitmapFusionFilter.h"
#include "BitmapDisplay.h"
#include <FilterData.h>
namespace Regards
{
	namespace Filter
	{
		class CDiaporamaEffect : public CBitmapFusionFilter
		{
		public:
			CDiaporamaEffect() {};
			~CDiaporamaEffect() {};

			int GetTypeFilter()
			{
				return IDM_DIAPORAMA_TRANSITION;
			}

			virtual void SetTransitionBitmap(const bool& start, IBitmapDisplay* bmpViewer, CImageLoadingFormat* bmpSecond)
			{
				bmpViewer->StartTransitionEffect(bmpSecond, true);
			}

			virtual void AfterRender(CImageLoadingFormat* nextPicture, CRenderBitmapOpenGL* renderOpenGL, IBitmapDisplay* bmpViewer, const int& etape, const float& scale_factor, const bool& isNext, float& ratio)
			{
				if (etape < 110)
				{
					ratio = ratio + 0.0005;
					bmpViewer->CalculCenterPositionPicture();
				}
			}
		};
	}
}