
#pragma once
#include "FilterWindowParam.h"

namespace Regards
{
	namespace Filter
    {
        class CBm3dWindowFilter : public CFilterWindowParam
        {
        public:
            CBm3dWindowFilter();
            ~CBm3dWindowFilter();
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
            wxString libelleEffectSigma;
        };
    }
}

