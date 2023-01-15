#pragma once
#include "FilterWindowParam.h"
#include <Metadata.h>

namespace Regards::Filter
{
	class CCartoonFilter : public CFilterWindowParam
	{
	public:
		CCartoonFilter();
		~CCartoonFilter() override;
		int GetTypeFilter() override;
		int TypeApplyFilter() override;
		wxString GetFilterLabel() override;
		int GetNameFilter() override;

		void Filter(CEffectParameter* effectParameter, const wxString& filename,
		            IFiltreEffectInterface* filtreInterface) override
		{
		};
		void Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
		            IFiltreEffectInterface* filtreInterface) override;
		void FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
		                       const wxString& key) override;
		void ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
		                        CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput,
		                        int& heightOutput) override;
		CImageLoadingFormat* ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer) override;
		void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
		                  const bool& preview) override;
		bool NeedPreview() override;
		CEffectParameter* GetEffectPointer() override;
		CEffectParameter* GetDefaultEffectParameter() override;
		bool IsSourcePreview() override;
		void ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
		                              CFiltreEffet* filtreEffet, CDraw* dessing) override;

	private:
		void AddMetadataElement(vector<CMetadata>& element, wxString value, int key);
		wxString libelleEffectMode;
	};
}
