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
			~CGaussianBlurFilter();
			int GetNameFilter();
			int GetTypeFilter();
			wxString GetFilterLabel();
			int TypeApplyFilter();
			void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface){};
			void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface);
			void FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key);
			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview);
			bool NeedPreview();
			CEffectParameter* GetEffectPointer();
			CEffectParameter* GetDefaultEffectParameter();
			void ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput);
			CImageLoadingFormat* ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer);
		
		private:
			wxString libelleEffectRadius;
			wxString libelleEffectBoxSize;
		};
	}
}

