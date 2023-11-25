#pragma once
#include "BitmapFusionFilter.h"
#include "BitmapDisplay.h"
#include <effect_id.h>

namespace Regards::Filter
{
	class CDiaporamaEffect : public CBitmapFusionFilter
	{
	public:
		CDiaporamaEffect()
		{
		};

		~CDiaporamaEffect() override
		{
		};

		int GetTypeFilter() override
		{
			return IDM_DIAPORAMA_TRANSITION;
		}

		bool NeedToReload() override
		{
			return false;
		}

		void SetTransitionBitmap(const bool& start, IBitmapDisplay* bmpViewer,
		                         CImageLoadingFormat* bmpSecond) override
		{
			bmpViewer->StartTransitionEffect(bmpSecond, true);
		}

		void AfterRender(CImageLoadingFormat* nextPicture, CRenderBitmapOpenGL* renderOpenGL,
		                 IBitmapDisplay* bmpViewer, const int& etape, const float& scale_factor, const bool& isNext,
		                 float& ratio) override
		{
			if (etape < 110)
			{
				ratio = ratio + 0.0005;
				bmpViewer->CalculCenterPositionPicture();
			}
		}
	};
}
