#pragma once
//
//  BrightAndContrastFilter.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "FilterWindowParam.h"

namespace Regards
{
	namespace Viewer
	{
		class CRedEyeFilter : public CFilterWindowParam
		{
		public:
			CRedEyeFilter();
			~CRedEyeFilter();
			int GetTypeFilter();
			void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface) {};
			void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface);
			void FilterChangeParam(CEffectParameter * effectParameter, CTreeElementValue * valueData, const wxString &key);
			CImageLoadingFormat * ApplyEffect(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer);
			CImageLoadingFormat * ApplyMouseMoveEffect(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer, CDraw * dessing);

		private:

			void Drawing(wxMemoryDC * dc, Regards::Control::CBitmapWndViewer * bitmapViewer, CDraw * m_cDessin);
			void ApplyPreviewEffect(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * m_cDessin, int & widthOutput, int & heightOutput);

		};
	}
}

