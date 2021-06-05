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
		private:
			wxString libelleEffectRadius;
			wxString libelleEffectBoxSize;
		};
	}
}

