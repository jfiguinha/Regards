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
    namespace Viewer
    {      
        class CThumbnailViewerEffectWnd : public CWindowMain
        {
        public:
            CThumbnailViewerEffectWnd(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeScrollBar & themeScroll, const CThemeThumbnail & themeThumbnail);
            ~CThumbnailViewerEffectWnd(void);
            
            void UpdateScreenRatio();
            void Resize();
            wxString GetFilename();
            void SetFile(const wxString & filename, CRegardsBitmap * bitmap);
        private:
            
            CScrollbarWnd * thumbnailEffectScroll = nullptr;
            CThumbnailViewerEffect * thumbnailEffect = nullptr;
        };
    }
}

