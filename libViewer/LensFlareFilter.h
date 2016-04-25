//
//  CLensFlareFilter.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#pragma once
#include "FilterWindowParam.h"

namespace Regards
{
    namespace Viewer
    {
        class CLensFlareFilter : public CFilterWindowParam
        {
        public:
            CLensFlareFilter();
            ~CLensFlareFilter();
            int GetTypeFilter();
            void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface);
            void FilterChangeParam(CEffectParameter * effectParameter, const int &valueData, const wxString &key);
            
        private:
            wxString libelleEffectIntensity = L"";
            wxString libelleEffectLightness = L"";
            wxString libelleEffectRadius = L"";
            wxString libelleColor = L"";
        };
    }
}