//
//  FiltreEffectWnd.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 02/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#include "FiltreEffect.h"
#include <TreeWithScrollbar.h>
using namespace Regards::Window;

class CImageLoadingFormat;

namespace Regards
{
    namespace Scanner
    {
        class CInfoEffectWnd;
        class CPanelInfosWnd;
        
        class CFiltreEffectScrollWnd : public CTreeWithScrollbar
        {
        public:
            CFiltreEffectScrollWnd(wxWindow* parent, wxWindowID id, const CThemeScrollBar & themeScroll, const CThemeTree & themeTree);
            ~CFiltreEffectScrollWnd(void);
            void ApplyEffect(const int &numItem, CInfoEffectWnd * historyEffectWnd, CPanelInfosWnd * panelInfos, const wxString &filename, const int & isVideo);
            void OnFiltreOk(const int &numFiltre, CInfoEffectWnd * historyEffectWnd);
			void OnFiltreCancel();
            CFiltreEffect * GetFiltreEffect();
			int GetNumFiltre();

        private:

			void OnUpdateFilter(wxCommandEvent& event);

			//CImageLoadingFormat * SetBitmapEffect(const int &effect, CEffectParameter * effectParameter, CRegardsBitmap * bitmap);
			CRegardsBitmap * bitmap;
            CEffectParameter * effectParameter;
            CFiltreEffect * filtreEffectOld;
			int numFiltre;
        };
    }
}

