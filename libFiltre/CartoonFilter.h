#pragma once
#include "FilterWindowParam.h"
#include <Metadata.h>
namespace Regards
{
	namespace Filter
    {
        class CCartoonFilter: public CFilterWindowParam
        {
        public:
			CCartoonFilter();
            ~CCartoonFilter();
            int GetTypeFilter();
			void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface){};
            void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface);
            void FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key);
			void ApplyPreviewEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * m_cDessin, int & widthOutput, int & heightOutput);
			CImageLoadingFormat * ApplyEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer);
        private:

			void AddMetadataElement(vector<CMetadata> & element, wxString value, int key);
            wxString libelleEffectMode;
        };
    }
}
