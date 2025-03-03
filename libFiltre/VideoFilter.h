//
//  SwirlFilter.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#pragma once
#include "FilterWindowParam.h"

class CVideoEffectParameter;
class CMetadata;

namespace Regards::Filter
{
	class CVideoFilter : public CFilterWindowParam
	{
	public:
		CVideoFilter();
		~CVideoFilter() override;
		int GetNameFilter() override;
		wxString GetFilterLabel() override;
		int GetTypeFilter() override;
		void Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
		            IFiltreEffectInterface* filtreInterface) override;
		void Filter(CEffectParameter* effectParameter, const wxString& filename,
		            IFiltreEffectInterface* filtreInterface) override;
		void FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
		                       const wxString& key) override;

	private:
		void AddMetadataElement(vector<CMetadata>& element, wxString value, int key);
		wxString enableEffect;
		wxString enableDenoiseEffect;
		wxString enableSharpenEffect;
		wxString enableColorEffect;
		wxString enableGrayScale;
		wxString enableBandCEffect;
		wxString effectMedianLevel;
		wxString effectSharpenLevel;
		wxString toneConstrastLevel;
		wxString toneBrightnessLevel;
		wxString effectDenoisingSigmaU;
		wxString effectDenoisingSigmaK;
		wxString effectDenoisingThreshold;
		wxString enableOpenglDenoising;
		wxString effectDenoising;
		wxString colorBoostRed;
		wxString colorBoostGreen;
		wxString colorBoostBlue;
		wxString enableVHSEffect;
		wxString showFPS;
		wxString libelleAudio;
		wxString libelleVideo;
		wxString libelleSubtitle;
		wxString enableSubtitle;
		wxString enableBicubicInterpolation;
		wxString enableOpenCL;
		wxString threshold;
		wxString enableFilmgrain;
		wxString enableColorisation;
		wxString enableRestore;
		wxString enableSepia;
		wxString libelleScale;
		wxString libelleZoom;
		wxString libelleAutoContrast;
		wxString libelleStabilize;
		wxString libelleStabilizeNbFrame;
		wxString libelleTemplateWindowSize;
		wxString libellesearchWindowSize;

	};
}
