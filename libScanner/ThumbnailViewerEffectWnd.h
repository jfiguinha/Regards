//
//  ThumbnailViewerEffectWnd.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 02/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#pragma once
#include "ThumbnailViewerEffect.h"
#include <ScrollbarWnd.h>
using namespace Regards::Control;

namespace Regards
{
    namespace Scanner
    {      
        class CThumbnailViewerEffectWnd : public CWindowMain
        {
        public:
            CThumbnailViewerEffectWnd(wxWindow* parent, wxWindowID idCTreeWithScrollbarInterface, const CThemeScrollBar & themeScroll, const CThemeThumbnail & themeThumbnail);
            virtual ~CThumbnailViewerEffectWnd(void);
            
            void UpdateScreenRatio();
            void Resize();
            wxString GetFilename();
            void SetFile(const wxString & filename);
        private:
            
            CScrollbarWnd * thumbnailEffectScroll;
            CThumbnailViewerEffect * thumbnailEffect;
        };
    }
}

