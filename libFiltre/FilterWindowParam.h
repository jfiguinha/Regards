//
//  FilterWindowParam.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#pragma once
#include <FiltreEffectInterface.h>
#include <TreeElementValue.h>
#include <EffectParameter.h>

class CRegardsBitmap;

class CFilterWindowParam
{
public:
    CFilterWindowParam();
    virtual ~CFilterWindowParam();
    virtual int GetTypeFilter() = 0;
    virtual void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface) = 0;
    virtual void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface) = 0;
    virtual void FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key) = 0;
};
