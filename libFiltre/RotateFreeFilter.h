//
//  RotateFree.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#pragma once
#include "FilterWindowParam.h"

namespace Regards::Filter
{
	class CRotateFreeFilter : public CFilterWindowParam
	{
	public:
		CRotateFreeFilter();
		~CRotateFreeFilter() override;
		int GetTypeFilter() override;
		wxString GetFilterLabel() override;
		int TypeApplyFilter() override;
		int GetNameFilter() override;

		void Filter(CEffectParameter* effectParameter, const wxString& filename,
		            IFiltreEffectInterface* filtreInterface) override
		{
		};
		void Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
		            IFiltreEffectInterface* filtreInterface) override;
		void FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
		                       const wxString& key) override;
		void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
		                  const bool& preview) override;
		bool NeedPreview() override;
		bool NeedToShrink() override;
		CEffectParameter* GetEffectPointer() override;
		CEffectParameter* GetDefaultEffectParameter() override;
		bool IsSourcePreview() override;
		void ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
		                              CFiltreEffet* filtreEffet, CDraw* dessing) override;
		void ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
		                        CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput,
		                        int& heightOutput) override;
		CImageLoadingFormat* ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer) override;
		void CancelPreview(IBitmapDisplay* bitmapViewer) override;

	private:
		wxString libelleRotationAngle;
	};
}
