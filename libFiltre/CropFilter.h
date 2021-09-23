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
	namespace Filter
	{
		class CCropFilter : public CFilterWindowParam
		{
		public:
			CCropFilter();
			~CCropFilter();
			CDraw* GetDrawingPt();
			wxString GetFilterLabel();
			int GetNameFilter();
			bool IsOpenCLCompatible();
			int GetTypeFilter();
			bool SupportMouseSelection();
			void SetCursor();
			int TypeApplyFilter();
			void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface) {};
			void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface);
			void FilterChangeParam(CEffectParameter * effectParameter, CTreeElementValue * valueData, const wxString &key);
			CImageLoadingFormat * ApplyEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer);
			CImageLoadingFormat * ApplyMouseMoveEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CDraw * dessing);
			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview);
			bool NeedPreview();
			bool SupportMouseClick();
			bool NeedToUpdateSource();
		
		private:

			void Drawing(wxMemoryDC * dc, IBitmapDisplay * bitmapViewer, CDraw * m_cDessin);
			void ApplyPreviewEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * m_cDessin, int & widthOutput, int & heightOutput);

			wxString libelleEffectSize;
		};
	}
}

