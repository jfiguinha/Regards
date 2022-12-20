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
	namespace Filter
    {
        class CCloudsFilter : public CFilterWindowParam
        {
        public:
            CCloudsFilter();
            ~CCloudsFilter();
            wxString GetFilterLabel();
            int GetTypeFilter();
            bool IsOpenCLCompatible();
            int TypeApplyFilter();
            int GetNameFilter();
			void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface){};
            void Filter(CEffectParameter * effectParameter, cv::Mat & source, const wxString& filename, IFiltreEffectInterface * filtreInterface);
            void FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key);
            void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview);
            bool NeedPreview();
            CEffectParameter* GetEffectPointer();
            CEffectParameter* GetDefaultEffectParameter();
            bool IsSourcePreview();
            void ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* dessing);
            void ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput);
            CImageLoadingFormat* ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer);
        private:
            wxString libelleCloudsFrequency;
            wxString libelleCloudsAmplitude;
			wxString libelleEffectIntensity;
            wxString libelleCloudsColorFrontRed;
            wxString libelleCloudsColorFrontGreen;
            wxString libelleCloudsColorFrontBlue;
            wxString libelleCloudsColorBackRed;
            wxString libelleCloudsColorBackGreen;
            wxString libelleCloudsColorBackBlue;
        };
    }
}