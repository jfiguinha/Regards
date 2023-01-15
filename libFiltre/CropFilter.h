#pragma once
//
//  BrightAndContrastFilter.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "FilterWindowParam.h"

namespace Regards
{
	namespace Filter
	{
		class CCropFilter : public CFilterWindowParam
		{
		public:
			CCropFilter();
			~CCropFilter() override;
			CDraw* GetDrawingPt() override;
			wxString GetFilterLabel() override;
			int GetNameFilter() override;
			bool IsOpenCLCompatible() override;
			int GetTypeFilter() override;
			bool SupportMouseSelection() override;
			void SetCursor() override;
			int TypeApplyFilter() override;

			void Filter(CEffectParameter* effectParameter, const wxString& filename,
			            IFiltreEffectInterface* filtreInterface) override
			{
			};
			void Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
			            IFiltreEffectInterface* filtreInterface) override;
			void FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
			                       const wxString& key) override;
			CImageLoadingFormat* ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer) override;
			CImageLoadingFormat* ApplyMouseMoveEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
			                                          CDraw* dessing) override;
			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override;
			bool NeedPreview() override;
			bool SupportMouseClick() override;
			bool NeedToUpdateSource() override;

		private:
			void Drawing(wxMemoryDC* dc, IBitmapDisplay* bitmapViewer, CDraw* m_cDessin) override;
			void ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
			                        CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput,
			                        int& heightOutput) override;

			wxString libelleEffectSize;
		};
	}
}
