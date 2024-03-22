#pragma once
#include "CropFilter.h"
//
//  BrightAndContrastFilter.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "FilterWindowParam.h"

namespace Regards::Filter
{
	class CInpaintFilter : public CCropFilter
	{
	public:
		CInpaintFilter();
		~CInpaintFilter() override;

        int GetTypeFilter() override;
        int GetNameFilter() override;
        wxString GetFilterLabel() override;
        CImageLoadingFormat* ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer) override;
	private:
        cv::Mat GenerateMaskFromZone(const cv::Rect & zone, const cv::Mat & src);
	};
}
