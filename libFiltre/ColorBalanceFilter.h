//
//  CColorBalanceFilter.hpp
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
        class CColorBalanceFilter : public CFilterWindowParam
        {
        public:
            CColorBalanceFilter();
            ~CColorBalanceFilter();
            int GetTypeFilter();
			void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface){};
            void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface);
            void FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key);
			void ApplyOpenGLShader(CRenderOpenGL * renderOpenGL, CEffectParameter * effectParameter, const int &textureID, const wxRect &screen);
			void DisableOpenGLShader();

        private:
            wxString libelleEffectColorRed;
            wxString libelleEffectColorGreen;
            wxString libelleEffectColorBlue;
        };
    }
}