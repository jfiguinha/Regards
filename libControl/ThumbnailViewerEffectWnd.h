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
	namespace Control
	{
		class CThumbnailViewerEffectWnd : public CWindowMain
		{
		public:
			CThumbnailViewerEffectWnd(wxWindow* parent, wxWindowID idCTreeWithScrollbarInterface,
			                          const CThemeScrollBar& themeScroll, const CThemeThumbnail& themeThumbnail,
			                          int panelInfosId, bool checkValidity);
			~CThumbnailViewerEffectWnd(void) override;

			void UpdateScreenRatio() override;
			void Resize() override;
			wxString GetFilename();
			void SetFile(const wxString& filename);

		private:
			CScrollbarWnd* thumbnailEffectScroll;
			CThumbnailViewerEffect* thumbnailEffect;
		};
	}
}
