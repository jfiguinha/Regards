
#pragma once
#include "FilterWindowParam.h"

namespace Regards
{
	namespace Filter
    {
        class CBilateralFilter : public CFilterWindowParam
        {
        public:
            CBilateralFilter();
            ~CBilateralFilter();
            int GetTypeFilter();
            int TypeApplyFilter();
			void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface){};
            void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface);
            void FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key);
            void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview);
            bool NeedPreview();
            CEffectParameter* GetEffectPointer();
            CEffectParameter* GetDefaultEffectParameter();

        private:
			wxString libelleEffectsigmaX;
			wxString libelleEffectsigmaP;
            wxString libelleEffectSize;
        };
    }
}

