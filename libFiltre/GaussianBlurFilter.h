#pragma once
#include "FilterWindowParam.h"

namespace Regards
{
	namespace Filter
	{
		class CGaussianBlurFilter : public CFilterWindowParam
		{
		public:
			CGaussianBlurFilter();
			~CGaussianBlurFilter() override;
			int GetNameFilter() override;
			int GetTypeFilter() override;
			wxString GetFilterLabel() override;
			int TypeApplyFilter() override;

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
			CEffectParameter* GetEffectPointer() override;
			CEffectParameter* GetDefaultEffectParameter() override;
			bool IsSourcePreview() override;
			void ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
			                              CFiltreEffet* filtreEffet, CDraw* dessing) override;
			void ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
			                        CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput,
			                        int& heightOutput) override;
			CImageLoadingFormat* ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer) override;

		private:
			wxString libelleEffectRadius;
			wxString libelleEffectBoxSize;
		};
	}
}
