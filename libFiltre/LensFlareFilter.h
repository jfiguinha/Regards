//
//  CLensFlareFilter.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#pragma once
#include "FilterWindowParam.h"

namespace Regards::Filter
{
	class CLensFlareFilter : public CFilterWindowParam
	{
	public:
		int TypeApplyFilter() override;
		CLensFlareFilter();
		~CLensFlareFilter() override;
		int GetNameFilter() override;
		int GetTypeFilter() override;
		wxString GetFilterLabel() override;
		bool SupportMouseClick() override;
		CDraw* GetDrawingPt() override;
		bool IsOpenCLCompatible() override;

		void Filter(CEffectParameter* effectParameter, const wxString& filename,
		            IFiltreEffectInterface* filtreInterface) override
		{
		};
		void Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
		            IFiltreEffectInterface* filtreInterface) override;
		void FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
		                       const wxString& key) override;
		void ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
		                        CFiltreEffet* filtreEffet, CDraw* dessing, int& widthOutput,
		                        int& heightOutput) override;
		void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
		                  const bool& preview) override;
		bool NeedPreview() override;
		CEffectParameter* GetEffectPointer() override;
		CEffectParameter* GetDefaultEffectParameter() override;
		bool IsSourcePreview() override;
		CImageLoadingFormat* ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer) override;
		void ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
		                              CFiltreEffet* filtreEffet, CDraw* dessing) override;

	private:
		//void LensFlare(CFiltreEffet * filtreEffet, const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity, const int &posLeft, const int &posTop);
		void Drawing(wxMemoryDC* dc, IBitmapDisplay* bitmapViewer, CDraw* m_cDessin) override;

		wxString libelleEffectIntensity;
		wxString libelleEffectLightness;
		wxString libelleEffectRadius;
		wxString libelleColor;
	};
}
