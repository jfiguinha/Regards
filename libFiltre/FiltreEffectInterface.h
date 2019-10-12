//
//  FiltreEffectInterface.h
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#pragma once
#include <TreeElementValue.h>

class IFiltreEffectInterface
{
public:
    virtual ~IFiltreEffectInterface() = 0;
    virtual void AddTreeInfos(const wxString &exifKey, CTreeElementValue * position, void * value, const int & typeValue = 1, const int &type = 1) = 0;
};
