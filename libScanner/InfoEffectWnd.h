//
//  InfoEffectWnd.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 02/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#include "InfoEffect.h"
#include <TreeWithScrollbar.h>
using namespace Regards::Window;

namespace Regards
{
    namespace Scanner
    {
        class CInfoEffectWnd : public CTreeWithScrollbar
        {
        public:
            CInfoEffectWnd(wxWindow* parent, wxWindowID id, const CThemeScrollBar & themeScroll, const CThemeTree & themeTree);
            ~CInfoEffectWnd(void);

            void AddModification(CRegardsBitmap * bitmap,const wxString & libelle);
            void HistoryUpdate(CRegardsBitmap * bitmap,const wxString & filename, const wxString & historyLibelle, CModificationManager * modificationManager);
            
        private:
          
            CInfoEffect * historyEffectOld;
        };
    }
}
