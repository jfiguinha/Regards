//
//  FiltreEffectWnd.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 02/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#pragma once
#include "FiltreEffect.h"
#include <TreeWithScrollbar.h>
using namespace Regards::Window;
class CImageLoadingFormat;

namespace Regards::Control
{
	class CInfoEffectWnd;

	class CFiltreEffectScrollWnd : public CTreeWithScrollbar
	{
	public:
		CFiltreEffectScrollWnd(wxWindow* parent, wxWindowID id, const CThemeScrollBar& themeScroll,
		                       const CThemeTree& themeTree, int bitmapWindowId);
		~CFiltreEffectScrollWnd(void);

		void ApplyEffect(const int& numItem, CInfoEffectWnd* historyEffectWnd, const wxString& filename,
		                 const int& isVideo, int panelId, int previewId);
		void OnFiltreOk(const int& numFiltre, CInfoEffectWnd* historyEffectWnd);
		void OnFiltreCancel();
		CFiltreEffect* GetFiltreEffect();
		int GetNumFiltre();

	private:
		void OnUpdateFilter(wxCommandEvent& event);
		void SetBitmapToViewer(CImageLoadingFormat* bitmap);
		//CImageLoadingFormat * SetBitmapEffect(const int &effect, CEffectParameter * effectParameter, CRegardsBitmap * bitmap);
		CImageLoadingFormat * bitmap;
		std::unique_ptr<CEffectParameter> effectParameter;
		std::unique_ptr<CFiltreEffect> filtreEffectOld;
		int numFiltre;
		int bitmapWindowId;
	};
}
