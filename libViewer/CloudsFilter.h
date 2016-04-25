//
//  CloudsFilter.hpp
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
        class CCloudsFilter : public CFilterWindowParam
        {
        public:
            CCloudsFilter();
            ~CCloudsFilter();
            int GetTypeFilter();
            void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface);
            void FilterChangeParam(CEffectParameter * effectParameter, const int &valueData, const wxString &key);
            
        private:
            wxString libelleCloudsFrequency = L"";
            wxString libelleCloudsAmplitude = L"";
            wxString libelleCloudsColorFrontRed = L"";
            wxString libelleCloudsColorFrontGreen = L"";
            wxString libelleCloudsColorFrontBlue = L"";
            wxString libelleCloudsColorBackRed = L"";
            wxString libelleCloudsColorBackGreen = L"";
            wxString libelleCloudsColorBackBlue = L"";
        };
    }
}