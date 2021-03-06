//
//  MotionBlurFilter.hpp
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
        class CMotionBlurFilter : public CFilterWindowParam
        {
        public:
            CMotionBlurFilter();
            ~CMotionBlurFilter();
            int GetNameFilter();
            wxString GetFilterLabel();
            int GetTypeFilter();
            int TypeApplyFilter();
            bool IsOpenCLCompatible();
			void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface){};
            void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface);
            void FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key);
			void ApplyOpenGLShader(CRenderOpenGL * renderOpenGL, CEffectParameter * effectParameter, const int &textureID);
            void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview);
            bool NeedPreview();
            CEffectParameter* GetEffectPointer();
            CEffectParameter* GetDefaultEffectParameter();
            bool IsOpenGLCompatible();
        private:
            wxString libelleEffectRadius;
            wxString libelleEffectSigma;
            wxString libelleEffectAngle;
        };
    }
}
