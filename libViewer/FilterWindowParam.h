//
//  FilterWindowParam.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "FiltreEffectInterface.h"
#include <RegardsBitmap.h>
#include <EffectParameter.h>

namespace Regards
{
    namespace Viewer
    {
        class CFilterWindowParam
        {
        public:
            CFilterWindowParam();
            ~CFilterWindowParam();
            virtual int GetTypeFilter() = 0;
            virtual void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface) = 0;
            virtual void FilterChangeParam(CEffectParameter * effectParameter, const int &valueData, const wxString &key) = 0;
        };
    }
}