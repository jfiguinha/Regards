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
	namespace Filter
    {
        class CLensFlareFilter : public CFilterWindowParam
        {
        public:

            int TypeApplyFilter();
            CLensFlareFilter();
            ~CLensFlareFilter();
            int GetTypeFilter();
            bool SupportMouseClick();
            bool IsOpenCLCompatible();
			void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface){};
            void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface);
            void FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key);
			void ApplyPreviewEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * dessing, int & widthOutput, int & heightOutput);
            void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview);
            bool NeedPreview();
            CEffectParameter* GetEffectPointer();
            CEffectParameter* GetDefaultEffectParameter();

        private:

			CImageLoadingFormat * ApplyEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer);
			void LensFlare(CFiltreEffet * filtreEffet, const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity, const int &posLeft, const int &posTop);
			void Drawing(wxMemoryDC * dc, IBitmapDisplay * bitmapViewer, CDraw * m_cDessin);

            wxString libelleEffectIntensity;
            wxString libelleEffectLightness;
            wxString libelleEffectRadius;
            wxString libelleColor;
        };
    }
}