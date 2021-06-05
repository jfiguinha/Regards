#pragma once
//
//  SwirlFilter.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#pragma once
#include "FilterWindowParam.h"

namespace Regards
{
    namespace Filter
    {
        class CVignetteFilter : public CFilterWindowParam
        {
        public:
            CVignetteFilter();
            ~CVignetteFilter();
            int TypeApplyFilter();
            int GetNameFilter();
            int GetTypeFilter();
            wxString GetFilterLabel();
            void Filter(CEffectParameter* effectParameter, const wxString& filename, IFiltreEffectInterface* filtreInterface) {};
            void Filter(CEffectParameter* effectParameter, CRegardsBitmap* source, IFiltreEffectInterface* filtreInterface);
            void FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData, const wxString& key);
            void ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput);
            CImageLoadingFormat* ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer);
            void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview);
            bool NeedPreview();
            CEffectParameter* GetEffectPointer();
            CEffectParameter* GetDefaultEffectParameter();

        private:
            wxString libelleEffectRadius;
            wxString libelleEffectPower;
            CRegardsBitmap* source;
        };
    }
}