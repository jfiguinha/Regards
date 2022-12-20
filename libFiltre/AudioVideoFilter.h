#pragma once
#include "FilterWindowParam.h"

class CVideoEffectParameter;
class CMetadata;

namespace Regards
{
	namespace Filter
    {
        class CAudioVideoFilter : public CFilterWindowParam
        {
        public:
            CAudioVideoFilter();
            ~CAudioVideoFilter();
			int GetNameFilter();
            int GetTypeFilter();
			wxString GetFilterLabel();
			void Filter(CEffectParameter * effectParameter, cv::Mat & source, const wxString& filename, IFiltreEffectInterface * filtreInterface);
			void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface);
            void FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key);

        private:
			void AddMetadataElement(vector<CMetadata> & element, wxString value, int key);
			wxString libelleAudio;
			wxString libelleVideo;
			wxString libelleSubtitle;
			wxString enableSubtitle;

        };

	}
}