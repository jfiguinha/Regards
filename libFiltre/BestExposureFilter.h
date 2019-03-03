#pragma once
#include "FilterWindowParam.h"

namespace Regards
{
    namespace Viewer
    {
        class CBestExposureFilter: public CFilterWindowParam
        {
        public:
            CBestExposureFilter();
            ~CBestExposureFilter();
            int GetTypeFilter();
			void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface){};
            void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface);
            void FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key);
            
        private:
            wxString libelleEffectTmo;
        };
    }
}

