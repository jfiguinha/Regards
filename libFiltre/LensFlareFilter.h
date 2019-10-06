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
			void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface){};
            void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface);
            void FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key);
			void ApplyPreviewEffect(CEffectParameter * effectParameter, CFiltreEffet * filtreEffet, CDraw * dessing);
        private:

			void LensFlare(CFiltreEffet * filtreEffet, const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity, const int &posLeft, const int &posTop);

            wxString libelleEffectIntensity;
            wxString libelleEffectLightness;
            wxString libelleEffectRadius;
            wxString libelleColor;
        };
    }
}