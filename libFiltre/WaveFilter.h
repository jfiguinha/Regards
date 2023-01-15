//
//  CLensFlareFilter.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#pragma once
#include "FilterWindowParam.h"

namespace Regards
{
	namespace Filter
	{
		class CWaveFilter : public CFilterWindowParam
		{
		public:
			int TypeApplyFilter() override;
			CWaveFilter();
			~CWaveFilter() override;
			int GetNameFilter() override;
			wxString GetFilterLabel() override;
			int GetTypeFilter() override;
			bool SupportMouseClick() override;
			bool IsOpenCLCompatible() override;
			CDraw* GetDrawingPt() override;

			void Filter(CEffectParameter* effectParameter, const wxString& filename,
			            IFiltreEffectInterface* filtreInterface) override
			{
			};
			void Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
			            IFiltreEffectInterface* filtreInterface) override;
			void FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
			                       const wxString& key) override;
			CImageLoadingFormat* ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer) override;
			bool NeedPreview() override;
			CEffectParameter* GetEffectPointer() override;
			CEffectParameter* GetDefaultEffectParameter() override;
			bool IsSourcePreview() override;
			void ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
			                              CFiltreEffet* filtreEffet, CDraw* dessing) override;
			void ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
			                        CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput,
			                        int& heightOutput) override;

		private:
			void Drawing(wxMemoryDC* dc, IBitmapDisplay* bitmapViewer, CDraw* m_cDessin) override;

			wxString libelleEffectScale;
			wxString libelleEffectHeight;
			wxString libelleEffectRadius;
		};
	}
}
