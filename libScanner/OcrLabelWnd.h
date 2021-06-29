//
//  InfoEffectWnd.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 02/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#include "OcrLabel.h"
#include <TreeWithScrollbar.h>
using namespace Regards::Window;

namespace Regards
{
    namespace Scanner
    {
        class COcrLabelWnd : public CTreeWithScrollbar
        {
        public:
			COcrLabelWnd(wxWindow* parent, wxWindowID id, const CThemeScrollBar & themeScroll, const CThemeTree & themeTree, int idWindow);
            ~COcrLabelWnd(void);
			void Init();
            void Update(vector<ChOcrElement *> &labelList);
            
        private:
			int idWindow;
            COcrLabel * ocrLabelOld;
        };
    }
}
