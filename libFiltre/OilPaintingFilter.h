#pragma once
#include "FilterWindowParam.h"

namespace Regards
{
	namespace Filter
	{
		class COilPaintingFilter : public CFilterWindowParam
		{
		public:
			COilPaintingFilter();
			~COilPaintingFilter() override;
			int GetTypeFilter() override;
			int GetNameFilter() override;
			wxString GetFilterLabel() override;

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
			int TypeApplyFilter() override;
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
			wxString libelleSize;
			wxString libelleDynRatio;
		};
	}
}
