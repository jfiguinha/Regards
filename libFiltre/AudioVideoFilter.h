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
			~CAudioVideoFilter() override;
			int GetNameFilter() override;
			int GetTypeFilter() override;
			wxString GetFilterLabel() override;
			void Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
			            IFiltreEffectInterface* filtreInterface) override;
			void Filter(CEffectParameter* effectParameter, const wxString& filename,
			            IFiltreEffectInterface* filtreInterface) override;
			void FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
			                       const wxString& key) override;

		private:
			void AddMetadataElement(vector<CMetadata>& element, wxString value, int key);
			wxString libelleAudio;
			wxString libelleVideo;
			wxString libelleSubtitle;
			wxString enableSubtitle;
		};
	}
}
