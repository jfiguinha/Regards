
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
            void Filter(CEffectParameter* effectParameter, CRegardsBitmap* source, IFiltreEffectInterface* filtreInterface);
            void FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData, const wxString& key);
            void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview);
            bool NeedPreview();
            CEffectParameter* GetEffectPointer();
            CEffectParameter* GetDefaultEffectParameter();

        private:
            wxString libelleEffectBlur;
            wxString libelleEffectBokehThreshold;
            wxString libelleEffectBokehThreshold2;
            wxString libelleEffectBokehDilatation;
            wxString libelleEffectBokehDilatation2;
        };
    }
}

