//
//  FiltreEffectWnd.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 02/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#include "FiltreEffect.h"
#include <ScrollbarWnd.h>
#include <TreeWindow.h>
using namespace Regards::Window;


namespace Regards
{
    namespace Viewer
    {
        class CInfoEffectWnd;
        class CPanelInfosWnd;
        
        class CFiltreEffectScrollWnd : public CWindowMain
        {
        public:
            CFiltreEffectScrollWnd(wxWindow* parent, wxWindowID id, const CThemeScrollBar & themeScroll, const CThemeTree & themeTree);
            ~CFiltreEffectScrollWnd(void);
            
            void UpdateScreenRatio();
            void Resize();
            void ApplyEffect(const int &numItem, CInfoEffectWnd * historyEffectWnd, CPanelInfosWnd * panelInfos, const wxString &filename, const int & isVideo);
            void OnFiltreOk(const int &numFiltre, CInfoEffectWnd * historyEffectWnd);
            CFiltreEffect * GetFiltreEffect();
			int GetNumFiltre();
        private:
            
			CRegardsBitmap * bitmap;
            CEffectParameter * effectParameter;
            CScrollbarWnd * filtreEffectScroll;
            CTreeWindow * treeFiltreEffect;
            CFiltreEffect * filtreEffectOld;
			int numFiltre;
        };
    }
}

