
#pragma once
#include "FilterWindowParam.h"

namespace Regards
{
    namespace Viewer
    {
        class CBilateralFilter : public CFilterWindowParam
        {
        public:
            CBilateralFilter();
            ~CBilateralFilter();
            int GetTypeFilter();
			void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface){};
            void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface);
            void FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key);
            
        private:
			wxString libelleEffectsigmaX;
			wxString libelleEffectsigmaP;
            wxString libelleEffectSize;
        };
    }
}

