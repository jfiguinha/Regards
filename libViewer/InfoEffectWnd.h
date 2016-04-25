//
//  InfoEffectWnd.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 02/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#include "InfoEffect.h"
#include <ScrollbarWnd.h>
using namespace Regards::Control;

namespace Regards
{
    namespace Viewer
    {
        class CInfoEffectWnd : public CWindowMain
        {
        public:
            CInfoEffectWnd(wxWindow* parent, wxWindowID id, const CThemeScrollBar & themeScroll, const CThemeTree & themeTree);
            ~CInfoEffectWnd(void);
            
            void UpdateScreenRatio();
            void Resize();
            void AddModification(CRegardsBitmap * bitmap,const wxString & libelle);
            void HistoryUpdate(CRegardsBitmap * bitmap,const wxString & filename, const wxString & historyLibelle, CBitmapWndViewer * bitmapViewer, CModificationManager * modificationManager);
            
        private:
            
            CScrollbarWnd * historyEffectScroll = nullptr;
            CTreeWindow * treeHistoryEffect = nullptr;
            CInfoEffect * historyEffectOld = nullptr;
        };
    }
}
