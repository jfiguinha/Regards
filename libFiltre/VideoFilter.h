//
//  SwirlFilter.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#pragma once
#include "FilterWindowParam.h"
#include <StreamInfo.h>

class CVideoEffectParameter;
class CMetadata;

namespace Regards
{
	namespace Filter
    {
        class CVideoFilter : public CFilterWindowParam
        {
        public:
            CVideoFilter();
            ~CVideoFilter();
            int GetTypeFilter();
			void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface);
			void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface);
            void FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key);


        private:
			void AddMetadataElement(vector<CMetadata> & element, wxString value, int key);
			wxString enableEffect;
			wxString enableBlurEffect;
			wxString enableSharpenEffect;
			wxString enableColorEffect;
			wxString enableGrayScale;
			wxString enableBandCEffect;
			wxString effectMedianLevel;
			wxString effectSharpenLevel;
			wxString toneConstrastLevel;
			wxString toneBrightnessLevel;
			//wxString effectDenoisingSigmaU;
			//wxString effectDenoisingSigmaK;
			//wxString effectDenoisingThreshold;
			wxString effectDenoising;
			wxString colorBoostRed;
			wxString colorBoostGreen;
			wxString colorBoostBlue;
			wxString showFPS;
			wxString libelleAudio;
			wxString libelleVideo;
			wxString libelleSubtitle;
			wxString enableSubtitle;
            wxString enableBicubicInterpolation;
            wxString enableOpenCL;
			wxString threshold;
			wxString enableFilmgrain;
			wxString libelleScale;
			wxString libelleZoom;
        };
    }
}