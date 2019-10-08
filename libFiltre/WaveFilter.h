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
        class CWaveFilter : public CFilterWindowParam
        {
        public:
			CWaveFilter();
            ~CWaveFilter();
            int GetTypeFilter();
			void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface){};
            void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface);
            void FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key);
			CImageLoadingFormat * ApplyEffect(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer);
        private:

			void ApplyPreviewEffect(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * m_cDessin, int & widthOutput, int & heightOutput);
			void Drawing(wxMemoryDC * dc, Regards::Control::CBitmapWndViewer * bitmapViewer, CDraw * m_cDessin);

            wxString libelleEffectScale;
            wxString libelleEffectHeight;
            wxString libelleEffectRadius;
        };
    }
}