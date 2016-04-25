//
//  BrightAndContrastFilter.hpp
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
        class CBrightAndContrastFilter : public CFilterWindowParam
        {
        public:
            CBrightAndContrastFilter();
            ~CBrightAndContrastFilter();
            int GetTypeFilter();
            void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface);
            void FilterChangeParam(CEffectParameter * effectParameter, const int &valueData, const wxString &key);
            
        private:
            wxString libelleEffectContrast = L"";
            wxString libelleEffectLightness = L"";
        };
    }
}

