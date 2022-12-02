//
//  InfoEffectWnd.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 02/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#pragma once
#include "InfoEffect.h"
#include <TreeWithScrollbar.h>
using namespace Regards::Window;

class CImageLoadingFormat;

namespace Regards
{
	namespace Control
	{
		class CInfoEffectWnd : public CTreeWithScrollbar
		{
		public:
			CInfoEffectWnd(wxWindow* parent, wxWindowID id, const CThemeScrollBar& themeScroll,
			               const CThemeTree& themeTree, int bitmap_window_id);
			~CInfoEffectWnd(void) override;

			void AddModification(CImageLoadingFormat* bitmap, const wxString& libelle);
			void HistoryUpdate(CImageLoadingFormat* bitmap, const wxString& filename, const wxString& historyLibelle,
			                   CModificationManager* modificationManager);

		private:
			CInfoEffect* historyEffectOld;
			int bitmapWindowId;
			//const wxWindowID id_;
			//const CThemeScrollBar& theme_scroll_;
		};
	}
}
