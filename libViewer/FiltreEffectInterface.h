//
//  FiltreEffectInterface.h
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#pragma once
#include <vector>
using namespace std;

namespace Regards
{
    namespace Viewer
    {
        class IFiltreEffectInterface
        {
        public:
            virtual void AddTreeInfos(const wxString &exifKey, const int &position, const vector<int> & value) = 0;
        };
    }
}