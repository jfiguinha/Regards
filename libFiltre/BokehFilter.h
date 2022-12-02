
#pragma once
#include "FilterWindowParam.h"

namespace Regards
{
    namespace Filter
    {
        class CBokehFilter : public CFilterWindowParam
        {
        public:
            CBokehFilter();
            ~CBokehFilter();
            int GetNameFilter();
            int GetTypeFilter();
            int TypeApplyFilter();

            wxString GetFilterLabel();
            void Filter(CEffectParameter* effectParameter, const wxString& filename, IFiltreEffectInterface* filtreInterface) {};
            void Filter(CEffectParameter* effectParameter, cv::Mat & source, const wxString& filename, IFiltreEffectInterface* filtreInterface);
            void FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData, const wxString& key);
            void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview);
            bool NeedPreview();
            CEffectParameter* GetEffectPointer();
            CEffectParameter* GetDefaultEffectParameter();
            void ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput);
            CImageLoadingFormat* ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer);
            bool IsSourcePreview();
            void ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* dessing);
        
        private:
            wxString libelleEffectRadius;
            wxString libelleEffectBoxSize;
            wxRect faceRect;
            int nbFace = 0;
        };
    }
}

